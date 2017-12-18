/* --------------- Program main inits */

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    WNDCLASSEX wc;
    MSG msg;
    int WinLeft;
    int WinTop;
    INITCOMMONCONTROLSEX CommCtrls;
    HACCEL hAccel;
    
    CommCtrls.dwSize = sizeof(CommCtrls);
    CommCtrls.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_WIN95_CLASSES |
                      ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_PROGRESS_CLASS | ICC_COOL_CLASSES |
                      ICC_USEREX_CLASSES | ICC_INTERNET_CLASSES | ICC_PAGESCROLLER_CLASS | ICC_DATE_CLASSES |
                      ICC_HOTKEY_CLASS | ICC_UPDOWN_CLASS | ICC_ANIMATE_CLASS;
    if(InitCommonControlsEx(&CommCtrls) == 0) InitCommonControls();
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNWINDOW;
    wc.lpfnWndProc = &WinProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szClassName;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(ICON_APP));
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassEx(&wc);
    WinLeft = 0;
    WinTop = 0;
    // Center the window if necessary
    if(WinWidth < GetSystemMetrics(SM_CXSCREEN)) WinLeft = (GetSystemMetrics(SM_CXSCREEN) - WinWidth) >> 1;
    if(WinHeight < GetSystemMetrics(SM_CYSCREEN)) WinTop = (GetSystemMetrics(SM_CYSCREEN) - WinHeight) >> 1;
    // Add some pixels around to make the inner part to fit the dimensions requirements
    WinWidth += GetSystemMetrics(SM_CXBORDER);
    WinHeight += GetSystemMetrics(SM_CYBORDER);
    WinHeight += GetSystemMetrics(SM_CYCAPTION);
    hWnd = CreateWindowEx(0, szClassName, szWindowName, WS_OVERLAPPEDWINDOW,
                          WinLeft, WinTop, WinWidth, WinHeight, 0, 0, hInstance, 0);
    hAccel = LoadAccelerators(hInstance,MAKEINTRESOURCE(ACCELERATORS_0));
    SetMenu(hWnd, LoadMenu(hInstance,MAKEINTRESOURCE(MENUS_0)));
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        if(hAccel) TranslateAccelerator(hWnd, hAccel, &msg);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return(msg.wParam);
}
