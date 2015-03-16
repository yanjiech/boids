/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "base/CCScheduler.h"
#include "base/ccMacros.h"
#include "base/CCDirector.h"
#include "base/utlist.h"
#include "base/ccCArray.h"
#include "base/CCScriptSupport.h"

NS_CC_BEGIN

// data structures

// A list double-linked list used for "updates with priority"
typedef struct _listEntry
{
    struct _listEntry   *prev, *next;
	ccSchedulerFunc     callback;
	void				*hash_key;
    int                 priority;
    bool                paused;
    bool                markedForDeletion; // selector will no longer be called and entry will be removed at end of the next tick
} tListEntry;

typedef struct _hashUpdateEntry
{
    tListEntry          **list;        // Which list does it belong to ?
    tListEntry          *entry;        // entry in the list
	void				*hash_key;
    ccSchedulerFunc     callback;
    UT_hash_handle      hh;
} tHashUpdateEntry;

// Hash Element used for "selectors with interval"
typedef struct _hashSelectorEntry
{
    ccArray             *timers;
	void				*hash_key;
    int                 timerIndex;
    Timer               *currentTimer;
    bool                currentTimerSalvaged;
    bool                paused;
    UT_hash_handle      hh;
} tHashTimerEntry;

// implementation Timer

Timer::Timer()
: _scheduler(nullptr)
, _elapsed(-1)
, _runForever(false)
, _useDelay(false)
, _timesExecuted(0)
, _repeat(0)
, _delay(0.0f)
, _interval(0.0f)
{
}

void Timer::setupTimerWithInterval(float seconds, unsigned int repeat, float delay)
{
	_elapsed = -1;
	_interval = seconds;
	_delay = delay;
	_useDelay = (_delay > 0.0f) ? true : false;
	_repeat = repeat;
	_runForever = (_repeat == CC_REPEAT_FOREVER) ? true : false;
}

void Timer::update(float dt)
{
    if (_elapsed == -1)
    {
        _elapsed = 0;
        _timesExecuted = 0;
    }
    else
    {
        if (_runForever && !_useDelay)
        {//standard timer usage
            _elapsed += dt;
            if (_elapsed >= _interval)
            {
                trigger();

                _elapsed = 0;
            }
        }    
        else
        {//advanced usage
            _elapsed += dt;
            if (_useDelay)
            {
                if( _elapsed >= _delay )
                {
                    trigger();
                    
                    _elapsed = _elapsed - _delay;
                    _timesExecuted += 1;
                    _useDelay = false;
                }
            }
            else
            {
                if (_elapsed >= _interval)
                {
                    trigger();
                    
                    _elapsed = 0;
                    _timesExecuted += 1;

                }
            }

            if (!_runForever && _timesExecuted > _repeat)
            {    //unschedule timer
                cancel();
            }
        }
    }
}


// TimerTargetSelector

TimerTargetSelector::TimerTargetSelector()
: _target(nullptr)
, _selector(nullptr)
{
}

bool TimerTargetSelector::initWithSelector(Scheduler* scheduler, SEL_SCHEDULE selector, Ref* target, float seconds, unsigned int repeat, float delay)
{
    _scheduler = scheduler;
    _target = target;
    _selector = selector;
    setupTimerWithInterval(seconds, repeat, delay);
    return true;
}

void TimerTargetSelector::trigger()
{
    if (_target && _selector)
    {
        (_target->*_selector)(_elapsed);
    }
}

void TimerTargetSelector::cancel()
{
    _scheduler->unschedule(_selector, _target);
}

// TimerTargetCallback

TimerTargetCallback::TimerTargetCallback()
: _hash_key(nullptr)
, _callback(nullptr)
{
}

bool TimerTargetCallback::initWithCallback(Scheduler* scheduler, const ccSchedulerFunc& callback, void *hash_key, const std::string& key, float seconds, unsigned int repeat, float delay)
{
    _scheduler = scheduler;
	_hash_key = hash_key;
    _callback = callback;
    _key = key;
    setupTimerWithInterval(seconds, repeat, delay);
    return true;
}

void TimerTargetCallback::trigger()
{
    if (_callback)
    {
        _callback(_elapsed);
    }
}

void TimerTargetCallback::cancel()
{
    _scheduler->_unschedule(_key, _hash_key);
}

#if CC_ENABLE_SCRIPT_BINDING

// TimerScriptHandler

bool TimerScriptHandler::initWithScriptHandler(int handler, float seconds, unsigned int repeat, float delay)
{
    _scriptHandler = handler;
    _elapsed = -1;
    setupTimerWithInterval(seconds, repeat, delay);
    
    return true;
}

void TimerScriptHandler::trigger()
{
    if (0 != _scriptHandler)
    {
        SchedulerScriptData data(_scriptHandler,_elapsed);
        ScriptEvent event(kScheduleEvent,&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
}

void TimerScriptHandler::cancel()
{

}

#endif

// implementation of Scheduler

// Priority level reserved for system services.
const int Scheduler::PRIORITY_SYSTEM = INT_MIN;

// Minimum priority level for user scheduling.
const int Scheduler::PRIORITY_NON_SYSTEM_MIN = PRIORITY_SYSTEM + 1;

Scheduler::Scheduler(void)
: _timeScale(1.0f)
, _updatesNegList(nullptr)
, _updates0List(nullptr)
, _updatesPosList(nullptr)
, _hashForUpdates(nullptr)
, _hashForTimers(nullptr)
, _currentTarget(nullptr)
, _currentTargetSalvaged(false)
, _updateHashLocked(false)
#if CC_ENABLE_SCRIPT_BINDING
, _scriptHandlerEntries(20)
#endif
{
    // I don't expect to have more than 30 functions to all per frame
    _functionsToPerform.reserve(30);
}

Scheduler::~Scheduler(void)
{
    unscheduleAll();
}

void Scheduler::removeHashElement(_hashSelectorEntry *element)
{
    ccArrayFree(element->timers);
    HASH_DEL(_hashForTimers, element);
    free(element);
}

void Scheduler::_schedule(const ccSchedulerFunc& callback, void *hash_key, float interval, unsigned int repeat, float delay, bool paused, const std::string& key)
{
	CCASSERT(hash_key, "Argument hash_key must be non-nullptr");
    CCASSERT(!key.empty(), "key should not be empty!");

    tHashTimerEntry *element = nullptr;
	HASH_FIND_PTR(_hashForTimers, &hash_key, element);

    if (! element)
    {
        element = (tHashTimerEntry *)calloc(sizeof(*element), 1);
		element->hash_key = hash_key;

		HASH_ADD_PTR(_hashForTimers, hash_key, element);

        // Is this the 1st element ? Then set the pause level to all the selectors of this target
        element->paused = paused;
    }
    else
    {
        CCASSERT(element->paused == paused, "");
    }

    if (element->timers == nullptr)
    {
        element->timers = ccArrayNew(10);
    }
    else 
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetCallback *timer = static_cast<TimerTargetCallback*>(element->timers->arr[i]);

            if (key == timer->getKey())
            {
                CCLOG("CCScheduler#scheduleSelector. Selector already scheduled. Updating interval from: %.4f to %.4f", timer->getInterval(), interval);
                timer->setInterval(interval);
                return;
            }        
        }
        ccArrayEnsureExtraCapacity(element->timers, 1);
    }

    TimerTargetCallback *timer = new (std::nothrow) TimerTargetCallback();
	timer->initWithCallback(this, callback, hash_key, key, interval, repeat, delay);
    ccArrayAppendObject(element->timers, timer);
    timer->release();
}

void Scheduler::_unschedule(const std::string &key, void *hash_key)
{
    // explicity handle nil arguments when removing an object
	if (hash_key == nullptr || key.empty())
    {
        return;
    }

    //CCASSERT(target);
    //CCASSERT(selector);

    tHashTimerEntry *element = nullptr;
	HASH_FIND_PTR(_hashForTimers, &hash_key, element);

    if (element)
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetCallback *timer = static_cast<TimerTargetCallback*>(element->timers->arr[i]);

            if (key == timer->getKey())
            {
                if (timer == element->currentTimer && (! element->currentTimerSalvaged))
                {
                    element->currentTimer->retain();
                    element->currentTimerSalvaged = true;
                }

                ccArrayRemoveObjectAtIndex(element->timers, i, true);

                // update timerIndex in case we are in tick:, looping over the actions
                if (element->timerIndex >= i)
                {
                    element->timerIndex--;
                }

                if (element->timers->num == 0)
                {
                    if (_currentTarget == element)
                    {
                        _currentTargetSalvaged = true;
                    }
                    else
                    {
                        removeHashElement(element);
                    }
                }

                return;
            }
        }
    }
}

void Scheduler::priorityIn(tListEntry **list, const ccSchedulerFunc& callback, void *hash_key, int priority, bool paused)
{
    tListEntry *listElement = new tListEntry();

    listElement->callback = callback;
	listElement->hash_key = hash_key;
    listElement->priority = priority;
    listElement->paused = paused;
    listElement->next = listElement->prev = nullptr;
    listElement->markedForDeletion = false;

    // empty list ?
    if (! *list)
    {
        DL_APPEND(*list, listElement);
    }
    else
    {
        bool added = false;

        for (tListEntry *element = *list; element; element = element->next)
        {
            if (priority < element->priority)
            {
                if (element == *list)
                {
                    DL_PREPEND(*list, listElement);
                }
                else
                {
                    listElement->next = element;
                    listElement->prev = element->prev;

                    element->prev->next = listElement;
                    element->prev = listElement;
                }

                added = true;
                break;
            }
        }

        // Not added? priority has the higher value. Append it.
        if (! added)
        {
            DL_APPEND(*list, listElement);
        }
    }

    // update hash entry for quick access
    tHashUpdateEntry *hashElement = (tHashUpdateEntry *)calloc(sizeof(*hashElement), 1);
	hashElement->hash_key = hash_key;
    hashElement->list = list;
    hashElement->entry = listElement;
	HASH_ADD_PTR(_hashForUpdates, hash_key, hashElement);
}

void Scheduler::appendIn(_listEntry **list, const ccSchedulerFunc& callback, void *hash_key, bool paused)
{
    tListEntry *listElement = new tListEntry();

    listElement->callback = callback;
	listElement->hash_key = hash_key;
	listElement->priority = 0; //WeiYuemin: when calling appendIn, the priority will always be zero
    listElement->paused = paused;
    listElement->markedForDeletion = false;

    DL_APPEND(*list, listElement);

    // update hash entry for quicker access
    tHashUpdateEntry *hashElement = (tHashUpdateEntry *)calloc(sizeof(*hashElement), 1);
	hashElement->hash_key = hash_key;
    hashElement->list = list;
    hashElement->entry = listElement;
	HASH_ADD_PTR(_hashForUpdates, hash_key, hashElement);
}

void Scheduler::schedulePerFrame(const ccSchedulerFunc& callback, void *hash_key, int priority, bool paused) //weiyuemin: 这个target其实只是hash_key
{
    tHashUpdateEntry *hashElement = nullptr;
	HASH_FIND_PTR(_hashForUpdates, &hash_key, hashElement);
	if (hashElement) 
	{
		// check if priority has changed
		if ((*hashElement->list)->priority != priority)
		{
			if (_updateHashLocked)
			{
				CCLOG("warning: you CANNOT change update priority in scheduled function. hash_key: %x, old_priority: %d, new_priority: %d", (long)hash_key, (*hashElement->list)->priority, priority);
				hashElement->entry->markedForDeletion = false;
				hashElement->entry->paused = paused;
				return;
			}
			else
			{
				// will be added again outside if (hashElement).
				_unscheduleUpdate(hash_key);
			}
		}
		else
		{
			CCLOG("warning: reschedule (%x) with same priority: %d", (long)hash_key, priority);
			hashElement->entry->markedForDeletion = false;
			hashElement->entry->paused = paused;
			return;
		}
    }

    // most of the updates are going to be 0, that's way there
    // is an special list for updates with priority 0
    if (priority == 0)
    {
		appendIn(&_updates0List, callback, hash_key, paused);
    }
    else if (priority < 0)
    {
		priorityIn(&_updatesNegList, callback, hash_key, priority, paused);
    }
    else
    {
        // priority > 0
		priorityIn(&_updatesPosList, callback, hash_key, priority, paused);
    }
}

bool Scheduler::_isScheduled(const std::string& key, void *hash_key)
{
    CCASSERT(!key.empty(), "Argument key must not be empty");
	CCASSERT(hash_key, "Argument hash_key must be non-nullptr");
    
    tHashTimerEntry *element = nullptr;
	HASH_FIND_PTR(_hashForTimers, &hash_key, element);
    
    if (!element)
    {
        return false;
    }
    
    if (element->timers == nullptr)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetCallback *timer = static_cast<TimerTargetCallback*>(element->timers->arr[i]);
            
            if (key == timer->getKey())
            {
                return true;
            }
        }
        
        return false;
    }
    
    return false;  // should never get here
}

void Scheduler::removeUpdateFromHash(struct _listEntry *entry)
{
    tHashUpdateEntry *element = nullptr;

	HASH_FIND_PTR(_hashForUpdates, &entry->hash_key, element);
    if (element)
    {
        // list entry
        DL_DELETE(*element->list, element->entry);
        CC_SAFE_DELETE(element->entry);

        // hash entry
        HASH_DEL(_hashForUpdates, element);
        free(element);
    }
}

void Scheduler::_unscheduleUpdate(void *hash_key)
{
	if (hash_key == nullptr)
    {
        return;
    }

    tHashUpdateEntry *element = nullptr;
	HASH_FIND_PTR(_hashForUpdates, &hash_key, element);
    if (element)
    {
        if (_updateHashLocked)
        {
			//CCLOG("unscheduleUpdate [%x] but hash locked", (long)hash_key);
            element->entry->markedForDeletion = true;
        }
        else
        {
			//CCLOG("unscheduleUpdate [%x] and remove from hash", (long)hash_key);
            this->removeUpdateFromHash(element->entry);
        }
    }
	else
	{
		//add by weiyuemin
		//CCLOG("warning: unschedule but hash element not found. hash_key: [%x]", (long)hash_key);
	}
}

void Scheduler::unscheduleAll(void)
{
    unscheduleAllWithMinPriority(PRIORITY_SYSTEM);
}

void Scheduler::unscheduleAllWithMinPriority(int minPriority)
{
    // Custom Selectors
    tHashTimerEntry *element = nullptr;
    tHashTimerEntry *nextElement = nullptr;
    for (element = _hashForTimers; element != nullptr;)
    {
        // element may be removed in unscheduleAllSelectorsForTarget
        nextElement = (tHashTimerEntry *)element->hh.next;
		_unscheduleAllForTarget(element->hash_key);

        element = nextElement;
    }

    // Updates selectors
    tListEntry *entry, *tmp;
    if(minPriority < 0)
    {
        DL_FOREACH_SAFE(_updatesNegList, entry, tmp)
        {
            if(entry->priority >= minPriority)
            {
				_unscheduleUpdate(entry->hash_key);
            }
        }
    }

    if(minPriority <= 0)
    {
        DL_FOREACH_SAFE(_updates0List, entry, tmp)
        {
			_unscheduleUpdate(entry->hash_key);
        }
    }

    DL_FOREACH_SAFE(_updatesPosList, entry, tmp)
    {
        if(entry->priority >= minPriority)
        {
			_unscheduleUpdate(entry->hash_key);
        }
    }
#if CC_ENABLE_SCRIPT_BINDING
    _scriptHandlerEntries.clear();
#endif
}

void Scheduler::_unscheduleAllForTarget(void *hash_key)
{
    // explicit nullptr handling
	if (hash_key == nullptr)
    {
        return;
    }

    // Custom Selectors
    tHashTimerEntry *element = nullptr;
	HASH_FIND_PTR(_hashForTimers, &hash_key, element);

    if (element)
    {
        if (ccArrayContainsObject(element->timers, element->currentTimer)
            && (! element->currentTimerSalvaged))
        {
            element->currentTimer->retain();
            element->currentTimerSalvaged = true;
        }
        ccArrayRemoveAllObjects(element->timers);

        if (_currentTarget == element)
        {
            _currentTargetSalvaged = true;
        }
        else
        {
            removeHashElement(element);
        }
    }

    // update selector
	_unscheduleUpdate(hash_key);
}

#if CC_ENABLE_SCRIPT_BINDING
unsigned int Scheduler::scheduleScriptFunc(unsigned int handler, float interval, unsigned int repeat, float delay, bool paused)
{
    SchedulerScriptHandlerEntry* entry = SchedulerScriptHandlerEntry::create(handler, interval, repeat, delay, paused);
    _scriptHandlerEntries.pushBack(entry);
    return entry->getEntryId();
}

void Scheduler::unscheduleScriptEntry(unsigned int scheduleScriptEntryID)
{
    for (ssize_t i = _scriptHandlerEntries.size() - 1; i >= 0; i--)
    {
        SchedulerScriptHandlerEntry* entry = _scriptHandlerEntries.at(i);
        if (entry->getEntryId() == (int)scheduleScriptEntryID)
        {
            entry->markedForDeletion();
            break;
        }
    }
}

#endif

void Scheduler::_resumeTarget(void *hash_key)
{
	CCASSERT(hash_key != nullptr, "");

    // custom selectors
    tHashTimerEntry *element = nullptr;
	HASH_FIND_PTR(_hashForTimers, &hash_key, element);
    if (element)
    {
        element->paused = false;
    }

    // update selector
    tHashUpdateEntry *elementUpdate = nullptr;
	HASH_FIND_PTR(_hashForUpdates, &hash_key, elementUpdate);
    if (elementUpdate)
    {
        CCASSERT(elementUpdate->entry != nullptr, "");
        elementUpdate->entry->paused = false;
    }
}

void Scheduler::_pauseTarget(void *hash_key)
{
	CCASSERT(hash_key != nullptr, "");

    // custom selectors
    tHashTimerEntry *element = nullptr;
	HASH_FIND_PTR(_hashForTimers, &hash_key, element);
    if (element)
    {
        element->paused = true;
    }

    // update selector
    tHashUpdateEntry *elementUpdate = nullptr;
	HASH_FIND_PTR(_hashForUpdates, &hash_key, elementUpdate);
    if (elementUpdate)
    {
        CCASSERT(elementUpdate->entry != nullptr, "");
        elementUpdate->entry->paused = true;
    }
}

bool Scheduler::_isTargetPaused(void *hash_key)
{
	CCASSERT(hash_key != nullptr, "hash_key must be non nil");

    // Custom selectors
    tHashTimerEntry *element = nullptr;
	HASH_FIND_PTR(_hashForTimers, &hash_key, element);
    if( element )
    {
        return element->paused;
    }
    
    // We should check update selectors if target does not have custom selectors
	tHashUpdateEntry *elementUpdate = nullptr;
	HASH_FIND_PTR(_hashForUpdates, &hash_key, elementUpdate);
	if ( elementUpdate )
    {
		return elementUpdate->entry->paused;
    }
    
    return false;  // should never get here
}

std::set<void*> Scheduler::pauseAllTargets()
{
    return pauseAllTargetsWithMinPriority(PRIORITY_SYSTEM);
}

std::set<void*> Scheduler::pauseAllTargetsWithMinPriority(int minPriority)
{
    std::set<void*> idsWithSelectors;

    // Custom Selectors
    for(tHashTimerEntry *element = _hashForTimers; element != nullptr;
        element = (tHashTimerEntry*)element->hh.next)
    {
        element->paused = true;
		idsWithSelectors.insert(element->hash_key);
    }

    // Updates selectors
    tListEntry *entry, *tmp;
    if(minPriority < 0)
    {
        DL_FOREACH_SAFE( _updatesNegList, entry, tmp ) 
        {
            if(entry->priority >= minPriority)
            {
                entry->paused = true;
				idsWithSelectors.insert(entry->hash_key);
            }
        }
    }

    if(minPriority <= 0)
    {
        DL_FOREACH_SAFE( _updates0List, entry, tmp )
        {
            entry->paused = true;
			idsWithSelectors.insert(entry->hash_key);
        }
    }

    DL_FOREACH_SAFE( _updatesPosList, entry, tmp ) 
    {
        if(entry->priority >= minPriority) 
        {
            entry->paused = true;
			idsWithSelectors.insert(entry->hash_key);
        }
    }

    return idsWithSelectors;
}

void Scheduler::resumeTargets(const std::set<void*>& hash_keysToResume)
{
	for (const auto &hash_key : hash_keysToResume) {
		_resumeTarget(hash_key);
    }
}

void Scheduler::performFunctionInCocosThread(const std::function<void ()> &function)
{
    _performMutex.lock();

    _functionsToPerform.push_back(function);

    _performMutex.unlock();
}

// main loop
void Scheduler::update(float dt)
{
    _updateHashLocked = true;

    if (_timeScale != 1.0f)
    {
        dt *= _timeScale;
    }

    //
    // Selector callbacks
    //

    // Iterate over all the Updates' selectors
    tListEntry *entry, *tmp;

    // updates with priority < 0
    DL_FOREACH_SAFE(_updatesNegList, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->callback(dt);
        }
    }

    // updates with priority == 0
    DL_FOREACH_SAFE(_updates0List, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->callback(dt);
        }
    }

    // updates with priority > 0
    DL_FOREACH_SAFE(_updatesPosList, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->callback(dt);
        }
    }

    // Iterate over all the custom selectors
    for (tHashTimerEntry *elt = _hashForTimers; elt != nullptr; )
    {
        _currentTarget = elt;
        _currentTargetSalvaged = false;

        if (! _currentTarget->paused)
        {
            // The 'timers' array may change while inside this loop
            for (elt->timerIndex = 0; elt->timerIndex < elt->timers->num; ++(elt->timerIndex))
            {
                elt->currentTimer = (Timer*)(elt->timers->arr[elt->timerIndex]);
                elt->currentTimerSalvaged = false;

                elt->currentTimer->update(dt);

                if (elt->currentTimerSalvaged)
                {
                    // The currentTimer told the remove itself. To prevent the timer from
                    // accidentally deallocating itself before finishing its step, we retained
                    // it. Now that step is done, it's safe to release it.
                    elt->currentTimer->release();
                }

                elt->currentTimer = nullptr;
            }
        }

        // elt, at this moment, is still valid
        // so it is safe to ask this here (issue #490)
        elt = (tHashTimerEntry *)elt->hh.next;

        // only delete currentTarget if no actions were scheduled during the cycle (issue #481)
        if (_currentTargetSalvaged && _currentTarget->timers->num == 0)
        {
            removeHashElement(_currentTarget);
        }
    }

    // delete all updates that are marked for deletion
    // updates with priority < 0
    DL_FOREACH_SAFE(_updatesNegList, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }

    // updates with priority == 0
    DL_FOREACH_SAFE(_updates0List, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }

    // updates with priority > 0
    DL_FOREACH_SAFE(_updatesPosList, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }

    _updateHashLocked = false;
    _currentTarget = nullptr;

#if CC_ENABLE_SCRIPT_BINDING
    //
    // Script callbacks
    //

    // Iterate over all the script callbacks
    if (!_scriptHandlerEntries.empty())
    {
        for (auto i = _scriptHandlerEntries.size() - 1; i >= 0; i--)
        {
            SchedulerScriptHandlerEntry* eachEntry = _scriptHandlerEntries.at(i);
            if (eachEntry->isMarkedForDeletion())
            {
                _scriptHandlerEntries.erase(i);
            }
            else if (!eachEntry->isPaused())
            {
                eachEntry->getTimer()->update(dt);
            }
        }
    }
#endif
    //
    // Functions allocated from another thread
    //

    // Testing size is faster than locking / unlocking.
    // And almost never there will be functions scheduled to be called.
    if( !_functionsToPerform.empty() ) {
        _performMutex.lock();
        // fixed #4123: Save the callback functions, they must be invoked after '_performMutex.unlock()', otherwise if new functions are added in callback, it will cause thread deadlock.
        auto temp = _functionsToPerform;
        _functionsToPerform.clear();
        _performMutex.unlock();
        for( const auto &function : temp ) {
            function();
        }
        
    }
}

void Scheduler::schedule(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused)
{
    CCASSERT(target, "Argument target must be non-nullptr");
    
	void* hash_key = (void*)target->_ID;

    tHashTimerEntry *element = nullptr;
	HASH_FIND_PTR(_hashForTimers, &hash_key, element);
    
    if (! element)
    {
        element = (tHashTimerEntry *)calloc(sizeof(*element), 1);
		element->hash_key = hash_key;
        
		HASH_ADD_PTR(_hashForTimers, hash_key, element);
        
        // Is this the 1st element ? Then set the pause level to all the selectors of this target
        element->paused = paused;
    }
    else
    {
        CCASSERT(element->paused == paused, "");
    }
    
    if (element->timers == nullptr)
    {
        element->timers = ccArrayNew(10);
    }
    else
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetSelector *timer = static_cast<TimerTargetSelector*>(element->timers->arr[i]);
            
            if (selector == timer->getSelector())
            {
                CCLOG("CCScheduler#scheduleSelector. Selector already scheduled. Updating interval from: %.4f to %.4f", timer->getInterval(), interval);
                timer->setInterval(interval);
                return;
            }
        }
        ccArrayEnsureExtraCapacity(element->timers, 1);
    }
    
    TimerTargetSelector *timer = new (std::nothrow) TimerTargetSelector();
    timer->initWithSelector(this, selector, target, interval, repeat, delay);
    ccArrayAppendObject(element->timers, timer);
    timer->release();
}

void Scheduler::schedule(SEL_SCHEDULE selector, Ref *target, float interval, bool paused)
{
    this->schedule(selector, target, interval, CC_REPEAT_FOREVER, 0.0f, paused);
}

bool Scheduler::isScheduled(SEL_SCHEDULE selector, Ref *target)
{
    CCASSERT(selector, "Argument selector must be non-nullptr");
    CCASSERT(target, "Argument target must be non-nullptr");

	void* hash_key = (void*)target->_ID;
    
    tHashTimerEntry *element = nullptr;
	HASH_FIND_PTR(_hashForTimers, &hash_key, element);
    
    if (!element)
    {
        return false;
    }
    
    if (element->timers == nullptr)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetSelector *timer = static_cast<TimerTargetSelector*>(element->timers->arr[i]);
            
            if (selector == timer->getSelector())
            {
                return true;
            }
        }
        
        return false;
    }
    
    return false;  // should never get here
}

void Scheduler::unschedule(SEL_SCHEDULE selector, Ref *target)
{
    // explicity handle nil arguments when removing an object
    if (target == nullptr || selector == nullptr)
    {
        return;
    }

	void* hash_key = (void*)target->_ID;
    
    //CCASSERT(target);
    //CCASSERT(selector);
    
    tHashTimerEntry *element = nullptr;
	HASH_FIND_PTR(_hashForTimers, &hash_key, element);
    
    if (element)
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            TimerTargetSelector *timer = static_cast<TimerTargetSelector*>(element->timers->arr[i]);
            
            if (selector == timer->getSelector())
            {
                if (timer == element->currentTimer && (! element->currentTimerSalvaged))
                {
                    element->currentTimer->retain();
                    element->currentTimerSalvaged = true;
                }
                
                ccArrayRemoveObjectAtIndex(element->timers, i, true);
                
                // update timerIndex in case we are in tick:, looping over the actions
                if (element->timerIndex >= i)
                {
                    element->timerIndex--;
                }
                
                if (element->timers->num == 0)
                {
                    if (_currentTarget == element)
                    {
                        _currentTargetSalvaged = true;
                    }
                    else
                    {
                        removeHashElement(element);
                    }
                }
                
                return;
            }
        }
    }
}

NS_CC_END
