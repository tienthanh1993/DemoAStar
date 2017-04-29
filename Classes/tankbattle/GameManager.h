//
//  GameManager.h
//  TankBattle
//
//  Created by Pham Thanh on 3/5/17.
//
//

#ifndef GameManager_h
#define GameManager_h

#include <cocos2d.h>
class GameManager {
public:
    GameManager();
    ~GameManager();
    static GameManager* _instance;
    static GameManager* getInstance();
    static void destroyInstance();
    cocos2d::ValueMap getTankBoss(int level);
    cocos2d::ValueMap getTankEnemy(int level);
    cocos2d::ValueMap getBullet(int level);
    cocos2d::ValueMap getMission(int level);
    cocos2d::ValueMap getMissionMain(int level);
    cocos2d::ValueMap getMissionAdditional(int level);
    
private:
    void loadData();
    CC_SYNTHESIZE(int, _level, Level);
    
    //tank boss list
    cocos2d::ValueVector _tankBossArr;
    
    //tank enemy list
    cocos2d::ValueVector _tankEnemyArr;
    //bullet list
    cocos2d::ValueVector _bulletArr;
    //mission by level
    cocos2d::ValueVector _missionArr;
    //main mission list
    cocos2d::ValueVector _missionMainArr;
    //additional mission list
    cocos2d::ValueVector _missionAdditionalArr;
};

#endif /* GameManager_h */
