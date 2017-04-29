//
//  BGameScene.h
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#ifndef BGameScene_h
#define BGameScene_h

#include <cocos2d.h>
#include <ui/CocosGui.h>
class BGameScene : public cocos2d::Layer {
public:
    //creator
    static cocos2d::Scene* createScene();
    CREATE_FUNC(BGameScene);
    
    //constructor
    BGameScene();
    ~BGameScene();
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
protected:
    //protected
private:
    //private
};

#endif /* BGameScene_h */
