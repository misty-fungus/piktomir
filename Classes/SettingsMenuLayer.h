//
//  SettingsMenuLayer.h
//  pictomir-cocos2d-x
//
//  Created by Admin on 7/31/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__SettingsMenuLayer__
#define __pictomir_cocos2d_x__SettingsMenuLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

class SettingsMenuLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
    
    CREATE_FUNC(SettingsMenuLayer);
    
    ~SettingsMenuLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
    static cocos2d::CCScene* scene();
private:
    
    cocos2d::extension::CCControlSlider *backgroundSlider;
    cocos2d::extension::CCControlSlider *effectSlider;
    
    cocos2d::CCMenuItemSprite *soundOffButton;
    cocos2d::CCMenuItemSprite *soundOnButton;
    
    void backgroundVolumeChanged(CCObject *sender, cocos2d::extension::CCControlEvent controlEvent);
    void effectVolumeChanged(CCObject *sender, cocos2d::extension::CCControlEvent controlEvent);
    void toggleSound(CCObject *object);
    
    void selectLanguage(CCObject *sender);
    
    void mainMenu(CCObject *sender);
    
};

#endif /* defined(__pictomir_cocos2d_x__SettingsMenuLayer__) */
