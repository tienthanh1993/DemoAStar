//
//  Building.cpp
//  TankBattle
//
//  Created by Pham Thanh on 4/1/17.
//
//

#include "Building.h"
#include "AnimationHelper.h"
#include "GameScene.h"
#include "constants.h"

USING_NS_CC;

Building* Building::create(int level) {
    Building* build = new Building();
    if (build && build->init(level)) {
        build->autorelease();
        return build;
    }
    CC_SAFE_RELEASE(build);
    return nullptr;
}

bool Building::init(int level) {
    if (!Node::init()) {
        return false;
    }
    
    if (level<=10){
        _buildLight =  Sprite::create("res/build/desertNest_h.png");
        _build = Sprite::create("res/build/desertNest.png");
        _life  = _lifeMax = 1000;
        
    }else if (level <= 20){
        _buildLight =  Sprite::create("res/build/flatlandsNest_h.png");
        _build = Sprite::create("res/build/flatlandsNest.png");
        _life  = _lifeMax = 1300;
    }else if (level <= 30){
        _buildLight =  Sprite::create("res/build/highlandNest_h.png");
        _build = Sprite::create("res/build/highlandNest.png");
        _life  = _lifeMax = 1500;
    }else if (level <= 40){
        _buildLight =  Sprite::create("res/build/snowfieldNest_h.png");
        _build = Sprite::create("res/build/snowfieldNest.png");
        _life  = _lifeMax = 2000;
    }else if (level <= 50){
        _buildLight =  Sprite::create("res/build/oceanNest_h.png");
        _build = Sprite::create("res/build/oceanNest.png");
        _life  = _lifeMax = 5000;
    }
    
    this->setContentSize(_build->getContentSize());
    
    _build->setAnchorPoint(Vec2(0,0));
    _buildLight->setAnchorPoint(Vec2(0,0));
    
    
    Sprite* bloodd = Sprite::create("res/blood/bossd.png");
    bloodd->setPosition(Vec2(_build->getContentSize().width * 0.5,_build->getContentSize().height));
    this->addChild(bloodd);
    //blood bar
    _lifeBar = ProgressTimer::create(Sprite::create("res/blood/bossbuildblood.png"));
    _lifeBar->setType(ProgressTimer::Type::BAR);
    _lifeBar->setMidpoint(Vec2(0,0));
    _lifeBar->setBarChangeRate(Vec2(1,0));
    _lifeBar->setPosition(Vec2(_build->getContentSize().width * 0.5,_build->getContentSize().height));
    _lifeBar->setPercentage(100.0);
    
   
    
    this->addChild(_build,1);
    this->addChild(_buildLight,2);
    _buildLight->setVisible(true);
    _build->addChild(_lifeBar);
    return true;
}

Building::Building() {
    _life = 100;
    _lifeMax = 100;
}

Building::~Building() {
    
}


void Building::receiveDamage(float damage) {
    int nDamage = damage;
    if (nDamage <= 0) {
        nDamage = 1;
    }
    
    setLife(getLife()-nDamage);
     _lifeBar->setPercentage(_life*100/_lifeMax);
    if (getLife() <= 0) {
        auto spr = Sprite::create("res/animation/first.png");
        spr->setPosition(this->getPosition());
         GAMESCENE->addObject2Map(spr);
          spr->runAction(Sequence::create(Animate::create(AnimationHelper::getAnimation("enemytankboom")),DelayTime::create(0.5f), RemoveSelf::create(true),nullptr));
        this->setVisible(false);
    }

}
