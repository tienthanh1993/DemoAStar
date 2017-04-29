//
//  AnimationHelper.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/8/17.
//
//

#include "AnimationHelper.h"
#include "constants.h"
USING_NS_CC;

void AnimationHelper::addAnimation(const char* name,const char* plist,const char* picName,int arrNum){

   SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist);
    Vector<SpriteFrame*> animaFrames(arrNum);
    
    char str[100] = {0};
    
    for(int i=1;i<=arrNum;i++){
        sprintf(str,"%s%i.png",picName,i);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        animaFrames.pushBack(frame);
    }
    Animation* animation = Animation::createWithSpriteFrames(animaFrames, 0.016f);
  
    AnimationCache::getInstance()->addAnimation(animation,name);
 
}

cocos2d::Animation* AnimationHelper::getAnimation(char* name) {
  return AnimationCache::getInstance()->getAnimation(name);
}

cocos2d::Sprite* AnimationHelper::createEffect(char* name,float duration,Vec2 position) {
    auto spr = Sprite::create(ANI_FIRST);
    auto ani = Animate::create(getAnimation(name));
    ani->setDuration(duration);
    spr->runAction(Sequence::create(ani, RemoveSelf::create(true),nullptr));
    return spr;
}
