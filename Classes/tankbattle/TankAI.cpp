//
//  TankAI.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/8/17.
//
//

#include "TankAI.h"
#include "constants.h"
#include "GameScene.h"
#include "TankManager.h"
#include "PathFinder.h"

USING_NS_CC;

float getAngleOfTwoVectors(cocos2d::Point vec1, cocos2d::Point vec2)
{
    auto vectorFromVec1ToVec2 = vec2 - vec1;
    // the angle between two vectors
    return  CC_RADIANS_TO_DEGREES(vectorFromVec1ToVec2.getAngle());
}


void TankAI::active(Node* tank) {
    
    _tank = dynamic_cast<Tank*>(tank);
    if (!_tank) {
        return;
    }
    if (_tank->getKind() == tank_player ) {
        processPlayer();
        return;
    }
    if (_tank->getKind() == tank_friend) {
        processFriendTank();
        return;
    }
    processAI(_tank->getAIMode());
}

void TankAI::processAI(int aiMode) {
    //currently only one mode
    switch (_tank->getAIState()) {
        case state_idle: // do nothing
            
            idle();
            break;
        case     state_roaming: // walk around
            roaming();
            break;
        case     state_fighting: // attack current enemy
            fighting();
            break;
        case     state_chasing: // follow enemy to attack
            chasing();
            break;
        case     state_hunting: // find enemy
            hunting();
            break;
        case     state_flee: // run around when low HP
            flee();
            break;
        default:
            changeState();
            break;
    }
}

void TankAI::processPlayer() {
    if (!_tank->getTarget())
    {
        int min = 999999;
        Tank* pTankMin = nullptr;
        for (auto tank : TankManager::getInstance()->getTankList()) {
            Tank* pTank = (Tank*)tank;
            
            if (pTank == _tank) {
                continue;
            }
            if (!pTank->isVisible()) {
                continue;
            }
            if (pTank->getKind() == tank_friend) {
                continue;
            }
            if (pTankMin == nullptr) {
                pTankMin = pTank;
                min = pTank->getPosition().getDistance(_tank->getPosition()) ;
                continue;
            }
            
            if (pTank->getPosition().getDistance(_tank->getPosition())< min) {
                min = pTank->getPosition().getDistance(_tank->getPosition()) ;
                pTankMin = pTank;
            }
        }
        if (pTankMin) {
            _tank->setTarget(pTankMin);
            
            return;
        } else {
            //            min = 999999;
            //            __Array* canDesArr = GAMESCENE->getTankMap()->getCanDes();
            //            Node* pObjMin = nullptr;
            //            for(int i=0;i < canDesArr->count();i++) {
            //                Node* pObj = (Node*)canDesArr->getObjectAtIndex(i);
            //
            //                if (!pObj->isVisible()) {
            //                    continue;
            //                }
            //
            //                if (pObjMin == nullptr) {
            //                    pObjMin = pObj;
            //                    min = pObj->getPosition().getDistance(_tank->getPosition()) ;
            //                    continue;
            //                }
            //
            //                if (pObj->getPosition().getDistance(_tank->getPosition())< min) {
            //                    min = pObj->getPosition().getDistance(_tank->getPosition()) ;
            //                    pObjMin = pObj;
            //                }
            //            }
            //            if (pObjMin) {
            //                _tank->setTarget((Tank*)pObjMin);
            //            }
        }
    } else {
        if(!_tank->getTarget()->isVisible()) {
            _tank->setAutoShot(false);
            _tank->setTarget(nullptr);
        } else {
            if (_tank->getAutoShot()) {
                float angle = getAngleOfTwoVectors(_tank->getPosition(),_tank->getTarget()->getPosition());
                _tank->sendCommand(do_shoot,-1,0,angle);
            }
        }
        
    }
    
    // auto aim enemy ?
    processRunPath();
}

void TankAI::processFriendTank() {
    processRunPath();
}

void TankAI::processRunPath() {
    if (_tank->getDesPos().isZero()) {
        //if path not empty
        // set despoint = next item
        if (_tank->getRunPath()&& !_tank->getRunPath()->empty()) {
            _tank->setDesPos(_tank->getRunPath()->front());
            
            CCLOG("run to [%lu] %f %f  - %f %f",_tank->getRunPath()->size(),_tank->getDesPos().x,_tank->getDesPos().y,_tank->getPosition().x,_tank->getPosition().y);
            if (_tank->getRunPath()->size()==1) {
                GAMESCENE->getPathFinderLayer()->removeAllChildren();
            }
            return;
        } else {
            
        }
    } else {
        //if circle collision
        if (_tank->getPosition().getDistance(_tank->getDesPos()) <= 5) {
            //set despoint = 0
            
            
            
            _tank->getRunPath()->pop_front();
            _tank->setDesPos(Vec2::ZERO);
            _tank->sendCommand(do_stand);
            //let next game loop find new des
            return;
        } else {
            //calc dir
            float angle = getAngleOfTwoVectors(_tank->getPosition(),_tank->getDesPos());
            //send command walk
            _tank->sendCommand(do_move,-1,0,angle);
        }
    }
}

bool TankAI::needChangeState() {
    if (_tank->getNextAITime() < GAMESCENE->getGameLoop()) {
        return true;
    }
    return false;
}

void TankAI::changeState(int state) {
    int state1 = state_roaming;
    int state2 = state_hunting;
    int newState = RandomHelper::random_int(state1, state2);
    if (state != -1) {
        newState = state;
    }
    if (_tank->getAIState() == state_roaming) {
         _tank->sendCommand(do_stand);
        _tank->setDesPos(Vec2::ZERO );
    }
    if (_tank->getAIState() == state_chasing) {
         _tank->sendCommand(do_stand);
        _tank->setRunPath(nullptr);
        _tank->setDesPos(Vec2::ZERO);
    }
    // CCLOG("newstate = %d",newState);
    _tank->setAIState(newState);
    _tank->setNextAITime(GAMESCENE->getGameLoop() + _tank->getMaxAITime());
}

bool TankAI::inEyeShoot() {
    // check if bullet can hurt enemy
    if (!_tank->getTarget()) {
        return false;
    }
    if (!_tank->getTarget()->isVisible()) {
        return false;
    }
    if (_tank->getKind() == tank_boss) {
        return true;
    }
//    Point positionOrigin = _tank->getPosition();
//    float angle = _tank->getPosition().getAngle(_tank->getTarget()->getPosition());
//    Point positionNext = positionOrigin + Vec2(3.0f*cos(CC_DEGREES_TO_RADIANS(angle)),3.0f*sin(CC_DEGREES_TO_RADIANS(angle)));
   GameScene* game = GAMESCENE;
    
    //    while (!_tank->getTarget()->getBoundingBox().containsPoint(positionNext)) {
    //
    //        //can't destroy object
    //        __Array* arr = game->getTankMap()->getCannotDes();
    //        for(int i=0;i < arr->count();i++) {
    //            Node* nodeObj = (Node*)arr->getObjectAtIndex(i);
    //            if (nodeObj->isVisible() && nodeObj->getBoundingBox().containsPoint(positionNext)) {
    //                return false;
    //            }
    //        }
    //         positionOrigin = positionNext;
    //          positionNext = positionOrigin + Vec2(3.0f*cos(CC_DEGREES_TO_RADIANS(angle)),3.0f*sin(CC_DEGREES_TO_RADIANS(angle)));
    //
    //    }
    
    Vec2 p1 = _tank->getPosition();
    Vec2 p2 = _tank->getTarget()->getPosition();
    
    float x = p1.x;
    float y = p1.y;
    
    float vectorX = p2.x - p1.x;
    float vectorY = p2.y - p1.y;
    
    
    for (int i = 0; i < 9;i++)
    {
        x += vectorX/10;
        y += vectorY/10;
        
        
        for (int j = 0;j < game->getTankMap()->getCanDes()->count();j++)
        {
            Sprite* sp = (Sprite*)game->getTankMap()->getCanDes()->getObjectAtIndex(j);
            if (sp->isVisible() && !sp->getPosition().equals(p2) && sp->getBoundingBox().containsPoint(Vec2(x,y)))
            {
                return false;
                break;
            }
        }
        for (int k = 0;k < game->getTankMap()->getCannotDes()->count();k++)
        {
            Sprite* sp = (Sprite*)game->getTankMap()->getCannotDes()->getObjectAtIndex(k);
            if (sp->isVisible() && !sp->getPosition().equals(p2)&& sp->getBoundingBox().containsPoint(Vec2(x,y)))
            {
                return false;
                break;
            }
        }
    }
    
    x = p1.x;
    y = p1.y;
    
    return true;
}

void TankAI::flee() {
    if (needChangeState()) {
        changeState();
        return;
    }
    
    //
    if (!inEyeShoot()) {
        changeState();
        return;
    }
    if (_tank->getDesPos().isZero()) {
        //if path not empty
        // set despoint = next item
        if (!_tank->getRunPath()->empty()) {
            _tank->setDesPos(_tank->getRunPath()->front());
            _tank->getRunPath()->pop_front();
            return;
        } else {
            // get random nearby position
            Vec2 newPosition = PathFinder::getInstance()->findNearbyPos(_tank->getPosition(), 4);Vec2(RandomHelper::random_int(200, 1200),RandomHelper::random_int(0,720));
            //set to despos
            _tank->setDesPos(newPosition);
        }
    } else {
        //if circle collision
        if (_tank->getBoundingBox().intersectsCircle(_tank->getDesPos(), 10)) {
            //set despoint = 0
            _tank->setDesPos(Vec2::ZERO);
            //let next game loop find new des
            return;
        } else {
            //calc dir
            float angle = getAngleOfTwoVectors(_tank->getPosition(),_tank->getDesPos());
            //send command walk
            _tank->sendCommand(do_move,-1,0,angle);
        }
    }
}

void TankAI::roaming() {
    if (needChangeState()) {
        changeState();
        return;
    }
    if(_tank->getDirection() == 181 || RandomHelper::random_int(0, 1000) < 10) {
        int dir[4] = {0,90,180,-90};
        _tank->setDirection(dir[RandomHelper::random_int(0, 3)]);
    } else {
        _tank->sendCommand(do_move,-1,0,_tank->getDirection());
        if(RandomHelper::random_int(1, 100)<5) {
            //            CCLOG("do shoot  2 = %01f",_tank->getDirection());
            _tank->sendCommand(do_shoot,-1,0,_tank->getDirection());
        }
    }
}
//
void TankAI::chasing() {
    if (needChangeState()) {
        changeState();
        return;
    }
    if (!_tank->getTarget()) {
        return;
    }
    if (inEyeShoot()) {
        changeState(state_fighting);
        return;
    }
    //follow enemy
    // get position
    // run to position
    if (_tank->getRunPath()&&_tank->getRunPath()->size() > 0) {
         processRunPath();
        return;
    }
    
    auto path = PathFinder::getInstance()->findWalkablePathAstar(_tank->getPosition(), _tank->getTarget()->getPosition());
    std::deque<Vec2> *queue = new std::deque<Vec2>();
    for(int i=path.size()-1;i >=0;i-- ) {
        queue->push_back(path.at(i));
    }
    _tank->setDesPos(Vec2::ZERO);
    _tank->setRunPath(queue);
//    float angle = getAngleOfTwoVectors(_tank->getPosition(),_tank->getTarget()->getPosition());
//    //send command walk
//    _tank->sendCommand(do_move,-1,0,angle);
    
}

void TankAI::hunting() {
    if (needChangeState()) {
        changeState();
        return;
    }
    
    Tank* targetTank = nullptr;
    int minDistance = 1000000;
    for (auto tank : TankManager::getInstance()->getTankList()) {
        Tank* pTank = (Tank*)tank;
        if (pTank == _tank) {
            continue;
        }
        if (pTank->getKind() == _tank->getKind()) {// do not shoot friend tank
            // if player need this must check tank team
            //CCLOG("no enemy");
            continue;
        }
        if (!pTank->isVisible()) {
            continue;
        }
        //check if target tank not dead
        
        float distance = pTank->getPosition().getDistanceSq(_tank->getPosition());
        if (distance< minDistance) {
            minDistance = distance;
            targetTank = pTank;
        }
    }
    if (targetTank != nullptr) {
        _tank->setTarget(targetTank);
        changeState(state_chasing); // start chasing this enemy
    } else {
        changeState(state_roaming);
    }
}

void TankAI::fighting() {
    if (needChangeState()) {
        changeState();
        return;
    }
    
    Tank* targetTank = _tank->getTarget();
    if (targetTank != nullptr) {
        if (/*tank is dead or */ !targetTank->isVisible()) {
            _tank->setTarget(nullptr);
            changeState(state_hunting);
            return;
        } else if (!inEyeShoot()) {
            changeState(state_chasing);
            return;
        } else {
            _tank->sendCommand(do_stand);
            float angle = getAngleOfTwoVectors(_tank->getPosition(),targetTank->getPosition());
            //shoot
            _tank->sendCommand(do_shoot,-1,0,angle);
            //            CCLOG("do shoot %01f",angle);
        }
    } else {
        changeState(state_hunting);
        return;
    }
}

void TankAI::idle() {
    
    changeState();
}
