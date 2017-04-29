#include "HelloWorldScene.h"
#include "tankbattle/SceneFactory.h"
#include "tankbattle/constants.h"
#include "rpg/RGameScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

  
    
    
    auto tankBattle = MenuItemLabel::create(Label::createWithTTF("Demo Tank Battle", "fonts/arial.ttf", 72),[&](Ref* sender) {
        
        
        auto scene = SceneFactory::create(loading_scene);
        
        // run
        Director::getInstance()->replaceScene(scene);
        CCLOG("enter tank battle demo");
    });
    
    auto bomberMan = MenuItemLabel::create(Label::createWithTTF("Demo Bomberman", "fonts/arial.ttf", 72),[&](Ref* sender) {
        
        CCLOG("enter bomber man demo");
    });
    
    
    auto towerDefense = MenuItemLabel::create(Label::createWithTTF("Demo TowerDefense", "fonts/arial.ttf", 72),[&](Ref* sender) {
        
        CCLOG("enter tower defense demo");
    });
    
    auto rpg = MenuItemLabel::create(Label::createWithTTF("Demo RPG 2.5d", "fonts/arial.ttf", 72),[&](Ref* sender) {
        
        
        Director::getInstance()->replaceScene(RGameScene::createScene());
        CCLOG("enter rpg2.5d");
    });
    
    auto gameMenu = Menu::create(tankBattle,rpg, NULL);
    gameMenu->alignItemsVerticallyWithPadding(30);
    menu->setPosition(visibleSize/2);
    menu->alignItemsHorizontally();
    this->addChild(gameMenu, 1);
    
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
