//
//  CountdownLayer.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/16/17.
//
//

#include "CountdownLayer.h"
#include "constants.h"

USING_NS_CC;


CountdownLayer::CountdownLayer() {
    
}

CountdownLayer::~CountdownLayer() {
    
}

bool CountdownLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/ui/countdown.plist");
    auto pListener = EventListenerTouchOneByOne::create();
    pListener->setSwallowTouches(true);
    pListener->onTouchBegan = [](Touch *pTouch, Event *pEvent){return true;};
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
 
    Size size = Director::getInstance()->getWinSize();
    
    Sprite *bgNum = Sprite::createWithSpriteFrameName(COUNTDOWN_BG);
    bgNum->setPosition(Vec2(size.width/2,size.height/2));
    this->addChild(bgNum,15,111+1);
    
    Sprite *labelNum1 = Sprite::createWithSpriteFrameName(COUNTDOWN_NUM1);
    labelNum1->setPosition(Vec2(size.width/2,size.height/2));
    labelNum1->setVisible(false);
    this->addChild(labelNum1,15);
    
    Sprite *labelNum2 = Sprite::createWithSpriteFrameName(COUNTDOWN_NUM2);
    labelNum2->setPosition(Vec2(size.width/2,size.height/2));
    labelNum2->setVisible(false);
    this->addChild(labelNum2,15);
    
    Sprite *labelNum3 = Sprite::createWithSpriteFrameName(COUNTDOWN_NUM3);
    labelNum3->setPosition(Vec2(size.width/2,size.height/2));
    this->addChild(labelNum3,15);
    
    Sprite *labelNum4 = Sprite::createWithSpriteFrameName(COUNTDOWN_GO);
    labelNum4->setPosition(Vec2(size.width/2,size.height/2));
    labelNum4->setVisible(false);
    this->addChild(labelNum4,15);
    
    ScaleBy *scale1 = ScaleBy::create(0.5f,2.0f);
    ScaleBy *scale2 = ScaleBy::create(0.3f,0.0f);
    RotateBy *rotate1 = RotateBy::create(0.16f,0,360);
    
    DelayTime *delay1 = DelayTime::create(0.5f);
    DelayTime *delay2 = DelayTime::create(1.0f);
    DelayTime *delay3 = DelayTime::create(1.5f);
    DelayTime *delay4 = DelayTime::create(2.0f);
    DelayTime *delay5 = DelayTime::create(0.3f);
    DelayTime *delay6 = DelayTime::create(2.3f);

   auto callback  = CallFuncN::create([&](Node *sender ){
       __NotificationCenter::getInstance()->postNotification("onGameStart");
        this->removeFromParent();
    });
    
    labelNum3->runAction(Sequence::create(delay1,Show::create(),scale1,Hide::create(),NULL));
    labelNum2->runAction(Sequence::create(delay2,Show::create(),scale1,Hide::create(),NULL));
    labelNum1->runAction(Sequence::create(delay3,Show::create(),scale1,Hide::create(),NULL));
    bgNum->runAction(Sequence::create(delay6,scale2,Hide::create(),NULL));
    labelNum4->runAction(Sequence::create(delay4,Show::create(),rotate1,scale1,scale2,delay5,Hide::create(),callback,NULL));
    
    
    return true;
}

void CountdownLayer::registerEvent() {
    
}

void CountdownLayer::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);}

void CountdownLayer::onEnter() {
    Layer::onEnter();
    registerEvent();
}

void CountdownLayer::onExit() {
    Layer::onExit();
    unRegisterEvent();
}

