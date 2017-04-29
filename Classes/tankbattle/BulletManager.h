//
//  BulletManager.h
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#ifndef BulletManager_h
#define BulletManager_h

#include <cocos2d.h>
class BulletManager {
public:
    BulletManager();
    ~BulletManager();
    static BulletManager* _instance;
    static BulletManager* getInstance();
    static void DestroyInstance();
     void removeAllBullet();
    cocos2d::Vector<cocos2d::Node*>& getBulletList();
    cocos2d::Node* getBullet(int id);
    void removeBullet(int id);
    int addBullet(cocos2d::Node* obj);
private:
    cocos2d::Vector<cocos2d::Node*> _bulletList;
};
#endif /* BulletManager_h */
