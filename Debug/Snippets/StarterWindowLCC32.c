// ------------------------------------------
// Standard starter file for windows applications
// ------------------------------------------
// buildblock RELEASE
//  CAPT "[BINDIR]\lcc.exe" -O "%2" -Fo"%1.obj"
//  CAPT "[BINDIR]\lcclnk.exe" -s -o "%1.exe" -subsystem windows "%1.obj"
// buildblockend
// buildblock DEBUG
//  CAPT "[BINDIR]\lcc.exe" -g2 -g5 "%2" -Fo"%1.obj"
//  CAPT "[BINDIR]\lcclnk.exe" -o "%1.exe" -subsystem windows "%1.obj"
// buildblockend

// --------------- Includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// -----------------
// --- WinMain() ---
// -----------------
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    return(0);
}
