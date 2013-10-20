//
//  ProgramControlLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/17/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "ProgramControlLayer.h"
#include "GameParams.h"
#include "PMTextureCache.h"
#include "GameLayer.h"

#ifdef MAP_EDITOR
#include "MapEditorGameLayer.h"
#endif

USING_NS_CC;

bool ProgramControlLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    startProgramButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getStartIconSprite(),
                                                  PMTextureCache::instanse()->getStartIconSprite_Selected(),
                                                  PMTextureCache::instanse()->getStartIconSprite_Disabled(),
                                                  parent, menu_selector(GameLayer::start));
    
    stopProgramButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getStopIconSprite(),
                                                 PMTextureCache::instanse()->getStopIconSprite_Selected(),
                                                 PMTextureCache::instanse()->getStopIconSprite_Disabled(),
                                                 parent, menu_selector(GameLayer::restartLevel));
    
    makeStepButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getMakeStepIconSprite(),
                                              PMTextureCache::instanse()->getMakeStepIconSprite_Selected(),
                                              PMTextureCache::instanse()->getMakeStepIconSprite_Disabled(),
                                              parent, menu_selector(GameLayer::makeProgramStep));
    
    resetButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getResetIconSprite(),
                                              PMTextureCache::instanse()->getResetIconSprite_Selected(),
                                              PMTextureCache::instanse()->getResetIconSprite_Disabled(),
                                              parent, menu_selector(GameLayer::restartLevel));
    
    resetButton->setPosition( ccp(0, 0) );
    resetButton->setAnchorPoint( CCPointZero );
    stopProgramButton->setPosition( ccp( 3 * (PROGRAM_CONTROL_BUTTON_SIZE + PROGRAM_CONTROL_BUTTON_SEPARATOR), 0) );
    stopProgramButton->setAnchorPoint( CCPointZero );
    startProgramButton->setPosition( ccp( PROGRAM_CONTROL_BUTTON_SIZE + PROGRAM_CONTROL_BUTTON_SEPARATOR, 0) );
    startProgramButton->setAnchorPoint( CCPointZero );
    makeStepButton->setPosition( ccp(2 * (PROGRAM_CONTROL_BUTTON_SIZE + PROGRAM_CONTROL_BUTTON_SEPARATOR),0) );
    makeStepButton->setAnchorPoint( CCPointZero );
    
    programMenu = CCMenu::create(resetButton,startProgramButton, makeStepButton, stopProgramButton, NULL);
    programMenu->setAnchorPoint( CCPointZero );
    programMenu->setContentSize(CCSizeMake(4 * (PROGRAM_CONTROL_BUTTON_SIZE + PROGRAM_CONTROL_BUTTON_SEPARATOR), PROGRAM_CONTROL_BUTTON_SIZE) );
    
    programMenu->setPosition( CCPointZero );
    
    
    addChild(programMenu,10);
    
    
#ifdef MAP_EDITOR
    editButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getEditIconSprite(),PMTextureCache::instanse()->getEditIconSprite_Selected(),PMTextureCache::instanse()->getEditIconSprite_Disabled(), MapEditorGameLayer::getInstanse(), menu_selector(MapEditorGameLayer::setEdit));
    editButton->setPosition( ccp( -60, 0) );
    editButton->setAnchorPoint( CCPointZero );
    programMenu->addChild(editButton,10);
    programMenu->setContentSize(CCSizeMake(5 * (PROGRAM_CONTROL_BUTTON_SIZE + PROGRAM_CONTROL_BUTTON_SEPARATOR), PROGRAM_CONTROL_BUTTON_SIZE) );
#endif
    
    setContentSize(programMenu->getContentSize());
    
    return true;
}


void ProgramControlLayer::setEnabled(bool flag)
{
    startProgramButton->setEnabled(flag);
    stopProgramButton->setEnabled(flag);
    makeStepButton->setEnabled(flag);
#ifdef MAP_EDITOR
    editButton->setEnabled(flag);
#endif
}


void ProgramControlLayer::saveStates()
{
    startButtonState = startProgramButton->isEnabled();
    stopProgramButtonState = stopProgramButton->isEnabled();
    makeStepButtonState = makeStepButton->isEnabled();
    resetButtonState = resetButton->isEnabled();
}

void ProgramControlLayer::restoreStates()
{
    startProgramButton->setEnabled(startButtonState);
    stopProgramButton->setEnabled(stopProgramButtonState);
    makeStepButton->setEnabled(makeStepButtonState);
    resetButton->setEnabled(resetButtonState);
}