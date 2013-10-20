//
//  PMTextureCache.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/9/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__PMTextureCache__
#define __pictomir_cocos2d_x__PMTextureCache__

#include <iostream>
#include "cocos2d.h"
#include "UniversalWorld.h"
#include "UniversalRobot.h"


#define TEXTURECACHE_SYNTHESIZE_VECTOR(varName, funName)\
protected:  std::vector<std::string> varName;\
protected:  std::vector<std::string> varName##_Selected;\
protected:  std::vector<std::string> varName##_Disabled;\
public: cocos2d::CCTexture2D * get##funName##Texture(int var)\
{ return  cocos2d::CCTextureCache::sharedTextureCache()->addImage(varName[var].c_str()); }\
public: cocos2d::CCTexture2D * get##funName##Texture_Selected(int var) \
{ return  cocos2d::CCTextureCache::sharedTextureCache()->addImage(varName##_Selected[var].c_str()); }\
public: cocos2d::CCTexture2D * get##funName##Texture_Disabled(int var) \
{ return  cocos2d::CCTextureCache::sharedTextureCache()->addImage(varName##_Disabled[var].c_str()); }\
public: cocos2d::CCSprite * get##funName##Sprite(int var) { return  cocos2d::CCSprite::create(varName[var].c_str()); }\
public: cocos2d::CCSprite * get##funName##Sprite_Selected(int var) { return  cocos2d::CCSprite::create(varName##_Selected[var].c_str()); }\
public: cocos2d::CCSprite * get##funName##Sprite_Disabled(int var) { return  cocos2d::CCSprite::create(varName##_Disabled[var].c_str()); }

#define TEXTURECACHE_SET_TEXTURE(varName, index, iconName)\
varName[index] = (string(PICT_PATH)+ string(iconName)+".png").c_str();\
varName##_Selected[index] = (string(PICT_PATH)+ string(iconName)+"_Selected.png").c_str();\
varName##_Disabled[index] = (string(PICT_PATH)+ string(iconName)+"_Disabled.png").c_str();



#define TEXTURECACHE_SYNTHESIZE(varName, funName)\
protected:  std::string varName;\
protected:  std::string varName##_Selected;\
protected:  std::string varName##_Disabled;\
public: cocos2d::CCTexture2D * get##funName##Texture(void) \
{ return  cocos2d::CCTextureCache::sharedTextureCache()->addImage(varName.c_str()); }\
public: cocos2d::CCTexture2D * get##funName##Texture_Selected(void) \
{ return  cocos2d::CCTextureCache::sharedTextureCache()->addImage(varName##_Selected.c_str()); }\
public: cocos2d::CCTexture2D * get##funName##Texture_Disabled(void) \
{ return  cocos2d::CCTextureCache::sharedTextureCache()->addImage(varName##_Disabled.c_str()); } \
public: cocos2d::CCSprite * get##funName##Sprite(void) { return  cocos2d::CCSprite::create(varName.c_str()); }\
public: cocos2d::CCSprite * get##funName##Sprite_Selected(void) { return  cocos2d::CCSprite::create(varName##_Selected.c_str()); }\
public: cocos2d::CCSprite * get##funName##Sprite_Disabled(void) { return  cocos2d::CCSprite::create(varName##_Disabled.c_str()); }

#define TEXTURECACHE_CREATE_TEXTURE(varName, iconName)\
varName = PICT_PATH iconName".png";\
varName##_Selected = PICT_PATH iconName"_Selected.png";\
varName##_Disabled = PICT_PATH iconName"_Disabled.png";


class PMTextureCache
{
private:
    std::string mapTexture;
    std::string mapTexture_Disabled;
    std::string mapEditTexture;
    std::vector<std::string> robotTextures;
    
    TEXTURECACHE_SYNTHESIZE_VECTOR(robotMethodTextures, RobotMethod)
    TEXTURECACHE_SYNTHESIZE_VECTOR(repeaterTextures, Repeater)
    TEXTURECACHE_SYNTHESIZE_VECTOR(robotConditionTextures, RobotCondition)
    
    std::string lockTexture;
    std::string unlockTexture;

    TEXTURECACHE_SYNTHESIZE(methodBackgroundTexture,MethodBackground);
    std::string methodBackgroundTexture_Broken;
    
    TEXTURECACHE_SYNTHESIZE(repeaterBackgroundTexture,RepeaterBackground);
    TEXTURECACHE_SYNTHESIZE(conditionBackgroundTexture,ConditionBackground);
    
    WorldType worldType;
    
    bool robotsLoaded;

    TEXTURECACHE_SYNTHESIZE(programATexture, ProgramA)
    TEXTURECACHE_SYNTHESIZE(programBTexture, ProgramB)
    TEXTURECACHE_SYNTHESIZE(programCTexture, ProgramC)
    
    TEXTURECACHE_SYNTHESIZE(startIconTexture, StartIcon)
    TEXTURECACHE_SYNTHESIZE(editIconTexture, EditIcon)
    TEXTURECACHE_SYNTHESIZE(pauseIconTexture, PauseIcon)
    TEXTURECACHE_SYNTHESIZE(makeStepIconTexture, MakeStepIcon)
    TEXTURECACHE_SYNTHESIZE(stopIconTexture, StopIcon)
    TEXTURECACHE_SYNTHESIZE(resetIconTexture, ResetIcon)
    TEXTURECACHE_SYNTHESIZE(nextLevelIconTexture, NextLevelIcon)
    TEXTURECACHE_SYNTHESIZE(prevLevelIconTexture, PrevLevelIcon)
    TEXTURECACHE_SYNTHESIZE(showIconTexture, ShowIcon)
    TEXTURECACHE_SYNTHESIZE(hideIconTexture, HideIcon)
    TEXTURECACHE_SYNTHESIZE(pasteIconTexture, PasteIcon)
    TEXTURECACHE_SYNTHESIZE(clearIconTexture, ClearIcon)
    
    TEXTURECACHE_SYNTHESIZE(soundOffIconTexture, SoundOffIcon)
    TEXTURECACHE_SYNTHESIZE(soundOnIconTexture, SoundOnIcon)
    
    TEXTURECACHE_SYNTHESIZE(infoIconTexture, InfoIcon)
    
public:
    
    ~PMTextureCache();
    
    void initMapTexture(WorldType type,int tileset);
    void initRobotTextures(UniversalWorld *world);
    void init();
    
    cocos2d::CCSprite *getProgramSprite(int cmd);
    cocos2d::CCSprite *getProgramSprite_Selected(int cmd);
    cocos2d::CCSprite *getProgramSprite_Disabled(int cmd);
    
    cocos2d::CCSprite *getMapGrassTile(int tile);
    cocos2d::CCSprite *getMapWallTile(int wall);
    
    cocos2d::CCSprite *getMapEditGrassTile(int tile);
    cocos2d::CCSprite *getMapEditWallTile(int wall);
    cocos2d::CCSprite *getMapGrassDisabledTile(int tile);
    cocos2d::CCSprite *getMapWallDisabledTile(int wall);
    
    cocos2d::CCTexture2D *getRobotTexture(int type)
    {
        return cocos2d::CCTextureCache::sharedTextureCache()->addImage(robotTextures[type].c_str());
    }
    
    cocos2d::CCSprite *getLockSprite()
    {
        return cocos2d::CCSprite::create(lockTexture.c_str());
    }
    
    cocos2d::CCSprite *getUnlockSprite()
    {
        return cocos2d::CCSprite::create(unlockTexture.c_str());
    }
    
    cocos2d::CCTexture2D *getMethodBackgroundTexture_Broken()
    {
        return cocos2d::CCTextureCache::sharedTextureCache()->addImage(methodBackgroundTexture_Broken.c_str());
    }
    
    cocos2d::CCSprite *getMethodBackgroundSprite_Broken()
    {
        return cocos2d::CCSprite::create(methodBackgroundTexture_Broken.c_str());
    }
    
    static PMTextureCache *_instanse;
    static PMTextureCache *instanse();
    static void deleteInstanse();
};

#endif /* defined(__pictomir_cocos2d_x__PMTextureCache__) */
