//
//  RobotPrefs.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 15.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "RobotPrefs.h"
#include "MapEditorGameLayer.h"
#include "Robot4Edit.h"

USING_NS_CC;

RobotPrefsScrolLayer* RobotPrefsScrolLayer::instanse = NULL;

RobotPrefsScrolLayer* RobotPrefsScrolLayer::getInstanse() {
    if(!instanse) instanse = new RobotPrefsScrolLayer;
    return instanse;
}

void RobotPrefsScrolLayer::visit()
{
    CCRect scissorRect = CCRect(frame.origin.x, CCDirector::sharedDirector()->getWinSize().height-frame.size.height, frame.size.width, frame.size.height);
    
    kmGLPushMatrix();
    glEnable(GL_SCISSOR_TEST);
    
    CCEGLView::sharedOpenGLView()->setScissorInPoints(scissorRect.origin.x, scissorRect.origin.y,
                                                     scissorRect.size.width, scissorRect.size.height);
    
    CCNode::visit();
    glDisable(GL_SCISSOR_TEST);
    kmGLPopMatrix();

}

void RobotPrefsScrolLayer::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate (this, 0);
    CCLayer::onEnter();
}
//
void RobotPrefsScrolLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
//#endif
bool RobotPrefsScrolLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if(!active)
        return false;
    if(!frame.containsPoint(ccpAdd(this->convertTouchToNodeSpace(touch), getPosition()))) return false;
    
    CCPoint touchPoint = touch->getStartLocation();

    
    startPoint = touchPoint;
    return true;
}


void RobotPrefsScrolLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    printf("%f\n",touch->getDelta().x);
    
    setPosition(ccpAdd(getPosition(),ccp(touch->getDelta().x,0)));
}

void RobotPrefsScrolLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if(getPosition().x + frame.size.width * MapEditorGameLayer::getInstanse()->getWorld()->robotCount() < frame.origin.x + frame.size.width) {
        activeRobot = MapEditorGameLayer::getInstanse()->getWorld()->robotCount() - 1;
        setPosition(ccp(frame.origin.x - frame.size.width * activeRobot,0));
        return;
    }
    if (getPosition().x < frame.origin.x ) {
        activeRobot = 0;
        setPosition(ccp(frame.origin.x,0));
        return;
    }
    
    activeRobot = (int)((-getPosition().x+frame.origin.x)/frame.size.width - 0.0001 + 1/2);
    
    setPosition(ccp(frame.origin.x + activeRobot * frame.size.width, 0));
}

void RobotPrefsScrolLayer::finishLoading() {
    removeAllChildrenWithCleanup(true);
    for (int i = 0; i < MapEditorGameLayer::getInstanse()->getWorld()->robotCount(); ++i) {
        UniversalRobot * robot = MapEditorGameLayer::getInstanse()->getWorld()->robotAt(i);
        
        if (robot->type() == pmRobot4) {
                Robot4Edit * robot4Edit = Robot4Edit::create();
                robot4Edit->setAnchorPoint(ccp(0,i*frame.size.width));
                robot4Edit->setPosition(0,frame.size.height);
                robot4Edit->setRobot((Robot4*)robot);
                addChild(robot4Edit);
        }
    }
}

