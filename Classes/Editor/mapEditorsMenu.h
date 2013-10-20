//
//  mapEditorsMenu.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 11.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__mapEditorsMenu__
#define __pictomir_cocos2d_x__mapEditorsMenu__

#include "cocos2d.h"
#include "GameParams.h"
#include "PMTextureCache.h"
USING_NS_CC;

enum PMMap4Edit {
    None = -1,
    EditingNone4,
    EditingGrass4,
    EditingWater4,
    EditingPaintingGrass4,
    EditingPaintedGrass4,
    
    EditingLeftWall4,
    EditingUpWall4,
    EditingNoLeftWall4,
    EditingNoUpWall4
};

class map4EditorsMenu : public cocos2d::CCObject {
    SYNTHESIZE(PMMap4Edit, active, Active );
    
    SYNTHESIZE(cocos2d::CCMenu*, editMenu, EditMenu);
    
    cocos2d::CCMenuItemSprite * editors[9];
    cocos2d::CCMenuItemSprite * activeEditor;
    
public:
    static map4EditorsMenu* instanse;
    static map4EditorsMenu* getInstanse();
    
    map4EditorsMenu () {
        active = None;
        editMenu = cocos2d::CCMenu::create();
        
        editMenu->setScaleY(0.5);
        
        activeEditor = cocos2d::CCMenuItemSprite::create(CCSprite::create("MapsPics/Active.png"), CCSprite::create("MapsPics/Active.png"), CCSprite::create("MapsPics/Active_Disabled.png"), NULL, NULL);
        activeEditor->setScaleY(2);
        activeEditor->setPosition(ccp(1000,1000));
        editMenu->addChild(activeEditor);
        
        editors[0] = cocos2d::CCMenuItemSprite::create(PMTextureCache::instanse()->getMapEditGrassTile(MapElement::None), PMTextureCache::instanse()->getMapEditGrassTile(MapElement::None),PMTextureCache::instanse()->getMapGrassDisabledTile(MapElement::None),this, menu_selector(map4EditorsMenu::selectAction));
        editors[0]->setPosition(ccp(0,0));
        editors[0]->setRotation(45);
        editMenu->addChild(editors[0]);
        
        editors[1] = cocos2d::CCMenuItemSprite::create(PMTextureCache::instanse()->getMapEditGrassTile(MapElement::Grass), PMTextureCache::instanse()->getMapEditGrassTile(MapElement::Grass),PMTextureCache::instanse()->getMapGrassDisabledTile(MapElement::Grass),this, menu_selector(map4EditorsMenu::selectAction));
        editors[1]->setPosition(ccp(100,0));
        editors[1]->setRotation(45);
        editMenu->addChild(editors[1]);
        
        editors[2] = cocos2d::CCMenuItemSprite::create(PMTextureCache::instanse()->getMapEditGrassTile(MapElement::Water), PMTextureCache::instanse()->getMapEditGrassTile(MapElement::Water),PMTextureCache::instanse()->getMapGrassDisabledTile(MapElement::Water),this, menu_selector(map4EditorsMenu::selectAction));
        editors[2]->setPosition(ccp(200,0));
        editors[2]->setRotation(45);
        editMenu->addChild(editors[2]);
        
        editors[3] = cocos2d::CCMenuItemSprite::create(PMTextureCache::instanse()->getMapEditGrassTile(MapElement::GreenGrass), PMTextureCache::instanse()->getMapEditGrassTile(MapElement::GreenGrass),PMTextureCache::instanse()->getMapGrassDisabledTile(MapElement::GreenGrass),this, menu_selector(map4EditorsMenu::selectAction));
        editors[3]->setPosition(ccp(0,-100));
        editors[3]->setRotation(45);
        editMenu->addChild(editors[3]);
        
        editors[4] = cocos2d::CCMenuItemSprite::create(PMTextureCache::instanse()->getMapEditGrassTile(MapElement::BlueGrass), PMTextureCache::instanse()->getMapEditGrassTile(MapElement::BlueGrass),PMTextureCache::instanse()->getMapGrassDisabledTile(MapElement::BlueGrass),this, menu_selector(map4EditorsMenu::selectAction));
        editors[4]->setPosition(ccp(100,-100));
        editors[4]->setRotation(45);
        editMenu->addChild(editors[4]);
        
        editors[5] = cocos2d::CCMenuItemSprite::create(PMTextureCache::instanse()->getMapEditWallTile(0), PMTextureCache::instanse()->getMapEditWallTile(0),PMTextureCache::instanse()->getMapWallDisabledTile(0),this, menu_selector(map4EditorsMenu::selectAction));
        editors[5]->setPosition(ccp(200,-110));
        editors[5]->setRotation(45);
        editMenu->addChild(editors[5]);
        
        editors[6] = cocos2d::CCMenuItemSprite::create(PMTextureCache::instanse()->getMapEditWallTile(1), PMTextureCache::instanse()->getMapEditWallTile(1),PMTextureCache::instanse()->getMapWallDisabledTile(1),this, menu_selector(map4EditorsMenu::selectAction));
        editors[6]->setPosition(ccp(0,-220));
        editors[6]->setRotation(45);
        editMenu->addChild(editors[6]);
        
        CCSprite * noLeftWall = PMTextureCache::instanse()->getMapEditWallTile(0);
        CCSprite * no = CCSprite::create("MapsPics/No.png");
        no->setPosition(CCPointZero);
        no->setAnchorPoint(CCPointZero);
        noLeftWall->addChild(no);
        
        CCSprite * noUpWall = PMTextureCache::instanse()->getMapEditWallTile(1);
        no = CCSprite::create("MapsPics/No.png");
        no->setPosition(CCPointZero);
        no->setAnchorPoint(CCPointZero);
        noUpWall->addChild(no);
        
        CCSprite * noLeftWall_Disabled = PMTextureCache::instanse()->getMapWallDisabledTile(0);
        no = CCSprite::create("MapsPics/No_Disabled.png");
        no->setPosition(CCPointZero);
        no->setAnchorPoint(CCPointZero);
        noLeftWall_Disabled->addChild(no);
        CCSprite * noUpWall_Disabled = PMTextureCache::instanse()->getMapWallDisabledTile(1);
        no = CCSprite::create("MapsPics/No_Disabled.png");
        no->setPosition(CCPointZero);
        no->setAnchorPoint(CCPointZero);
        noUpWall_Disabled->addChild(no);
        
        editors[7] = cocos2d::CCMenuItemSprite::create(noLeftWall, noLeftWall, noLeftWall_Disabled,this, menu_selector(map4EditorsMenu::selectAction));
        editors[7]->setPosition(ccp(100,-220));
        editors[7]->setRotation(45);
        editMenu->addChild(editors[7]);
        
        editors[8] = cocos2d::CCMenuItemSprite::create(noUpWall, noUpWall, noUpWall_Disabled,this, menu_selector(map4EditorsMenu::selectAction));
        editors[8]->setPosition(ccp(200,-220));
        editors[8]->setRotation(45);
        editMenu->addChild(editors[8]);
        
        editMenu->retain();
    }
    
    void selectAction (cocos2d::CCObject * sender) {
        
        
        PMMap4Edit tmp = None;
        for (int i = 0; i < 9; ++i) {
            if (sender == editors[i]) {
                tmp = (PMMap4Edit)i;
                break;
            }
        }
        
        if (active == tmp) {
            active = None;
            activeEditor->setPosition(ccp(1000,1000));
        } else {
            active = tmp;
            activeEditor->setPosition(((CCMenuItemImage*)sender)->getPosition());
        }
    }
    
    void setEnabled (bool flag) {
        for (int i = 0; i < 9; ++i) {
            editors[i]->setEnabled(flag);
        }
        activeEditor->setEnabled(flag);
    }
};

#endif /* defined(__pictomir_cocos2d_x__mapEditorsMenu__) */
