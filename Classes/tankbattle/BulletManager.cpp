//
//  BulletManager.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#include "BulletManager.h"
#include "Bullet.h"

USING_NS_CC;
static int unique_bullet_id = 1;
USING_NS_CC;
BulletManager* BulletManager::_instance = nullptr;

BulletManager::BulletManager() {
    
}

BulletManager::~BulletManager() {
    
}

BulletManager* BulletManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new BulletManager();
    }
    return _instance;
}

void BulletManager::DestroyInstance() {
    delete _instance;
}

cocos2d::Vector<cocos2d::Node*>& BulletManager::getBulletList() {
    return _bulletList;
}

cocos2d::Node* BulletManager::getBullet(int id) {
    for (int i=0;i< _bulletList.size();i++) {
        if ( ((Bullet*)_bulletList.at(i))->getId() == id) {
            return _bulletList.at(i);
        }
    }
    return nullptr;
}

void BulletManager::removeBullet(int id) {
    for (int i=0;i< _bulletList.size();i++) {
        if ( ((Bullet*)_bulletList.at(i))->getId() == id) {
            _bulletList.erase(i);
            return;
        }
    }
    CCLOG("no object to remove %d",id);
}
void BulletManager::removeAllBullet() {
    _bulletList.clear();
}
int BulletManager::addBullet(cocos2d::Node* obj) {
    unique_bullet_id++;
    ((Bullet*)obj)->setId((unique_bullet_id));
    _bulletList.pushBack(obj);
    return unique_bullet_id;
}
