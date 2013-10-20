//
//  Map4.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Map4__
#define __pictomir_cocos2d_x__Map4__

#include <iostream>
#include "Map.h"
#include <vector>
#include <libxml/xmlreader.h>
#include "utils.h"
#include <string.h>
#include "ProgramLayer.h"
#include "Task.h"
#include "Hints.h"
#include "ControlLayer4.h"
#include "PMTextureCache.h"


#ifdef MAP_EDITOR
#include "mapEditorsMenu.h"
#endif

class World4;

enum Map4Tiles
{
    MAP4_ROBOT_START = 1,
    MAP4_GREEN_GRASS = 2,
    MAP4_BLUE_GRASS = 3,
    MAP4_UNPAINT_GRASS = 4,
    MAP4_PAINT_GRASS = 5,

    MAP4_ST2_WALL = 7,
    MAP4_ST3_WALL = 6,
    MAP4_ST23_WALL = 8
};

class Map4 : public Map
{
    friend class XmlWorldReader;
    friend class XMLWorldSaver;
    
public:
    
    enum Direction
    {
        Down = 0,
        Right,
        Up,
        Left
    };
    
    Map4() :
        Map(),
        mapElementSize(GameParams::getInstanse()->mapTileSize),
        paintedGrassCount(0),
        width(0),
        height(0),
        tileSet(0),
        grassCount(0)
    {}
    
    
    Map4(World4* parent,int width, int height, int layerNum, int tileset):
        Map((UniversalWorld *)parent),
        mapElementSize(cocos2d::CCSizeMake(64,64)),
        paintedGrassCount(0),
        width(width),
        height(height),
        tileSet(tileset),
        grassCount(0)
    {
        mapElements.resize(layerNum);
    }
    
    static Map4 *create();
    static Map4 *create(World4* parent,int width, int height, int layerNum, int tileset);
    
    ~Map4()
    {

    }
    
    cocos2d::CCSize mapElementSize;
    
    SYNTHESIZE(int, paintedGrassCount, PaintedGrassCount);
    SYNTHESIZE(int, width, Width);
    SYNTHESIZE(int, height, Height);
    SYNTHESIZE(int, tileSet, TileSet);
    SYNTHESIZE(int, grassCount, GrassCount);
    SYNTHESIZE(ControlLayer4 *,controlLayer,ControlLayer);
    
    bool init();
    
    virtual void drawMap();
    virtual void update();
    virtual void clean();
    virtual void centerMap();
    
    virtual float getRealWidth() const {
        return width * mapElementSize.width;
    }
    
    virtual float getRealHeight() const {
        return height * mapElementSize.height;
    }
    
    World4 *world() {
        return (World4 *)_world;
    }
    
    void incPaintedGrass() {
        ++paintedGrassCount;
    }
    
    cocos2d::CCPoint Coordinates(int x, int y, double dx, double dy);
    
    void incGrassCount()
    {
        ++grassCount;
    }
    
    virtual void setEnabled(bool flag);
    
    virtual MapType type()
    {
        return pmMap4;
    }
    
#ifdef MAP_EDITOR
    CCSprite* resizeSprite;
    
    void resize (int width_, int height_);
    void changeLayers (int layers);
    void deleteMapElement (MapElement* mapElement,int layer);
    void drawMapElement (MapElement* mapElement,int layer);
    
    void changeElementTypeAtPoint (cocos2d::CCPoint, int layer);
    cocos2d::CCPoint dxdyAtCcp(cocos2d::CCPoint point);
    cocos2d::CCPoint coordAtCcp(cocos2d::CCPoint point);
    
    int resizeAtTouch(cocos2d::CCPoint point);
#endif
    
private:    
    int getWallZOrder(int x, int y, int layer);
};

#endif /* defined(__pictomir_cocos2d_x__Map4__) */
