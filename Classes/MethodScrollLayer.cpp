//
//  MethodScrollLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/19/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "MethodScrollLayer.h"

#include "MethodStackLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

MethodScrollLayer *MethodScrollLayer::create(MethodStackLayer *parent)
{
    MethodScrollLayer *pRet = new MethodScrollLayer(parent);
    
    if(pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

bool MethodScrollLayer::init()
{
    if(!CCScrollView::init())
        return false;

    return true;
}


bool MethodScrollLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if(!active)
        return false;
    
    CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
    
    if(parent->getLastSprite()!= NULL && parent->getLastSprite()->boundingBox().containsPoint(touchPoint))
    {
        startPoint = parent->getLastSprite()->getPosition();
        dragStarted = true;
        removed = false;
        return true;
    }
    
    return CCScrollView::ccTouchBegan(touch,pEvent);
}


void MethodScrollLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if(dragStarted)
    {
        CCPoint diff = touch->getDelta();
        diff.x = 0;
        
        CCPoint currentPos = parent->getLastSprite()->getPosition();
        parent->getLastSprite()->setPosition( ccpAdd(currentPos, diff) );
    }
    
    CCScrollView::ccTouchMoved(touch,pEvent);
}

void MethodScrollLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if(dragStarted)
    {
        if(parent->getLastSprite()->getPositionY() > getContentSize().height && !removed)
        {
            removed = true;
            parent->removeMethod();
        }
        else
        {
            CCFiniteTimeAction* moveButton = CCMoveTo::create(SYSTEM_ANIMATION_DELAY / 2, startPoint);
            parent->getLastSprite()->runAction(moveButton);
        }
    }
    
    CCScrollView::ccTouchEnded(touch,pEvent);
}
