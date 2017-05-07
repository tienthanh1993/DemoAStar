//
//  constants.h
//  TankBattle
//
//  Created by Pham Thanh on 3/3/17.
//
//

#ifndef constants_h
#define constants_h
#define RATIO (1.5 - 720.0f/Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width)

#define GAMESCENE ( (GameScene*) Director::getInstance()->getRunningScene()->getChildByTag(1) )
#define LIFE_MAX 10
#define TIME_PER_LIFE 3600 // second


enum TankKind {
    tank_begin,
    tank_player,
    tank_normal,
    tank_friend,
    tank_boss,
};


enum SceneType {
    loading_scene,
    menu_scene,
    level_scene,
    game_scene,
    shop_scene,
};
enum EquipType {
    kCannon = 0,
    kBody,
    kChain,
    kChip,
    kEquipTypeEnd,
};

const char equipKey[kEquipTypeEnd][32] = {
    "canonlevel",
    "bodylevel",
    "chainlevel",
    "chiplevel",
};

const char attributeKey[kEquipTypeEnd][32] = {
    "attack",
    "defense",
    "speed",
    "hp",
};
const char addAttributeKey[kEquipTypeEnd][32] = {
    "addAttack%d",
    "addDefence%d",
    "addSpeed%d",
    "addHp%d",
};
enum AttrType {
    kAttack = 20,
    kDefense,
    kSpeed,
    kLife,
};

enum MissionType {
    kKillAllEnenmy = 1,
    kKillBoss,
};
enum BranchMissionType {
    kBCompleteIn80s = 1,
    kBCompleteIn70s,
    kBCompleteIn60s,
    kBProtectTroops,
    kBDestroyBuilding,
    kBUnused,
    kBHp40p,
    kBHp50p,
    kBHp60p,
    kBUseSpecialSkill,
    kBClearAllObs,
    kBKillAllEnenmy,
};
const int EQUIP_MAX_LEVEL = 6;

const int gDir2[8][2] = {
    {0,-1}, // top
    {-1,0}, //left
    {0,+1}, //bot
    {+1,0}, // right
    {-1,-1}, //top left
    {+1,-1}, //top right
    {-1,1}, // bot left
    {+1,1} //bot right
};

#define MAX_WIDTH 20
#define MAX_HEIGHT 12

#define GAME_FPS 18

//ENTITY DEFINE
#define MAP_FILE "res/data/map.plist"
#define PLAYER_SHADOW "res/player/shadow.png"
#define PLAYER_BODY "res/player/body%d.png"
#define PLAYER_CANNON "res/player/cannon%d.png"
#define PLAYER_CHAIN "res/player/chain%d0.png"
#define PLAYER_CHAIN1 "res/player/chain%d1.png"

#define BLOOD_BG "res/blood/bloodd.png"
#define PLAYER_BLOOD "res/blood/myblood.png"
#define ENEMY_BLOOD "res/blood/enemyblood.png"
#define BOSS_BLODD_BG "res/blood/bossd.png"
#define BOSS_BLOOD "res/blood/bossbuildblood.png"


#define ENEMY_SHADOW "res/enemy/shadow.png"
#define ENEMY_BODY "res/enemy/body%d.png"
#define ENEMY_CANNON "res/enemy/cannon%d.png"
#define ENEMY_CHAIN "res/enemy/chain%d0.png"
#define ENEMY_CHAIN1 "res/enemy/chain%d1.png"

#define FRIEND_BODY "res/friend/friendtank.png"
#define FRIEND_BODY1 "res/friend/friendtank_h.png"

#define BOSS_SHADOW "res/enemy/shadow.png"
#define BOSS_BODY "res/boss/body%d1.png"
#define BOSS_BODY1 "res/boss/body%d1.png"
#define BOSS_CANNON "res/boss/cannon%d1.png"
#define BOSS_CANNON1 "res/boss/cannon%d2.png"
#define BOSS_CHAIN "res/boss/body%d1.png"
#define BOSS_CHAIN1 "res/boss/body%d2.png"

#define BOSS_BORDER "res/boss/border%d1.png"
#define BOSS_BORDER1 "res/boss/border%d2.png"

#define BULLET "res/bullet/bullet%d.png"

#define BOOM_FILE "res/animation/boom.plist"



//GAME UI DEFINE
#define FONT_ARIAL "fonts/arial.ttf"
//menu
#define UI_BACKGROUND "res/ui/background.png"
#define UI_LOADING "res/ui/loading.png"
#define UI_LEVEL_TIME "res/ui/stageTime.png"
#define UI_TANK_LEFT "res/ui/leftTank.png"
#define UI_GOLD_LABEL "res/shop/label_gold.png"


#define MENU_PLAY "button/play.png"
#define MENU_SETTINGS "button/settings.png"
#define MENU_SETTINGS1 "button/settings.png"
#define MENU_SHOP "button/shop.png"
#define MENU_HELP "button/help.png"

//level
#define LEVEL_BG "res/ui/chapter_1.png"
#define LEVEL_BG_2 "level/bg_gray.png"
#define LEVEL_CLOUD "res/ui/cloud_1.png"

#define CHAPTER_FILE "res/data/chapterlist.plist"
#define LEVEL_LOCATION_FILE "res/data/levelLocation.plist"
#define LEVEL_UNLOCK "level/stageUnlock.png"
#define LEVEL_BRONZE "level/bronze.png"
#define LEVEL_SILVER "level/silver.png"
#define LEVEL_GOLD "level/gold.png"

#define LEVEL_BG_FORMAT "res/map/bg%d.png"

//shop
#define EQUIPMENT_FILE "res/data/equipment.plist"
#define SHOP_BG "res/ui/shop/bg.png"
#define SHOP_DESCRIPTION "shop/bg_description.png"

#define SHOP_MENU_BACK "button/menu_back_N.png"
#define SHOP_MENU_BACK1 "button/menu_back_S.png"
#define SHOP_MENU_SHOP "button/shop.png"
#define SHOP_MENU_SHOP1 "button/shop1.png"
#define SHOP_LABEL_GOLD "shop/label_gold.png"
#define SHOP_MENU_ADDCOIN "shop/add.png"
#define SHOP_MENU_ADDCOIN1 "shop/add_h.png"

#define SHOP_LABEL_ADD "shop/add.png"
#define SHOP_LABEL_ADD1 "shop/add_h.png"
#define SHOP_BG_PROPERTY "shop/bg_property.png"

#define SHOP_MENU_BG_1 "shop/menu_bg1.png"
#define SHOP_MENU_BG_2 "shop/menu_bg2.png"
#define SHOP_MENU_SEL "shop/menu_sel.png"
#define SHOP_MENU_UPGRADE_MAX "button/menu_upgrade_max.png"
#define SHOP_MENU_UPGRADE_N "button/menu_upgrade_N.png"
#define SHOP_MENU_UPGRADE_S "button/menu_upgrade_S.png"


//game
#define ANI_FIRST "res/animation/first.png"
#define DOT_WHITE "control/dot_white.png"
#define DOT_RED "control/dot_red.png"
#define DOT_GREEN "control/dot_green.png"

#define GAME_PAUSE "button/menu_pause.png"
#define GAME_PAUSE1 "button/menu_pause1.png"

#define ROCKER_FIRE "control/rockerFire.png"
#define ROCKER_BG "control/rockerBG.png"
#define ROCKER "control/rocker.png"



//EFFECT
#define FIRE_EFFECT "res/animation/fireeffect.plist"
#define BOOM "res/animation/boom.plist"
#define ENEMY_BOOM "res/animation/enemytankboom.plist"
#define BOSS_BOOM "res/animation/bossboom.plist"
#define PLANE_BOOM "res/animation/planeboom.plist"
#define PLAYER_APPEAR "res/animation/playerAppear.plist"
#define ENEMY_APPEAR "res/animation/enemyAppear.plist"
#define ROADBLOCK "res/animation/roadblock.plist"
#define PORTAL "res/animation/portal.plist"
#define PROP_EFFECT "res/animation/propefect.plist"
#define FLAG "res/animation/flag.plist"
#define JETONE "res/animation/jetone.plist"
#define JETTWO "res/animation/jettwo.plist"

#define HELP_BG "res/ui/help/background.png"
#define SETTING_CLOSE "button/close.png"
#define SETTING_BG "res/ui/settings/setting_bg.png"
#define SETTING_OFF "button/off.png"
#define SETTING_ON "button/on.png"

#define LEVEL_ENTER_BG "res/ui/mission_bg.png"
#define LEVEL_ENTER_RANKBTN "button/rankBtn.png"
#define LEVEL_ENTER_CLOSE "button/closebt.png"
#define LEVEL_ENTER_CLOSE1 "button/closebt_h.png"

#define LEVEL_ENTER_GO "button/go.png"
#define LEVEL_ENTER_GO1 "button/go1.png"


#define COUNTDOWN_BG "numBg.png"
#define COUNTDOWN_NUM1 "num11.png"
#define COUNTDOWN_NUM2 "num22.png"
#define COUNTDOWN_NUM3 "num33.png"
#define COUNTDOWN_GO "numGo.png"

#define GAMEEND_BG "res/ui/gameend/result_bg.png"
#define GAMEEND_BG1 "res/ui/gameend/result_bg1.png"
#define GAMEEND_RETRY "button/retry.png"
#define GAMEEND_RETRY1 "button/retry1.png"
#define GAMEEND_RETURN "button/return.png"
#define GAMEEND_RETURN1 "button/return1.png"
#define GAMEEND_RESTART "button/restart.png"
#define GAMEEND_RESTART1 "button/restart1.png"
#define GAMEEND_CONTINUE "button/continue.png"
#define GAMEEND_CONTINUE1 "button/continue1.png"
#define GAMEEND_STAR_GOLD "res/ui/gameend/star_gold.png"
#define GAMEEND_STAR_GREY "res/ui/gameend/star_grey.png"
#define GAMEEND_PAUSE_BG "res/ui/gameend/pause_bg.png"
#define CLOSEX_BUTTON "res/ui/dialog/cancel_btn.png"

float getAngleOfTwoVectors(cocos2d::Point vec1, cocos2d::Point vec2);

#define TOTAL_RESOUCE 11
const char gResources[TOTAL_RESOUCE][255]= {
    "res/control/control",
    "res/ui/button",
    "res/ui/level",
    "res/animation/boom",
    "res/animation/fireeffect",
    "res/animation/enemytankboom",
    "res/animation/bossboom",
    "res/animation/playerAppear",
    "res/animation/enemyAppear",
    "res/ui/chapter_1",
    "res/ui/cloud_1"
};
#endif /* constants_h */
