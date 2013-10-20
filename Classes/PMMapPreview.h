//
//  PMMapPreview.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 01.08.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__PMMapPreview__
#define __pictomir_cocos2d_x__PMMapPreview__

#include <iostream>
#include "cocos2d.h"

const int PMMapPreviewWidth = 200;
const int PMMapPreviewHeight = 200;
const int PMMapPreviewOffset = 10;

class PMMapPreview : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    virtual bool initWithWorldMap (int world, int map);
    static PMMapPreview* create(int world, int map);
    
    static PMMapPreview* create();
    
    PMMapPreview () :
        world(0),
        map(0)
    {
        
    }
    
    ~PMMapPreview()
    {
        removeAllChildrenWithCleanup(true);
    }
private:
    int map;
    int world;
    
    virtual void visit();
};

#endif /* defined(__pictomir_cocos2d_x__PMMapPreview__) */
