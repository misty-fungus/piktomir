//
//  HintLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/10/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__HintLayer__
#define __pictomir_cocos2d_x__HintLayer__

#include <iostream>
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class HintLayer : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    
    //CREATE_FUNC(HintLayer);
   
    HintLayer(std::string text):
        cocos2d::CCLayerColor(),
        text(text)
    {}
    
    static HintLayer *create(std::string text);
    static int hintCount;
    
    ~HintLayer();
private:
    std::string text;
};
#ifdef MAP_EDITOR
class HintEditLayer : public cocos2d::CCLayerColor
{
public:
    CCEditBox* hint;
    
    virtual bool init();
    
    HintEditLayer(std::string text, std::string placeHolder):
    cocos2d::CCLayerColor(),
    text(text),
    placeHolder(placeHolder)
    {}
    
    static HintEditLayer *create(std::string text,std::string placeHolder);
    static int hintCount;
    
    ~HintEditLayer();
private:
    std::string text;
    std::string placeHolder;
};
#endif

#endif /* defined(__pictomir_cocos2d_x__HintLayer__) */
