//
//  LevelScene.h
//  TankBattle
//
//  Created by Pham Thanh on 3/14/17.
//
//

#ifndef LevelScene_h
#define LevelScene_h

#include <cocos2d.h>
#include <ui/CocosGui.h>
#include "GameScene.h"

class LevelScene : public cocos2d::Layer {
public:
    //creator
    static cocos2d::Scene* createScene();
    CREATE_FUNC(LevelScene);
    
    //constructor
   LevelScene();
    ~LevelScene();
    //init
    virtual bool init();
    //normal
    
    void registerEvent();
    
    void unRegisterEvent();
    
    void onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype);
    
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event);
    
    //override
    void onEnter();
    void onExit();
    void lifeUpdate(float dt);
protected:
    //protected
private:
    //private
    void onEnterGame(Ref* data);
    void go(float dt);
    cocos2d::Scene* _gameScene = nullptr;
    cocos2d::ui::Button* _backBtn;
    cocos2d::ui::Button* _shopBtn;
    cocos2d::Sprite* _starBg;
    cocos2d::Label* _starLabel;

    cocos2d::Sprite* _lifeTimerBg;
    cocos2d::Label* _lifeTimerLabel;
    cocos2d::Sprite* _lifeBg;
    cocos2d::Label* _lifeLabel;
    
    cocos2d::Layer* _enterDialog;
    
    CC_SYNTHESIZE(cocos2d::Sprite*, _background, Background);
    CC_SYNTHESIZE(cocos2d::ui::ScrollView*, _scrollView, ScrollView);
    CC_SYNTHESIZE(cocos2d::Layer*, _layer, Layer);
    CC_SYNTHESIZE(int, _maxPage, MaxPage);
    CC_SYNTHESIZE(int, _passedLevel, PassedLevel);
    

    cocos2d::ValueVector _chapterList;
    cocos2d::ValueVector _levelLocation;
    cocos2d::Size _pageSize;
    
    int _lifeTimeLeft = 3600;
    int _lifeLeft = 0;
    int _lifeMax = 5;
    
};

#endif /* LevelScene_h */
