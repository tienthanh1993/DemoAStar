//
//  GameLose.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/16/17.
//
//

#include "GameLose.h"
#include "SceneFactory.h"
#include "constants.h"
#include "utils/SoundManager.h"
USING_NS_CC;
using namespace ui;

GameLose::GameLose() {
    
}

GameLose::~GameLose() {
    
}

bool GameLose::init() {
    if (!Layer::init()) {
        return false;
    }
        Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _background = Sprite::create(GAMEEND_BG1);
    _background->setPosition(Vec2(visibleSize.width * 0.5,visibleSize.height * 0.5));
    addChild(_background,3);
    
    _shopBtn =   Button::create(MENU_SHOP,"","",TextureResType::PLIST);
    _shopBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType touchType) {
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
        Director::getInstance()->replaceScene(SceneFactory::create(level_scene));
    });
    _shopBtn->setPosition(Vec2(_background->getContentSize().width * 0.87,_background->getContentSize().height * 0.5));
    _background->addChild(_shopBtn,4);
    
   _returnBtn = Button::create(GAMEEND_RETURN,"","",TextureResType::PLIST);
    _returnBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType touchType) {
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
        Director::getInstance()->replaceScene(SceneFactory::create(level_scene));
    });
    
   _retryBtn = Button::create(GAMEEND_RESTART,"","",TextureResType::PLIST);
   _retryBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType touchType) {
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
       //removeFromParentAndCleanup(true);
        Director::getInstance()->replaceScene(SceneFactory::create(game_scene));
       
    });
    _returnBtn->setPosition(Vec2(_background->getContentSize().width * 0.5 - _returnBtn->getContentSize().width/2 - 25 ,_background->getContentSize().height * 0.23));
    _retryBtn->setPosition(Vec2(_background->getContentSize().width * 0.5 + _retryBtn->getContentSize().width/2 + 25,_background->getContentSize().height * 0.23));
    

    _background->addChild(_returnBtn);
    _background->addChild(_retryBtn);
    return true;
}

void GameLose::registerEvent() {
    
}

void GameLose::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);}

void GameLose::onEnter() {
    Layer::onEnter();
    registerEvent();
     SoundManager::getInstance()->playEffect("res/sound/sfx_defeat.mp3");
    this->setScale(0);
    
    Action* popupLayer = Sequence::create(
                                          ScaleTo::create(0.2f, 1.1f),
                                          ScaleTo::create(0.1f,  0.8f), NULL);
    this->runAction(popupLayer);
    

}

void GameLose::onExit() {
    Layer::onExit();
    unRegisterEvent();
}

