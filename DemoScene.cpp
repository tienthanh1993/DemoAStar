//
//  DemoScene.cpp
//  TankBattle
//
//  Created by Pham Thanh on 4/6/17.
//
//

#include "DemoScene.h"
#include "PathFinderDemo.h"
#include "GameManager.h"
#include "PathFinder.h"
#include "AnimationHelper.h"

USING_NS_CC;
using namespace ui;
cocos2d::Scene* DemoScene::createScene() {
    auto scene = Scene::create();
    
    auto layer = DemoScene::create();
    
    scene->addChild(layer);
    
    return scene;
}
//constructor
DemoScene::DemoScene() {
    
}

DemoScene::~DemoScene() {
    
}

//init
bool DemoScene::init() {
    if (!Layer::init()) {
        return false;
    }
    GameManager::getInstance()->setLevel(1);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _tankMap = TankMap::create();
    //_tankMap->getCanDesLayer()->setVisible(false);
   // _tankMap->getCannotDesLayer()->setVisible(false);
    addChild(_tankMap);
   
    

    
    
    PathFinderDemo::getInstance()->setUp(_tankMap);
    _movePathLayer = Layer::create();
    _movePathLayer->setContentSize(visibleSize);
    _movePathLayer->setAnchorPoint(Vec2::ZERO);
    addChild(_movePathLayer);
    
    _stepLayer = Layer::create();
    _stepLayer->setContentSize(visibleSize);
    _stepLayer->setAnchorPoint(Vec2::ZERO);
    addChild(_stepLayer);
    
    
    
    
    Point from = PathFinderDemo::getInstance()->tile2poscenter(Point(7,6));
    Point to = PathFinderDemo::getInstance()->tile2poscenter(Point(14,6));


     AnimationHelper::addAnimation("flag",FLAG,"flag",3);
    
    auto _portal = Sprite::create(ANI_FIRST);
    auto animation = AnimationHelper::getAnimation("flag");
    animation->setLoops(-1);
    animation->setDelayPerUnit(0.084f);
    _portal->runAction(Animate::create(animation));
    _portal->setContentSize(Size(_portal->getContentSize().width * 0.6,_portal->getContentSize().height * 0.6));
    _portal->setPosition(from);
    addChild(_portal);

    auto toFlag = Sprite::create(ANI_FIRST);
    auto animation2 = AnimationHelper::getAnimation("flag");
    animation2->setLoops(-1);
    animation2->setDelayPerUnit(0.084f);
    toFlag->runAction(Animate::create(animation2));
    toFlag->setContentSize(Size(toFlag->getContentSize().width * 0.6,toFlag->getContentSize().height * 0.6));
    toFlag->setPosition(to);
    addChild(toFlag);
    
//auto path = PathFinderDemo::getInstance()->findWalkablePathAstar(from,to);
PathFinderDemo::getInstance()->findWalkablePathDijsktra(from,to);
//   PathFinderDemo::getInstance()->findWalkablePathBestFirstSearch(from,to);
    _totalStep = PathFinderDemo::getInstance()->getSteps().size();
//Dijkstra
    auto label = Label::createWithTTF("Dijkstra without obstacle", "fonts/arial.ttf", 32);
    label->setPosition(Vec2(1000,720/2));
    addChild(label);
   schedule(schedule_selector(DemoScene::mainLoop), 10/60);
    
   return true;
}
//normal

void DemoScene::registerEvent() {
    
}

void DemoScene::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void DemoScene::onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }
}

void DemoScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event) {
    
}

void DemoScene::mainLoop(float dt) {
    _gameLoops++;
    if (_gameLoops < 1200) {
        CCLOG("%d",_gameLoops);
        return;
    }
    if (_gameLoops%10==0) {
        
        if(PathFinderDemo::getInstance()->getSteps().size() > 0) {
            Vec2 p = PathFinderDemo::getInstance()->getSteps().front()->getPosition();
            
            PathFinderDemo::getInstance()->getSteps().pop_front();
            int curStep =  PathFinderDemo::getInstance()->getSteps().size();
   
            auto label = Label::createWithTTF(StringUtils::format("%d",_totalStep - curStep), "fonts/arial.ttf", 32);
            label->setPosition(PathFinderDemo::getInstance()->tile2poscenter(p));
            _stepLayer->addChild(label);
        }
        if (PathFinderDemo::getInstance()->getSteps().size() == 1) {
            auto drawNode = DrawNode::create();
            Vec2 * nodeList = new Vec2[PathFinderDemo::getInstance()->getPath().size()];
            for (int i=0; i<  PathFinderDemo::getInstance()->getPath().size();i++) {
                nodeList[i] =PathFinderDemo::getInstance()->getPath().at(i);
            }
            drawNode->setLineWidth(10);
            drawNode->drawPoly(nodeList, PathFinderDemo::getInstance()->getPath().size(), false, Color4F(0.5f, 0.0f, 1.0f, 1.0f));
            _movePathLayer->addChild(drawNode);
        }
    }
}

//override
void DemoScene::onEnter() {
    Layer::onEnter();
    registerEvent();
}

void DemoScene::onExit() {
    Layer::onExit();
    unRegisterEvent();
}
