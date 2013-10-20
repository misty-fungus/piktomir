//
//  Map.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 16.12.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#include "Map.h"
#include "ScrollLayer.h"
#include "PlayerRobot.h"
#include "UniversalWorld.h"

USING_NS_CC;

void Map::setLayerOpacity(int layer, int opacity)
{
    for(int i = 0; i < layers[layer]->getMapLayer()->getChildrenCount(); ++i)
        ((CCSprite *)layers[layer]->getMapLayer()->getChildren()->objectAtIndex(i))->setOpacity(opacity);
}

void Map::setEnabled(bool flag)
{
    enabled = flag;
    scroll->setActive(flag);
}

void Map::scrollToInitialPosition()
{
//    GameParams *params = GameParams::getInstanse();
    
//    double centerX = params->mapSize().width/2;
//    double centerY = params->mapSize().height/2;
    
    
//    for(int i = 0; i <= DISPLAYED_LAYERS/2 && (activeLayer + i) < layers.size(); ++i)
//    {
//        float x = LAYER_POSITION_X(i);
//        float y = LAYER_POSITION_Y(x + centerX, centerX, centerY) ;
//        double scale = SCALE_FUNCTION(x);
//        
//        scroll->reorderChild(layers[activeLayer + i], activeLayer + DISPLAYED_LAYERS/2 + i);
//        
//        CCFiniteTimeAction* moveLayer = CCMoveTo::create(GetGameParams->getVelocity() / 2,
//                                                         ccp(x + centerX,y));
//        CCFiniteTimeAction* scaleLayer = CCScaleTo::create(GetGameParams->getVelocity() / 2,
//                                                           scale);
//        
//        CCFiniteTimeAction* moveEnd = CCCallFuncN::create( this,
//                                                          callfuncN_selector(Map::update));
//        
//        setLayerOpacity(activeLayer + i, LAYER_OPACITY(x) );
//        
//        layers[activeLayer + i]->getMapLayer()->runAction( scaleLayer );
//        layers[activeLayer + i]->runAction( CCSequence::create(moveLayer, moveEnd, NULL) );
//        
//        
//    }
//    
//    for(int i = 1; i <= DISPLAYED_LAYERS/2 && activeLayer - i >= 0; ++i)
//    {
//        
//        float x =  -(LAYER_POSITION_X(i));
//        float y = LAYER_POSITION_Y(x + centerX, centerX, centerY) ;
//        double scale = SCALE_FUNCTION(x);
//        
//        scroll->reorderChild(layers[activeLayer - i], activeLayer + DISPLAYED_LAYERS/2 - i);
//        
//        CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY / 2,
//                                                         ccp(x + centerX,y));
//        CCFiniteTimeAction* scaleLayer = CCScaleTo::create(SYSTEM_ANIMATION_DELAY / 2,
//                                                           scale);
//        
//        CCFiniteTimeAction* moveEnd = CCCallFuncN::create( this,
//                                                          callfuncN_selector(Map::update));
//        
//        layers[activeLayer - i]->getMapLayer()->runAction( scaleLayer );
//        layers[activeLayer - i]->runAction( CCSequence::create(moveLayer, moveEnd, NULL) );
//        
//        setLayerOpacity(activeLayer - i, LAYER_OPACITY(x) );
//        
//    }
    
    CCFiniteTimeAction* moveLayer = CCMoveTo::create(SYSTEM_ANIMATION_DELAY / 2,
                                                     CCPointZero);
    
    CCFiniteTimeAction* moveEnd = CCCallFunc::create( this,
                                                      callfunc_selector(Map::update));
    
    scroll->getParallax()->runAction( CCSequence::create(moveLayer, moveEnd, NULL) );
}

/*void Map::visit()
{
    CCRect scissorRect = CCRect(0, 0, GetGameParams->mapSize().width, GetGameParams->mapSize().height);
     
    kmGLPushMatrix();
    glEnable(GL_SCISSOR_TEST);
    
    CCEGLView::sharedOpenGLView()->setScissorInPoints(scissorRect.origin.x, scissorRect.origin.y,
                                                      scissorRect.size.width, scissorRect.size.height);
    
    CCNode::visit();
    glDisable(GL_SCISSOR_TEST);
    kmGLPopMatrix();
    
}*/