//
//  HUDLayer.h
//  TankBattle
//
//  Created by Pham Thanh on 4/8/17.
//
//

#ifndef HUDLayer_h
#define HUDLayer_h

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include "Joystick.h"

class HUDLayer : public cocos2d::Layer {
public:
    // creator
    CREATE_FUNC(HUDLayer);
    
    // constructor
    HUDLayer();
    virtual ~HUDLayer();
    
    //init
    virtual bool init();
    void initUI();
    
    //normal
    void registerEvent();
    void unRegisterEvent();
    
    //override
    void onEnter();
    void onExit();
    void onFireTouch(Ref *sender, cocos2d::ui::Widget::TouchEventType touchtype);
    void onJoyStickEvent(float interval,float x , float y,JoystickEventType type);
    
    
    
    void updateInfo(float dt);
protected:
    
    
private:
    // private func
    CC_SYNTHESIZE(cocos2d::ui::Button*, _fireBtn, FireBtn);
    CC_SYNTHESIZE(Joystick*, _joyStick, JoyStick);
    
    CC_SYNTHESIZE(cocos2d::ui::Button*, _pauseBtn, PauseBtn);
    CC_SYNTHESIZE(cocos2d::Sprite*, _moneyBg, MoneyBg);
    CC_SYNTHESIZE(cocos2d::Label*, _moneyLabel, MoneyLabel);
    CC_SYNTHESIZE(cocos2d::Sprite*, _enemyIcon, EnemyIcon);
    CC_SYNTHESIZE(cocos2d::Label*, _enemyLabel, EnemyLabel);
    CC_SYNTHESIZE(cocos2d::Sprite*, _timerIcon, TimerIcon);
    CC_SYNTHESIZE(cocos2d::Label*, _timerLabel, TimerLabel);
    CC_SYNTHESIZE(int, _money, Money);
    
    CC_SYNTHESIZE(cocos2d::Layer*, _gameScene, NewGameScene);
    
    // private variable
};

#endif /* HUDLayer_h */
