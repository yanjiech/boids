//
//  HintNode.h
//  Boids
//
//  Created by chenyanjie on 6/12/15.
//
//

#ifndef __Boids__HintNode__
#define __Boids__HintNode__

#include "cocos2d.h"

class HintNode : public cocos2d::Node {
private:
    std::string _name;
    cocos2d::Sprite* _icon;
    
public:
    HintNode();
    virtual ~HintNode();
    
    static HintNode* create( const std::string& name, cocos2d::Sprite* icon );
    virtual bool init( const std::string& name, cocos2d::Sprite* icon );
    
    void setHintName( const std::string& name ) { _name = name; }
    const std::string& getHintName() { return _name; }
};

#endif /* defined(__Boids__HintNode__) */
