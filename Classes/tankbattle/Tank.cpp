//
//  Tank.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/7/17.
//
//

#include "Tank.h"
#include "constants.h"
#include "GameManager.h"
#include "Bullet.h"
#include "GameScene.h"
#include "AnimationHelper.h"
#include "TankAI.h"
#include "TankManager.h"
#include "utils/SoundManager.h"
#include "PathFinder.h"

const int tank_size = 70;
const int tag_chain_animation = 1;

USING_NS_CC;
TankAI tankAI;
cocos2d::Rect Tank::getBoundingBox() {
    Rect rect = Rect(this->getPosition().x - this->getContentSize().width/3,
                     this->getPosition().y - this->getContentSize().height/3,
                     this->getContentSize().width/3 * 2,this->getContentSize().height/3 * 2);
    return rect;
}

Tank* Tank::createTankPlayer(int cannon,int body, int chain) {
    Tank* tankPlayer = new Tank();
    if (tankPlayer && tankPlayer->initTankPlayer(cannon, body, chain)) {
        tankPlayer->autorelease();
        TankManager::getInstance()->addTank(tankPlayer);
        return tankPlayer;
    }
    CC_SAFE_DELETE(tankPlayer);
    return nullptr;
}

bool Tank::initTankPlayer(int cannon,int body, int chain) {
    if (!Node::init()) {
        return false;
    }
    _autoShotTarget = false;
    _aiMode = 1;
    setKind(tank_player);
    
    if (cannon > EQUIP_MAX_LEVEL) {
        cannon = EQUIP_MAX_LEVEL;
    }
    
    if (body > EQUIP_MAX_LEVEL) {
        body = EQUIP_MAX_LEVEL;
    }

    if (chain > EQUIP_MAX_LEVEL) {
        chain = EQUIP_MAX_LEVEL;
    }
    _cannonSpr = nullptr;
    _tankNode = nullptr;
    _bodySpr = nullptr;
    _chainSpr = nullptr;
    _chainAnimation = nullptr;
//    UserDefault::getInstance()->setIntegerForKey("hp",30);
//    UserDefault::getInstance()->setIntegerForKey("defense",12);
//    UserDefault::getInstance()->setIntegerForKey("attack",1500);;
//    UserDefault::getInstance()->setIntegerForKey("speed",4.5f);
//    UserDefault::getInstance()->setIntegerForKey("canonlevel",1);
//    UserDefault::getInstance()->setIntegerForKey("bodylevel",1);
//    UserDefault::getInstance()->setIntegerForKey("chainlevel",1);
//    UserDefault::getInstance()->setIntegerForKey("chiplevel",1);
    _cannonLevel = cannon;
    _bodyLevel = body;
    _chainLevel = chain;
    
    _lastFireTime = 0;
    _life =  UserDefault::getInstance()->getIntegerForKey("hp",30);
    _lifeMax =  UserDefault::getInstance()->getIntegerForKey("hp",30);
    _defense = _baseDefense = UserDefault::getInstance()->getIntegerForKey("defense",12);
    _attack =  _baseAttack = UserDefault::getInstance()->getIntegerForKey("attack",15);;
    _speed = _baseSpeed =  UserDefault::getInstance()->getIntegerForKey("speed",4.5f);
    
    
    if (_speed > 10) {
        _speed = 10;
        UserDefault::getInstance()->setIntegerForKey("speed",10);
    }
    _bulletTime = GameManager::getInstance()->getBullet(_cannonLevel).at("bullettime").asFloat();
    _bulletType = _cannonLevel;
    //load sprite
    _shadowSpr = Sprite::create(PLAYER_SHADOW);
    _tankNode = Node::create();
    setChain(_chainLevel);
    setBody(_bodyLevel);
   this->addChild(_tankNode);
   setCannon(_cannonLevel);
    //blood bar
    _lifeBarBg = Sprite::create(BLOOD_BG);
    _lifeBarBg->setPosition(Vec2(0,_cannonSpr->getContentSize().height/2));
    this->addChild(_lifeBarBg);

    _lifeBar = ProgressTimer::create(Sprite::create(PLAYER_BLOOD));
    _lifeBar->setType(ProgressTimer::Type::BAR);
    _lifeBar->setMidpoint(Vec2(0,0));
    _lifeBar->setBarChangeRate(Vec2(1, 0));
    _lifeBar->setPosition(Vec2(0,_cannonSpr->getContentSize().height/2));
    _lifeBar->setPercentage(100);
    
    this->addChild(_lifeBar);
 
    
    this->setContentSize(_cannonSpr->getContentSize());
    

    
    return true;
}


Tank* Tank::createTankEnemy(int level) {
    Tank* tankEnemy = new Tank();
    if (tankEnemy && tankEnemy->initTankEnemy(level)) {
        tankEnemy->autorelease();
        TankManager::getInstance()->addTank(tankEnemy);
        return tankEnemy;
    }
    CC_SAFE_RELEASE(tankEnemy);
    return nullptr;
}

bool Tank::initTankEnemy(int level) {
    if (!Node::init()) {
        return false;
    }
    setKind(tank_normal);
    ValueMap tankOpt = GameManager::getInstance()->getTankEnemy(level);
    _level = level;
    _aiMode = tankOpt.at("ai").asInt();
    _attack = _baseAttack = tankOpt.at("attack").asFloat();
    _speed = _baseSpeed= tankOpt.at("speed").asFloat();
    _defense = _baseDefense = tankOpt.at("defense").asFloat();
    _life = tankOpt.at("life").asFloat();
    _lifeMax = tankOpt.at("life").asFloat();
    
    _chainLevel = tankOpt.at("chain").asInt();
    _bodyLevel = tankOpt.at("body").asInt();
    _cannonLevel = tankOpt.at("cannon").asInt();
    _bulletTime = GameManager::getInstance()->getBullet(_cannonLevel).at("bullettime").asFloat();
    
    _life = tankOpt.at("life").asInt();
    _lifeMax = tankOpt.at("life").asInt();
    
    
     _bulletType = tankOpt.at("bullettype").asInt();
    
    _shadowSpr = Sprite::create(ENEMY_SHADOW);
    _tankNode = Node::create();
    setChain(_chainLevel);
    setBody(_bodyLevel);
    this->addChild(_tankNode);
    setCannon(_cannonLevel);
    
    //blood bar
    _lifeBarBg = Sprite::create(BLOOD_BG);
    _lifeBarBg->setPosition(Vec2(0,_cannonSpr->getContentSize().height/2));
    this->addChild(_lifeBarBg);

    _lifeBar = ProgressTimer::create(Sprite::create(ENEMY_BLOOD));
    _lifeBar->setType(ProgressTimer::Type::BAR);
    _lifeBar->setMidpoint(Vec2(0,0));
    _lifeBar->setBarChangeRate(Vec2(1, 0));
    _lifeBar->setPosition(Vec2(0,_cannonSpr->getContentSize().height/2));
    _lifeBar->setPercentage(100);
    this->addChild(_lifeBar);
    this->setContentSize(_cannonSpr->getContentSize());

    return true;
}

Tank* Tank::createTankBoss(int level) {
    Tank* tankBoss = new Tank();
    if (tankBoss && tankBoss->initTankBoss(level)) {
        tankBoss->autorelease();
        TankManager::getInstance()->addTank(tankBoss);
        return tankBoss;
    }
    
    CC_SAFE_DELETE(tankBoss);
    return nullptr;
}


Tank* Tank::createFriendUnit() {
    Tank* tankBoss = new Tank();
    if (tankBoss && tankBoss->initFriendUnit()) {
        tankBoss->autorelease();
        TankManager::getInstance()->addTank(tankBoss);
        return tankBoss;
    }
    
    CC_SAFE_DELETE(tankBoss);
    return nullptr;
}
bool Tank::initFriendUnit() {
    if (!Node::init()) {
        return false;
    }
    setKind(tank_friend);
    setAIMode(1);
    
    setLife(GameManager::getInstance()->getLevel()*100/10);
    setLifeMax(GameManager::getInstance()->getLevel()*100/10);
    setSpeed(1.5f);
    setDefense(20);
    
    
    _chainSpr = Sprite::create(FRIEND_BODY);
    //blood bar
    _lifeBarBg = Sprite::create(BLOOD_BG);
    _lifeBarBg->setPosition(Vec2(0,_chainSpr->getContentSize().height/2));
    this->addChild(_lifeBarBg);
    auto blood = Sprite::create(PLAYER_BLOOD);
    blood->retain();
    _lifeBar = ProgressTimer::create(blood);
    _lifeBar->setType(ProgressTimer::Type::BAR);
    _lifeBar->setMidpoint(Vec2(0,0));
    _lifeBar->setBarChangeRate(Vec2(1, 0));
    _lifeBar->setPosition(Vec2(0,_chainSpr->getContentSize().height/2));
    _lifeBar->setPercentage(100);
    
    this->addChild(_lifeBar);
    
    Vector<SpriteFrame*> animFrames(2);
    auto frame = SpriteFrame::create(FRIEND_BODY,Rect(0,0,tank_size,tank_size));
    animFrames.pushBack(frame);
    frame = SpriteFrame::create(FRIEND_BODY1,Rect(0,0,tank_size,tank_size));
    animFrames.pushBack(frame);
    
    _chainAnimation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    _chainAnimation->setRestoreOriginalFrame(true);
    _chainAnimation->setLoops(-1);
    _chainAnimation->retain();
    
    _tankNode = Node::create();
    _tankNode->addChild(_chainSpr);
    this->setContentSize(_chainSpr->getContentSize());
    this->addChild(_tankNode);
    
    
    return true;
}
bool Tank::initTankBoss(int level) {
    if (!Node::init()) {
        return false;
    }
    setKind(tank_normal);
    _isBoss = true;
    ValueMap tankOpt = GameManager::getInstance()->getTankBoss(level);
    _level = level;
    _attack = _baseAttack = tankOpt.at("attack").asFloat();
    _speed = _baseSpeed = tankOpt.at("speed").asFloat();
    _defense = _baseDefense = tankOpt.at("defense").asFloat();
    _life = tankOpt.at("life").asFloat();
    _lifeMax = tankOpt.at("life").asFloat();
    
    _chainLevel = tankOpt.at("id").asInt();
    _bodyLevel = tankOpt.at("id").asInt();
    _cannonLevel = tankOpt.at("id").asInt();
    _bulletTime = GameManager::getInstance()->getBullet(12).at("bullettime").asFloat();
    _bulletType = 12 + _level;
    
   _shadowSpr = Sprite::create("res/boss/shadow.png");
    _bodySpr = Sprite::create(StringUtils::format(BOSS_BODY,_bodyLevel).c_str());
    _cannonSpr = Sprite::create(StringUtils::format(BOSS_CANNON,_cannonLevel).c_str());
    _chainSpr = Sprite::create(StringUtils::format(BOSS_CHAIN,_chainLevel).c_str());
    
    _borderSpr = Sprite::create(StringUtils::format(BOSS_BORDER,level).c_str());
    
    Vector<SpriteFrame*> borderFrames(2);
    auto frame0 = SpriteFrame::create(StringUtils::format(BOSS_BORDER,level).c_str(),Rect(0,0,_borderSpr->getContentSize().width,_borderSpr->getContentSize().height));
    borderFrames.pushBack(frame0);
    frame0 = SpriteFrame::create(StringUtils::format(BOSS_BORDER1,level).c_str(),Rect(0,0,_borderSpr->getContentSize().width,_borderSpr->getContentSize().height));
    borderFrames.pushBack(frame0);
    auto borderAni = Animation::createWithSpriteFrames(borderFrames);
    borderAni->setLoops(-1);
    borderAni->setDelayPerUnit(0.5f);
    _borderSpr->runAction(RepeatForever::create(Animate::create(borderAni)));
    
    
    Vector<SpriteFrame*> canonFrames(2);
    auto frame2 = SpriteFrame::create(StringUtils::format(BOSS_CANNON,_cannonLevel).c_str(),Rect(0,0,_cannonSpr->getContentSize().width,_cannonSpr->getContentSize().height));
    canonFrames.pushBack(frame2);
    frame2 = SpriteFrame::create(StringUtils::format(BOSS_CANNON1,_cannonLevel).c_str(),Rect(0,0,_cannonSpr->getContentSize().width,_cannonSpr->getContentSize().height));
    canonFrames.pushBack(frame2);
    auto cannonAni = Animation::createWithSpriteFrames(canonFrames);
    cannonAni->setLoops(-1);
    cannonAni->setDelayPerUnit(0.1f);
    _cannonSpr->runAction(RepeatForever::create(Animate::create(cannonAni)));
    
    Vector<SpriteFrame*> animFrames(2);
    auto frame = SpriteFrame::create(StringUtils::format(BOSS_BODY,_bodyLevel).c_str(),Rect(0,0,_bodySpr->getContentSize().width,_bodySpr->getContentSize().height));
    animFrames.pushBack(frame);
    frame = SpriteFrame::create(StringUtils::format(BOSS_BODY1,_bodyLevel).c_str(),Rect(0,0,_bodySpr->getContentSize().width,_bodySpr->getContentSize().height));
    animFrames.pushBack(frame);
    
    _chainAnimation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    _chainAnimation->setRestoreOriginalFrame(true);
    _chainAnimation->setLoops(-1);
    _chainAnimation->retain();
    _chainSpr->runAction(RepeatForever::create(Animate::create(_chainAnimation)));
    _bodySpr->runAction(RepeatForever::create(Animate::create(_chainAnimation)));
    this->addChild(_shadowSpr);
    _tankNode = Node::create();
    _tankNode->addChild(_borderSpr);
    _tankNode->addChild(_chainSpr);
    //  _tankNode->addChild(_bodySpr);
    this->setContentSize(_cannonSpr->getContentSize());
    this->addChild(_tankNode);
    this->addChild(_cannonSpr);
    
    
    _lifeBarBg = Sprite::create(BOSS_BLODD_BG);
    _lifeBarBg->setPosition(Vec2(0,_cannonSpr->getContentSize().height/2));
    this->addChild(_lifeBarBg);
    auto blood = Sprite::create(BOSS_BLOOD);
    blood->retain();
    _lifeBar = ProgressTimer::create(blood);
    _lifeBar->setType(ProgressTimer::Type::BAR);
    _lifeBar->setMidpoint(Vec2(0,0));
    _lifeBar->setBarChangeRate(Vec2(1, 0));
    _lifeBar->setPosition(Vec2(0,_cannonSpr->getContentSize().height/2));
    _lifeBar->setPercentage(100);
    
    this->addChild(_lifeBar);
    
    
    return true;
}

//Tank::Tank() {
//    _cannonSpr = nullptr;
//    _tankNode = nullptr;
//    _bodySpr = nullptr;
//    _chainSpr = nullptr;
//    _chainAnimation = nullptr;
//}
Tank::~Tank() {
    CCLOG("tank desotroy %d",_id);
}

void Tank::active(float dt) {
    
    processState();
    processCommand();
    processStatus();
    if (_aiMode && _doing != do_dead) {
        tankAI.active(this);
    }
}

void Tank::receiveDamage(Tank* attacker, int damage) {
    if (_invincible) {
        return;
    }
    //receive damage from attacker
    int nDamage = damage/_defense;
    if(attacker&&attacker->getKind() == tank_player) {
        nDamage*=4;
    }
    CCLOG("damage =%d %d/%d",nDamage,_life,_lifeMax);
    if (nDamage <= 0) {
        nDamage = 1;
    }
    
    addLife(-nDamage);
    if (getLife() <= 0) {
        //CCLOG("killed by");
    }
    
}

// process current state: auto aim,freeze, burn, slow, fast movement...
void Tank::processState() {
    //implement later
    if (_doing == do_dead) {
        return;
    }
    
    
}

// process player,ai command
void Tank::processCommand() {
    if (_doing == do_dead) {
        return;
    }
    switch (_cmd_kind) {
        case do_stand:
            _doing = do_stand;
            if (_chainSpr&&_chainSpr->getActionByTag(tag_chain_animation)) {
                _chainSpr->stopActionByTag(tag_chain_animation);
            }
            break;
        case do_move:
            if (!_chainSpr) {
                break;
            }
            
            _doing = do_move;
            _doing_x = _cmd_x;
            _doing_y = _cmd_y;
            _doing_z = _cmd_z;
            
            if(_doing_x == -1) {
                if(!getTarget() || (getTarget()&& !getTarget()->isVisible()))
                    setGunRotation(_doing_z);
                setRotation(_doing_z);
                setDirection(_doing_z);
            } else {
                //calc dir
                
            }
            if (!_chainSpr->getActionByTag(tag_chain_animation) || (_chainSpr->getActionByTag(tag_chain_animation)&&_chainSpr->getActionByTag(1)->isDone() ))
            {
                
                auto act = Animate::create(_chainAnimation);
                act->setTag(tag_chain_animation);
                _chainSpr->runAction(act);
            }
            break;
        case do_shoot:
            if (_cmd_x == -1) { // shot by direction
                shoot(_cmd_z);
            } else { // shot by position
                // need calc agent between current position to target position
                shoot(_cmd_z);
            }
            break;
        case do_dead: {
            
            dead();
            
        }
            break;
        default:
            break;
    }
    _cmd_kind = do_none;
}

// process current action
void Tank::processStatus() {
    switch (_doing) {
        case do_stand:
            
            break;
        case do_move:
            
            //convert direction to rotation
            //     setGunRotation(_direction);
            setRotation(_direction);
            move(_direction);
            
            break;
        default:
            break;
    }
}

void Tank::sendCommand(int cmd, int x,int y, int z) {
    _cmd_kind = cmd;
    _cmd_x = x;
    _cmd_y = y;
    _cmd_z = z;
  
}

void Tank::setRotation(float rotationAngle) {
    rotationAngle = rotationAngle*-1 + 90;
    _tankNode->setRotation(rotationAngle);
    
    
}

void Tank::setGunRotation(float rotationAngle) {
    if (!_cannonSpr) {
        return;
    }
    rotationAngle = rotationAngle*-1 + 90;
    _cannonSpr->setRotation(rotationAngle );
}

void Tank::shoot(int dir) {
    if ( _lastFireTime > GAMESCENE->getGameLoop()) {
        return;
    }
    
    if ( this->getCannonSpr()->getActionByTag(999)) {
        return;
    }
    
    
    //direct rotate and shoot
    //    setGunRotation(dir);
    //    auto bullet = Bullet::createBullet(getId(), getCannonLevel());
    //    bullet->setPosition(getPosition());
    //    bullet->flyTo(getCannonSpr()->getRotation());
    //    Director::getInstance()->getRunningScene()->addChild(bullet);
    
    
    auto callback = CallFuncN::create([&](Node* sender) {
        
        Tank* tank=  this;
        
        
        Bullet* bullet = nullptr;
         bullet = Bullet::createBullet(getId(),_bulletType);
        if(getIsBoss()) {
            bullet->setCanDesStone(true);
            tank->_lastFireTime = GAMESCENE->getGameLoop() + tank->_bulletTime*60;
        } else if(getKind() == tank_player) {
            tank->_lastFireTime = GAMESCENE->getGameLoop() + tank->_bulletTime*10;
        } else {
            tank->_lastFireTime = GAMESCENE->getGameLoop() + tank->_bulletTime*30;
        }

        bullet->setDamage(tank->getAttack());
        bullet->setPosition(sender->getParent()->getPosition());
        bullet->flyTo(tank->getCannonSpr()->getRotation());
        if(tank->getKind() == tank_player) {
            switch(getCannonLevel()){
                case 1:
                case 7:
                    SoundManager::getInstance()->playEffect("res/sound/sfx_fire1.mp3");
                    break;
                case 2:
                case 8:
                    SoundManager::getInstance()->playEffect("res/sound/sfx_fire2.mp3");
                    break;
                case 3:
                case 9:
                    SoundManager::getInstance()->playEffect("res/sound/sfx_fire3.mp3");
                    break;
                default:
                    SoundManager::getInstance()->playEffect("res/sound/sfx_fire4.mp3");
                    break;
            }
        }
        GAMESCENE->addObject2Map(bullet);
    });
    float rotationAngle = dir*-1 + 90;
    float curRotation = this->getCannonSpr()->getRotation();
    if (curRotation < 0) {
        curRotation = 360 + curRotation;
    }
    
    if (rotationAngle < 0) {
        rotationAngle = 360 + rotationAngle;
    } else if(rotationAngle > 360 ) {
        rotationAngle = rotationAngle - 360*(rotationAngle/360);
    }
    
    auto act = Sequence::createWithTwoActions(RotateTo::create(0.1f, rotationAngle), callback);
    act->setTag(999);
    this->getCannonSpr()->runAction(act);
    
}

void Tank::appear() {

    if(getKind() == tank_player) {
        this->addChild(AnimationHelper::createEffect("playerAppear", 0.45f, Vec2::ZERO));
    } else if(getKind() == tank_normal ) {
        this->addChild(AnimationHelper::createEffect("enemyAppear", 0.45f, Vec2::ZERO));
    }
}

void Tank::dead() {
    _doing = do_dead;
    SoundManager::getInstance()->playEffect("res/sound/sfx_explosiontank.mp3");
    // run animation
    switch (getKind()) {
        case tank_friend:
        case tank_player: {
            auto spr = Sprite::create("res/animation/first.png");
            spr->setPosition(this->getPosition());
            GAMESCENE->addObject2Map(spr);
            spr->runAction(Sequence::create(Animate::create(AnimationHelper::getAnimation("boom")),DelayTime::create(0.5f), RemoveSelf::create(true),nullptr));
            this->setVisible(false);
        }
            
            break;
        case tank_normal: {
            auto spr = Sprite::create("res/animation/first.png");
            spr->setPosition(this->getPosition());
            GAMESCENE->addObject2Map(spr);
            if(_isBoss) {
                spr->runAction(Sequence::create(Animate::create(AnimationHelper::getAnimation("bossboom")),DelayTime::create(0.5f), RemoveSelf::create(true),nullptr));
            } else {
                spr->runAction(Sequence::create(Animate::create(AnimationHelper::getAnimation("enemytankboom")),DelayTime::create(0.5f), RemoveSelf::create(true),nullptr));
            }
            this->setVisible(false);
        }
            break;
        default:
            break;
    }
    //post without threadsafe will cause game crash!
    //__NotificationCenter::getInstance()->postNotification("onTankDead",this);
    
    GAMESCENE->onTankDead(this);
}
void Tank::move(int dir) {
    //if (_doing_x == -1)
    { // move by direction

        if (abs(_direction)== 181) {
            return;
        }
        
        Point positionOrigin = this->getPosition();
        float rad = CC_DEGREES_TO_RADIANS(_direction);
        float offsetX = _speed*cos(rad);
        float offsetY = _speed*sin(rad);
        Point positionNext = positionOrigin + Vec2(offsetX,offsetY);
        
 
        
        if (isBoundary(positionNext.x,positionNext.y)) {
            if(_aiMode && getKind() != tank_player)
                setDirection(181);
            return;
        }
        
        GameScene* game = (GameScene*)Director::getInstance()->getRunningScene()->getChildByTag(1);
        
        //can't destroy object
        __Array* arr = game->getTankMap()->getCannotDes();
        for(int i=0;i < arr->count();i++) {
            Node* nodeObj = (Node*)arr->getObjectAtIndex(i);
            
            if (nodeObj->isVisible() && nodeObj->getBoundingBox().intersectsCircle(positionNext, (_chainSpr->getContentSize().width - 20)/2)) {

                if (tank_player && fixPos(positionOrigin, positionNext)) {
                    setPosition(positionNext);
                }
                
                if(_aiMode && getKind() != tank_player)
                    setDirection(181);
                 return;
            }
        }
        
        //destroyable object
        __Array* canDes = game->getTankMap()->getCanDes();
        for(int i=0;i < canDes->count();i++) {
            Node* nodeObj = (Node*)canDes->getObjectAtIndex(i);
            
            if (nodeObj->isVisible() && nodeObj->getBoundingBox().intersectsCircle(positionNext, (_chainSpr->getContentSize().width - 20)/2)) {
                
                if (tank_player && fixPos(positionOrigin, positionNext)) {
                    setPosition(positionNext);
                }
                
                if(_aiMode && getKind() != tank_player)
                    setDirection(181);
                return;
            }
        }
        
        if (game->getBuilding() && game->getBuilding()->isVisible()) {
            if (game->getBuilding()->getBoundingBox().intersectsCircle(positionNext, (_chainSpr->getContentSize().width - 20)/2)) {
                if(_aiMode && getKind() != tank_player)
                    setDirection(181);
                return;
            }
        }
        for (auto tank : TankManager::getInstance()->getTankList()) {
            Tank* pTank = (Tank*)tank;
            if (pTank == this) {
                continue;
            }
            if (!pTank->isVisible()) {
                continue;
            }
            if (pTank->getBoundingBox().intersectsCircle(positionNext,(_chainSpr->getContentSize().width - 20)/2)) {
                if (positionNext.getDistance(pTank->getPosition()) > getPosition().getDistance(pTank->getPosition())) { // if leave from center
                    
                    
                    continue;
                }
                
                //stop follow path if collision with other tank
                if(getKind() == tank_player) {
                    if (getRunPath()) {
                        setDesPos(Vec2::ZERO);
                        getRunPath()->clear();
                        setRunPath(nullptr);
                        GAMESCENE->getPathFinderLayer()->removeAllChildren();
                        sendCommand(do_stand);
                        CCLOG("enter here");
                        return;
                    }
                }
                return;
            }
        }
        
        
        //other enemy tank
        if (_walkSound%10==0) {
            if(getKind() == tank_player)
                SoundManager::getInstance()->playEffect("res/sound/sfx_walk.mp3");
            _walkSound = 1;
        }
        _walkSound++;
        this->setPosition(positionNext);
        
    }
    //    else { // move by position
    //
    //    }
}


bool Tank::isBoundary(float x,float y){
    Size winSize = Director::getInstance()->getWinSize();
    float tankWidth = this->getContentSize().width/2;
    float tankHeight = this->getContentSize().height/2;
    
    if( x < tankWidth/2 + 120 || x>( GAMESCENE->getTankMap()->getMapWidth() - tankWidth - 80) || y<tankHeight/2 || y>(winSize.height - tankHeight/2) )
        return true;
    else
        return false;
}

void Tank::addLife(int life) {
    if (_doing == do_dead || _lifeMax == 0) {
        return;
    }
    _life = _life + life;
    
    if (_life <= 0) {
        _life = 0;
        sendCommand(do_dead);
    }
    _lifeBar->setPercentage(_life*100/_lifeMax);
}


void Tank::setChain(int level) {

    _chainLevel = level;
    if (_chainSpr) {
        auto chain = _chainSpr;
       if (getKind() == tank_player) {
           if (level > EQUIP_MAX_LEVEL) {
               level = EQUIP_MAX_LEVEL;
           }
           chain->stopAllActions();
           chain->setTexture(StringUtils::format(PLAYER_CHAIN,level));
           if (_chainAnimation) {
               chain->stopActionByTag(tag_chain_animation);
               _chainAnimation->release();
           }
           Vector<SpriteFrame*> animFrames(2);
           auto frame = SpriteFrame::create(StringUtils::format(PLAYER_CHAIN,level).c_str(),Rect(0,0,tank_size,tank_size));
           animFrames.pushBack(frame);
           frame = SpriteFrame::create(StringUtils::format(PLAYER_CHAIN1,level).c_str(),Rect(0,0,tank_size,tank_size));
           animFrames.pushBack(frame);
           
           _chainAnimation = Animation::createWithSpriteFrames(animFrames, 0.1f);
           _chainAnimation->setRestoreOriginalFrame(true);
           _chainAnimation->setLoops(-1);
           _chainAnimation->retain();
           
       } else if(getKind() == tank_normal) {
           chain->setTexture(StringUtils::format(ENEMY_CHAIN,level));
           Vector<SpriteFrame*> animFrames(2);
           auto frame = SpriteFrame::create(StringUtils::format(ENEMY_CHAIN,level).c_str(),Rect(0,0,tank_size,tank_size));
           animFrames.pushBack(frame);
           frame = SpriteFrame::create(StringUtils::format(ENEMY_CHAIN1,level).c_str(),Rect(0,0,tank_size,tank_size));
           animFrames.pushBack(frame);
           
           _chainAnimation = Animation::createWithSpriteFrames(animFrames, 0.1f);
           _chainAnimation->setRestoreOriginalFrame(true);
           _chainAnimation->setLoops(-1);
           _chainAnimation->retain();
       } else {
           
       }
        
        return;
    }
    
    if (getKind() == tank_player) {
        if (level > EQUIP_MAX_LEVEL) {
            level = EQUIP_MAX_LEVEL;
        }
        
        _chainSpr = Sprite::create(StringUtils::format(PLAYER_CHAIN,level));
        _chainSpr->setTag(kTankChain);
        _tankNode->addChild(_chainSpr);
        if (_chainAnimation) {
            _chainAnimation->release();
        }
        Vector<SpriteFrame*> animFrames(2);
        auto frame = SpriteFrame::create(StringUtils::format(PLAYER_CHAIN,level).c_str(),Rect(0,0,tank_size,tank_size));
        animFrames.pushBack(frame);
        frame = SpriteFrame::create(StringUtils::format(PLAYER_CHAIN1,level).c_str(),Rect(0,0,tank_size,tank_size));
        animFrames.pushBack(frame);
        
        _chainAnimation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        _chainAnimation->setRestoreOriginalFrame(true);
        _chainAnimation->setLoops(-1);
        _chainAnimation->retain();
        
        
    } else if(getKind() == tank_normal) {
        _chainSpr = Sprite::create(StringUtils::format(PLAYER_CHAIN,level));
        _chainSpr->setTag(kTankChain);
        _tankNode->addChild(_chainSpr);
        
        if (_chainAnimation) {
            _chainAnimation->release();
        }
        Vector<SpriteFrame*> animFrames(2);
        auto frame = SpriteFrame::create(StringUtils::format(ENEMY_CHAIN,level).c_str(),Rect(0,0,tank_size,tank_size));
        animFrames.pushBack(frame);
        frame = SpriteFrame::create(StringUtils::format(ENEMY_CHAIN1,level).c_str(),Rect(0,0,tank_size,tank_size));
        animFrames.pushBack(frame);
        
        _chainAnimation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        _chainAnimation->setRestoreOriginalFrame(true);
        _chainAnimation->setLoops(-1);
        _chainAnimation->retain();
    } else {
        CCLOG("not support setChain");
    }
    
}

void Tank::setCannon(int level) {

    _cannonLevel = level;
    if (_cannonSpr) {
        auto cannon = _cannonSpr;
        if (getKind() == tank_player) {
            if (level > EQUIP_MAX_LEVEL) {
                level = EQUIP_MAX_LEVEL;
            }
            _bulletType = level;
            cannon->setTexture(StringUtils::format(PLAYER_CANNON,level));
        } else if(getKind() == tank_normal) {
            cannon->setTexture(StringUtils::format(ENEMY_CANNON,level));
        } else {
            
        }
            this->setContentSize(_cannonSpr->getContentSize());
        return;
    }
    
    if (getKind() == tank_player) {
        if (level > EQUIP_MAX_LEVEL) {
            level = EQUIP_MAX_LEVEL;
        }
        _cannonSpr = Sprite::create(StringUtils::format(PLAYER_CANNON,level).c_str());
        _cannonSpr->setTag(kTankCannon);
        _bulletType = level;
       addChild(_cannonSpr);
    } else if(getKind() == tank_normal) {
        _cannonSpr = Sprite::create(StringUtils::format(ENEMY_CANNON,level).c_str());
        _cannonSpr->setTag(kTankCannon);
        addChild(_cannonSpr);
    } else {
        CCLOG("not support setChain");
    }
        this->setContentSize(_cannonSpr->getContentSize());
}

void Tank::setBody(int level) {

    _bodyLevel = level;
    if (_bodySpr) {
        auto cannon = _bodySpr;
        if (getKind() == tank_player) {
            if (level > EQUIP_MAX_LEVEL) {
                level = EQUIP_MAX_LEVEL;
            }
            cannon->setTexture(StringUtils::format(PLAYER_BODY,level));
        } else if(getKind() == tank_normal) {
            cannon->setTexture(StringUtils::format(ENEMY_BODY,level));
        } else {
            
        }
        return;
    }
    
    if (getKind() == tank_player) {
        if (level > EQUIP_MAX_LEVEL) {
            level = EQUIP_MAX_LEVEL;
        }
        _bodySpr = Sprite::create(StringUtils::format(PLAYER_BODY,level).c_str());
        _bodySpr->setTag(kTankBody);
        _tankNode->addChild(_bodySpr);
    } else if(getKind() == tank_normal) {
        _bodySpr = Sprite::create(StringUtils::format(ENEMY_BODY,level).c_str());
        _bodySpr->setTag(kTankBody);
        _tankNode->addChild(_bodySpr);
    } else {
        CCLOG("not support setChain");
    }
}

bool Tank::fixPos(cocos2d::Point origin, cocos2d::Point& next) {
    PathFinder* p = PathFinder::getInstance();
    int dir = _direction;
    float rad, offsetX,offsetY;
    
    Point tile = p->pos2tile(next);
    Point tile1 = p->pos2tile(Point(next.x - 25,next.y + 25)); // left up
    Point tile2 = p->pos2tile(Point(next.x + 25,next.y + 25)); // right up
    Point tile3 = p->pos2tile(Point(next.x + 25,next.y - 25)); // right down
    Point tile4 = p->pos2tile(Point(next.x - 25,next.y - 25)); // left down
    
    bool cw1 = p->walkable(tile1.x, tile1.y);
    bool cw2 = p->walkable(tile2.x, tile2.y);
    bool cw3 = p->walkable(tile3.x, tile3.y);
    bool cw4 = p->walkable(tile4.x, tile4.y);
    
    if (dir == 0) { // right
        if (!cw2 || !cw3) {
            if (cw2) {
                setRotation(90);
                rad = CC_DEGREES_TO_RADIANS(90);
            } else if (cw3){
                setRotation(270);
                rad = CC_DEGREES_TO_RADIANS(270);
            }
            
            offsetX = _speed*cos(rad);
            offsetY = _speed*sin(rad);
            if (tile2.y != tile3.y) {
                next.x =origin.x;
                next.y =origin.y + (!cw2)*offsetY + (!cw3)*offsetY;
                if (isBoundary(next.x, next.y)) {
                    return false;
                }
                   return true;
            } else {
                next.x = origin.x - (!cw2)*offsetX - (!cw3)*offsetX;
                return false;
            }
         
        }
        if( !cw2 && !cw3) {
            return false;
        }
    } else if(dir == 90) {//up
        
        if (!cw1 || !cw2) {
            if (cw1) {
                rad = CC_DEGREES_TO_RADIANS(0);
            } else if (cw2){
                rad = CC_DEGREES_TO_RADIANS(180);
            }
            
            offsetX = _speed*cos(rad);
            offsetY = _speed*sin(rad);
            if (tile1.x != tile2.x) {
                next.y = origin.y;
                next.x = origin.x - (!cw1)*offsetX - (!cw2)*offsetX;
                if (isBoundary(next.x, next.y)) {
                    return false;
                }
                   return true;
            } else {
                next.y = next.y - (!cw1)*offsetY - (!cw2)*offsetY;
                return false;
            }
        }
        if( !cw1 && !cw2) {
            return false;
        }
        
    } else if(dir == 180) { // left
        if (!cw1 || !cw4) {
            if (cw1) {
                rad = CC_DEGREES_TO_RADIANS(90);
            } else if (cw4){
                rad = CC_DEGREES_TO_RADIANS(270);
            }
            
            offsetX = _speed*cos(rad);
            offsetY = _speed*sin(rad);
            if (tile1.y != tile4.y) {
                next.x =origin.x;
                next.y =origin.y + (!cw1)*offsetY + (!cw4)*offsetY;
                if (isBoundary(next.x, next.y)) {
                    return false;
                }
                return true;
            } else {
                next.x = origin.x - (!cw1)*offsetX - (!cw4)*offsetX;
                return false;
            }
        }
        if( !cw1 && !cw4) {
            return false;
        }
        
    } else if(dir == 270) {// down
        if (!cw3 || !cw4) {
            if (cw4) {
                rad = CC_DEGREES_TO_RADIANS(0);
            } else if (cw3){
                rad = CC_DEGREES_TO_RADIANS(180);
            }
            
            offsetX = _speed*cos(rad);
            offsetY = _speed*sin(rad);
            if (tile3.x != tile4.x) {
                next.y = origin.y;
                next.x = origin.x - (!cw4)*offsetX - (!cw3)*offsetX;
                if (isBoundary(next.x, next.y)) {
                    return false;
                }
                   return true;
            } else {
                next.y = next.y - (!cw4)*offsetY - (!cw3)*offsetY;
                return false;
            }
        }
        if( !cw3 && !cw4) {
            return false;
        }
    }
    return false;
}

void Tank::restoreAttack(float dt) {
    CCLOG("old attack = %f base = %f",_attack,_baseAttack);
    _attack = _baseAttack;
    setCannon(UserDefault::getInstance()->getIntegerForKey("canonlevel",1));
}

void Tank::restoreDefense(float dt) {
    
    CCLOG("restore defense = %f  base %f",_defense,_baseDefense);
    _defense = _baseDefense;
    setBody(UserDefault::getInstance()->getIntegerForKey("bodylevel",1));
}

void Tank::restoreSpeed(float dt) {
    CCLOG("restore speed = %f  base %f",_speed,_baseSpeed);
    _speed = _baseSpeed;
      setChain(UserDefault::getInstance()->getIntegerForKey("chainlevel",1));
}

void Tank::restoreInviceble(float dt) {
    _invincible = false;
}
