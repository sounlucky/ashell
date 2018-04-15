#include <windows.h>
#include <gdiplus.h>
#include "desktop.h"

HINSTANCE global_instance;

//entry point
int CALLBACK WinMain(
    HINSTANCE   instance,
    HINSTANCE   ,
    LPSTR       ,
    int         
) { 
    global_instance = instance;
    
    //init gdi plus
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    //init window clases
    init_window_classes();
    
    //will not return
    manage_desktop();
}