//
//  GameWinLayer.h
//  TankBattle
//
//  Created by Pham Thanh on 3/16/17.
//
//

#ifndef GameWinLayer_h
#define GameWinLayer_h

#include <cocos2d.h>
#include <ui/CocosGUI.h>

class GameWinLayer : public cocos2d::Layer {
public:
    // creator
    CREATE_FUNC(GameWinLayer);
    
    // constructor
    GameWinLayer();
    virtual ~GameWinLayer();
    
    //init
    virtual bool init();
    
    //normal
    void registerEvent();
    void unRegisterEvent();
    
    //override
    void onEnter();
    void onExit();
    
    void setStar(int idx,std::string mission, bool gold);
protected:
    
    
private:
    // private func
    
    // private variable
    cocos2d::Sprite* _background;
    cocos2d::ui::Button* _returnBtn;
    cocos2d::ui::Button* _continueBtn;
    cocos2d::Label* _mission[3];
    cocos2d::Sprite* _star[3];
};

#endif /* GameWinLayer_h */
