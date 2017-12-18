#ifndef _STARTUP_H_
#define _STARTUP_H_

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 16

// ------------------------------------------------------
// structures

typedef struct
{
	HINSTANCE hInstance;
	const char *className;
} Application;

typedef struct
{
	Application *application;
	char *title;
	int width;
	int height;
	int bitsPerPixel;
	BOOL isFullScreen;
} GL_WindowInit;

typedef struct
{
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	GL_WindowInit init;
	BOOL isVisible;
	LONGLONG LastTickCount;
	LONGLONG FirstTickCount;
} GL_Window;

// ------------------------------------------------------
// Global variables
extern int _FullScreen;
extern GL_Window *g_window;
extern BOOL g_keys[256];

// ------------------------------------------------------
// Public functions
void Terminate_Application(GL_Window *window);
BOOL Initialize(void);
void Deinitialize(void);
void Update(float milliseconds, float TotalTime);
BOOL Render_On_Screen(GL_Window *window);
void Draw(void);
void Reshape_GL(int width, int height);

#endif
