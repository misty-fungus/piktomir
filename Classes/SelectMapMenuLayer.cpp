//
//  SelectMapMenuLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "SelectMapMenuLayer.h"
#include "GameLayer.h"
#include "utils.h"
#include "MainMenuLayer.h"
#include "SelectMenuLayer.h"
#include "ScrollLayer.h"
#include "PMMapPreview.h"

CCScene* SelectMapMenuLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SelectMapMenuLayer *layer = SelectMapMenuLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer,0,0);
    
    // return the scene
    return scene;
}

bool SelectMapMenuLayer::init()
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
    
    int worldPart = GetGameParams->getWorldPart();
    
    CCLabelTTF *worldNumber = CCLabelTTF::create(CCString::createWithFormat(CCLocalizedString("WorldPattern", "NotLocalized"), worldPart+1)->m_sString.c_str(), "Marker Felt", 32);
    worldNumber->setPosition( ccp(screenSize.width / 2, screenSize.height / 2 + 50 + PMMapPreviewHeight / 2) );
    addChild(worldNumber,1);
    
    scroll = CCScrollView::create();
#ifdef MAP_EDITOR
    scroll->setViewSize( CCSizeMake(MIN(screenSize.width, (GetGameParams->worlds[worldPart]+1)*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset), PMMapPreviewHeight) );
    scroll->setPosition( ccp(MAX(0, screenSize.width / 2 - ((GetGameParams->worlds[worldPart]+1)*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset) / 2),screenSize.height / 2 - PMMapPreviewHeight / 2) );
    scroll->setContentSize( CCSizeMake((GetGameParams->worlds[worldPart]+1)*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, PMMapPreviewHeight) );
#else
    scroll->setViewSize( CCSizeMake(MIN(screenSize.width, GetGameParams->worlds[worldPart]*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset), PMMapPreviewHeight) );
    scroll->setPosition( ccp(MAX(0, screenSize.width / 2 - (GetGameParams->worlds[worldPart]*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset) / 2),screenSize.height / 2 - PMMapPreviewHeight / 2) );
    scroll->setContentSize( CCSizeMake(GetGameParams->worlds[worldPart]*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, PMMapPreviewHeight) );
#endif
    scroll->setContentOffset( ccp(0, 0), true);
    scroll->setClippingToBounds(true);
    scroll->setDirection(kCCScrollViewDirectionHorizontal);
    scroll->setZoomScale(0.5);
    scroll->setBounceable(true);
    
    for (int i = 0; i < GetGameParams->worlds[worldPart]; ++i) {
        PMMapPreview * preview = PMMapPreview::create(worldPart, i+1);
        preview->setPosition(ccp(i*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
        scroll->addChild(preview);
    }
    
#ifdef MAP_EDITOR
    setTouchPriority(-150);
    dragPreview = NULL;
    ISdrag = false;
    addMapPreview = PMMapPreview::create();
    addMapPreview->setPosition(ccp(GetGameParams->worlds[worldPart]*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
    scroll->addChild(addMapPreview);
#endif
    
    addChild(scroll);
    
#ifdef MAP_EDITOR
    CCMenuItemFont *uploadToServer = CCMenuItemFont::create("Загрузить на сервер", this, menu_selector(SelectMapMenuLayer::uploadMapsToServer));
    uploadToServer->setPosition( ccp (170, screenSize.height - 125) );
    uploadToServer->setColor(ccc3(20, 200, 20));
    navigationMenu->addChild(uploadToServer);
    
    CCMenuItemFont *deleteWorld = CCMenuItemFont::create("Удалить мир", this, menu_selector(SelectMapMenuLayer::DeleteWorld));
    deleteWorld->setPosition( ccp (170, screenSize.height - 175) );
    deleteWorld->setColor(ccc3(200, 20, 20));
    navigationMenu->addChild(deleteWorld);
#endif
    
    
    CCMenuItemFont *goToMain = CCMenuItemFont::create(CCLocalizedString("BackToMainMenu", "NotLocalized"), this, menu_selector(SelectMapMenuLayer::mainMenu));
    CCMenuItemFont *goToSelectWorld = CCMenuItemFont::create(CCLocalizedString("BackToWorldSelect", "NotLocalized"), this, menu_selector(SelectMapMenuLayer::worldsMenu));
    goToMain->setPosition( ccp (170, screenSize.height - 25) );
    goToSelectWorld->setPosition( ccp (170, screenSize.height - 75) );
    
    navigationMenu->addChild(goToMain);
    navigationMenu->addChild(goToSelectWorld);
    navigationMenu->setPosition( CCPointZero );
    addChild(navigationMenu, 1);
    
    return true;
    
}

void SelectMapMenuLayer::mainMenu(CCObject *sender)
{
    CCTransitionSlideInL* trans = CCTransitionSlideInL::create( 1.5f * SYSTEM_ANIMATION_DELAY, MainMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void SelectMapMenuLayer::worldsMenu(CCObject *sender)
{
    CCTransitionSlideInL* trans = CCTransitionSlideInL::create( 1.5f * SYSTEM_ANIMATION_DELAY, SelectMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

#ifdef MAP_EDITOR
void SelectMapMenuLayer::addMap (CCObject *sender)
{
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    
    addMapPreview->setPosition(ccpAdd(addMapPreview->getPosition(),ccp(PMMapPreviewWidth+PMMapPreviewOffset,0)));
    scroll->setContentSize( CCSizeMake((GetGameParams->worlds[GetGameParams->getWorldPart()]+2)*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, PMMapPreviewHeight) );
    
    
    float newX = scroll->getContainer()->getPosition().x - PMMapPreviewWidth - PMMapPreviewOffset;
    
    scroll->setContentOffset(ccp(newX, 0),true);
    
    
    
    
    GetGameParams->worlds[GetGameParams->getWorldPart()]++;
    
    std::string mapPath = writablePath+"Maps/World"+to_string(GetGameParams->getWorldPart()) + "/Lvl"+to_string(GameParams::getInstanse()->worlds[GetGameParams->getWorldPart()])+".map";
    
    char MAP[10000] = "";
    
    std::string newMapPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("new.map");
    
    FILE* map = fopen(newMapPath.c_str(), "rb");
    fread(MAP,1,10000,map);
    //fscanf(map,"%[]",MAP);
    fclose(map);

    
    FILE* newmap = fopen(mapPath.c_str(), "wb");
    
    fprintf(newmap,"%s",MAP);
    fclose(newmap);
    
    MapEditorGameLayer::getInstanse()->mapVer++;
    MapEditorGameLayer::getInstanse()->saveWorldsetLocal();
    
    PMMapPreview * preview = PMMapPreview::create(GetGameParams->getWorldPart(), GetGameParams->worlds[GetGameParams->getWorldPart()]);
    preview->setPosition(ccp((GetGameParams->worlds[GetGameParams->getWorldPart()]-1)*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
    scroll->addChild(preview);
}

void SelectMapMenuLayer::uploadMapsToServer (CCObject *sender)
{
    MapEditorGameLayer::getInstanse()->uploadMapsToServer();
}


void SelectMapMenuLayer::DeleteWorld (CCObject * sender)
{
system(("rm -rf "+writablePath + "Maps/World"+to_string(GetGameParams->getWorldPart())).c_str());
 
    for (int i = GetGameParams->getWorldPart()+1; i <= GetGameParams->worlds.size(); ++i) {
        std:: string oldMapName = writablePath + "Maps/World"+to_string(i);
        std:: string newMapName = writablePath + "Maps/World"+to_string(i-1);
        rename(oldMapName.c_str(), newMapName.c_str());
    }
    GetGameParams->worlds.erase(GetGameParams->worlds.begin()+GetGameParams->getWorldPart());
    
    MapEditorGameLayer::getInstanse()->saveWorldsetLocal();
 
    CCTransitionSlideInL* trans = CCTransitionSlideInL::create( 1.5f * SYSTEM_ANIMATION_DELAY, SelectMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}
 

#endif

void SelectMapMenuLayer::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();
}

void SelectMapMenuLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool SelectMapMenuLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
#ifdef MAP_EDITOR
    time = getTimeMs();
#endif
    return scroll->ccTouchBegan(touch, pEvent);
}

void SelectMapMenuLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
#ifdef MAP_EDITOR
    CCPoint dif = ccpSub(touch->getLocation(),touch->getStartLocation());
    
    dif.x = fabs(dif.x);
    dif.y = fabs(dif.y);
    
    if (dif.x < 20 && dif.y < 20 && !dragPreview)
    {
        if (time + 1000 < getTimeMs())
        {
            CCPoint touchPoint = scroll->getContainer()->convertToNodeSpace(touch->getStartLocation());
            
            mapNum = (touchPoint.x/(PMMapPreviewOffset+PMMapPreviewWidth)) + 1;
            
            if ( mapNum < 1 ) return;
            if ( mapNum > GetGameParams->worlds[GetGameParams->getWorldPart()]) return;
            
            ISdrag = true;
            
            dragPreview = PMMapPreview::create(GetGameParams->getWorldPart(), mapNum);
            dragPreview->setScale(1.2);
            dragPreview->setContentSize(CCSizeMake(PMMapPreviewWidth+20, PMMapPreviewHeight+20));
            if(dragPreview)addChild(dragPreview);
            scroll->ccTouchCancelled(touch, pEvent);
            
            delta = ((CCNode*)scroll->getContainer()->getChildren()->objectAtIndex(mapNum-1))->convertToNodeSpace(touch->getLocation());
            ((CCNode*)scroll->getContainer()->getChildren()->objectAtIndex(mapNum-1))->setVisible(false);
        }
    }
    
    if (ISdrag) {
        if(dragPreview){
            dragPreview->setPosition(ccpSub(convertTouchToNodeSpace(touch),delta));
            
            if(scroll->convertTouchToNodeSpace(touch).x < 200) {
                scroll->setContentOffset(ccpAdd(scroll->getContentOffset(), ccp(20,0)), true);
            }
            
            if(scroll->convertTouchToNodeSpace(touch).x > scroll->getViewSize().width-200) {
                scroll->setContentOffset(ccpAdd(scroll->getContentOffset(), ccp(-20,0)), true);
            }
            
            CCPoint touchPoint = scroll->getContainer()->convertToNodeSpace(touch->getLocation());
            
            int mapNum_ = (touchPoint.x/(PMMapPreviewOffset+PMMapPreviewWidth));
            
            if (mapNum_ > mapNum-1) {
                for (int i = 0; i < mapNum-1; ++i) {
                    ((CCNode*)scroll->getContainer()->getChildren()->objectAtIndex(i))->setPosition(ccp(i*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
                }
                for (int i = mapNum; i < mapNum_+1; ++i) {
                    ((CCNode*)scroll->getContainer()->getChildren()->objectAtIndex(i))->setPosition(ccp((i-1)*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
                }
                for (int i = mapNum_+1; i < scroll->getContainer()->getChildren()->count(); ++i) {
                    ((CCNode*)scroll->getContainer()->getChildren()->objectAtIndex(i))->setPosition(ccp(i*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
                }
            }
            else {
                for (int i = 0; i < mapNum_; ++i) {
                    ((CCNode*)scroll->getContainer()->getChildren()->objectAtIndex(i))->setPosition(ccp(i*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
                }
                for (int i = mapNum_; i < mapNum-1; ++i) {
                    ((CCNode*)scroll->getContainer()->getChildren()->objectAtIndex(i))->setPosition(ccp((i+1)*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
                }
                for (int i = mapNum; i < scroll->getContainer()->getChildren()->count(); ++i) {
                    ((CCNode*)scroll->getContainer()->getChildren()->objectAtIndex(i))->setPosition(ccp(i*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
                }
            }
        }
    }
#endif
}

void SelectMapMenuLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
    if(scroll->getContentOffset().x > 0 )scroll->setContentOffset(CCPointZero,true);
    if(scroll->getContentOffset().x <  - scroll->getContentSize().width + scroll->getViewSize().width )scroll->setContentOffset(ccp(-scroll->getContentSize().width + scroll->getViewSize().width,0),true);
    
    scroll->ccTouchEnded(touch, pEvent);
     #ifdef MAP_EDITOR
    if(dragPreview)
    {
        removeChild(dragPreview, true);
        dragPreview = NULL;
    }
    

    if(ISdrag) {
        ISdrag = false;
        
        CCPoint touchPoint = scroll->getContainer()->convertTouchToNodeSpace(touch);
        
        int newmapnum = (touchPoint.x/(PMMapPreviewOffset+PMMapPreviewWidth)) + 1;
        
        if (newmapnum > GetGameParams->worlds[GetGameParams->getWorldPart()] )
            newmapnum = GetGameParams->worlds[GetGameParams->getWorldPart()];
        
        if (newmapnum < 1) {
            newmapnum = 1;
        }
        
        printf("start map %d\n", mapNum);
        printf("end map %d\n", newmapnum);
        
        if(newmapnum == mapNum)
        {
            ((CCNode*)scroll->getContainer()->getChildren()->objectAtIndex(mapNum-1))->setVisible(true);
            for (int i = 0; i < scroll->getContainer()->getChildren()->count(); ++i) {
                ((CCNode*)scroll->getContainer()->getChildren()->objectAtIndex(i))->setPosition(ccp(i*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
            }
            return;
        }
        
        rename((writablePath + "Maps/World"+to_string(GetGameParams->getWorldPart())+"/Lvl"+to_string(mapNum)+".map").c_str(), (writablePath+"tmp.map").c_str());
        
        for (int i = mapNum; i != newmapnum; i+=sign(newmapnum-mapNum)) {
            rename((writablePath + "Maps/World"+to_string(GetGameParams->getWorldPart())+"/Lvl"+to_string(i+sign(newmapnum-mapNum))+".map").c_str(), (writablePath + "Maps/World"+to_string(GetGameParams->getWorldPart())+"/Lvl"+to_string(i)+".map").c_str());
        }
        
        rename((writablePath+"tmp.map").c_str(), (writablePath + "Maps/World"+to_string(GetGameParams->getWorldPart())+"/Lvl"+to_string(newmapnum)+".map").c_str());
        
        scroll->getContainer()->removeAllChildrenWithCleanup(true);
        
        for (int i = 0; i < GetGameParams->worlds[GetGameParams->getWorldPart()]; ++i) {
            PMMapPreview * preview = PMMapPreview::create(GetGameParams->getWorldPart(), i+1);
            preview->setPosition(ccp(i*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
            scroll->addChild(preview);
        }
        
        addMapPreview = PMMapPreview::create();
        addMapPreview->setPosition(ccp(GetGameParams->worlds[GetGameParams->getWorldPart()]*(PMMapPreviewWidth + PMMapPreviewOffset) + PMMapPreviewOffset, 0));
        scroll->addChild(addMapPreview);
    }
    
    else {
#endif

		CCPoint dif = ccpSub(touch->getLocation(),touch->getStartLocation());
        
        dif.x = fabs(dif.x);
        dif.y = fabs(dif.y);
        
        if (dif.x < 5 && dif.y < 5) {
            CCPoint touchPoint = scroll->getContainer()->convertTouchToNodeSpace(touch);
            
            int map = (touchPoint.x/(PMMapPreviewOffset+PMMapPreviewWidth)) + 1;
            
            if (map > 0 && map <= GetGameParams->worlds[GetGameParams->getWorldPart()]) {
                GetGameParams->setLevel(map);
                GetGameParams->setContinueGame(false);
                
#ifdef MAP_EDITOR
                MapEditorGameLayer::getInstanse()->reloadWorld();
                CCTransitionFade* trans = CCTransitionFade::create( 1.5f * SYSTEM_ANIMATION_DELAY, MapEditorGameLayer::scene_());
#else
                CCTransitionFade* trans = CCTransitionFade::create( 1.5f * SYSTEM_ANIMATION_DELAY, GameLayer::scene());
#endif
                CCDirector::sharedDirector()->replaceScene(trans);
            }
#ifdef MAP_EDITOR
            else if (map-1 == GetGameParams->worlds[GetGameParams->getWorldPart()] )
            {
                addMap(NULL);
            }
#endif
            
        }
#ifdef MAP_EDITOR
    }
#endif
}

