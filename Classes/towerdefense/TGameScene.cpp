//
//  TGameScene.cpp
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#include "TGameScene.h"
USING_NS_CC;
using namespace ui;
cocos2d::Scene* TGameScene::createScene() {
    auto scene = Scene::create();
    
    auto layer = TGameScene::create();
    
    scene->addChild(layer);
    
    return scene;
}
//constructor
TGameScene::TGameScene() {
    
}

TGameScene::~TGameScene() {
    
}

//init
bool TGameScene::init() {
    if (!Layer::init()) {
        return false;
    }
    return true;
    
}
//normal

void TGameScene::registerEvent() {
    
}

void TGameScene::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void TGameScene::onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }
}

void TGameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event) {
    
}

//override
void TGameScene::onEnter() {
    Layer::onEnter();
    registerEvent();
}

void TGameScene::onExit() {
    Layer::onExit();
    unRegisterEvent();
}
