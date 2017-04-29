//
//  BHUDLayer.cpp
//  DemoAStar
//
//  Created by Pham Thanh on 4/26/17.
//
//

#include "BHUDLayer.h"
USING_NS_CC;


BHUDLayer::BHUDLayer() {
    
}

BHUDLayer::~BHUDLayer() {
    
}

bool BHUDLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    return true;
}

void BHUDLayer::registerEvent() {
    
}

void BHUDLayer::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);}

void BHUDLayer::onEnter() {
    Layer::onEnter();
    registerEvent();
}

void BHUDLayer::onExit() {
    Layer::onExit();
    unRegisterEvent();
}

