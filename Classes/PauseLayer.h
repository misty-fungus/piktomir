//
//  PauseLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__PauseLayer__
#define __pictomir_cocos2d_x__PauseLayer__

#include <iostream>
#include "cocos2d.h"

class GameLayer;

class PauseLayer : public cocos2d::CCLayerColor
{
public:
    
    PauseLayer():
        cocos2d::CCLayerColor(),
        parent(NULL),
        isRobotLoose(false)
    {}
    
    PauseLayer(GameLayer *parent,bool isRobotLoose):
        cocos2d::CCLayerColor(),
        parent(parent),
        isRobotLoose(isRobotLoose)
    {}
    
    virtual bool init();
    
    CREATE_FUNC(PauseLayer);
    
    static PauseLayer *create(GameLayer *parent,bool isRobotLoose);
    
    void show();
    void hide();
    
    void selfRemove();
    
    ~PauseLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
private:
    GameLayer *parent;
    bool isRobotLoose;
    
    void mainMenu(CCObject *sender);
    void nextLevel(CCObject *sender);
    void exitGame(CCObject *sender);
};



#endif /* defined(__pictomir_cocos2d_x__PauseLayer__) */
