//
//  RPlayer.cpp
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#include "RPlayer.h"
USING_NS_CC;

RPlayer::RPlayer() {
    _runPath = nullptr;
    _desPos = Vec2::ZERO;
}

RPlayer::~RPlayer() {
    
}

RPlayer* RPlayer::createPlayer() {
    auto player = new RPlayer();
    if (player && player->init()) {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool RPlayer::init() {
    if (!Node::init()) {
        return false;
    }
    
    _spr = Sprite::create("res/rpg/player_stand_1.png");
    _spr->setAnchorPoint(Vec2(0.5f,0));
    addChild(_spr);
    
    return true;
}

const int speed = 5;
void RPlayer::active(float dt) {
    
    if (getRunPath()) {
        if(getPosition().getDistance(getDesPos()) <= speed) {
            getRunPath()->pop_front();
            setDesPos(Vec2::ZERO);
        }
        if(getRunPath()->size() > 0 ) {
            if (getDesPos().isZero()) {
                setDesPos(getRunPath()->front());
                CCLOG("run to1 [%lu] %f %f  - %f %f",getRunPath()->size(),getDesPos().x,getDesPos().y,getPosition().x,getPosition().y);
            }
        }
        
        if (!getDesPos().isZero())
        {
            auto vectorFromVec1ToVec2 = getDesPos() - getPosition();
            float angle = vectorFromVec1ToVec2.getAngle();
            float offsetX = speed*cos(angle);
            float offsetY = speed*sin(angle);
            setPosition(getPosition() + Vec2(offsetX,offsetY));
            
            if(!_spr->getActionByTag(1)) {
                setAni(1,1);
            } else {
                if(offsetX > 0) {
                    _spr->setFlippedX(true);
                } else {
                    _spr->setFlippedX(false);
                }
            }
        }
    }
}

void RPlayer::setAni(int kind,float angle) {
    auto animate = getAnimateByName("res/rpg/player_move_", 0.2f, 4);
    auto repeatanimate = RepeatForever::create(animate);
    repeatanimate->setTag(1);
    _spr->runAction(repeatanimate);
    
}


Animate* RPlayer::getAnimateByName(std::string animName,float delay,int animNum){
    Animation* animation = Animation::create();
    for(unsigned int i = 1;i<=animNum;i++){
        std::string frameName = animName;
        frameName.append(StringUtils::format("%d",i)).append(".png");
        animation->addSpriteFrameWithFile(frameName.c_str());
    }    animation->setDelayPerUnit(delay);
    animation->setRestoreOriginalFrame(true);
    Animate* animate = Animate::create(animation);
    return animate;
}
