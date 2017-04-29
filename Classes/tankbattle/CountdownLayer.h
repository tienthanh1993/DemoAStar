//
//  CountdownLayer.h
//  TankBattle
//
//  Created by Pham Thanh on 3/16/17.
//
//

#ifndef CountdownLayer_h
#define CountdownLayer_h

#include <cocos2d.h>

class CountdownLayer : public cocos2d::Layer {
public:
    // creator
    CREATE_FUNC(CountdownLayer);
    
    // constructor
    CountdownLayer();
    virtual ~CountdownLayer();
    
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

#endif /* CountdownLayer_h */
