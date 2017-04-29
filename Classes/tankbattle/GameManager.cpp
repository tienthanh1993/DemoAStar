//
//  GameManager.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/5/17.
//
//

#include "GameManager.h"
USING_NS_CC;
#define TANK_BOSS_FILE "res/data/tank_boss.plist"
#define TANK_ENEMY_FILE "res/data/tank_enemy.plist"
#define BULLET_FILE "res/data/bullet.plist"
#define MISSION_FILE "res/data/mission.plist"
#define MISSION_MAIN_FILE "res/data/mission_main.plist"
#define MISSION_ADDITIONAL_FILE "res/data/mission_additional.plist"

GameManager* GameManager::_instance = nullptr;

GameManager::GameManager() {

}

GameManager::~GameManager() {
    _tankBossArr.clear();
    _tankEnemyArr.clear();
    _bulletArr.clear();
    _missionArr.clear();
    _missionAdditionalArr.clear();
    _missionMainArr.clear();
}

GameManager* GameManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new GameManager();
        _instance->loadData();
    }
    return _instance;
}

void GameManager::destroyInstance() {
    delete _instance;
}

cocos2d::ValueMap GameManager::getTankBoss(int level) {
    return _tankBossArr.at(level -1).asValueMap();
}
cocos2d::ValueMap GameManager::getTankEnemy(int level) {
    return _tankEnemyArr.at(level -1).asValueMap();
}
cocos2d::ValueMap GameManager::getBullet(int level) {
    for (Value m : _bulletArr) {
        if (m.asValueMap()["bullettype"].asInt() == level) {
            return m.asValueMap();
        }
    }
    return _bulletArr.at(0).asValueMap();
}
cocos2d::ValueMap GameManager::getMission(int level) {
    if(level > _missionArr.size()) {
        level = 1;
    }
    return _missionArr.at(level -1).asValueMap();
}

cocos2d::ValueMap GameManager::getMissionMain(int level) {
    return _missionMainArr.at(level -1).asValueMap();
}

cocos2d::ValueMap GameManager::getMissionAdditional(int level) {
    return _missionAdditionalArr.at(level -1).asValueMap();
}


void GameManager::loadData() {
    _tankBossArr =  FileUtils::getInstance()->getValueVectorFromFile(TANK_BOSS_FILE);
    _tankEnemyArr =  FileUtils::getInstance()->getValueVectorFromFile(TANK_ENEMY_FILE);
    _bulletArr =  FileUtils::getInstance()->getValueVectorFromFile(BULLET_FILE);
    _missionArr = FileUtils::getInstance()->getValueVectorFromFile(MISSION_FILE);
    _missionMainArr = FileUtils::getInstance()->getValueVectorFromFile(MISSION_MAIN_FILE);
    _missionAdditionalArr = FileUtils::getInstance()->getValueVectorFromFile(MISSION_ADDITIONAL_FILE);
}
