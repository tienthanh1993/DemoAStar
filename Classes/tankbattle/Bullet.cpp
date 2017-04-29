//
//  Bullet.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#include "Bullet.h"
#include "GameManager.h"
#include "BulletManager.h"
#include "TankManager.h"
#include "GameScene.h"
#include "AnimationHelper.h"
#include "PathFinder.h"
#include "utils/SoundManager.h"
#include "constants.h"

USING_NS_CC;


Bullet* Bullet::createBullet( int owner, int level) {
    Bullet* bullet = new Bullet();
    if (bullet &&  bullet->init(owner,level)) {
        bullet->autorelease();
        BulletManager::getInstance()->addBullet(bullet);
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return NULL;
}

Bullet::Bullet() {
    
}

Bullet::~Bullet() {
    
}

bool Bullet::init(int owner, int level) {
    if (!Node::init())
        return false;
    //load bullet data by level
    _ownerID = owner;
    
    _level = level;
    
    ValueMap bulletData = GameManager::getInstance()->getBullet(level);
    
    _speed = bulletData.at("bullettime").asFloat();
    _damage = bulletData.at("bulletdamage").asFloat();
    _canDesStone = bulletData.at("desstone").asBool();
    
    _isbounded = false;
    
    _bulletSprite = Sprite::create(StringUtils::format(BULLET,_level).c_str());
    
    this->setAnchorPoint(Vec2::ZERO);
    _bulletSprite->setAnchorPoint(Point(0.5f,1.0f));
    
    this->addChild(_bulletSprite,8);
    
    
     SpriteFrameCache::getInstance()->addSpriteFramesWithFile(BOOM_FILE);

    
    return true;
}

void Bullet::flyTo(int dir) {
  
    
    
    //show shoot effect
    int angle = dir*-1 + 90 ; // convert rotation to angle
    

    this->setRotation((dir));
    _offsetX = 10.0f * cosf(CC_DEGREES_TO_RADIANS(angle));
    _offsetY = 10.0f * sinf(CC_DEGREES_TO_RADIANS(angle));
    this->setVisible(true);
    this->setPosition(getPosition()+ Vec2( cosf(CC_DEGREES_TO_RADIANS(angle))*110/2,sinf(CC_DEGREES_TO_RADIANS(angle))*110/2));
}

void Bullet::active(float dt) {
    
    this->setPosition(this->getPosition()+ Vec2(_offsetX,_offsetY));
    
    //check collision with entity
    //check collision with map
    //movevent
      Tank* pOwner = TankManager::getInstance()->getTank(_ownerID);
    for (auto tank : TankManager::getInstance()->getTankList()) {
        Rect rect = tank->getBoundingBox();
        rect.setRect(tank->getPosition().x - tank->getContentSize().width/3,
                     tank->getPosition().y - tank->getContentSize().height/3,
                     tank->getContentSize().width/3 * 2,tank->getContentSize().height/3 * 2);
        if (rect.containsPoint(getPosition()) ) {
            // check relation between 
            
            if(!tank->isVisible())
                continue;
          
            if (pOwner && tank) {
                // no friend shot
                if (pOwner->getKind() == tank->getKind()) {
                    continue;
                }
                if (pOwner->getKind() == tank_player && tank->getKind() == tank_friend) {
                    continue;
                }
            }
            
            
            tank->receiveDamage(pOwner, _damage);
            this->setPosition(tank->getPosition());
             boom();
            this->setVisible(false);
        }
    }
    GameScene* game = (GameScene*)Director::getInstance()->getRunningScene()->getChildByTag(1);
    
    //can't destroy object
   {
        __Array* arr = game->getTankMap()->getCannotDes();
        for(int i=0;i < arr->count();i++) {
            Node* nodeObj = (Node*)arr->getObjectAtIndex(i);
            if (nodeObj->isVisible() && nodeObj->getBoundingBox().containsPoint(getPosition())) {
                if (_canDesStone) {
                    nodeObj->setVisible(false);
                    PathFinder::getInstance()->open(nodeObj->getPosition());
                }
                this->setPosition(nodeObj->getPosition());
                
                 boom();
                this->setVisible(false);
                return;
            }
        }
    }
    
    //destroyable object
    __Array* canDes = game->getTankMap()->getCanDes();
    for(int i=0;i < canDes->count();i++) {
        Node* nodeObj = (Node*)canDes->getObjectAtIndex(i);
        if (nodeObj->isVisible() && nodeObj->getBoundingBox().containsPoint(getPosition())) {
            nodeObj->setVisible(false);
            PathFinder::getInstance()->open(nodeObj->getPosition());
            this->setPosition(nodeObj->getPosition());
            
            boom();
            this->setVisible(false);
            return;
        }
    }
    
    if (GAMESCENE->getBuilding() && pOwner) {
        if(GAMESCENE->getBuilding()->isVisible()&&pOwner->getKind() == tank_player && GAMESCENE->getBuilding()->getBoundingBox().containsPoint(getPosition())) {
            GAMESCENE->getBuilding()->receiveDamage(_damage);
            boom();
            this->setVisible(false);
        }
    }
    
    
    if (getPositionX() > GAMESCENE->getTankMap()->getMapWidth()|| getPositionX() <= 0) {
        this->setVisible(false);
        return;
    }
    if (getPositionY() > Director::getInstance()->getWinSize().height || getPositionY() <= 0) {
        this->setVisible(false);
        return;
    }
}

void Bullet::boom() {

    //auto ani = Sequence::createWithTwoActions(Animate::create(AnimationHelper::getAnimation("boom")), RemoveSelf::create());
    auto spr = Sprite::create("res/animation/first.png");
    spr->setPosition(this->getPosition());
    GAMESCENE->addObject2Map(spr);
    spr->runAction(Sequence::create(Animate::create(AnimationHelper::getAnimation("boom")),DelayTime::create(0.5f), RemoveSelf::create(true),nullptr));
  //  _bulletSprite->runAction(ani);
}

