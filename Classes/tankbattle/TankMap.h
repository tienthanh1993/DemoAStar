//
//  TankMap.h
//  TankBattle
//
//  Created by Pham Thanh on 3/6/17.
//
//

#ifndef TankMap_h
#define TankMap_h

#include <cocos2d.h>

class MapNode {
public:
    MapNode(cocos2d::Point position,MapNode * parrent) {
        _parrent = parrent;
        _position = position;
        _hScore = 0;
        _gScore = 0;
        _fScore = 0;
        _open = false;
        _close = false;
    }
    ~MapNode() {
        if (_parrent) {
            delete _parrent;
        }
    }
    
     // For each node, the cost of getting from the start node to that node.
    inline int getGScore(MapNode* p) {
        _gScore = p->getPosition().getDistance(getPosition());
        return _gScore;
    }

    inline int getHScore(MapNode* p) {
        _hScore = p->getPosition().getDistanceSq(getPosition());
        return _hScore;
    }
    
    inline int getScore(MapNode* p) {
        _fScore = getGScore(p) + getHScore(p);
        return _fScore;
    }
    
    CC_SYNTHESIZE(bool,_open,Open);
    CC_SYNTHESIZE(bool,_close,Close);
    CC_SYNTHESIZE(float,_gScore,GScore);
    CC_SYNTHESIZE(float,_hScore,HScore);
    CC_SYNTHESIZE(float,_fScore,FScore);
    CC_SYNTHESIZE(cocos2d::Point,_position,Position);
    CC_SYNTHESIZE(MapNode*,_parrent,Parrent);
};

class TankMap : public cocos2d::Layer {
public:
    TankMap();
    virtual ~TankMap();
    virtual bool init();
    void onEnter();
    CREATE_FUNC(TankMap);


    
private:
    CC_SYNTHESIZE(cocos2d::TMXTiledMap*, _tileMap,TiledMap);
    CC_SYNTHESIZE(cocos2d::TMXLayer*, _canDesLayer,CanDesLayer);
    CC_SYNTHESIZE(cocos2d::TMXLayer*, _cannotDesLayer,CannotDesLayer);
    
    
    CC_SYNTHESIZE(cocos2d::__Array*, _canDes,CanDes);
    CC_SYNTHESIZE(cocos2d::__Array*, _cannotDes,CannotDes);
    CC_SYNTHESIZE(cocos2d::__Array*, _spawnPoints,SpawnPoint);
    CC_SYNTHESIZE(cocos2d::__Array*, _roadPoints,RoadPoint);

    CC_SYNTHESIZE(std::vector<cocos2d::Vec2>,_spawns,Spawns);
    CC_SYNTHESIZE(cocos2d::Point, _tankPoint, TankPoint);
    CC_SYNTHESIZE(cocos2d::Point, _endPoint, EndPoint);
    CC_SYNTHESIZE(cocos2d::Point, _buildingPoint, BuildingPoint);
    CC_SYNTHESIZE(cocos2d::Point, _bossPoint, BossPoint);
    
    CC_SYNTHESIZE(float, _mapWidth, MapWidth);
    CC_SYNTHESIZE(float, _mapHeight, MapHeight);
    int m_nTs_width;
    int m_nTs_height;
};
#endif /* TankMap_h */
