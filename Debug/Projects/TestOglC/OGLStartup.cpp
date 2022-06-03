// ------------------------------------------------------
// Variables
BOOL g_isProgramLooping;
int g_FullScreen;
int g_ShowCursor;
GL_Window *g_window;
BOOL g_keys[256];
LARGE_INTEGER HTimerVal;
LONGLONG HTimerFreq;

// ------------------------------------------------------
// Name: Terminate_Application()
// Desc: Initiate termination procedure
void Terminate_Application(GL_Window *window) {
	PostMessage(window->hWnd, WM_QUIT, 0, 0);
	g_isProgramLooping = FALSE;
}

// ------------------------------------------------------
// Name: Reshape_GL()
// Desc: Resize the opengl viewport
void Reshape_GL(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// ------------------------------------------------------
// Name: Change_Screen_Resolution()
// Desc: Resize the screen with given dimensions and depth
BOOL Change_Screen_Resolution(int width, int height, int bitsPerPixel) {
	DEVMODE dmScreenSettings;
	ZeroMemory(&dmScreenSettings, sizeof (DEVMODE));
	dmScreenSettings.dmSize	= sizeof (DEVMODE);
	dmScreenSettings.dmPelsWidth = width;
	dmScreenSettings.dmPelsHeight = height;
	dmScreenSettings.dmBitsPerPel = bitsPerPixel;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if(ChangeDisplaySettings(&dmScreenSettings, 4) != DISP_CHANGE_SUCCESSFUL) return(FALSE);
	return(TRUE);
}

// -----------------------------------------------------------------------
// Name: Render_On_Screen()
// Desc: Select the main screen as rendering target
BOOL Render_On_Screen(GL_Window *window)	{
	return(wglMakeCurrent(window->hDC, window->hRC));
}

// -----------------------------------------------------------------------
// Name: Create_Window_GL()
// Desc: Create the main window
BOOL Create_Window_GL(GL_Window *window)	{
	MSG msg;
	DWORD windowStyle;
	DWORD windowExtendedStyle;
	g_window = window;
	INITCOMMONCONTROLSEX CommCtrls;

	switch(MessageBox(0, "Run in fullscreen ?", window->init.title, MB_YESNOCANCEL | MB_ICONQUESTION)) {
		case IDYES:
			g_FullScreen = TRUE;
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return(FALSE);
	}
// Force window mode if constant _DEBUG is defined
#ifdef _DEBUG
	g_FullScreen = FALSE;
#endif

	CommCtrls.dwSize = sizeof(CommCtrls);
	CommCtrls.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_WIN95_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_PROGRESS_CLASS | ICC_COOL_CLASSES | ICC_USEREX_CLASSES | ICC_INTERNET_CLASSES | ICC_PAGESCROLLER_CLASS | ICC_DATE_CLASSES | ICC_HOTKEY_CLASS | ICC_UPDOWN_CLASS | ICC_ANIMATE_CLASS;
	if(InitCommonControlsEx(&CommCtrls) == 0) InitCommonControls();
	if(g_FullScreen) {
		windowStyle = WS_POPUP;
		windowExtendedStyle = WS_EX_TOPMOST;
	} else {
		windowStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_POPUP | WS_MINIMIZEBOX;
		windowExtendedStyle = WS_EX_APPWINDOW;
	}
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		window->init.bitsPerPixel,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int wwidth = window->init.width;
	int wheight = window->init.height;
	int wtop = 0;
	int wleft = 0;
	if(wwidth > GetSystemMetrics(SM_CXSCREEN)) wwidth = GetSystemMetrics(SM_CXSCREEN);
	if(wheight > GetSystemMetrics(SM_CYSCREEN)) wheight = GetSystemMetrics(SM_CYSCREEN);
	if(!g_FullScreen) {
		// Center the window if necessary
		if(wwidth < GetSystemMetrics(SM_CXSCREEN)) wleft = (GetSystemMetrics(SM_CXSCREEN) - wwidth) >> 1;
		if(wheight < GetSystemMetrics(SM_CYSCREEN)) wtop = (GetSystemMetrics(SM_CYSCREEN) - wheight) >> 1;
		// Add some pixels around to make the inner part to fit the dimensions requirements
		wwidth += GetSystemMetrics(SM_CXBORDER);
		wheight += GetSystemMetrics(SM_CYBORDER);
		wheight += GetSystemMetrics(SM_CYCAPTION);
	}
	GLuint PixelFormat;
	window->hWnd = CreateWindowEx(windowExtendedStyle, window->init.application->className,
							   window->init.title,
							   windowStyle | WS_VISIBLE,
							   wleft, wtop,
							   wwidth,
							   wheight,
							   HWND_DESKTOP,
							   0,
							   window->init.application->hInstance,
							   window);
	if(window->hWnd == 0) return(FALSE);
	if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE) != 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowCursor(g_ShowCursor);
	if(g_FullScreen) {
		if(Change_Screen_Resolution(window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE) {
			return(FALSE);
		} 
		MoveWindow(window->hWnd, 0, 0, window->init.width, window->init.height, 1);
	}
	window->hDC = GetDC(window->hWnd);
	if(window->hDC == 0) {
		DestroyWindow(window->hWnd);
		window->hWnd = 0;
		return(FALSE);
	}
	PixelFormat = ChoosePixelFormat(window->hDC, &pfd);
	if(PixelFormat == 0) {
		ReleaseDC(window->hWnd, window->hDC);
		window->hDC = 0;
		DestroyWindow(window->hWnd);
		window->hWnd = 0;
		return(FALSE);
	}
	if(SetPixelFormat(window->hDC, PixelFormat, &pfd) == FALSE) {
		ReleaseDC(window->hWnd, window->hDC);
		window->hDC = 0;
		DestroyWindow(window->hWnd);
		window->hWnd = 0;
		return(FALSE);
	}
	window->hRC = wglCreateContext(window->hDC);
	if(window->hRC == 0) {
		ReleaseDC(window->hWnd, window->hDC);
		window->hDC = 0;
		DestroyWindow(window->hWnd);
		window->hWnd = 0;
		return(FALSE);
	}
	if(Render_On_Screen(window) == FALSE) {
		wglDeleteContext(window->hRC);
		window->hRC = 0;
		ReleaseDC(window->hWnd, window->hDC);
		window->hDC = 0;
		DestroyWindow(window->hWnd);
		window->hWnd = 0;
		return(FALSE);
	}
	window->isVisible = TRUE;
	Reshape_GL(window->init.width, window->init.height);
	ZeroMemory(g_keys, 256 * sizeof(BOOL));
	return(TRUE);
}

// -----------------------------------------------------------------------
// Name: WindowProc()
// Desc: Messages handler for the main window
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	GL_Window *window = (GL_Window *)(GetWindowLong (hWnd, GWL_USERDATA));
	CREATESTRUCT *creation;

	switch (uMsg) {
		case WM_SYSCOMMAND:
			switch(wParam) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return(0);
			}
			break;
		case WM_CREATE:
			creation = (CREATESTRUCT *) (lParam);
			window = (GL_Window*)(creation->lpCreateParams);
			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
			return(0);

		case WM_CLOSE:
			Terminate_Application(window);
			return(0);

		case WM_ACTIVATE:
			if(g_FullScreen) {
				if((LOWORD(wParam) & WA_ACTIVE) || (LOWORD(wParam) & WA_CLICKACTIVE)) {
					Change_Screen_Resolution(g_window->init.width, g_window->init.height, g_window->init.bitsPerPixel);
					SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
					window->isVisible = TRUE;
				} else {
					ChangeDisplaySettings(NULL, 0);
					SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_SHOWWINDOW);
					ShowWindow(hWnd, SW_MINIMIZE);
					window->isVisible = FALSE;
				}
			}
			break;
		case WM_SIZE:
			switch (wParam) {
				case SIZE_MINIMIZED:
					window->isVisible = FALSE;
					return(0);

				case SIZE_MAXIMIZED:
					window->isVisible = TRUE;
					return(0);

				case SIZE_RESTORED:
					window->isVisible = TRUE;
					return(0);
			}
			break;
		case WM_KEYDOWN:
			if(wParam <= 255) {
				g_keys[wParam] = TRUE;
				return(0);
			}
			break;
		case WM_MOUSEMOVE:
			if(g_ShowCursor) {
				g_ShowCursor = FALSE;
				ShowCursor(g_ShowCursor);
			}
			break;
		case WM_NCMOUSEMOVE:
			if(!g_ShowCursor) {
				g_ShowCursor = TRUE;
				ShowCursor(g_ShowCursor);
			}
			break;
		case WM_KEYUP:
			if(wParam <= 255) {
				g_keys[wParam] = FALSE;
				return(0);
			}
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// -----------------------------------------------------------------------
// Name: RegisterWindowClass()
// Desc: Register the class for the main window
BOOL RegisterWindowClass(Application *application) {
	WNDCLASSEX windowClass;
	LOGBRUSH SubEditBrush;

	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = (WNDPROC) (WindowProc);
	windowClass.hInstance = application->hInstance;
	windowClass.hIcon = LoadIcon(application->hInstance, MAKEINTRESOURCE(ICON_APP));
	windowClass.hIconSm = windowClass.hIcon;
	SubEditBrush.lbStyle = BS_SOLID;
	SubEditBrush.lbHatch = 0;
	SubEditBrush.lbColor = 0;
	windowClass.hbrBackground = (HBRUSH) CreateBrushIndirect(&SubEditBrush);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = application->className;
	if(RegisterClassEx(&windowClass) == 0) return FALSE;
	return TRUE;
}

// -----------------------------------------------------------------------
// Name: WinMain()
// Desc: Program entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Application application;
	GL_Window window;
	MSG msg;
	float LastT;
	float FirstT;

	ZeroMemory(&window, sizeof (GL_Window));
	application.className = "OGLWindow";
	window.init.title = (char *) "OpenGL window";
	application.hInstance = hInstance;
	window.init.application = &application;
	window.init.width = SCREEN_WIDTH;
	window.init.height = SCREEN_HEIGHT;
	window.init.bitsPerPixel = SCREEN_BPP;
	if(RegisterWindowClass(&application) == FALSE) return(0);
	g_isProgramLooping = TRUE;
	if(Create_Window_GL(&window)) {
		if(Initialize() == FALSE) {
			Terminate_Application(&window);
		} else {
			// Init counter
			QueryPerformanceFrequency(&HTimerVal);
			HTimerFreq = HTimerVal.QuadPart * 0.001f;
			QueryPerformanceCounter(&HTimerVal);
			window.LastTickCount = HTimerVal.QuadPart;
			window.FirstTickCount = window.LastTickCount;
			msg.message = 0;
			while(msg.message != WM_QUIT) {
				if(PeekMessage(&msg, window.hWnd, 0, 0, PM_REMOVE)) {
					DispatchMessage(&msg);
				} else {
					if(window.isVisible == FALSE) {
						WaitMessage();
					} else {
						QueryPerformanceCounter(&HTimerVal);
						LastT = (((float) (HTimerVal.QuadPart - window.LastTickCount)) / HTimerFreq) * 0.001f;
						FirstT = (((float) (HTimerVal.QuadPart - window.FirstTickCount)) / HTimerFreq) * 0.001f;
						Update(LastT, FirstT);
						window.LastTickCount = HTimerVal.QuadPart;
						Draw();
						SwapBuffers(window.hDC);
					}
				}
			}
		}
		Deinitialize();
		ChangeDisplaySettings(NULL, 0);
		if(window.hWnd) {
			if(window.hDC) {
				wglMakeCurrent(window.hDC, 0);
				if(window.hRC != 0) wglDeleteContext(window.hRC);
				ReleaseDC(window.hWnd, window.hDC);
			}
			DestroyWindow(window.hWnd);
		}
		ShowCursor(TRUE);
	} else {
		g_isProgramLooping = FALSE;
	}
	UnregisterClass(application.className, application.hInstance);	
	return(0);
}
