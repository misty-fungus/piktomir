//
//  ProgramLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 30.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__MethodLayer__
#define __pictomir_cocos2d_x__MethodLayer__

#include <iostream>
#include <vector>
#include <libxml/xmlreader.h>
#include "cocos2d.h"
#include "Robot4.h"
#include "CCGL.h"
#include "Hint.h"
#include "HintLayer.h"
#include "SelectButton.h"

class ProgramLayer;

const time_t TOUCH_TIME = 200;

const int NAME_LABEL_TAG = 0;
const int NAME_SPRITE_TAG = 1;
const int INFO_BUTTON_TAG = 2;
const int REPEATER_BUTTON_TAG = 3;
const int CONDITION_BUTTON_TAG = 4;
const int REPEATER_SPRITE_TAG = 5;
const int CONDITION_SPRITE_TAG = 6;

const float BUTTON_TILE_SIZE = 40.0f;
const double METHODLAYER_TILE_OFFSET = 10.0;
const double BORDER_RADIUS = 10.0;

class MethodLayer : public cocos2d::CCLayerColor
{

    SYNTHESIZE(int, index, Index);
    SYNTHESIZE(ProgramLayer *, parent, Parent);
    
    SYNTHESIZE(int, methodID,MethodID);
    SYNTHESIZE(int, methodID_Exec,MethodID_Exec);
    
    SYNTHESIZE(int, height, Height);
    SYNTHESIZE(int, width, Width);
    
    SYNTHESIZE(int, condition,Condition);
    SYNTHESIZE(int, repeater, Repeater);
    SYNTHESIZE(bool, useCondition, UseCondition);
    SYNTHESIZE(bool, useRepeater, UseRepeater);

    SYNTHESIZE(Hint *,hint,Hint);
    
    SYNTHESIZE(std::string, name, Name);
    
    
    SYNTHESIZE(bool, hidden, Hidden);
    SYNTHESIZE(std::string, givenText, GivenText);
    SYNTHESIZE(std::string, doText, DoText);
    
    
    SYNTHESIZE(bool, resizable, Resizable);
    SYNTHESIZE(int, maxHeight, MaxHeight);
    SYNTHESIZE(int, minHeight, MinHeight);
    
    SYNTHESIZE(std::vector<int>, methods, Methods);
    SYNTHESIZE(std::vector<int>, backGround, BackGround);
    
private:
    enum TouchType
    {
        mlDrag,
        mlResize,
        mlSwitchRepeater,
        mlSwitchCondition,
        mlSwitchLock
    };
    
    bool enabled;
    
    float realWidth;
    float realHeight;
    
    cocos2d::CCLayer *backGroundLayer;
    cocos2d::CCLayer *methodLayer;
    
    float nameLength;

    
    bool hintShowed;
#ifndef MAP_EDITOR
    HintLayer *hintLayer;
#else
    HintEditLayer * hintLayer;
#endif
    
    cocos2d::CCMenuItemSprite *pushProgramButton;
    //for drag'n'drop
    cocos2d::CCSprite *draggedSprite;
    int draggedMethod;
    
    TouchType touchType;
    
    cocos2d::CCPoint startPoint;
    
    time_t draggingTime;
    
    cocos2d::CCMenuItemSprite *showHintButton;
    
public:
    
    enum BackgroundType
    {
        pmNone,
        pmNormal,
        pmBlocked
    };
    
    MethodLayer():
        cocos2d::CCLayerColor(),
        index(-1),
        parent(NULL),
        methodID(EMPTY_METHOD),
        methodID_Exec(EMPTY_METHOD),
        height(0),
        width(0),
        condition(EMPTY_CONDITION),
        repeater(1),
        useCondition(false),
        useRepeater(false),
        hint(NULL),
        enabled(true),
        methods(std::vector<int> ()),
        backGround(std::vector<int> ()),
        resizable(false),
        maxHeight(0),
        minHeight(0),
        realWidth(0),
        realHeight(0),
        backGroundLayer(NULL),
        methodLayer(NULL),
        name(std::string()),
        nameLength(0.0f),
        hidden(false),
        givenText(std::string()),
        doText(std::string()),
        hintShowed(false),
        hintLayer(NULL),
        pushProgramButton(NULL),
        draggedSprite(NULL),
        draggedMethod(-1),
        touchType(mlDrag),
        startPoint(cocos2d::CCPointZero),
        draggingTime(0),
        showHintButton(NULL)
#ifdef MAP_EDITOR
    ,
    deleteMenu(NULL)
#endif
    {}
    
    CREATE_FUNC(MethodLayer);
    
    MethodLayer(ProgramLayer *parent):
        cocos2d::CCLayerColor(),
        index(-1),
        parent(parent),
        methodID(EMPTY_METHOD),
        methodID_Exec(EMPTY_METHOD),
        height(0),
        width(0),
        condition(EMPTY_CONDITION),
        repeater(1),
        useCondition(false),
        useRepeater(false),
        hint(NULL),
        enabled(true),
        methods(std::vector<int> ()),
        backGround(std::vector<int> ()),
        resizable(false),
        maxHeight(0),
        minHeight(0),
        realWidth(0),
        realHeight(0),
        backGroundLayer(NULL),
        methodLayer(NULL),
        name(std::string()),
        nameLength(0.0f),
        hidden(false),
        givenText(std::string()),
        doText(std::string()),
        hintShowed(false),
        hintLayer(NULL),
        pushProgramButton(NULL),
        draggedSprite(NULL),
        draggedMethod(-1),
        touchType(mlDrag),
        startPoint(cocos2d::CCPointZero),
        draggingTime(0),
        showHintButton(NULL)
#ifdef MAP_EDITOR
        ,
        deleteMenu(NULL)
#endif
    {}
    
    ~MethodLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
    virtual bool init();
    
    friend class XmlWorldReader;
    friend class XMLWorldSaver;
    
    static MethodLayer* create(ProgramLayer *parent)
    {
        MethodLayer *layer = new MethodLayer(parent);
        
        layer->init();
        layer->autorelease();
        
        return layer;
    }
    
    cocos2d::CCSprite* resizeSprite;
    
    std::vector<_instruction> makeMethod();
    std::vector<_instruction> makeExecMethod();
    
    virtual void draw();
    
    void setButton(SelectButton *actionButton,cocos2d::CCTouch *touch);
    
    void highlightMethodBreak(int method);
    void highlightMethod(int method);
    void clearHighlightMethod(int method);
    
    void setControlledMode(bool flag);
    
    void addProgram(CCObject *sender);
    
    void drawFunctions();
    
    bool containsTouch(cocos2d::CCTouch *touch)
    {
        return backGroundLayer->boundingBox().containsPoint(this->convertTouchToNodeSpace(touch));
    }
    
    bool getEnabled()
    {
        return enabled;
    }
    
    void setEnabled(bool flag, bool button = true)
    {
        enabled = flag;
        
        if(button)
            pushProgramButton->setEnabled(flag);
        
        if(showHintButton != NULL)
            showHintButton->setEnabled(flag);
        
#ifdef MAP_EDITOR
        if(deleteMenu != NULL)
        {
            deleteMenu->setEnabled(flag);
            for (int i = 0; i < deleteMenu->getChildren()->count(); ++i) {
                ((cocos2d::CCMenuItem*)deleteMenu->getChildren()->objectAtIndex(i))->setEnabled(flag);
            }
        }
#endif
    }
    
private:    
    void addAction(int methodIndex,int methodID, bool byProgramLayer = false);
    void addCondition(int newCondition);
    void addRepeater(int repeater);
    
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    
    int methodAtTouch(cocos2d::CCTouch *touch);
    int lockAtTouch(cocos2d::CCTouch *touch);
    bool conditionAtTouch(cocos2d::CCTouch *touch);
    bool repeaterAtTouch(cocos2d::CCTouch *touch);
    
    void resize(int sgn);
    void resizeWidth(int sgn);
    
    void showHint(CCObject *sender);
    void drawHint();
    
#ifdef MAP_EDITOR
    int LockAtTouch;
    cocos2d::CCMenu * deleteMenu;
    void deleteMethod(CCObject* sender);
    void moveUp();
    void makeHint(CCObject* sender);
    
    std::vector<int> lastBackground;
    std::vector<int> newHint;
    
    void readyForSave();
#endif
};

#endif /* defined(__pictomir_cocos2d_x__ProgramLayer__) */
