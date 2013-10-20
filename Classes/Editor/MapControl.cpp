//
//  MapControl.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 12.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "MapControl.h"
#include "GameParams.h"
#include "cocos2d.h"
#include "MapEditorGameLayer.h"
#include "Robot4Edit.h"
#include "XmlWorldSaver.h"
#include "MainMenuLayer.h"
#include "ScrollLayer.h"
#include "SelectMapMenuLayer.h"

USING_NS_CC;

using namespace std;

MapControlLayer * MapControlLayer::instanse = NULL;

MapControlLayer* MapControlLayer::getInstanse () {
    return instanse;
}

MapControlLayer* MapControlLayer::create() {
    if(instanse){
        instanse->removeAllChildrenWithCleanup(true);
        instanse->release();
    }
    instanse = new MapControlLayer;
    instanse->init();
    return instanse;
}


void MapControlLayer::finishMapLoading() {
    layersCnt_ = ((Map4*)(MapEditorGameLayer::getInstanse()->getWorld()->getMap()))->getLayerCount();
    
    
    tilesetCnt_ = ((Map4*)(MapEditorGameLayer::getInstanse()->getWorld()->getMap()))->getTileSet()+1;
    
    char tmp[10] = "";
    
    sprintf(tmp,"%d", layersCnt_);
    layersCnt->setString(tmp);
    
    sprintf(tmp,"%d", tilesetCnt_);
    tilesetCnt->setString(tmp);
    
    RobotPrefsScrolLayer::getInstanse()->finishLoading();
}

void MapControlLayer::updateArrow ()
{
    if(!arrow)return;
    arrow->setNormalImage(CCSprite::create("System/down.png"));
    arrow->setSelectedImage(CCSprite::create("System/down.png"));
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    arrow->setPosition(ccp((screenSize.width-GameParams::getInstanse()->programLayerWidth)/2,0));
    showed = false;
}

void MapControlLayer::showLayer(CCObject * sender) {
    showed = !showed;
    if(showed) {
        arrow->setNormalImage(CCSprite::create("System/up.png"));
        arrow->setSelectedImage(CCSprite::create("System/up.png"));
        arrow->setPosition(ccpAdd(((CCMenuItemImage*)sender)->getPosition(),ccp(0,18)));
        
        CCFiniteTimeAction* moveLayer = CCMoveBy::create(0.4f, ccp(0,-sizeHeight));
        
        runAction( moveLayer );
        
        MapEditorGameLayer::getInstanse()->setEnabled(false);
    }
    else {
        arrow->setNormalImage(CCSprite::create("System/down.png"));
        arrow->setSelectedImage(CCSprite::create("System/down.png"));
        arrow->setPosition(ccpAdd(((CCMenuItemImage*)sender)->getPosition(),ccp(0,-18)));
        CCFiniteTimeAction* moveLayer = CCMoveBy::create(0.4f, ccp(0,sizeHeight));
        runAction( moveLayer );
        MapEditorGameLayer::getInstanse()->setEnabled(true);
    }
    
    MapEditorGameLayer::getInstanse()->getSpeedBarLayer()->setEnabled(!showed);
}

void MapControlLayer::plusLayers (CCObject * sender) {
    layersCnt_ ++;
    char tmp[10] = "";
    sprintf(tmp,"%d", layersCnt_);
    layersCnt->setString(tmp);
    
    MapEditorGameLayer::getInstanse()->changeMapLayers(layersCnt_);
}

void MapControlLayer::minusLayers (CCObject * sender) {
    layersCnt_ --;
    if(layersCnt_ == 0) {layersCnt_++;return;}
    
    MapEditorGameLayer::getInstanse()->changeMapLayers(layersCnt_);
    
    char tmp[10] = "";
    sprintf(tmp,"%d", layersCnt_);
    layersCnt->setString(tmp);
}

void MapControlLayer::plusTileset (CCObject * sender) {
    tilesetCnt_ ++;
    if(tilesetCnt_ == 2) {tilesetCnt_--;return;}
    char tmp[10] = "";
    sprintf(tmp,"%d", tilesetCnt_);
    tilesetCnt->setString(tmp);
    
    //MapEditorGameLayer::getInstanse()->changeTileset(tilesetCnt_-1);
}

void MapControlLayer::minusTileset (CCObject * sender) {
    tilesetCnt_ --;
    if(tilesetCnt_ == 0) {tilesetCnt_++;return;}
    
    //MapEditorGameLayer::getInstanse()->changeTileset(tilesetCnt_-1);
    
    char tmp[10] = "";
    sprintf(tmp,"%d", tilesetCnt_);
    tilesetCnt->setString(tmp);
}

bool MapControlLayer::init()
{
    if ( !CCLayerColor::init() )
    {
        return false;
    }
    
    removeAllChildrenWithCleanup(true);
    
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    sizeHeight = 200;
    showed = false;
    layersCnt_ = 0;
    
    m_bTouchEnabled = true;
    
    CCLabelTTF * mapLabel = CCLabelTTF::create("Карта", "Marker Felt", 22);
    mapLabel->setAnchorPoint(ccp(0,1));
    mapLabel->setPosition(ccp(25,sizeHeight-5));
    
    CCLabelTTF * layers = CCLabelTTF::create("Слои", "Marker Felt", 18);
    layers->setAnchorPoint(ccp(0,1));
    layers->setPosition(ccp(25,sizeHeight-35));
    
    CCMenuItemLabel* layersPlus = CCMenuItemLabel::create(CCLabelTTF::create("+", "Marker Felt", 18),this,menu_selector(MapControlLayer::plusLayers));
    layersPlus->setAnchorPoint(ccp(0,1));
    layersPlus->setPosition(ccp(135,sizeHeight-35));
    
    CCMenuItemLabel* layersMinus = CCMenuItemLabel::create(CCLabelTTF::create("-", "Marker Felt", 18),this,menu_selector(MapControlLayer::minusLayers));
    layersMinus->setAnchorPoint(ccp(0,1));
    layersMinus->setPosition(ccp(95,sizeHeight-35));
    
    layersCnt = CCLabelTTF::create("1", "Marker Felt", 18);
    layersCnt->setAnchorPoint(ccp(0,1));
    layersCnt->setPosition(ccp(110,sizeHeight-35));
    
    
    CCLabelTTF * tileset = CCLabelTTF::create("Тайлсет", "Marker Felt", 18);
    tileset->setAnchorPoint(ccp(0,1));
    tileset->setPosition(ccp(25,sizeHeight-65));
    
    CCMenuItemLabel* tilesetPlus = CCMenuItemLabel::create(CCLabelTTF::create("+", "Marker Felt", 18),this,menu_selector(MapControlLayer::plusTileset));
    tilesetPlus->setAnchorPoint(ccp(0,1));
    tilesetPlus->setPosition(ccp(135,sizeHeight-65));
    
    CCMenuItemLabel* tilesetMinus = CCMenuItemLabel::create(CCLabelTTF::create("-", "Marker Felt", 18),this,menu_selector(MapControlLayer::minusTileset));
    tilesetMinus->setAnchorPoint(ccp(0,1));
    tilesetMinus->setPosition(ccp(95,sizeHeight-65));
    
    tilesetCnt = CCLabelTTF::create("1", "Marker Felt", 18);
    tilesetCnt->setAnchorPoint(ccp(0,1));
    tilesetCnt->setPosition(ccp(110,sizeHeight-65));
    
    
    
    CCMenuItemLabel* SaveMap = CCMenuItemLabel::create(CCLabelTTF::create("Сохранить!", "Marker Felt", 22),this,menu_selector(MapControlLayer::SaveMap));
    SaveMap->setColor(ccc3(20, 200, 20));
    SaveMap->setAnchorPoint(ccp(0,1));
    SaveMap->setPosition(ccp(35,sizeHeight-95));
    
    CCMenuItemLabel* closeMap = CCMenuItemLabel::create(CCLabelTTF::create("Закрыть", "Marker Felt", 22),this,menu_selector(MapControlLayer::CloseMap));
    closeMap->setColor(ccc3(20, 20, 200));
    closeMap->setAnchorPoint(ccp(0,1));
    closeMap->setPosition(ccp(35,sizeHeight-125));
    
    CCMenuItemLabel* deleteMap = CCMenuItemLabel::create(CCLabelTTF::create("Удалить", "Marker Felt", 22),this,menu_selector(MapControlLayer::DeleteMap));
    deleteMap->setColor(ccc3(200, 20, 20));
    deleteMap->setAnchorPoint(ccp(0,1));
    deleteMap->setPosition(ccp(35,sizeHeight-155));
    
    setColor(ccc3(144,144,144));
    setOpacity(220);
    
    setContentSize(CCSizeMake(screenSize.width-GameParams::getInstanse()->programLayerWidth,sizeHeight));
    
    arrow = CCMenuItemImage::create("System/down.png", "System/down.png",this, menu_selector(MapControlLayer::showLayer));
    CCMenu * menu = CCMenu::create(arrow, SaveMap, closeMap, deleteMap, layersPlus, layersMinus, tilesetPlus, tilesetMinus, NULL);
    
    arrow->setAnchorPoint(ccp(0.5,1));
    arrow->setPosition(ccp((screenSize.width-GameParams::getInstanse()->programLayerWidth)/2,0));
    
    menu->setPosition(CCPointZero);
    menu->retain();
    addChild(menu,2);
    
    addChild(mapLabel,2);
    addChild(layers,2);
    addChild(tileset,2);
    
    addChild(layersCnt,2);
    addChild(tilesetCnt,2);
    
    // robots
    
    robotsPrefs = RobotPrefsScrolLayer::getInstanse();
    robotsPrefs->setContentSize(CCSizeMake(screenSize.width-GameParams::getInstanse()->programLayerWidth - 200,sizeHeight));
    robotsPrefs->setPosition(ccp(200,0));
    robotsPrefs->frame = CCRectMake(200, 0, screenSize.width-GameParams::getInstanse()->programLayerWidth - 200, sizeHeight);
    
//    Robot4Edit * robot4Edit = Robot4Edit::create();
//    robot4Edit->setAnchorPoint(ccp(0,0));
//    robot4Edit->setPosition(0,sizeHeight);
    
//    CCLabelTTF * scroll = CCLabelTTF::create("Скрол типо", "Marker Felt", 18);
//    scroll->setAnchorPoint(ccp(0,1));
//    scroll->setPosition(ccp(50,50));
//    robotsPrefs->addChild(robot4Edit);
//
    addChild(robotsPrefs,0);
//    addChild(robot4Edit,0);
    
    return true;
}

void MapControlLayer::SaveMap (CCObject * sender) {
    int worldPart = GetGameParams->getWorldPart();
    int level = GetGameParams->getLevel();
    std::string mapPath = writablePath+"Maps/World"+to_string(worldPart) + "/Lvl"+to_string(level)+".map";
    
    XMLWorldSaver(MapEditorGameLayer::getInstanse()->getWorld(), MapEditorGameLayer::getInstanse(), mapPath);
    // СОХРАНЕНИЕ
    
    CloseMap(NULL);
}

void MapControlLayer::CloseMap (CCObject * sender)
{
    CCPointObject * object = (CCPointObject *)MapEditorGameLayer::getInstanse()->getWorld()->getMap()->getScroll()->getParallax()->getParallaxArray()->arr[0];
    
    CCSprite *background = (CCSprite *)object->getChild();
    
    object->setRatio(ccp(1.0,1.0));
    object->setOffset( ccp(background->getContentSize().width / 2, background->getContentSize().height / 2) );
    
    CCTransitionSlideInL* trans = CCTransitionSlideInL::create( 1.5f * SYSTEM_ANIMATION_DELAY, SelectMapMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);

}

void MapControlLayer::DeleteMap (CCObject * sender)
{
    system(("rm "+writablePath + "Maps/World"+to_string(GetGameParams->getWorldPart())+"/Lvl"+to_string(GetGameParams->getLevel())+".map").c_str());
    
    for (int i = GetGameParams->getLevel()+1; i <= GetGameParams->worlds[GetGameParams->getWorldPart()]; ++i) {
        std:: string oldMapName = writablePath + "Maps/World"+to_string(GetGameParams->getWorldPart())+"/Lvl"+to_string(i)+".map";
        std:: string newMapName = writablePath + "Maps/World"+to_string(GetGameParams->getWorldPart())+"/Lvl"+to_string(i-1)+".map";
        rename(oldMapName.c_str(), newMapName.c_str());
    }
    GetGameParams->worlds[GetGameParams->getWorldPart()]--;
    MapEditorGameLayer::getInstanse()->saveWorldsetLocal();
   
    CloseMap(NULL);
}

void MapControlLayer::draw()
{
    CCLayerColor::draw();
    
    ccDrawInit();
    ccDrawColor4F(255,255,255,255);
    glLineWidth(2.0f);
    //draw lines
    
    ccDrawLine(ccp(200,0), ccp(200,sizeHeight));
}
