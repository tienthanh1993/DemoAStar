//
//  GameLose.h
//  TankBattle
//
//  Created by Pham Thanh on 3/16/17.
//
//

#ifndef GameLose_h
#define GameLose_h

#include <cocos2d.h>
#include <ui/CocosGUI.h>
class GameLose : public cocos2d::Layer {
public:
    // creator
    CREATE_FUNC(GameLose);
    
    // constructor
    GameLose();
    virtual ~GameLose();
    
    //init
    virtual bool init();
    
    //normal
    void registerEvent();
    void unRegisterEvent();
    
    //override
    void onEnter();
    void onExit();
    
protected:
    
    
private:
    // private func
    
    // private variable'
    cocos2d::Sprite* _background;
    cocos2d::ui::Button* _shopBtn;
    cocos2d::ui::Button* _returnBtn;
    cocos2d::ui::Button* _retryBtn;
    
};

#endif /* GameLose_h */
