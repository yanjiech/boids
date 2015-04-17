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
#include "../AI/Collidable.h"
#include "../AI/BoidsPolygon.h"
#include "spine/spine-cocos2dx.h"

class BlockNode : public TargetNode {
protected:
    std::string _block_name;
    
    bool _is_enabled;
    BoidsPolygon _boundaries;
    
public:
    BlockNode();
    virtual ~BlockNode();
    
    static BlockNode* create( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    virtual bool isEnabled() { return _is_enabled; }
    virtual void setEnabled( bool b );
    
    const std::string& getBlockName() { return _block_name; }
    void setBlockName( const std::string& name ) { _block_name = name; }
    
    BoidsPolygon& getBoundaries() { return _boundaries; }
};

class SpriteBlockNode : public BlockNode {
private:
    cocos2d::Sprite* _normal_sprite;
    cocos2d::Sprite* _destroyed_sprite;
    
public:
    SpriteBlockNode();
    virtual ~SpriteBlockNode();
    
    static SpriteBlockNode* create( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    virtual void setEnabled( bool b );
};

class SpineBlockNode : public BlockNode {
private:
    spine::SkeletonAnimation* _skeleton;
    
public:
    SpineBlockNode();
    virtual ~SpineBlockNode();
    
    static SpineBlockNode* create( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    virtual void setEnabled( bool b );
};

#endif /* defined(__Boids__BlockNode__) */
