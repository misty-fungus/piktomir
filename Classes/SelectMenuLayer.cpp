//
//  SelectMenuLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "SelectMenuLayer.h"
#include "SelectMapMenuLayer.h"
#include "utils.h"
#include "GameParams.h"
#include "MainMenuLayer.h"

USING_NS_CC;

CCScene* SelectMenuLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SelectMenuLayer *layer = SelectMenuLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer,0,0);
    
    // return the scene
    return scene;
}

bool SelectMenuLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    m_bKeypadEnabled=true;
    m_bTouchEnabled = true;
    
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    
    setPosition( ccp (0,0) );
    
    CCSprite *backGround  = CCSprite::create(PICT_PATH "Background/background1.png" );
    backGround->setPosition( ccp(screenSize.width/2, screenSize.height/2) );
    resizeSprite(backGround, screenSize.width, screenSize.height);
    addChild(backGround, -1);
    
    CCMenu *navigationMenu = CCMenu::create(NULL);
    worldSelectMenu = CCMenu::create();
    
    
    

    worldSelectMenu->setPosition( ccp(0, 50 * GetGameParams->worlds.size() / 2) );
    for (int i = 0; i < GetGameParams->worlds.size(); ++i) {
        CCMenuItemFont * world_ = CCMenuItemFont::create(
                                                         CCString::createWithFormat(CCLocalizedString("WorldPattern", "NotLocalized"), i+1)->m_sString.c_str(), this, menu_selector(SelectMenuLayer::setWorld)
                                                         );
        world_->setPosition( ccp(screenSize.width / 2, screenSize.height / 2 - i*50) );
        worldSelectMenu->addChild(world_);
    }

    
    
#ifdef MAP_EDITOR
    plusButton = CCMenuItemFont::create("+", this, menu_selector(SelectMenuLayer::addWorld));
    plusButton->setPosition( ccp(screenSize.width / 2, screenSize.height / 2 - GetGameParams->worlds.size()*50) );
    worldSelectMenu->addChild(plusButton,1);
#endif
    
    addChild(worldSelectMenu);
    
#ifdef MAP_EDITOR
    CCMenuItemFont *uploadToServer = CCMenuItemFont::create("Загрузить на сервер", this, menu_selector(SelectMenuLayer::uploadMapsToServer));
    uploadToServer->setPosition( ccp (170, screenSize.height - 75) );
    uploadToServer->setColor(ccc3(20, 200, 20));
    navigationMenu->addChild(uploadToServer);
#endif
    
    CCMenuItemFont *goToMain = CCMenuItemFont::create(CCLocalizedString("BackToMainMenu", "NotLocalized"), this, menu_selector(SelectMenuLayer::mainMenu));
    goToMain->setPosition( ccp (170, screenSize.height - 25) );
    
    navigationMenu->addChild(goToMain);
    navigationMenu->setPosition( CCPointZero );
    addChild(navigationMenu, 1);
    
    
    return true;
}

void SelectMenuLayer::setWorld(CCObject *sender)
{
    for(int i = 0; i < worldSelectMenu->getChildrenCount(); ++i) {
        if(worldSelectMenu->getChildren()->objectAtIndex(i) == sender) {
            GetGameParams->setWorldPart(i);
            
            CCTransitionSlideInR* trans = CCTransitionSlideInR::create( 1.5f * SYSTEM_ANIMATION_DELAY, SelectMapMenuLayer::scene());
            CCDirector::sharedDirector()->replaceScene(trans);
            break;
        }
    }
}

void SelectMenuLayer::mainMenu(CCObject *object)
{
    CCTransitionSlideInL* trans = CCTransitionSlideInL::create( 1.5f * SYSTEM_ANIMATION_DELAY, MainMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

#ifdef MAP_EDITOR
void SelectMenuLayer::addWorld (CCObject *object)
{
    worldSelectMenu->setPosition( ccp(0, 50 * GetGameParams->worlds.size() / 2) );
    
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    CCMenuItemFont * world_ = CCMenuItemFont::create(CCString::createWithFormat("Мир %ld", GetGameParams->worlds.size()+1)->m_sString.c_str(), this, menu_selector(SelectMenuLayer::setWorld));
    world_->setPosition( ccp(screenSize.width / 2, screenSize.height / 2 - GetGameParams->worlds.size()*50) );
    worldSelectMenu->addChild(world_);
    
    GameParams::getInstanse()->worlds.push_back(0);
    
    plusButton->setPosition( ccp(screenSize.width / 2, screenSize.height / 2 - GetGameParams->worlds.size()*50) );
    
    char tmp[100] = "";
    sprintf(tmp, "%sMaps/World%ld",writablePath.c_str(),GameParams::getInstanse()->worlds.size()-1);
    
    platform_mkdir(tmp);
    
    MapEditorGameLayer::getInstanse()->mapVer++;
    MapEditorGameLayer::getInstanse()->saveWorldsetLocal();
}

void SelectMenuLayer::uploadMapsToServer (CCObject *sender)
{
    MapEditorGameLayer::getInstanse()->uploadMapsToServer();
}
#endif
