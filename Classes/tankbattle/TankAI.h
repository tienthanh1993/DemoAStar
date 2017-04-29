//
//  TankAI.h
//  TankBattle
//
//  Created by Pham Thanh on 3/8/17.
//
//

#ifndef TankAI_h
#define TankAI_h

#include <cocos2d.h>
#include "Tank.h"

class TankAI {
public:
    void active(cocos2d::Node* tank);
    void processAI(int aiMode);
    void processPlayer();
    
    void processFriendTank();
private:
   void processRunPath();
    
    bool needChangeState();
    void changeState(int state = -1);
    
    bool inEyeShoot();
    
    void flee();
    void roaming();
    void chasing();
    void hunting();
    void fighting();
    void idle();
    
    Tank* _tank;
    
};

#endif /* TankAI_h */
