//
//  BlockNode.h
//  Boids
//
//  Created by chenyanjie on 4/3/15.
//
//

#ifndef __Boids__BlockNode__
#define __Boids__BlockNode__

#include "TargetNode.h"
#include "../AI/BoidsPolygon.h"
#include "HpBar.h"

class BlockNode : public TargetNode {
protected:
    BattleLayer* _battle_layer;
    std::string _block_name;
    
    bool _is_enabled;
    BoidsPolygon _boundaries;
    
    bool _need_repair;
    float _elapse;
    float _range;
    float _need_time;
    std::vector<std::string> _need_tag;
    
    cocos2d::Point _center;
    
    ProgressBar* _progress_bar;
    
public:
    BlockNode();
    virtual ~BlockNode();
    
    static BlockNode* create( BattleLayer* _battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( BattleLayer* _battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    virtual void updateFrame( float delta );
    
    bool isEnabled() { return _is_enabled; }
    void setEnabled( bool b ) { _is_enabled = b; }
    
    virtual void updateEnabled();
    
    const std::string& getBlockName() { return _block_name; }
    void setBlockName( const std::string& name ) { _block_name = name; }
    
    BoidsPolygon& getBoundaries() { return _boundaries; }
    
    float getRange() { return _range; }
    void setRange( float range ) { _range = range; }
    
    const cocos2d::Point& getCenter() { return _center; }
    void setCenter( const cocos2d::Point& center ) { _center = center; }
};

class SpriteBlockNode : public BlockNode {
private:
    cocos2d::Sprite* _normal_sprite;
    cocos2d::Sprite* _destroyed_sprite;
    
public:
    SpriteBlockNode();
    virtual ~SpriteBlockNode();
    
    static SpriteBlockNode* create( BattleLayer* _battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( BattleLayer* _battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    virtual void updateEnabled();
};

class SpineBlockNode : public BlockNode {
private:
    spine::SkeletonAnimation* _skeleton;
    
public:
    SpineBlockNode();
    virtual ~SpineBlockNode();
    
    static SpineBlockNode* create( BattleLayer* _battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( BattleLayer* _battle_layer, const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    virtual void updateEnabled();
};

#endif /* defined(__Boids__BlockNode__) */
