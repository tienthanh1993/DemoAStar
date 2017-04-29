//
//  TankMap.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/6/17.
//
//

#include "TankMap.h"
#include "GameManager.h"
#include "constants.h"
#include "PathFinder.h"

USING_NS_CC;
int gDir[8][2] = {
    {0,-1}, // top
    {0,+1}, //bot
    {-1,0}, //left
    {+1,0}, // right
    {-1,-1}, //top left
    {+1,-1}, //top right
    {-1,1}, // bot left
    {+1,1} //bot right
};
TankMap::TankMap() {
    
}

TankMap::~TankMap() {
    
}

void TankMap::onEnter() {
    Layer::onEnter();
    
   PathFinder::getInstance()->setUp(this);
}

bool TankMap::init() {
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    //read current level
    int stage = GameManager::getInstance()->getLevel();
    ValueVector arr = FileUtils::getInstance()->getValueVectorFromFile(MAP_FILE);
    if (stage > arr.size()) {
        stage = 1;
    }
    
    ValueMap dic = arr.at(stage-1).asValueMap();
    std::string str = dic.at("map").asString();
    
    //load background
    Sprite* bg = Sprite::create(StringUtils::format(LEVEL_BG_FORMAT,1+((stage-1)/10)).c_str());
    bg->setAnchorPoint(Vec2(0,0));
    //bg->setOpacity(100);
    bg->setPosition(Vec2(0,0));
    addChild(bg);
    
    //load map
    _tileMap = TMXTiledMap::create(str.c_str());
    _tileMap->setPosition(Vec2::ZERO);
    addChild(_tileMap);
    
    //Map width - height
    m_nTs_width = _tileMap->getTileSize().width;
    m_nTs_height = _tileMap->getTileSize().height;
    
    //spawn point
    TMXObjectGroup *spawnPoint = _tileMap->getObjectGroup("spawnPoints");
    
    //enemy tank point
    int spCount = 0;
    ValueMap dicSpawnPoints;
    
    while (true){
        dicSpawnPoints = spawnPoint->getObject(StringUtils::format("re%d",++spCount).c_str());
        CCLOG("spawn point = %lu %d",dicSpawnPoints.size(),dicSpawnPoints.empty());
        if(dicSpawnPoints.empty()) {
            break;
        }
        int x = dicSpawnPoints.at("x").asInt() + m_nTs_width * 0.5;
        int y = dicSpawnPoints.at("y").asInt() + m_nTs_height * 0.5;
 
        _spawns.push_back(Vec2(x,y));
    }
    
    //tank point
    ValueMap tankpointdic = spawnPoint->getObject(StringUtils::format("tankpoint").c_str());
    _tankPoint = Point(tankpointdic.at("x").asInt() + m_nTs_width * 0.5,tankpointdic.at("y").asInt() + m_nTs_height * 0.5);
    
    //boss point
    TMXObjectGroup* bossPoint = _tileMap->getObjectGroup("bossPoint");
    ValueMap bosspointdic = bossPoint->getObject(StringUtils::format("boss1").c_str());
    _bossPoint = Point(bosspointdic.at("x").asInt() + m_nTs_width * 0.5,bosspointdic.at("y").asInt() + m_nTs_height * 0.5);
    
    //building
    TMXObjectGroup* buildingPoint = _tileMap->getObjectGroup("buildingPoint");
    ValueMap buildingpointdic = buildingPoint->getObject(StringUtils::format("building1").c_str());
    _buildingPoint = Point(buildingpointdic.at("x").asInt(),buildingpointdic.at("y").asInt());
    
    
    //end point
    TMXObjectGroup* endPoint = _tileMap->getObjectGroup("endPoint");
    ValueMap endpointdic = endPoint->getObject(StringUtils::format("endpoint").c_str());
    _endPoint = Point(endpointdic.at("x").asInt() + m_nTs_width * 0.5,endpointdic.at("y").asInt() + m_nTs_height * 0.5);
    
    //road path
    TMXObjectGroup *roadPoints = _tileMap->getObjectGroup("roadPoints");
//
    _roadPoints = __Array::create();
     CC_SAFE_RETAIN(_roadPoints);
    
    
    ValueMap dicRoadPoints;
    
    for (int i=0; i < roadPoints->getObjects().size(); i++) {
        dicRoadPoints = roadPoints->getObjects().at(i).asValueMap();
        int x = dicRoadPoints.at("x").asInt() + m_nTs_width * 0.5;
        int y = dicRoadPoints.at("y").asInt() + m_nTs_height * 0.5;
        
        Node* node = Node::create();
        node->setPosition(Vec2(x,y));
        _roadPoints->addObject(node);
    }

    
    _canDesLayer = _tileMap->getLayer("canDes");
    _canDesLayer->getTexture()->setAntiAliasTexParameters();
    
    _cannotDesLayer = _tileMap->getLayer("cannotDes");
    _cannotDesLayer->getTexture()->setAntiAliasTexParameters();
    
    _canDes = __Array::create();
    CC_SAFE_RETAIN(_canDes);
    _cannotDes = __Array::create();
    CC_SAFE_RETAIN(_cannotDes);

    for (int i=0;i<20;i++)
    {
        for (int j=0;j<12;j++)
        {
            if (_canDesLayer->getTileGIDAt(Vec2(i,j)))
            {
                Sprite* sp = _canDesLayer->getTileAt(Vec2(i,j));
                sp->setAnchorPoint(Vec2(0.5,0.5));
                sp->setPosition(Vec2(sp->getPositionX() + sp->getContentSize().width /2 ,sp->getPositionY() + sp->getContentSize().height /2));
                _canDes->addObject(sp);
            }
            if (_cannotDesLayer->getTileGIDAt(Vec2(i,j)))
            {
                Sprite* sp = _cannotDesLayer->getTileAt(Vec2(i,j));
                sp->setAnchorPoint(Vec2(0.5,0.5));
                sp->setPosition(Vec2(sp->getPositionX() + sp->getContentSize().width /2 ,sp->getPositionY() + sp->getContentSize().height /2));
                _cannotDes->addObject(sp);
            }
            
        }
    }
    _mapWidth = (_tileMap->getTileSize().width )*_tileMap->getMapSize().width;
    _mapHeight = _tileMap->getTileSize().height*_tileMap->getMapSize().height;
    return true;
}

