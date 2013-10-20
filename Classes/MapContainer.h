//
//  Map4Container.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/2/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Map4Container__
#define __pictomir_cocos2d_x__Map4Container__

#include <iostream>

#include <iostream>
#include "cocos2d.h"
#include "GameParams.h"

class MapContainer : public cocos2d::CCLayerColor
{
    public :
    MapContainer() :
        cocos2d::CCLayerColor(),
        mapLayer(NULL)
    {}
    
    CREATE_FUNC(MapContainer);

    MapContainer(cocos2d::CCLayer *mapLayer) :
        cocos2d::CCLayerColor(),
        mapLayer(mapLayer)
    {

        cocos2d::CCPoint center = cocos2d::CCPointMake(mapLayer->getContentSize().width/2,mapLayer->getContentSize().height/2);
        
        init();
        setScaleY(0.5);
        setAnchorPoint( ccp(0,0) );
        setPosition(cocos2d::CCPointZero);
        
        mapLayer->setRotation( 45.0f );
        
        mapLayer->setAnchorPoint( ccp(0,0) );
        mapLayer->setPosition(ccp(0, 0));
        
        center = CCPointApplyAffineTransform(center,mapLayer->nodeToParentTransform());
       
        mapLayer->setPosition(ccp(-center.x,-center.y));
        
        addChild(mapLayer);
    }
    
    ~MapContainer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
    static MapContainer* create(cocos2d::CCLayer *_mapLayer)
    {
        MapContainer *container = new MapContainer(_mapLayer);
        
        container->init();
        container->autorelease();
        
        return container;
    }
    
protected:
    SYNTHESIZE(cocos2d::CCLayer *, mapLayer, MapLayer);
public:
 
};


#endif /* defined(__pictomir_cocos2d_x__Map4Container__) */
