/* --------------- Data Section */

HWND hWnd;
long WinWidth = 500;
long WinHeight = 350;
char *szClassName = "SDI_Class";
char *szWindowName = "SDI Application";
char *szQuit = "Are you sure ?";

/* --------------- Functions declarations */

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
