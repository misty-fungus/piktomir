//
//  GameParams.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__GameParams__
#define __pictomir_cocos2d_x__GameParams__

//#include <iostream>
#include "cocos2d.h"
#include <libxml/xmlreader.h>
#include <string>
#include <stdio.h>
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include <direct.h>
#endif
#include "CCLocalizedString.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#define PICT_PATH "Picts/"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#define PICT_PATH ""
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_MAC&&IOS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define PICT_PATH "Picts/"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#define PICT_PATH "Picts/"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_LINUX

#define  GetGameParams GameParams::getInstanse()

#define SYNTHESIZE_GAMEPARAM(varType, varName, funName) \
protected: varType varName; \
public: varType get##funName(void) const { return varName; } \
public: void set##funName(varType var) { \
    varName = var; \
    set##varType##ValueForKey(#varName, varName); \
    }

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: varType get##funName(void) const { return varName; }\
public: void set##funName(varType var){ varName = var; }

#define NodeContent(Node) (char*)xmlNodeGetContent(Node->children)
#define NodeProp(Node,Prop) (char*)xmlGetProp(Node, (xmlChar*)Prop)
#define getMethodLayers GameParams::getInstanse()->world->layer->getProgramLayer()->layers

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define writablePath (cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+"Piktomir/")
#else
#define writablePath cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define platform_mkdir(path) _mkdir(path)
#else
#define platform_mkdir(path) mkdir(path, S_IRWXU)
#endif

const float SYSTEM_ANIMATION_DELAY = 0.4f;

const int REPEATER_BUTTON_NUM = 8;

const int MaxRepeater = 200;

const int DISPLAYED_LAYERS = 4;
const float LAYER_POSITION_X_DELTA = 200.0f;
const float LAYER_POSITION_TANGENS = M_PI/4.0f;

const float EPS = 0.00000001f;

#define GET_I_BY_X(x) ((x)/LAYER_POSITION_X_DELTA)*((x)/LAYER_POSITION_X_DELTA)
#define LAYER_OPACITY(x) (fabs(x) < EPS) ? 255 : atan(1.0f/1.2f)/atan(GET_I_BY_X(x)/1.2) * 200.0f
#define LAYER_POSITION_X(i) LAYER_POSITION_X_DELTA * sqrt(i)
#define LAYER_POSITION_Y(x,centerX,centerY) (x - centerX) * tan(LAYER_POSITION_TANGENS) + centerY
#define SCALE_FUNCTION(x) 1.0f - atan(GET_I_BY_X(x)/1.2f)/atan(1.0f/1.2f) * 0.15f

class UniversalWorld;

class GameParams
{
public:
    GameParams():
        mapVersion(0),
        velocity(0.4),
        minVelocity(0.2),
        maxVelocity(1.0),
        screenSize(cocos2d::CCSize()),
#ifndef MAP_EDITOR
        programLayerWidth(300),
#else
        programLayerWidth(350),
#endif
        mapTileSize(64,64),
        world(NULL),
        appPath(cocos2d::CCString()),
        mapPath(cocos2d::CCString()),
        worldPart(0),
        level(1),
        options(NULL),
        optionsNode(NULL),
        optionsPath(writablePath + "GameParams.xml"),
        gameStarted(false),
        backgroundVolume(1.0f),
        effectVolume(1.0f),
        soundState(true),
        language(cocos2d::kLanguageRussian)
    {
        if (isXMLFileExist()) {
            options = xmlReadFile(optionsPath.c_str(),NULL,0);
            optionsNode = xmlDocGetRootElement(options);
        } else {
            options = xmlNewDoc((xmlChar*)"1.0");
            optionsNode = xmlNewNode(NULL, (xmlChar*)"options");
            xmlDocSetRootElement(options, optionsNode);
        }
        save();
        
        mapVersion = getintValueForKey("mapVersion", 0);
        worldPart = getintValueForKey("worldPart", 0);
        level = getintValueForKey("level", 1);
        gameStarted = getboolValueForKey("gameStarted", false);
        backgroundVolume = getdoubleValueForKey("backgroundVolume", 1.0);
        effectVolume = getdoubleValueForKey("effectVolume", 1.0);
        soundState = getboolValueForKey("soundState", true);
        velocity = getdoubleValueForKey("velocity", 0.4);
        language = getintValueForKey("language", cocos2d::CCApplication::sharedApplication()->getCurrentLanguage());
    }
    
    ~GameParams ()
    {
        save();
    }
    
    SYNTHESIZE_GAMEPARAM(int,mapVersion,MapVersion);
    
    double minVelocity;
    double maxVelocity;
    
    cocos2d::CCSize mapTileSize;
    
    cocos2d::CCSize screenSize;
    int programLayerWidth;
    
    UniversalWorld *world;
    
    cocos2d::CCString appPath;
    cocos2d::CCString mapPath;
    
    static GameParams *instanse;
    
    static GameParams *getInstanse();
    
    cocos2d::CCSize mapSize()
    {
        cocos2d::CCSize screen = screenSize;
        screen.width -= programLayerWidth;
        return screen;
    }
    
    SYNTHESIZE_GAMEPARAM(int,worldPart,WorldPart);
    SYNTHESIZE_GAMEPARAM(int,level,Level);
    SYNTHESIZE_GAMEPARAM(bool,gameStarted,GameStarted);
    SYNTHESIZE_GAMEPARAM(double,backgroundVolume,BackgroundVolume);
    SYNTHESIZE_GAMEPARAM(double,effectVolume,EffectVolume);
    SYNTHESIZE_GAMEPARAM(bool,soundState, SoundState);
    SYNTHESIZE_GAMEPARAM(int,language,Language);
    SYNTHESIZE_GAMEPARAM(double,velocity,Velocity);
    
    
    SYNTHESIZE(bool, continueGame, ContinueGame);
    
    void loadWorlds();
    
public:
    std::vector<int> worlds;
    void incWorldPart(void) {worldPart++;setWorldPart(worldPart);}
    void incLevel(void) {level++;setLevel(level);}
    
    void decWorldPart(void) {worldPart--;setWorldPart(worldPart);}
    void decLevel(void) {level--;setLevel(level);}
    // Prepearing to XML
    
private:
    xmlDocPtr options;
    xmlNodePtr optionsNode;
    std::string optionsPath;
    void save();
    bool isXMLFileExist();
    xmlNodePtr getXMLNodeForKey (const char* pKey);
    void setValueForKey(const char* pKey, const char* pValue);
    const char* getValueForKey(const char* pKey);
    
    void setintValueForKey(const char* pKey, int value);
    int getintValueForKey(const char* pKey, int defValue);
    void setdoubleValueForKey(const char* pKey, double value);
    double getdoubleValueForKey(const char* pKey, double defValue);
    void setboolValueForKey(const char* pKey, bool value);
    bool getboolValueForKey(const char* pKey, bool defValue);
};

#endif /* defined(__pictomir_cocos2d_x__GameParams__) */
