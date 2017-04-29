//
//  Bullet.h
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#ifndef Bullet_h
#define Bullet_h

#include <cocos2d.h>
class Bullet : public cocos2d::Node {
public:
    //level include damage, speed
    static Bullet* createBullet(int owner, int level);
    Bullet();
    ~Bullet();
    virtual bool init(int owner, int level);
    
    void active(float dt);
    void flyTo(int dir);
    void boom();
    
    CC_SYNTHESIZE(int, _id, Id);
    CC_SYNTHESIZE(int, _level, Level);
 
    CC_SYNTHESIZE(float, _offsetX, OffsetX);
    CC_SYNTHESIZE(float, _offsetY, OffsetY);
    
    CC_SYNTHESIZE(float, _speed, Speed);
    CC_SYNTHESIZE(float, _damage, Damage);
    CC_SYNTHESIZE(bool, _isbounded, IsBound);
    CC_SYNTHESIZE(bool, _canDesStone, CanDesStone);
    CC_SYNTHESIZE(int, _ownerID, OwnerID);
    
    CC_SYNTHESIZE(cocos2d::Sprite*, _bulletSprite, BulletSprite);
    CC_SYNTHESIZE(cocos2d::Animation*, _boomAnimation, BoomAnimation);
protected:
private:
};
#endif /* Bullet_h */
