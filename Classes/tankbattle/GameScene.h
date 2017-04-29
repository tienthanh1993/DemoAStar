//
//  GameScene.h
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#ifndef GameScene_h
#define GameScene_h

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include "TankMap.h"
#include "Tank.h"
#include "Joystick.h"
#include "Building.h"
#include "HUDLayer.h"
#include "CountdownLayer.h"

class GameScene : public cocos2d::Layer {
public:
    //creator
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameScene);
    
    //constructor
    GameScene();
    virtual ~GameScene();
    //init
    virtual bool init();
    //normal
    
    void registerEvent();
    
    void unRegisterEvent();
    
    static GameScene* getInstance();
    
    void onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype);
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event);
    
    //override
    void onEnter();
    void onExit();
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    //register on dead event
    void onTankDead(cocos2d::Ref* sender);
    
    void onGameStart(cocos2d::Ref* sender);
    
    void mainLoop(float dt);
    void update(float dt) override;
    void setViewPointCenter(cocos2d::Point position);
 
    void addObject2Map(cocos2d::Node * obj);
    void removeObjectFromMap(cocos2d::Node * obj);
    
protected:
    //protected
private:
    //private
    void initGameParam();
    
    void initMap();
    
    void createPlayerTank();
    
    void initAnimation();
    
    void initUI();
    
    void initPlayerData();
    
    void initMission();
    
    void gameWin();
    
    void gameLose();
    
    void drawAim(cocos2d::Vec2 p1, cocos2d::Vec2 p2);
    

    int _keyPressing;
    

    //map
    CC_SYNTHESIZE(TankMap*,_tankMap,TankMap);
    
    
    //game object
    CC_SYNTHESIZE(cocos2d::Sprite*, _portal, Portal);
    CC_SYNTHESIZE(Tank*, _tankPlayer, TankPlayer);

    
    //player var
    CC_SYNTHESIZE(int, _money, Money);

    //game loops
    CC_SYNTHESIZE(int64_t, _gameLoops, GameLoop);
    CC_SYNTHESIZE(int64_t, _gameTime, GameTime);
    //game state
    CC_SYNTHESIZE(bool, _isPaused, IsPaused);
    CC_SYNTHESIZE(HUDLayer*, _hudLayer, HUDLayer);
    //basic ui & control

    CountdownLayer* _countDownLayer;
    
    
    CC_SYNTHESIZE(cocos2d::Layer*, _pathFinderLayer, PathFinderLayer);
    
    int _lifeLeft = 0;
    
    //mission info
    
    bool checkBranchMission(int missionId);
    
    CC_SYNTHESIZE(cocos2d::ValueMap, _missonInfo, MissionInfo);
    
    CC_SYNTHESIZE(int,_category,Category); // tank type
    
    
    CC_SYNTHESIZE(int,_delayAddEnemy,DelayAddEnemy); // enemy need to add
    
    CC_SYNTHESIZE(int,_totalEnemy,TotalEnemy); // total enemy need to kill
    
    CC_SYNTHESIZE(int,_retainNum,RetainNum); // concurrent enemy on map
    
    CC_SYNTHESIZE(int,_initNum,initNum); // concurrent enemy on map
    
    CC_SYNTHESIZE(int,_killedEnemy,killedEnemy); // enemy has killed
    
    CC_SYNTHESIZE(int,_currentEnemy,CurrentEnemy);// current enemy on map
    
    CC_SYNTHESIZE(int,_missionType,MissionType); // mission types
    
    CC_SYNTHESIZE(int,_enemyType,EnemyType); // enemy types
    
    CC_SYNTHESIZE(int,_mission1,Mission1); // 1 star time
    
    CC_SYNTHESIZE(int,_mission2,Mission2); // 2 star branch
    
    CC_SYNTHESIZE(int,_mission3,Mission3); // 3 star  tank
    
    CC_SYNTHESIZE(int,_enemyTankMoney,TankMoney);// tank money

    CC_SYNTHESIZE(bool,_isBoosKilled,IsBossKilled); // kill boss mission
    
    CC_SYNTHESIZE(bool,_isFriendKilled,IsFriendKilled); // protect friend mission
    
    CC_SYNTHESIZE(Tank*, _friendTank, FriendTank);
    
    CC_SYNTHESIZE(bool, _isBuildingDestroyed, IsBuildingDestroyed)// destroy building mission
    
    CC_SYNTHESIZE(Building*, _building, Building)// destroy building mission
};

#endif /* GameScene_h */
