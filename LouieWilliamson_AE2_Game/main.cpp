#include <d3d11.h> 
#include <dxgi.h> 
#include <D3DX11.h>
#include <windows.h>
#include <DxErr.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <DirectXMath.h>
#include <sstream>
#include <time.h>
#include "Game.h"

using namespace DirectX;

//Global Variables
HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;
Game* game;

//FPS Variables
int frameCount = 0;
const float FPS = 60;
const float frameDelay = 1000 / FPS;
float frameStart;
float frameTime;
float currentFPS;

//Forward Declarations
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Shutdown();
void PrintFloat(float convert);

//Entry point to the program. 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("Failed to Create Window");
		return 0;
	}

	//Create the game, initialise it and reset the timer so it can be used for FPS
	game = new Game(g_hWnd);
	game->Initialise(g_hInst);
	game->timer.Reset(); 

	//get seed for random numbers
	srand(time(0)); 

	//Main Message loop
	MSG msg = { 0 };
	
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Game Loop starts

			//Get time at the start of the frame
			frameStart = game->timer.DeltaTime();

			game->Update();

			//Frame time is delta time - start time
			frameTime = game->timer.DeltaTime() - frameStart;

			//If frame time is too quick, delay by the difference between them
			if (frameDelay > frameTime)
			{
				Sleep(frameDelay - frameTime);
			}

			//Get FPS count
			currentFPS = 1.0f / game->timer.DeltaTime();

			//Set it to the game so it can be displayed
			game->SetFPS(currentFPS);
		}
		//if game is no longer running, quit
		if (!game->isRunning())
		{
			msg.message = WM_QUIT;
		}
	}

	//Clean up pointers
	Shutdown();

	//End of program
	return (int)msg.wParam;
}

//Register class and create window
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow)
{
	//The app name?
	char Name[100] = "Louie Williamson";
	char g_appName[100] = "The Milky Way Wars\0";

	//Register class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = Name;

	if (!RegisterClassEx(&wcex)) return E_FAIL;

	//Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 1280, 960 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(Name, g_appName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}

//Called every time the applcation recieves a message
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//Clean up pointer objects
void Shutdown()
{
	if (game)
	{
		delete game;
		game = nullptr;
	}
}

//Used for Debugging, prints a float to the output
void PrintFloat(float convert)
{
	stringstream ss;
	ss << convert << "\n";
	string teststring = ss.str();

	const char* s = teststring.c_str();

	OutputDebugString(s);
}