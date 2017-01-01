//OPENGL 3.2 DEMO FOR RENDERING OBJECTS LOADED FROM OBJ FILES

//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle

#include <windows.h>									// Header File For Windows
#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include "console.h"
#include "shaders/Shader.h"								// include shader header file, this is not part of OpenGL
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "Actor.h"
#include "Camera.h"
#include "Car.h"
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "AI.h"


#define BACK_CAM 0
#define TOP_CAM 1
#define DASH_CAM 2
#define NUM_CAM_STATES 3
#define COLOR_INC 0.002f
#define LIGHT_INC 0.001f


ConsoleWindow console;
Shader* shader;  ///shader object 
OBJLoader objLoader;
glm::mat4 ProjectionMatrix;								// matrix for the orthographic projection
glm::mat4 ModelViewMatrix;								// matrix for the modelling and viewing
glm::mat4 viewingMatrix;
//Material properties
float Material_Ambient[4] = {0.4f, 0.4f, 0.4f, 1.0f};
float Material_Diffuse[4] = {0.5f, 0.5f, 0.5f, 1.0f};
float Material_Specular[4] = {0.5f,0.5f,0.5f,1.0f};
float Material_Shininess = 50;
//Light Properties
float Light_Ambient_And_Diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float Light_Specular[4] = {1.0f,1.0f,1.0f,1.0f};
float LightPos[4] = {0.0f, 1.0f, 1.0f, 0.0f};
bool test=false;
bool toNight = true;
float dayColours = 1.0f;
float dayLight = 1.0f;
void disableShine();
void enableShine();
//Input
int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;
int screenWidth=600, screenHeight=600;
bool keys[256];
Actor *ground,*sky;
std::vector<Actor*> collList;
Car *car;
Camera *camera;
int camState = BACK_CAM;
AI *car2;



//OPENGL FUNCTION PROTOTYPES
void display();										//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();										//called in winmain when the program starts.
void processKeys();									//called in winmain to process keyboard input
void dayTick();


/*************    START OF OPENGL FUNCTIONS   ****************/

void disableShine()
{
	Light_Specular[0] = 0;
	Light_Specular[1] = 0;
	Light_Specular[2] = 0;
	glUniform4fv(glGetUniformLocation(shader->handle(), "light_specular"), 1, Light_Specular);
}

void enableShine()
{
	Light_Specular[0] = dayColours;
	Light_Specular[1] = dayColours;
	Light_Specular[2] = dayColours;
	glUniform4fv(glGetUniformLocation(shader->handle(), "light_specular"), 1, Light_Specular);
}

void display()									
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	glUseProgram(shader->handle());					// use the shader

	dayTick();

	if (camState == BACK_CAM)
		camera->backCam();
	else if (camState == TOP_CAM)
		camera->topCam();
	else if (camState == DASH_CAM)
		camera->dashCam();

	camera->render();	
	
	car->tick();

	car2->aiTick();

	ground->render();
	sky->render();

	disableShine();


	for (int i = 0; i < collList.size(); i++)
	{
		//if(collList[i] != car)
			collList.at(i)->render();
	}
	
	enableShine();
	

	//car->render();
	

	glFlush();
}


void init()
{
	glClearColor(0.41,0.84,0.99,0.0);					//sets the clear colour to blue
														//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour
	glEnable(GL_DEPTH_TEST);
	

	shader = new Shader;
	if(!shader->load("BasicView", "glslfiles/basicTransformationsWithDisplacement.vert", "glslfiles/basicTransformationsWithDisplacement.frag"))
	{
		cout << "failed to load shader" << endl;
	}		
	
	

	glUseProgram(shader->handle());  // use the shader

	collList.push_back(new Actor("TestModels/Scene/Object001.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object002.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object003.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object004.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object005.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object006.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object007.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object008.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object009.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object010.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object011.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object012.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object013.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object014.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object015.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object016.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object017.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object018.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object019.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object020.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object021.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object022.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object023.obj"));
	collList.push_back(new Actor("TestModels/Scene/Object024.obj"));

	ground = new Actor("TestModels/floor.obj");

	sky = new Actor("Testmodels/Scene/sky.obj");
	sky->setYPos(-10);

	car = new Car();
	car->setZPos(-30);
	car2 = new AI();
	car2->setXPos(3);
	
	try {
		collList.push_back(dynamic_cast<Actor*>(car2));
		collList.push_back(dynamic_cast<Actor*>(car));
	}
	catch (exception& e) {

	}

	camera = new Camera();
	camera->setFocusPoint(car);

	


	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_CLAMP);
	
	

	glUniform4fv(glGetUniformLocation(shader->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(shader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(shader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(shader->handle(), "light_specular"), 1, Light_Specular);
	glUniform4fv(glGetUniformLocation(shader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(shader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(shader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(shader->handle(), "material_shininess"), Material_Shininess);
}

void processKeys()
{
	bool noSteer = true, noPedal = true;
	if(keys[VK_UP])
	{
		//camera->setYPos(camera->getPos().y +0.1);
		car->upKey();
		noPedal = false;
	}
	if(keys[VK_DOWN])
	{
		//camera->setYPos(camera->getPos().y - 0.1);
		car->downKey();
		noPedal = false;
	}
	if(keys[VK_LEFT])
	{
		//camera->setXPos(camera->getPos().x - 0.1);
		car->leftKey();
		noSteer = false;
	}
	if(keys[VK_RIGHT])
	{
		//camera->setXPos(camera->getPos().x + 0.1);
		car->rightKey();
		noSteer = false;
	}
	if(keys[VK_SPACE])
	{
		//camera->setZPos(camera->getPos().z - 0.1);
		car->brake();
	}
	if(keys[VK_SHIFT])
	{
		//camera->setZPos(camera->getPos().z + 0.1);
		camState++;
		camState = camState % NUM_CAM_STATES;
		keys[VK_SHIFT] = false;
	}
	if (noSteer) {
		car->noSteer();
	}
	if (noPedal) {
		car->noPedal();
	}
}

void dayTick()
{
	if (toNight)
	{
		dayColours -= COLOR_INC;
		dayLight -= LIGHT_INC;
	}
	else
	{
		dayColours += COLOR_INC;
		dayLight += LIGHT_INC;
	}
	if (dayColours > 1)
		toNight = true;
	else if (dayColours < 0)
		toNight = false;

	glClearColor(dayColours*0.41, dayColours*0.84, dayColours, 0.0);
	Light_Specular[0] = dayColours;
	Light_Specular[1] = dayColours;
	Light_Specular[2] = dayColours;
	Light_Ambient_And_Diffuse[0] = dayLight;
	Light_Ambient_And_Diffuse[1] = dayLight;
	Light_Ambient_And_Diffuse[2] = dayLight;
	glUniform4fv(glGetUniformLocation(shader->handle(), "light_specular"), 1, Light_Specular);
	glUniform4fv(glGetUniformLocation(shader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(shader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
}

void reshape(int width, int height)							// Resize the OpenGL window
{
	screenWidth = width; screenHeight = height;             // to ensure the mouse coordinates match 
														    // we will use these values to set the coordinate system
	glViewport(0, 0, width, height);						// Reset The Current Viewport

															//Set the projection matrix
	ProjectionMatrix = glm::perspective(60.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 1.0f, 200.0f);
}

/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	  // Declaration For WndProc
void KillGLWindow();									  // releases and destroys the window
bool CreateGLWindow(char* title, int width, int height);  //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);    // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	console.Open();


	// Create Our OpenGL Window
	if (!CreateGLWindow("Car Sim",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;

			processKeys();							//process keyboard
			
			display();								// Draw The Scene
			SwapBuffers(hDC);						// Swap Buffers (Double Buffering)
		}
	}

	console.Close();

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	HGLRC tempContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, tempContext);

	glewInit();

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};
	
    if(wglewIsSupported("WGL_ARB_create_context") == 1)
    {
		hRC = wglCreateContextAttribsARB(hDC,0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hDC, hRC);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		hRC = tempContext;
		cout << " not possible to make context "<< endl;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	cout << GLVersionString << endl;

	//OpenGL 3.2 way of checking the version
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}



