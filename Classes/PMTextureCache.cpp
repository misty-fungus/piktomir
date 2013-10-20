//
//  PMTextureCache.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/9/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "PMTextureCache.h"
#include "Robot4.h"

USING_NS_CC;
using namespace std;

PMTextureCache *PMTextureCache::_instanse = NULL;

PMTextureCache *PMTextureCache::instanse()
{
    if(_instanse == NULL)
    {
        _instanse = new PMTextureCache;
        _instanse->init();
    }
    
    return _instanse;
}

void PMTextureCache::deleteInstanse()
{
    CC_SAFE_DELETE(_instanse);
}

PMTextureCache::~PMTextureCache()
{
    CCTextureCache::sharedTextureCache()->removeAllTextures();
}

void PMTextureCache::initMapTexture(WorldType type,int tileset)
{
    worldType = type;
    
    switch(worldType)
    {
        case pmWorld4 :
            mapTexture = cocos2d::CCString::createWithFormat(PICT_PATH "MapsPics/world4_tileset%d.png",tileset)->m_sString.c_str();
            mapTexture_Disabled = cocos2d::CCString::createWithFormat(PICT_PATH "MapsPics/world4_tileset%d_Disabled.png",tileset)->m_sString.c_str();
            mapEditTexture = cocos2d::CCString::createWithFormat(PICT_PATH "MapsPics/world4_tileset%d_Edit.png",tileset)->m_sString.c_str();
            break;
        default: break;
    }
}

void PMTextureCache::initRobotTextures(UniversalWorld *world)
{
    if(!robotsLoaded)
    {
    
        for(int i = 0; i < world->robotCount(); ++i)
        {
            switch(world->robotAt(i)->type())
            {
                case pmRobot4:
                    robotTextures[pmRobot4] = PICT_PATH "Robot/robot4.png";
                    break;
                case pmOven4:
                    robotTextures[pmOven4] = PICT_PATH "Robot/oven4.png";
                    break;
                case pmTrasportRobot4:
                    robotTextures[pmTrasportRobot4] = "";
                default: break;
            }
        }
    
        robotsLoaded = true;
    }
    
    if(world->getBaseRobot() != NULL)
    {
        switch(world->getBaseRobot()->type())
        {
            case pmRobot4:                
                robotMethodTextures.resize(world->getBaseRobot()->getNativeMethodCount());
                robotMethodTextures_Selected.resize(world->getBaseRobot()->getNativeMethodCount());
                robotMethodTextures_Disabled.resize(world->getBaseRobot()->getNativeMethodCount());
                
                robotConditionTextures.resize(world->getBaseRobot()->getNativeMethodCount());
                robotConditionTextures_Selected.resize(world->getBaseRobot()->getNativeMethodCount());
                robotConditionTextures_Disabled.resize(world->getBaseRobot()->getNativeMethodCount());
                
                TEXTURECACHE_SET_TEXTURE(robotMethodTextures,Robot4::TurnLeft,"Robot Methods/left")
                TEXTURECACHE_SET_TEXTURE(robotMethodTextures,Robot4::TurnRight,"Robot Methods/right")
                TEXTURECACHE_SET_TEXTURE(robotMethodTextures,Robot4::Move,"Robot Methods/forward")
                TEXTURECACHE_SET_TEXTURE(robotMethodTextures,Robot4::Paint,"Robot Methods/paint")
                
                TEXTURECACHE_SET_TEXTURE(robotConditionTextures,Robot4::GrassIsGreen,"Conditions/cond1")
                TEXTURECACHE_SET_TEXTURE(robotConditionTextures,Robot4::GrassIsBlue,"Conditions/cond2")
                TEXTURECACHE_SET_TEXTURE(robotConditionTextures,Robot4::NoWall,"Conditions/cond3")
                TEXTURECACHE_SET_TEXTURE(robotConditionTextures,Robot4::Wall,"Conditions/cond4")
                
                break;
            case pmOven4:
                break;
            default: break;
        }

        robotMethodTextures[EMPTY_METHOD] = PICT_PATH "Robot Methods/empty_method.png";
        robotMethodTextures_Selected[EMPTY_METHOD] = PICT_PATH "Robot Methods/empty_method.png";
        robotMethodTextures_Disabled[EMPTY_METHOD] = PICT_PATH "Robot Methods/empty_method.png";
        
        robotConditionTextures[EMPTY_CONDITION] = PICT_PATH "Conditions/cond.png";
        robotConditionTextures_Selected[EMPTY_CONDITION] = PICT_PATH "Conditions/cond_Selected.png";
        robotConditionTextures_Disabled[EMPTY_CONDITION] = PICT_PATH "Conditions/cond_Disabled.png";
        
    }
}

CCSprite *PMTextureCache::getMapGrassTile(int tile)
{
    switch(worldType)
    {
        case pmWorld4: return CCSprite::create(mapTexture.c_str(), CCRectMake(tile * 64.0, 0.0, 64.0, 64.0));
        default: return NULL;
    }
}

CCSprite *PMTextureCache::getMapWallTile(int wall)
{
    switch(worldType)
    {
        case pmWorld4: return CCSprite::create(mapTexture.c_str(), CCRectMake(4*64.0 + wall * 78.0, 0.0, 78.0, 78.0));
        default: return NULL;
    }
}

CCSprite *PMTextureCache::getMapEditGrassTile(int tile)
{
    switch(worldType)
    {
        case pmWorld4: return CCSprite::create(mapEditTexture.c_str(), CCRectMake((tile+1) * 64.0, 0.0, 64.0, 64.0));
        default: return NULL;
    }
}

CCSprite *PMTextureCache::getMapEditWallTile(int wall)
{
    switch(worldType)
    {
        case pmWorld4: return CCSprite::create(mapEditTexture.c_str(), CCRectMake(5*64.0 + wall * 78.0, 0.0, 78.0, 78.0));
        default: return NULL;
    }
}

CCSprite *PMTextureCache::getMapGrassDisabledTile(int tile)
{
    switch(worldType)
    {
        case pmWorld4: return CCSprite::create(mapTexture_Disabled.c_str(), CCRectMake((tile+1) * 64.0, 0.0, 64.0, 64.0));
        default: return NULL;
    }
}

CCSprite *PMTextureCache::getMapWallDisabledTile(int wall)
{
    switch(worldType)
    {
        case pmWorld4: return CCSprite::create(mapTexture_Disabled.c_str(), CCRectMake(5*64.0 + wall * 78.0, 0.0, 78.0, 78.0));
        default: return NULL;
    }
}

CCSprite *PMTextureCache::getProgramSprite(int cmd)
{
    switch(cmd)
    {
        case CMDA_EXECUTE:return getProgramASprite();
        case CMDB_EXECUTE:return getProgramBSprite();
        case CMDC_EXECUTE:return getProgramCSprite();
    }
    
    return NULL;
}

CCSprite *PMTextureCache::getProgramSprite_Selected(int cmd)
{
    switch(cmd)
    {
        case CMDA_EXECUTE:return getProgramASprite_Selected();
        case CMDB_EXECUTE:return getProgramBSprite_Selected();
        case CMDC_EXECUTE:return getProgramCSprite_Selected();
    }
    
    return NULL;
}

CCSprite *PMTextureCache::getProgramSprite_Disabled(int cmd)
{
    switch(cmd)
    {
        case CMDA_EXECUTE:return getProgramASprite_Disabled();
        case CMDB_EXECUTE:return getProgramBSprite_Disabled();
        case CMDC_EXECUTE:return getProgramCSprite_Disabled();
    }
    
    return NULL;
}

void PMTextureCache::init()
{
    robotTextures.resize(pmRobotCount);
    
    robotsLoaded = false;
    
    repeaterTextures.resize(REPEATER_BUTTON_NUM);
    repeaterTextures_Selected.resize(REPEATER_BUTTON_NUM);
    repeaterTextures_Disabled.resize(REPEATER_BUTTON_NUM);
    
    for(int i = 1; i < REPEATER_BUTTON_NUM; ++i)
    {
        string image = CCString::createWithFormat("Repeaters/rep%d",i)->m_sString;
        TEXTURECACHE_SET_TEXTURE(repeaterTextures,i,image.c_str())
    }
    
    TEXTURECACHE_CREATE_TEXTURE(programATexture,"Robot Methods/A");
    TEXTURECACHE_CREATE_TEXTURE(programBTexture,"Robot Methods/B");
    TEXTURECACHE_CREATE_TEXTURE(programCTexture,"Robot Methods/C");
    
    TEXTURECACHE_CREATE_TEXTURE(methodBackgroundTexture,"System/actionBackground");
    TEXTURECACHE_CREATE_TEXTURE(repeaterBackgroundTexture,"Repeaters/rep");
    TEXTURECACHE_CREATE_TEXTURE(conditionBackgroundTexture,"Conditions/cond");
    methodBackgroundTexture_Broken = PICT_PATH "System/actionBackground_Broken.png";
    
    lockTexture = PICT_PATH "System/lock.png";
    unlockTexture = PICT_PATH "System/Unlock.png";
    
    TEXTURECACHE_CREATE_TEXTURE(editIconTexture,"System/EditIcon");
    TEXTURECACHE_CREATE_TEXTURE(startIconTexture,"System/StartIcon");
    TEXTURECACHE_CREATE_TEXTURE(pauseIconTexture,"System/PauseIcon");
    TEXTURECACHE_CREATE_TEXTURE(stopIconTexture,"System/StopIcon");
    TEXTURECACHE_CREATE_TEXTURE(makeStepIconTexture,"System/MakeStepIcon");
    TEXTURECACHE_CREATE_TEXTURE(resetIconTexture,"System/ResetIcon");
    TEXTURECACHE_CREATE_TEXTURE(nextLevelIconTexture,"System/NextLevelIcon");
    TEXTURECACHE_CREATE_TEXTURE(prevLevelIconTexture,"System/PrevLevelIcon");
    TEXTURECACHE_CREATE_TEXTURE(showIconTexture,"System/ShowIcon");
    TEXTURECACHE_CREATE_TEXTURE(hideIconTexture,"System/HideIcon");
    TEXTURECACHE_CREATE_TEXTURE(pasteIconTexture,"System/PasteIcon");
    TEXTURECACHE_CREATE_TEXTURE(clearIconTexture,"System/ClearIcon");
    
    TEXTURECACHE_CREATE_TEXTURE(soundOffIconTexture,"System/SoundOffIcon");
    TEXTURECACHE_CREATE_TEXTURE(soundOnIconTexture,"System/SoundOnIcon");
    
    TEXTURECACHE_CREATE_TEXTURE(infoIconTexture,"System/InfoIcon");

}
