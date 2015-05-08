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
#include "../AI/BoidsPolygon.h"

class BuildingNode : public TargetNode {
private:
    std::string _building_name;
    cocos2d::Point _center;
    cocos2d::Sprite* _displayed_sprite;
    BoidsPolygon _boundaries;
    
protected:
    bool _is_enabled;
    
public:
    BuildingNode();
    virtual ~BuildingNode();
    
    static BuildingNode* create( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    void setCenter( const cocos2d::Point& center ) { _center = center; }
    const cocos2d::Point& getCenter() { return _center; }
    
    virtual void updateFrame( float delta ) {}
    
    const std::string& getBuildingName() { return _building_name; }
    void setBuildingName( const std::string& name ) { _building_name = name; }
    
    BoidsPolygon& getBoundaries() { return _boundaries; }
    
    virtual void setCollidable( bool b );
    
    bool isEnabled() { return _is_enabled; }
    void setEnabled( bool b ) { _is_enabled = b; }
    
    virtual bool willCollide( cocos2d::Point pos, float radius ) { return false; }
    virtual bool willCollide( TargetNode* unit) { return false; }
    virtual bool willCollide( TargetNode* unit, cocos2d::Point unit_new_pos ) { return false; }
};

class BuffBuildingNode : public BuildingNode {
private:
    float _range;
    
    float _duration;
    float _elapse;
    
    cocos2d::ValueMap _buff_data;
    
public:
    BuffBuildingNode();
    virtual ~BuffBuildingNode();
    
    static BuffBuildingNode* create( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    virtual void updateFrame( float delta );
    
    float getRange() { return _range; }
    void setRange( float range ) { _range = range; }
};

#endif /* defined(__Boids__BuildingNode__) */
