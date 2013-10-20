//
//  PMMapPreview.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 01.08.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "PMMapPreview.h"
#include "utils.h"
#include "XmlWorldReader.h"
#include "UniversalWorld.h"
#include "ScrollLayer.h"

USING_NS_CC;

using namespace std;

bool PMMapPreview::init()
{
    if (!CCLayerColor::init()) {
        return false;
    }
    
    setContentSize(CCSizeMake(PMMapPreviewWidth, PMMapPreviewHeight));
    
    setColor(ccGRAY);
    setOpacity(144);
    
    PMMapPreview::world = 0;
    PMMapPreview::map = 0;
    
    CCLabelTTF * label = CCLabelTTF::create("Новая карта", "Marker Felt", 22.0);
    label->setAnchorPoint(ccp(0.5, 0));
    label->setPosition(ccp(PMMapPreviewWidth / 2, 5));
    
    CCSprite * sprite = CCSprite::create("System/Preview.png");
    sprite->setAnchorPoint(ccp(0.5, 0.5));
    sprite->setPosition(ccp(PMMapPreviewWidth / 2, PMMapPreviewHeight / 2));
    
    addChild(sprite);
    addChild(label);
    
    return true;
}

bool PMMapPreview::initWithWorldMap(int world, int map)
{
    if (!CCLayerColor::init()) {
        return false;
    }
    
    setContentSize(CCSizeMake(PMMapPreviewWidth, PMMapPreviewHeight));
    
    setColor(ccGRAY);
    setOpacity(144);
    
    PMMapPreview::world = world;
    PMMapPreview::map = map;
    
    CCString mapPathString;
    mapPathString.initWithFormat("%s/World%d/Lvl%d.map", GameParams::getInstanse()->mapPath.m_sString.c_str(), world, map);
    
    std::string mapPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(mapPathString.m_sString.c_str());
    
    XmlWorldReader xml;
    UniversalWorld *worldClass = xml.readWorld(NULL,mapPath);
    
    if(worldClass != NULL)
    {
        worldClass->getMap()->getScroll()->getParallax()->removeChild(worldClass->getMap()->getLayer(),true);
        
        MapContainer *mapLayer = worldClass->getMap()->getLayer();
        

        
        addChild( worldClass->getMap()->getLayer(), 0);
        
        worldClass->getMap()->getLayer()->setScaleX(getContentSize().width /  mapLayer->getContentSize().width);
        worldClass->getMap()->getLayer()->setScaleY(getContentSize().width /  mapLayer->getContentSize().width / 2);
        
        
        mapLayer->setPosition( ccp(PMMapPreviewWidth / 2, PMMapPreviewHeight / 2) );
        
        CCLabelTTF * label = CCLabelTTF::create(("Карта "+to_string(map)).c_str(), "Marker Felt", 22.0);
        label->setAnchorPoint(ccp(0.5, 0));
        label->setPosition(ccp(PMMapPreviewWidth / 2, 5));
        
        addChild(label , 1);
        
        worldClass->getMap()->removeAllChildrenWithCleanup(true);
        
        delete worldClass;
    }
    else
    {
        char errorString[255];
        
        sprintf(errorString, "Load map error!\n%s",xml.getError().c_str());
        
        CCLabelTTF * label = CCLabelTTF::create(errorString, "Marker Felt", 15.0);
        label->setAnchorPoint(ccp(0.5, 0.5));
        label->setPosition(ccp(PMMapPreviewWidth / 2, PMMapPreviewHeight / 2));
        
        addChild(label);
    }
    
    
    return true;
}

void PMMapPreview::visit()
{
    CCPoint zeroPoint = convertToWorldSpace( CCPointZero );
    
     CCRect scissorRect = CCRect(zeroPoint.x, zeroPoint.y, getContentSize().width, getContentSize().height);
     
     kmGLPushMatrix();
     glEnable(GL_SCISSOR_TEST);
     
     CCEGLView::sharedOpenGLView()->setScissorInPoints(scissorRect.origin.x, scissorRect.origin.y,
                                                        scissorRect.size.width, scissorRect.size.height);
     
     CCNode::visit();
     glDisable(GL_SCISSOR_TEST);
     kmGLPopMatrix();
 

    
}

PMMapPreview* PMMapPreview::create(int world, int map)
{
    PMMapPreview *pRet = new PMMapPreview();
    pRet->initWithWorldMap(world, map);
    pRet->autorelease();
    return pRet;
}

PMMapPreview* PMMapPreview::create()
{
    PMMapPreview *pRet = new PMMapPreview();
    pRet->init();
    pRet->autorelease();
    return pRet;
}