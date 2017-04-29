//
//  SoundManager.cpp
//  FarmConnect
//
//  Created by Pham Thanh on 10/20/16.
//
//

#include "SoundManager.h"
#include <SimpleAudioEngine.h>
#define SOUND CocosDenshion::SimpleAudioEngine::getInstance()
USING_NS_CC;

static SoundManager* _instance = nullptr;
SoundManager* SoundManager::getInstance() {
    if (_instance == nullptr) {
        _instance = new SoundManager();
        
    }
    return _instance;
}

SoundManager::SoundManager() {
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5f);
}

SoundManager::~SoundManager() {
    
}

bool SoundManager::isEffect() {
    return UserDefault::getInstance()->getBoolForKey("effect", true);
}

bool SoundManager::isBackground() {
     return UserDefault::getInstance()->getBoolForKey("background", true);
}

void SoundManager::setEffectEnable(bool enable) {
    UserDefault::getInstance()->setBoolForKey("effect", enable);
}
void SoundManager::setBackgroundEnable(bool enable) {
    if (!enable) {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    } else {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
       UserDefault::getInstance()->setBoolForKey("background", enable);
}

void SoundManager::playEffect(const char* pszFilePath) {
    if(UserDefault::getInstance()->getBoolForKey("effect", true)) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pszFilePath);
    }
}

void SoundManager::playBackground(const char* pszFilePath,bool repeat) {
    if(UserDefault::getInstance()->getBoolForKey("background", true)) {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath,repeat);
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
    }
}

void SoundManager::stopBackground() {
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void SoundManager::pause() {
     CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void SoundManager::resume() {
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}
