#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

// uncomment below line, open debug console
// #define USE_WIN32_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	eglView->setFrameSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) - 50);

	SetWindowTextA(eglView->getHWnd(),"Piktomir");

    int ret = CCApplication::sharedApplication()->run();

#ifdef USE_WIN32_CONSOLE
    FreeConsole();
#endif

    return ret;
}
