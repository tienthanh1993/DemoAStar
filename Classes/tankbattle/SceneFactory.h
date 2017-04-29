//
//  SceneFactory.h
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#ifndef SceneFactory_h
#define SceneFactory_h

#include <cocos2d.h>

class SceneFactory {
public:
    static cocos2d::Scene* create(int sceneType);
    
};

#endif /* SceneFactory_h */
