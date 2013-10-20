//
//  MapEditorGameLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 10.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "MapEditorGameLayer.h"
#include "MapControl.h"

MapEditorGameLayer *MapEditorGameLayer::_instanse = NULL;

MapEditorGameLayer *MapEditorGameLayer::getInstanse()
{
    if(_instanse == NULL)
    {
        _instanse = new MapEditorGameLayer;
        _instanse->init();
    }
    
    return _instanse;
}

void MapEditorGameLayer::reloadWorld() {
    removeAllChildrenWithCleanup(true);
    MapControlLayer* mapControlLayer = MapControlLayer::create();
    mapControlLayer->updateArrow();
    mapControlLayer->setPosition(ccp(0,cocos2d::CCDirector::sharedDirector()->getWinSize().height));
    addChild(mapControlLayer,50);
    gameLayerInit();
    loadWorld();
    mapControlLayer->finishMapLoading();
}

cocos2d::CCScene* MapEditorGameLayer::scene_()
{
    // 'scene' is an autorelease object
    cocos2d::CCScene *scene = cocos2d::CCScene::create();
    
    // 'layer' is an autorelease object
    MapEditorGameLayer *layer = MapEditorGameLayer::getInstanse();
    
    
    // add layer as a child to scene
    scene->addChild(layer,0,0);
    
    // return the scene
    return scene;
}

bool MapEditorGameLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    mapVer = GameParams::getInstanse()->getMapVersion();
    
    m_bKeypadEnabled=true;
    m_bTouchEnabled = true;
    
    editing = false;
    FTPUser = NULL;
    
    FTPLogin = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("Login");
    FTPPass = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("Pass");
    
    if (FTPPass != "") {
        FTPUser = new FTPuser(writablePath,FTPLogin,FTPPass);
        if(FTPUser->checkFTP())
        {
        
        }
        else
        {
            
        }
    }
    
    GameParams::getInstanse()->screenSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    
    //baseRobotManager->setControlledMode(true);
    //schedule(schedule_selector(GameLayer::makeStep), 0.01f);
    
    return true;
}

bool MapEditorGameLayer::changeMapSize (int width, int height) {
    if (getWorld()->type() == pmWorld4) {
        for (int i = 0; i < world->robotCount(); ++i) {
            UniversalRobot * robot = world->robotAt(i);
            if (robot->type() == pmRobot4) {
                PlayerRobot2D* robot_ = (PlayerRobot2D*)robot;
                if (robot_->getCurPosition().x >= width || robot_->getCurPosition().y >= height) {
                    return false;
                }
            }
        }
        ((Map4*)(getWorld()->getMap()))->resize (width,height);
    }
    return true;
}

bool MapEditorGameLayer::changeMapLayers (int layers) {
    if (getWorld()->type() == pmWorld4) {
        for (int i = 0; i < world->robotCount(); ++i) {
            UniversalRobot * robot = world->robotAt(i);
            if (robot->type() == pmRobot4) {
                PlayerRobot2D* robot_ = (PlayerRobot2D*)robot;
                if (robot_->getCurLayer() >= layers) {
                    return false;
                }
            }
        }
        ((Map4*)(getWorld()->getMap()))->changeLayers (layers);
    }
    
    return true;
}

void zipFolder(std::string src, std::string dst)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    system(("cd "+src+"\nzip -r -q "+dst+" .").c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif
}

void MapEditorGameLayer::saveWorldsetLocal ()
{
    xmlDocPtr worldSet = xmlNewDoc((xmlChar*)"1.0");
    xmlNodePtr worldsNode = xmlNewNode(NULL, (xmlChar*)"worlds");
    xmlDocSetRootElement(worldSet, worldsNode);
    
    char tmp[10] = "";
    sprintf(tmp, "%ld", GameParams::getInstanse()->worlds.size());
    xmlSetProp(worldsNode, BAD_CAST "count", BAD_CAST tmp);
    if(mapVer != GameParams::getInstanse()->getMapVersion()) sprintf(tmp, "%d",GameParams::getInstanse()->getMapVersion()+1);
    else sprintf(tmp, "%d",GameParams::getInstanse()->getMapVersion());
    xmlSetProp(worldsNode, BAD_CAST "version", BAD_CAST tmp);
    
    for (int i = 0; i < GameParams::getInstanse()->worlds.size(); ++i) {
        xmlNodePtr worldNode = xmlNewNode(NULL, (xmlChar*)"world");
        sprintf(tmp, "%d", GameParams::getInstanse()->worlds[i]);
        xmlSetProp(worldNode, BAD_CAST "count", BAD_CAST tmp);
        xmlSetProp(worldNode, BAD_CAST "worldtype", BAD_CAST "0");
        xmlAddChild(worldsNode, worldNode);
    }
    
    xmlSaveFile((writablePath+"Maps/Worlds.xml").c_str(), worldSet);
}

void MapEditorGameLayer::uploadMapsToServer()
{
    saveWorldsetLocal();
    
    zipFolder(writablePath+"Maps", writablePath+"Maps.zip");
    std::string mapPathStr = writablePath+"Maps.zip";
    FILE* ver = fopen((writablePath+"ver.txt").c_str(), "w");
    fprintf(ver, "%d",GameParams::getInstanse()->getMapVersion());
    fclose(ver);
    FTPUser->uploadMap(writablePath+"ver.txt",mapPathStr);
}

