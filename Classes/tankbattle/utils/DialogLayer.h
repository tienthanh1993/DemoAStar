//
//  DialogLayer.h
//  PopoRun
//
//  Created by Pham Thanh on 10/26/16.
//
//

#ifndef DialogLayer_h
#define DialogLayer_h

#include <cocos2d.h>
#include <ui/CocosGUI.h>

class DialogLayer : public cocos2d::Layer
{
public:
    // creator
    static DialogLayer* create(std::string txt);
    CREATE_FUNC(DialogLayer);
    
    // contructor
    DialogLayer();
    virtual ~DialogLayer();
    
    // init method
    virtual bool init();
    
    // normal method
    void setTxt(std::string txt);
    void setCallback(std::function<void(cocos2d::Node*,bool)> callback);
    // overide method
protected:
    virtual void onEnter();
    virtual void onExit();
    
private:
    cocos2d::Sprite* _background;
    cocos2d::LayerColor* _shadow;
//    cocos2d::Sprite* _title;
    cocos2d::Label* _txt;
    cocos2d::ui::Button* _okBtn;
    cocos2d::ui::Button* _cancleBtn;
    std::function<void(cocos2d::Node*,bool)> _callback; // sender is current dialog, true is ok, false is cancle
};
#endif /* DialogLayer_h */
