//
//  BuildingNode.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__BuildingNode__
#define __Boids__BuildingNode__

#include "TargetNode.h"

class BuildingNode : public TargetNode {
private:
    cocos2d::Point _center;
    cocos2d::Sprite* _displayed_sprite;
    
public:
    BuildingNode();
    virtual ~BuildingNode();
    
    static BuildingNode* create( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init(  const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    void setCenter( const cocos2d::Point& center ) { _center = center; }
    const cocos2d::Point& getCenter() { return _center; }
};

#endif /* defined(__Boids__BuildingNode__) */
