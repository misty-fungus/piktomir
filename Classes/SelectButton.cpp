//
//  SelectButton.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 09.12.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#include "SelectButton.h"
#include "GameParams.h"
#include "PMTextureCache.h"

USING_NS_CC;

CCSprite *SelectButton::cloneSprite()
{
    CCSprite *newSpr = CCSprite::createWithTexture(((CCSprite *)this->getSelectedImage())->getTexture());
    newSpr->setPosition(getPosition());
    
    return newSpr;
}

void SelectButton::startAnimation()
{
    startPosition = getPosition();
    CCActionInterval* jump = CCJumpBy::create(2.0f * SYSTEM_ANIMATION_DELAY, ccp(0,0), getContentSize().height / 2.5, 1);
    //CCFiniteTimeAction* moveLayerDown = CCMoveTo::create(SYSTEM_ANIMATION_DELAY, ccp(getPositionX(),getPositionY()) );
    
    runnigAction = runAction( CCRepeatForever::create(jump) );
}

void SelectButton::jumpOnce()
{
    startPosition = getPosition();
    CCActionInterval* jump = CCJumpBy::create(2.0f * SYSTEM_ANIMATION_DELAY, ccp(0,0), getContentSize().height / 2.5, 1);
    
    runnigAction = runAction( jump );
}

void SelectButton::stopAnimation()
{
    if(runnigAction != NULL)
    {
        this->stopAction(runnigAction);
        runnigAction = NULL;
    }
    
    CCFiniteTimeAction *initial = CCMoveTo::create(SYSTEM_ANIMATION_DELAY / 2.0f, startPosition);
    
    runAction(initial);
}

void SelectButton::select()
{
    if(isMethod())
    {
        if(methodID >= EMPTY_METHOD)
            setNormalImage(PMTextureCache::instanse()->getRobotMethodSprite_Selected(methodID));
        else
            setNormalImage(PMTextureCache::instanse()->getProgramSprite_Selected(methodID));
    }
    
    if(isCondition())
        setNormalImage(PMTextureCache::instanse()->getRobotConditionSprite_Selected(condition));
    
    if(isRepeater())
        setNormalImage(PMTextureCache::instanse()->getRepeaterSprite_Selected(repeater));
    
}

void SelectButton::deselect()
{
    if(isMethod())
    {
        if(methodID >= EMPTY_METHOD)
            setNormalImage(PMTextureCache::instanse()->getRobotMethodSprite(methodID));
        else
            setNormalImage(PMTextureCache::instanse()->getProgramSprite(methodID));
    }
    
    if(isCondition())
        setNormalImage(PMTextureCache::instanse()->getRobotConditionSprite(condition));
    
    if(isRepeater())
        setNormalImage(PMTextureCache::instanse()->getRepeaterSprite(repeater));
}