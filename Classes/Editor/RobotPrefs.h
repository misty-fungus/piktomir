//
//  RobotPrefs.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 15.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__RobotPrefs__
#define __pictomir_cocos2d_x__RobotPrefs__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameParams.h"

class RobotPrefsScrolLayer : public cocos2d::CCLayer
{
public:
    
    RobotPrefsScrolLayer() :
    cocos2d::CCLayer()
    {
        activeRobot = 0;
        active = true;
        init();
    }
    
    virtual void visit();
    
    void finishLoading ();
    
    cocos2d::CCRect frame;
    
    static RobotPrefsScrolLayer * instanse;
    static RobotPrefsScrolLayer * getInstanse();
    
protected:
    SYNTHESIZE(bool, active, Active);
    SYNTHESIZE(int, activeRobot, ActiveRobot);
private:
    cocos2d::CCPoint startPoint;
    
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
};

#endif /* defined(__pictomir_cocos2d_x__RobotPrefs__) */
