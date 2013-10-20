//
//  Robot4.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#include "Robot4.h"
#include "World4.h"
#include "GameParams.h"
#include "GameLayer.h"
#include "PMTextureCache.h"
#include "EnvRobot2D.h"

USING_NS_CC;

using namespace std;

int sign (int a)
{
    if (a > 0) {
        return 1;
    } else if (a < 0) {
        return -1;
    } else return 0;
}

bool Robot4::init()
{
    if( !CCSprite::init() )
        return false;
    
    stepCount = 0;

    //retain();
    
    std::pair<int,RobotMethod> _move(Robot4::Move, RobotMethod(this,(ROBOT_NATIVE_METHOD)&Robot4::move, Robot4::MoveReverse));
    std::pair<int,RobotMethod> _turnLeft(Robot4::TurnLeft, RobotMethod(this,(ROBOT_NATIVE_METHOD)&Robot4::turnLeft,Robot4::TurnRight));
    std::pair<int,RobotMethod> _turnRight(Robot4::TurnRight, RobotMethod(this,(ROBOT_NATIVE_METHOD)&Robot4::turnRight,Robot4::TurnLeft));
    std::pair<int,RobotMethod> _paint(Robot4::Paint, RobotMethod(this,(ROBOT_NATIVE_METHOD)&Robot4::paint, Robot4::UnPaint));
    std::pair<int,RobotMethod> _moveReverse(Robot4::MoveReverse, RobotMethod(this,(ROBOT_NATIVE_METHOD)&Robot4::moveReverse, Robot4::Move));
    std::pair<int,RobotMethod> _unPaint(Robot4::UnPaint, RobotMethod(this,(ROBOT_NATIVE_METHOD)&Robot4::unPaint, Robot4::Paint));
    
    methodList.insert(_move);
    methodList.insert(_turnLeft);
    methodList.insert(_turnRight);
    methodList.insert(_paint);
    methodList.insert(_moveReverse);
    methodList.insert(_unPaint);
    
    return true;
}

void Robot4::initTexture()
{
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile(PICT_PATH "Robot/robot4.plist");
    
    CCSpriteFrame *frame = cache->spriteFrameByName(CCString::createWithFormat("robot%d.png" , 5 * curDirection)->m_sString.c_str());
    
    setDisplayFrame(frame);
    
    removeAllChildrenWithCleanup(true);
}

void Robot4::restartRobot()
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    removeAllChildrenWithCleanup(true);
    stopAllActions();
    setPosition(startPosition, startLayer, startDirection);
    map->getControlLayer()->setStepCount(0);
}

void Robot4::setPosition(CCPoint point, int layer, int direction)
{
    oldPosition = curPosition = point;
    oldDirection = curDirection = direction;
    oldLayer = curLayer = layer;
    
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    map->mapElements[layer][point.y][point.x]->isRobotHere = true;
    map->mapElements[layer][point.y][point.x]->robotHere = this;
        
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile(PICT_PATH "Robot/robot4.plist");
    
    CCSpriteFrame *frame = cache->spriteFrameByName(CCString::createWithFormat("robot%d.png" , 5 * curDirection)->m_sString.c_str());
    
    setDisplayFrame(frame);

    
    CCSprite::setPosition( map->Coordinates(point.x, point.y, -25, 25) );
}


bool Robot4::isGrassAt(CCPoint point, int layer)
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    return (map->mapElements[layer][point.y][point.x]->getType() == MapElement::GreenGrass ||
            map->mapElements[layer][point.y][point.x]->getType() == MapElement::BlueGrass ||
            map->mapElements[layer][point.y][point.x]->getType() == MapElement::Grass ||
            map->mapElements[layer][point.y][point.x]->getType() == MapElement::Water);
}

void Robot4::moveUp()
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    --curPosition.y;
    
    if(curPosition.y < 0 || map->mapElements[curLayer][curPosition.y+1][curPosition.x]->nearElements[Map4::Up] == NULL || !isGrassAt(curPosition, curLayer))
    {
        state = UniversalRobot::Broken;
    }
    
    
    map->mapElements[curLayer][curPosition.y+1][curPosition.x]->isRobotHere = false;
    map->mapElements[curLayer][curPosition.y+1][curPosition.x]->robotHere = NULL;
    
    map->mapElements[curLayer][curPosition.y][curPosition.x]->isRobotHere = true;
    map->mapElements[curLayer][curPosition.y][curPosition.x]->robotHere=this;
    
}

void Robot4::moveDown()
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    int height = map->getHeight();
    
    ++curPosition.y;
    
    if(curPosition.y > height || map->mapElements[curLayer][curPosition.y-1][curPosition.x]->nearElements[Map4::Down] == NULL || !isGrassAt(curPosition, curLayer))
    {
        state = UniversalRobot::Broken;
    }
    
    map->mapElements[curLayer][curPosition.y-1][curPosition.x]->isRobotHere = false;
    map->mapElements[curLayer][curPosition.y-1][curPosition.x]->robotHere = NULL;
    
    map->mapElements[curLayer][curPosition.y][curPosition.x]->isRobotHere = true;
    map->mapElements[curLayer][curPosition.y][curPosition.x]->robotHere=this;
}

void Robot4::moveLeft()
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    --curPosition.x;
    
    if(curPosition.x < 0 || map->mapElements[curLayer][curPosition.y][curPosition.x+1]->nearElements[Map4::Left] == NULL || !isGrassAt(curPosition, curLayer)) {
        state = UniversalRobot::Broken;
    }
    
    map->mapElements[curLayer][curPosition.y][curPosition.x+1]->isRobotHere = false;
    map->mapElements[curLayer][curPosition.y][curPosition.x+1]->robotHere = NULL;
    
    map->mapElements[curLayer][curPosition.y][curPosition.x]->isRobotHere = true;
    map->mapElements[curLayer][curPosition.y][curPosition.x]->robotHere=this;
    
}

void Robot4::moveRight()
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    int width = map->getWidth();
    
    ++curPosition.x;
    
    if(curPosition.x > width || map->mapElements[curLayer][curPosition.y][curPosition.x-1]->nearElements[Map4::Right] == NULL || !isGrassAt(curPosition, curLayer))
    {
        state = UniversalRobot::Broken;
    }
    
    map->mapElements[curLayer][curPosition.y][curPosition.x-1]->isRobotHere = false;
    map->mapElements[curLayer][curPosition.y][curPosition.x-1]->robotHere = NULL;
    
    map->mapElements[curLayer][curPosition.y][curPosition.x]->isRobotHere = true;
    map->mapElements[curLayer][curPosition.y][curPosition.x]->robotHere=this;
}

void Robot4::endMove()
{
    setOldPosition(getCurPosition());
    
    if(getTask() == NULL)
        state = UniversalRobot::Ready;
    else if(task->Complete())
        state = UniversalRobot::Win;
    else
        state = UniversalRobot::Ready;
    
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    map->getControlLayer()->setStepCount(stepCount);
    map->centerMap();
}

void Robot4::endPaint()
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    CCSprite *tmp = map->mapElements[curLayer][curPosition.y][curPosition.x]->getGrassSprite();
    
    if(map->mapElements[curLayer][curPosition.y][curPosition.x]->isPainted) {
        
    }
    else {
        map->mapElements[curLayer][curPosition.y][curPosition.x]->isPainted = true;
        task->PaintedGrassInc();//map->incPaintedGrass();
        tmp->setTextureRect( CCRectMake(3*64.0, 0.0, 64.0, 64.0) );
    }
    
    if(getTask() == NULL)
        state = UniversalRobot::Ready;
    else if(task->Complete())
        state = UniversalRobot::Win;
    else
        state = UniversalRobot::Ready;
    
    map->getControlLayer()->setStepCount(stepCount);
}

void Robot4::endUnPaint()
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    CCSprite *tmp = map->mapElements[curLayer][curPosition.y][curPosition.x]->getGrassSprite();
    
    if(map->mapElements[curLayer][curPosition.y][curPosition.x]->isPainted) {
        map->mapElements[curLayer][curPosition.y][curPosition.x]->isPainted = false;
        task->PaintedGrassDec();//map->incPaintedGrass();
        tmp->setTextureRect( CCRectMake(2*64.0, 0.0, 64.0, 64.0) );
    }
    else {

    }
    
    if(getTask() == NULL)
        state = UniversalRobot::Ready;
    else if(task->Complete())
        state = UniversalRobot::Win;
    else
        state = UniversalRobot::Ready;
    
    map->getControlLayer()->setStepCount(stepCount);
}

void Robot4::endTurn()
{
    oldDirection = curDirection;
    
    
    if(getTask() == NULL)
        state = UniversalRobot::Ready;
    else if(task->Complete())
        state = UniversalRobot::Win;
    else
        state = UniversalRobot::Ready;
    
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    map->getControlLayer()->setStepCount(stepCount);
}

void Robot4::endTouch()
{
    if(getTask() == NULL)
        state = UniversalRobot::Ready;
    else if(task->Complete() )
        state = UniversalRobot::Win;
    else
        state = UniversalRobot::Ready;
    
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    map->getControlLayer()->setStepCount(stepCount);
}

void Robot4::moveReverse()
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    int X = curPosition.x;
    int Y = curPosition.y;
    
    switch(curDirection)
    {
        case 0 : Y--; break;
        case 1 : X--; break;
        case 2 : Y++; break;
        case 3 : X++; break;
    }
    
    bool move = true;
    
    if(map->mapElements[getCurLayer()][Y][X]->envRobotHere != NULL)
    {
        ((EnvRobot *)map->mapElements[getCurLayer()][Y][X]->robotHere)->touch(this);
        
        ++stepCount;
        
        if(state != UniversalRobot::Broken)
        {
            state = UniversalRobot::Working;
            CCDelayTime *TouchDelay = CCDelayTime::create(GetGameParams->getVelocity() / 2);
            CCFiniteTimeAction* TouchEnd = CCCallFunc::create( this,
                                                               callfunc_selector(Robot4::endTouch));
            runAction( CCSequence::create(TouchDelay,TouchEnd, NULL) );
        }
    }
    
    if(move)
    {
        
        switch(curDirection)
        {
            case 0 : {
                moveUp();
                map->update();
                break;
            }
            case 1 : {
                moveLeft();
                map->update();
                break;
            }
            case 2 : {
                map->update();
                moveDown();
                break;
            }
            case 3 : {
                map->update();
                moveRight();
                break;
            }
                
        }
        
        //        if(getTask() == NULL)
        //            state = UniversalRobot::Ready;
        //
        //        if(getTask() != NULL && getTask()->Complete())
        //            state = UniversalRobot::Win;
        
        ++stepCount;
        
        if(state != UniversalRobot::Broken /*&& state != UniversalRobot::Win*/ )
        {
            state = UniversalRobot::Working;
            CCFiniteTimeAction* MoveRobot = CCMoveTo::create(GetGameParams->getVelocity() / 2,
                                                             map->Coordinates(curPosition.x, curPosition.y, -25, 25) );
            CCFiniteTimeAction* MoveEnd = CCCallFunc::create( this,
                                                              callfunc_selector(Robot4::endMove));
            runAction( CCSequence::create(MoveRobot, MoveEnd, NULL) );
        }
    }
}

void Robot4::move()
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    int X = curPosition.x;
    int Y = curPosition.y;
    
    switch(curDirection)
    {
        case 0 : Y++; break;
        case 1 : X++; break;
        case 2 : Y--; break;
        case 3 : X--; break;
    }
    
    bool move = true;
    
    if(map->mapElements[getCurLayer()][Y][X]->envRobotHere != NULL)
    {
        ((EnvRobot *)map->mapElements[getCurLayer()][Y][X]->robotHere)->touch(this);
        
        ++stepCount;
        
        if(state != UniversalRobot::Broken)
        {
            state = UniversalRobot::Working;
            CCDelayTime *TouchDelay = CCDelayTime::create(GetGameParams->getVelocity() / 2);
            CCFiniteTimeAction* TouchEnd = CCCallFunc::create( this,
                                                              callfunc_selector(Robot4::endTouch));
            runAction( CCSequence::create(TouchDelay,TouchEnd, NULL) );
        }
    }
    if(move)
    {
        
        switch(curDirection)
        {
            case 0 : {
                moveDown();
                map->update();
                break;
            }
            case 1 : {
                moveRight();
                map->update();
                break;
            }
            case 2 : {
                map->update();
                moveUp();
                break;
            }
            case 3 : {
                map->update();
                moveLeft();
                break;
            }

        }
        
//        if(getTask() == NULL)
//            state = UniversalRobot::Ready;
//        
//        if(getTask() != NULL && getTask()->Complete())
//            state = UniversalRobot::Win;
        
        ++stepCount;
         
        if(state != UniversalRobot::Broken /*&& state != UniversalRobot::Win*/ )
        {
            state = UniversalRobot::Working;
            CCFiniteTimeAction* MoveRobot = CCMoveTo::create(GetGameParams->getVelocity() / 2,
                                                              map->Coordinates(curPosition.x, curPosition.y, -25, 25) );
            CCFiniteTimeAction* MoveEnd = CCCallFunc::create( this,
                                                              callfunc_selector(Robot4::endMove));
            runAction( CCSequence::create(MoveRobot, MoveEnd, NULL) );
        }
    }
}

void Robot4::turnLeft()
{
    
    curDirection = (curDirection + 1) % 4;
    ++stepCount;
    
    state = UniversalRobot::Working;
    
    CCAnimationCache *cache = CCAnimationCache::sharedAnimationCache();
    cache->addAnimationsWithFile(PICT_PATH "Robot/robot4-animations.plist");
    CCAnimation *animation = cache->animationByName(CCString::createWithFormat("%d->%d" , oldDirection, curDirection)->m_sString.c_str());
    animation->setDelayPerUnit(GetGameParams->getVelocity()/5);
    CCAnimate *animate = CCAnimate::create(animation);

    
    CCFiniteTimeAction* TurnEnd = CCCallFunc::create( this,
                                                       callfunc_selector(Robot4::endTurn));
    runAction( CCSequence::create(animate,TurnEnd, NULL) );
}

void Robot4::turnRight()
{
    curDirection = (curDirection + 3) % 4;
    ++stepCount;
    
    state = UniversalRobot::Working;
    
    CCAnimationCache *cache = CCAnimationCache::sharedAnimationCache();
    cache->addAnimationsWithFile(PICT_PATH "Robot/robot4-animations.plist");
    CCAnimation *animation = cache->animationByName(CCString::createWithFormat("%d->%d" , oldDirection, curDirection)->m_sString.c_str());
    animation->setDelayPerUnit(GetGameParams->getVelocity()/5);
    CCAnimate *animate = CCAnimate::create(animation);
    
    CCFiniteTimeAction* TurnEnd = CCCallFunc::create( this,
                                                      callfunc_selector(Robot4::endTurn));
    runAction( CCSequence::create(animate,TurnEnd, NULL) );
}

void Robot4::paint()
{
    World4 *world = (World4 *)getParentWorld();
	Map4 *map = world->getMap4();
    
    if (map->mapElements[getCurLayer()][curPosition.y][curPosition.x]->getType() != MapElement::GreenGrass) {
			return;
	}
    
    ++stepCount;
    state = UniversalRobot::Working;
    
    CCAnimationCache *cache = CCAnimationCache::sharedAnimationCache();
    cache->addAnimationsWithFile(PICT_PATH "Robot/robot4-animations.plist");
    CCAnimation *animation = cache->animationByName(CCString::createWithFormat("paint%d", curDirection)->m_sString.c_str());
    animation->setDelayPerUnit(GetGameParams->getVelocity()/3.5);
    CCAnimate *animate = CCAnimate::create(animation);

    
    CCFiniteTimeAction* PaintEnd = CCCallFunc::create( this,
                                                      callfunc_selector(Robot4::endPaint));
    runAction( CCSequence::create(animate,PaintEnd, NULL) );
}

void Robot4::unPaint()
{
    World4 *world = (World4 *)getParentWorld();
	Map4 *map = world->getMap4();
    
    if (map->mapElements[getCurLayer()][curPosition.y][curPosition.x]->getType() != MapElement::GreenGrass) {
        return;
	}
    
    ++stepCount;
    state = UniversalRobot::Working;
    
    CCAnimationCache *cache = CCAnimationCache::sharedAnimationCache();
    cache->addAnimationsWithFile(PICT_PATH "Robot/robot4-animations.plist");
    CCAnimation *animation = cache->animationByName(CCString::createWithFormat("paint%d", curDirection)->m_sString.c_str());
    animation->setDelayPerUnit(GetGameParams->getVelocity()/3.5);
    CCAnimate *animate = CCAnimate::create(animation);
    
    CCFiniteTimeAction* PaintEnd = CCCallFunc::create( this,
                                                       callfunc_selector(Robot4::endUnPaint));
    runAction( CCSequence::create(animate,PaintEnd, NULL) );
}

void Robot4::win()
{
    //fireWorks = CCParticleFireworks::create();
    
    //fireWorks->setPosition(CCPointZero);
    
    //addChild(fireWorks);
}

void Robot4::destroy()
{    
    CCAnimationCache *cache = CCAnimationCache::sharedAnimationCache();
    cache->addAnimationsWithFile(PICT_PATH "Robot/robot4-animations.plist");
    CCAnimation *animation = cache->animationByName(CCString::createWithFormat("destroy%d", curDirection)->m_sString.c_str());
    animation->setDelayPerUnit(GetGameParams->getVelocity()/3);
    CCAnimate *animate = CCAnimate::create(animation);

    
    runAction( CCRepeatForever::create(animate) );
}

bool Robot4::checkCondition(int condition)
{
    Condition cond  = (Condition) condition;
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    ++stepCount;
    map->getControlLayer()->setStepCount(stepCount);
    
    if (cond == Robot4::GrassIsBlue) {
        return map->mapElements[curLayer][curPosition.y][curPosition.x]->isPainted;
    } else if (cond == Robot4::GrassIsGreen) {
        return !map->mapElements[curLayer][curPosition.y][curPosition.x]->isPainted;
    } else if (cond == Robot4::Wall) {
        return isNearWall();
    } else if (cond == Robot4::NoWall) {
        return !isNearWall();
    }
    return true;
}

bool Robot4::isNearWall()
{
    Map4 *map = ((World4 *)parentWorld)->getMap4();
    
    if (curDirection == 0) {
        return map->mapElements[curLayer][curPosition.y][curPosition.x]->nearElements[Map4::Down] == NULL;
    } else if (curDirection == 1) {
        return map->mapElements[curLayer][curPosition.y][curPosition.x]->nearElements[Map4::Right] == NULL;
    } else if (curDirection == 2) {
        return map->mapElements[curLayer][curPosition.y][curPosition.x]->nearElements[Map4::Up] == NULL;
    } else {
        return map->mapElements[curLayer][curPosition.y][curPosition.x]->nearElements[Map4::Left] == NULL;
    }
}