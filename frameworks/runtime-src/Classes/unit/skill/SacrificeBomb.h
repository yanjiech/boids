//
//  SacrificeBomb.h
//  Boids
//
//  Created by chenyanjie on 6/4/15.
//
//

#ifndef __Boids__SacrificeBomb__
#define __Boids__SacrificeBomb__

#include "SkillNode.h"

class SacrificeBomb : public SkillNode {
private:
    float _duration;
    float _elapse;
    float _range;
    
    cocos2d::Sprite* _sp_range;
    
    class DamageCalculate* _calculator;
public:
    SacrificeBomb();
    virtual ~SacrificeBomb();
    
    static SacrificeBomb* create( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    virtual bool init( UnitNode* owner, const cocos2d::ValueMap& data, const cocos2d::ValueMap& params );
    
    virtual void updateFrame( float delta );
    
    virtual void begin();
    virtual void end();
    
    class DamageCalculate* getDamageCaculator();
    void setDamageCalculator( class DamageCalculate* calculator );
};

#endif /* defined(__Boids__SacrificeBomb__) */
