//
//  Map.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 16.12.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Map__
#define __pictomir_cocos2d_x__Map__

#include <iostream>

#include "MapElement.h"
#include "cocos2d.h"
#include "MapContainer.h"

class UniversalWorld;
class ScrollLayer;

enum MapType
{
    pmMap,
    pmMap4
};

typedef std::vector<std::vector<MapElement *> > Element;

class Map : public cocos2d::CCLayer
{
    friend class XmlWorldReader;
    friend class XMLWorldSaver;
    
public :
    Map():
        _world(NULL)
    {}
    
    Map(UniversalWorld *world):
        _world(world)
    {}
    
    virtual ~Map()
    {
        for(int i = 0; i < mapElements.size(); ++i)
        {
            for(int j = 0; j < mapElements[i].size(); ++j)
                for(int k = 0; k < mapElements[i][j].size(); ++k)
                CC_SAFE_DELETE(mapElements[i][j][k]);
        }
        
        removeAllChildrenWithCleanup(true);
        
        cocos2d::CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    }
    
    SYNTHESIZE(UniversalWorld *, _world, World);
public:    
    std::vector< Element > mapElements;
    
    std::vector<MapContainer *> layers;
    
    void setLayerOpacity(int layer, int opacity);
    
    virtual void drawMap() = 0;
    virtual void update() = 0;
    virtual void clean() = 0;
    virtual void centerMap() = 0;
    
    
    MapContainer *getLayer() const
    {
        return layers[activeLayer];
    }
    
    int getLayerCount() const
    {
        return layers.size();
    }
    
    MapContainer *getLayer(int i) const
    {
        return layers[i];
    }

    virtual void setEnabled(bool flag);
    
    virtual MapType type()
    {
        return pmMap;
    }
    
    bool isEnabled()
    {
        return enabled;
    }
    
    void  scrollToInitialPosition();
    
    virtual float getRealHeight() const = 0;
    virtual float getRealWidth() const = 0;
protected:
    SYNTHESIZE(ScrollLayer *,scroll,Scroll);
    SYNTHESIZE(int, activeLayer, ActiveLayer);
    
    bool enabled;
    
//    virtual void visit();
};

#endif /* defined(__pictomir_cocos2d_x__Map__) */
