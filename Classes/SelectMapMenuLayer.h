//
//  SelectMapMenuLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__SelectMapMenuLayer__
#define __pictomir_cocos2d_x__SelectMapMenuLayer__

#include <iostream>

#include "cocos-ext.h"
#include "PMMapPreview.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SelectMapMenuLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
    
    CREATE_FUNC(SelectMapMenuLayer);
    
    static cocos2d::CCScene* scene();
    
    ~SelectMapMenuLayer()
    {
        removeAllChildrenWithCleanup(true);
        cocos2d::CCTextureCache::sharedTextureCache()->removeAllTextures();
    }
    
private:
    CCScrollView * scroll;
    CCMenu *levelSelectMenu;
    
    void mainMenu(CCObject *sender);
    void worldsMenu(CCObject *sender);
    
    virtual void onEnter();
    virtual void onExit();
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    
#ifdef MAP_EDITOR
    PMMapPreview * dragPreview;
    bool ISdrag;
    int mapNum;
    time_t time;
    CCPoint delta;
    
    PMMapPreview * addMapPreview;
    void addMap (CCObject *sender);
    void uploadMapsToServer (CCObject *sender);
    
    void DeleteWorld (CCObject * sender);
#endif
};

#endif /* defined(__pictomir_cocos2d_x__SelectMapMenuLayer__) */
