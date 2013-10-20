//
//  Task.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 2/22/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Task__
#define __pictomir_cocos2d_x__Task__

#include <libxml/xmlreader.h>
#include <cocos2d.h>
#include "GameParams.h"

class UniversalRobot;

class Task
{
    friend class XMLWorldSaver;
    UniversalRobot *robot;
    SYNTHESIZE(int, grassToPaint, GrassToPaint)
    SYNTHESIZE(int, paintedGrass, PaintedGrass)
    SYNTHESIZE(int, furnaceToBurn, FurnaceToBurn)
    SYNTHESIZE(int, burnedFurnace, BurnedFurnace)
    SYNTHESIZE(cocos2d::CCPoint, endPoint, EndPoint)
    SYNTHESIZE(int, endLayer, EndLayer)
public:
    Task (UniversalRobot *robot);
    
    static Task* create(UniversalRobot *robot)
    {        
        return new Task(robot);
    }
    
    bool Complete();
    
    void PaintedGrassInc() {
        paintedGrass++;
    }
    
    void PaintedGrassDec() {
        paintedGrass--;
    }
    
    void GrassToPaintInc() {
        grassToPaint++;
    }
    
    void BurnedFurnaceInc() {
        burnedFurnace++;
    }
    
    void FurnaceToBurnInc() {
        furnaceToBurn++;
    }
    
    void clear();
};

#endif /* defined(__pictomir_cocos2d_x__Task__) */
