//
//  LevelScene.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/14/17.
//
//

#include "LevelScene.h"
#include "LevelEnterDialog.h"
#include "SceneFactory.h"
#include "constants.h"
#include "GameManager.h"
#include "utils/SoundManager.h"

USING_NS_CC;
using namespace ui;
cocos2d::Scene* LevelScene::createScene() {
    auto scene = Scene::create();
    
    auto layer = LevelScene::create();
    
    scene->addChild(layer);
    
    return scene;
}
//constructor
LevelScene::LevelScene() {
    
}

LevelScene::~LevelScene() {

}

//init
bool LevelScene::init() {
    if (!Layer::init()) {
        return false;
    }
     Size winSize = Director::getInstance()->getVisibleSize();
    
    _background = Sprite::create(UI_BACKGROUND);
    _background->setPosition(winSize/2);
    addChild(_background);
    

    
    
    _layer = Layer::create();
 
    
   
    Sprite* tempSp = Sprite::create(LEVEL_BG);
    
    _pageSize = tempSp->getContentSize() ;
   // _pageSize.width = _pageSize.width + (_pageSize.width - _pageSize.width*RATIO) ;
    
    _passedLevel = UserDefault::getInstance()->getIntegerForKey("passedLevel",1);

    
    _chapterList = FileUtils::getInstance()->getValueVectorFromFile(CHAPTER_FILE);
    _maxPage = _chapterList.size();
    _levelLocation = FileUtils::getInstance()->getValueVectorFromFile(LEVEL_LOCATION_FILE);
    
    _layer->setContentSize(Size(_pageSize.width*_maxPage,_pageSize.height));
    
    float startX = _pageSize.width/2;
    
    for (int i=0;i<_maxPage;i++)
    {
        ValueMap dic = _chapterList.at(i).asValueMap();
        
        ValueMap dic1 = _levelLocation.at(i).asValueMap();
        
        Sprite* sp = Sprite::create(dic.at("bg").asString().c_str());
        sp->setPosition(Vec2(startX + i * _pageSize.width,_pageSize.height/2));
        
        int max = dic.at("max").asInt();
        
        for (int j = 0; j < max ; j++)
        {
            int levelNum = 10*i + j+1;	// stageNum(1~50)
            
            int star = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("starOfStage%d",levelNum).c_str(),0);

            const char* normalImage = NULL;
            
            switch (star)
            {
                case 0:
                    normalImage = LEVEL_UNLOCK;
                    break;
                case 1:
                    normalImage = LEVEL_BRONZE;
                    break;
                case 2:
                    normalImage = LEVEL_SILVER;
                    break;
                case 3:
                    normalImage = LEVEL_GOLD;
                    break;
            }
//            if (levelNum >= 6) {
//                 normalImage = LEVEL_UNLOCK;
//            }
            
            
            Button* menuItem = Button::create(normalImage,"","",TextureResType::PLIST);
            //UserDefault::getInstance()->setIntegerForKey("lastSelectedLevel", levelNum);
            menuItem->setUserData(new int(levelNum));
            menuItem->addTouchEventListener(CC_CALLBACK_2(LevelScene::onButtonTouch,this));
            
            menuItem->setPosition(PointFromString(dic1.at(StringUtils::format("location%d",j+1).c_str()).asString().c_str()));
            menuItem->setTag(levelNum);

            if (levelNum == _passedLevel)
            {
            
               ScaleTo* a = ScaleTo::create(0.25f,1.3f);
                ScaleTo* b = ScaleTo::create(0.25f,1.0f);
                Sequence* seq = Sequence::create(a,b,NULL);
                
               Sprite* c = Sprite::createWithSpriteFrameName(normalImage);
                c->setPosition(menuItem->getPosition());
                sp->addChild(c,1);
                c->runAction(RepeatForever::create(seq));
            }
            
         
            if(_passedLevel < i*10+j+1)
            {
                menuItem->setEnabled(false);
            }
            
   
            Sprite* bg_gray = Sprite::createWithSpriteFrameName(LEVEL_BG_2);
            bg_gray->setPosition((menuItem->getPosition()+Vec2(0,- 0.65 * menuItem->getContentSize().height)));
            Label* lb = Label::createWithTTF(StringUtils::format("%d",i*10+j+1).c_str(),FONT_ARIAL,24);
            lb->setPosition(menuItem->getPosition()+Vec2(0,- 0.65 * menuItem->getContentSize().height));
            sp->addChild(bg_gray,1);
            sp->addChild(lb,2);
            sp->addChild(menuItem);
        }
        _layer->addChild(sp);

    }
    
    
    
    
    
    int chapter = (_passedLevel-1)/10;

    for(int i=chapter+1;i<=4;i++)
    {
        Sprite * cloud = Sprite::create(LEVEL_CLOUD);
        cloud->setPosition(Vec2(startX + i * _pageSize.width,_pageSize.height/2));
        _layer->addChild(cloud,3);
    }
     //_pageSize.width = _pageSize.width + (_pageSize.width - _pageSize.width*RATIO) ;
    _scrollView = ScrollView::create();
    _scrollView->addChild(_layer);
    _scrollView->setTouchEnabled(true);
    _scrollView->setBounceEnabled(false);
    _scrollView->setScrollBarEnabled(false);
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
 
    _scrollView->setContentSize(Size(Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width, _pageSize.height));
    _scrollView->setInnerContainerSize(Size(_maxPage *(_pageSize.width ), _pageSize.height));
    CCLOG("size1: %f  size2 : %f",_scrollView->getContentSize().width, _scrollView->getInnerContainerSize().width);
    _layer->setAnchorPoint(Point::ZERO);
    _scrollView->setPosition(Point::ZERO);

    this->addChild(_scrollView);
    
    
    _backBtn = Button::create(SHOP_MENU_BACK,"","",TextureResType::PLIST);
    _backBtn->setPosition(Vec2(winSize.width * 0.1,winSize.height * 0.93));
    _backBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType touchType) {
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
         SoundManager::getInstance()->playEffect("res/sound/sfx_click.mp3");
        Director::getInstance()->replaceScene(SceneFactory::create(level_scene));
    });
    addChild(_backBtn);
    _shopBtn = Button::create(SHOP_MENU_SHOP,"","",TextureResType::PLIST);
    _shopBtn->setPosition(Vec2(winSize.width * 0.9,winSize.height * 0.93));
 
    _shopBtn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType touchType) {
        if (touchType != Widget::TouchEventType::ENDED) {
            return;
        }
         SoundManager::getInstance()->playEffect("res/sound/sfx_click.mp3");
    });
    addChild(_shopBtn);

    
    
    
    
    _lifeTimeLeft  = UserDefault::getInstance()->getIntegerForKey("lifeTimeLeft",TIME_PER_LIFE);
    double now = utils::gettime() - UserDefault::getInstance()->getDoubleForKey("timeExit", utils::gettime());

    CCLOG("now %f= %f ",utils::gettime(),now);
    
    _lifeLeft = UserDefault::getInstance()->getIntegerForKey("lifeLeft",-1);
    _lifeMax = LIFE_MAX;
    if (_lifeLeft == -1) {
        _lifeLeft = LIFE_MAX;
        UserDefault::getInstance()->setIntegerForKey("lifeLeft",LIFE_MAX);
    }

    if (now > 0) {
        _lifeTimeLeft = _lifeTimeLeft - now;
        int totalLifeAdd = now/TIME_PER_LIFE;
        if (totalLifeAdd >= 1) {
            _lifeLeft += totalLifeAdd;
             _lifeTimeLeft = TIME_PER_LIFE;
        } else {
            _lifeTimeLeft -= now;
        }
    }
        CCLOG("_lifeTimeLeft = %d",_lifeTimeLeft);
    UserDefault::getInstance()->setIntegerForKey("lifeTimeLeft", _lifeTimeLeft);
    _lifeBg = Sprite::create(UI_TANK_LEFT);//"res/ui/lifeBg.png");
    _lifeBg->setPosition(0.3*winSize.width,  0.93*winSize.height);
    _lifeBg->setVisible(false);
    addChild(_lifeBg);
    
    _lifeLabel = Label::createWithBMFont("fonts/Matura.fnt","1/1");//, "fonts/arial.ttf", 32);
    _lifeLabel->setPosition(0.4*winSize.width, 0.93*winSize.height);
    _lifeLabel->setString(StringUtils::format("%d/%d",_lifeLeft,_lifeMax));
    _lifeLabel->setVisible(false);
    addChild(_lifeLabel);
    _lifeTimerLabel = Label::createWithBMFont("fonts/Matura.fnt","01:00 +1");//, "fonts/arial.ttf", 32);
    _lifeTimerLabel->setScale(0.5f);
    _lifeTimerLabel->setString(StringUtils::format("%02d:%02d +1",_lifeTimeLeft/60,_lifeTimeLeft%60));
    _lifeTimerLabel->setPosition(0.4*winSize.width,  0.88*winSize.height);
    _lifeTimerLabel->setVisible(false);
    addChild(_lifeTimerLabel);
    
 
   
    
    //
    _starBg = Sprite::create("res/ui/gameend/star_gold.png");
    _starBg->setPosition(0.6*winSize.width, 0.93*winSize.height);
    addChild(_starBg);
    
    _starLabel = Label::createWithBMFont("fonts/Matura.fnt", "0");
    _starLabel->setPosition(0.75*winSize.width,0.93*winSize.height);
    _starLabel->setString(StringUtils::format("%d",UserDefault::getInstance()->getIntegerForKey("star",0)).c_str());
    addChild(_starLabel);
    //
    

    
    return true;
}
//normal

void LevelScene::registerEvent() {
    __NotificationCenter::getInstance()->addObserver(this,CC_CALLFUNCO_SELECTOR(LevelScene::onEnterGame), "enterGame", nullptr);
}

void LevelScene::unRegisterEvent() {
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void LevelScene::onButtonTouch(cocos2d::Ref* sender,cocos2d::ui::Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }
     SoundManager::getInstance()->playEffect("res/sound/sfx_click.mp3");
    Button* btn = (Button*)sender;
    int level = *(int*)btn->getUserData();
    CCLOG("select level %d",level);
    _enterDialog = LevelEnterDialog::create(level);
    addChild(_enterDialog);
    
    
}

void LevelScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event) {
    
}

//override
void LevelScene::onEnter() {
    Layer::onEnter();
    registerEvent();

    schedule(schedule_selector(LevelScene::lifeUpdate),1.0f);
}

void LevelScene::onExit() {
    Layer::onExit();
    unRegisterEvent();
     UserDefault::getInstance()->setIntegerForKey("lifeTimeLeft",_lifeTimeLeft);
     UserDefault::getInstance()->setDoubleForKey("timeExit", utils::gettime());
    CCLOG("time exit = %f",utils::gettime());
}


void LevelScene::onEnterGame(Ref* data) {
    
    int level = *(int*)data;
    CCLOG("enter level %d",level);
    GameManager::getInstance()->setLevel(level);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* ld = Sprite::create(UI_LOADING);
    ld->setPosition(Vec2(visibleSize.width * 0.5,visibleSize.height * 0.5));
    addChild(ld,4);
    Label* lb = Label::createWithTTF("loading...",FONT_ARIAL,50);
    lb->setPosition(Vec2(visibleSize.width * 0.75,visibleSize.height * 0.1));
    addChild(lb,4);
    

    //_gameScene = SceneFactory::create(game_scene);
    Director::getInstance()->replaceScene(SceneFactory::create(game_scene));
    //_enterDialog->removeFromParent();
  //  _gameScene->retain();
    //scheduleOnce(schedule_selector(LevelScene::go),0.0f);
}


void LevelScene::go(float dt) {
//    _gameScene->autorelease();
  //  Director::getInstance()->replaceScene(_gameScene);
  
}

void LevelScene::lifeUpdate(float dt) {

    if (_lifeTimeLeft > 0 ) {
        _lifeTimeLeft--;
        _lifeTimerLabel->setString(StringUtils::format("%02d:%02d +1",_lifeTimeLeft/60,_lifeTimeLeft%60).c_str());
    } else {
        _lifeTimeLeft = TIME_PER_LIFE;
        if(_lifeLeft < _lifeMax)
        _lifeLeft++;
        UserDefault::getInstance()->setIntegerForKey("lifeLeft", _lifeLeft);
    }
   
}
