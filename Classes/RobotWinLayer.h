//
//  RobotWinLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__RobotWinLayer__
#define __pictomir_cocos2d_x__RobotWinLayer__

#include <iostream>
#include "cocos2d.h"

class GameLayer;

class RobotWinLayer : public cocos2d::CCLayerColor
{
public:
    
    RobotWinLayer():
        cocos2d::CCLayerColor(),
        stepCount(0),
        parent(NULL)
    {}
    
    RobotWinLayer(GameLayer *parent,int stepCount):
        cocos2d::CCLayerColor(),
        stepCount(stepCount),
        parent(parent)
    {}
    
    virtual bool init();
    
    CREATE_FUNC(RobotWinLayer);
    
    static RobotWinLayer *create(GameLayer *parent, int stepCount);
    
    void show();
    void hide();
    
    ~RobotWinLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
private:
    int stepCount;
    GameLayer *parent;
    
    void mainMenu(CCObject *sender);
    void nextLevel(CCObject *sender);
    
    void selfRemove();
};
#endif /* defined(__pictomir_cocos2d_x__RobotWinLayer__) */
