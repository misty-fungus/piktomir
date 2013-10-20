//
//  RobotLooseLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "RobotLooseLayer.h"
#include "GameLayer.h"
#include "MainMenuLayer.h"

USING_NS_CC;

RobotLooseLayer *RobotLooseLayer::create(GameLayer *parent)
{
    RobotLooseLayer *pRet = new RobotLooseLayer(parent);
    
    if(pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}


bool RobotLooseLayer::init()
{
    CCSize mapSize = GameParams::getInstanse()->mapSize();
    
    if ( !initWithColor(ccc4(145,145,145,255 * 0.45f)) )
    {
        return false;
    }
    
    CCLabelTTF *label = CCLabelTTF::create(CCLocalizedString("RobotBroken", "NotLocalized"), "Marker Felt", 42);
    
    label->setAnchorPoint( CCPointZero );
    label->setPosition( CCPointZero );
    
    addChild(label, 20);
    
    setContentSize(CCSizeMake(label->getContentSize().width, label->getContentSize().height));
    setPosition(ccp(mapSize.width / 2 - getContentSize().width / 2, -getContentSize().height));
    
    return true;
}

void RobotLooseLayer::mainMenu(CCObject *sender)
{
    CCTransitionSlideInL* trans = CCTransitionSlideInL::create( 1.5f * SYSTEM_ANIMATION_DELAY, MainMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void RobotLooseLayer::nextLevel(CCObject *sender)
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

void RobotLooseLayer::selfRemove()
{
    parent->removeChild(this,true);
}

void RobotLooseLayer::show()
{
    CCSize mapSize = GameParams::getInstanse()->mapSize();
    
    CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY, ccp(mapSize.width / 2 - getContentSize().width / 2, 70));
    
    runAction( moveLayer );
}

void RobotLooseLayer::hide()
{
    retain();
    
    CCSize mapSize = GameParams::getInstanse()->mapSize();
    
    CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY,  ccp(mapSize.width / 2 - getContentSize().width / 2, -getContentSize().height));
    
    CCFiniteTimeAction* moveEnd = CCCallFunc::create( this,
                                                      callfunc_selector(RobotLooseLayer::selfRemove));
    
    runAction( CCSequence::create(moveLayer, moveEnd, NULL) );
}