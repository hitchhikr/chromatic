// ------------------------------------------
// Standard starter file for console applications
// ------------------------------------------
// buildblock RELEASE
//  CAPT "[BINDIR]\lcc.exe" -O "%2" -Fo"%1.obj"
//  CAPT "[BINDIR]\lcclnk.exe" -s -o "%1.exe" -subsystem console "%1.obj"
// buildblockend
// buildblock DEBUG
//  CAPT "[BINDIR]\lcc.exe" -g2 -g5 "%2" -Fo"%1.obj"
//  CAPT "[BINDIR]\lcclnk.exe" -o "%1.exe" -subsystem console "%1.obj"
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
