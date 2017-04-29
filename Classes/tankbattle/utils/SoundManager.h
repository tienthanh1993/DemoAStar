//
//  SoundManager.h
//  FarmConnect
//
//  Created by Pham Thanh on 10/20/16.
//
//

#ifndef SoundManager_h
#define SoundManager_h

#include <cocos2d.h>

class SoundManager {
public:
    // creator
    static SoundManager* getInstance();
    // contructor
    SoundManager();
    ~SoundManager();
    // init
    // normal
    bool isEffect();
    bool isBackground();
    void setEffectEnable(bool enable);
    void setBackgroundEnable(bool enable);
    void playEffect(const char* pszFilePath);
    void playBackground(const char* pszFilePath,bool repeat = true);
    void stopBackground();
    void pause();
    void resume();
    // override
    
protected:
    
private:
    
};
#endif /* SoundManager_h */
