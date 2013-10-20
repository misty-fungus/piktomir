//
//  Robot4Edit.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 15.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Robot4Edit__
#define __pictomir_cocos2d_x__Robot4Edit__

#include "Robot4.h"

class Robot4Edit : public cocos2d::CCLayer {
    Robot4* robot_;
    
    int sizeHeight;
    virtual bool init();
    
    void plusEndX (CCObject * sender);
    void minusEndX (CCObject * sender);
    void plusEndY (CCObject * sender);
    void minusEndY (CCObject * sender);
//    void plusEndLayer (CCObject * sender);
//    void minusEndLayer (CCObject * sender);
    void plusEndGrass (CCObject * sender);
    void minusEndGrass (CCObject * sender);
//    void plusEndOven (CCObject * sender);
//    void minusEndOven (CCObject * sender);
    
//    void plusLayer (CCObject * sender);
//    void minusLayer (CCObject * sender);
    
    void showRobotStates (CCObject * sender);
//    void showEndRobotStates (CCObject * sender);
//
//    void setEndDown (CCObject * sender);
//    void setEndUp (CCObject * sender);
//    void setEndLeft (CCObject * sender);
//    void setEndRight (CCObject * sender);
    
    void setDown (CCObject * sender);
    void setUp (CCObject * sender);
    void setLeft (CCObject * sender);
    void setRight (CCObject * sender);
    
//    void setBaseRobot (CCObject * sender);
    
//    void showRobotTypes (CCObject * sender);
//    void changeType (CCObject * sender);
    
//    void deleteRobot (CCObject * sender);
    
    cocos2d::CCMenuItemLabel *stateLable;
//    cocos2d::CCMenuItemLabel *endStateLable;
    
public:
    virtual void draw();
    Robot4Edit() : cocos2d::CCLayer(),
//        layerCnt_ (0),
        state(0),
        endXCnt_ (0),
        endYCnt_ (0),
//        endLayerCnt_ (0),
//        endStateCnt_ (0),
//        layerCnt (NULL),
        endXCnt (NULL),
        endYCnt (NULL),
//        endLayerCnt (NULL),
//        baserobot(false),
        combobox(false),
        endGrassCnt_(0)
    {
    }
    
    CREATE_FUNC(Robot4Edit);
    
//    cocos2d::CCLabelTTF* layerCnt;
    
    cocos2d::CCLabelTTF* endXCnt;
    cocos2d::CCLabelTTF* endYCnt;
//    cocos2d::CCLabelTTF* endLayerCnt;
    
    cocos2d::CCLabelTTF* paintGrassCnt;
//    cocos2d::CCLabelTTF* ovenBurnCnt;
    
    cocos2d::CCLayerColor * types;
    
//    cocos2d::CCMenuItemImage * baseRobotCheckBox;
    
    void setRobot(Robot4* robot);
    
//    int layerCnt_;
    
    int endXCnt_;
    int endYCnt_;
//    int endLayerCnt_;
//    int endStateCnt_;
    int endGrassCnt_;
//    int endOvenCnt_;
    
//    bool baserobot;
    int state;
    
    bool combobox;
    
    void onShowCombobox(CCObject* sender);
    void onEndCombobox(CCObject* sender);
    cocos2d::CCMenu * menu;
};

#endif /* defined(__pictomir_cocos2d_x__Robot4Edit__) */
