//
//  GameLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__GameLayer__
#define __pictomir_cocos2d_x__GameLayer__

#include <iostream>
#include "cocos2d.h"
#include "GameParams.h"
#include "World4.h"
#include "ProgramLayer.h"
#include <libxml/xmlreader.h>
#include "Hint.h"
#include "PMTextureCache.h"

#include "RobotLooseLayer.h"
#include "RobotWinLayer.h"
#include "PauseLayer.h"
#include "SpeedBarLayer.h"
#include "ProgramControlLayer.h"
#include "ChangeLevelMenu.h"
#include "MethodStackLayer.h"

class GameLayer:public cocos2d::CCLayer
{
    SYNTHESIZE(ProgramLayer *, programLayer, ProgramLayer);
    SYNTHESIZE(SpeedBarLayer *, speedBarLayer, SpeedBarLayer);
    SYNTHESIZE(ProgramControlLayer *, programControlLayer, ProgramControlLayer);
    SYNTHESIZE(ChangeLevelMenu *, changeLevelMenu, ChangeLevelMenu);
    
    SYNTHESIZE(RobotWinLayer *, winLayer, WinLayer);
    SYNTHESIZE(RobotLooseLayer *, looseLayer, LooseLayer);
    SYNTHESIZE(PauseLayer *, pauseLayer, PauseLayer);
    SYNTHESIZE(MethodStackLayer *, methodStackLayer, MethodStackLayer);
    
    
    SYNTHESIZE(Hint*, hint, Hint);
    SYNTHESIZE(UniversalWorld *,world,World);
public:
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(GameLayer);
    
    GameLayer():
        cocos2d::CCLayer(),
        programLayer(NULL),
        speedBarLayer(NULL),
        programControlLayer(NULL),
        changeLevelMenu(NULL),
        winLayer(NULL),
        looseLayer(NULL),
        pauseLayer(NULL),
        methodStackLayer(NULL),
        hint(NULL),
        world(NULL),
        highlightedLayer(-1),
        highlightedButton(-1),
        programStarted(false),
        pauseButton(NULL)
    {
        cocos2d::CCTextureCache::sharedTextureCache()->removeAllTextures();
    }
    
    ~GameLayer()
    {
        CC_SAFE_DELETE(hint);
        CC_SAFE_DELETE(world);
        
        removeAllChildrenWithCleanup(true);
        
        cocos2d::CCTextureCache::sharedTextureCache()->removeAllTextures();
    }
    
    void saveMap();
       
protected:
    
    int highlightedLayer;
    int highlightedButton;
    
    bool programStarted;
    
    cocos2d::CCMenuItemSprite *pauseButton;
    
    cocos2d::CCMenuItemSprite *soundOffButton;
    cocos2d::CCMenuItemSprite *soundOnButton;
    
    bool programLayerState;
    bool speedBarState;
    bool mapState;    
    
    void gameLayerInit();
    void loadWorld();
    void loadWorld(std::string mapPath);
    //Handlers
    
    void makeStep(float s);
    
    void winLayerInit();
    void looseLayerInit();
    void pauseLayerInit(bool isRobotLoose);
    
    void drawHint();
    
    void saveStates();
    void restoreStates();
    
public:    //methods
    void continueGame();
    void continueLevel(CCObject *object);
    void restartLevel(CCObject *object);
    
    void start(CCObject *object);
    void pause(CCObject *object);
    void toggleSound(CCObject *object);
    void makeProgramStep(CCObject *object);
    
    void robotWin();
    void robotLoose();
    void robotFailure();
    
    void setEnabled(bool state);
    void setEnabledForGame(bool state);
    
    
    void highLightBreakButon();
    void highLightButon(int layerIndex,int button);
    void clearHighlitedButton();
    
#ifdef MAP_EDITOR
    HintEditLayer *hintLayer;
    void readyForSave () {
        std::string text = hintLayer->hint->getText();
        if(text != "")
        {
            hint->hintParts[0].type = MethodHint;
            hint->hintParts[0].hintText = text;
        } else hint = NULL;
    }
#endif
};

#endif /* defined(__pictomir_cocos2d_x__GameLayer__) */
