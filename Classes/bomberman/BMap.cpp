//
//  BMap.cpp
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#include "BMap.h"
USING_NS_CC;


BMap::BMap() {
    
}

BMap::~BMap() {
    
}

bool BMap::init() {
    if (!Layer::init()) {
        return false;
    }
    return true;
}

void BMap::registerEvent() {
    
}

void BMap::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);}

void BMap::onEnter() {
    Layer::onEnter();
    registerEvent();
}

void BMap::onExit() {
    Layer::onExit();
    unRegisterEvent();
}

