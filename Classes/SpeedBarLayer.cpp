//
//  SpeedBarLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "SpeedBarLayer.h"

USING_NS_CC;

bool SpeedBarLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSize screenSize = GameParams::getInstanse()->screenSize;
    
    m_bTouchEnabled = true;
    
    
    CCSprite *speedBack = CCSprite::create(PICT_PATH "System/speedBar_Empty.png");
    speedBack->setPosition( CCPointZero );
    speedBack->setAnchorPoint( CCPointZero );
    
    addChild(speedBack, 0);
    
    double velocity = GameParams::getInstanse()->getVelocity();
    
    CCTexture2D *speedTexture = CCTextureCache::sharedTextureCache()->addImage(PICT_PATH "System/speedBar.png");
    
    float width  = speedTexture->getContentSize().width * (GetGameParams->maxVelocity - velocity);
    float height = speedTexture->getContentSize().height;
    
    speed = CCSprite::createWithTexture(speedTexture, CCRectMake(0.0, 0.0, width, height) );
    speed->setPosition( CCPointZero );
    speed->setAnchorPoint( CCPointZero );
    
    addChild(speed, 1);

    setContentSize( speedBack->getContentSize() );

    return true;
}

void SpeedBarLayer::fast(CCObject *sender)
{
    double velocity = GameParams::getInstanse()->getVelocity();
    double maxVelocity = GameParams::getInstanse()->maxVelocity;
    double minVelocity = GameParams::getInstanse()->minVelocity;
    velocity-=0.05;
    if (velocity < minVelocity) {
        velocity = minVelocity;
    }
    GameParams::getInstanse()->setVelocity(velocity);
    
    double ratio = (maxVelocity - velocity) /(maxVelocity - minVelocity);
    speed->setTextureRect( CCRectMake(0.0, 0.0, 100.0 * ratio, 10.0) );
    speed->setPosition(ccp( 19 + 50.0 * ratio, GameParams::getInstanse()->screenSize.height - 9));
}

void SpeedBarLayer::slow(CCObject *sender)
{
    double velocity = GameParams::getInstanse()->getVelocity();
    double maxVelocity = GameParams::getInstanse()->maxVelocity;
    double minVelocity = GameParams::getInstanse()->minVelocity;
    velocity+=0.05;
    if (velocity > maxVelocity) {
        velocity = maxVelocity;
    }
    GameParams::getInstanse()->setVelocity(velocity);
    
    double ratio = (maxVelocity - velocity) /(maxVelocity - minVelocity);
    speed->setTextureRect( CCRectMake(0.0, 0.0, 100.0 * ratio, 10.0) );
    speed->setPosition(ccp( 19 + 50.0 * ratio, GameParams::getInstanse()->screenSize.height - 9));
    
}

void SpeedBarLayer::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();
}

void SpeedBarLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool SpeedBarLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if(!enabled)
        return false;
    
    CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
    
    if(boundingBox().containsPoint(getParent()->convertTouchToNodeSpace(touch)) )
    {
        float velocity = GetGameParams->maxVelocity - touchPoint.x / getContentSize().width;
        
        if(touchPoint.x / getContentSize().width >= GetGameParams->minVelocity)
        {
            float width  = touchPoint.x;
            float height = getContentSize().height;
            
            speed->setTextureRect( CCRectMake(0, 0, width, height));
            
            GameParams::getInstanse()->setVelocity(velocity);
            

        }
        return true;
    }
    
    return false;
}


void SpeedBarLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
 
    if(touchPoint.x < 0 || touchPoint.x > getContentSize().width)
        return;
    
    float velocity = GetGameParams->maxVelocity - touchPoint.x / getContentSize().width;
    
    if(touchPoint.x / getContentSize().width >= GetGameParams->minVelocity)
    {
        float width  = touchPoint.x;
        float height = getContentSize().height;
        
        speed->setTextureRect( CCRectMake(0, 0, width, height));
        
        GameParams::getInstanse()->setVelocity(velocity);
        
        
    }
    
}

void SpeedBarLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
 
    if(touchPoint.x < 0 || touchPoint.x > getContentSize().width)
        return;
    
    float velocity = GetGameParams->maxVelocity - touchPoint.x / getContentSize().width;
    
    if(touchPoint.x / getContentSize().width >= GetGameParams->minVelocity)
    {
        float width  = touchPoint.x;
        float height = getContentSize().height;
        
        speed->setTextureRect( CCRectMake(0, 0, width, height));
        
        GameParams::getInstanse()->setVelocity(velocity);
        
        
    }
    
}

void SpeedBarLayer::setEnabled(bool flag)
{
    enabled = flag;
    //speedMenu->setEnabled(flag);
}

