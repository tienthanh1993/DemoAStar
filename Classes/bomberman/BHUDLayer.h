//
//  BHUDLayer.h
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#ifndef BHUDLayer_h
#define BHUDLayer_h

#include <cocos2d.h>

class BHUDLayer : public cocos2d::Layer {
public:
    // creator
    CREATE_FUNC(BHUDLayer);
    
    // constructor
    BHUDLayer();
    virtual ~BHUDLayer();
    
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
};

#endif /* BHUDLayer_h */
