//
//  OptionsLayer.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 19.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "OptionsLayer.h"
#include "MainMenuLayer.h"
#include "utils.h"
#include "cocos-ext.h"
#include "FTPutils.h"
#include "FTPuser.h"
#include "MapEditorGameLayer.h"

USING_NS_CC;

CCScene* OptionsLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    OptionsLayer *layer = OptionsLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer,0,0);
    
    // return the scene
    return scene;
}

bool OptionsLayer::init()
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
    
    
    CCMenuItemFont *goToMain = CCMenuItemFont::create("Назад в главное меню", this, menu_selector(OptionsLayer::mainMenu));
    goToMain->setPosition( ccp (170, screenSize.height - 25) );
    CCMenu *navigationMenu = CCMenu::create(goToMain, NULL);
    navigationMenu->setPosition( CCPointZero );
    addChild(navigationMenu, 1);
    
    CCLabelTTF * FTPLable = CCLabelTTF::create("Настройки FTP", "Marker Felt", 28);
    FTPLable->setAnchorPoint(ccp(0.5,0.5));
    FTPLable->setPosition(ccp(screenSize.width / 2 + 40, screenSize.height / 2 + 50));
    addChild(FTPLable,10);
    
    cocos2d::extension::CCScale9Sprite* EditBoxSprite = cocos2d::extension::CCScale9Sprite::create("System/scalesprite.png");
    
    CCLabelTTF * LoginLabel = CCLabelTTF::create("Логин", "Marker Felt", 24);
    LoginLabel->setAnchorPoint(ccp(1,0.5));
    LoginLabel->setPosition(ccp(screenSize.width / 2 - 10, screenSize.height / 2));
    addChild(LoginLabel,10);
    
    login = cocos2d::extension::CCEditBox::create(CCSizeMake(200, 40), EditBoxSprite);
    login->setText(MapEditorGameLayer::getInstanse()->FTPLogin.c_str());
    login->setFontColor(ccc3(0, 0, 0));
    login->setAnchorPoint(ccp(0,0.5));
    login->setPosition(ccp(screenSize.width / 2 + 10, screenSize.height / 2));
    addChild(login,10);
    
    EditBoxSprite = cocos2d::extension::CCScale9Sprite::create("System/scalesprite.png");
    
    CCLabelTTF * PassLabel = CCLabelTTF::create("Пароль", "Marker Felt", 24);
    PassLabel->setAnchorPoint(ccp(1,0.5));
    PassLabel->setPosition(ccp(screenSize.width / 2 - 10, screenSize.height / 2 - 50));
    addChild(PassLabel,10);
    
    pass = cocos2d::extension::CCEditBox::create(CCSizeMake(200, 40), EditBoxSprite);
    pass->setFontColor(ccc3(0, 0, 0));
    pass->setAnchorPoint(ccp(0,0.5));
    pass->setPosition(ccp(screenSize.width / 2 + 10, screenSize.height / 2 - 50));
    addChild(pass,10);
    
    CCMenuItemFont *OK = CCMenuItemFont::create("OK", this, menu_selector(OptionsLayer::OK));
    OK->setPosition( ccp(screenSize.width / 2 + 40, screenSize.height / 2 - 100) );
    CCMenu *OKMenu = CCMenu::create(OK, NULL);
    OKMenu->setPosition( CCPointZero );
    addChild(OKMenu, 1);
    
    return true;
}

void OptionsLayer::mainMenu(CCObject *object)
{
    CCTransitionSlideInL* trans = CCTransitionSlideInL::create( 1.5f * SYSTEM_ANIMATION_DELAY, MainMenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(trans);
}

void OptionsLayer::OK(CCObject *object)
{
    printf("%s", login->getText());
    std::string tmpLogin = login->getText();
    std::string tmpPass = pass->getText();
    MD5 md5;
    std::string tmpPassHashStr = md5.digestString(tmpPass.c_str());
    FTPuser * tmpFTPUser = new FTPuser(writablePath, tmpLogin, tmpPassHashStr);
    if(tmpFTPUser->checkFTP()) {
        if(MapEditorGameLayer::getInstanse()->FTPUser) delete MapEditorGameLayer::getInstanse()->FTPUser;
        MapEditorGameLayer::getInstanse()->FTPUser = tmpFTPUser;
        MapEditorGameLayer::getInstanse()->FTPLogin = tmpLogin;
        MapEditorGameLayer::getInstanse()->FTPPass = tmpPassHashStr;
        
        // настройки
        
        CCUserDefault::sharedUserDefault()->setStringForKey("Login", tmpLogin);
        CCUserDefault::sharedUserDefault()->setStringForKey("Pass", tmpPassHashStr);
    }
    else
    {
        CCMessageBox ("Неверный логин/пароль", "Неверный логин/пароль");
    }
    
    mainMenu(object);
}