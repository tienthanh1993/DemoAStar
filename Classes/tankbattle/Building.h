//
//  Building.h
//  TankBattle
//
//  Created by Pham Thanh on 4/1/17.
//
//

#ifndef Building_h
#define Building_h

#include <cocos2d.h>
class Building : public cocos2d::Node {
public:
    static Building* create(int level);
    virtual bool init(int level);
    
    Building();
    virtual ~Building();
    
    void receiveDamage(float damage);
private:
    CC_SYNTHESIZE(int, _id, Id);
    CC_SYNTHESIZE(int, _life, Life);
    CC_SYNTHESIZE(int, _lifeMax, LifeMax);
    CC_SYNTHESIZE(cocos2d::Sprite*, _build, Build);
    CC_SYNTHESIZE(cocos2d::Sprite*, _buildLight, BuildLight);
    
   CC_SYNTHESIZE(cocos2d::ProgressTimer*, _lifeBar, LifeBar); //
    
};
#endif /* Building_h */
