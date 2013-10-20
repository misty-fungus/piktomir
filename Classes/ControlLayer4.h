//
//  ControlLayer4.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/2/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__ControlLayer4__
#define __pictomir_cocos2d_x__ControlLayer4__

#include <iostream>
#include "cocos2d.h"
#include "RobotManager.h"


class ControlLayer4 : public cocos2d::CCLayer
{
public:
    ControlLayer4():
        cocos2d::CCLayer(),
        manager(NULL)
    {}
    
    ControlLayer4(RobotManager *manager):
        cocos2d::CCLayer(),
        manager(manager)
    {
    }
    
    CREATE_FUNC(ControlLayer4);
    
    static ControlLayer4* create(RobotManager *manager)
    {
        ControlLayer4 *layer = new ControlLayer4(manager);
        
        layer->init();
        layer->autorelease();
        
        return layer;
    }
    
    bool init();
    
    void setStepCount(int stepCount);
    
    void setEnabled(bool flag);
    
protected:
    cocos2d::CCMenu *menu;
    SYNTHESIZE(RobotManager *,manager,Manager);
    cocos2d::CCLabelTTF *labelCount;
};


#endif /* defined(__pictomir_cocos2d_x__ControlLayer4__) */
