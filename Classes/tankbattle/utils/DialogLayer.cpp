//
//  DialogLayer.cpp
//  PopoRun
//
//  Created by Pham Thanh on 10/26/16.
//
//

#include "DialogLayer.h"
#include "constants.h"

USING_NS_CC;
using namespace ui;
#define DIALOG_PANEL "res/ui/dialog/small_bg.png"
#define DIALOG_OK_BTN "res/ui/dialog/accept_btn.png"
#define DIALOG_CANCEL_BTN "res/ui/dialog/cancel_btn.png"

DialogLayer* DialogLayer::create(std::string txt) {
    DialogLayer *pRet = new(std::nothrow) DialogLayer();
    if (pRet && pRet->init())
    {
        pRet->setTxt(txt);
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

DialogLayer::DialogLayer() {
    
}

DialogLayer::~DialogLayer() {
    
}

bool DialogLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    auto pListener = EventListenerTouchOneByOne::create();
    
    pListener->setSwallowTouches(true);
    pListener->onTouchBegan = [](Touch *pTouch, Event *pEvent){return true;};
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
    Size visibleSize = Director::getInstance()->getWinSize();

    this->setPosition(Vec2(0.5f*visibleSize.width,0.5f*visibleSize.height));
//    _shadow = LayerColor::create(Color4B::WHITE);
//    _shadow->setOpacity(80);
//    _shadow->setContentSize(visibleSize);
//    _shadow->setPosition(Vec2(-visibleSize.width/2,-visibleSize.height/2));
 this->setScale(0);
    _background = Sprite::create(DIALOG_PANEL);
 //   _background->setAnchorPoint(Vec2());
    //_background->setPosition(Vec2(-_background->getContentSize().width*0.5f,-_background->getContentSize().height*0.5f));
    _txt = Label::createWithTTF("", "fonts/arial.ttf", 35);
    _txt->setPosition(Vec2(_background->getContentSize().width/2,_background->getContentSize().height/1.5));
    _txt->setWidth(_background->getContentSize().width*0.8);
    //_txt->setHeight(170);
    _txt->setColor(Color3B::WHITE);
    _okBtn = Button::create(DIALOG_OK_BTN,"","");
    _okBtn->setPosition(Vec2(0.5*_background->getContentSize().width,_okBtn->getContentSize().height));
    _okBtn->addTouchEventListener([&](Ref* sender,Widget::TouchEventType touchtype){ if(touchtype != Widget::TouchEventType::ENDED) return; if(_callback) { _callback(this,true); return; } this->removeFromParentAndCleanup(true);});
    _cancleBtn = Button::create(DIALOG_CANCEL_BTN,"","");
    _cancleBtn->setPosition(Vec2(0.85f*_background->getContentSize().width,_background->getContentSize().height*0.85f));
    _cancleBtn->addTouchEventListener([&](Ref* sender,Widget::TouchEventType touchtype){ if(touchtype != Widget::TouchEventType::ENDED) return; if(_callback) { _callback(this,false); return;} this->removeFromParentAndCleanup(true);});
    
    return true;
}

void DialogLayer::setCallback(std::function<void (Node *, bool)> callback) {
    _callback = callback;
}

void DialogLayer::setTxt(std::string txt) {
    _txt->setString(txt);
}

void DialogLayer::onEnter() {
    Layer::onEnter();
    //addChild(_shadow);
    addChild(_background);
    _background->addChild(_txt);
    _background->addChild(_okBtn);
    _background->addChild(_cancleBtn);
   
    
    Action* popupLayer = Sequence::create(
                                          ScaleTo::create(0.2f, 1.1f),
                                          ScaleTo::create(0.1f, RATIO), NULL);
    this->runAction(popupLayer);
}

void DialogLayer::onExit() {

    
    Layer::onExit();

}
