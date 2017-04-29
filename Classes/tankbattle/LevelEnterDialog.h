//
//  LevelEnterDialog.h
//  TankBattle
//
//  Created by Pham Thanh on 3/14/17.
//
//

#ifndef LevelEnterDialog_h
#define LevelEnterDialog_h

#include <cocos2d.h>
#include <ui/CocosGui.h>

class LevelEnterDialog : public cocos2d::Layer {
public:
    // creator

    
    static LevelEnterDialog* create(int level);
    // constructor
    virtual ~LevelEnterDialog();
    
    //init
    virtual bool init(int level);
    
    //normal
    void registerEvent();
    void unRegisterEvent();
    
    //override
    void onEnter();
    void onExit();
    
protected:
    
    
private:
    // private func
   void onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype);
    // private variable
    int _level;
    cocos2d::ui::Button* _closeBtn;
    cocos2d::ui::Button* _goBtn;
    cocos2d::ui::Button* _rankBtn;
};

#endif /* LevelEnterDialog_h */
