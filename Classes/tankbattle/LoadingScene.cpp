//
//  LoadingScene.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#include "LoadingScene.h"
#include "constants.h"
#include "SceneFactory.h"
#include "AnimationHelper.h"
#include "GameManager.h"

USING_NS_CC;
using namespace ui;
cocos2d::Scene* LoadingScene::createScene() {
    auto scene = Scene::create();
    
    auto layer = LoadingScene::create();
    
    scene->addChild(layer);
    
    return scene;
}
//constructor
LoadingScene::LoadingScene() {
    
}

LoadingScene::~LoadingScene() {
    
}

//init
bool LoadingScene::init() {
    if (!Layer::init()) {
        return false;
    }
    Size winSize = Director::getInstance()->getWinSize();
    _background = Sprite::create("res/ui/background.png");
    _background->setPosition(winSize/2);
    addChild(_background);
    
    _loadingProgress = Label::createWithTTF("0%", "fonts/arial.ttf", 64);
    _loadingProgress->setPosition(Vec2(winSize.width*0.9,winSize.height*0.1));
    addChild(_loadingProgress);
    
    
    return true;
    
}
//normal

void LoadingScene::registerEvent() {
    
}

void LoadingScene::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void LoadingScene::onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }
}

void LoadingScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event) {
    
}

void LoadingScene::onResourcesLoaded(Texture2D* texture) {
    std::string filePath = texture->getPath();
    filePath.erase(filePath.end() - 3, filePath.end());
    filePath = filePath.append("plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filePath.c_str(), texture);
    
    CCLOG("loaded %s %d/%d",filePath.c_str(),_totalResourcesLoaded,TOTAL_RESOUCE -1);
    _loadingProgress->setString(StringUtils::format("%d%%",_totalResourcesLoaded*100/(TOTAL_RESOUCE-1)));
    if (_totalResourcesLoaded == TOTAL_RESOUCE/2) {
        AnimationHelper::addAnimation("fireeffect",FIRE_EFFECT,"kp",3);
        AnimationHelper::addAnimation("boom",BOOM,"",11);
        AnimationHelper::addAnimation("enemytankboom",ENEMY_BOOM,"kp",10);
        AnimationHelper::addAnimation("bossboom",BOSS_BOOM,"bossboom",6);
        AnimationHelper::addAnimation("planeboom",PLANE_BOOM,"boomplane",11);
        AnimationHelper::addAnimation("playerAppear",PLAYER_APPEAR,"playerAppear",11);
        AnimationHelper::addAnimation("enemyAppear",ENEMY_APPEAR,"enemyAppear",11);
        AnimationHelper::addAnimation("jetone",JETONE,"jet",3);
        AnimationHelper::addAnimation("jettwo",JETTWO,"jettwo",2);
        AnimationHelper::addAnimation("roadblock",ROADBLOCK,"roadblock",9);
        AnimationHelper::addAnimation("flag",FLAG,"flag",3);
        AnimationHelper::addAnimation("portal",PORTAL,"portal",9);
        AnimationHelper::addAnimation("prop",PROP_EFFECT,"propefect",17);
        GameManager::getInstance();
    }
    
    if (_totalResourcesLoaded == TOTAL_RESOUCE -1) {
        GameManager::getInstance()->setLevel(1);
        CCLOG("loading success %d/%d",_totalResourcesLoaded,TOTAL_RESOUCE-1);
        Director::getInstance()->replaceScene(SceneFactory::create(level_scene));
    }
    
    _totalResourcesLoaded++;

}

//override
void LoadingScene::onEnter() {
    Layer::onEnter();
    registerEvent();
    
    
    auto textureCache = Director::getInstance()->getTextureCache();
    
    for (int i=0; i< TOTAL_RESOUCE; i++) {
        textureCache->addImageAsync(StringUtils::format("%s.png",gResources[i]), CC_CALLBACK_1(LoadingScene::onResourcesLoaded,this));
    }
    
}

void LoadingScene::onExit() {
    Layer::onExit();
    unRegisterEvent();
}
