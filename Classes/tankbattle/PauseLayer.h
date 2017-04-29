//
//  PauseLayer.h
//  TankBattle
//
//  Created by Pham Thanh on 3/18/17.
//
//

#ifndef PauseLayer_h
#define PauseLayer_h

#include <cocos2d.h>
#include <ui/CocosGUI.h>
class PauseLayer : public cocos2d::Layer {
public:
    // creator
    CREATE_FUNC(PauseLayer);
    
    // constructor
    PauseLayer();
    virtual ~PauseLayer();
    
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
    
    // private variable
    cocos2d::Sprite* _background;
    cocos2d::ui::Button* _continueBtn;
    cocos2d::ui::Button* _retryBtn;
    cocos2d::ui::Button* _returnBtn;
};

#endif /* PauseLayer_h */
