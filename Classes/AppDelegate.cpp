#include "AppDelegate.h"
#include "MainMenuLayer.h"
#include "AppMacros.h"
#include "utils.h"
#include <sys/stat.h>
#include "SimpleAudioEngine.h"

#include "GameParams.h"
#include "FTPuser.h"
#include "PMTextureCache.h"

USING_NS_CC;
using namespace CocosDenshion;


AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
    SimpleAudioEngine::end();
//    PMTextureCache::deleteInstanse();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);
    pDirector->setProjection(kCCDirectorProjection2D);
    pDirector->setDisplayStats(false);
    


    
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    platform_mkdir(writablePath.substr(0,writablePath.size()-1).c_str());
    
    std::vector<std::string> searchPaths;
    searchPaths.push_back(mediumResource.directory);
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    pDirector->setContentScaleFactor(mediumResource.size.height/designResolutionSize.height);
    
#endif

#ifndef MAP_EDITOR
//#ifndef DEBUG

    std::string writableMapPath = writablePath + "Maps";
    GameParams::getInstanse()->mapPath = writableMapPath;
    
    FTPuser *ftpUser = new FTPuser(writablePath,"developer");
    
    if(ftpUser->checkFTP())
    {
        int version;
        
        if((version = ftpUser->downloadVerMap()) != -1)
        {
            int myVersion = GameParams::getInstanse()->getMapVersion();
            
            if(myVersion < version || !folderExists(writableMapPath))
            {
                if(ftpUser->downloadMap())
                {                    
                    rmdir(writableMapPath.c_str());
                    
                    
                    platform_mkdir((writablePath + "Maps").c_str());
                    
                    if(unzipFile((writablePath + "Maps.zip").c_str(), writablePath + "Maps/" ) == 0)
                    {
                        GameParams::getInstanse()->setMapVersion(version);
                    }
                    else
                    {
                        CCLOGINFO("Error unzipping map archive");
                    }

                }
            }
            else if(folderExists(writableMapPath))
            {
                CCLOGINFO("Your maps verison is up to date, using local copy!");
            }
        }
    }
    else
    {
        CCLOGINFO("No FTP connection, using local copy!");
    }

    if(!folderExists(writableMapPath))
        GameParams::getInstanse()->mapPath = "Maps";

//#else
//	GameParams::getInstanse()->mapPath = "Maps";
//    
//    GameParams::getInstanse()->loadWorlds();
//    
//#endif
	GameParams::getInstanse()->loadWorlds();
#else
    GameParams::getInstanse()->mapPath.initWithFormat((writablePath+"Maps/").c_str());
#endif

    
    
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
	CCSize frameSize = pEGLView->getFrameSize();
    
    if (frameSize.height > mediumResource.size.height)
	{
        std::vector<std::string> searchPaths;
        searchPaths.push_back(largeResource.directory);
        searchPaths.push_back(".");
        CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        pDirector->setContentScaleFactor(largeResource.size.height/designResolutionSize.height);
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > iPhone5Resource.size.height)
    {
        std::vector<std::string> searchPaths;
        searchPaths.push_back(mediumResource.directory);
        searchPaths.push_back(".");
        CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        pDirector->setContentScaleFactor(mediumResource.size.height/designResolutionSize.height);
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else if (frameSize.width == iPhone5Resource.size.height)
    {
		std::vector<std::string> searchPaths;
        searchPaths.push_back(iPhone5Resource.directory);
        searchPaths.push_back(".");
        CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        pEGLView->setDesignResolutionSize(iPhone5Resource.size.width, iPhone5Resource.size.height, kResolutionNoBorder);
    }
    
    else if (frameSize.width == iPhoneResource.size.height)
    {
		std::vector<std::string> searchPaths;
        searchPaths.push_back(iPhone5Resource.directory);
        searchPaths.push_back(".");
        CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        pEGLView->setDesignResolutionSize(iPhoneResource.size.width, iPhoneResource.size.height, kResolutionNoBorder);
    }
    
    
#endif
    
    GameParams::getInstanse()->screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCLOG("screenSize.width = %f, height = %f", GameParams::getInstanse()->screenSize.width, GameParams::getInstanse()->screenSize.height);
    
#ifndef MAP_EDITOR
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/background.wav", true);
    
    if(!GetGameParams->getSoundState())
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(GetGameParams->getBackgroundVolume());
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(GetGameParams->getEffectVolume());
    
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sounds/loose_sound.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Sounds/win_sound.wav");
#endif

 /*   CCLOG("%f",(&*******&*&******&*****&***&*&*******&****&******&*&***&*****&*&******&sin)(10));
    CCLOG("%f",(&******&***&***********&**&***&*****&***&*&*****&***&***&****&*&******&sin)(10));
    CCLOG("%f",(&******&*********&*****&*&*****&***&***&**&****&*****&***&************&sin)(10));
    CCLOG("%f",(&*******&********&*****&*******&***&******&****&***&*&***&************&sin)(10));
    CCLOG("%f",(&********&*******&*****&*******&***&******&****&**&**&***&************&sin)(10));
    CCLOG("%f",(&*********&******&*****&*******&***&******&****&*&***&***&***&*&******&sin)(10));
    CCLOG("%f",(&**********&*****&*****&*******&***&******&****&*****&***&***&*&******&sin)(10));
    CCLOG("%f",(&******&***&*****&*****&*******&****&*****&*****&***&***&***&*&*******&sin)(10));
    CCLOG("%f",(&*******&*&******&*****&*******&*****&**&*&*&****&*&***&***&*&********&sin)(10));
    CCLOG("%f",(&sin)(10));
*/
    
    pDirector->runWithScene(MainMenuLayer::scene());
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
	if(GetGameParams->getSoundState())
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
	if(GetGameParams->getSoundState())
		 SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
