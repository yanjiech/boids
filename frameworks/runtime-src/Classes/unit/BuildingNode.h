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
#include "HpBar.h"

class BuildingNode : public TargetNode {
protected:
    std::string _building_name;
    
    cocos2d::Point _center;
    
    BoidsPolygon _boundaries;
    
    cocos2d::Sprite* _range_sprite;
    
    ProgressBar* _progress_bar;
    
    bool _is_enabled;
    float _range;
    float _duration;
    float _elapse;
    
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
    
    cocos2d::Sprite* getRangeSprite() { return _range_sprite; }
    void setRangeSprite( cocos2d::Sprite* sprite );
    
    float getRange() { return _range; }
    void setRange( float range ) { _range = range; }
};

class BuffBuildingNode : public BuildingNode {
private:
    cocos2d::ValueMap _buff_data;
    cocos2d::Sprite* _displayed_sprite;
    
public:
    BuffBuildingNode();
    virtual ~BuffBuildingNode();
    
    static BuffBuildingNode* create( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    virtual void updateFrame( float delta );
};

class ChestNode : public BuildingNode {
private:
    cocos2d::ValueMapIntKey _drop_items;
    
    spine::SkeletonAnimation* _skeleton;
    spine::SkeletonAnimation* _open_effect;
    
public:
    ChestNode();
    virtual ~ChestNode();
    
    static ChestNode* create( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( BattleLayer* battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    virtual void updateFrame( float delta );
    
    void openChest();
    
    void onOpenEffectAnimationCompleted( int track_index );
};

#endif /* defined(__Boids__BuildingNode__) */
