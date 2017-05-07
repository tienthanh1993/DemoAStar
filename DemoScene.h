//
//  DemoScene.h
//  TankBattle
//
//  Created by Pham Thanh on 4/6/17.
//
//

#ifndef DemoScene_h
#define DemoScene_h

#include <cocos2d.h>
#include <ui/CocosGui.h>
#include "TankMap.h"
class DemoScene : public cocos2d::Layer {
public:
    //creator
    static cocos2d::Scene* createScene();
    CREATE_FUNC(DemoScene);
    
    //constructor
    DemoScene();
    ~DemoScene();
    //init
    virtual bool init();
    //normal
    
    void mainLoop(float dt);
    
    void registerEvent();
    
    void unRegisterEvent();
    
    void onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype);
    
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event);
    
    //override
    void onEnter();
    void onExit();
protected:
    //protected
private:
    int _gameLoops = 0;
    int _totalStep = 0;
    int _curStep = 0;
    //private
     CC_SYNTHESIZE(TankMap*,_tankMap,TankMap);
    cocos2d::Layer* _movePathLayer;
    cocos2d::Layer* _stepLayer;
};

#endif /* DemoScene_h */
