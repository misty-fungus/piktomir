//
//  SpeedBarLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__SpeedBarLayer__
#define __pictomir_cocos2d_x__SpeedBarLayer__

#include <iostream>
#include "GameParams.h"

class SpeedBarLayer : public cocos2d::CCLayer
{
public:
    
    virtual bool init();
    
    CREATE_FUNC(SpeedBarLayer);
    
    void setEnabled(bool flag);
    
    bool isEnabled()
    {
        return enabled;
    }
    
    ~SpeedBarLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
private:
    cocos2d::CCSprite *speed;
    bool enabled;
    
    void fast(CCObject *sender);
    void slow(CCObject *sender);
    
    
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    
};

#endif /* defined(__pictomir_cocos2d_x__SpeedBarLayer__) */
