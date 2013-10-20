//
//  MethodScrollLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/19/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__MethodScrollLayer__
#define __pictomir_cocos2d_x__MethodScrollLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameParams.h"

class MethodStackLayer;

class MethodScrollLayer : public cocos2d::extension::CCScrollView
{
public:
    
    MethodScrollLayer():
        cocos2d::extension::CCScrollView(),
        active(false),
        parent(NULL),
        dragStarted(false)
    {}
    
    MethodScrollLayer(MethodStackLayer *parent):
        cocos2d::extension::CCScrollView(),
        active(false),
        parent(parent),
        dragStarted(false)
    {}
        
    virtual bool init();
    
    CREATE_FUNC(MethodScrollLayer);
    
    static MethodScrollLayer *create(MethodStackLayer *parent);
    
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    
    ~MethodScrollLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
private:
    SYNTHESIZE(bool, active, Active);
    
    MethodStackLayer *parent;
    bool dragStarted;
    cocos2d::CCPoint startPoint;
    bool removed;
    

};

#endif /* defined(__pictomir_cocos2d_x__MethodScrollLayer__) */
