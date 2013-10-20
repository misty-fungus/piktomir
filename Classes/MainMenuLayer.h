//
//  MenuLayer.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__MenuLayer__
#define __pictomir_cocos2d_x__MenuLayer__

#include <iostream>
#include "cocos2d.h"

#include "NetEngine.h"

#ifdef MAP_EDITOR
#include "mapEditorsMenu.h"
#include "OptionsLayer.h"
#include "MapEditorGameLayer.h"
#endif

class MainMenuLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
    
    CREATE_FUNC(MainMenuLayer);
    
    static cocos2d::CCScene* scene();
    
    ~MainMenuLayer()
    {
        removeAllChildrenWithCleanup(true);
    }
    
private:
        
    void newGame(CCObject *sender);
    void newNetGame(CCObject *sender);
    void joinNetGame(CCObject *sender);
    void continueGame(CCObject *sender);
    void selectLevel(CCObject *sender);
    void settings(CCObject *sender);
    void exitGame(CCObject *sender);
    
#ifdef MAP_EDITOR
    void showOptionsLayer (CCObject *sender);
    void downloadFromFTP (CCObject * sender);
    void openLocal (cocos2d::CCObject *sender);
#endif
};


#endif /* defined(__pictomir_cocos2d_x__MenuLayer__) */
