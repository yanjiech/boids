//
//  TargetNode.cpp
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#include "TargetNode.h"
#include "../Utils.h"

using namespace cocos2d;

TargetNode::TargetNode() :
_target_data( nullptr ),
_deploy_id( 0 )
{
    
}

TargetNode::~TargetNode() {
    CC_SAFE_RELEASE( _target_data );
}

bool TargetNode::init() {
    if( !Node::init() ) {
        return false;
    }
    this->setAttackable( true );
    return true;
}

void TargetNode::updateFrame( float delta ) {
    do {
        if( !this->isDying() ) {
            //skill
            auto itr = _behaviors.find( BEHAVIOR_NAME_SKILL );
            if( itr != _behaviors.end() && itr->second->isEnabled() ) {
                if( itr->second->behave( delta ) ) {
                    break;
                }
            }
            //attack
            itr = _behaviors.find( BEHAVIOR_NAME_ATTACK );
            if( itr != _behaviors.end() && itr->second->isEnabled() ) {
                if( itr->second->behave( delta ) ) {
                    break;
                }
            }
            itr = _behaviors.find( BEHAVIOR_NAME_MOVE );
            if( itr != _behaviors.end() && itr->second->isEnabled() ) {
                if( itr->second->behave( delta ) ) {
                    break;
                }
            }
            itr = _behaviors.find( BEHAVIOR_NAME_IDLE );
            if( itr != _behaviors.end() && itr->second->isEnabled() ) {
                if( itr->second->behave( delta ) ) {
                    break;
                }
            }
        }
    }while( false );
    
    this->updateComponents( delta );
}

ElementData* TargetNode::getTargetData() {
    return _target_data;
}

void TargetNode::setTargetData( ElementData* data ) {
    CC_SAFE_RELEASE( _target_data );
    _target_data = data;
    CC_SAFE_RETAIN( _target_data );
}

void TargetNode::updateComponents( float delta ) {
    cocos2d::Map<std::string, UnitNodeComponent*> components = _components;
    for( auto pair : components ) {
        UnitNodeComponent* comp = pair.second;
        if( comp->shouldRecycle() ) {
            this->removeUnitComponent( comp->getName() );
        }
        else {
            comp->updateFrame( delta );
        }
    }
}

bool TargetNode::addUnitComponent( UnitNodeComponent* component, const std::string& key, eComponentLayer layer_type ) {
    if( _components.find( key ) == _components.end() ) {
        _components.insert( key, component );
        this->addChild( component, layer_type );
        return true;
    }
    return false;
}

void TargetNode::removeUnitComponent( const std::string& key ) {
    auto itr = _components.find( key );
    if( itr != _components.end() ) {
        UnitNodeComponent* component = itr->second;
        _components.erase( itr );
        component->removeFromParent();
    }
}

void TargetNode::adjustAllUnitComponents() {
    
}

void TargetNode::removeAllUnitComponents() {
    for( auto pair : _components ) {
        pair.second->removeFromParent();
    }
    _components.clear();
}

void TargetNode::addBehavior( const std::string& key, BehaviorBase* behavior ) {
    _behaviors.insert( key, behavior );
}

void TargetNode::removeBehavior( const std::string& key ) {
    _behaviors.erase( key );
}

bool TargetNode::isDying() {
    return false;
}

bool TargetNode::isAlive() {
    return false;
}

bool TargetNode::willCollide( cocos2d::Point pos, float radius ) {
    cocos2d::Vec2 d = this->getPosition() - pos;
    return d.length() <= _target_data->collide + radius;
}

bool TargetNode::willCollide( TargetNode* unit) {
    return this->willCollide( unit->getPosition(), unit->getTargetData()->collide );
}

bool TargetNode::willCollide( TargetNode* unit, cocos2d::Point unit_new_pos ) {
    cocos2d::Vec2 d = this->getPosition() - unit_new_pos;
    return d.length() <= _target_data->collide + unit->getTargetData()->collide;
}

bool TargetNode::isFoeOfCamp( eTargetCamp opponent_camp ) {
    eTargetCamp camp = this->getTargetCamp();
    if( camp == eTargetCamp::Player || camp == eTargetCamp::Ally ) {
        if( opponent_camp == eTargetCamp::Enemy || opponent_camp == eTargetCamp::Wild ) {
            return true;
        }
    }
    else if( camp == eTargetCamp::Enemy ) {
        if( opponent_camp == eTargetCamp::Player || opponent_camp == eTargetCamp::Ally || opponent_camp == eTargetCamp::Wild ) {
            return true;
        }
    }
    else if( camp == eTargetCamp::Wild ) {
        if( opponent_camp == eTargetCamp::Player || opponent_camp == eTargetCamp::Ally || opponent_camp == eTargetCamp::Enemy ) {
            return true;
        }
    }
    return false;
}

void TargetNode::addUnitTag( const std::string& tag ) {
    _unit_tags.push_back( Value( tag ) );
}

void TargetNode::removeUnitTag( const std::string& tag ) {
    auto itr = _unit_tags.begin();
    while( itr != _unit_tags.end() ) {
        if( itr->asString() == tag ) {
            itr = _unit_tags.erase( itr );
        }
        else {
            ++itr;
        }
    }
}

void TargetNode::setUnitTags( const std::string& tag_string ) {
    _unit_tags.clear();
    std::vector<std::string> tags;
    Utils::split( tag_string, tags, ',' );
    for( auto str : tags ) {
        _unit_tags.push_back( Value( str ) );
    }
}

bool TargetNode::hasUnitTag( const std::string& tag_name ) {
    for( auto itr = _unit_tags.begin(); itr != _unit_tags.end(); ++itr ) {
        if( itr->asString() == tag_name ) {
            return true;
        }
    }
    return false;
}