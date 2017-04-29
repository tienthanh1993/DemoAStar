//
//  SceneFactory.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#include "SceneFactory.h"
#include "constants.h"
#include "LoadingScene.h"
#include "GameScene.h"
#include "LevelScene.h"


USING_NS_CC;

cocos2d::Scene* SceneFactory::create(int sceneType) {
    cocos2d::Scene* scene = nullptr;
    switch (sceneType) {
        case loading_scene:
            scene = LoadingScene::createScene();
        break;
        case menu_scene:
            
        break;
        case level_scene:
            scene = LevelScene::createScene();
            break;
        case game_scene:
            scene = GameScene::createScene();
        break;
        case shop_scene:
            
        break;
        default:
            scene = Scene::create();
            break;
    }
  return scene;
}
