//
//  ScrollLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 10.04.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "ScrollLayer.h"
#include "GameLayer.h"

#ifdef MAP_EDITOR
#include "MapEditorGameLayer.h"
#endif

USING_NS_CC;


bool ScrollLayer::init()
{
    parallax = CCParallaxNode::create();
    parallax->setPosition( CCPointZero );
    
    CCSize mapSize = GetGameParams->mapSize();
    
    CCSprite *backGround = CCSprite::create(PICT_PATH "Background/background2.png");
    //backGround->setPosition( ccp(mapSize.width /2 , mapSize.height / 2) );
    //backGround->setAnchorPoint( CCPointZero );
    
    //resizeSprite(backGround, mapSize.width , mapSize.height);
    
    backGround->setTag(0);
    parallax->addChild(backGround, -1, ccp(0.4f,0.5f), ccp(mapSize.width /2 , mapSize.height / 2));

    
    addChild(parallax);
    
    return true;
}

void ScrollLayer::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();
}

void ScrollLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool ScrollLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if(!active)
        return false;
    
    int activeLayer = layer->getActiveLayer();
    CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
    
    if(boundingBox().containsPoint(touchPoint))
    {                
        CCPoint mapPoint = layer->getLayer(activeLayer)->getMapLayer()->convertTouchToNodeSpace(touch);
        CCSize  mapSize = layer->getLayer(activeLayer)->getMapLayer()->getContentSize();
        
#ifdef MAP_EDITOR
        if (((Robot4*)layer->getWorld()->robotAt(0))->boundingBox().containsPoint(mapPoint))
        {
            touchType = DragRobot;
            return true;
        }
        
        if ( ((Map4*)MapEditorGameLayer::getInstanse()->getWorld()->getMap())->resizeSprite->boundingBox().containsPoint(mapPoint)) {
            touchType = ResizeMap;
            startPoint = mapPoint;
            return true;
        }
#endif
        
        if( mapPoint.x >= -((Map4*)layer)->mapElementSize.width / 2 && mapPoint.x <= mapSize.width &&  mapPoint.y >= 0 && mapPoint.y <= mapSize.height  + ((Map4*)layer)->mapElementSize.height / 2)
        {
            startLayerPosition =parallax->getPosition();
            startPoint = touchPoint;
            startSize = getContentSize();
            touchType = DragLayer;
#ifdef MAP_EDITOR
            if(MapEditorGameLayer::getInstanse()->editing) {
                touchType = EditLayer;
                if(layer->type() == pmMap4) {
                    ((Map4*)layer)->changeElementTypeAtPoint(mapPoint, activeLayer);
                }
            }
#endif
            return true;
        }
        else if(layer->getLayerCount() != 1)
        {
            double centerX = GameParams::getInstanse()->mapSize().width/2;
            double centerY = GameParams::getInstanse()->mapSize().height/2;
        
            layer->scrollToInitialPosition();
            
            if( touchPoint.y <= LAYER_POSITION_Y(touchPoint.x + SCROLLLAYER_TOUCH_OFFSET, centerX , centerY) &&
               touchPoint.y >= LAYER_POSITION_Y(touchPoint.x - SCROLLLAYER_TOUCH_OFFSET, centerX , centerY) )
            {
                startPoint  = touchPoint;
                touchType = ScrollLayers;
                return true;
            }
        }
    }
    return false;
}


void ScrollLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    int activeLayer = layer->getActiveLayer();
    CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
   
    // set the new sprite position
    if(touchType == DragLayer)
    {
        CCPoint diff = touch->getDelta();
        
        CCPoint currentPos = parallax->getPosition();
        parallax->setPosition( ccpAdd(currentPos, diff) );
        
        startPoint = touchPoint;
        
        return;
    }
#ifdef MAP_EDITOR
    if(touchType == EditLayer) {
        CCPoint touch_ = layer->getLayer(activeLayer)->getMapLayer()->convertTouchToNodeSpace(touch);
        
        if(layer->type() == pmMap4) {
            ((Map4*)layer)->changeElementTypeAtPoint(touch_, activeLayer);
        }
        return;
    }
    
    if (touchType == DragRobot) {
        CCPoint touch_ = layer->getLayer(activeLayer)->getMapLayer()->convertTouchToNodeSpace(touch);
        ((Robot4*)layer->getWorld()->robotAt(0))->setPosition(((Map4*)layer)->coordAtCcp(touch_), ((Robot4*)layer->getWorld()->robotAt(0))->getCurLayer(), ((Robot4*)layer->getWorld()->robotAt(0))->getCurDirection());
        ((Map4*)layer)->update();
        return;
    }
    
    if (touchType == ResizeMap) {
        CCPoint touch_ = layer->getLayer(activeLayer)->getMapLayer()->convertTouchToNodeSpace(touch);
        int tmp = ((Map4*)layer)->resizeAtTouch(((Map4*)layer)->dxdyAtCcp(ccpSub(touch_, startPoint)));
        if(tmp == 0) {
            startPoint = touch_;
        }
        if(tmp == 1) {
            startPoint.x = touch_.x;
        }
        if(tmp == 2) {
            startPoint.y = touch_.y;
        }
        
        return;
    }
    
#endif
    if(touchType == ScrollLayers)
    {
        double centerX = GameParams::getInstanse()->mapSize().width/2;
        double centerY = GameParams::getInstanse()->mapSize().height/2;
        
        CCPoint touchPoint = this->convertTouchToNodeSpace(touch);
        double dx = startPoint.x - touchPoint.x;
        
        double x,y;
        
        for(int i = 0; i < layer->getLayerCount(); ++i)
        {
            double curLyaerX = LAYER_POSITION_X(fabs((float)activeLayer - i));
            double prevLyaerX = (dx > 0.0) ? LAYER_POSITION_X(fabs((float)activeLayer - i + 1)) : LAYER_POSITION_X(fabs((float)activeLayer - i - 1));
            x = layer->getLayer(i)->getPositionX() - dx * fabs(curLyaerX - prevLyaerX) / LAYER_POSITION_X(1);
            
            y = LAYER_POSITION_Y(x, centerX , centerY);
            
            layer->getLayer(i)->setPosition( ccp(x,y) );
            
            int opacity = LAYER_OPACITY(x - centerX);
            if(opacity > 255)
                opacity = 255;

            layer->setLayerOpacity(i, opacity);
            layer->getLayer(i)->getMapLayer()->setScale(SCALE_FUNCTION(x - centerX));
            
            if(fabs(x - centerX - LAYER_POSITION_X(0)) <= SCROLLLAYER_ACTIVELAYER_DELTA)
            {
                layer->setActiveLayer(i);
                activeLayer = i;
                
                CCLOG("ScrollLayer - set Active Layer %d",i);
            }
            
        }
        
        startPoint = touchPoint;
    }

}

void ScrollLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    int activeLayer = layer->getActiveLayer();
    
    double centerX = GameParams::getInstanse()->mapSize().width/2;
    double centerY = GameParams::getInstanse()->mapSize().height/2;
    
    
    if(touchType == DragLayer)
    {
#ifdef MAP_EDITOR
        if (fabs(touch->getStartLocation().x - touch->getLocation().x) < 20 && fabs(touch->getStartLocation().y - touch->getLocation().y) < 20) {
            CCPoint touch_ = layer->getLayer(activeLayer)->getMapLayer()->convertTouchToNodeSpace(touch);
            
            if(layer->type() == pmMap4) {
                ((Map4*)layer)->changeElementTypeAtPoint(touch_, activeLayer);
            }
        }
#endif
        setContentSize(startSize);
        
        CCPoint movePoint;
        
        CCLayer *mapLayer = layer->getLayer(activeLayer)->getMapLayer();
        
        CCSize mapSize = mapLayer->boundingBox().size;
        mapSize.height  /= 2;
        
        CCPoint minPoint = mapLayer->boundingBox().origin;
        
        minPoint = layer->getLayer(activeLayer)->convertToWorldSpaceAR(minPoint);
        CCPoint maxPoint = CCPointMake(minPoint.x + mapSize.width,minPoint.y + mapSize.height);
        
        
        if(mapSize.width > getContentSize().width)
        {
            if(minPoint.x > 0)
                movePoint.x = mapLayer->boundingBox().size.width/2 - GetGameParams->mapSize().width/2;
            else if(maxPoint.x < GetGameParams->mapSize().width)
                 movePoint.x = GetGameParams->mapSize().width/2 - mapLayer->boundingBox().size.width/2;
            else
                movePoint.x = parallax->getPositionX();
        }
        else
            movePoint.x = startLayerPosition.x;
        
        if(mapSize.height > getContentSize().height)
        {
            
            if(minPoint.y > 0)
                movePoint.y = mapLayer->boundingBox().size.height/2;
            else if(maxPoint.y < GetGameParams->mapSize().height)
                movePoint.y = GetGameParams->mapSize().height - mapLayer->boundingBox().size.height/2;
            else
                movePoint.x = parallax->getPositionX();
        }
        else
            movePoint.y = startLayerPosition.y;
        
        CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY / 2,
                                                         movePoint);
            
        parallax->runAction(moveLayer);
        
        return;
    }
    
    if(touchType == ScrollLayers)
    {
        
        double min = GameParams::getInstanse()->mapSize().width;

        for(int i = 0; i < layer->getLayerCount(); ++i)
        {
            if(fabs( layer->getLayer(i)->getPositionX() - centerX - LAYER_POSITION_X(0) ) < min)
            {
                layer->setActiveLayer(i);
                
                min = fabs( layer->getLayer(i)->getPositionX() - centerX - LAYER_POSITION_X(0));
            }
        }
        
        activeLayer = layer->getActiveLayer();
        
        for(int i = 0; i < layer->getLayerCount(); ++i)
        {
            double x = ( i < activeLayer ) ? -(LAYER_POSITION_X( fabs((float)activeLayer - i) )) : LAYER_POSITION_X( fabs((float)activeLayer - i) );
            double y = LAYER_POSITION_Y(x + centerX, centerX, centerY);
            
            CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY / 2,
                                                              ccp(x+centerX,y));
            CCFiniteTimeAction* scaleLayer = CCScaleTo::create(SYSTEM_ANIMATION_DELAY / 2,
                                                             SCALE_FUNCTION(x));
           // CCFiniteTimeAction* fadeLayer = CCFadeTo::create(GetGameParams->getVelocity() / 2,
                                                     //          LAYER_OPACITY(x));
           // CCSpawn *fadeAndScaleAction = CCSpawn::createWithTwoActions(fadeLayer, scaleLayer);
            
            layer->getLayer(i)->getMapLayer()->runAction(scaleLayer /*CCSpawn::createWithTwoActions(moveLayer, scaleLayer)*/);
            layer->getLayer(i)->runAction(moveLayer);
            layer->setLayerOpacity(i, LAYER_OPACITY(x));
        }
    }

}
