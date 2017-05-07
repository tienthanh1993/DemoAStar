//
//  HUDLayer.cpp
//  TankBattle
//
//  Created by Pham Thanh on 4/8/17.
//
//

#include "HUDLayer.h"
#include "constants.h"
#include "PauseLayer.h"
#include "GameScene.h"
USING_NS_CC;
using namespace ui;

HUDLayer::HUDLayer() {
    _gameScene = nullptr;
}

HUDLayer::~HUDLayer() {
    
}

bool HUDLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    initUI();
    return true;
}

void HUDLayer::registerEvent() {
    
}

void HUDLayer::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);}

void HUDLayer::onEnter() {
    Layer::onEnter();
    registerEvent();
  
    schedule(schedule_selector(HUDLayer::updateInfo), 1.0f);
}

void HUDLayer::onExit() {
    Layer::onExit();
    unRegisterEvent();
    unschedule(schedule_selector(HUDLayer::updateInfo));
}



void HUDLayer::initUI() {
    
    Size _visibleSize = Director::getInstance()->getVisibleSize();
    
    //init control
    _joyStick = Joystick::create(ROCKER_BG, 178, ROCKER, 178);
    _joyStick->setPosition(Vec2(150,150));
    _joyStick->setHandleEventListener(JOYSTICK_CALLBACK(HUDLayer::onJoyStickEvent,this));
    
    this->addChild(_joyStick,8);
    _joyStick->setOpacity(100);
    
    _fireBtn = Button::create(ROCKER_FIRE,"","",TextureResType::PLIST);
    _fireBtn->setPosition(Vec2(_visibleSize.width - 150,150));
    
    _fireBtn->addTouchEventListener(CC_CALLBACK_2(HUDLayer::onFireTouch,this));
    _fireBtn->setOpacity(100);
    _fireBtn->setScale(2.0f);
    addChild(_fireBtn,8);
    
    
    _pauseBtn = Button::create(GAME_PAUSE,"","",TextureResType::PLIST);
    _pauseBtn->setPosition(Vec2(_visibleSize.width * 0.05,_visibleSize.height * 0.94));
    _pauseBtn->addTouchEventListener([&](Ref* sender,Widget::TouchEventType touchType){
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
        Button* btn = (Button*)sender;
        auto parrent = btn->getParent();
        auto pauseLayer = PauseLayer::create();
        parrent->addChild(pauseLayer,100);
        
        
    });
    addChild(_pauseBtn,100);
    
    _moneyBg = Sprite::create(UI_GOLD_LABEL);
    _moneyBg->setPosition(Vec2(_visibleSize.width*0.18,_visibleSize.height*0.95));
    this->addChild(_moneyBg,100);
    
    
    _money = UserDefault::getInstance()->getIntegerForKey("money",0);
    _moneyLabel = Label::createWithBMFont("fonts/Matura.fnt",StringUtils::format("%d",_money).c_str());
    _moneyLabel->setPosition(Vec2(_visibleSize.width*0.30,_visibleSize.height*0.95));
    addChild(_moneyLabel,100);
    
    _enemyIcon = Sprite::create(UI_TANK_LEFT);
    _enemyIcon->setPosition(Vec2(_visibleSize.width * 0.66,_visibleSize.height * 0.95));
    addChild(_enemyIcon,100);

    _timerIcon = Sprite::create(UI_LEVEL_TIME);
    _timerIcon->setPosition(Vec2(_visibleSize.width*0.42,_visibleSize.height*0.95));
    addChild(_timerIcon);
    
    
    _timerLabel= Label::createWithBMFont("fonts/Matura.fnt",StringUtils::format("%02d:%02d",0,0).c_str());
    _timerLabel->setPosition(Vec2(_visibleSize.width*0.526,_visibleSize.height*0.95));
    addChild(_timerLabel,100);
    
    _enemyLabel = Label::createWithBMFont("fonts/Matura.fnt",StringUtils::format("%d/%d",0 , 0).c_str());
    _enemyLabel->setPosition(Vec2(_visibleSize.width * 0.77,_visibleSize.height * 0.95));
    this->addChild(_enemyLabel,100);
    
    
}



void HUDLayer::onFireTouch(Ref *sender, Widget::TouchEventType touchtype) {
    if (!_gameScene) {
        return;
    }
    if (touchtype == Widget::TouchEventType::BEGAN) {
        if(((GameScene*)_gameScene)->getTankPlayer()->getTarget()) {
            if(((GameScene*)_gameScene)->getTankPlayer()->getTarget()->isVisible()) {
                float angle = getAngleOfTwoVectors(((GameScene*)_gameScene)->getTankPlayer()->getPosition(),((GameScene*)_gameScene)->getTankPlayer()->getTarget()->getPosition());
                ((GameScene*)_gameScene)->getTankPlayer()->sendCommand(do_shoot,-1,0,angle);
            } else {
                ((GameScene*)_gameScene)->getTankPlayer()->setTarget(nullptr);
            }
        } else {
            ((GameScene*)_gameScene)->getTankPlayer()->sendCommand(do_shoot, -1, 0, ((GameScene*)_gameScene)->getTankPlayer()->getDirection());
        }
    }
}

void HUDLayer::onJoyStickEvent(float interval,float x , float y,JoystickEventType type) {
    if (!_gameScene) {
        return;
    }
    if (type == JoystickEventType::JET_TOUCH_BEGIN || type == JoystickEventType::JET_TOUCH_MOVE) {
        Point pos1 = Point::ZERO;
        Point pos2 = Point(x,y);
        Point pos = pos2 - pos1;
        int degree = CC_RADIANS_TO_DEGREES(pos.getAngle());
        if (degree < 0) {
            degree = 180 + (180 - (-degree));
        }
        if (degree <= 45 || degree >= 315) {
            degree = 0;
        } else if (degree > 45 && degree <= 135) {
            degree = 90;
        } else if (degree > 135 && degree <= 225) {
            degree = 180;
        } else if (degree > 225 && degree < 315) {
            degree = 270;
        }
        if (((GameScene*)_gameScene)->getTankPlayer()->getDoing() == do_move && ((GameScene*)_gameScene)->getTankPlayer()->getDirection() == degree) {
            return;
        }
        ((GameScene*)_gameScene)->getTankPlayer()->sendCommand(do_move,-1,0,degree);
    } else {
        ((GameScene*)_gameScene)->getTankPlayer()->sendCommand(do_stand);
    }
}

void HUDLayer::updateInfo(float dt) {
    if (!_gameScene) {
        return;
    }
    
    if (((GameScene*)_gameScene)->getTankPlayer()) {
        Tank* tank = ((GameScene*)_gameScene)->getTankPlayer();
        if (tank->getAttack() > tank->getBaseAttack()) {
            
        }
        if (tank->getBaseSpeed() > tank->getBaseSpeed()) {
            
        }
        if (tank->getBaseDefense() > tank->getBaseDefense()) {
            
        }
    }
    
    getTimerLabel()->setString(StringUtils::format("%02lld:%02lld",((GameScene*)_gameScene)->getGameTime()/60/60,(((GameScene*)_gameScene)->getGameTime()/60)%60));
    
     getEnemyLabel()->setString(StringUtils::format("%d/%d",((GameScene*)_gameScene)->getTotalEnemy()-((GameScene*)_gameScene)->getkilledEnemy(),((GameScene*)_gameScene)->getTotalEnemy()));
    
    getMoneyLabel()->setString(StringUtils::format("%d",((GameScene*)_gameScene)->getMoney()));
    
}
