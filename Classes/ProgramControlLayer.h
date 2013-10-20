//
//  ProgramControlLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/17/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__ProgramControlLayer__
#define __pictomir_cocos2d_x__ProgramControlLayer__

#include <iostream>
#include "GameParams.h"

class GameLayer;

const float PROGRAM_CONTROL_BUTTON_SIZE = 50.0f;
const float PROGRAM_CONTROL_BUTTON_SEPARATOR = 5.0f;

class ProgramControlLayer : public cocos2d::CCLayer
{
public:
    
    virtual bool init();
    
    CREATE_FUNC(ProgramControlLayer);
    
    void setEnabled(bool flag);
    
    SYNTHESIZE(cocos2d::CCMenuItemSprite *, startProgramButton, StartProgramButton);
    SYNTHESIZE(cocos2d::CCMenuItemSprite *, stopProgramButton, StopProgramButton);
    SYNTHESIZE(cocos2d::CCMenuItemSprite *, makeStepButton, MakeStepButton);
    SYNTHESIZE(cocos2d::CCMenuItemSprite *, resetButton, ResetButton);
    
#ifdef MAP_EDITOR
    SYNTHESIZE(cocos2d::CCMenuItemSprite *, editButton, EditButton);
#endif
    
    ProgramControlLayer() :
        cocos2d::CCLayer(),
        parent(NULL)
    {}

    
    ProgramControlLayer(GameLayer *parent) :
        cocos2d::CCLayer(),
        parent(parent)
    {}
    
    static ProgramControlLayer* create(GameLayer *parent)
    {
        ProgramControlLayer *programLayer = new ProgramControlLayer(parent);
        
        programLayer->init();
        programLayer->autorelease();
        
        return programLayer;
    }
    
    ~ProgramControlLayer()
    {
        removeAllChildrenWithCleanup(true);
    }

    void saveStates();
    void restoreStates();
private:
    GameLayer *parent;
    cocos2d::CCMenu *programMenu;
    
    bool startButtonState;
    bool stopProgramButtonState;
    bool makeStepButtonState;
    bool resetButtonState;
};

#endif /* defined(__pictomir_cocos2d_x__ProgramControlLayer__) */
