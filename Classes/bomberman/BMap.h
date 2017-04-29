//
//  BMap.h
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#ifndef BMap_h
#define BMap_h

#include <cocos2d.h>

class BMap : public cocos2d::Layer {
public:
    // creator
    CREATE_FUNC(BMap);
    
    // constructor
    BMap();
    virtual ~BMap();
    
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

#endif /* BMap_h */
