//
//  TankManager.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#include "TankManager.h"
#include "Tank.h"

USING_NS_CC;
static int unique_tank_id = 1;
USING_NS_CC;
TankManager* TankManager::_instance = nullptr;

TankManager::TankManager() {
    
}

TankManager::~TankManager() {

}

TankManager* TankManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new TankManager();
    }
    return _instance;
}

void TankManager::DestroyInstance() {
    delete _instance;
}

cocos2d::Vector<Tank*>& TankManager::getTankList() {
    return _tankList;
}

Tank* TankManager::getTank(int id) {
    for (int i=0;i< _tankList.size();i++) {
        if (((Tank*)_tankList.at(i))->getId() == id) {
            return _tankList.at(i);
        }
    }
    return nullptr;
}

void TankManager::removeTank(int id) {
    for (int i=0;i< _tankList.size();i++) {
        if (((Tank*)_tankList.at(i))->getId() == id) {
            _tankList.erase(i);
            return;
        }
    }
    CCLOG("no object to remove %d",id);
}
void TankManager::removeAllTank() {
    _tankList.clear();
}
int TankManager::addTank(Tank* obj) {
    unique_tank_id++;
    ((Tank*)obj)->setId(unique_tank_id);
    
    
    _tankList.pushBack(obj);
    return unique_tank_id;
}
float g_attackFactor[12] = {0.6f,0.8f,0.8f,2.0f,2.8f,2.5f,4.2f,4.5f,4.5f,6.0f,6.0f,6.0f};
float g_defenceFactor[12] = {0.3f,0.3f,0.3f,0.3f,0.3f,0.2f,0.5f,0.4f,0.3f,0.5f,0.4f,0.3f};
float g_hpFactor[12] = {0.5f,0.4f,0.5f,0.5f,0.4f,0.5f,0.6f,0.6f,0.4f,0.6f,0.6f,0.4f};

int g_baseAttack[4] = {5,5,6,4};
int g_baseDefense[4] = {5,5,6,6};
int g_baseHP[4] = {10,10,8,10};

bool TankManager::loadTankAttr(Tank* tank,int level) {
    int ai = tank->getAIMode();
    
    float attack = 5 ;
    float defense = 5;
    float hp = 10;
    
    
    int param4 = 0;
    int param3 = 0;
    int param2 = 0;
    int param1 = 0;
    
    if(level > 0 && level <21 ){
        param1 = 19;
        param2 = param3 = param4 = 0;
    } else  if(level > 20 && level <31 ){
        param1 = 19;
        param2 = level - 20;
        param3 = param4 = 0;
    } else  if(level > 30 && level <41 ){
        param1 = 19;
        param2 = 10;
        param3 = level - 30;
        param4 = 0;
    } else  if(level > 40 && level <51 ){
        param1 = 19;
        param2 = 10;
        param3 = 10;
        param4 = level - 40;
    }
    attack = g_baseAttack[ai-1];
    defense = g_baseDefense[ai-1];
    hp = g_baseHP[ai-1];
    if(ai == 1){
        attack += param1*g_attackFactor[2] + param2*g_attackFactor[5] + param3*g_attackFactor[8] + param4*g_attackFactor[11];
        defense += param1*g_defenceFactor[2] + param2*g_defenceFactor[5] + param3*g_defenceFactor[8] + param4*g_defenceFactor[11];
        hp += param1*g_hpFactor[2] + param2*g_hpFactor[5] + param3*g_hpFactor[8] + param4*g_hpFactor[11];
    }
    else if(ai == 2){
        attack += param1*g_attackFactor[1] + param2*g_attackFactor[4] + param3*g_attackFactor[7] + param4*g_attackFactor[10];
        defense += param1*g_defenceFactor[1]+ param2*g_defenceFactor[4] + param3*g_defenceFactor[7] + param4*g_defenceFactor[10];
        hp += param1*g_hpFactor[1]+ param2*g_hpFactor[4] + param3*g_hpFactor[7] + (level-1)*g_hpFactor[10];
    }else if(ai == 3){
        attack += param1*g_attackFactor[0] + param2*g_attackFactor[3] + param3*g_attackFactor[6] + param4*g_attackFactor[9];
        defense += param1*g_defenceFactor[0] + param2*g_defenceFactor[3] + param3*g_defenceFactor[6] + param4*g_defenceFactor[9];
        hp += param1*g_hpFactor[0] + param2*g_hpFactor[3] + param3*g_hpFactor[6] + param4*g_hpFactor[9];
    }
    tank->setLife(hp);
    tank->setLifeMax(hp);
    tank->setAttack(attack);
    tank->setDefense(defense);
    CCLOG("life:%f, attack %f, defense %f",hp,attack,defense);
    
    return true;
}
