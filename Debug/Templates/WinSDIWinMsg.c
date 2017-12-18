/* --------------- Messages handling */

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CREATE:
            return(0);
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case 1000:
                    SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
                    break;
            }
            break;
        case WM_CLOSE:
            if(MessageBox(hWnd, szQuit, szWindowName, MB_YESNO | MB_ICONQUESTION) == IDNO)
            {
                return(0);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}
