//
//  GameLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//
// TODO LIST
//1) карты из xml - DONE
//2) кнопка плей:
//уровень с повторителем в главной функции, нажать плей, когда установлен повторитель и нет методов, падает - FIXED
//уровень с повторителем ПУСТЫМ, нажимаем плей, потом устанавливаем повторитель и методы, плей не пашет - FIXED
//3) левел 1.10 просто нажимаем плей, проигрываем, не работает кнопка заново - FIXED
//4) движение робота не через шедуль - LATER
//5) дергание карты при игре на маленьком экране - DONE
//6) можно просто много подряд тыкать в кнопку плей, он будет начинать заново выполнять программу - FIXED
//7) с изначальных позиций нельзя делать драгндроп - NOT TO DO
//8) экран вы проиграли не по центру получается
//9) нажать на пустую клетку для действие без выбранного действия - падение - FIXED
//10) прогрессбра скорости - FIXED
//11) конец программы достигается, но я не прогирываю - FIXED
//12) Прокрутка леера с функциями - DONE
//13) подстветка выполняющейся команды- DONE
//14) И надо как-то сделать чтобы константы определились сами в зависимости от устройства, и рисовались кнопки на айпаде больше - DONE
//15) Подсказки
//16) сохранение игры
//17) скругления на айфоне - можно зафигачить туда тайл с картинкой поворота:) - DONE

#include "GameLayer.h"
#include "ProgramLayer.h"
#include <pthread.h>
#include "SimpleAudioEngine.h"

#include "XmlWorldReader.h"
#include "XmlWorldSaver.h"

USING_NS_CC;

using namespace CocosDenshion;

CCScene* GameLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameLayer *layer = GameLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer,0,0);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    m_bKeypadEnabled=true;
    m_bTouchEnabled = true;
    
    gameLayerInit();

    if(GetGameParams->getContinueGame())
        continueGame();
    else
        loadWorld();
    
    return true;
}

void GameLayer::setEnabledForGame(bool state)
{
    speedBarLayer->setEnabled(state);
    programLayer->setEnabled(state);
    changeLevelMenu->setEnabled(state);
    
    programControlLayer->getStartProgramButton()->setEnabled(state);
    programControlLayer->getResetButton()->setEnabled(state);
}

void GameLayer::setEnabled(bool state)
{
    programLayer->setEnabled(state);
    speedBarLayer->setEnabled(state);
    world->getMap()->setEnabled(state);
    
    if(methodStackLayer != NULL)
        methodStackLayer->setEnabled(state);
    
    programControlLayer->setEnabled(state);
    changeLevelMenu->setEnabled(state);
}

void GameLayer::saveStates()
{
    programLayerState = programLayer->isEnabled();
    speedBarState = speedBarLayer->isEnabled();
    mapState = world->getMap()->isEnabled();
    
    if(methodStackLayer)methodStackLayer->saveStates();
    programControlLayer->saveStates();
    changeLevelMenu->saveStates();
}

void GameLayer::restoreStates()
{
    programLayer->setEnabled(programLayerState);
    speedBarLayer->setEnabled(speedBarState);
    world->getMap()->setEnabled(mapState);

    if(methodStackLayer)methodStackLayer->restoreStates();
    programControlLayer->restoreStates();
    changeLevelMenu->restoreStates();
}

void GameLayer::continueLevel(CCObject *object)
{
    if(pauseLayer != NULL)
    {
        if(pauseButton)pauseButton->setEnabled(true);
        pauseLayer->hide();
        pauseLayer = NULL;
    }
    
    restoreStates();
    
    if(world->getBaseRobotManager()->getState() == RobotManager::Working)
        programControlLayer->getStartProgramButton()->setEnabled(false);
    
    resumeSchedulerAndActions();
}

void GameLayer::restartLevel(CCObject *object)
{
    setEnabled(true);
    
    programControlLayer->getResetButton()->setEnabled(false);
    
    programLayer->reset();
    
    programStarted = false;
    
    world->getMap()->clean();
    world->getMap()->update();
    
    
    for (int i = 0; i < world->robotCount(); ++i)
    {
        world->robotManagerAt(i)->clear();
        world->robotAt(i)->cleanTempMethods();
        world->robotAt(i)->setState(UniversalRobot::Ready);
        world->robotAt(i)->setStepCount(0);
        world->robotManagerAt(i)->setState(RobotManager::Paused);
    }
    
    clearHighlitedButton();
    
    unscheduleAllSelectors();
    
    
    if(methodStackLayer)methodStackLayer->clear();
    
    if(winLayer != NULL)
    {
        winLayer->hide();
        winLayer = NULL;
    }
    
    if(pauseLayer != NULL)
    {
        pauseLayer->hide();
        pauseLayer = NULL;
    }
    
    if(looseLayer != NULL)
    {
        looseLayer->hide();
        looseLayer = NULL;
    }
    
    highlightedLayer = -1;

    schedule(schedule_selector(GameLayer::makeStep), 0.01f);
}


void GameLayer::continueGame()
{
    loadWorld(writablePath + "save.map");
}


void GameLayer::saveMap()
{
    XMLWorldSaver xml(world, this ,writablePath + "save.map");
    GetGameParams->setGameStarted(true);
}

void GameLayer::drawHint()
{
#ifndef MAP_EDITOR
    if(hint->hintParts.size() == 0)
        return;
    
    HintLayer *hintLayer = HintLayer::create(hint->hintParts[0].hintText);
    addChild(hintLayer,20);
#else
    if(hint->hintParts.size() == 0)
    {
        HintPart part;
        part.type = MethodHint;
        part.hintText = "";
        hint->hintParts.push_back(part);
    }
    hintLayer = HintEditLayer::create(hint->hintParts[0].hintText, "Подсказка");
    addChild(hintLayer,20);
#endif
}

void GameLayer::loadWorld(std::string mapPath)
{
    
    XmlWorldReader *xml= new XmlWorldReader;
    world = xml->readWorld(this,mapPath);
    
    if(world != NULL && programLayer != NULL)
    {
        world->getBaseRobotManager()->setGameLayer(this);
        world->getBaseRobotManager()->setState(RobotManager::Paused);
        schedule(schedule_selector(GameLayer::makeStep), 0.01f);
        
        GameParams::getInstanse()->world = world;
        
        //parallax->addChild(world->getMap(), -1, ccp(1.0f,1.0f), CCPointZero);
        addChild( world->getMap(), 0);

        programStarted = false;
        
        setEnabled(true);
        programControlLayer->getResetButton()->setEnabled(false);
        
#ifdef MAP_EDITOR
        if (hint == NULL) {
            hint = Hint::create();
        }
#endif
        if(hint != NULL)
            drawHint();
        
#ifndef MAP_EDITOR
        methodStackLayer = MethodStackLayer::create(this, world->getBaseRobotManager());
        addChild(methodStackLayer , 10);
#else
        methodStackLayer = NULL;
#endif
    }
    else
    {
        char errorString[255];
        
        sprintf(errorString, "Load map error!\n%s",xml->getError().c_str());
        CCMessageBox(errorString, "Error");
    }
    
    delete xml;
}

void GameLayer::loadWorld()
{
    int worldPart = GetGameParams->getWorldPart();
    int level = GetGameParams->getLevel();
    CCString mapPathString;
    mapPathString.initWithFormat("%s/World%d/Lvl%d.map", GameParams::getInstanse()->mapPath.m_sString.c_str(), worldPart, level);
    
    std::string mapPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(mapPathString.m_sString.c_str());


    loadWorld(mapPath);
#ifndef MAP_EDITOR
    if(world != NULL)
        saveMap();
#endif
}


void GameLayer::gameLayerInit()
{
    
    CCSize screenSize = GetGameParams->screenSize;
    
    setPosition(CCPointZero);

	CCSize mapSize = GetGameParams->mapSize();

    speedBarLayer = SpeedBarLayer::create();
    
    addChild(speedBarLayer,2);    
    
    CCString * worldLabel = CCString::createWithFormat(CCLocalizedString("WorldAndLevelPattern", "NotLocalized"),
                                                     GetGameParams->getWorldPart()+1,
                                                     GetGameParams->getLevel(),
                                                     GetGameParams->worlds[GetGameParams->getWorldPart()]);
    
    CCLabelTTF * labelWorld = CCLabelTTF::create(worldLabel->getCString(), "Marker Felt", 18);
    labelWorld->setAnchorPoint( CCPointZero );
    labelWorld->setPosition( ccp(10,
                                 mapSize.height - 10 - labelWorld->getContentSize().height) );
    
    speedBarLayer->setPosition( ccp(mapSize.width / 4 - 40,
                                    mapSize.height - speedBarLayer->getContentSize().height - 20) );
    
    addChild(labelWorld, 2, 2);
    
    programControlLayer = ProgramControlLayer::create(this);
    
    programControlLayer->getResetButton()->setEnabled(false);
    
    programControlLayer->setPosition( ccp(mapSize.width - programControlLayer->getContentSize().width - 25,
                                          mapSize.height - programControlLayer->getContentSize().height - 10) );
    
    
    addChild(programControlLayer, 10);
    
    changeLevelMenu = ChangeLevelMenu::create();
    
    addChild(changeLevelMenu, 10);

#ifndef MAP_EDITOR
    pauseButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getPauseIconSprite(),
                                           PMTextureCache::instanse()->getPauseIconSprite_Selected(),
                                           PMTextureCache::instanse()->getPauseIconSprite_Disabled(),
                                           this, menu_selector(GameLayer::pause));
    
    soundOffButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getSoundOffIconSprite(),
                                           PMTextureCache::instanse()->getSoundOffIconSprite_Selected(),
                                           PMTextureCache::instanse()->getSoundOffIconSprite_Disabled(),
                                           this, menu_selector(GameLayer::toggleSound));
    
    soundOnButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getSoundOnIconSprite(),
                                              PMTextureCache::instanse()->getSoundOnIconSprite_Selected(),
                                              PMTextureCache::instanse()->getSoundOnIconSprite_Disabled(),
                                              this, menu_selector(GameLayer::toggleSound));
    
    pauseButton->setPosition( CCPointZero );
    soundOffButton->setPosition( ccp(pauseButton->getContentSize().width + 10,0) );
    soundOnButton->setPosition( ccp(pauseButton->getContentSize().width + 10,0) );
    
    soundOffButton->setVisible(!GetGameParams->getSoundState());
    soundOnButton->setVisible(GetGameParams->getSoundState());
    
    CCMenu *topMenu = CCMenu::create(pauseButton,soundOffButton, soundOnButton, NULL);
    
    topMenu->setContentSize( CCSizeMake(pauseButton->getContentSize().width, pauseButton->getContentSize().height) );
    topMenu->setAnchorPoint( CCPointZero );
    topMenu->setPosition( ccp (mapSize.width / 2, mapSize.height - topMenu->getContentSize().height / 2 - 10) );

    addChild(topMenu, 10);
#endif
}

void GameLayer::start(CCObject *object)
{
    setEnabledForGame(false);
    
    for(int j = 0 ; j < world->robotCount(); ++j)
    {
        world->robotManagerAt(j)->setState(RobotManager::Working);
        
        if(!programStarted)
        {
            
            if(world->robotAt(j) == world->getBaseRobot())
            {
                for(int i = 0; i < programLayer->layers.size(); ++i)
                {
                    std::pair<int,RobotMethod> method(programLayer->layers[i]->getMethodID(), RobotMethod(world->getBaseRobot(), programLayer->layers[i]->makeMethod(),  RobotMethod::TempMethod));
                    world->getBaseRobot()->methodList.insert(method);
                    
                    std::pair<int,RobotMethod> execMethod(programLayer->layers[i]->getMethodID_Exec(), RobotMethod(world->getBaseRobot(), programLayer->layers[i]->makeExecMethod(),  RobotMethod::TempMethod));
                    
                    world->getBaseRobot()->methodList.insert(execMethod);
                }
                
                
            }
            
            if(!world->robotAt(j)->getMethodList().empty())
                world->robotManagerAt(j)->addInstructions(world->robotAt(j)->getMethod(CMDMAIN_EXECUTE).instructionList);
            
        }
        
    }
    
    if(!programStarted)
        programStarted = true;

}

void GameLayer::makeProgramStep(CCObject *object)
{
    setEnabledForGame(false);
    programControlLayer->getStartProgramButton()->setEnabled(true);
    
    if(!programStarted)
        start(NULL);
    
    for(int j = 0 ; j < world->robotCount(); ++j)
    {
        world->robotManagerAt(j)->setState(RobotManager::Controlled);
    }
}

void GameLayer::makeStep(float s)
{
    for (int i = 0; i < world->robotCount(); ++i)
    {
        world->robotManagerAt(i)->loop();
    }
}

void GameLayer::winLayerInit()
{
    winLayer = RobotWinLayer::create(this,world->getBaseRobot()->getStepCount());
    addChild(winLayer, 20);
    winLayer->show();
}

void GameLayer::looseLayerInit()
{
    looseLayer = RobotLooseLayer::create(this);
    addChild(looseLayer, 20);
    looseLayer->show();
}

void GameLayer::pauseLayerInit(bool isRobotLoose)
{
    pauseLayer = PauseLayer::create(this, isRobotLoose);
    addChild(pauseLayer, 20);
    
    if(pauseButton)pauseButton->setEnabled(false);
    pauseLayer->show();
}

void GameLayer::robotWin()
{
    setEnabled(false);
    programControlLayer->getResetButton()->setEnabled(true);
    changeLevelMenu->setEnabled(true);
        
    unscheduleAllSelectors();
    
    winLayerInit();
    

    if(GetGameParams->getSoundState())
        SimpleAudioEngine::sharedEngine()->playEffect("Sounds/win_sound.wav");
}

void GameLayer::robotLoose()
{
    setEnabled(false);
    changeLevelMenu->setEnabled(true);
    programControlLayer->getResetButton()->setEnabled(true);
    
    unscheduleAllSelectors();
    
    highLightBreakButon();
    
    
    looseLayerInit();
    
    if(GetGameParams->getSoundState())
        SimpleAudioEngine::sharedEngine()->playEffect("Sounds/loose_sound.wav");
}

void GameLayer::robotFailure()
{
    setEnabled(false);
    changeLevelMenu->setEnabled(true);
    programControlLayer->getResetButton()->setEnabled(true);
    
    unscheduleAllSelectors();
}

void GameLayer::pause(CCObject *object)
{
    saveStates();
    
    setEnabled(false);
    if(pauseButton)pauseButton->setEnabled(false);
    
    
    pauseSchedulerAndActions();
    
    pauseLayerInit(world->getBaseRobot()->getState() == UniversalRobot::Broken);
}

void GameLayer::toggleSound(CCObject *object)
{
    GetGameParams->setSoundState(!GetGameParams->getSoundState());
    
    soundOffButton->setVisible(!GetGameParams->getSoundState());
    soundOnButton->setVisible(GetGameParams->getSoundState());
    
    if(!GetGameParams->getSoundState())
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    else
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void GameLayer::highLightBreakButon()
{
    if(highlightedButton != -1 && highlightedLayer != -1)
    {
        MethodLayer *layer = programLayer->layers[highlightedLayer];
        layer->highlightMethodBreak(highlightedButton);
    }
}

void GameLayer::highLightButon(int layerIndex,int button)
{
    if(highlightedLayer != -1)
        clearHighlitedButton();
    
    highlightedLayer = layerIndex;
    highlightedButton = button;
    
    MethodLayer *layer = programLayer->layers[highlightedLayer];
    layer->highlightMethod(highlightedButton);
}


void GameLayer::clearHighlitedButton()
{
    if(highlightedLayer == -1)
        return;
    
    MethodLayer *layer = programLayer->layers[highlightedLayer];
    layer->clearHighlightMethod(highlightedButton);
}
