#define WIN32_LEAN_AND_MEAN
#include "ArkanoidScratch.h"
#include <windows.h>
#include <stdlib.h>
#include <iostream>

// Alien File

uint32_t ScreenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH] = { 0 };

static HINSTANCE HInstance{ 0 };
static DWORD Ticks{ 0 };
static bool IsActive{ true };
static POINT CursorPos{};
static bool Quited{ false };
static LARGE_INTEGER qpc_frequency{ 0 };
static LARGE_INTEGER qpc_ref_time{ 0 };

bool IsWindowActive()
{
	return IsActive;
}

void ClearScreenBuffer()
{
	memset( ScreenBuffer, 0, sizeof( ScreenBuffer ) );
}

bool IsKeyPressed( int button_vk_code )
{
	return IsActive && ( GetAsyncKeyState( button_vk_code ) & 0x8000 );
}

bool IsMouseButtonPressed( int mouse_button_index )
{
	if ( !IsActive ) return false;

	if ( mouse_button_index == 0 ) return GetAsyncKeyState( VK_LBUTTON ) != 0;

	if ( mouse_button_index == 1 ) return GetAsyncKeyState( VK_RBUTTON ) != 0;

	return false;
}

int GetCursorPosX()
{
	return CursorPos.x;
}

int GetCursorPoxY()
{
	return CursorPos.y;
}

void ScheduleQuitGame()
{
	Quited = true;
}

static void CALLBACK update_proc( HWND hwnd )
{
	if ( Quited ) return;

	IsActive = GetActiveWindow() == hwnd;

	GetCursorPos( &CursorPos );
	ScreenToClient( hwnd, &CursorPos );

	LARGE_INTEGER t;
	QueryPerformanceCounter( &t );

	float dt = float( (unsigned long)( t.QuadPart - qpc_ref_time.QuadPart ) ) / qpc_frequency.QuadPart;
	if ( dt > 0.1f ) dt = 0.1f;

	Tick( dt );

	if ( !Quited )
	{
		Draw();
		RedrawWindow( hwnd, NULL, 0, RDW_INVALIDATE | RDW_UPDATENOW );
	}

	qpc_ref_time = t;
}

static LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint( hwnd, &ps );

			BITMAPINFOHEADER bih;
			bih.biSize = sizeof( bih );
			bih.biWidth = SCREEN_WIDTH;
			bih.biHeight = -SCREEN_HEIGHT;
			bih.biPlanes = 1;
			bih.biBitCount = 32;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = 0;
			bih.biXPelsPerMeter = 96;
			bih.biYPelsPerMeter = 96;
			bih.biClrUsed = 0;
			bih.biClrImportant = 0;
			SetDIBitsToDevice( hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, SCREEN_HEIGHT, ScreenBuffer, (BITMAPINFO *)&bih, DIB_RGB_COLORS );

			EndPaint( hwnd, &ps );
		}
		break;
		case WM_QUIT:
		case WM_DESTROY:
			Quited = true;
			break;
		default:
			return DefWindowProc( hwnd, message, wParam, lParam );
	}
	return 0;
}

int APIENTRY wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	SetProcessDPIAware();
	HInstance = hInstance;
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW );
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = "GameTemplateWndClass";
	wcex.hIconSm = 0;
	RegisterClassExA( &wcex );

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = SCREEN_WIDTH;
	rect.bottom = SCREEN_HEIGHT;
	AdjustWindowRectEx( &rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE, 0 );

	HWND hwnd = CreateWindowA( wcex.lpszClassName,
	                           "Game",
	                           WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
	                           CW_USEDEFAULT,
	                           0,
	                           rect.right - rect.left,
	                           rect.bottom - rect.top,
	                           nullptr,
	                           nullptr,
	                           hInstance,
	                           nullptr );

	if ( !hwnd ) return 0;

	ShowWindow( hwnd, nCmdShow );
	UpdateWindow( hwnd );

	QueryPerformanceFrequency( &qpc_frequency );
	QueryPerformanceCounter( &qpc_ref_time );

	Ticks = GetTickCount();
	Initialize();

	MSG msg;
	while ( !Quited )
	{
		while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		update_proc( hwnd );
		Sleep( 0 );
	}

	Finalize();

	return (int)msg.wParam;
}
