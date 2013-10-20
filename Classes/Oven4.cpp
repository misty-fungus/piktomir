//
//  Furnace.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 3/14/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "World4.h"
#include "GameParams.h"
#include "GameLayer.h"
#include "Oven4.h"
#include "PMTextureCache.h"

USING_NS_CC;

using namespace std;


bool Oven4::init()
{
    if( !CCSprite::init() )
        return false;
    
    stepCount = 0;
    
    return true;
}

void Oven4::initTexture()
{
    CCTexture2D *texture = PMTextureCache::instanse()->getRobotTexture( type() );
    setTexture(texture);
    setTextureRect( CCRectMake(curDirection * 64, 71.5 * burnState, 64.0, 71.5) );
}

void Oven4::restartRobot()
{
    burnState = Off;
    setPosition(startPosition, startLayer, startDirection);
}

void Oven4::setPosition(CCPoint point, int layer, int direction)
{
    curDirection = state;
    curLayer = layer;
    curPosition = point;
    
    Map4 * map= ((World4 *)parentWorld)->getMap4();
    
    map->mapElements[curLayer][curPosition.y][curPosition.x]->envRobotHere = this;
    
    setTextureRect( CCRectMake(curDirection * 64.0, 71.5 * burnState, 64.0, 71.5) );
    CCSprite::setPosition( map->Coordinates(curPosition.x, curPosition.y, -15, 35) );
}

void Oven4::touch (PlayerRobot* robot)
{
    burnState = (BurnState)(1-burnState);
    setTextureRect( cocos2d::CCRectMake(curDirection * 64.0, 71.5 * burnState, 64.0, 71.5) );
    
    robot->getTask()->BurnedFurnaceInc();
}
