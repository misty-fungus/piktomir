//
//  ScrollLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 10.04.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__ScrollLayer__
#define __pictomir_cocos2d_x__ScrollLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Map.h"

const double SCROLLLAYER_TOUCH_OFFSET = 200.0;
const double SCROLLLAYER_ACTIVELAYER_RANGE = 50.0;
const double SCROLLLAYER_ACTIVELAYER_DELTA = 3.0;

class ScrollLayer : public cocos2d::CCLayer
{
    enum TouchType {
        DragLayer,
        ScrollLayers,
        EditLayer,
        DragRobot,
        ResizeMap
    };
    
public:
    
    ScrollLayer() :
        cocos2d::CCLayer(),
        layer(NULL)
    {
        init();
    }
    ScrollLayer(Map *layer,cocos2d::CCSize size) :
        cocos2d::CCLayer(),
        layer(layer)
    {
        init();
        setContentSize(size);
        //setViewSize(size);
    }
    
    ~ScrollLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
    static ScrollLayer* create()
    {
        ScrollLayer *layer = new ScrollLayer();
        layer->autorelease();
        return layer;
    }
    
    static ScrollLayer* create(Map *layer,cocos2d::CCSize size)
    {
        ScrollLayer *ret = new ScrollLayer(layer,size);
        ret->autorelease();
        return ret;
    }

    virtual bool init();
    
    void removeBackground()
    {
        parallax->removeChildByTag(0, true);
    }
    
protected:
    
    SYNTHESIZE(cocos2d::CCParallaxNode *, parallax,Parallax);
    SYNTHESIZE(bool, active, Active);
private:
    Map *layer;
    cocos2d::CCPoint startPoint;
    cocos2d::CCPoint startLayerPosition;
    cocos2d::CCSize  startSize;
    TouchType touchType;
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
   
};



#endif /* defined(__pictomir_cocos2d_x__ScrollLayer__) */
