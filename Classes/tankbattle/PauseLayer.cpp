//
//  PauseLayer.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/18/17.
//
//

#include "PauseLayer.h"
#include "SceneFactory.h"
#include "constants.h"
#include "utils/SoundManager.h"

USING_NS_CC;
using namespace ui;

PauseLayer::PauseLayer() {
    
}

PauseLayer::~PauseLayer() {
    
}

bool PauseLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    
    auto pListener = EventListenerTouchOneByOne::create();
    pListener->setSwallowTouches(true);
    pListener->onTouchBegan = [](Touch *pTouch, Event *pEvent){return true;};
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
  
    _background = Sprite::create(GAMEEND_PAUSE_BG);
    _background->setPosition(Vec2(visibleSize.width * 0.5,visibleSize.height * 0.5));
    addChild(_background);
    
    _continueBtn = Button::create(GAMEEND_CONTINUE,"","",TextureResType::PLIST);
    _continueBtn->setPosition(Vec2(_background->getContentSize().width/2,_background->getContentSize().height/2 + _continueBtn->getContentSize().height + 10));
    _continueBtn->addTouchEventListener([&](Ref* sender,Widget::TouchEventType touchType) {
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
       this->removeFromParent();
       
    });
    _background->addChild(_continueBtn);
    _retryBtn = Button::create(GAMEEND_RETRY,"","",TextureResType::PLIST);
    _retryBtn->setPosition(_background->getContentSize()/2);
    _retryBtn->addTouchEventListener([&](Ref* sender,Widget::TouchEventType touchType) {
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
        int lifeLeft = UserDefault::getInstance()->getIntegerForKey("lifeLeft",0);
        if (lifeLeft > 0) {
            UserDefault::getInstance()->setIntegerForKey("lifeLeft",lifeLeft-1);
            Director::getInstance()->replaceScene(SceneFactory::create(game_scene));
        } else {
            Director::getInstance()->replaceScene(SceneFactory::create(level_scene));
        }
        
        
    });
    _background->addChild(_retryBtn);
    _returnBtn = Button::create(GAMEEND_RETURN,"","",TextureResType::PLIST);
    _returnBtn->setPosition(Vec2(_background->getContentSize().width/2,_background->getContentSize().height/2 - _continueBtn->getContentSize().height - 10));
    _returnBtn->addTouchEventListener([&](Ref* sender,Widget::TouchEventType touchType) {
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
        Director::getInstance()->replaceScene(SceneFactory::create(level_scene));
        
    });
    _background->addChild(_returnBtn);
        return true;
}

void PauseLayer::registerEvent() {
    
}

void PauseLayer::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);}

void PauseLayer::onEnter() {
    Layer::onEnter();
    registerEvent();
//    this->setScale(0);
//    
//    Action* popupLayer = Sequence::create(
//                                          ScaleTo::create(0.2f, 1.1f),
//                                          ScaleTo::create(0.1f, 1.0f), NULL);
//    this->runAction(popupLayer);
    Director::getInstance()->pause();
    SoundManager::getInstance()->pause();
}

void PauseLayer::onExit() {
    Layer::onExit();
    unRegisterEvent();
    Director::getInstance()->resume();
    SoundManager::getInstance()->resume();
    
}

