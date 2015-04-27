//
//  MapData.cpp
//  Boids
//
//  Created by Xin Xu on 11/18/14.
//
//

#include "MapData.h"
#include "Utils.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "ui/UITextField.h"
#include "BETypes.h"
#include <fstream>
#include "./util/CocosUtils.h"
#include "./constant/BoidsConstant.h"

using namespace cocos2d;

MapData::MapData() {
    
}

MapData::~MapData() {
}

MapData* MapData::create( const std::string& path ) {
    MapData* ret = new MapData();
    if( ret && ret->init( path ) ) {
        ret->autorelease();
        return ret;
    }
    else {
        CC_SAFE_DELETE( ret );
        return nullptr;
    }
}

bool MapData::init( const std::string& path ) {
    _path = path;
    auto json = Utils::stringFormat("%s/meta.json", path.c_str());
    _metaData = FileUtils::getInstance()->getStringFromFile(json);
    if (_metaData.length() == 0) {
        _metaData = "{}";
    }
    auto mapfile = Utils::stringFormat("%s/map.tmx", path.c_str());
    _mapData = FileUtils::getInstance()->getStringFromFile(mapfile);
    if (_mapData.empty())
    {
        cocos2d::log("[FATAL] map file(%s) not found or read failed !!", mapfile.c_str());
        return false;
    }
    preprocessMapData();
    return true;
}

const std::string& MapData::getMetaData() {
    return _metaData;
}

void MapData::dumpMetaData(const std::string& content) {
    std::ofstream file;
    auto json = Utils::stringFormat("%s/meta.json", _path.c_str());
    file.open(json);
    file << content;
    file.close();
}

const std::string& MapData::getMapData() {
    return _mapData;
}

cocos2d::Sprite* MapData::spriteFromObject( TMXTiledMap* map, const cocos2d::Value& o, bool createFromCache ) {
    auto object = o.asValueMap();
    int gid = object.at("gid").asInt();
    if( gid != 0 ) {
        bool flipped_horizontally = false;
        bool flipped_vertically = false;
        bool flipped_diagonally = false;
        if( ( gid & FLIPPED_HORIZONTALLY ) != 0 ) {
            flipped_horizontally = true;
            gid &= 0x7fffffff;
        }
        if( ( gid & FLIPPED_VERTICALLY ) != 0 ) {
            flipped_vertically = true;
            gid &= 0xbfffffff;
        }
        if( ( gid & FLIPPED_DIAGONALLY ) != 0 ) {
            flipped_diagonally = true;
            gid &= 0xdfffffff;
        }
        auto x = object.at("x").asFloat();
        auto y = object.at("y").asFloat();
        auto p = map->getPropertiesForGID(gid).asValueMap();
        
        if( p.find( "source" ) != p.end() ) {
            auto source = p.at("source").asString();
            Sprite *sprite = nullptr;
            if (createFromCache) {
                sprite = Sprite::createWithSpriteFrameName(source);
            } else {
                auto spritePath = Utils::stringFormat("%s/%s", this->_path.c_str(), source.c_str());
                sprite = Sprite::create(spritePath);
            }
            sprite->setAnchorPoint(Vec2::ZERO);
            sprite->setPosition( Point( x, y ) );
            if( flipped_horizontally ) {
                sprite->setFlippedX( true );
            }
            if( flipped_vertically ) {
                sprite->setFlippedY( true );
            }
            return sprite;
        }
    }
    return nullptr;
}

TMXTiledMap* MapData::generateTiledMapWithFlags(int flags) {
	assert(_mapData.size() && "_mapData empty");
    auto map = TMXTiledMap::createWithXML(_mapData, _path);
    float map_width = map->getMapSize().width * map->getTileSize().width;
    float map_height = map->getTileSize().height * map->getMapSize().height;
    map->setContentSize( Size( map_width, map_height ) );
    
    if ((flags & (int)TMXObjectLayerFlag::OnGround) && map->getObjectGroup("onground") != nullptr) {
        auto onGroundLayer = Layer::create();
        map->addChild(onGroundLayer, -99, "onGroundLayer");
        auto ogObjects = map->getObjectGroup("onground")->getObjects();
        for (auto o : ogObjects) {
            auto sp = this->spriteFromObject( map, o, true );
            if( sp ) {
                onGroundLayer->addChild(sp);
            }
        }
    }
    
    if ((flags & (int)TMXObjectLayerFlag::Object) && map->getObjectGroup("vision") != nullptr) {
        auto objectLayer = Layer::create();
        map->addChild(objectLayer, 0, "objectLayer");
        auto objects = map->getObjectGroup("vision")->getObjects();
        for (auto o : objects) {
            auto sp = this->spriteFromObject( map, o, true );
            if( sp ) {
                objectLayer->addChild(sp, (int)-sp->getPosition().x - (int)sp->getPosition().y * (int)map_width );
            }
        }
    }
    
    return map;
}

cocos2d::ValueMap MapData::getAreaMapByName( const std::string& name ) {
    cocos2d::ValueMap ret;
    const ValueVector& position_vector = _meta_json.at( "positions" ).asValueVector();
    for( auto itr = position_vector.begin(); itr != position_vector.end(); ++itr ) {
        const ValueMap& pos_data = itr->asValueMap();
        if( pos_data.at( "name" ).asString() == name ) {
            ret = pos_data;
            break;
        }
    }
    return ret;
}

cocos2d::ValueVector MapData::getAreasVectorByTag( const std::string& tag_name ) {
    ValueVector ret;
    const ValueVector& position_vector = _meta_json.at( "positions" ).asValueVector();
    for( auto itr = position_vector.begin(); itr != position_vector.end(); ++itr ) {
        const ValueMap& pos_data = itr->asValueMap();
        auto sitr = pos_data.find( "tag" );
        if( sitr != pos_data.end() ) {
            if( sitr->second.asString() == tag_name ) {
                ret.push_back( Value( pos_data ) );
            }
        }
    }
    return ret;
    return ret;
}

cocos2d::ValueMap MapData::getAreaMapByPosition( const cocos2d::Point& pos ) {
    ValueMap ret;
    const ValueVector& position_vector = _meta_json.at( "positions" ).asValueVector();
    for( auto itr = position_vector.begin(); itr != position_vector.end(); ++itr ) {
        const ValueMap& pos_data = itr->asValueMap();
        const ValueMap& rect_data = pos_data.at( "rect" ).asValueMap();
        float x = rect_data.at( "x" ).asFloat();
        float y = rect_data.at( "y" ).asFloat();
        float width = rect_data.at( "width" ).asFloat();
        float height = rect_data.at( "height" ).asFloat();
        Rect rect = Rect( x, y, width, height );
        if( rect.containsPoint( pos ) ) {
            ret = pos_data;
            break;
        }
    }
    
    return ret;
}

void MapData::preprocessMapData() {
    rapidjson::Document meta_json;
    meta_json.Parse<0>(_metaData.c_str());
    _meta_json = CocosUtils::jsonObjectToValueMap( meta_json );
}
