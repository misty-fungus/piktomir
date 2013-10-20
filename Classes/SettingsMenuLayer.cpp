//
//  SettingsMenuLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Admin on 7/31/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "SettingsMenuLayer.h"
#include "utils.h"
#include "GameParams.h"
#include "MainMenuLayer.h"
#include "PMTextureCache.h"

#include "SimpleAudioEngine.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

CCScene* SettingsMenuLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SettingsMenuLayer *layer = SettingsMenuLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer,0,0);
    
    // return the scene
    return scene;
}

bool SettingsMenuLayer::init()
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
    
    CCLabelTTF *soundLabel = CCLabelTTF::create(CCLocalizedString("Sound", "NotLocalized"), "Marker Felt", 26);
    
    soundLabel->setPosition( ccp(screenSize.width / 2, screenSize.height / 2 + 150) );
    
    addChild(soundLabel , 1);
    
    soundOffButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getSoundOffIconSprite(),
                                              PMTextureCache::instanse()->getSoundOffIconSprite_Selected(),
                                              PMTextureCache::instanse()->getSoundOffIconSprite_Disabled(),
                                              this, menu_selector(SettingsMenuLayer::toggleSound));
    
    soundOnButton = CCMenuItemSprite::create(PMTextureCache::instanse()->getSoundOnIconSprite(),
                                             PMTextureCache::instanse()->getSoundOnIconSprite_Selected(),
                                             PMTextureCache::instanse()->getSoundOnIconSprite_Disabled(),
                                             this, menu_selector(SettingsMenuLayer::toggleSound));
    
    soundOffButton->setPosition( CCPointZero );
    soundOnButton->setPosition( CCPointZero );
    
    soundOffButton->setVisible(!GetGameParams->getSoundState());
    soundOnButton->setVisible(GetGameParams->getSoundState());
    
    CCMenu *soundMenu = CCMenu::create(soundOffButton, soundOnButton, NULL);
    
    soundMenu->setPosition( ccp(screenSize.width / 2 + soundLabel->getContentSize().width, screenSize.height / 2 + 145) );
    addChild(soundMenu , 1);
    
    CCLabelTTF *backgroundVolumeLabel = CCLabelTTF::create(CCLocalizedString("BackgroundVolume", "NotLocalized"), "Marker Felt", 20);
    CCLabelTTF *effectVolumeLabel = CCLabelTTF::create(CCLocalizedString("EffectVolume", "NotLocalized"), "Marker Felt", 20);
    
    backgroundVolumeLabel->setAnchorPoint( CCPointZero );
    effectVolumeLabel->setAnchorPoint( CCPointZero );
    
    backgroundSlider = CCControlSlider::create(PICT_PATH "System/sliderTrack.png",
                                               PICT_PATH "System/sliderProgress.png",
                                               PICT_PATH "System/sliderThumb.png");
    
    backgroundSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingsMenuLayer::backgroundVolumeChanged), CCControlEventValueChanged);
    
    backgroundSlider->setAnchorPoint( CCPointZero );
    backgroundSlider->setMaximumAllowedValue( 1.0 );
    backgroundSlider->setMinimumAllowedValue( 0.0 );
    backgroundSlider->setValue(GetGameParams->getBackgroundVolume());
    
    effectSlider = CCControlSlider::create(PICT_PATH "System/sliderTrack.png",
                                           PICT_PATH "System/sliderProgress.png",
                                           PICT_PATH "System/sliderThumb.png");
    
    effectSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingsMenuLayer::effectVolumeChanged), CCControlEventValueChanged);
    
    effectSlider->setAnchorPoint( CCPointZero );
    effectSlider->setMaximumAllowedValue( 1.0 );
    effectSlider->setMinimumAllowedValue( 0.0 );
    effectSlider->setValue(GetGameParams->getEffectVolume());
    
    
    backgroundVolumeLabel->setPosition(ccp(screenSize.width / 2 - backgroundVolumeLabel->getContentSize().width, screenSize.height / 2 + 100));
    backgroundSlider->setPosition( ccp(screenSize.width / 2 + 20 , screenSize.height / 2 + 96) );
    effectVolumeLabel->setPosition(ccp(screenSize.width / 2 - effectVolumeLabel->getContentSize().width, screenSize.height / 2 + 50));
    effectSlider->setPosition( ccp(screenSize.width / 2 + 20, screenSize.height / 2 + 44) );
    
    addChild(backgroundVolumeLabel);
    addChild(backgroundSlider);
    addChild(effectVolumeLabel);
    addChild(effectSlider);
    
    CCLabelTTF *languageLabel = CCLabelTTF::create(CCLocalizedString("Language", "NotLocalized"), "Marker Felt", 26);
    
    languageLabel->setPosition( ccp(screenSize.width / 2, screenSize.height / 2 ) );
    addChild(languageLabel , 1);
    
    CCMenuItemSprite *russianLanguage = CCMenuItemImage::create(PICT_PATH "Languages/russian_flag.png",
                                                                 PICT_PATH "Languages/russian_flag_Selected.png", NULL,
                                                                 this, menu_selector(SettingsMenuLayer::selectLanguage));
    
    CCMenuItemSprite *usaLanguage = CCMenuItemImage::create(PICT_PATH "Languages/usa_flag.png",
                                                                PICT_PATH "Languages/usa_flag_Selected.png", NULL,
                                                                this, menu_selector(SettingsMenuLayer::selectLanguage));
    
    CCMenu *languageMenu = CCMenu::create(russianLanguage, usaLanguage, NULL);
    
    russianLanguage->setTag(kLanguageRussian);
    russianLanguage->setPosition(CCPointZero);
    russianLanguage->setAnchorPoint(CCPointZero);
    
    usaLanguage->setTag(kLanguageEnglish);
    usaLanguage->setPosition( ccp(russianLanguage->getContentSize().width + 10,0) );
    usaLanguage->setAnchorPoint(CCPointZero);
    
    languageMenu->setContentSize(CCSizeMake(2*russianLanguage->getContentSize().width, russianLanguage->getContentSize().height));
    languageMenu->setPosition( ccp(screenSize.width / 2 - languageMenu->getContentSize().width/2,
                                   screenSize.height / 2 - 25 - languageMenu->getContentSize().height) );
    addChild(languageMenu, 1);
    
    CCMenu *navigationMenu = CCMenu::create(NULL);

    CCMenuItemFont *goToMain = CCMenuItemFont::create(CCLocalizedString("BackToMainMenu", "NotLocalized"), this, menu_selector(SettingsMenuLayer::mainMenu));
    goToMain->setPosition( ccp (170, screenSize.height - 25) );
    
    navigationMenu->addChild(goToMain);
    navigationMenu->setPosition( CCPointZero );
    addChild(navigationMenu, 1);
    
    return true;
}

void SettingsMenuLayer::backgroundVolumeChanged(CCObject *sender, CCControlEvent controlEvent)
{
    GetGameParams->setBackgroundVolume(backgroundSlider->getValue());
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(backgroundSlider->getValue());
}

void SettingsMenuLayer::effectVolumeChanged(CCObject *sender, CCControlEvent controlEvent)
{
    GetGameParams->setEffectVolume(effectSlider->getValue());
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(effectSlider->getValue());
}

void SettingsMenuLayer::selectLanguage(CCObject *sender)
{
    CCResetLanguage();
    
    GetGameParams->setLanguage( ((CCNode *)sender)->getTag() );
    
    CCDirector::sharedDirector()->replaceScene(SettingsMenuLayer::scene());
}

void SettingsMenuLayer::toggleSound(CCObject *object)
{
    GetGameParams->setSoundState(!GetGameParams->getSoundState());
    
    soundOffButton->setVisible(!GetGameParams->getSoundState());
    soundOnButton->setVisible(GetGameParams->getSoundState());
    
    if(!GetGameParams->getSoundState())
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    else
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void SettingsMenuLayer::mainMenu(CCObject *object)
{
    CCTransitionSlideInL* trans = CCTransitionSlideInL::create( 1.5f * SYSTEM_ANIMATION_DELAY, MainMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}
