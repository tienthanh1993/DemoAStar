//
//  RGameScene.h
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#ifndef RGameScene_h
#define RGameScene_h

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include "RPlayer.h"

class RGameScene : public cocos2d::Layer {
public:
    //creator
    static cocos2d::Scene* createScene();
    CREATE_FUNC(RGameScene);
    
    //constructor
    RGameScene();
    ~RGameScene();
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
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void mainLoop(float dt);
    
    
    
    void setViewPointCenter(cocos2d::Point position);
protected:
    //protected
private:
    //private
    CC_SYNTHESIZE(int, _mapWidth, MapWidth);
    CC_SYNTHESIZE(int, _mapHeight, MapHeight);
    
    cocos2d::TMXTiledMap* _map;
    RPlayer* _player;
};

#endif /* RGameScene_h */
