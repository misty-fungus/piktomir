//
//  TransportRobot4.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/16/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "TransportRobot4.h"

#include "World4.h"
#include "GameParams.h"
#include "PMTextureCache.h"

USING_NS_CC;

using namespace std;


bool TransportRobot4::init()
{
    if( !CCSprite::init() )
        return false;
    
    stepCount = 0;
    
    //retain();
    
    return true;
}

void TransportRobot4::initTexture()
{
//    CCTexture2D *texture = PMTextureCache::instanse()->getRobotTexture( type() );
//    setTexture(texture);
    //setTextureRect( CCRectMake(curDirection * 64, 71.5 * burnState, 64.0, 71.5) );
}

void TransportRobot4::restartRobot()
{
    setPosition(startPosition, startLayer, startDirection);
}

void TransportRobot4::setPosition(CCPoint point, int layer, int direction)
{
    curDirection = state;
    curLayer = layer;
    curPosition = point;
    
    Map4 * map= ((World4 *)parentWorld)->getMap4();
    
    map->mapElements[curLayer][curPosition.y][curPosition.x]->envRobotHere = this;
    
    CCSprite::setPosition( map->Coordinates(curPosition.x, curPosition.y, -15, 35) );
}

void TransportRobot4::touch (PlayerRobot* robot)
{

    
}