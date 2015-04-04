//
//  DamageCalculate.h
//  Boids
//
//  Created by Yanjie Chen on 3/13/15.
//
//

#ifndef __Boids__DamageCalculate__
#define __Boids__DamageCalculate__

#include "cocos2d.h"

typedef std::function<float(float, class UnitData*, class UnitData*)> DamageCalculator;

class DamageCalculate : public cocos2d::Ref {
private:
    std::string _calculator_name;
    float _base_damage;
    
    DamageCalculator _calculator;
    
public:
    DamageCalculate();
    virtual ~DamageCalculate();
    
    static DamageCalculate* create( const std::string& calculator_name, float base_damage );
    virtual bool init( const std::string& calculator_name, float base_damage );
    
    cocos2d::ValueMap calculateDamage( class UnitData* atker_data, class UnitData* defer_data );
    cocos2d::ValueMap calculateDamageWithoutMiss( class UnitData* atker_data, class UnitData* defer_data );
    
    static float calculateDamage( const std::string calculator_name, float base_damage, class UnitData* atker_data, class UnitData* defer_data );
    
    static float calculateResistance( float def, float negl, float neglp );
    
    static bool doesHit( float hit, float dodge, float atker_level, float defer_level );
    
    static bool doesCritical( float cri, float ten, float atker_level, float defer_level );
};

#endif /* defined(__Boids__DamageCalculate__) */
