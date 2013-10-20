//
//  Map4.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#include "Map4.h"
#include "GameParams.h"
#include "World4.h"
#include "GameLayer.h"
#include "ScrollLayer.h"
#include "MapContainer.h"
#include "ScrollLayer.h"
#include "ControlLayer4.h"

#ifdef MAP_EDITOR
#include "MapEditorGameLayer.h"
#endif

USING_NS_CC;

cocos2d::CCPoint Map4::Coordinates(int x, int y, double dx, double dy)
{
    return ccp(mapElementSize.width * x + dx,getRealHeight()-mapElementSize.height * y + dy);
}

Map4 *Map4::create()
{
    Map4 *map = new Map4();
    map->autorelease();
    return map;
}

Map4 *Map4::create(World4* parent,int width, int height, int layerNum, int tileset)
{
    Map4 *map = new Map4(parent, width, height, layerNum, tileset);
    map->init();
    map->autorelease();
    return map;
}

bool Map4::init()
{
    if( !CCLayer::init() ) 
    {
        return false;
    }
    
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    
    setContentSize(CCSizeMake(screenSize.width  - GameParams::getInstanse()->programLayerWidth, screenSize.height));
    setPosition( ccp(0, 0) );
    
    
    for (int i = 0; i < mapElements.size(); ++i) {
        mapElements[i] = std::vector<std::vector<MapElement *> >();
        mapElements[i].resize(height);
        for (int j = 0; j < height; j++) {
            mapElements[i][j] = std::vector<MapElement *>();
            mapElements[i][j].resize(width);
            for (int k = 0; k < width; k++) {
                mapElements[i][j][k] = new MapElement(4);
            }
        }
    }
    
    return true;
}

void Map4::drawMap()
{
    GameParams *params = GameParams::getInstanse();
    CCSize screenSize = GameParams::getInstanse()->screenSize;
    
    
    scroll  = ScrollLayer::create(this,CCSizeMake(screenSize.width  - params->programLayerWidth,params->screenSize.height));
    scroll->setPosition(CCPointZero);
    
    activeLayer = ((PlayerRobot2D *)_world->getBaseRobot())->getStartLayer();
    
    for (int j = 0; j < mapElements.size(); ++j) {
        CCLayer *mapLayer = CCLayer::create();
        mapLayer->setContentSize(CCSizeMake(getRealWidth(), getRealHeight()));
        MapContainer *mapContainer  = MapContainer::create(mapLayer);
        mapContainer->setContentSize(CCSizeMake(getRealWidth(), getRealHeight()));
        
        
        layers.push_back(mapContainer);
        
        
        
        //mapLayer->setRotation( 45.0f );
    
        for (int  i = 0; i < _world->robotCount(); ++i)
        {
            if(_world->robotAt(i)->parentType() == pmPlayerRobot)
            {
                PlayerRobot2D *robot = (PlayerRobot2D *)_world->robotAt(i);
                
                if (robot->getCurLayer() == j)
                {
                    robot->setRotation(-45.0f);
                    robot->setScaleY(2.0f);
                    mapLayer->addChild(robot,2);
                    
                }
            }
            else if(_world->robotAt(i)->parentType() == pmEnvRobot)
            {
                EnvRobot2D *robot = (EnvRobot2D *)_world->robotAt(i);
                
                if (robot->getCurLayer() == j)
                {
                    robot->setRotation(-45.0f);
                    robot->setScaleY(2.0f);
                    mapLayer->addChild(robot,2);
                    
                }
            }
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                CCSprite *sprite = mapElements[j][y][x]->getGrassSprite();
                
                if (sprite != NULL)
                {
                    mapLayer->addChild(sprite, 0);
                }
                
                sprite = mapElements[j][y][x]->getWallSprite();
                
                if (sprite != NULL) {
                    mapLayer->addChild(sprite,1);
                }
            }
        }
        
#ifdef MAP_EDITOR
        resizeSprite = CCSprite::create("System/unchecked.png");
        resizeSprite->setPosition(Coordinates(width, 0, -mapElementSize.width / 2, mapElementSize.height / 2));
        mapLayer->addChild(resizeSprite,1);
#endif
        
        //layers[j]->setScaleY(0.5);
        
        if(j <= activeLayer + DISPLAYED_LAYERS/2 && j>= activeLayer - DISPLAYED_LAYERS/2 ) {
            scroll->getParallax()->addChild(layers[j], 1, ccp(1.0, 1.0), ccp(GameParams::getInstanse()->mapSize().width/2, GameParams::getInstanse()->mapSize().height/2));
        }
        
    }
    
    scrollToInitialPosition();
    
    addChild(scroll);
    
    controlLayer = ControlLayer4::create(world()->getBaseRobotManager());
    addChild(controlLayer,10);
}

void Map4::update()
{    
    for (int j = 0; j < mapElements.size(); ++j)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                CCSprite *sprite = mapElements[j][y][x]->getWallSprite();
                mapElements[j][y][x]->setWallZOrder(2 * getWallZOrder(x,y,j));
                if (sprite != NULL)
                {
                    layers[j]->getMapLayer()->reorderChild(sprite, mapElements[j][y][x]->getWallZOrder());
                }
            }
        }
    }
    
    for (int i = 0; i < _world->robotCount(); ++i)
    {
        if(_world->robotAt(i)->parentType() == pmPlayerRobot)
        {
            PlayerRobot2D *robot  = (PlayerRobot2D *)_world->robotAt(i);
        
            int x = robot->getCurPosition().x;
            int y = robot->getCurPosition().y;
            int layer = robot->getCurLayer();
        
            layers[layer]->getMapLayer()->reorderChild(robot, mapElements[layer][y][x]->getWallZOrder() + 1);
        }
        if(_world->robotAt(i)->parentType() == pmEnvRobot)
        {
            EnvRobot2D *robot  = (EnvRobot2D *)_world->robotAt(i);
            
            int x = robot->getCurPosition().x;
            int y = robot->getCurPosition().y;
            int layer = robot->getCurLayer();
            
            layers[layer]->getMapLayer()->reorderChild(robot, mapElements[layer][y][x]->getWallZOrder() + 1);
        }
        
    }
    
    centerMap();
}

int Map4::getWallZOrder(int x, int y, int layer)
{
    int ZX = 0;
    int ZY = 0;
    
    for (int i = 0; i < _world->robotCount(); ++i)
    {
        PlayerRobot2D *robot  = (PlayerRobot2D *)_world->robotAt(i);
        
        if (robot->getCurLayer()!=layer)
            continue;
        else if (x >robot->getCurPosition().x)
            ZY++;
        else if (y > robot->getCurPosition().y)
            ZX++;
    }
    
    if (ZX == _world->robotCount() )
        return 2*ZX;
    else if (ZY == _world->robotCount())
        return 2*ZY;
    else if (ZX != 0 || ZY != 0)
        return ZX+ZY;
    
    return 0;
}

void Map4::centerMap()
{
    int layer = ((PlayerRobot2D *)_world->getBaseRobot())->getCurLayer();
    
    CCPoint robotPosotion = layers[layer]->getMapLayer()->convertToWorldSpaceAR(((PlayerRobot2D *)_world->getBaseRobot())->getPosition());
    
    CCFiniteTimeAction* moveLayerX = NULL;
    CCFiniteTimeAction* moveLayerY = NULL;
    
    //CCLayer *mapLayer = layers[layer];
    
    if(robotPosotion.x < 25 )
        moveLayerX = CCMoveTo::create(SYSTEM_ANIMATION_DELAY, ccp(scroll->getParallax()->getPositionX() + 100 ,scroll->getParallax()->getPositionY()));
    
    if(robotPosotion.x > GetGameParams->mapSize().width - 25)
        moveLayerX = CCMoveTo::create(SYSTEM_ANIMATION_DELAY, ccp(scroll->getParallax()->getPositionX() - 100 ,scroll->getParallax()->getPositionY()));
    
    if(robotPosotion.y < 25)
        moveLayerY = CCMoveTo::create(SYSTEM_ANIMATION_DELAY, ccp(scroll->getParallax()->getPositionX() ,scroll->getParallax()->getPositionY() + 100));
    
    if(robotPosotion.y > GetGameParams->mapSize().height - 25)
        moveLayerY = CCMoveTo::create(SYSTEM_ANIMATION_DELAY, ccp(scroll->getParallax()->getPositionX() ,scroll->getParallax()->getPositionY() - 100));
    
    if(moveLayerX != NULL && moveLayerY !=NULL)
        scroll->getParallax()->runAction( CCSequence::create(moveLayerX, moveLayerY, NULL));
    
    else if(moveLayerY != NULL && moveLayerX == NULL)
        scroll->getParallax()->runAction( CCSequence::create(moveLayerY, NULL));
    
    else if(moveLayerY == NULL && moveLayerX != NULL)
        scroll->getParallax()->runAction( CCSequence::create(moveLayerX, NULL));
}

void Map4::clean()
{
    for(int i = 0; i < world()->robotCount(); ++i)
    {
        if(world()->robotAt(i)->getTask() != NULL)
            world()->robotAt(i)->getTask()->clear();
    }
    
    for (int j = 0; j < layers.size(); ++j) {
        for (int i = 0; i < height; ++i) {
            for (int k = 0; k < width; ++k) {
                mapElements[j][i][k]->isPainted = false;
                mapElements[j][i][k]->isRobotHere = false;
                mapElements[j][i][k]->robotHere = NULL;
                
                if (mapElements[j][i][k]->getType() == MapElement::GreenGrass) {
                    mapElements[j][i][k]->getGrassSprite()->setTextureRect( CCRectMake(2*64, 0.0, 64, 64) );
                }
            }
        }
    }
    
    for (int i = 0; i < world()->robotCount(); ++i)
    {
        if(_world->robotAt(i)->parentType() == pmPlayerRobot)
        {
            PlayerRobot2D *robot = (PlayerRobot2D *)_world->robotAt(i);
            
            mapElements[robot->getStartLayer()][robot->getStartPosition().y][robot->getStartPosition().x]->isRobotHere = true;
            mapElements[robot->getStartLayer()][robot->getStartPosition().y][robot->getStartPosition().x]->robotHere = world()->robotAt(i);
            
            robot->restartRobot();
        }
    }
    
    scroll->setPosition( CCPointZero );
    
    scrollToInitialPosition();
    
    update();
}

void Map4::setEnabled(bool flag)
{
    Map::setEnabled(flag);
    
    controlLayer->setEnabled(flag);
}

#ifdef MAP_EDITOR

void Map4::deleteMapElement (MapElement* mapElement,int layer) {
    layers[layer]->getMapLayer()->removeChild(mapElement->getGrassSprite());
    layers[layer]->getMapLayer()->removeChild(mapElement->getWallSprite());
}

void Map4::drawMapElement (MapElement* mapElement,int layer) {
    if(mapElement->getGrassSprite()!=NULL)layers[layer]->getMapLayer()->addChild(mapElement->getGrassSprite(),0);
    if(mapElement->getWallSprite()!=NULL)layers[layer]->getMapLayer()->addChild(mapElement->getWallSprite(),1);
}


int Map4::resizeAtTouch(cocos2d::CCPoint point)
{
    MapEditorGameLayer::getInstanse()->changeMapSize (width+point.x, height+point.y);
    resizeSprite->setPosition(Coordinates(width, 0, -mapElementSize.width / 2, mapElementSize.height / 2));
    
    if(point.x!=0 && point.y != 0) return 0;
    if(point.x!=0) return 1;
    if(point.y!=0) return 2;
    return -1;
}

void Map4::resize (int width_, int height_) {
    if (width_ < width) {
        int dw = width-width_;
        for (int i = 0; i < layers.size(); ++i) {
            for (int j = 0; j < height; ++j) {
                
                mapElements[i][j][width_-1]->nearElements[Map4::Right] = NULL;
                for (int k = 0; k < dw; ++k) {
                    deleteMapElement(mapElements[i][j][width-k-1],i);
                    mapElements[i][j].pop_back();
                }
            }
        }
    } else if (width_ > width) {
        int dw = width_-width;
        for (int i = 0; i < mapElements.size(); ++i) {
            for (int j = 0; j < height; ++j) {
                mapElements[i][j].resize(width_);
                
                for (int k = 0; k < dw; ++k) {
                    mapElements[i][j][width+k] = new MapElement(4);
                    
                    mapElements[i][j][width+k]->setElement(MapElement::Grass);
                    
                    if (j > 0) {
                        mapElements[i][j][width+k]->nearElements[Map4::Up] = mapElements[i][j-1][width+k];
                        mapElements[i][j-1][width+k]->nearElements[Map4::Down] = mapElements[i][j][width+k];
                    }
                    mapElements[i][j][width+k]->nearElements[Map4::Down] = NULL;
                    
                    mapElements[i][j][width+k]->nearElements[Map4::Right] = NULL;
                    mapElements[i][j][width+k]->nearElements[Map4::Left] = mapElements[i][j][width+k-1];
                    mapElements[i][j][width+k-1]->nearElements[Map4::Right] = mapElements[i][j][width+k];
                    drawMapElement(mapElements[i][j][width+k],i);
                }
            }
        }
    }
    
    width=width_;
    
    if (height_ < height) {
        for (int i = 0; i < mapElements.size(); ++i) {
            for (int j = 0; j < width; ++j) {
                mapElements[i][height_-1][j]->nearElements[Map4::Down] = NULL;
            }
            for (int k = height-1; k >= height_; --k)
                for (int j = 0; j < width; ++j)
                {
                    deleteMapElement(mapElements[i][k][j],i);
                }
            mapElements[i].pop_back();
        }
    } else if (height_ > height) {
        for (int i = 0; i < mapElements.size(); ++i) {
            mapElements[i].resize(height_);
            for (int k = height; k < height_; ++k) {
                mapElements[i][k] = std::vector<MapElement *>();
                mapElements[i][k].resize(width_);
                for (int j = 0; j < width_; j++) {
                    mapElements[i][k][j] = new MapElement(4);
                }
                
                for (int j = 0; j < width_; ++j) {
                    mapElements[i][k][j]->setElement(MapElement::Grass);
                    
                    mapElements[i][k][j]->nearElements[Map4::Up] = mapElements[i][k-1][j];
                    mapElements[i][k-1][j]->nearElements[Map4::Down] = mapElements[i][k][j];
                    
                    mapElements[i][k][j]->nearElements[Map4::Down] = NULL;
                    mapElements[i][k][j]->nearElements[Map4::Right] = NULL;
                    
                    if(j > 0) {
                        mapElements[i][k][j]->nearElements[Map4::Left] = mapElements[i][k][j-1];
                        mapElements[i][k][j-1]->nearElements[Map4::Right] = mapElements[i][k][j];
                    }
                    
                    drawMapElement(mapElements[i][k][j],i);
                }
            }
        }
    }
    
    height=height_;
    
    for (int i = 0; i < mapElements.size(); ++i) {
        layers[i]->getMapLayer()->setContentSize(CCSizeMake(getRealWidth(), getRealHeight()));
        layers[i]->setContentSize(CCSizeMake(getRealWidth(), getRealHeight()));
        for (int j = 0; j < height; ++j) {
            for (int k = 0; k < width; ++k) {
                cocos2d::CCPoint coord = Coordinates(k,j,0,0);
                if(mapElements[i][j][k]->getWallSprite())mapElements[i][j][k]->getWallSprite()->setPosition(ccpAdd(coord, ccp(-7,7)));
                if(mapElements[i][j][k]->getGrassSprite())mapElements[i][j][k]->getGrassSprite()->setPosition(coord );
            }
        }
    }
    
    
    for (int i = 0; i < GetGameParams->world->robotCount(); ++i) {
        if(getWorld()->robotAt(i)->type() == pmRobot4)((PlayerRobot2D*)(getWorld()->robotAt(i)))->setPosition(Coordinates(((PlayerRobot2D*)(getWorld()->robotAt(i)))->getCurPosition().x, ((PlayerRobot2D*)(getWorld()->robotAt(i)))->getCurPosition().y, -20, 20));
    }
    
    update();
}

void Map4::changeLayers (int layers) {
    if (layers < mapElements.size()) {
        int dl = mapElements.size() - layers;
        for (int i = 0; i < dl; ++i) {
            scroll->removeChild(Map::layers[mapElements.size()-1-i]);
            mapElements.pop_back();
            Map::layers.pop_back();
        }
    } else if (layers > mapElements.size()) {
        int dl = layers - mapElements.size();
        mapElements.resize(layers);
        for (int i = 0; i < dl; ++i) {
            mapElements[layers-dl+i] = std::vector<std::vector<MapElement *> >();
            mapElements[layers-dl+i].resize(height);
            for (int j = 0; j < height; ++j) {
                mapElements[layers-dl+i][j].resize(width);
                for (int k = 0; k < width; ++k) {
                    mapElements[layers-dl+i][j][k] = new MapElement(4);
                    mapElements[layers-dl+i][j][k]->setElement(MapElement::Grass);
                    if(k > 0) {
                        mapElements[layers-dl+i][j][k]->nearElements[Map4::Up] = mapElements[layers-dl+i][j][k-1];
                        mapElements[layers-dl+i][j][k-1]->nearElements[Map4::Down] = mapElements[layers-dl+i][j][k];
                    }
                    mapElements[layers-dl+i][j][k]->nearElements[Map4::Down] = NULL;
                    mapElements[layers-dl+i][j][k]->nearElements[Map4::Right] = NULL;
                    
                    if(j > 0) {
                        mapElements[layers-dl+i][j][k]->nearElements[Map4::Left] = mapElements[layers-dl+i][j-1][k];
                        mapElements[layers-dl+i][j-1][k]->nearElements[Map4::Right] = mapElements[layers-dl+i][j][k];
                    }
                    cocos2d::CCPoint coord = Coordinates(k,j,0,0);
                    if(mapElements[layers-dl+i][j][k]->getWallSprite())mapElements[layers-dl+i][j][k]->getWallSprite()->setPosition(ccpAdd(coord, ccp(7,-7)));
                    if(mapElements[layers-dl+i][j][k]->getGrassSprite())mapElements[layers-dl+i][j][k]->getGrassSprite()->setPosition( coord );
                }
            }
        }
        for (int j = layers-dl; j < mapElements.size(); ++j) {
            CCLayer *mapLayer = CCLayer::create();
            mapLayer->setContentSize(CCSizeMake(getRealWidth(), getRealHeight()));
            MapContainer *mapContainer  = MapContainer::create(mapLayer);
            mapContainer->setContentSize(CCSizeMake(getRealWidth(), getRealHeight()));
            
            
            Map::layers.push_back(mapContainer);
            
            
            
            //mapLayer->setRotation( 45.0f );
            
            for (int  i = 0; i < _world->robotCount(); ++i)
            {
                PlayerRobot2D *robot = (PlayerRobot2D *)_world->robotAt(i);
                
                if (robot->getCurLayer() == j)
                {
                    robot->setRotation(-45.0f);
                    robot->setScaleY(2.0f);
                    mapLayer->addChild(robot,2);
                    
                }
            }
            
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    CCSprite *sprite = mapElements[j][y][x]->getGrassSprite();
                    
                    if (sprite != NULL)
                    {
                        mapLayer->addChild(sprite, 0);
                    }
                    
                    sprite = mapElements[j][y][x]->getWallSprite();
                    
                    if (sprite != NULL) {
                        mapLayer->addChild(sprite,1);
                    }
                }
            }
            
            //layers[j]->setScaleY(0.5);
            
            if(j <= activeLayer + DISPLAYED_LAYERS/2 && j>= activeLayer - DISPLAYED_LAYERS/2 ) {
                scroll->addChild(Map::layers[j]);
            }
            
        }
        
        
    }
    
    scrollToInitialPosition();
}


cocos2d::CCPoint Map4::dxdyAtCcp(cocos2d::CCPoint point)
{
    int x = (point.x) / mapElementSize.width;
    int y =  (point.y) / mapElementSize.height;
    return ccp(x,y);
}

cocos2d::CCPoint Map4::coordAtCcp(cocos2d::CCPoint point)
{
    int x = (point.x + mapElementSize.width/2) / mapElementSize.width;
    if (x < 0) {
        x = 0;
    }
    if (x >= width) x = width-1;
    int y =  (getRealHeight() - point.y + mapElementSize.width/2) / mapElementSize.height;
    if (y < 0) {
        y = 0;
    }
    if (y >= height) y = height-1;
    return ccp(x,y);
}

void Map4::changeElementTypeAtPoint (cocos2d::CCPoint point, int layer) {
    PMMap4Edit active = map4EditorsMenu::getInstanse()->getActive();
    if (active == None) {
        return;
    }
    int x = (point.x + mapElementSize.width/2) / mapElementSize.width;
    int y =  (getRealHeight() - point.y + mapElementSize.width/2) / mapElementSize.height;
    if( x < 0 || y < 0 || x >= width || y >= height ) return;
    
    if (active <= EditingPaintedGrass4) {
        CCPoint pos = mapElements[layer][y][x]->getGrassSprite()->getPosition();
        
        layers[layer]->getMapLayer()->removeChild(mapElements[layer][y][x]->getGrassSprite());
        mapElements[layer][y][x]->setElement(active-1);
        if(mapElements[layer][y][x]->getGrassSprite()) {
            layers[layer]->getMapLayer()->addChild(mapElements[layer][y][x]->getGrassSprite());
            mapElements[layer][y][x]->getGrassSprite()->setPosition(pos);
        }
        
        update();
        return;
    }
    
    if (active == EditingLeftWall4 ) {
        CCPoint pos = ccpAdd( mapElements[layer][y][x]->getGrassSprite()->getPosition(),ccp(-7,7));
        bool UpWall = false;
        if(!mapElements[layer][y][x]->nearElements[Left]) {
            return;
        }
        if(!mapElements[layer][y][x]->nearElements[Up]) UpWall = true;
        
        mapElements[layer][y][x]->nearElements[Left]->nearElements[Right] = NULL;
        mapElements[layer][y][x]->nearElements[Left] = NULL;
        
        CCSprite * sprite = mapElements[layer][y][x]->getWallSprite();
        if(sprite)layers[layer]->getMapLayer()->removeChild(sprite);
        if ( x==0 || y == 0 ) {
            sprite = NULL;
        } else if(UpWall) {
            sprite = PMTextureCache::instanse()->getMapWallTile(2);
        } else {
            sprite = PMTextureCache::instanse()->getMapWallTile(0);
        }
        mapElements[layer][y][x]->setWallSprite(sprite);
        if(sprite) {
            sprite->setPosition(pos);
            layers[layer]->getMapLayer()->addChild(sprite,1);
        }
        
        update();
        return;
    }
    
    if (active == EditingUpWall4 ) {
        CCPoint pos = ccpAdd( mapElements[layer][y][x]->getGrassSprite()->getPosition(),ccp(-7,7));
        bool LeftWall = false;
        if(!mapElements[layer][y][x]->nearElements[Up]) {
            return;
        }
        if(!mapElements[layer][y][x]->nearElements[Left]) LeftWall = true;
        
        mapElements[layer][y][x]->nearElements[Up]->nearElements[Down] = NULL;
        mapElements[layer][y][x]->nearElements[Up] = NULL;
        
        CCSprite * sprite = mapElements[layer][y][x]->getWallSprite();
        if(sprite)layers[layer]->getMapLayer()->removeChild(sprite);
        if ( x==0 || y == 0 ) {
            sprite = NULL;
        } else if(LeftWall) {
            sprite = PMTextureCache::instanse()->getMapWallTile(2);
        } else {
            sprite = PMTextureCache::instanse()->getMapWallTile(1);
        }
        mapElements[layer][y][x]->setWallSprite(sprite);
        if(sprite) {
            sprite->setPosition(pos);
            layers[layer]->getMapLayer()->addChild(sprite,1);
        }
        
        update();
        return;
    }
    
    if (active == EditingNoLeftWall4 ) {
        CCPoint pos = ccpAdd( mapElements[layer][y][x]->getGrassSprite()->getPosition(),ccp(-7,7));
        bool UpWall = false;
        if(mapElements[layer][y][x]->nearElements[Left]) {
            return;
        }
        if ( x==0 || y == 0 ) return;
        
        if(!mapElements[layer][y][x]->nearElements[Up]) UpWall = true;
        
        mapElements[layer][y][x]->nearElements[Left] = mapElements[layer][y][x-1];
        mapElements[layer][y][x-1]->nearElements[Right] = mapElements[layer][y][x];
        
        CCSprite * sprite = mapElements[layer][y][x]->getWallSprite();
        if(sprite) {
            layers[layer]->getMapLayer()->removeChild(sprite);
        }
        
        if(UpWall) {
            sprite = PMTextureCache::instanse()->getMapWallTile(1);
        } else sprite = NULL;
        mapElements[layer][y][x]->setWallSprite(sprite);
        if(sprite) {
            sprite->setPosition(pos);
            layers[layer]->getMapLayer()->addChild(sprite,1);
        }
        
        update();
        return;
    }
    
    if (active == EditingNoUpWall4 ) {
        CCPoint pos = ccpAdd( mapElements[layer][y][x]->getGrassSprite()->getPosition(),ccp(-7,7));
        bool LeftWall = false;
        if(mapElements[layer][y][x]->nearElements[Up]) {
            return;
        }
        if ( x==0 || y == 0 ) return;
        
        if(!mapElements[layer][y][x]->nearElements[Left]) LeftWall = true;
        
        mapElements[layer][y][x]->nearElements[Up] = mapElements[layer][y-1][x];
        mapElements[layer][y-1][x]->nearElements[Down] = mapElements[layer][y][x];
        
        CCSprite * sprite = mapElements[layer][y][x]->getWallSprite();
        if(sprite)layers[layer]->getMapLayer()->removeChild(sprite);
        
        if(LeftWall) {
            sprite = PMTextureCache::instanse()->getMapWallTile(0);
        } else sprite = NULL;
        
        mapElements[layer][y][x]->setWallSprite(sprite);
        if(sprite) {
            sprite->setPosition(pos);
            layers[layer]->getMapLayer()->addChild(sprite,1);
        }
        
        update();
        return;
    }
}
#endif