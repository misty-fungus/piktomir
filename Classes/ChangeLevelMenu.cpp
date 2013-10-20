//
//  ChangeLevelMenu.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/17/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "ChangeLevelMenu.h"
#include "PMTextureCache.h"
#include "GameLayer.h"

USING_NS_CC;

bool ChangeLevelMenu::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    GameParams *params = GameParams::getInstanse();
    
    CCMenuItemSprite *nextLevel = CCMenuItemSprite::create(PMTextureCache::instanse()->getNextLevelIconSprite(),
                                                  PMTextureCache::instanse()->getNextLevelIconSprite_Selected(),
                                                  PMTextureCache::instanse()->getNextLevelIconSprite_Disabled(),
                                                           this, menu_selector(ChangeLevelMenu::nextLevel));
    
    CCMenuItemSprite *prevLevel = CCMenuItemSprite::create(PMTextureCache::instanse()->getPrevLevelIconSprite(),
                                                           PMTextureCache::instanse()->getPrevLevelIconSprite_Selected(),
                                                           PMTextureCache::instanse()->getPrevLevelIconSprite_Disabled(),
                                                           this, menu_selector(ChangeLevelMenu::prevLevel));
    
    prevLevel->setPosition( ccp(20, 20) );
    prevLevel->setAnchorPoint(CCPointZero );
    nextLevel->setPosition( ccp(params->mapSize().width -  nextLevel->getContentSize().width - 20, 20) );
    nextLevel->setAnchorPoint(CCPointZero );
    
    menu = CCMenu::create(nextLevel,prevLevel, NULL);
    menu->setAnchorPoint( CCPointZero );
    menu->setContentSize(CCSizeMake(params->mapSize().width, nextLevel->getContentSize().height + 20) );
    
    menu->setPosition( CCPointZero );
    
    setPosition( ccp(0,0) );
    setAnchorPoint( CCPointZero );
    
    addChild(menu,10);
    
    return true;
}

void ChangeLevelMenu::saveStates()
{
    state = ((CCMenuItem *)menu->getChildren()->objectAtIndex(0))->isEnabled();
}

void ChangeLevelMenu::restoreStates()
{
    for(int i = 0; i < menu->getChildrenCount(); ++i)
    {
        ((CCMenuItem *)menu->getChildren()->objectAtIndex(i))->setEnabled(state);
    }
}

void ChangeLevelMenu::setEnabled(bool flag)
{
    for(int i = 0; i < menu->getChildrenCount(); ++i)
    {
         ((CCMenuItem *)menu->getChildren()->objectAtIndex(i))->setEnabled(flag);
    }
}

void ChangeLevelMenu::nextLevel(CCObject *sender)
{
    GetGameParams->incLevel();
    if (GetGameParams->getLevel() == GetGameParams->worlds[GetGameParams->getWorldPart()] + 1)
    {
        GetGameParams->incWorldPart();
        GetGameParams->setLevel(1);
    }
    
    if (GetGameParams->getWorldPart() >= GetGameParams->worlds.size())
    {
        GetGameParams->setWorldPart(0);
    }
    
    GetGameParams->setContinueGame(false);    
    
    CCTransitionFade* trans = CCTransitionFade::create( 1.5f * SYSTEM_ANIMATION_DELAY, GameLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void ChangeLevelMenu::prevLevel(CCObject *sender)
{
    GetGameParams->decLevel();
    if (GetGameParams->getLevel() == 0)
    {
        GetGameParams->decWorldPart();
        
        if (GetGameParams->getWorldPart() < 0)
        {
            GetGameParams->setLevel(1);
            GetGameParams->setWorldPart(0);
        }
		else
			GetGameParams->setLevel( GetGameParams->worlds[GetGameParams->getWorldPart()]);
    }
    
    GetGameParams->setContinueGame(false);
    
    CCTransitionFade* trans = CCTransitionFade::create( 1.5f * SYSTEM_ANIMATION_DELAY, GameLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}
