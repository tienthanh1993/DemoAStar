//
//  RGameScene.cpp
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#include "RGameScene.h"
#include "PathFinder.h"
USING_NS_CC;
using namespace ui;
cocos2d::Scene* RGameScene::createScene() {
    auto scene = Scene::create();
    
    auto layer = RGameScene::create();
    
    scene->addChild(layer);
    
    return scene;
}
//constructor
RGameScene::RGameScene() {
    
}

RGameScene::~RGameScene() {
    
}

//init
bool RGameScene::init() {
    if (!Layer::init()) {
        return false;
    }
    _map = TMXTiledMap::create("res/rpg/map.tmx");
    _mapWidth = _map->getTileSize().width* _map->getMapSize().width;
    _mapHeight=_map->getTileSize().height* _map->getMapSize().height;
    RPathFinder::getInstance()->setUp(_map);
    _player = RPlayer::createPlayer();
    _player->setPosition(Vec2(270,270));
    _map->addChild(_player,2);
    
    addChild(_map);
    return true;
    
}
//normal

void RGameScene::registerEvent() {
    
}

void RGameScene::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void RGameScene::onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }
}

void RGameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event) {
    
}

//override
void RGameScene::onEnter() {
    Layer::onEnter();
    registerEvent();
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan     = CC_CALLBACK_2(RGameScene::onTouchBegan,this);
    touchListener->onTouchMoved     = CC_CALLBACK_2(RGameScene::onTouchMoved,this);
    touchListener->onTouchEnded     = CC_CALLBACK_2(RGameScene::onTouchEnded,this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(RGameScene::onTouchCancelled,this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener,this);
    schedule(schedule_selector(RGameScene::mainLoop), 1.0f/60);
}

void RGameScene::onExit() {
    Layer::onExit();
    unRegisterEvent();
    unschedule(schedule_selector(RGameScene::mainLoop));
}

bool RGameScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
  
    return true;
}

void RGameScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {

}

void RGameScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    Vec2 touchPos = _map->convertTouchToNodeSpaceAR(pTouch);
    auto path = RPathFinder::getInstance()->findWalkablePathAstar(_player->getPosition(), touchPos);
    
    std::deque<Vec2> *queue = nullptr;
    if(_player->getRunPath() == nullptr) {
        queue = new std::deque<Vec2>();
    } else {
        queue = _player->getRunPath();
        queue->clear();
    }
    
    for(int i=path.size()-1;i >=0;i-- ) {
        queue->push_back(path.at(i));
    }
    _player->setDesPos(Vec2::ZERO);
    _player->setRunPath(queue);
    CCLOG("path size :%lu",queue->size());
   // _player->setPosition(touchPos);
}

void RGameScene::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {

}


void RGameScene::mainLoop(float dt) {
    setViewPointCenter(_player->getPosition());
    _player->active(dt);
}

void RGameScene::setViewPointCenter(cocos2d::Point position) {
    Size winSize = Director::getInstance()->getWinSize();
    
    float x = MAX(position.x, winSize.width/2);
    float y = MAX(position.y, winSize.height/2);
    x = MIN(x, ( (getMapWidth() )) - winSize.width / 2);
    y = MIN(y, (getMapHeight()) - winSize.height/2);
    Point actualPosition = Point(x, y);
    
    Point centerOfView = Point(winSize.width/2, winSize.height/2);
    Point viewPoint = (centerOfView- actualPosition);
    if (actualPosition.x < winSize.width/2) {
        return;
    }
    _map->setPosition(viewPoint);
}



