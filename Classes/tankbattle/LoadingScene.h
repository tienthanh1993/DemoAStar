//
//  LoadingScene.h
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#ifndef LoadingScene_h
#define LoadingScene_h

#include <cocos2d.h>
#include <ui/CocosGui.h>
class LoadingScene : public cocos2d::Layer {
public:
    //creator
    static cocos2d::Scene* createScene();
    CREATE_FUNC(LoadingScene);
    
    //constructor
    LoadingScene();
    ~LoadingScene();
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
    
    void onResourcesLoaded(cocos2d::Texture2D* texture);
protected:
    //protected
private:
    //private
    cocos2d::Sprite* _background;
    cocos2d::Label* _loadingProgress;
    int _totalResourcesLoaded = 0;
};

#endif /* LoadingScene_h */
