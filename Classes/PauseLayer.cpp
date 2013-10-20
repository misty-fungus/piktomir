//
//  PauseLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "PauseLayer.h"
#include "GameLayer.h"
#include "MainMenuLayer.h"
#include "ScrollLayer.h"

USING_NS_CC;

PauseLayer *PauseLayer::create(GameLayer *parent,bool isRobotLoose)
{
    PauseLayer *pRet = new PauseLayer(parent,isRobotLoose);
    
    if(pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

bool PauseLayer::init()
{
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    
    if ( !initWithColor(ccc4(145,145,145,255), screenSize.width, screenSize.height) )
    {
        return false;
    }
    
    m_bKeypadEnabled=true;
    m_bTouchEnabled = true;
    
    setOpacity( 255 * 0.45f );
    
    CCMenuItemFont *pauseItem2 = CCMenuItemFont::create(CCLocalizedString("MainMenu", "NotLocalized"), this , menu_selector(PauseLayer::mainMenu));
    CCMenuItemFont *pauseItem3  = NULL;
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
    pauseItem3 = CCMenuItemFont::create(CCLocalizedString("Exit", "NotLocalized"), this , menu_selector(PauseLayer::exitGame));
    pauseItem3->setPosition(screenSize.width/2, screenSize.height/2);
#endif
    
    CCMenu *pauseMenu = CCMenu::create(pauseItem2, pauseItem3, NULL);
    
    addChild(pauseMenu);
    
    if(!isRobotLoose)
    {
        CCMenuItemFont *pauseItem1 = CCMenuItemFont::create(CCLocalizedString("ContinueLevel", "NotLocalized"), parent , menu_selector(GameLayer::continueLevel));
        pauseItem1->setPosition(screenSize.width/2, screenSize.height/2 + 100);
        pauseMenu->addChild(pauseItem1);
    }
    
    pauseMenu->setPosition( CCPointZero );
    pauseItem2->setPosition(screenSize.width/2, screenSize.height/2 + 50);
    
    setPosition(ccp(0, screenSize.height));
    
    return true;
}

void PauseLayer::mainMenu(CCObject *sender)
{
    //wtf ?!!!!!
    CCPointObject * object = (CCPointObject *)parent->getWorld()->getMap()->getScroll()->getParallax()->getParallaxArray()->arr[0];
    
    CCSprite *background = (CCSprite *)object->getChild();
    
    object->setRatio(ccp(1.0,1.0));
    object->setOffset( ccp(background->getContentSize().width / 2, background->getContentSize().height / 2) );
    
    CCTransitionSlideInL* trans = CCTransitionSlideInL::create( 1.5f * SYSTEM_ANIMATION_DELAY, MainMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void PauseLayer::nextLevel(CCObject *sender)
{
    GetGameParams->incLevel();
    if (GetGameParams->getLevel() == GetGameParams->worlds[GetGameParams->getWorldPart()] + 1) {
        GetGameParams->incWorldPart();
        GetGameParams->setLevel(1);
    }
    
    if (GetGameParams->getWorldPart() >= GetGameParams->worlds.size()) {
        GetGameParams->setWorldPart(0);
    }
    
    CCTransitionFade* trans = CCTransitionFade::create( 1.5f * SYSTEM_ANIMATION_DELAY, GameLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}


void PauseLayer::exitGame(CCObject *sender)
{
    removeAllChildrenWithCleanup(true);
    exit(0);
}

void PauseLayer::selfRemove()
{
    parent->removeChild(this,true);
}

void PauseLayer::show()
{
    
    CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY, CCPointZero);
    
    runAction( moveLayer );
}

void PauseLayer::hide()
{
    retain();
    
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    
    CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY, ccp(0, screenSize.height));
    
    CCFiniteTimeAction* moveEnd = CCCallFunc::create( this,
                                                      callfunc_selector(RobotLooseLayer::selfRemove));
    
    runAction( CCSequence::create(moveLayer, moveEnd, NULL) );
}