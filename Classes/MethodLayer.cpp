//
//  ProgramLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 30.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#include "MethodLayer.h"
#include <zlib.h>
#include "utils.h"
#include <string.h>
#include "ProgramLayer.h"
#include "GameParams.h"
#include "CCGL.h"
#include "PMTextureCache.h"
#include "GameParams.h"
#include "MethodStackLayer.h"
#include "GameLayer.h"

USING_NS_CC;
using namespace std;


bool MethodLayer::init()
{
    if ( !initWithColor(ccc4(255,0,0,255)) )
    {
        return false;
    }
    
    setTouchPriority(-129);
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    
    draggedSprite = NULL;
    draggedMethod = -1;
    hint = NULL;
    hintLayer = NULL;
    
    
    return true;
}

void MethodLayer::drawFunctions()
{
#ifdef MAP_EDITOR
    useRepeater = true;
    useCondition = true;
    resizable = true;
    
    newHint = std::vector<int>(0);
    
    if(lastBackground.size()!=backGround.size())lastBackground = backGround;
#endif
    
    if(hidden)
    {
        string givenLabelText = CCLocalizedString("GivenText", "Not localized");
        string doLabelText = CCLocalizedString("DoText", "Not localized");
        
        givenLabelText += givenText;
        doLabelText += doText;
        
        CCLabelTTF* givenLabel = CCLabelTTF::create(givenLabelText.c_str(), "Arial", 18, CCSizeZero,kCCTextAlignmentLeft);
        CCLabelTTF* doLabel = CCLabelTTF::create(doLabelText.c_str(), "Arial", 18, CCSizeZero,kCCTextAlignmentLeft);
        
        realWidth = MAX(givenLabel->getContentSize().width, doLabel->getContentSize().width);
        realHeight = givenLabel->getContentSize().height + doLabel->getContentSize().height + 20;
        
        givenLabel->setPosition( ccp(0,doLabel->getContentSize().height + 20 ) );
        givenLabel->setAnchorPoint( ccp(0,0) );
        
        doLabel->setPosition( ccp(0,10) );
        doLabel->setAnchorPoint( ccp(0,0) );
        
        addChild(givenLabel);
        addChild(doLabel);

    }
    else
    {
        int useCorR = (useRepeater || useCondition) ? 1 : 0;
        int useCAndR = (useRepeater && useCondition) ? 1 : 0;
        int addWidth = (useCAndR && width < 2)? 1 : 0;
        
        
        realWidth = width * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) + addWidth * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) - METHODLAYER_TILE_OFFSET;
        realHeight = height * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) + useCorR * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET);
    }
    
    string display_name;
    CCSprite *nameSprite = NULL;
    
    if(methodID == CMDMAIN){
        display_name = CCLocalizedString("MainProgram", "NotLocalized");
    } else if(methodID == CMDA) {
        display_name = CCLocalizedString("SubProgram", "NotLocalized");
        nameSprite = PMTextureCache::instanse()->getProgramASprite();
    } else if(methodID == CMDB) {
        display_name = CCLocalizedString("SubProgram", "NotLocalized");
        nameSprite = PMTextureCache::instanse()->getProgramBSprite();
    } else if(methodID == CMDC) {
        display_name = CCLocalizedString("SubProgram", "NotLocalized");
        nameSprite = PMTextureCache::instanse()->getProgramCSprite();
    }
    
    CCLabelTTF* nameLabel = CCLabelTTF::create(display_name.c_str(), "Arial", 18, CCSizeZero,kCCTextAlignmentLeft);
    
    
    nameLabel->setPosition( ccp(-BORDER_RADIUS / 2,realHeight + METHODLAYER_TILE_OFFSET ) );
    nameLabel->setAnchorPoint( ccp(0,0) );
    
    nameLength = nameLabel->getContentSize().width;
    
    if(nameSprite != NULL)
    {
        addChild(nameSprite , 0, NAME_SPRITE_TAG);
        nameSprite->setPosition( ccp(nameLength, realHeight + BORDER_RADIUS ));
        nameSprite->setAnchorPoint( CCPointZero );
        nameSprite->setScale(0.8f);
        nameLength += nameSprite->getContentSize().width;
    }
    
    

    
#ifdef MAP_EDITOR
    
    CCMenuItemImage *saveHintSprite = CCMenuItemImage::create("System/SaveHintIcon.png", "System/SaveHintIcon_Selected.png", "System/SaveHintIcon_Disabled.png", this, menu_selector(MethodLayer::makeHint));
    saveHintSprite->setPosition(ccp(0,-3));
    saveHintSprite->setAnchorPoint(CCPointZero);
    deleteMenu = CCMenu::create(saveHintSprite, NULL);
    deleteMenu->setAnchorPoint(CCPointZero);
    deleteMenu->setPosition(ccp(nameLength + 40, realHeight + BORDER_RADIUS / 2));
    addChild(deleteMenu, 2);
    
    if(name != "main")
    {
        CCMenuItemImage *deleteSprite = CCMenuItemImage::create("System/DeleteIcon.png", "System/DeleteIcon_Selected.png", "System/DeleteIcon_Disabled.png", this, menu_selector(MethodLayer::deleteMethod));
        deleteSprite->setPosition(ccp(40,0));
        deleteSprite->setAnchorPoint(CCPointZero);
        deleteMenu->addChild(deleteSprite);
        deleteMenu->setAnchorPoint(CCPointZero);
        deleteMenu->setPosition(ccp(nameLength + 40, realHeight + BORDER_RADIUS / 2));
    }
#endif

    addChild(nameLabel, 2, NAME_LABEL_TAG);
    
    setContentSize( CCSizeMake(realWidth, realHeight + nameLabel->getContentSize().height + METHODLAYER_TILE_OFFSET) );
    
    if(!hidden)
    {
        backGroundLayer  = CCLayer::create();
        backGroundLayer->setContentSize( CCSizeMake(realWidth,realHeight) );
        backGroundLayer->setPosition( CCPointZero );
    
        methodLayer  = CCLayer::create();
        methodLayer->setContentSize( CCSizeMake(realWidth,realHeight) );
        methodLayer->setPosition( CCPointZero );
        
        addChild(backGroundLayer);
        addChild(methodLayer);
        
        int Y = realHeight;
        int X = 0;
        
        Y -= BUTTON_TILE_SIZE;
        
        if (useRepeater)
        {
            CCSprite *tmp = PMTextureCache::instanse()->getRepeaterBackgroundSprite();
            tmp->setPosition( ccp(X, Y) );
            tmp->setAnchorPoint( CCPointZero );
            
            addChild(tmp, 12, REPEATER_BUTTON_TAG);
            
            if(repeater != 0  && repeater != -1)
            {
                tmp = PMTextureCache::instanse()->getRepeaterSprite(repeater);
            }
            else if (repeater == -1)
            {
                tmp = PMTextureCache::instanse()->getRepeaterSprite_Disabled(1);
            }
            else
            {
                tmp = PMTextureCache::instanse()->getRepeaterBackgroundSprite();
            }
            
            tmp->setPosition( ccp(X, Y) );
            tmp->setAnchorPoint( CCPointZero );
            addChild(tmp, 13, REPEATER_SPRITE_TAG);
            
            X += BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET;
            
            if(!useCondition)
                Y -= (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET);
            
        }
        
        if (useCondition)
        {
            CCSprite *tmp = PMTextureCache::instanse()->getConditionBackgroundSprite();
            tmp->setPosition( ccp(X, Y) );
            tmp->setAnchorPoint( CCPointZero );
            addChild(tmp, 12, CONDITION_BUTTON_TAG);
            
            
            if(condition != EMPTY_CONDITION && condition != -1)
            {
                tmp = PMTextureCache::instanse()->getRobotConditionSprite(condition);
            }
            else if (condition == -1)
            {
                tmp = PMTextureCache::instanse()->getConditionBackgroundSprite_Disabled();
            }
            else
            {
                tmp = PMTextureCache::instanse()->getConditionBackgroundSprite();
            }
            tmp->setPosition( ccp(X, Y) );
            tmp->setAnchorPoint( CCPointZero );
            
            addChild(tmp, 13, CONDITION_SPRITE_TAG);
            
            Y -= (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET);
        }
        
        for (int j = 0; j < width * height; ++j)
        {
            if(backGround[j] != pmNone)
            {
                CCSprite *tmp = PMTextureCache::instanse()->getMethodBackgroundSprite();
                tmp->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) * (j%width) + BUTTON_TILE_SIZE / 2,
                                      Y - (j/width) * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) + BUTTON_TILE_SIZE / 2)  );
                //tmp->setAnchorPoint( CCPointZero );
                
                backGroundLayer->addChild(tmp,12);
                
                if(methods[j] >= EMPTY_METHOD)
                {
                    tmp = PMTextureCache::instanse()->getRobotMethodSprite(methods[j]);
                }
                else if(methods[j] >= CMDC_EXECUTE && methods[j] < EMPTY_METHOD)
                {
                    tmp = PMTextureCache::instanse()->getProgramSprite(methods[j]);
                }
                
                tmp->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) * (j%width), Y - (j/width) * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET))  );
                tmp->setAnchorPoint( CCPointZero );
                
                methodLayer->addChild(tmp, 13);
                
            }
#ifdef MAP_EDITOR
            else
            {
                CCSprite *tmp = PMTextureCache::instanse()->getMethodBackgroundSprite_Disabled();
                tmp->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) * (j%width), Y - (j/width) * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET))  );
                tmp->setAnchorPoint( CCPointZero );
                
                backGroundLayer->addChild(tmp,12);
                
                if(methods[j] >= EMPTY_METHOD)
                {
                    tmp = PMTextureCache::instanse()->getRobotMethodSprite_Disabled(methods[j]);
                }
                else if(methods[j] >= CMDC_EXECUTE && methods[j] < EMPTY_METHOD)
                {
                    tmp = PMTextureCache::instanse()->getProgramSprite(methods[j]);
                }
                
                tmp->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) * (j%width), Y - (j/width) * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET))  );
                tmp->setAnchorPoint( CCPointZero );
                
                methodLayer->addChild(tmp, 13);
            }
#endif
            if(backGround[j] == pmBlocked)
            {
                CCSprite *tmp = PMTextureCache::instanse()->getLockSprite();
                tmp->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) * (j%width) + BUTTON_TILE_SIZE - tmp->getContentSize().width,
                                      Y - (j/width) * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET))  );
                tmp->setAnchorPoint( CCPointZero );
                methodLayer->addChild(tmp, 14);
            }
#ifdef MAP_EDITOR
            else
            {
                CCSprite *tmp = PMTextureCache::instanse()->getUnlockSprite();
                tmp->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) * (j%width) + BUTTON_TILE_SIZE - tmp->getContentSize().width,
                                      Y - (j/width) * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET))  );
                tmp->setAnchorPoint( CCPointZero );
                methodLayer->addChild(tmp, 14);
            }
#endif
            
        }
        
        
        if(resizable)
        {
            resizeSprite = CCSprite::create("System/unchecked.png");
            if(width == 1) resizeSprite->setPosition(ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) + BUTTON_TILE_SIZE -10, -10));
            else resizeSprite->setPosition(ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) * (width-1) + BUTTON_TILE_SIZE -10, -10));
            resizeSprite->setAnchorPoint(CCPointZero);
            addChild(resizeSprite,1);
        }
        
        pushProgramButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getPasteIconSprite(),
                                                                   PMTextureCache::instanse()->getPasteIconSprite_Selected(),
                                                                   PMTextureCache::instanse()->getPasteIconSprite_Disabled(),
                                                                   this, menu_selector(MethodLayer::addProgram));
        
        pushProgramButton->setPosition( ccp(GetGameParams->programLayerWidth,0) );
        pushProgramButton->setAnchorPoint( CCPointZero );
        
        CCMenu *menu = CCMenu::create(pushProgramButton, NULL);
        
        menu->setContentSize( CCSizeMake(GetGameParams->programLayerWidth, pushProgramButton->getContentSize().height));
        
        menu->setPosition( ccp(0,getContentSize().height - nameLabel->getContentSize().height - METHODLAYER_TILE_OFFSET - menu->getContentSize().height) );
        menu->setAnchorPoint( CCPointZero );
        
        addChild(menu, 10);
    }
    
    drawHint();
}


bool MethodLayer::conditionAtTouch(cocos2d::CCTouch *touch)
{
    if(!useCondition)
        return false;
    
    CCSprite *sprite  = (CCSprite *)getChildByTag(CONDITION_SPRITE_TAG);
    
    CCRect boundingBox = sprite->boundingBox();
    boundingBox.setRect(boundingBox.getMinX() - METHODLAYER_TILE_OFFSET/2, boundingBox.getMinY() + METHODLAYER_TILE_OFFSET/2,
                        boundingBox.size.width + METHODLAYER_TILE_OFFSET, boundingBox.size.height + METHODLAYER_TILE_OFFSET);

    return boundingBox.containsPoint(this->convertTouchToNodeSpace(touch));
    
}

bool MethodLayer::repeaterAtTouch(cocos2d::CCTouch *touch)
{
    if(!useRepeater)
        return false;
    
    CCSprite *sprite  = (CCSprite *)getChildByTag(REPEATER_SPRITE_TAG);
    
    CCRect boundingBox = sprite->boundingBox();
    boundingBox.setRect(boundingBox.getMinX() - METHODLAYER_TILE_OFFSET/2, boundingBox.getMinY() + METHODLAYER_TILE_OFFSET/2,
                        boundingBox.size.width + METHODLAYER_TILE_OFFSET, boundingBox.size.height + METHODLAYER_TILE_OFFSET);
    
    return boundingBox.containsPoint(this->convertTouchToNodeSpace(touch));
}

int MethodLayer::methodAtTouch(CCTouch *touch)
{
    for(int j = 0; j < methods.size(); ++j)
    {
        CCSprite *sprite  = (CCSprite *)backGroundLayer->getChildren()->objectAtIndex(j);
        
        CCRect boundingBox = sprite->boundingBox();
        boundingBox.setRect(boundingBox.getMinX() - METHODLAYER_TILE_OFFSET/2, boundingBox.getMinY() + METHODLAYER_TILE_OFFSET/2,
                            boundingBox.size.width + METHODLAYER_TILE_OFFSET, boundingBox.size.height + METHODLAYER_TILE_OFFSET);
        
        if(boundingBox.containsPoint(this->convertTouchToNodeSpace(touch)))
        {
            return j;
        }
        
    }
    
    return -1;
}

int MethodLayer::lockAtTouch(CCTouch *touch)
{
    for(int j = methods.size(); j < 2*methods.size(); ++j)
    {
        CCSprite *sprite  = (CCSprite *)methodLayer->getChildren()->objectAtIndex(j);
        
        CCRect boundingBox = sprite->boundingBox();
        
        if(boundingBox.containsPoint(methodLayer->convertTouchToNodeSpace(touch)))
        {
            return j-methods.size();
        }
        
    }
    
    return -1;
}

void MethodLayer::resizeWidth(int sgn)
{
    if(sgn == 1)
    {
        width++;
        for(int i = 0; i < height; ++i)
        {
            
            methods.insert(methods.begin() + width*(i+1) - 1, 0);
            backGround.insert(backGround.begin() + width*(i+1) - 1, 1);
        }
    }
    else
    {
        for(int i = 0; i < height; ++i)
        {
            methods.erase(methods.begin() + width*(height - i) - 1);
            backGround.erase(backGround.begin() + width*(height - i) - 1);
        }
        width--;
    }
    removeAllChildrenWithCleanup(true);
    drawFunctions();
}

void  MethodLayer::resize(int sgn)
{
    if(sgn == 1)
    {
        for(int i = 0; i < width; ++i)
        {
            
            methods.push_back(0);
            backGround.push_back(1);
        }
    }
    else
    {
        for(int i = 0; i < width; ++i)
        {
            methods.pop_back();
            backGround.pop_back();
        }
    }
    setContentSize( CCSizeMake(getContentSize().width, getContentSize().height + sgn * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET)) );
    realHeight += sgn * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET);
    height += sgn;
    
    removeAllChildrenWithCleanup(true);
    drawFunctions();
    parent->update();
    
    
//    for(int j = 0; j < methods.size(); ++j)
//    {
//        CCSprite *sprite  = (CCSprite *)backGroundLayer->getChildren()->objectAtIndex(j);
//        sprite->setPosition( ccp(sprite->getPositionX(),sprite->getPositionY() + sgn * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET)) );
//        
//        sprite  = (CCSprite *)methodLayer->getChildren()->objectAtIndex(j);
//        sprite->setPosition( ccp(sprite->getPositionX(),sprite->getPositionY() + sgn * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET)) );
//    }
//    
//    if(sgn == 1)
//    {
//        for(int i = 0; i < width; ++i)
//        {
//            CCSprite *tmp = PMTextureCache::instanse()->getMethodBackgroundSprite();
//            tmp->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) * i, METHODLAYER_TILE_OFFSET)  );
//            tmp->setAnchorPoint( CCPointZero );
//            
//            backGroundLayer->addChild(tmp,12);
//            
//          
//            tmp = PMTextureCache::instanse()->getRobotMethodSprite(EMPTY_METHOD);
//
//            
//            tmp->setPosition( ccp((BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET) * i, METHODLAYER_TILE_OFFSET)  );
//            tmp->setAnchorPoint( CCPointZero );
//            
//            methodLayer->addChild(tmp, 13);
//            
//            methods.push_back(EMPTY_METHOD);
//            backGround.push_back(pmNormal);
//        }
//    }
//    else
//    {
//        for(int i = 0; i < width; ++i)
//        {
//            CCSprite *sprite  = (CCSprite *)backGroundLayer->getChildren()->objectAtIndex(methods.size() - 1);
//            backGroundLayer->removeChild(sprite);
//            
//            sprite  = (CCSprite *)methodLayer->getChildren()->objectAtIndex(methods.size() - 1);
//            methodLayer->removeChild(sprite);
//            
//            methods.pop_back();
//            backGround.pop_back();
//        }
//
//    }
//    
//    setContentSize( CCSizeMake(getContentSize().width, getContentSize().height + sgn * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET)) );
//    realHeight += sgn * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET);
//    height += sgn;
//    
//    getChildByTag(NAME_LABEL_TAG)->setPosition( ccp(-BORDER_RADIUS / 2,realHeight + METHODLAYER_TILE_OFFSET ) );
//    
//    CCSprite *nameSprite = (CCSprite *)getChildByTag(NAME_SPRITE_TAG);
//    
//    if(nameSprite != NULL)
//        nameSprite->setPosition( ccp(nameLength - nameSprite->getContentSize().width, realHeight + BORDER_RADIUS / 2));
//    
//    if(deleteMenu)deleteMenu->setPosition(ccp(nameLength, realHeight + BORDER_RADIUS / 2));
//    
//    if(hint != NULL)
//        getChildByTag(INFO_BUTTON_TAG)->setPosition( ccp(nameLength + 15, getContentSize().height - 12) );
//    
//    if(useRepeater)
//    {
//        getChildByTag(REPEATER_BUTTON_TAG)->setPosition( ccp(0, realHeight - BUTTON_TILE_SIZE) );
//        getChildByTag(REPEATER_SPRITE_TAG)->setPosition( ccp(0, realHeight - BUTTON_TILE_SIZE) );
//    }
//    
//    if(useCondition)
//    {
//        int useR = (useRepeater) ? 1 : 0;
//        
//        getChildByTag(CONDITION_BUTTON_TAG)->setPosition( ccp(useR * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET), realHeight - BUTTON_TILE_SIZE) );
//        getChildByTag(CONDITION_SPRITE_TAG)->setPosition( ccp(useR * (BUTTON_TILE_SIZE + METHODLAYER_TILE_OFFSET), realHeight - BUTTON_TILE_SIZE) );
//    }
//    
//#ifdef MAP_EDITOR
//    lastBackground = backGround;
//#endif
//    parent->update();
}

bool MethodLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    draggedSprite = NULL;
    
    CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
    
#ifdef MAP_EDITOR
    LockAtTouch = lockAtTouch(touch);
    if (LockAtTouch!=-1) {
        printf("%d",LockAtTouch);
        touchType = mlSwitchLock;
        return true;
    }
#endif
    
    if(!enabled || hidden)
        return false;
    
    draggingTime = getTimeMs();
    
    draggedMethod = methodAtTouch(touch);
    bool isConditionAtTouch = conditionAtTouch(touch);
    bool isRepeaterAtTouch = repeaterAtTouch(touch);
    
    if( touch && draggedMethod != -1 )
    {
        if(backGround[draggedMethod] == pmBlocked)
        {
#ifndef MAP_EDITOR
            return false;
#endif
        }
        
        if(methods[draggedMethod] != EMPTY_METHOD)
        {
            draggedSprite = CCSprite::createWithTexture( ((CCSprite *)methodLayer->getChildren()->objectAtIndex(draggedMethod))->getTexture() );
            draggedSprite->setPosition( ccp(-100,-100) );
            addChild(draggedSprite);
            reorderChild(draggedSprite, 20);
        
        }
        else
        {
            draggedSprite = NULL;
        }
        
        CCTexture2D *selectedTexture = PMTextureCache::instanse()->getMethodBackgroundTexture_Selected();
        CCSprite *sprite = (CCSprite *)backGroundLayer->getChildren()->objectAtIndex(draggedMethod);
        
        sprite->setTexture(selectedTexture);
        sprite->setContentSize(selectedTexture->getContentSize());
        sprite->setTextureRect(CCRectMake(0,0,selectedTexture->getContentSize().width,selectedTexture->getContentSize().height));
        
        
        touchType = mlDrag;
        
        return true;
    }
    else if(isRepeaterAtTouch)
    {
#ifdef MAP_EDITOR 
        touchType = mlSwitchRepeater;
#else
        touchType = mlDrag;
#endif
        
        return true;
    }
    else if(isConditionAtTouch)
    {
#ifdef MAP_EDITOR 
        touchType = mlSwitchCondition;
#else
        touchType = mlDrag;
#endif
        return true;
    }

#ifndef MAP_EDITOR 
    // here will be a sprite
    if(resizable && touchPoint.y >=0 && touchPoint.y <= 50 && touchPoint.x >= realWidth - 50 && touchPoint.x <= realWidth)
    {
        touchType = mlResize;
        startPoint = parent->convertTouchToNodeSpace(touch);
        
        return true;
    }
#else
    if(resizeSprite->boundingBox().containsPoint(touchPoint))
    {
        touchType = mlResize;
        startPoint = parent->convertTouchToNodeSpace(touch);
        
        return true;
    }
#endif
    
    
    return false;
    
}


void MethodLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if( touch && draggedSprite != NULL && touchType == mlDrag)
        draggedSprite->setPosition(convertTouchToNodeSpace(touch));
#ifndef MAP_EDITOR
    if(touch && touchType == mlResize)
    {
        CCPoint touchPoint  = parent->convertTouchToNodeSpace(touch);
        
        int diff = ccpSub(startPoint, touchPoint).y;
        
        if(diff >= BUTTON_TILE_SIZE)
        {
            if(height + 1 > maxHeight)
                return;
                
            resize(1);
        
            startPoint = touchPoint;
        }
        
        if(diff <= -BUTTON_TILE_SIZE)
        {
            if(height - 1 < minHeight)
                return;
            
            resize(-1);
            
            startPoint = touchPoint;
        }
    }
#else
    if(touch && touchType == mlResize)
    {
        CCPoint touchPoint  = parent->convertTouchToNodeSpace(touch);
        
        int diff = ccpSub(startPoint, touchPoint).y;
        
        if(diff >= BUTTON_TILE_SIZE)
        {
            resize(1);
            startPoint.y = touchPoint.y;
        }
        
        if(diff <= -BUTTON_TILE_SIZE)
        {
            if(height - 1 < 1)
                return;
            
            resize(-1);
            startPoint.y = touchPoint.y;
        }
        
        
        diff = ccpSub(startPoint, touchPoint).x;
        
        if(diff >= BUTTON_TILE_SIZE)
        {
            if(width < 2)
                return;
            resizeWidth(-1);
            startPoint.x = touchPoint.x;
        }
        
        if(diff <= -BUTTON_TILE_SIZE)
        {
            resizeWidth(1);
            startPoint.x = touchPoint.x;
        }
    }
#endif
    
    CCLayer::ccTouchMoved(touch, pEvent);
}

void MethodLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
#ifdef MAP_EDITOR
    CCPoint start = touch->getStartLocation();
    CCPoint end = touch->getLocation();
    
    CCPoint dif = ccpSub(start, end);
    
    if (touchType == mlDrag && fabs(dif.x) < 10 && fabs(dif.y) < 10 ) {
        if(backGround[draggedMethod])
        {
            lastBackground[draggedMethod] = backGround[draggedMethod];
            backGround[draggedMethod] = 0;
        }
        else backGround[draggedMethod] = lastBackground[draggedMethod];
        removeAllChildrenWithCleanup(true);
        drawFunctions();
        return;
    }
    
    if (touchType == mlSwitchLock && fabs(dif.x) < 10 && fabs(dif.y) < 10) {
        if (backGround[LockAtTouch] == 1) {
            backGround[LockAtTouch] = 2;
        }
        else if (backGround[LockAtTouch] == 2) {
            backGround[LockAtTouch] = 1;
        } else {
            if (lastBackground[LockAtTouch] == 1) {
                lastBackground[LockAtTouch] = 2;
            }
            else if (lastBackground[LockAtTouch] == 2) {
                lastBackground[LockAtTouch] = 1;
            }
        }
        removeAllChildrenWithCleanup(true);
        drawFunctions();
        return;
    }
    
#endif

    static bool disableRepeater = false;
    static int oldRepeater = -1;
    static bool disableCondition = false;
    static int oldCondition = -1;

    if( touch && touchType == mlSwitchRepeater)
    {
        if (disableRepeater && repeater != -1) {
            disableRepeater = false;
        }
        
        if (disableRepeater)
        {
            addRepeater(oldRepeater);
            disableRepeater = false;
        }
        else
        {
            oldRepeater = repeater;
            addRepeater(-1);
            disableRepeater = true;
        }
    }
    else if( touch && touchType == mlSwitchCondition)
    {
        if (disableCondition && condition != -1) {
            disableCondition = false;
        }
        
        if (disableCondition)
        {
            addCondition(oldCondition);
            disableCondition = false;
        }
        else
        {
            oldCondition = condition;
            addCondition(-1);
            disableCondition = true;
        }
    }
    else if( touch && touchType == mlDrag)
    {
        int droppedMethod = methodAtTouch(touch);
            
        if(getTimeMs() - draggingTime < TOUCH_TIME)
        {
            SelectButton *activeButton = parent->getActiveButton();
           
            setButton(activeButton, touch);
            
            if(draggedSprite != NULL)
                removeChild(draggedSprite, true);
            
        }
        else if(droppedMethod != -1 && draggedSprite != NULL)
        {
            addAction(droppedMethod, methods[draggedMethod]);
            
            removeChild(draggedSprite, true);
        }
        else if(draggedMethod != -1)
        {
            addAction(draggedMethod, EMPTY_METHOD);
            
            if(draggedSprite != NULL)
                removeChild(draggedSprite, true);
        }
        
        if(draggedMethod != -1)
        {
            CCTexture2D *normalTexture = PMTextureCache::instanse()->getMethodBackgroundTexture();
            CCSprite *sprite = (CCSprite *)backGroundLayer->getChildren()->objectAtIndex(draggedMethod);
            
            sprite->setTexture(normalTexture);
            sprite->setContentSize(normalTexture->getContentSize());
            sprite->setTextureRect(CCRectMake(0,0,normalTexture->getContentSize().width,normalTexture->getContentSize().height));
        }

    }
    
    CCLayer::ccTouchEnded(touch, pEvent);
}

void MethodLayer::setButton(SelectButton *activeButton, CCTouch *touch)
{
    if(activeButton != NULL)
    {
        int method = methodAtTouch(touch);
        bool isConditionAtTouch = conditionAtTouch(touch);
        bool isRepeaterAtTouch = repeaterAtTouch(touch);
        
        if(activeButton->isMethod() && method != -1 && backGround[method] != pmBlocked )
            addAction(method, activeButton->getMethodID(), true);
        else if(activeButton->isCondition() && isConditionAtTouch)
            addCondition(activeButton->getCondition());
        else if(activeButton->isRepeater() && isRepeaterAtTouch)
            addRepeater(activeButton->getRepeater());
    }
}

void MethodLayer::addAction(int methodIndex,int newMethodID, bool byProgramLayer)
{
    CCTexture2D *tex = NULL;
    
    if(methods[methodIndex] != newMethodID)
    {
        if(newMethodID >= EMPTY_METHOD)
            tex = PMTextureCache::instanse()->getRobotMethodTexture(newMethodID);
        else
            tex = PMTextureCache::instanse()->getProgramSprite(newMethodID)->getTexture();
        
        methods[methodIndex] = newMethodID;
    }
    else if(parent->getActiveButton() != NULL && byProgramLayer)
    {
        methods[methodIndex] = EMPTY_METHOD;
        tex = PMTextureCache::instanse()->getRobotMethodTexture(EMPTY_METHOD);
    }
    
    if(tex != NULL)
        ((CCSprite *)methodLayer->getChildren()->objectAtIndex(methodIndex))->setTexture(tex);

    parent->getParent()->saveMap();
}

void MethodLayer::addCondition(int newCondition)
{
    CCTexture2D *tex = NULL;
    
    if(newCondition == -1) {
        tex = PMTextureCache::instanse()->getRobotConditionTexture_Disabled(0);
        condition = -1;
    }
    
    else if(condition != newCondition)
    {
        tex = PMTextureCache::instanse()->getRobotConditionTexture(newCondition);
        
        condition = (Robot4::Condition)newCondition;
    }
    else if(parent->getActiveButton() != NULL)
    {
        condition = EMPTY_CONDITION;
        tex = PMTextureCache::instanse()->getConditionBackgroundTexture();
    }
    
    if(tex != NULL)
        ((CCSprite *)getChildByTag(CONDITION_SPRITE_TAG))->setTexture(tex);
    
    parent->getParent()->saveMap();
}

void MethodLayer::addRepeater(int newRepeater)
{
    CCTexture2D *tex = NULL;
    
    if(newRepeater == -1) {
        tex = PMTextureCache::instanse()->getRepeaterTexture_Disabled(1);
        repeater = -1;
    }
    
    else if(repeater != newRepeater)
    {
        tex = PMTextureCache::instanse()->getRepeaterTexture(newRepeater);
        
        repeater = newRepeater;
    }
    else if(parent->getActiveButton() != NULL)
    {
        repeater = 1;
        tex = PMTextureCache::instanse()->getRepeaterTexture(repeater);
    }
    
    if(tex != NULL)
        ((CCSprite *)getChildByTag(REPEATER_SPRITE_TAG))->setTexture(tex);
    
    parent->getParent()->saveMap();
}

void MethodLayer::setControlledMode(bool flag)
{
    setEnabled( !flag , false);
    
    if(!hidden)
    {
        double endX = realWidth + BORDER_RADIUS;
        
        if(!flag)
            endX = GetGameParams->programLayerWidth;
        
        CCFiniteTimeAction* moveButton = CCMoveTo::create(SYSTEM_ANIMATION_DELAY / 2, ccp(endX,0) );
        pushProgramButton->runAction(moveButton);
    }
}

void MethodLayer::addProgram(CCObject *sender)
{
    std::vector<int> methods = ((GameLayer *)parent->getParent())->getMethodStackLayer()->getProgram();
    
    for(int j = 0; j < methods.size() && j < width * height; ++j)
    {
        addAction(j, methods[j]);
    }
    
    for(int j = methods.size(); j < width * height; ++j)
    {
        addAction(j, EMPTY_METHOD);
    }

}

void MethodLayer::drawHint()
{
#ifdef MAP_EDITOR
    if(hint == NULL) {
        hint = Hint::create();
        
        HintPart part;
        part.type = TextHint;
        part.hintText = "";
        hint->hintParts.push_back(part);
    }
#endif
    if(hint != NULL)
    {
        showHintButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getInfoIconSprite(),
                                                                    PMTextureCache::instanse()->getInfoIconSprite_Selected(),
                                                                    PMTextureCache::instanse()->getInfoIconSprite_Disabled(),
                                                                    this, menu_selector(MethodLayer::showHint));
        
        CCMenu *menu = CCMenu::create(showHintButton, NULL);
        
        double x = nameLength + showHintButton->getContentSize().width / 2;
        double y = getContentSize().height - showHintButton->getContentSize().height / 2;
        
        showHintButton->setPosition( CCPointZero );
        menu->setPosition( ccp(x,y) );
        
        addChild(menu, 10, INFO_BUTTON_TAG);
    }
}

void MethodLayer::showHint(CCObject *sender)
{
#ifndef MAP_EDITOR
    for(int i = 0; i < hint->hintParts.size(); ++i)
    {
        if(hint->hintParts[i].type == TextHint)
        {
            if(!hintShowed)
            {
                hintLayer = HintLayer::create(hint->hintParts[i].hintText);
                
                
                parent->getParent()->addChild(hintLayer,20);
                hintShowed = true;
            }
            else
            {
                parent->getParent()->removeChild(hintLayer);
                hintShowed = false;
            }
        }
        else if(hint->hintParts[i].type == MethodHint)
        {
            std::vector<int> methodsCopy = methods;
            for(int j = 0; j < hint->hintParts[i].funcHint.size(); ++j)
            {
                addAction(j, hint->hintParts[i].funcHint[j]);
            }
            
            hint->hintParts[i].funcHint = methodsCopy;
        }
    }
#else
    if(!hintShowed)
    {
        std::string HintName = "";
        
        switch (methodID) {
            case CMDMAIN:
                HintName = "Подсказка для главной программы";
                break;
            case CMDA:
                HintName = "Подсказка для подпрограммы А";
                break;
            case CMDB:
                HintName = "Подсказка для подпрограммы B";
                break;
            case CMDC:
                HintName = "Подсказка для подпрограммы C";
                break;
        }
        
        if (hint->hintParts[0].type == TextHint) hintLayer = HintEditLayer::create(hint->hintParts[0].hintText,HintName);
        else hintLayer = HintEditLayer::create(hint->hintParts[1].hintText,HintName);
        
        parent->getParent()->addChild(hintLayer,20);
        hintShowed = true;
    }
    else
    {
        std::string text = hintLayer->hint->getText();
        if(text!="") {
            HintPart part;
            part.type = TextHint;
            part.hintText = text;
            
            if(hint->hintParts[0].type == TextHint)
            {
                hint->hintParts[0] = part;
            }
            else {
                if (hint->hintParts.size() == 1)
                {
                    hint->hintParts.push_back(part);
                }
                else hint->hintParts[1] = part;
            }
        }
        parent->getParent()->removeChild(hintLayer);
        hintShowed = false;
    }
#endif
}

void MethodLayer::highlightMethodBreak(int method)
{
    CCTexture2D *selectedTexture = PMTextureCache::instanse()->getMethodBackgroundTexture_Broken();
    CCSprite *sprite = (CCSprite *)backGroundLayer->getChildren()->objectAtIndex(method);
    
    sprite->setTexture(selectedTexture);
    sprite->setContentSize(selectedTexture->getContentSize());
    sprite->setTextureRect(CCRectMake(0,0,selectedTexture->getContentSize().width,selectedTexture->getContentSize().height));
}

void MethodLayer::highlightMethod(int method)
{
    CCTexture2D *selectedTexture = PMTextureCache::instanse()->getMethodBackgroundTexture_Selected();
    CCSprite *sprite = (CCSprite *)backGroundLayer->getChildren()->objectAtIndex(method);
    
    sprite->setTexture(selectedTexture);
    sprite->setContentSize(selectedTexture->getContentSize());
    sprite->setTextureRect(CCRectMake(0,0,selectedTexture->getContentSize().width,selectedTexture->getContentSize().height));
}

void MethodLayer::clearHighlightMethod(int method)
{
    CCTexture2D *normalTexture = PMTextureCache::instanse()->getMethodBackgroundTexture();
    CCSprite *sprite = (CCSprite *)backGroundLayer->getChildren()->objectAtIndex(method);
    
    sprite->setTexture(normalTexture);
    sprite->setContentSize(normalTexture->getContentSize());
    sprite->setTextureRect(CCRectMake(0,0,normalTexture->getContentSize().width,normalTexture->getContentSize().height));
}

void ccDrawArc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments)
{

    float theta = arc_angle / float(num_segments - 1);
	float tangetial_factor = tanf(theta);
    
	float radial_factor = cosf(theta);
    
	
	float x = r * cosf(start_angle);
	float y = r * sinf(start_angle);
    
    CCPoint *arc_vertices;
    arc_vertices = (CCPoint *)malloc(sizeof(CCPoint) * (num_segments));
    
	for(int ii = 0; ii < num_segments; ii++)
	{
        arc_vertices[ii].x = x+ cx;
        arc_vertices[ii].y = y +cy;
        
		float tx = -y;
		float ty = x;
        
		x += tx * tangetial_factor;
		y += ty * tangetial_factor;
        
		x *= radial_factor;
		y *= radial_factor;
    }
    
    ccDrawPoly(arc_vertices, num_segments, false);
    free(arc_vertices);
}

void MethodLayer::draw()
{
    // FUCK my brain!
    //drawing a rectangle with border-radius

    double endX = realWidth + BORDER_RADIUS / 2 ;
    double startY = realHeight - BORDER_RADIUS / 2;
    
    double endY = METHODLAYER_TILE_OFFSET + BORDER_RADIUS / 2 ;
    
    int startX =  - BORDER_RADIUS / 2;
    
    //CCLayerColor::draw();
    
    ccDrawInit();
    ccDrawColor4F(255,255,255,255);
    glLineWidth(1.5f);
    //draw lines
    
    ccDrawLine(ccp(startX + BORDER_RADIUS,startY + BORDER_RADIUS), ccp(endX - BORDER_RADIUS ,startY + BORDER_RADIUS));
    ccDrawLine(ccp(startX,startY), ccp(startX ,endY));
    ccDrawLine(ccp(startX + BORDER_RADIUS,endY - BORDER_RADIUS), ccp(endX - BORDER_RADIUS ,endY - BORDER_RADIUS));
    ccDrawLine(ccp(endX ,endY), ccp(endX ,startY));
    
    //draw circles
    ccDrawArc( startX + BORDER_RADIUS,startY , BORDER_RADIUS,CC_DEGREES_TO_RADIANS(90),CC_DEGREES_TO_RADIANS(90), 20);
    ccDrawArc( startX + BORDER_RADIUS,endY , BORDER_RADIUS,CC_DEGREES_TO_RADIANS(180),CC_DEGREES_TO_RADIANS(90), 20);
    ccDrawArc( endX - BORDER_RADIUS,endY, BORDER_RADIUS,CC_DEGREES_TO_RADIANS(270),CC_DEGREES_TO_RADIANS(90), 20);
    ccDrawArc( endX - BORDER_RADIUS,startY, BORDER_RADIUS,CC_DEGREES_TO_RADIANS(0),CC_DEGREES_TO_RADIANS(90), 20);
    
    
}

vector<_instruction> MethodLayer::makeMethod()
{
    return RobotMethod::makeMethod(methods,index);
}

std::vector<_instruction> MethodLayer::makeExecMethod()
{
    return RobotMethod::makeExecuteMethod(methodID, useRepeater, repeater, useCondition, condition);
}

#ifdef MAP_EDITOR
void MethodLayer::deleteMethod(CCObject* sender)
{
    parent->getScroll()->getContainer()->removeChild(this, true);
    parent->layers.erase(parent->layers.begin()+index);
    
    if (index < parent->layers.size()) {
        for (int i = index; i < parent->layers.size(); ++i) {
            parent->layers[i]->moveUp();
        }
    }
    
    parent->update();
}

void MethodLayer::moveUp ()
{
    switch (index) {
        case 2:
            methodID = CMDA;
            methodID_Exec = CMDA_EXECUTE;
            name = "Command 1";
            break;
        case 3:
            methodID = CMDB;
            methodID_Exec = CMDB_EXECUTE;
            name = "Command 2";
            break;
    }
    index--;
    
    removeChildByTag(NAME_SPRITE_TAG);
    
    CCSprite* nameSprite = NULL;
    
   if(methodID == CMDA) {
        nameSprite = PMTextureCache::instanse()->getProgramASprite();
    } else if(methodID == CMDB) {
        nameSprite = PMTextureCache::instanse()->getProgramBSprite();
    } else if(methodID == CMDC) {
        nameSprite = PMTextureCache::instanse()->getProgramCSprite();
    }
    
    if(nameSprite != NULL)
    {
        addChild(nameSprite , 0, NAME_SPRITE_TAG);
        nameSprite->setPosition( ccp(nameLength-nameSprite->getContentSize().width, realHeight + BORDER_RADIUS / 2));
        nameSprite->setAnchorPoint( CCPointZero );
        nameSprite->setScale(0.8f);
    }
}

void MethodLayer::makeHint (CCObject* sender)
{
    newHint = methods;
}

void MethodLayer::readyForSave ()
{
    if (repeater == -1) {
        repeater = 0;
        useRepeater = false;
    }
    
    if (condition == -1) {
        condition = 0;
        useCondition = false;
    }
    
    if (newHint.size() == methods.size()) {
        
        HintPart part;
        part.type = MethodHint;
        part.funcHint = newHint;
        
        if(hint->hintParts[0].type == MethodHint)
        {
            hint->hintParts[0] = part;
        }
        else {
            if (hint->hintParts.size() == 1)
            {
                hint->hintParts.push_back(part);
            }
            else hint->hintParts[1] = part;
        }
    }
    
    
    if(hint->hintParts[0].type == TextHint && hint->hintParts[0].hintText == "")
    {
        hint->hintParts.erase(hint->hintParts.begin());
    }
    else {
        if(hint->hintParts.size() == 1 && hint->hintParts[1].type == TextHint && hint->hintParts[1].hintText == "")
        {
            hint->hintParts.erase(hint->hintParts.end());
        }
    }
}


#endif
