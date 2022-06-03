// ------------------------------------------
// Standard starter file for console applications
// ------------------------------------------
// buildblock RELEASE
//  CAPT "[BINDIR]\sc.exe" "%2" -c -mn -o -cpp -ff -o"%1.obj"
//  CAPT "[BINDIR]\link.exe" /SU:console "%1.obj"
// buildblockend
// buildblock DEBUG
//  CAPT "[BINDIR]\sc.exe" "%2" -c -mn -g -cpp -ff -o"%1.obj"
//  CAPT "[BINDIR]\link.exe" /CO /DEB /SU:console "%1.obj"
// buildblockend

// --------------- Includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// --------------
// --- Main() ---
// --------------
int main(int argc, char *argv[])
{
    return(0);
}
