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
#include "../AI/Border.h"

class BlockNode : public TargetNode, public Collidable {
private:
    bool _is_enabled;
    std::vector<Border> _boundaries;
    
    cocos2d::Sprite* _normal_sprite;
    cocos2d::Sprite* _destroyed_sprite;
    
    int _last_collide_border_id;
    
public:
    BlockNode();
    virtual ~BlockNode();
    
    static BlockNode* create( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    virtual bool init( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties );
    
    //collidable implementations
    virtual bool willCollide( cocos2d::Point pos, float radius );
    virtual bool willCollide( class UnitNode* unit, cocos2d::Point unit_new_pos );
    virtual bool getAdvisedNewDir( class UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir );
    virtual int getPriority() const;
    
    bool isEnabled() { return _is_enabled; }
    void setEnabled( bool b );
};

#endif /* defined(__Boids__BlockNode__) */
