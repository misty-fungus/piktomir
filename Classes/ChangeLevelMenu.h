//
//  ChangeLevelMenu.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/17/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__ChangeLevelMenu__
#define __pictomir_cocos2d_x__ChangeLevelMenu__

#include <iostream>

#include <iostream>
#include "GameParams.h"

class GameLayer;

class ChangeLevelMenu : public cocos2d::CCLayer
{
public:
    
    virtual bool init();
    
    CREATE_FUNC(ChangeLevelMenu);
    
    void setEnabled(bool flag);
    
    ChangeLevelMenu() :
        cocos2d::CCLayer(),
        parent(NULL)
    {}
    
    
    ChangeLevelMenu(GameLayer *parent) :
        cocos2d::CCLayer(),
        parent(parent)
    {}
    
    static ChangeLevelMenu* create(GameLayer *parent)
    {
        ChangeLevelMenu *programLayer = new ChangeLevelMenu(parent);
        
        programLayer->init();
        programLayer->autorelease();
        
        return programLayer;
    }
    
    ~ChangeLevelMenu()
    {
        removeAllChildrenWithCleanup(true);
    }
    
    void saveStates();
    void restoreStates();
    
private:
    GameLayer *parent;
    cocos2d::CCMenu *menu;
    
    void nextLevel(CCObject *sender);
    void prevLevel(CCObject *sender);
    
    bool state;
};

#endif /* defined(__pictomir_cocos2d_x__ChangeLevelMenu__) */
