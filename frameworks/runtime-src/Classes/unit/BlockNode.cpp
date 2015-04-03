//
//  BlockNode.cpp
//  Boids
//
//  Created by chenyanjie on 4/3/15.
//
//

#include "BlockNode.h"
#include "UnitNode.h"
#include "../BoidsMath.h"

using namespace cocos2d;

BlockNode::BlockNode() {
    
}

BlockNode::~BlockNode() {
    
}

BlockNode* BlockNode::create( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    BlockNode* ret = new BlockNode();
    if( ret && ret->init( grid_properties, obj_properties ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool BlockNode::init( const cocos2d::ValueMap& grid_properties, const cocos2d::ValueMap& obj_properties ) {
    if( !TargetNode::init() ) {
        return false;
    }

    std::string frame_name = grid_properties.at( "source" ).asString();
    
    _normal_sprite = Sprite::createWithSpriteFrameName( frame_name );
    _normal_sprite->setPosition( Point( _normal_sprite->getContentSize().width / 2, _normal_sprite->getContentSize().height / 2 ) );
    this->addChild( _normal_sprite );
    
    frame_name = frame_name.substr( 0, frame_name.size() - 4 ) + "_destroyed.png";
    _destroyed_sprite = Sprite::createWithSpriteFrameName( frame_name );
    _destroyed_sprite->setPosition( _normal_sprite->getPosition() );
    this->addChild( _destroyed_sprite );
    
    this->ignoreAnchorPointForPosition( false );
    this->setContentSize( _normal_sprite->getContentSize() );
    this->setAnchorPoint( Point::ZERO );
    this->setPosition( Point( obj_properties.at( "x" ).asFloat(), obj_properties.at( "y" ).asFloat() ) );
   
    this->setEnabled( obj_properties.at( "enabled" ).asBool() );
    
    if( grid_properties.at( "flipped_horizontally" ).asBool() ) {
        _normal_sprite->setFlippedX( true );
        _destroyed_sprite->setFlippedX( true );
    }
    else if( grid_properties.at( "flipped_vertically" ).asBool() ) {
        _normal_sprite->setFlippedY( true );
        _destroyed_sprite->setFlippedX( true );
    }
    else if( grid_properties.at( "flipped_diagonally" ).asBool() ) {
        _normal_sprite->setFlippedX( true );
        _normal_sprite->setFlippedY( true );
        _destroyed_sprite->setFlippedX( true );
        _destroyed_sprite->setFlippedX( true );
    }
    
    const ValueMap& boundary_data = grid_properties.at( "boundary" ).asValueMap();
    const ValueVector& points = boundary_data.at( "points" ).asValueVector();
    float base_x = boundary_data.at( "x" ).asFloat();
    float base_y = boundary_data.at( "y" ).asFloat();
    int count = (int)points.size();
    for( int i = 0; i < count; i++ ) {
        int next = ( i + 1 ) % count;
        int next_next = ( i + 2 ) % count;
        
        const ValueMap& t = points.at( i ).asValueMap();
        Point pt = Point( base_x + t.at( "x" ).asFloat(), base_y + t.at( "y" ).asFloat() );
        
        const ValueMap& t1 = points.at( next ).asValueMap();
        Point pt1 = Point( base_x + t1.at( "x" ).asFloat(), base_y + t1.at( "y" ).asFloat() );
        
        const ValueMap& t2 = points.at( next_next ).asValueMap();
        Point pt2 = Point( base_x + t2.at( "x" ).asFloat(), base_y + t2.at( "y" ).asFloat() );
        
        Border b = Border( pt, pt1, pt2 );
        _boundaries.push_back( b );
    }
    
//    this->setEnabled( true );
    
    return true;
}

void BlockNode::setEnabled( bool b ) {
    _is_enabled = b;
    if( _is_enabled ) {
        _normal_sprite->setVisible( false );
        _destroyed_sprite->setVisible( true );
    }
    else {
        _normal_sprite->setVisible( true );
        _destroyed_sprite->setVisible( false );
    }
}

bool BlockNode::willCollide( cocos2d::Point pos, float radius ) {
    if( !_is_enabled ) {
        return false;
    }
    int count = (int)_boundaries.size();
    for( int i = 0; i <  count; i++ ) {
        Border b = _boundaries.at( i );
        if( b.willCollide( pos, radius ) ) {
            _last_collide_border_id = i;
            return true;
        }
    }
    return false;
}

bool BlockNode::willCollide( class UnitNode* unit, cocos2d::Point unit_new_pos ) {
    return this->willCollide( unit_new_pos, unit->getUnitData()->collide );
}

bool BlockNode::getAdvisedNewDir( class UnitNode* unit, cocos2d::Vec2 old_dir, cocos2d::Vec2& new_dir ) {
    Border b = _boundaries.at( _last_collide_border_id );
    Vec2 vec = b.vec;
    
    cocos2d::Vec2 perpendicular_towards_outside = Geometry::clockwisePerpendicularVecToLine(vec);
    if (Fuzzy::_greater(old_dir.cross(perpendicular_towards_outside), 0.0f))
    {
        new_dir = Geometry::clockwiseRotate1(-vec); //再额外转一度，免得正好和边平行
    }
    else
    {
        new_dir = Geometry::anticlockwiseRotate1(vec); //再额外转一度，免得正好和边平行
    }
    
    //确认转向后和最初的向量没有超过90度
    return Geometry::deviateLessThan90(old_dir, new_dir);
}

int BlockNode::getPriority() const {
    return INT_MAX;
}