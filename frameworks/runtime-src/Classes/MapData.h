//
//  MapData.h
//  Boids
//
//  Created by Xin Xu on 11/18/14.
//
//

#ifndef __Boids__MapData__
#define __Boids__MapData__

#include "cocos2d.h"

enum class TMXObjectLayerFlag {
    Background = 1,
    OnGround = 2,
    Object = 4
};

class MapData : public cocos2d::Ref {
public:
    MapData(const std::string& path);
    virtual ~MapData();
    const std::string& getMetaData();
    void dumpMetaData(const std::string& content);
    const std::string& getMapData();
    cocos2d::TMXTiledMap *generateTiledMapWithFlags(int flags);
    void loadImagesToCache();
    void removeImagesFromCache();
    
    const cocos2d::ValueMap& getMapJson() { return _map_json; }
    const cocos2d::ValueMap& getMetaJson() { return _meta_json; }
    
    cocos2d::ValueMap getAreaMapByName( const std::string& name );
    cocos2d::ValueVector getAreasVectorByTag( const std::string& tag_name );
    cocos2d::ValueMap getAreaMapByPosition( const cocos2d::Point& pos );
    
    cocos2d::Sprite* spriteFromObject( cocos2d::TMXTiledMap* map, const cocos2d::Value& o, bool createFromCache );
    
private:
    cocos2d::ValueMap _map_json;
    cocos2d::ValueMap _meta_json;
    
    std::string _mapData;
    std::string _metaData;
    std::string _path;
    std::vector<std::string> _plistList;
    
    void preprocessMapData();
};

#endif /* defined(__Boids__MapData__) */
