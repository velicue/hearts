#pragma comment(lib, "ws2_32.lib")

#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include <cstdio>

USING_NS_CC;

SOCKET ClientSocket;

int dx[4]={0,-72,0,72};
int dy[4]={-101,0,101,0};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance


    AppDelegate app;
    EGLView* eglView = EGLView::getInstance();
    eglView->setViewName("Hearts");
    eglView->setFrameSize(1080, 720);
    return Application::getInstance()->run();
}
