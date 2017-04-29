//
//  Tank.h
//  TankBattle
//
//  Created by Pham Thanh on 3/7/17.
//
//

#ifndef Tank_h
#define Tank_h

#include <cocos2d.h>


enum CmdKind {
    do_none,
    do_stand,
    do_move,
    do_shoot,
    do_dead,
};

enum AIState {
    state_idle, // do nothing
    state_roaming, // walk around
    state_hunting, // find enemy
    state_fighting, // attack enemy
    state_chasing, // follow enemy to attack
    state_flee, // run around when low HP
};



class Tank : public cocos2d::Node {
public:
    enum TANK_SPEC{
        kTankBody = 1,
        kTankCannon,
        kTankChain
    };
    static Tank* createTankPlayer(int cannon,int body, int chain);
    static Tank* createTankEnemy(int level);
    static Tank* createTankBoss(int level);
    static Tank* createFriendUnit();
    
    virtual bool initTankPlayer(int cannon,int body, int chain);
    virtual bool initTankEnemy(int level);
    virtual bool initTankBoss(int level);
    virtual bool initFriendUnit();
   // Tank();
    virtual ~Tank();
    //tank update
    void active(float dt);
    //move by direction
    void move(int dir);
    //shoot by direction
    void shoot(int dir);
    //appear effect
    void appear();
    //on dead
    void dead();
    
    //is out screen
    bool isBoundary(float x,float y);
    
    //
    cocos2d::Rect getBoundingBox();
    
    //receive damage
    void receiveDamage(Tank* attacker, int damage);
    
    // process current state: freeze, burn, slow, fast movement...
    void processState();
    
    // process player,ai command
    void processCommand();
    
    // process current action
    void processStatus();
    
    //send command
    void sendCommand(int cmd, int x = 0,int y = 0, int z = 0);
    
    //tank rotation
    void setRotation(float angle);
    
    void setGunRotation(float rotationAngle);
    
    void setChain(int level);
    void setCannon(int level);
    void setBody(int level);
    
private:
    bool fixPos(cocos2d::Point origin, cocos2d::Point& next);
    
    void restoreAttack(float dt);
    
    void restoreDefense(float dt);
    
    void restoreSpeed(float dt);
    
    void restoreInviceble(float dt);
    
    
    void addLife(int life);
    
    int _cmd_kind;
    int _cmd_x;
    int _cmd_y;
    int _cmd_z;
    
    CC_SYNTHESIZE(int,_doing,Doing); // current doing action
    int _doing_x;
    int _doing_y;
    int _doing_z;
    
    CC_SYNTHESIZE(bool, _rotateAndShoot, RotateAndShoot);
    
    CC_SYNTHESIZE(int, _id, Id);
    
    CC_SYNTHESIZE(int,_direction,Direction); // current doing action
    CC_SYNTHESIZE(int,_walkSound,WalkSound);
    
    CC_SYNTHESIZE(int, _life, Life);
    CC_SYNTHESIZE(int, _lifeMax, LifeMax);
    
    
    CC_SYNTHESIZE(float, _baseAttack, BaseAttack);
    CC_SYNTHESIZE(float, _baseSpeed, BaseSpeed);
    CC_SYNTHESIZE(float, _baseDefense, BaseDefense);
    
    CC_SYNTHESIZE(float, _attack, Attack);
    CC_SYNTHESIZE(float, _speed, Speed);
    CC_SYNTHESIZE(float, _defense, Defense);
    
    
    
    
    
    CC_SYNTHESIZE(bool, _invincible, Invincible);
    
    
    CC_SYNTHESIZE(float, _bulletTime, BulletTime);
    CC_SYNTHESIZE(int, _bulletType, BulletType);
    
    
    //ai mode
    CC_SYNTHESIZE(bool, _autoShotTarget, AutoShot);
    CC_SYNTHESIZE(int, _aiMode, AIMode);
    CC_SYNTHESIZE(int, _aiState, AIState);
    //ai timeout for one state
    CC_SYNTHESIZE(int64_t, _maxAITime, MaxAITime);
    //next time excute ai
    CC_SYNTHESIZE(int64_t, _nextAITime, NextAITime);
    CC_SYNTHESIZE(cocos2d::Vec2, _desPos, DesPos);
    CC_SYNTHESIZE(std::deque<cocos2d::Vec2>*, _runPath, RunPath);
    
    CC_SYNTHESIZE(Tank*, _target, Target);
    CC_SYNTHESIZE(int, _kind, Kind);
    CC_SYNTHESIZE(bool, _isBoss, IsBoss);
    
    
    CC_SYNTHESIZE(int, _level, Level);
    
    CC_SYNTHESIZE(int, _chainLevel, ChainLevel);
    CC_SYNTHESIZE(int, _bodyLevel, BodyLevel);
    
    CC_SYNTHESIZE(int, _cannonLevel, CannonLevel);
    CC_SYNTHESIZE(int, _lastFireTime, LastFireTime);
    
    
    CC_SYNTHESIZE(cocos2d::Node*, _tankNode, TankNode);
    
    CC_SYNTHESIZE(cocos2d::Sprite*, _lifeBarBg, LifeBarBg); //
    CC_SYNTHESIZE(cocos2d::ProgressTimer*, _lifeBar, LifeBar); //
    
    CC_SYNTHESIZE(cocos2d::Sprite*, _borderSpr, BorderSpr); // boss
    CC_SYNTHESIZE(cocos2d::Sprite*, _bodySpr, BodySpr);
    CC_SYNTHESIZE(cocos2d::Sprite*, _cannonSpr, CannonSpr);
    
    CC_SYNTHESIZE(cocos2d::Sprite*, _chainSpr, ChainSpr);
    CC_SYNTHESIZE(cocos2d::Animation*, _chainAnimation, ChainAnimation);
    

    
    CC_SYNTHESIZE(cocos2d::Sprite*, _shadowSpr, ShadowSpr);
};
#endif /* Tank_h */
