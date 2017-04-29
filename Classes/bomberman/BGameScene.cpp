//
//  BGameScene.cpp
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#include "BGameScene.h"
USING_NS_CC;
using namespace ui;
cocos2d::Scene* BGameScene::createScene() {
    auto scene = Scene::create();
    
    auto layer = BGameScene::create();
    
    scene->addChild(layer);
    
    return scene;
}
//constructor
BGameScene::BGameScene() {
    
}

BGameScene::~BGameScene() {
    
}

//init
bool BGameScene::init() {
    if (!Layer::init()) {
        return false;
    }
    return true;
    
}
//normal

void BGameScene::registerEvent() {
    
}

void BGameScene::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void BGameScene::onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }
}

void BGameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event) {
    
}

//override
void BGameScene::onEnter() {
    Layer::onEnter();
    registerEvent();
}

void BGameScene::onExit() {
    Layer::onExit();
    unRegisterEvent();
}
