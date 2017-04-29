//
//  LevelEnterDialog.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/14/17.
//
//

#include "LevelEnterDialog.h"
#include "GameManager.h"
#include "utils/SoundManager.h"
#include "constants.h"
#include "SceneFactory.h"

USING_NS_CC;
using namespace ui;



LevelEnterDialog::~LevelEnterDialog() {
    
}
LevelEnterDialog* LevelEnterDialog::create(int level) {
    auto levelDialog = new LevelEnterDialog();
    if (level && levelDialog->init(level) ) {
        levelDialog->autorelease();
        return levelDialog;
    }
    CC_SAFE_DELETE(levelDialog);
    return nullptr;
}

bool LevelEnterDialog::init(int level) {
    if (!Layer::init()) {
        return false;
    }
    _level = level;
    auto pListener = EventListenerTouchOneByOne::create();
    pListener->setSwallowTouches(true);
    pListener->onTouchBegan = [](Touch *pTouch, Event *pEvent){return true;};
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    setScale(0.8f);

    ValueMap mission = GameManager::getInstance()->getMission(level);
    

    int mt = mission.at("missiontype").asInt();
    
    ValueMap mainDic = GameManager::getInstance()->getMissionMain(mt);;
    
    
    std::string des = mainDic.at("describe").asString();
    

    Sprite *missionBg = Sprite::create(LEVEL_ENTER_BG);
    missionBg->setPosition(Vec2(visibleSize.width * 0.5,visibleSize.height * 0.5));
    addChild(missionBg);
    

    Label* lb = Label::createWithTTF(StringUtils::format("Level %d",level).c_str(),FONT_ARIAL,40);
    lb->setPosition(Vec2(missionBg->getContentSize().width*0.5,missionBg->getContentSize().height*0.9));
    missionBg->addChild(lb);
    
    _closeBtn = Button::create(CLOSEX_BUTTON,"","",TextureResType::LOCAL);
    _closeBtn->setPosition(Vec2(missionBg->getContentSize().width*0.88,missionBg->getContentSize().height*0.9));
    _closeBtn->addTouchEventListener(CC_CALLBACK_2(LevelEnterDialog::onButtonTouch,this));

    missionBg->addChild(_closeBtn,3);
    
    
    
    _rankBtn = Button::create(LEVEL_ENTER_RANKBTN,"","",TextureResType::PLIST);
    _rankBtn->setPosition(Vec2(missionBg->getContentSize().width*0.15,missionBg->getContentSize().height*0.14));
    _rankBtn->addTouchEventListener(CC_CALLBACK_2(LevelEnterDialog::onButtonTouch,this));
    missionBg->addChild(_rankBtn);
    
    _goBtn = Button::create(LEVEL_ENTER_GO,"","",TextureResType::PLIST);
    _goBtn->setPosition(Vec2(missionBg->getContentSize().width*0.5,missionBg->getContentSize().height*0.14));
    _goBtn->addTouchEventListener(CC_CALLBACK_2(LevelEnterDialog::onButtonTouch,this));

    missionBg->addChild(_goBtn,3);
    

    Label *mainMission = Label::createWithTTF(des.c_str(),FONT_ARIAL,40);
    mainMission->setColor(Color3B::YELLOW);
    mainMission->setAnchorPoint(Vec2(0,0));
    mainMission->setPosition(Vec2(missionBg->getContentSize().width*0.1,missionBg->getContentSize().height*0.65));
    missionBg->addChild(mainMission,3);
    
 
    for(int i = 1;i<=3;i++){
        int temp = mission.at(StringUtils::format("mission%d",i).c_str()).asInt();
        ValueMap additionalDic = GameManager::getInstance()->getMissionAdditional(temp);
        std::string des = additionalDic.at("describe").asString();
        
        Label *additionalMission = Label::createWithTTF(StringUtils::format("%d.%s",i,des.c_str()).c_str(),FONT_ARIAL,40);
        additionalMission->setColor(Color3B::YELLOW);
        additionalMission->setPosition(Vec2(missionBg->getContentSize().width * 0.1,missionBg->getContentSize().height * (0.45-(i-1)*0.1)));
        additionalMission->setAnchorPoint(Vec2(0,0));
        missionBg->addChild(additionalMission,3);
    }
  
    return true;
}

void LevelEnterDialog::registerEvent() {
    
}

void LevelEnterDialog::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);}

void LevelEnterDialog::onEnter() {
    Layer::onEnter();
    registerEvent();
    this->setScale(0);
    
    Action* popupLayer = Sequence::create(
                                          ScaleTo::create(0.2f, 1.1f),
                                          ScaleTo::create(0.1f, 0.75f), NULL);
    this->runAction(popupLayer);
}

void LevelEnterDialog::onExit() {
    Layer::onExit();
    unRegisterEvent();
}

void LevelEnterDialog::onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }
    SoundManager::getInstance()->playEffect("res/sound/sfx_click.mp3");
    if (sender == _goBtn) {
        __NotificationCenter::getInstance()->postNotification("enterGame",(Ref*)&_level);
       
    } else if (sender == _closeBtn) {
        removeFromParent();
    } else if(sender == _rankBtn) {
    }
}
