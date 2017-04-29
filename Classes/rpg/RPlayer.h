//
//  RPlayer.h
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#ifndef RPlayer_h
#define RPlayer_h

#include <cocos2d.h>

class RPlayer : public cocos2d::Node {
public:
    RPlayer();
    virtual ~RPlayer();
    static RPlayer* createPlayer();
    virtual bool init();
    void active(float dt);
    void setAni(int kind,float angle);
    cocos2d::Animate* getAnimateByName(std::string animName,float delay,int animNum);
private:
    CC_SYNTHESIZE(cocos2d::Vec2, _desPos, DesPos);
    CC_SYNTHESIZE(std::deque<cocos2d::Vec2>*, _runPath, RunPath);
    cocos2d::Sprite* _spr = nullptr;
};

#endif /* RPlayer_h */
