//
//  RobotLooseLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__RobotLooseLayer__
#define __pictomir_cocos2d_x__RobotLooseLayer__

#include <iostream>
#include "cocos2d.h"

class GameLayer;

class RobotLooseLayer : public cocos2d::CCLayerColor
{
private:
    GameLayer *parent;
    
    void mainMenu(CCObject *sender);
    void nextLevel(CCObject *sender);
public:
    
    RobotLooseLayer():
        cocos2d::CCLayerColor(),
        parent(NULL)
    {}
    
    RobotLooseLayer(GameLayer *parent):
        cocos2d::CCLayerColor(),
        parent(parent)
    {}
    
    virtual bool init();
    
    CREATE_FUNC(RobotLooseLayer);
    
    static RobotLooseLayer *create(GameLayer *parent);
    
    void show();
    void hide();
    
    void selfRemove();
    
    ~RobotLooseLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
};

#endif /* defined(__pictomir_cocos2d_x__RobotLooseLayer__) */
