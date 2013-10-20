//
//  OptionsLayer.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 19.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__OptionsLayer__
#define __pictomir_cocos2d_x__OptionsLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;

class OptionsLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
    
    CREATE_FUNC(OptionsLayer);
    
    static cocos2d::CCScene* scene();
private:
    CCEditBox * login;
    CCEditBox * pass;
    
    void mainMenu(CCObject *sender);
    void OK(CCObject *object);
};

#endif /* defined(__pictomir_cocos2d_x__OptionsLayer__) */
