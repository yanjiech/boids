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

using namespace cocos2d;

MapData::MapData(const std::string& path) {
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
		return;
	}
    preprocessMapData();
}

MapData::~MapData() {
}

const std::string& MapData::getMetaData() {
    return _metaData;
}

void MapData::loadImagesToCache() {
    if (_plistList.size() > 0) {
        for (auto plist : _plistList) {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist);
        }
    } else {
        CCLOG("WARNNING: use individual images for map %s", this->_path.c_str());
        manuallyLoadImagesToCache();
    }
}

void MapData::removeImagesFromCache() {
    for (auto plist : _plistList) {
        SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plist);
    }
    for (auto spriteFrameKey : _manuallyLoadedImages) {
        SpriteFrameCache::getInstance()->removeSpriteFrameByName(spriteFrameKey);
    }
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

TMXTiledMap *MapData::generateTiledMapWithFlags(int flags) {
	assert(_mapData.size() && "_mapData empty");
    auto map = TMXTiledMap::createWithXML(_mapData, _path);
    map->setContentSize(Size(map->getMapSize().width * map->getTileSize().width, map->getTileSize().height * map->getMapSize().height));
    
    
    auto spriteFromObject = [this, map](Value& o, bool createFromCache) {
        auto object = o.asValueMap();
        auto gid = object.at("gid").asInt();
        auto x = object.at("x").asFloat();
        auto y = object.at("y").asFloat();
        auto p = map->getPropertiesForGID(gid);
        auto source = p.asValueMap().at("source").asString();
        Sprite *sprite = nullptr;
        if (createFromCache) {
            sprite = Sprite::createWithSpriteFrameName(source);
        } else {
            auto spritePath = Utils::stringFormat("%s/%s", this->_path.c_str(), source.c_str());
            sprite = Sprite::create(spritePath);
        }
        sprite->setAnchorPoint(Vec2::ZERO);
        sprite->setPosition(Vec2(x, y));
        return sprite;
    };
    
    if ((flags & (int)TMXObjectLayerFlag::Background) && map->getObjectGroup("background") != nullptr) {
        auto bgLayer = Layer::create();
        map->addChild(bgLayer, -100, "backgroundLayer");
        auto bgObjects = map->getObjectGroup("background")->getObjects();
        for (auto o : bgObjects) {
            auto sp = spriteFromObject(o, false);
            bgLayer->addChild(sp);
        }
    }
    
    if ((flags & (int)TMXObjectLayerFlag::OnGround) && map->getObjectGroup("onground") != nullptr) {
        auto onGroundLayer = Layer::create();
        map->addChild(onGroundLayer, -99, "onGroundLayer");
        auto ogObjects = map->getObjectGroup("onground")->getObjects();
        for (auto o : ogObjects) {
            auto sp = spriteFromObject(o, true);
            onGroundLayer->addChild(sp);
        }
    }
    
    if ((flags & (int)TMXObjectLayerFlag::Object) && map->getObjectGroup("vision") != nullptr) {
        auto objectLayer = Layer::create();
        map->addChild(objectLayer, 0, "objectLayer");
        auto objects = map->getObjectGroup("vision")->getObjects();
        for (auto o : objects) {
            auto sp = spriteFromObject(o, true);
            objectLayer->addChild(sp);
        }
    }
    
    return map;
}

const rapidjson::Value& MapData::getAreaByName( const std::string& name ) {
    for( auto itr = _meta_json["positions"].onBegin(); itr != _meta_json["positions"].onEnd(); ++itr ) {
        const rapidjson::Value& pos = *itr;
        std::string pos_name = std::string( pos["name"].GetString(), pos["name"].GetStringLength() );
        if( pos_name == name ) {
            return pos;
        }
    }
    return CocosUtils::NULL_RAPIDJSON_VALUE;
}

cocos2d::ValueMap MapData::getAreaMapByName( const std::string& name ) {
    cocos2d::ValueMap ret;
    const rapidjson::Value& area_json = this->getAreaByName( name );
    if( !area_json.IsNull() ) {
        ret = CocosUtils::jsonObjectToValueMap( area_json );
    }
    return ret;
}

cocos2d::ValueVector MapData::getAreasVectorByTag( const std::string& tag_name ) {
    ValueVector ret;
    return ret;
}

cocos2d::ValueMap MapData::getAreaMapByPosition( const cocos2d::Point& pos ) {
    ValueMap ret;
    for( auto itr = _meta_json["positions"].onBegin(); itr != _meta_json["positions"].onEnd(); ++itr ) {
        const rapidjson::Value& pos_data = *itr;
        float x = (float)pos_data["rect"]["x"].GetDouble();
        float y = (float)pos_data["rect"]["y"].GetDouble();
        float width = (float)pos_data["rect"]["width"].GetDouble();
        float height = (float)pos_data["rect"]["height"].GetDouble();
        Rect rect = Rect( x, y, width, height );
        if( rect.containsPoint( pos ) ) {
            ret = CocosUtils::jsonObjectToValueMap( pos_data );
            break;
        }
    }
    
    return ret;
}

void MapData::preprocessMapData() {
    _meta_json.Parse<0>(_metaData.c_str());
    if (_meta_json.HasMember("plist")) {
        const rapidjson::Value& plist = _meta_json["plist"];
        for (int i = 0; i < plist.Size(); ++i) {
            _plistList.push_back(plist[i].GetString());
        }
    } else {
        auto mapName = Utils::lastComponentOfPath(_path);
        auto plist = Utils::stringFormat("maps/%s/vision.plist", mapName.c_str());
        if (FileUtils::getInstance()->isFileExist(plist)) {
            _plistList.push_back(plist);
        } else {
            int i = 0;
            while (true) {
                auto plist = Utils::stringFormat("maps/%s/vision%d.plist", mapName.c_str(), i);
                if (FileUtils::getInstance()->isFileExist(plist)) {
                    _plistList.push_back(plist);
                    ++i;
                } else {
                    break;
                }
            }
        }
        if (_plistList.size()) {
            rapidjson::Value plistArray;
            plistArray.SetArray();
            for (std::string p : _plistList) {
                rapidjson::Value pnode;
                pnode.SetString(p.c_str(), _meta_json.GetAllocator());
                plistArray.PushBack(pnode, _meta_json.GetAllocator());
            }
            _meta_json.AddMember("plist", plistArray, _meta_json.GetAllocator());
            rapidjson::StringBuffer buffer;
            rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<>>> writer(buffer);
            _meta_json.Accept(writer);
            _metaData = buffer.GetString();
        }
    }
}

void MapData::manuallyLoadImagesToCache() {
    auto map = TMXTiledMap::createWithXML(_mapData, _path);
    auto loadImage = [this, map](ValueMap& vm) {
        auto gid = vm.at("gid").asInt();
        auto p = map->getPropertiesForGID(gid);
        std::string imageName = p.asValueMap().at("source").asString();
        //std::string imagePath = Utils::stringFormat("%s/%s", _path.c_str(), imageName.c_str());
        std::string imagePath = imageName;
        auto sprite = Sprite::create(imagePath);
        if (sprite != nullptr) {
            SpriteFrameCache::getInstance()->addSpriteFrame(sprite->getSpriteFrame(), imageName);
            _manuallyLoadedImages.push_back(imageName);
        }
        auto visionStates = EditorTypeManager::getInstance()->getVisionObjectStateType()->getTypeVector();
        for (auto state : visionStates) {
            auto imageRawName = imageName.substr(0, imageName.length() - 4); // remove .png
            auto name = Utils::stringFormat("%s_%s.png", imageRawName.c_str(), state.c_str());
            //std::string imagePath = Utils::stringFormat("%s/%s", _path.c_str(), name.c_str());
            std::string imagePath = name;
            auto sprite = Sprite::create(imagePath);
            if (sprite != nullptr) {
                SpriteFrameCache::getInstance()->addSpriteFrame(sprite->getSpriteFrame(), name);
                _manuallyLoadedImages.push_back(name);
            }
        }
    };
    auto visionObjects = map->getObjectGroup("vision")->getObjects();
    for (auto o : visionObjects) {
        loadImage(o.asValueMap());
    }
    auto onGroundGroup = map->getObjectGroup("onground");
    if (onGroundGroup) {
        auto onGroundObjects = onGroundGroup->getObjects();
        for (auto o : onGroundObjects) {
            loadImage(o.asValueMap());
        }
    }
}
