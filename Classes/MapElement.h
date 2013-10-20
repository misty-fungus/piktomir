//
//  Map4Element.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Map4Element__
#define __pictomir_cocos2d_x__Map4Element__

#include <iostream>
#include "cocos2d.h"
#include "GameParams.h"

class UniversalRobot;

class MapElement
{
public:
    
    MapElement():
        type(Grass),
        isRobotHere(false),
        robotHere(NULL),
        envRobotHere(NULL),
        isPainted(false),
        nearElements(NULL),
        wallZOrder(0),
        grassSprite(NULL),
        wallSprite(NULL)
    {}
    
    static MapElement* create()
    {
        return new MapElement;
    }
    
    MapElement(int dimention):
        type(Grass),
        isRobotHere(false),
        robotHere(NULL),
        envRobotHere(NULL),
        isPainted(false),
        nearElements(new MapElement *[dimention]),
        wallZOrder(0),
        grassSprite(NULL),
        wallSprite(NULL)
    {}
    
    static MapElement* create(int dimention)
    {
        return new MapElement(dimention);
    }
    
    
    enum Type {
        None = -1,
        Grass,//0
        //Wall,
        Water,//1
        GreenGrass,//2
        BlueGrass//3
    };
    
    SYNTHESIZE(Type, type, Type);
    SYNTHESIZE(int, wallZOrder, WallZOrder);
    
    bool isRobotHere;    
    UniversalRobot *robotHere;
    UniversalRobot *envRobotHere;
    
    bool isPainted;
    
    MapElement **nearElements;
    
    SYNTHESIZE(cocos2d::CCSprite *, grassSprite, GrassSprite);
    SYNTHESIZE(cocos2d::CCSprite *, wallSprite, WallSprite);
    
    
    void setElement(int);
    void resize(float scale);
};


#endif /* defined(__pictomir_cocos2d_x__Map4Element__) */
