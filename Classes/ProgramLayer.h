//
//  ProgramLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 30.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__ProgramLayer__
#define __pictomir_cocos2d_x__ProgramLayer__

#include <iostream>
#include "MethodLayer.h"
#include "SelectButton.h"
#include <libxml/xmlreader.h>
#include "cocos-ext.h"

class GameLayer;

const float PROGAMLAYER_MENU_SEPARATOR = 20.0f;

const float PROGRAMLAYER_BORDER_X = 45.0f;
const float PROGRAMLAYER_BORDER_Y = 45.0f;

const float PROGRAMLAYER_PROGRAM_SEPARATOR = 10.0f;

class ProgramLayer : public cocos2d::CCLayerColor
{
    SYNTHESIZE(GameLayer *,parent,Parent);
    SYNTHESIZE(SelectButton *, activeButton, ActiveButton);
    SYNTHESIZE(cocos2d::CCMenu *, actionMenu, SelectMenu);
    SYNTHESIZE(cocos2d::extension::CCScrollView *, scroll, Scroll);

protected:
    
    bool isAnimationRunnnig;
    bool enabled;
    
    bool controlledMode;
    
    cocos2d::CCSprite *draggedSprite;
    SelectButton *draggedButton;
    time_t draggingTime;
    
public:
    
    std::vector<MethodLayer *> layers;
    
public:
    ProgramLayer() :
        cocos2d::CCLayerColor(),
        parent(NULL),
        activeButton(NULL),
        actionMenu(NULL),
        scroll(NULL),
        isAnimationRunnnig(false),
        enabled(true),
        controlledMode(false),
        draggedSprite(NULL),
        draggedButton(NULL),
        draggingTime(0)
    {}
    
    static ProgramLayer* create()
    {
        return new ProgramLayer();
    }
    
    ProgramLayer(GameLayer *parent) :
        cocos2d::CCLayerColor(),
        parent(parent),
        activeButton(NULL),
        actionMenu(NULL),
        scroll(NULL),
        isAnimationRunnnig(false),
        enabled(true),
        controlledMode(false),
        draggedSprite(NULL),
        draggedButton(NULL),
        draggingTime(0)
    {}
    
    static ProgramLayer* create(GameLayer *parent)
    {
        ProgramLayer *programLayer = new ProgramLayer(parent);
        
        programLayer->init();
        programLayer->autorelease();
        
        return programLayer;
    }
    
    ~ProgramLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
    virtual bool init();
    
    void setEnabled(bool state);
    
    bool isEnabled()
    {
        return enabled;
    }
    
    void reset();
    
    std::vector<_instruction> getMainMethod();

    void setControlledMode(bool flag);
    
    void drawLayers(UniversalWorld *world);
    
    void update();
private:
    
    void selectAction(CCObject *sender);
    void addAction(CCObject *sender);
    void addCondition(CCObject *sender);
    void addRepeater(CCObject *sender);
    
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    
    void findButtonAtTouch(cocos2d::CCTouch *touch);
    int layerAtTouch(cocos2d::CCTouch *touch);
    
    virtual void onEnter();
    virtual void onExit();

    
#ifdef MAP_EDITOR
    cocos2d::CCMenu * addFunctionMenu;
    void addMethod (CCObject * sender);
    
#endif
};

#endif /* defined(__pictomir_cocos2d_x__ProgramLayer__) */
