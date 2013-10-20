//
//  MenuLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "MainMenuLayer.h"
#include "SelectMenuLayer.h"
#include "SettingsMenuLayer.h"
#include "GameLayer.h"
#include "utils.h"

#include "CCLocalizedString.h"

USING_NS_CC;

CCScene* MainMenuLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainMenuLayer *layer = MainMenuLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer,0,0);
    
    // return the scene
    return scene;
}


bool MainMenuLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    m_bKeypadEnabled=true;
    m_bTouchEnabled = true;
    
    GameParams *params = GameParams::getInstanse();
    
    //setOpacity(255 * 0.45);
    setPosition( ccp (0,0) );
//    
    CCSprite *backGround  = CCSprite::create(PICT_PATH "Background/background1.png");
    backGround->setPosition( ccp(GetGameParams->screenSize.width/2, GetGameParams->screenSize.height/2) );
    resizeSprite(backGround, GetGameParams->screenSize.width, GetGameParams->screenSize.height);
    addChild(backGround, -1);
    
    
#ifndef MAP_EDITOR
    bool continue_ = GetGameParams->getGameStarted();
    
    CCMenuItemFont *item0 = NULL;
    
    if (continue_) {
        item0 = CCMenuItemFont::create(CCLocalizedString("ContinueGame", "NotLocalized"), this, menu_selector(MainMenuLayer::continueGame));
        item0->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2 + 100) );
    }
    
    CCMenuItemFont *item1 = CCMenuItemFont::create(CCLocalizedString("NewGame", "NotLocalized"), this, menu_selector(MainMenuLayer::newGame));
    CCMenuItemFont *item1_33 = CCMenuItemFont::create(CCLocalizedString("NewNetGame", "NotLocalized"), this, menu_selector(MainMenuLayer::newNetGame));
    CCMenuItemFont *item1_66 = CCMenuItemFont::create(CCLocalizedString("JoinNetGame", "NotLocalized"), this, menu_selector(MainMenuLayer::joinNetGame));
    CCMenuItemFont *item2 = CCMenuItemFont::create(CCLocalizedString("SelectLevel", "NotLocalized"), this, menu_selector(MainMenuLayer::selectLevel));
    CCMenuItemFont *item3 = CCMenuItemFont::create(CCLocalizedString("Settings", "NotLocalized"), this, menu_selector(MainMenuLayer::settings));
    CCMenuItemFont *item4 = NULL;
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
    item4 = CCMenuItemFont::create(CCLocalizedString("Exit", "NotLocalized"), this, menu_selector(MainMenuLayer::exitGame));
    item4->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2 - 100) );
#endif
    
    item1->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2 + 50) );
    //item1_33->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2) );
    //item1_66->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2 - 50) );
    item2->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2) );
    item3->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2 - 50) );
    
    CCMenu *menu = CCMenu::create(item1, /*item1_33, item1_66,*/ item2, item3, item4, NULL);
    
    if(item0 != NULL)
        menu->addChild(item0);
#else
    CCMenuItemFont *item1 = CCMenuItemFont::create("Редактировать локальную версию", this, menu_selector(MainMenuLayer::openLocal));
    CCMenuItemFont *item2 = CCMenuItemFont::create("Скачать с FTP", this, menu_selector(MainMenuLayer::downloadFromFTP));
    CCMenuItemFont *item3 = CCMenuItemFont::create("Настройки", this, menu_selector(MainMenuLayer::showOptionsLayer));
    CCMenuItemFont *item4 = NULL;
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
    item4 = CCMenuItemFont::create("Выход", this, menu_selector(MainMenuLayer::exitGame));
    item4->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2 - 100) );
#endif
    
    item1->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2 + 50) );
    item2->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2) );
    item3->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2 - 50) );
    
    
    CCMenu *menu = CCMenu::create(item1, item2, item3, item4, NULL);
#endif
    
    menu->setPosition(CCPointZero);
    
    addChild(menu, 1);
    
    
    return true; 
}


void MainMenuLayer::newGame(CCObject *sender)
{
    GetGameParams->setWorldPart(0);
    GetGameParams->setLevel(1);
    GetGameParams->setContinueGame(false);
    
    CCTransitionFade* trans = CCTransitionFade::create( 1.5f * SYSTEM_ANIMATION_DELAY, GameLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainMenuLayer::newNetGame(CCObject *object)
{
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32  && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//    //removeAllChildrenWithCleanup(true);
//    
//    pm_server_info server("PIKTOMIR TEST SERVER", "TEST MAP", 2);
//    PM_Server_thread server_thread(server);
//    
//    server_thread.execute();
//    MainMenuLayer::joinNetGame(NULL);
//#endif
}

void MainMenuLayer::joinNetGame(CCObject *sender)
{
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32  && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//    GameParams *params = GameParams::getInstanse();
//    
//    removeAllChildrenWithCleanup(true);
//    
//    CCMenuItemFont *update_item = CCMenuItemFont::create(CCLocalizedString("Update servers", "NotLocalized"), this, menu_selector(MainMenuLayer::newNetGame));
//    update_item->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2 - 50) );
//    
//    CCMenu *menu = CCMenu::create(update_item, NULL);
//    menu->setPosition(CCPointZero);
//    
//    PM_Client client;
//    
//    client.find_servers();
//    
//    int movement = 0;
//    CCMenuItemFont *item;
//        pm_server_info server1("PIKTOMIR TEST SERVER 2", "TEST MAP 2 1", 4);
//    client.servers.insert(server1);
//    for(set<pm_server_info, Compare_server_info>::iterator it = client.servers.begin(); it != client.servers.end(); it++)
//    {
//        item = CCMenuItemFont::create(it->server_name_, this, menu_selector(MainMenuLayer::joinNetGame));
//        item->setPosition( ccp(params->screenSize.width / 2, params->screenSize.height / 2 + movement) );
//        movement += 50;
//    
//        addChild(item);
//    }
//    
//    addChild(menu, 1);
//#endif
}

void MainMenuLayer::continueGame(CCObject *sender)
{
    GetGameParams->setContinueGame(true);
    
    CCTransitionFade* trans = CCTransitionFade::create( 1.5f * SYSTEM_ANIMATION_DELAY, GameLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainMenuLayer::selectLevel(CCObject *sender)
{
    CCTransitionSlideInR* trans = CCTransitionSlideInR::create( 1.5f * SYSTEM_ANIMATION_DELAY, SelectMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainMenuLayer::settings(CCObject *sender)
{
    CCTransitionSlideInR* trans = CCTransitionSlideInR::create( 1.5f * SYSTEM_ANIMATION_DELAY, SettingsMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainMenuLayer::exitGame(CCObject *sender)
{
    removeAllChildrenWithCleanup(true);
    exit(0);
}

#ifdef MAP_EDITOR
void MainMenuLayer::showOptionsLayer(CCObject *sender)
{
    CCTransitionSlideInR* trans = CCTransitionSlideInR::create( 1.5f * SYSTEM_ANIMATION_DELAY, OptionsLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainMenuLayer::downloadFromFTP(cocos2d::CCObject *sender) {
    if (MapEditorGameLayer::getInstanse()->FTPUser) {
        if (MapEditorGameLayer::getInstanse()->FTPUser->downloadMap()) {
            system(("rm -rf "+writablePath + "Maps/").c_str());
            
            
            rmdir((writablePath + "Maps/").c_str());
            printf("%s",(writablePath + "Maps").c_str());
            platform_mkdir((writablePath + "Maps").c_str());
            if(unzipFile((writablePath + "Maps.zip").c_str(),writablePath + "Maps/" ) == 0)
            {
                openLocal (NULL);
            }
            else
            {
                CCMessageBox ("Ошибка при распаковке карт", "Ошибка при распаковке карт");
            }
        }
        else
        {
            CCMessageBox ("Произошла ошибка скачивания", "Проверьте интернет-соединение");
        }
    }
    else
    {
        CCMessageBox ("Введите Логин/Пароль", "Введите Логин/Пароль");
        showOptionsLayer(NULL);
    }
}

void MainMenuLayer::openLocal (cocos2d::CCObject *sender)
{
    if (CCFileUtils::sharedFileUtils()->isFileExist(writablePath+"Maps/Worlds.xml"))
    {
        GameParams::getInstanse()->loadWorlds();
        
        CCTransitionSlideInR* trans = CCTransitionSlideInR::create( 1.5f * SYSTEM_ANIMATION_DELAY, SelectMenuLayer::scene());
        CCDirector::sharedDirector()->replaceScene(trans);
    } else
    {
        CCMessageBox ("Локальная копия отсутсвует", "Локальная копия отсутсвует");
    }
    
}
#endif

