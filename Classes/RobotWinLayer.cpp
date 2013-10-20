//
//  RobotWinLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "RobotWinLayer.h"
#include "GameLayer.h"
#include "MainMenuLayer.h"

USING_NS_CC;

RobotWinLayer *RobotWinLayer::create(GameLayer *parent,int stepCount)
{
    RobotWinLayer *pRet = new RobotWinLayer(parent, stepCount);
    
    if(pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

bool RobotWinLayer::init()
{
    CCSize mapSize = GameParams::getInstanse()->mapSize();
    
    if ( !initWithColor(ccc4(145,145,145,255 * 0.45f)) )
    {
        return false;
    }
    
    CCLabelTTF *label = CCLabelTTF::create(CCLocalizedString("LevelDone", "NotLocalized"), "Marker Felt", 42);
    
    label->setAnchorPoint( CCPointZero );
    label->setPosition( CCPointZero );
    
    addChild(label, 20);
    
    setContentSize(CCSizeMake(label->getContentSize().width, label->getContentSize().height));
    setPosition(ccp(mapSize.width / 2 - getContentSize().width/2, -getContentSize().height));
    
    return true;
}

void RobotWinLayer::mainMenu(CCObject *sender)
{
    CCTransitionSlideInL* trans = CCTransitionSlideInL::create( 1.5f * SYSTEM_ANIMATION_DELAY, MainMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void RobotWinLayer::nextLevel(CCObject *sender)
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

void RobotWinLayer::selfRemove()
{
    parent->removeChild(this,true);
}

void RobotWinLayer::show()
{
    CCSize mapSize = GameParams::getInstanse()->mapSize();
    
    CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY, ccp(mapSize.width / 2 - getContentSize().width/2 , 70));

    runAction( moveLayer );
}

void RobotWinLayer::hide()
{
    CCSize mapSize = GameParams::getInstanse()->mapSize();
    
    CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY,  ccp(mapSize.width / 2 - getContentSize().width/2, -getContentSize().height));
    
    CCFiniteTimeAction* moveEnd = CCCallFunc::create( this,
                                                      callfunc_selector(RobotWinLayer::selfRemove));
    
    runAction( CCSequence::create(moveLayer, moveEnd, NULL) );
}