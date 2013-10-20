//
//  MethodStackLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/19/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__MethodStackLayer__
#define __pictomir_cocos2d_x__MethodStackLayer__

#include <iostream>
#include "cocos2d.h"

#include "RobotManager.h"
#include "SelectButton.h"
#include "MethodScrollLayer.h"

class GameLayer;

class MethodStackLayer : public cocos2d::CCLayer
{
public:
    
    MethodStackLayer():
        cocos2d::CCLayer(),
        parent(NULL),
        robotManager(NULL)
    {}
    
    MethodStackLayer(GameLayer *parent,RobotManager *robotManager):
        cocos2d::CCLayer(),
        parent(parent),
        robotManager(robotManager)
    {}
    
    ~MethodStackLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
    virtual bool init();
    
    CREATE_FUNC(MethodStackLayer);
    
    static MethodStackLayer *create(GameLayer *parent, RobotManager *robotManager);
    
    void show(CCObject *sender);
    void hide(CCObject *sender);
    
    void setEnabled(bool flag);
    
    void addMethod(SelectButton *button);
    
    bool isLastButtonRepeater()
    {
        return lastButtonRepeater;
    }
    
    void clear(CCObject* object);
    void clear();
    
    bool working ()
    {
        return hideButton->isVisible();
    }
    
    std::vector<int> getProgram();
    
    void saveStates()
    {
        showHideState = showButton->isEnabled();
        clearState = clearButton->isEnabled();
        scrollState = scroll->getActive();
    }

    void restoreStates()
    {
        showButton->setEnabled(showHideState);
        hideButton->setEnabled(showHideState);
        clearButton->setEnabled(clearState);
        scroll->setActive(scrollState);
    }
    
private:
    
    GameLayer *parent;
    RobotManager *robotManager;
    
    cocos2d::CCMenuItemSprite* showButton;
    cocos2d::CCMenuItemSprite* hideButton;
    cocos2d::CCMenuItemSprite* clearButton;
    
    int stackCapasity;
    
    cocos2d::CCPoint topPoint;
    cocos2d::CCPoint curPoint;
    
    std::stack<_instruction> methodStack;
    std::stack<cocos2d::CCSprite *> spriteStack;
    
    bool lastButtonRepeater;
    SYNTHESIZE(cocos2d::CCSprite *, lastSprite, LastSprite);
    SelectButton *lastButton;
    MethodScrollLayer *scroll;
    
    bool showHideState;
    bool clearState;
    bool scrollState;
    
    void toggleIcon();
    
    void showEnded();
    void hideEnded();
    
    void reorderSprite();
    void endAddMethod();
    void startAddMethod();
    
    void removeMethod();
};

#endif /* defined(__pictomir_cocos2d_x__MethodStackLayer__) */
