//
//  GameWinLayer.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/16/17.
//
//

#include "GameWinLayer.h"
#include "SceneFactory.h"
#include "constants.h"
#include "GameManager.h"
#include "utils/SoundManager.h"
USING_NS_CC;
using namespace ui;

GameWinLayer::GameWinLayer() {
    
}

GameWinLayer::~GameWinLayer() {
    
}

bool GameWinLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _background = Sprite::create(GAMEEND_BG);
    _background->setPosition(Vec2(visibleSize.width * 0.5,visibleSize.height * 0.5));
    addChild(_background,3,77);
    
    _returnBtn = Button::create(GAMEEND_RETURN,"","",TextureResType::PLIST);
    _returnBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType touchType) {
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
        Director::getInstance()->replaceScene(SceneFactory::create(level_scene));
    });
    
    _continueBtn = Button::create(GAMEEND_CONTINUE,"","",TextureResType::PLIST);
    _continueBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType touchType) {
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
        GameManager::getInstance()->setLevel(GameManager::getInstance()->getLevel()+1);
        Director::getInstance()->replaceScene(SceneFactory::create(game_scene));
    });
    _returnBtn->setPosition(Vec2(_background->getContentSize().width * 0.5 - _returnBtn->getContentSize().width/2 - 25,_background->getContentSize().height * 0.13));
    _continueBtn->setPosition(Vec2(_background->getContentSize().width * 0.5 + _continueBtn->getContentSize().width/2 + 25,_background->getContentSize().height * 0.13));
  
    
    _background->addChild(_returnBtn,4);
    _background->addChild(_continueBtn,4);
    
    return true;
}

void GameWinLayer::registerEvent() {
    
}

void GameWinLayer::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);}

void GameWinLayer::onEnter() {
    Layer::onEnter();
    registerEvent();
     SoundManager::getInstance()->playEffect("res/sound/sfx_victory.mp3");
    this->setScale(0);
    
    Action* popupLayer = Sequence::create(
                                          ScaleTo::create(0.2f, 1.1f),
                                          ScaleTo::create(0.1f, 0.8f), NULL);
    this->runAction(popupLayer);
}

void GameWinLayer::onExit() {
    Layer::onExit();
    unRegisterEvent();
 
}

void GameWinLayer::setStar(int idx,std::string mission, bool gold) {
    if (gold) {
        _star[idx] = Sprite::create(GAMEEND_STAR_GOLD);
        _star[idx]->setPosition(Vec2(_background->getContentSize().width * 0.9,_background->getContentSize().height * (0.5 - 0.09 * idx)));
        

        _background->addChild(_star[idx],idx);
    } else {
        _star[idx] = Sprite::create(GAMEEND_STAR_GREY);
        _star[idx]->setPosition(Vec2(_background->getContentSize().width * 0.9,_background->getContentSize().height * (0.5 - 0.09 * idx)));
        _background->addChild(_star[idx],idx);
    }
    _mission[idx] = Label::createWithTTF(mission, FONT_ARIAL, 32);
    _mission[idx]->setAnchorPoint(Vec2(0,0.5f));
    _mission[idx]->setPosition(Vec2(_background->getContentSize().width * 0.1,_background->getContentSize().height * (0.5 - 0.09 * idx)));
    _background->addChild(_mission[idx]);
    
    ScaleBy *scale1=ScaleBy::create(0.1f,1.8f);
    ScaleBy *scale2=ScaleBy::create(0.1f,0.5f);
    ScaleTo *scale3 = ScaleTo::create(0.1f, 1.0f);
    _star[idx]->runAction(RepeatForever::create(Sequence::create(scale1, scale2,scale3,nullptr)));
  
}
