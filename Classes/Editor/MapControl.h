//
//  MapControl.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 12.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__MapControl__
#define __pictomir_cocos2d_x__MapControl__

#include <iostream>
#include "RobotPrefs.h"

class MapControlLayer : public cocos2d::CCLayerColor
{
    int sizeHeight;
    bool showed;
    
public:
    
    virtual bool init();
    void showLayer(CCObject * sender);
    void plusLayers (CCObject * sender);
    void minusLayers (CCObject * sender);
    
    void plusTileset (CCObject * sender);
    void minusTileset (CCObject * sender);
    
    void SaveMap (CCObject * sender);
    void CloseMap (CCObject * sender);
    void DeleteMap (CCObject * sender);
    
    static MapControlLayer* create();
    
    static MapControlLayer * instanse;
    static MapControlLayer * getInstanse();
    
    void finishMapLoading();
    
    virtual void draw();
    
    void updateArrow ();
    
private:
    cocos2d::CCMenuItemImage* arrow;
    
    cocos2d::CCLabelTTF* layersCnt;
    cocos2d::CCLabelTTF* tilesetCnt;
    
    RobotPrefsScrolLayer * robotsPrefs;

    int layersCnt_;
    int tilesetCnt_;
};

#endif /* defined(__pictomir_cocos2d_x__MapControl__) */
