//
//  ProgramLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 30.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#include "ProgramLayer.h"
#include "GameLayer.h"
#include "GameParams.h"
#include "cocos-ext.h"

#ifdef MAP_EDITOR
#include "MapEditorGameLayer.h"
#endif

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

bool ProgramLayer::init()
{
    if ( !CCLayerColor::init() )
    {
        return false;
    }

    
    activeButton = NULL;
    draggedSprite = NULL;
    //setTouchEnabled(true);
    
    GameParams *params = GameParams::getInstanse();
    
    scroll  = CCScrollView::create();
    scroll->setDirection(kCCScrollViewDirectionVertical);
    scroll->setZoomScale(0.5);
    scroll->setClippingToBounds(true);
    scroll->setBounceable(true);
    
    initWithColor(ccc4(100,100,100,255), params->programLayerWidth , params->screenSize.height);
    
    setOpacity(255);
    setPosition( ccp(params->screenSize.width - params->programLayerWidth, 0) );
    
    float lastY = params->screenSize.height - PROGRAMLAYER_BORDER_Y;
        
    actionMenu = CCMenu::create();
    
#ifdef MAP_EDITOR
    map4EditorsMenu * menu = map4EditorsMenu::getInstanse();
    menu->getEditMenu()->setPosition( ccp (PROGRAMLAYER_BORDER_X+25, lastY) );
    menu->getEditMenu()->setAnchorPoint(CCPointZero);
    
    addChild(menu->getEditMenu(),10);
    
    lastY-=180;
#endif
    
    actionMenu->setPosition( ccp (PROGRAMLAYER_BORDER_X, lastY) );
    addChild(actionMenu,10);
    
    addChild(scroll);

    reset();
    
    return true;
}

void ProgramLayer::drawLayers(UniversalWorld *world)
{
    GameParams *params = GameParams::getInstanse();
    
    for(int i = 1; i < world->getBaseRobot()->getNativeMethodCount(); ++i)
    {
        CCSprite *normalImage = PMTextureCache::instanse()->getRobotMethodSprite(i);
        CCSprite *selectedImage = PMTextureCache::instanse()->getRobotMethodSprite(i);
        CCSprite *disabledImage = PMTextureCache::instanse()->getRobotMethodSprite_Disabled(i);
        
        SelectButton *button =  new SelectButton(SelectButton::Method, i, normalImage,selectedImage, disabledImage,
                                                 this, menu_selector(ProgramLayer::selectAction));
        
        button->setPosition( ccp((i-1) * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET / 2), 0) );
        actionMenu->addChild(button);
        
    }
    // controls
    float lastY = params->screenSize.height - PROGRAMLAYER_BORDER_Y;
    float lastActionY = 0.0f;
    
#ifndef MAP_EDITOR
    
    for(int i = 0; i < layers.size(); ++i)
    {
        if(layers[i]->getUseRepeater())
        {
            lastY -= PROGAMLAYER_MENU_SEPARATOR + BUTTON_TILE_SIZE;
            lastActionY -= PROGAMLAYER_MENU_SEPARATOR + BUTTON_TILE_SIZE;
            
            for(int i = 2; i < REPEATER_BUTTON_NUM; ++i)
            {
                CCSprite *normalImage = PMTextureCache::instanse()->getRepeaterSprite(i);
                CCSprite *selectedImage = PMTextureCache::instanse()->getRepeaterSprite(i);
                CCSprite *disabledImage = PMTextureCache::instanse()->getRepeaterSprite_Disabled(i);
                
                SelectButton *button = new SelectButton(SelectButton::Repeater, i, normalImage, selectedImage, disabledImage,
                                                        this, menu_selector(ProgramLayer::selectAction));
                
                button->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET / 2) * (i-2),lastActionY) );
                actionMenu->addChild(button);
            }
            break;
        }
    }
    
    //Conditions
    for(int i = 0; i <layers.size(); ++i)
    {
        if(layers[i]->getUseCondition())
        {
            lastY -= PROGAMLAYER_MENU_SEPARATOR + BUTTON_TILE_SIZE;
            lastActionY -= PROGAMLAYER_MENU_SEPARATOR + BUTTON_TILE_SIZE;
            
            for(int i = 1; i < world->getBaseRobot()->getConditionsCount() + 1; ++i)
            {
                CCSprite *normalImage = PMTextureCache::instanse()->getRobotConditionSprite(i);
                CCSprite *selectedImage = PMTextureCache::instanse()->getRobotConditionSprite(i);
                CCSprite *disabledImage = PMTextureCache::instanse()->getRobotConditionSprite_Disabled(i);
                
                SelectButton *button = new SelectButton(SelectButton::Condition, i , normalImage, selectedImage, disabledImage,
                                                        this, menu_selector(ProgramLayer::selectAction));
                
                button->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET / 2) * (i - 1),lastActionY) );
                actionMenu->addChild(button);
            }
            break;
        }
    }
  
#else
    
    lastY -= PROGAMLAYER_MENU_SEPARATOR + BUTTON_TILE_SIZE;
    lastActionY -= PROGAMLAYER_MENU_SEPARATOR + BUTTON_TILE_SIZE;
    
    for(int i = 1; i < REPEATER_BUTTON_NUM; ++i)
    {
        CCSprite *normalImage = PMTextureCache::instanse()->getRepeaterSprite(i);
        CCSprite *selectedImage = PMTextureCache::instanse()->getRepeaterSprite(i);
        CCSprite *disabledImage = PMTextureCache::instanse()->getRepeaterSprite_Disabled(i);
        
        SelectButton *button = new SelectButton(SelectButton::Repeater, i, normalImage, selectedImage, disabledImage,
                                                this, menu_selector(ProgramLayer::selectAction));
        
        button->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET / 2) * (i-1),lastActionY) );
        actionMenu->addChild(button);
    }
    lastY -= PROGAMLAYER_MENU_SEPARATOR + BUTTON_TILE_SIZE;
    lastActionY -= PROGAMLAYER_MENU_SEPARATOR + BUTTON_TILE_SIZE;
    
    for(int i = 0; i < world->getBaseRobot()->getConditionsCount() + 1; ++i)
    {
        CCSprite *normalImage = PMTextureCache::instanse()->getRobotConditionSprite(i);
        CCSprite *selectedImage = PMTextureCache::instanse()->getRobotConditionSprite(i);
        CCSprite *disabledImage = PMTextureCache::instanse()->getRobotConditionSprite_Disabled(i);
        
        SelectButton *button = new SelectButton(SelectButton::Condition, i , normalImage, selectedImage, disabledImage,
                                                this, menu_selector(ProgramLayer::selectAction));
        
        button->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET / 2) * (i),lastActionY) );
        actionMenu->addChild(button);
    }

    CCMenuItemLabel * addFunction = CCMenuItemLabel::create(CCLabelTTF::create("добавить подпрограмму", "Marker Felt", 22), this, menu_selector(ProgramLayer::addMethod));
    addFunction->setAnchorPoint(CCPointZero);
    addFunction->setPosition(CCPointZero);
    addFunctionMenu = CCMenu::create(addFunction, NULL);
    addFunctionMenu->setAnchorPoint(CCPointZero);
    scroll->addChild(addFunctionMenu, 10);
    
#endif
    
    for(int i = 0; i < layers.size(); ++i)
        scroll->addChild(layers[i], 10);
    
    update();

    for(int i = 1; i < layers.size(); ++i)
    {
        CCSprite *normalImage = PMTextureCache::instanse()->getProgramSprite(CMDA_EXECUTE - i + 1);
        CCSprite *selectedImage = PMTextureCache::instanse()->getProgramSprite(CMDA_EXECUTE - i + 1);
        CCSprite *disabledImage = PMTextureCache::instanse()->getProgramSprite_Disabled(CMDA_EXECUTE - i + 1);
        
        SelectButton *button = new SelectButton(SelectButton::Method ,CMDA_EXECUTE - i + 1, normalImage, selectedImage, disabledImage,
                                                this, menu_selector(ProgramLayer::selectAction));
        
        
        button->setPosition(ccp((i - 2 + world->getBaseRobot()->getNativeMethodCount()) * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET / 2), 0));
        actionMenu->addChild(button);
    }
}

void ProgramLayer::update()
{
    GameParams *params = GameParams::getInstanse();
    
    float lastY = params->screenSize.height - PROGRAMLAYER_BORDER_Y;
    for(int i = 0; i < layers.size(); ++i)
    {
        if(layers[i]->getUseRepeater())
        {
            lastY -= PROGAMLAYER_MENU_SEPARATOR + BUTTON_TILE_SIZE;
            break;
        }
    }
    //Conditions
    for(int i = 0; i < layers.size(); ++i)
    {
        if(layers[i]->getUseCondition())
        {
            lastY -= PROGAMLAYER_MENU_SEPARATOR + BUTTON_TILE_SIZE;
            break;
        }
    }
    
#ifdef MAP_EDITOR
    lastY = params->screenSize.height - PROGRAMLAYER_BORDER_Y - PROGAMLAYER_MENU_SEPARATOR - BUTTON_TILE_SIZE - PROGAMLAYER_MENU_SEPARATOR - BUTTON_TILE_SIZE;
    lastY -= 180;
#endif
    
    lastY -= 4 * PROGRAMLAYER_PROGRAM_SEPARATOR;
    
    float viewHeight = 0;
    float viewWidth = 0;
    
    float scrollLastY = 0;
    
    for (int i = 0; i < layers.size(); ++i)
    {
        viewHeight += layers[i]->getContentSize().height + PROGRAMLAYER_PROGRAM_SEPARATOR;
        viewWidth = MAX(layers[i]->getContentSize().width, viewWidth);
    }
    
    scroll->setViewSize( CCSizeMake(params->programLayerWidth,lastY) );
    
    scrollLastY = viewHeight;
    
    for (int i = 0; i < layers.size(); ++i)
    {
        layers[i]->setPosition( ccp(25,scrollLastY - layers[i]->getContentSize().height ) );
        lastY -= layers[i]->getContentSize().height + PROGRAMLAYER_PROGRAM_SEPARATOR;
        scrollLastY -= layers[i]->getContentSize().height + PROGRAMLAYER_PROGRAM_SEPARATOR;
        
    }
    
#ifdef MAP_EDITOR
    if(addFunctionMenu)addFunctionMenu->setPosition(ccp(25,scrollLastY - 30 ));
#endif

    scroll->setPosition( ccp(0, 0) );
    scroll->setContentSize( CCSizeMake(params->programLayerWidth,viewHeight ) );
    scroll->setContentOffset(ccp(0,lastY),false);
}

void ProgramLayer::findButtonAtTouch(CCTouch *touch)
{
    for(int j = 0; j < actionMenu->getChildrenCount(); ++j)
    {
        SelectButton *button  = (SelectButton*)actionMenu->getChildren()->objectAtIndex(j);

        if(button->boundingBox().containsPoint(actionMenu->convertTouchToNodeSpace(touch)))
        {
            draggedSprite = button->cloneSprite();
            draggedButton = button;
            
            return;
        }
        
    }
    
    draggedSprite = NULL;
    draggedButton = NULL;
}

int ProgramLayer::layerAtTouch(cocos2d::CCTouch *touch)
{
    for(int i = 0; i < layers.size(); ++i) {
        if(layers[i]->containsTouch(touch)) {
            return i;
        }
    }
    return -1;
}

void ProgramLayer::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
    CCLayerColor::onEnter();
}

void ProgramLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

bool ProgramLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if(!enabled)
        return false;
    
    draggingTime = getTimeMs();
    
    findButtonAtTouch(touch);

    if( touch && draggedSprite != NULL )
    {
        draggedButton->select();
        
        if(!controlledMode)
        {
            addChild(draggedSprite,20);
            draggedSprite->setPosition( ccp(-100,-100) );
        }
        
        return true;
    }
    else
        return false;
    
}


void ProgramLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if(controlledMode)
        return;
    
    if( touch )
        draggedSprite->setPosition(this->convertTouchToNodeSpace(touch));
    
    CCLayer::ccTouchMoved(touch, pEvent);
}

void ProgramLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if( touch )
    {        
        if(getTimeMs() - draggingTime < TOUCH_TIME || controlledMode)
        {
            selectAction(draggedButton);
            
            if(controlledMode)
            {
                parent->getMethodStackLayer()->addMethod(draggedButton);
                activeButton->jumpOnce();
            }
            
        }
        else
        {
            int layer = layerAtTouch(touch);
            
            if(layer != -1)
            {
                layers[layer]->setButton(draggedButton, touch);
            }

        }
        
        if(!controlledMode)
        {
            removeChild(draggedSprite, true);
        }
        draggedButton->deselect();
        
        draggedButton = NULL;
        draggedSprite = NULL;
        
    }
    CCLayer::ccTouchEnded(touch, pEvent);
}

void ProgramLayer::selectAction(CCObject *sender)
{
    SelectButton *senderButton = (SelectButton *)sender;
    
    if(isAnimationRunnnig)
    {
        isAnimationRunnnig = false;
        activeButton->stopAnimation();
    }
    
    if(senderButton->isMethod())
    {
        if(activeButton == NULL || activeButton->getMethodID() != senderButton->getMethodID())
        {
            activeButton = senderButton;
            
            if(!controlledMode)
            {
                isAnimationRunnnig = true;
                activeButton->startAnimation();
            }
        }
        else if(!controlledMode)
        {
            activeButton = NULL;
        }
    }
    else if(senderButton->isCondition() && !controlledMode)
    {
        if(activeButton == NULL || activeButton->getCondition() != senderButton->getCondition())
        {
            activeButton = senderButton;
            
            if(!controlledMode)
            {
                isAnimationRunnnig = true;
                activeButton->startAnimation();
            }
        }
        else if(!controlledMode)
        {
            activeButton = NULL;
        }
        
    }
    else if(senderButton->isRepeater())
    {
        if(activeButton == NULL || activeButton->getRepeater() != senderButton->getRepeater())
        {

            activeButton = senderButton;
            
            if(!controlledMode)
            {
                isAnimationRunnnig = true;
                activeButton->startAnimation();
            }
            
        }
        else if(!controlledMode)
        {
            activeButton = NULL;
        }
        
    }
}

void ProgramLayer::setControlledMode(bool flag)
{
    controlledMode = flag;
    
    for(int i = 0; i < layers.size(); ++i)
    {
        layers[i]->setControlledMode(flag);
    }
}

void ProgramLayer::setEnabled(bool state)
{
    enabled = state;
    
    for(int i = 0; i< actionMenu->getChildrenCount(); ++i)
    {
        ((CCMenuItem *)actionMenu->getChildren()->objectAtIndex(i))->setEnabled(state);
    }
    
#ifdef MAP_EDITOR
    map4EditorsMenu::getInstanse()->setEnabled(state);
    
    ((CCMenuItem*)addFunctionMenu->getChildren()->objectAtIndex(0))->setEnabled(state);
#endif
    
    for(int i = 0; i< layers.size(); ++i)
    {
        layers[i]->setEnabled(state);
    }
    
    if(isAnimationRunnnig)
    {
        if(!state)
            activeButton->stopAnimation();
        else
            activeButton->startAnimation();
    }
}

void ProgramLayer::reset()
{
    if(activeButton != NULL && isAnimationRunnnig)
        activeButton ->stopAnimation();
    
    isAnimationRunnnig = false;
    activeButton = NULL;
}

std::vector<_instruction> ProgramLayer::getMainMethod()
{
    MethodLayer *layer = layers[0];
    
    return RobotMethod::makeExecuteMethod(CMDMAIN, layer->getUseRepeater(), layer->getRepeater(), layer->getUseCondition(), layer->getCondition());
}

#ifdef MAP_EDITOR

void ProgramLayer::addMethod (CCObject * sender)
{
    if(layers.size() > 3) return;
    MethodLayer * methodLayer = MethodLayer::create(this);
    methodLayer->setIndex(layers.size());
    
    switch (layers.size()) {
        case 1:
            methodLayer->setMethodID(CMDA);
            methodLayer->setMethodID_Exec(CMDA_EXECUTE);
            methodLayer->setName("Command 1");
            break;
        case 2:
            methodLayer->setMethodID(CMDB);
            methodLayer->setMethodID_Exec(CMDB_EXECUTE);
            methodLayer->setName("Command 2");
            break;
        case 3:
            methodLayer->setMethodID(CMDC);
            methodLayer->setMethodID_Exec(CMDC_EXECUTE);
            methodLayer->setName("Command 3");
            break;
    }
    
    layers.push_back(methodLayer);
    
    methodLayer->setRepeater(1);
    methodLayer->setCondition(1);
    methodLayer->setUseRepeater(true);
    methodLayer->setUseCondition(true);
    methodLayer->setWidth(2);
    methodLayer->setHeight(1);
    
    methodLayer->setMethods (std::vector<int>(2,0));
    methodLayer->setBackGround (std::vector<int>(2,1));
    
    methodLayer->setHidden(false);
    
    methodLayer->setResizable(false);
    methodLayer->setMaxHeight(1);
    methodLayer->setMinHeight(1);
    
    methodLayer->drawFunctions();
    
    getScroll()->addChild(methodLayer, 10);
    
    update();
}

#endif


