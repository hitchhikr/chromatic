// ------------------------------------------
// Standard starter file for windows applications
// ------------------------------------------
// buildblock RELEASE
//  CAPT "[BINDIR]\gcc.exe" -c -s -mno-cygwin -I"[INCLUDEDIR]" -L"[LIBDIR]" "%2"
//  CAPT "[BINDIR]\gcc.exe" -o "%1.exe" -s -mwindows -mno-cygwin -fnative-struct -I"[INCLUDEDIR]" -L"[LIBDIR]" "%1.o"
// buildblockend
// buildblock DEBUG
//  CAPT "[BINDIR]\gcc.exe" -c -gcoff -mno-cygwin -I"[INCLUDEDIR]" -L"[LIBDIR]" "%2"
//  CAPT "[BINDIR]\gcc.exe" -o "%1.exe" -mwindows -mno-cygwin -fnative-struct -I"[INCLUDEDIR]" -L"[LIBDIR]" "%1.o"
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
