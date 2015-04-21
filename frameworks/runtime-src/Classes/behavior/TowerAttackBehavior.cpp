//
//  TowerAttackBehavior.cpp
//  Boids
//
//  Created by chenyanjie on 4/14/15.
//
//

#include "TowerAttackBehavior.h"
#include "../scene/BattleLayer.h"

using namespace cocos2d;

TowerAttackBehavior::TowerAttackBehavior() {
    
}

TowerAttackBehavior::~TowerAttackBehavior() {
    
}

TowerAttackBehavior* TowerAttackBehavior::create( TargetNode* tower ) {
    TowerAttackBehavior* ret = new TowerAttackBehavior();
    if( ret && ret->init( tower ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool TowerAttackBehavior::init( TargetNode* tower ) {
    if( !BehaviorBase::init( tower ) ) {
        return false;
    }
    
    return true;
}

bool TowerAttackBehavior::behave( float delta ) {
    TowerNode* tower = dynamic_cast<TowerNode*>( _target_node );
    if( tower->isBulletLoaded() ) {
        BattleLayer* battle_layer = tower->getBattleLayer();
    
        TargetNode* candidate_target = nullptr;
        float min_distance = (float)INT_MAX;
        Point tower_pos = tower->getPosition();
        
        do {
            Vector<UnitNode*> unit_candidates = battle_layer->getAliveOpponentsInRange( tower->getTargetCamp(), tower_pos, tower_pos, tower->getTargetData()->atk_range );
        
            if( unit_candidates.size() > 0 ) {
                for( auto unit : unit_candidates ) {
                    if( unit->isAttackable() ) {
                        float d = tower_pos.distance( unit->getPosition() );
                        if( d < min_distance ) {
                            min_distance = d;
                            candidate_target = unit;
                        }
                    }
                }
            }
            
            if( candidate_target ) {
                break;
            }
        
            Vector<TowerNode*> tower_candidates = battle_layer->getAliveTowersInRange( tower->getTargetCamp(), tower_pos, tower_pos, tower->getTargetData()->atk_range );
        
            if( tower_candidates.size() > 0 ) {
                for( auto t : tower_candidates ) {
                    if( t->isAttackable() ) {
                        float d = tower_pos.distance( t->getPosition() );
                        if( d < min_distance ) {
                            min_distance = d;
                            candidate_target = t;
                        }
                    }
                }
            }
        }while( false );
    
        if( candidate_target ) {
            tower->changeTowerState( eTowerState::TowerStateAttack );
            tower->attack( candidate_target );
            return true;
        }
        else {
            tower->changeTowerState( eTowerState::TowerStateIdle );
            return false;
        }
    }
    
    return false;
}