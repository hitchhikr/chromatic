float rquad;

// ------------------------------------------------------
// Name: Initialize()
// Desc: Perform the various initializations
BOOL Initialize(void)
{
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	return(TRUE);
}

// ------------------------------------------------------
// Name: Deinitialize()
// Desc: Free allocated resources
void Deinitialize(void)
{

}

// ------------------------------------------------------
// Name: Update()
// Desc: Update the scene
void Update(float milliseconds, float TotalTime)
{

	if(g_keys[VK_ESCAPE] == TRUE) Terminate_Application(g_window);
}

// ------------------------------------------------------
// Name: Draw()
// Desc: Draw the scene
void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
