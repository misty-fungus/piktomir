//
//  MapEditorGameLayer.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 10.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__MapEditorGameLayer__
#define __pictomir_cocos2d_x__MapEditorGameLayer__

#include "GameLayer.h"
#include "FTPuser.h"

class MapEditorGameLayer : public GameLayer {
public:
    std::string FTPLogin;
    std::string FTPPass;
    FTPuser * FTPUser;
    int mapVer;
    
public:
    bool editing;
    static MapEditorGameLayer *_instanse;
    static MapEditorGameLayer *getInstanse();
    
    static cocos2d::CCScene* scene_();
    virtual bool init();
    //CREATE_FUNC(MapEditorGameLayer);

    void reloadWorld();
    
    bool changeMapSize (int width, int height);
    bool changeMapLayers (int layers);
    
    void setEdit (cocos2d::CCObject * sender) {
        editing = !editing;
        if(!editing) ((cocos2d::CCMenuItemSprite*) sender)->setNormalImage(PMTextureCache::instanse()->getEditIconSprite());
        else ((cocos2d::CCMenuItemSprite*) sender)->setNormalImage(PMTextureCache::instanse()->getEditIconSprite_Selected());
    }
    
    void saveWorldsetLocal ();
    void uploadMapsToServer ();
    
    void setEnabled(bool flag) {
        GameLayer::setEnabled(flag);
        if(pauseButton)pauseButton->setEnabled(flag);
    }
};
#endif /* defined(__pictomir_cocos2d_x__MapEditorGameLayer__) */
