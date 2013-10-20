//
//  SelectMenuLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__SelectMenuLayer__
#define __pictomir_cocos2d_x__SelectMenuLayer__

#include <iostream>
#include "cocos2d.h"


class SelectMenuLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
    
    CREATE_FUNC(SelectMenuLayer);
    
    static cocos2d::CCScene* scene();
    
    ~SelectMenuLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
private:
    cocos2d::CCMenu *worldSelectMenu;
    
    void setWorld(CCObject *sender);
    void mainMenu(CCObject *sender);

#ifdef MAP_EDITOR
    void addWorld (CCObject *object);
    void uploadMapsToServer (CCObject *sender);
    
    cocos2d::CCMenuItemFont * plusButton;
#endif
};


#endif /* defined(__pictomir_cocos2d_x__SelectMenuLayer__) */
