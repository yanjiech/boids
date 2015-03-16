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

//modified a lot by WeiYuemin
//all void* target are changed to void* hash_key
//and the actual key is target->_ID (the Ref id which is unique), not the target's pointer value

#ifndef __CCSCHEDULER_H__
#define __CCSCHEDULER_H__

#include <functional>
#include <mutex>
#include <set>

#include "base/CCRef.h"
#include "base/CCVector.h"
#include "base/uthash.h"

NS_CC_BEGIN

/**
 * @addtogroup global
 * @{
 */

class Scheduler;

typedef std::function<void(float)> ccSchedulerFunc;
//
// Timer
//
/** @brief Light-weight timer */
//
class CC_DLL Timer : public Ref
{
protected:
    Timer();
public:
    /** get interval in seconds */
    inline float getInterval() const { return _interval; };
    /** set interval in seconds */
    inline void setInterval(float interval) { _interval = interval; };
    
    void setupTimerWithInterval(float seconds, unsigned int repeat, float delay);
    
    virtual void trigger() = 0;
    virtual void cancel() = 0;
    
    /** triggers the timer */
    void update(float dt);
    
protected:
    
    Scheduler* _scheduler; // weak ref
    float _elapsed;
    bool _runForever;
    bool _useDelay;
    unsigned int _timesExecuted;
    unsigned int _repeat; //0 = once, 1 is 2 x executed
    float _delay;
    float _interval;
};


class CC_DLL TimerTargetSelector : public Timer
{
public:
    TimerTargetSelector();

    /** Initializes a timer with a target, a selector and an interval in seconds, repeat in number of times to repeat, delay in seconds. */
    bool initWithSelector(Scheduler* scheduler, SEL_SCHEDULE selector, Ref* target, float seconds, unsigned int repeat, float delay);
    
    inline SEL_SCHEDULE getSelector() const { return _selector; };
    
    virtual void trigger() override;
    virtual void cancel() override;
    
protected:
    Ref* _target;
    SEL_SCHEDULE _selector;
};


class CC_DLL TimerTargetCallback : public Timer
{
public:
    TimerTargetCallback();
    
    /** Initializes a timer with a target, a lambda and an interval in seconds, repeat in number of times to repeat, delay in seconds. */
    bool initWithCallback(Scheduler* scheduler, const ccSchedulerFunc& callback, void *hash_key, const std::string& key, float seconds, unsigned int repeat, float delay);
    
    /**
     * @js NA
     * @lua NA
     */
    inline const ccSchedulerFunc& getCallback() const { return _callback; };
    inline const std::string& getKey() const { return _key; };
    
    virtual void trigger() override;
    virtual void cancel() override;
    
protected:
    void* _hash_key;
    ccSchedulerFunc _callback;
    std::string _key;
};

#if CC_ENABLE_SCRIPT_BINDING

class CC_DLL TimerScriptHandler : public Timer
{
public:
    bool initWithScriptHandler(int handler, float seconds, unsigned int repeat, float delay);
    inline int getScriptHandler() const { return _scriptHandler; };
    
    virtual void trigger() override;
    virtual void cancel() override;
    
private:
    int _scriptHandler;
};

#endif

//
// Scheduler
//
struct _listEntry;
struct _hashSelectorEntry;
struct _hashUpdateEntry;

#if CC_ENABLE_SCRIPT_BINDING
class SchedulerScriptHandlerEntry;
#endif

/** @brief Scheduler is responsible for triggering the scheduled callbacks.
You should not use system timer for your game logic. Instead, use this class.

There are 2 different types of callbacks (selectors):

- update selector: the 'update' selector will be called every frame. You can customize the priority.
- custom selector: A custom selector will be called every frame, or with a custom interval of time

The 'custom selectors' should be avoided when possible. It is faster, and consumes less memory to use the 'update selector'.

*/
class CC_DLL Scheduler : public Ref
{
	friend class TimerTargetCallback;

public:
    // Priority level reserved for system services.
    static const int PRIORITY_SYSTEM;
    
    // Minimum priority level for user scheduling.
    static const int PRIORITY_NON_SYSTEM_MIN;
    /**
     * @js ctor
     */
    Scheduler();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~Scheduler();

    inline float getTimeScale() { return _timeScale; }
    /** Modifies the time of all scheduled callbacks.
    You can use this property to create a 'slow motion' or 'fast forward' effect.
    Default is 1.0. To create a 'slow motion' effect, use values below 1.0.
    To create a 'fast forward' effect, use values higher than 1.0.
    @since v0.8
    @warning It will affect EVERY scheduled selector / action.
    */
    inline void setTimeScale(float timeScale) { _timeScale = timeScale; }

    /** 'update' the scheduler.
     You should NEVER call this method, unless you know what you are doing.
     * @js NA
     * @lua NA
     */
    void update(float dt);

    /////////////////////////////////////
    
    // schedule
    
    /** The scheduled method will be called every 'interval' seconds.
     If paused is true, then it won't be called until it is resumed.
     If 'interval' is 0, it will be called every frame, but if so, it's recommended to use 'scheduleUpdate' instead.
     If the 'callback' is already scheduled, then only the interval parameter will be updated without re-scheduling it again.
     repeat let the action be repeated repeat + 1 times, use CC_REPEAT_FOREVER to let the action run continuously
     delay is the amount of time the action will wait before it'll start
     @param key The key to identify the callback
     @since v3.0
     */
	inline void schedule(const ccSchedulerFunc& callback, cocos2d::Ref *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& key)
	{
		_schedule(callback, (void*)target->_ID, interval, repeat, delay, paused, key);
	}

    /** Calls scheduleCallback with CC_REPEAT_FOREVER and a 0 delay
     @since v3.0
     */
	inline void schedule(const ccSchedulerFunc& callback, cocos2d::Ref *target, float interval, bool paused, const std::string& key)
	{
		this->_schedule(callback, (void*)target->_ID, interval, CC_REPEAT_FOREVER, 0.0f, paused, key);
	}
    
    
    /** The scheduled method will be called every 'interval' seconds.
     If paused is true, then it won't be called until it is resumed.
     If 'interval' is 0, it will be called every frame, but if so, it's recommended to use 'scheduleUpdate' instead.
     If the selector is already scheduled, then only the interval parameter will be updated without re-scheduling it again.
     repeat let the action be repeated repeat + 1 times, use CC_REPEAT_FOREVER to let the action run continuously
     delay is the amount of time the action will wait before it'll start
     
     @since v3.0, repeat and delay added in v1.1
     */
    void schedule(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused);
    
    /** calls scheduleSelector with CC_REPEAT_FOREVER and a 0 delay */
    void schedule(SEL_SCHEDULE selector, Ref *target, float interval, bool paused);
    
    /** Schedules the 'update' selector for a given target with a given priority.
     The 'update' selector will be called every frame.
     The lower the priority, the earlier it is called.
     @since v3.0
     @lua NA
     */
    template <class T>
    void scheduleUpdate(T *target, int priority, bool paused)
    {
        this->schedulePerFrame([target](float dt){
            target->update(dt);
        }, (void*)target->_ID, priority, paused);
    }

#if CC_ENABLE_SCRIPT_BINDING
    // schedule for script bindings
    /** The scheduled script callback will be called every 'interval' seconds.
     If paused is true, then it won't be called until it is resumed.
     If 'interval' is 0, it will be called every frame.
     return schedule script entry ID, used for unscheduleScriptFunc().
     */
    unsigned int scheduleScriptFunc(unsigned int handler, float interval, unsigned int repeat, float delay, bool paused);
#endif
    /////////////////////////////////////
    
    // unschedule

    /** Unschedules a callback for a key and a given target.
     If you want to unschedule the 'callbackPerFrame', use unscheduleUpdate.
     @since v3.0
     */
	inline void unschedule(const std::string& key, cocos2d::Ref *target)
	{
		_unschedule(key, (void*)target->_ID);
	}

    /** Unschedule a selector for a given target.
     If you want to unschedule the "update", use unscheudleUpdate.
     @since v3.0
     */
    void unschedule(SEL_SCHEDULE selector, Ref *target);
    
    /** Unschedules the update selector for a given target
     @since v0.99.3
     */
	inline void unscheduleUpdate(cocos2d::Ref *target)
	{
		_unscheduleUpdate((void*)target->_ID);
	}
    
    /** Unschedules all selectors for a given target.
     This also includes the "update" selector.
     @since v0.99.3
     @js  unscheduleCallbackForTarget
     @lua NA
     */
	inline void unscheduleAllForTarget(cocos2d::Ref *target)
	{
		_unscheduleAllForTarget((void*)target->_ID);
	}
    
    /** Unschedules all selectors from all targets.
     You should NEVER call this method, unless you know what you are doing.
     @since v0.99.3
     */
    void unscheduleAll(void);
    
    /** Unschedules all selectors from all targets with a minimum priority.
     You should only call this with kPriorityNonSystemMin or higher.
     @since v2.0.0
     */
    void unscheduleAllWithMinPriority(int minPriority);
    
#if CC_ENABLE_SCRIPT_BINDING
    /** Unschedule a script entry. */
    void unscheduleScriptEntry(unsigned int scheduleScriptEntryID);
#endif
    
    /////////////////////////////////////
    
    // isScheduled
    
    /** Checks whether a callback associated with 'key' and 'target' is scheduled.
     @since v3.0.0
     */
	inline bool isScheduled(const std::string& key, cocos2d::Ref *target)
	{
		return _isScheduled(key, (void*)target->_ID);
	}
    
    /** Checks whether a selector for a given taget is scheduled.
     @since v3.0
     */
    bool isScheduled(SEL_SCHEDULE selector, Ref *target);
    
    /////////////////////////////////////
    
    /** Pauses the target.
     All scheduled selectors/update for a given target won't be 'ticked' until the target is resumed.
     If the target is not present, nothing happens.
     @since v0.99.3
     */
	inline void pauseTarget(cocos2d::Ref *target)
	{
		_pauseTarget((void*)target->_ID);
	}

    /** Resumes the target.
     The 'target' will be unpaused, so all schedule selectors/update will be 'ticked' again.
     If the target is not present, nothing happens.
     @since v0.99.3
     */
	inline void resumeTarget(cocos2d::Ref *target)
	{
		_resumeTarget((void*)target->_ID);
	}

    /** Returns whether or not the target is paused
    @since v1.0.0
    * In js: var isTargetPaused(var jsObject)
    * @lua NA 
    */
	inline bool _isTargetPaused(cocos2d::Ref *target)
	{
		return _isTargetPaused((void*)target->_ID);
	}

    /** Pause all selectors from all targets.
      You should NEVER call this method, unless you know what you are doing.
     @since v2.0.0
      */
    std::set<void*> pauseAllTargets();

    /** Pause all selectors from all targets with a minimum priority.
      You should only call this with kPriorityNonSystemMin or higher.
      @since v2.0.0
      */
    std::set<void*> pauseAllTargetsWithMinPriority(int minPriority);

    /** Resume selectors on a set of targets.
     This can be useful for undoing a call to pauseAllSelectors.
     @since v2.0.0
      */
    void resumeTargets(const std::set<void*>& hash_keysToResume);

    /** calls a function on the cocos2d thread. Useful when you need to call a cocos2d function from another thread.
     This function is thread safe.
     @since v3.0
     */
    void performFunctionInCocosThread( const std::function<void()> &function);
    
    /////////////////////////////////////
    
    // Deprecated methods:
    
    /** The scheduled method will be called every 'interval' seconds.
     If paused is true, then it won't be called until it is resumed.
     If 'interval' is 0, it will be called every frame, but if so, it's recommended to use 'scheduleUpdateForTarget:' instead.
     If the selector is already scheduled, then only the interval parameter will be updated without re-scheduling it again.
     repeat let the action be repeated repeat + 1 times, use CC_REPEAT_FOREVER to let the action run continuously
     delay is the amount of time the action will wait before it'll start
     @deprecated Please use 'Scheduler::schedule' instead.
     @since v0.99.3, repeat and delay added in v1.1
     */
    CC_DEPRECATED_ATTRIBUTE void scheduleSelector(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused)
    {
        schedule(selector, target, interval, repeat, delay, paused);
    };
    
    /** calls scheduleSelector with CC_REPEAT_FOREVER and a 0 delay 
     *  @deprecated Please use 'Scheduler::schedule' instead.
     */
    CC_DEPRECATED_ATTRIBUTE void scheduleSelector(SEL_SCHEDULE selector, Ref *target, float interval, bool paused)
    {
        schedule(selector, target, interval, paused);
    };
    
    /** Schedules the 'update' selector for a given target with a given priority.
     The 'update' selector will be called every frame.
     The lower the priority, the earlier it is called.
     @deprecated Please use 'Scheduler::scheduleUpdate' instead.
     @since v0.99.3
     */
    template <class T>
    CC_DEPRECATED_ATTRIBUTE void scheduleUpdateForTarget(T* target, int priority, bool paused) { scheduleUpdate(target, priority, paused); };
    
    /** Unschedule a selector for a given target.
     If you want to unschedule the "update", use unscheudleUpdateForTarget.
     @deprecated Please use 'Scheduler::unschedule' instead.
     @since v0.99.3
     */
    CC_DEPRECATED_ATTRIBUTE void unscheduleSelector(SEL_SCHEDULE selector, Ref *target) { unschedule(selector, target); };
    
    /** Checks whether a selector for a given taget is scheduled.
     @deprecated Please use 'Scheduler::isScheduled' instead.
     @since v0.99.3
     */
    CC_DEPRECATED_ATTRIBUTE bool isScheduledForTarget(Ref *target, SEL_SCHEDULE selector) { return isScheduled(selector, target); };
    
    /** Unschedules the update selector for a given target
     @deprecated Please use 'Scheduler::unscheduleUpdate' instead.
     @since v0.99.3
     */
    CC_DEPRECATED_ATTRIBUTE void unscheduleUpdateForTarget(Ref *target) { return unscheduleUpdate(target); };
    
protected:
	void _schedule(const ccSchedulerFunc& callback, void *hash_key, float interval, unsigned int repeat, float delay, bool paused, const std::string& key);

	void _unschedule(const std::string& key, void *hash_key);

	void _unscheduleUpdate(void *hash_key);

	void _unscheduleAllForTarget(void *hash_key);

	bool _isScheduled(const std::string& key, void *hash_key);

	void _pauseTarget(void *hash_key);
	void _resumeTarget(void *hash_key);
	bool _isTargetPaused(void *hash_key);

	//==original protected functions==

    /** Schedules the 'callback' function for a given target with a given priority.
     The 'callback' selector will be called every frame.
     The lower the priority, the earlier it is called.
     @note This method is only for internal use.
     @since v3.0
     */
    void schedulePerFrame(const ccSchedulerFunc& callback, void *hash_key, int priority, bool paused);
    
    void removeHashElement(struct _hashSelectorEntry *element);
    void removeUpdateFromHash(struct _listEntry *entry);

    // update specific

	void priorityIn(struct _listEntry **list, const ccSchedulerFunc& callback, void *hash_key, int priority, bool paused);
	void appendIn(struct _listEntry **list, const ccSchedulerFunc& callback, void *hash_key, bool paused);


    float _timeScale;

    //
    // "updates with priority" stuff
    //
    struct _listEntry *_updatesNegList;        // list of priority < 0
    struct _listEntry *_updates0List;            // list priority == 0
    struct _listEntry *_updatesPosList;        // list priority > 0
    struct _hashUpdateEntry *_hashForUpdates; // hash used to fetch quickly the list entries for pause,delete,etc

    // Used for "selectors with interval"
    struct _hashSelectorEntry *_hashForTimers;
    struct _hashSelectorEntry *_currentTarget;
    bool _currentTargetSalvaged;
    // If true unschedule will not remove anything from a hash. Elements will only be marked for deletion.
    bool _updateHashLocked;
    
#if CC_ENABLE_SCRIPT_BINDING
    Vector<SchedulerScriptHandlerEntry*> _scriptHandlerEntries;
#endif
    
    // Used for "perform Function"
    std::vector<std::function<void()>> _functionsToPerform;
    std::mutex _performMutex;
};

// end of global group
/// @}

NS_CC_END

#endif // __CCSCHEDULER_H__
