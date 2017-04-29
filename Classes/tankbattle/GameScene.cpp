//
//  GameScene.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#include "GameScene.h"
#include "GameManager.h"
#include "AnimationHelper.h"
#include "PathFinder.h"
#include "TankManager.h"
#include "constants.h"
#include "CountdownLayer.h"
#include "SceneFactory.h"
#include "GameWinLayer.h"
#include "GameLose.h"
#include "PauseLayer.h"
#include "BulletManager.h"

#include "utils/SoundManager.h"

#include "Bullet.h"
#include "Building.h"
#include "HUDLayer.h"
USING_NS_CC;
using namespace ui;
static GameScene *g_GameScene = nullptr;

cocos2d::Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto gscene = GameScene::create();
    gscene->setTag(1);
    //g_GameScene->setTag(1);
    scene->addChild(gscene);
    
    return scene;
}
//constructor
GameScene::GameScene() {
    _keyPressing = 0;
    _delayAddEnemy = 0;
    _isBuildingDestroyed = false;
    _isFriendKilled = false;
    _building = nullptr;
    _portal = nullptr;
    _friendTank = nullptr;
}

GameScene::~GameScene() {
    //    TankManager::getInstance()->removeAllTank();
    //    BulletManager::getInstance()->removeAllBullet();
    
    
}
GameScene* GameScene::getInstance() {
    if (g_GameScene) {
        return g_GameScene;
    }
    return nullptr;
}

//init
bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }
    
    //    //init parameter
    initGameParam();
    //
    //    //init effect
    initAnimation();
    //
    //
    //    //init map
    initMap();
    //    CCLOG("1");
    
    //    //set default player data
    initPlayerData();
    //    CCLOG("2");
    createPlayerTank();
    //    CCLOG("3");
    initMission();
    //    CCLOG("4");
    initUI();
    //    CCLOG("5");
    
    
      _countDownLayer = CountdownLayer::create();
      addChild(_countDownLayer);
    
    return true;
}

void GameScene::initMission() {
    //init mission
    int curLevel = GameManager::getInstance()->getLevel();
    
    _missonInfo = GameManager::getInstance()->getMission(curLevel);
    
    
    
    _category = _missonInfo.at("category").asInt();// tank type
    _totalEnemy = _missonInfo.at("count").asInt() ;// total tank
    _retainNum =  _missonInfo.at("retainnum").asInt(); // concurrent tank on map
    _initNum =  _missonInfo.at("initnum").asInt(); // concurrent tank on map
    _missionType =_missonInfo.at("missiontype").asInt(); // mission type
    _enemyTankMoney=_missonInfo.at("money").asInt();// money drop
    
    _killedEnemy = 0;
    
    _currentEnemy = 0;
    
    
    _mission1 =_missonInfo.at("mission1").asInt(); //branch mission 1
    _mission2 =_missonInfo.at("mission2").asInt(); //branch mission 2
    _mission3 =_missonInfo.at("mission3").asInt(); //branch mission 3
    
    if (_missionType == kKillBoss) {
        int bossLevel = _missonInfo.at("boss").asInt();
        _currentEnemy++;
        _totalEnemy++;
        auto boss = Tank::createTankBoss(bossLevel);
        boss->setPosition(_tankMap->getBossPoint());
        boss->setAIMode(1);
        boss->setMaxAITime(GAME_FPS*15);//5s 1 lan
        addObject2Map(boss);
        SoundManager::getInstance()->playBackground("res/sound/music_level_boss.mp3");
    }
    
    if (_mission2 == kBProtectTroops) {
        _friendTank = Tank::createFriendUnit();
        _friendTank->setPosition(((Node*)_tankMap->getRoadPoint()->getObjectAtIndex(0))->getPosition());
        
        
        
        std::deque<Vec2> *queue = new std::deque<Vec2>();
        for(int i=0;i < _tankMap->getRoadPoint()->count();i++ ) {
            queue->push_back(((Node*)_tankMap->getRoadPoint()->getObjectAtIndex(i))->getPosition());
        }
        
        _friendTank->setDesPos(Vec2::ZERO);
        _friendTank->setRunPath(queue);
        addObject2Map(_friendTank);
        SoundManager::getInstance()->playBackground("res/sound/music_level_b.mp3");
        
    }
    if (_mission2 == kBDestroyBuilding) {
        _building = Building::create(GameManager::getInstance()->getLevel());
        _building->setAnchorPoint(Vec2(0.5f,0.5f) );
        _building->setPosition(_tankMap->getBuildingPoint() + _building->getContentSize()/2);
        addObject2Map(_building);
        SoundManager::getInstance()->playBackground("res/sound/music_level_a.mp3");
        
    }
    
    _enemyType = _missonInfo.at("enemy1").asInt();
    for (; _currentEnemy <= _initNum; _currentEnemy++) {
        Tank* bot = Tank::createTankEnemy(_enemyType);
        TankManager::getInstance()->loadTankAttr(bot, GameManager::getInstance()->getLevel());
        bot->setMaxAITime(GAME_FPS*15);//5s 1 lan
        bot->setPosition(_tankMap->getSpawns()[_currentEnemy]);
        CCLOG("add new tank %d",_currentEnemy);
        bot->appear();
        addObject2Map(bot);
    }
    
}
//normal

void GameScene::registerEvent() {
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameScene::onTankDead), "onTankDead", nullptr);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameScene::onGameStart), "onGameStart", nullptr);
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    listener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event){
        _keyPressing--;
        if (_keyPressing) {
            return;
        }
        switch (code) {
            case cocos2d::EventKeyboard::KeyCode::KEY_W:
                _tankPlayer->sendCommand(do_stand, -1, 0, 90);
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                _tankPlayer->sendCommand(do_stand, -1, 0, 270);
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                _tankPlayer->sendCommand(do_stand, -1, 0, 180);
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                _tankPlayer->sendCommand(do_stand, -1, 0, 0);
                break;
            default:
                break;
        }
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan     = CC_CALLBACK_2(GameScene::onTouchBegan,this);
    touchListener->onTouchMoved     = CC_CALLBACK_2(GameScene::onTouchMoved,this);
    touchListener->onTouchEnded     = CC_CALLBACK_2(GameScene::onTouchEnded,this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled,this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener,this);
    
    
}

void GameScene::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void GameScene::onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event) {
    
    _keyPressing++;
    switch (code) {
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            _tankPlayer->sendCommand(do_move, -1, 0, 90);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            _tankPlayer->sendCommand(do_move, -1, 0, 270);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            _tankPlayer->sendCommand(do_move, -1, 0, 180);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            _tankPlayer->sendCommand(do_move, -1, 0, 0);
            break;
        default:
            break;
    }
}

//override
void GameScene::onEnter() {
    Layer::onEnter();
    registerEvent();

    _lifeLeft = UserDefault::getInstance()->getIntegerForKey("lifeLeft",0);
    
    setViewPointCenter(_tankPlayer->getPosition());

}

void GameScene::onExit() {
    Layer::onExit();
    unRegisterEvent();
    
}


bool GameScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    
    return true;
}

void GameScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    
}

void GameScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    
    _pathFinderLayer->removeAllChildren();
    Point test = _tankMap->convertTouchToNodeSpace(pTouch);
    
    
    for (auto bot : TankManager::getInstance()->getTankList()) {
        if(bot->isVisible()&& bot != _tankPlayer && ((Tank*)bot)->getBoundingBox().containsPoint(test)) {
            CCLOG("shoot this tank");
            _tankPlayer->setDesPos(Vec2::ZERO);
            _tankPlayer->setRunPath(nullptr);
            _tankPlayer->setTarget((Tank*)bot);
            float angle = getAngleOfTwoVectors(_tankPlayer->getPosition(),bot->getPosition());
            //shoot
            _tankPlayer->sendCommand(do_shoot,-1,0,angle);
            _tankPlayer->setAutoShot(true);
            return;
        }
    }
    
    __Array* canDesArr = _tankMap->getCanDes();
    for(int i=0;i < canDesArr->count();i++) {
        Node* nodeObj = (Node*)canDesArr->getObjectAtIndex(i);
        if (nodeObj->isVisible() && nodeObj->getBoundingBox().containsPoint(test)) {
            CCLOG("shot obs");
            _tankPlayer->setDesPos(Vec2::ZERO);
            _tankPlayer->setRunPath(nullptr);
            _tankPlayer->setTarget((Tank*)nodeObj);
            float angle = getAngleOfTwoVectors(_tankPlayer->getPosition(),nodeObj->getPosition());
            _tankPlayer->sendCommand(do_shoot,-1,0,angle);
            _tankPlayer->setAutoShot(true);
            return;
        }
    }
    if(getBuilding() ) {
        if (getBuilding()->isVisible() && getBuilding()->getBoundingBox().containsPoint( test)) {
            CCLOG("shot obs");
            _tankPlayer->setDesPos(Vec2::ZERO);
            _tankPlayer->setRunPath(nullptr);
            _tankPlayer->setTarget((Tank*)getBuilding() );
            float angle = getAngleOfTwoVectors(_tankPlayer->getPosition(),getBuilding() ->getPosition() + getBuilding()->getContentSize()/2);
            _tankPlayer->sendCommand(do_shoot,-1,0,angle);
            _tankPlayer->setAutoShot(true);
            return;
        }
    }
    _tankPlayer->setTarget(nullptr);
    bool findPath = true;
    if (_portal&&_portal->getBoundingBox().containsPoint(test)) {
        findPath = true;
    }
//    for (auto prop : PropsManager::getInstance()->getPropList()) {
//        if(prop->isVisible()&&  prop->getBoundingBox().containsPoint(test + prop->getContentSize()/2)) {
//            findPath = true;
//            break;
//        }
//    }
    
    if (findPath) {
        auto path = PathFinder::getInstance()->findWalkablePathAstar(_tankPlayer->getPosition(),test);
        std::deque<Vec2> *queue = new std::deque<Vec2>();
        for(int i=path.size()-1;i >=0;i-- ) {
            queue->push_back(path.at(i));
            auto spr = Sprite::createWithSpriteFrameName(DOT_WHITE);
            spr->setPosition(path.at(i));
            _pathFinderLayer->addChild(spr);
        }
        _tankPlayer->setDesPos(Vec2::ZERO);
        _tankPlayer->setRunPath(queue);
        return;
    }
    //set run path to this
    _tankPlayer->setRunPath(nullptr);
    _tankPlayer->setDesPos(Vec2::ZERO);
    
    CCLOG("no tank %f %f %f %f",pTouch->getLocation().x,pTouch->getLocation().y,test.x,test.y);
    
}

void GameScene::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    
    
}

void GameScene::onGameStart(cocos2d::Ref* sender) {
    CCLOG("on game started");

    this->scheduleUpdate();
    this->resume();
    
    
    for (int i = TankManager::getInstance()->getTankList().size() - 1; i >=0 ; i--) {
        Tank* tank = TankManager::getInstance()->getTankList().at(i);
        tank->appear();
    }
     //schedule(schedule_selector(GameScene::mainLoop), 1.0f/60.0f);
}
void GameScene::onTankDead(cocos2d::Ref* sender) {
    
    if (!sender) {
        CCLOG("no sender tank dead");
        return;
    }
    CCLOG("dead callback");
    Tank* deadTank= (Tank*)sender;
    
    
    int isAddCoin = rand()%10;
    if(isAddCoin %2 == 0){
        Size size= Director::getInstance()->getWinSize();
        Label *enemyTankMoney=Label::createWithBMFont("fonts/Matura.fnt",StringUtils::format("%d",_enemyTankMoney).c_str());
        enemyTankMoney->setPosition(deadTank->getPosition());
        enemyTankMoney->runAction(MoveTo::create(0.4f,Vec2(size.width*0.30,size.height*0.95)));
        ScaleTo *scale0=ScaleTo::create(0.3f,1.5f);
        ScaleTo *scale1=ScaleTo::create(0.3f,0.9f);
        Sequence *goldSequence=Sequence::create(scale0,scale1,RemoveSelf::create(true),NULL);
        enemyTankMoney->runAction(goldSequence);
        addChild(enemyTankMoney,1,101);
        
        _money += _enemyTankMoney;

    }
    
}



void GameScene::setViewPointCenter(Point position) {
    
    Size winSize = Director::getInstance()->getWinSize();
    
    float x = MAX(position.x, winSize.width/2);
    float y = MAX(position.y, winSize.height/2);
    x = MIN(x, ( (_tankMap->getMapWidth() )) - winSize.width / 2);
    y = MIN(y, (_tankMap->getMapHeight()) - winSize.height/2);
    Point actualPosition = Point(x, y);
    
    Point centerOfView = Point(winSize.width/2, winSize.height/2);
    Point viewPoint = (centerOfView- actualPosition);
    if (actualPosition.x < winSize.width/2) {
        return;
    }
    _tankMap->setPosition(viewPoint);
}

void GameScene::addObject2Map(cocos2d::Node * obj) {
    _tankMap->addChild(obj);
}

void GameScene::removeObjectFromMap(cocos2d::Node * obj) {
    _tankMap->removeChild(obj);
}
void GameScene::mainLoop(float dt)  {
    
}

void GameScene::update(float dt) {
    Layer::update(dt);
    _gameLoops++;
    _gameTime++;
    
    if(_tankPlayer->getPositionX() >= Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width/2.0f && _tankPlayer->getPositionX() <= 1280 - Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width/2) {
        setViewPointCenter(_tankPlayer->getPosition());
    }

    if (_tankPlayer && _tankPlayer->getTarget()&& _tankPlayer->getTarget()->isVisible()) {
        drawAim(_tankPlayer->getPosition(),_tankPlayer->getTarget()->getPosition());
    } else {
        if(_tankMap->getChildByTag(1000))
        _tankMap->removeChildByTag(1000);
    }
    
    
    
    //tank loop: movement, ai, ...
    
    //  _tankPlayer->active(dt);
    //  bot->active(dt);
    for (auto bot : TankManager::getInstance()->getTankList()) {
        ((Tank*)bot)->active(dt);
    }
    
    //CCLOG("dt = %f ",dt);
    //bullet loop: movement, collision,...
    for (auto bullet : BulletManager::getInstance()->getBulletList()) {
        if(bullet->isVisible()) {
            ((Bullet*)bullet)->active(dt);
        }
    }
    
    
    
    if (getBuilding()) {
        if (getBuilding()->getLife() <= 0) {
            _isBuildingDestroyed = true;
            _building->setVisible(false);
        }
    }
    
    
    if (_killedEnemy == _totalEnemy) {
        
        if (_portal && _portal->getBoundingBox().intersectsRect(_tankPlayer->getBoundingBox())) {
            if (_missionType == kKillBoss) {
                if (!_isBoosKilled) {
                    return;
                }
            }
            //unschedule(schedule_selector(GameScene::mainLoop));
            this->unscheduleUpdate();
            gameWin();
        }
        if (_portal == nullptr) {
            _portal = Sprite::create(ANI_FIRST);
            auto animation = AnimationHelper::getAnimation("portal");
            animation->setLoops(-1);
            animation->setDelayPerUnit(0.084f);
            _portal->runAction(Animate::create(animation));
            _portal->setContentSize(Size(_portal->getContentSize().width * 0.6,_portal->getContentSize().height * 0.6));
            _portal->setPosition(_tankMap->getEndPoint());
            addObject2Map(_portal);
        }
    }
    
    
    for (int i=BulletManager::getInstance()->getBulletList().size()-1;i>=0;i--) {
        auto bullet = BulletManager::getInstance()->getBulletList().at(i);
        if(!bullet->isVisible()) {
            BulletManager::getInstance()->getBulletList().erase(i);
        }
    }
   
    
    for (int i = TankManager::getInstance()->getTankList().size() - 1; i >=0 ; i--) {
        Tank* tank = TankManager::getInstance()->getTankList().at(i);
        if (!tank->isVisible()) {
            if (tank->getKind() == tank_player) {
                gameLose();
                //unschedule(schedule_selector(GameScene::mainLoop));
                this->unscheduleUpdate();
            } else if (tank->getKind() == tank_friend) {
                _isFriendKilled = true;
            } else if(tank->getIsBoss()) {
                _isBoosKilled = true;
                _killedEnemy++;
                _currentEnemy--;
            } else {
                _killedEnemy++;
                _currentEnemy--;
                if(_killedEnemy + _currentEnemy < _totalEnemy) {
                    auto bot = Tank::createTankEnemy(_enemyType);
                    TankManager::getInstance()->loadTankAttr(bot, GameManager::getInstance()->getLevel());
                    bot->setMaxAITime(GAME_FPS*15);//5s 1 lan
                    int spawnSize = _tankMap->getSpawns().size() -1;
                    int spawnIdx = RandomHelper::random_int(0,spawnSize);
                    bot->setPosition(_tankMap->getSpawns()[spawnIdx]);
                    //bot->appear();
                    addObject2Map(bot);
                    _currentEnemy++;
                }
                
                CCLOG("current = %d killed = %d total %d  -- delay %d",_currentEnemy,_killedEnemy,_totalEnemy,_delayAddEnemy);
            }
            TankManager::getInstance()->getTankList().erase(i);
        }
    }
}

void GameScene::initGameParam() {

    TankManager::getInstance()->removeAllTank();
    BulletManager::getInstance()->removeAllBullet();
    
    _gameLoops = 0;
    _gameTime = 0;
    _tankPlayer = nullptr;
    _building = nullptr;
    _isPaused = false;
    _isBoosKilled = false;
    _isFriendKilled = false;
    _isBuildingDestroyed = false;
    
}

void GameScene::initMap() {
    _tankMap = TankMap::create();
    _pathFinderLayer = Layer::create();
    _tankMap->addChild(_pathFinderLayer);
    
    addChild(_tankMap);
    
}

void GameScene::createPlayerTank() {
    int _canonLevel = UserDefault::getInstance()->getIntegerForKey("canonlevel",1);
    int _bodyLevel = UserDefault::getInstance()->getIntegerForKey("bodylevel",1);
    int _chainLevel = UserDefault::getInstance()->getIntegerForKey("chainlevel",1);
    CCLOG("player Tank %d %d %d",_canonLevel,_bodyLevel,_chainLevel);
    _tankPlayer = Tank::createTankPlayer(_canonLevel, _bodyLevel , _chainLevel);
    _tankPlayer->setDirection(90);
    _tankPlayer->setPosition(_tankMap->getTankPoint());
    //_tankPlayer->appear();
    addObject2Map(_tankPlayer);
}

void GameScene::initAnimation() {
    
}

void GameScene::initUI() {
    

    
    _money = UserDefault::getInstance()->getIntegerForKey("money") ;
    
    _hudLayer = HUDLayer::create();
    _hudLayer->setNewGameScene(this);
    addChild(_hudLayer);
    
}

void GameScene::initPlayerData() {
    //set default player data
    UserDefault::getInstance()->getFloatForKey("attack",15.0f);
    UserDefault::getInstance()->getFloatForKey("defence",12.0f);
    UserDefault::getInstance()->getFloatForKey("speed",4.5f);
    UserDefault::getInstance()->getFloatForKey("hp",30.0f);
    UserDefault::getInstance()->getIntegerForKey("money",0);
    UserDefault::getInstance()->getIntegerForKey("star",0);
    UserDefault::getInstance()->getIntegerForKey("canonlevel",1);
    UserDefault::getInstance()->getIntegerForKey("bodylevel",1);
    UserDefault::getInstance()->getIntegerForKey("chainlevel",1);
    UserDefault::getInstance()->getIntegerForKey("chiplevel",1);
}

void GameScene::gameWin() {
    // calc game stats
    //show game win
    auto winLayer = GameWinLayer::create();
    //set star
    //set money ...
    
    
    std::string des1 = GameManager::getInstance()->getMissionAdditional(_mission1).at("describe").asString();
    std::string des2 = GameManager::getInstance()->getMissionAdditional(_mission2).at("describe").asString();
    std::string des3 = GameManager::getInstance()->getMissionAdditional(_mission3).at("describe").asString();
    
    int star = 0;
    if(checkBranchMission(_mission1)) {
        winLayer->setStar(0,des1,1);
        star++;
    } else {
        winLayer->setStar(0,des1,0);
    }
    if(checkBranchMission(_mission2)) {
        winLayer->setStar(1,des2,1);
        star++;
    } else {
        winLayer->setStar(1,des2,0);
    }
    if(checkBranchMission(_mission3)) {
        winLayer->setStar(2,des3,1);
        star++;
    } else {
        winLayer->setStar(2,des3,0);
    }
    
    //set money got from this mission
    UserDefault::getInstance()->setIntegerForKey("money", _money);
    
    //stored star
    int level = GameManager::getInstance()->getLevel();
    if (level <= 0 || level > 50) {
        level = 1;
        GameManager::getInstance()->setLevel(level);
    }
    int tmpStar = 0;
    if (level != 50)
    {
        int starOfCurrentStage = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("starOfStage%d",level).c_str(),0);
        
        if (star >= starOfCurrentStage)
        {
            UserDefault::getInstance()->setIntegerForKey(StringUtils::format("starOfStage%d",level).c_str(),star);
            tmpStar = star - starOfCurrentStage;
        }
        int passedStage = UserDefault::getInstance()->getIntegerForKey("passedLevel",0);
        if (passedStage <= level)
        {
            UserDefault::getInstance()->setIntegerForKey("passedLevel",level+1);
        }
    }else if (level == 50)
    {
        int starOfCurrentStage = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("starOfStage%d",level).c_str(),0);
        
        if (star >= starOfCurrentStage)
        {
            UserDefault::getInstance()->setIntegerForKey(StringUtils::format("starOfStage%d",level).c_str(),star);
            tmpStar = star - starOfCurrentStage;
        }
    }
    UserDefault::getInstance()->setIntegerForKey("star",UserDefault::getInstance()->getIntegerForKey("star",0) + tmpStar);
    
    //add bonus
    
    addChild(winLayer);
    
    
}

void GameScene::gameLose() {
    auto loseLayer = GameLose::create();
    addChild(loseLayer);
    _lifeLeft--;
    UserDefault::getInstance()->setIntegerForKey("lifeLeft",_lifeLeft);
}


bool GameScene::checkBranchMission(int missionId) {
    bool ok = false;
    switch (missionId) {
        case kBCompleteIn80s:
            CCLOG("second = %lld %d",_gameTime/60,80);
            if (_gameTime/60 < 80) {
                ok = true;
            }
            break;
        case kBCompleteIn70s:
            CCLOG("second = %lld %d",_gameTime/60,70);
            if (_gameTime/60 < 70) {
                ok = true;
            }
            break;
        case kBCompleteIn60s:
            CCLOG("second = %lld %d",_gameTime/60,60);
            if (_gameTime/60 < 60) {
                ok = true;
            }
            break;
        case kBProtectTroops:
            ok = !_isFriendKilled;
            break;
        case kBDestroyBuilding:
            ok = _isBuildingDestroyed;
            break;
        case kBUnused:
            break;
        case kBHp40p: {
            float percent = _tankPlayer->getLife()*100/ _tankPlayer->getLifeMax();
            if (percent > 40) {
                ok = true;
            }
        }
            break;
        case kBHp50p: {
            float percent = _tankPlayer->getLife()*100/ _tankPlayer->getLifeMax();
            if (percent > 60) {
                ok = true;
            }
        }
            break;
        case kBHp60p: {
            float percent = _tankPlayer->getLife()*100/ _tankPlayer->getLifeMax();
            if (percent > 60) {
                ok = true;
            }
        }
            break;
        case kBUseSpecialSkill:
            ok = false;
            break;
        case kBClearAllObs: {
            CCLOG("clear obs");
            ok = true;
            __Array* canDesArr = _tankMap->getCanDes();
            for(int i=0;i < canDesArr->count();i++) {
                Node* nodeObj = (Node*)canDesArr->getObjectAtIndex(i);
                if (nodeObj->isVisible()) {
                    CCLOG("clear obs false");
                    ok = false;
                    break;
                }
            }
            
        }
            break;
        case kBKillAllEnenmy:
            ok = true;
            break;
        default:
            CCLOG("unknown branch mission");
            break;
    }
    return ok;
}


void GameScene::drawAim(Vec2 p1,Vec2 p2) {
    
    if(_tankMap->getChildByTag(1000)) {
        _tankMap->getChildByTag(1000)->setPosition(p2);
    } else {
        auto spr = Sprite::create("res/player/aim0.png");
        spr->setPosition(p2);
        spr->setTag(1000);
        spr->setLocalZOrder(999999);
        spr->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.2f, 1.1f),ScaleTo::create(0.1f, 1.0f), NULL)));
        _tankMap->addChild(spr);
    }
}

