//
//  HintLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/10/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "HintLayer.h"
#include "PMTextureCache.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

int HintLayer::hintCount;

HintLayer::~HintLayer()
{
    removeAllChildrenWithCleanup(true);
    --hintCount;
}

HintLayer *HintLayer::create(std::string text)
{
    HintLayer *pRet = new HintLayer(text);
    
    if(pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }

}

bool HintLayer::init()
{
    ++hintCount;
    
    if ( !initWithColor(ccc4(145,145,145,255 * 0.4)) )
    {
        return false;
    }
 
    CCLabelTTF *label = CCLabelTTF::create(text.c_str(), "Marker Felt", 20);
    CCSprite *sprite = PMTextureCache::instanse()->getInfoIconSprite();
    
    setContentSize( CCSizeMake(label->getContentSize().width + sprite->getContentSize().width,sprite->getContentSize().height) );
    
    label->setPosition( ccp(sprite->getContentSize().width,0) );
    sprite->setPosition( CCPointZero );
    label->setAnchorPoint( ccp(0,0) );
    sprite->setAnchorPoint( ccp(0,0) );   
    
    addChild(label);
    addChild(sprite);
    
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    
    setPosition( ccp(160, screenSize.height - 50 - hintCount * sprite->getContentSize().height) );
    
    return true;
}
#ifdef MAP_EDITOR
int HintEditLayer::hintCount;

HintEditLayer::~HintEditLayer()
{
    --hintCount;
}

HintEditLayer *HintEditLayer::create(std::string text, std::string placeHolder)
{
    HintEditLayer *pRet = new HintEditLayer(text, placeHolder);
    
    if(pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

bool HintEditLayer::init()
{
    ++hintCount;
    
    if ( !initWithColor(ccc4(145,145,145,255 * 0.4)) )
    {
        return false;
    }
    
    CCScale9Sprite* EditBoxSprite = CCScale9Sprite::create("System/scalesprite.png");
    
    CCSprite *sprite = PMTextureCache::instanse()->getInfoIconSprite();
    hint = CCEditBox::create(CCSizeMake(400, 30), EditBoxSprite);
    hint->setFontColor(ccBLACK);
    
    hint->setPlaceholderFont("Marker Felt",18);
    hint->setFont("Marker Felt",18);
    
    hint->setPlaceHolder(placeHolder.c_str());
    hint->setText(text.c_str());
    
    hint->setAnchorPoint(CCPointZero);
    hint->setPosition(ccp(sprite->getContentSize().width,0));
    
    setContentSize( CCSizeMake(hint->getContentSize().width + sprite->getContentSize().width,sprite->getContentSize().height) );
    
    sprite->setPosition( CCPointZero );
    sprite->setAnchorPoint( ccp(0,0) );
    
    addChild(hint);
    addChild(sprite);
    
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    
    setPosition( ccp(160, screenSize.height - 50 - hintCount * sprite->getContentSize().height) );
    
    return true;
}
#endif

