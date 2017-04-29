//
//  AnimationHelper.h
//  TankBattle
//
//  Created by Pham Thanh on 3/8/17.
//
//

#ifndef AnimationHelper_h
#define AnimationHelper_h

#include <cocos2d.h>
class AnimationHelper {
public:
    static void addAnimation(const char* name,const char* plist,const char* picName,int arrNum);
    static cocos2d::Animation* getAnimation(char* name);
    static cocos2d::Sprite* createEffect(char* name,float duration,cocos2d::Vec2 position);
    
};
#endif /* AnimationHelper_h */
