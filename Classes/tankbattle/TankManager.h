//
//  TankManager.h
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#ifndef TankManager_h
#define TankManager_h

#include <cocos2d.h>
#include "Tank.h"

class TankManager {
public:
    TankManager();
    ~TankManager();
    static TankManager* _instance;
    static TankManager* getInstance();
    static void DestroyInstance();
    
    cocos2d::Vector<Tank*>& getTankList();
    Tank* getTank(int id);
    void removeTank(int id);
    void removeAllTank();
    int addTank(Tank* obj);
    bool loadTankAttr(Tank* tank,int level) ;
private:
    cocos2d::Vector<Tank*> _tankList;
};
#endif /* TankManager_h */
