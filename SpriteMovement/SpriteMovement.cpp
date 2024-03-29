// SpriteMovement.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SpriteMovement.h"

#include <iostream>
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const wchar_t SPRITE_FILENAME[15] = L"ball.bmp";
const int SPRITE_WIDTH = 150;
const int SPRITE_HEIGHT = 150;
const int MOVE_OFFSET = 10;
const int MOVE_OFFSET_MULTIPLIER = 3;
bool shiftDown = false;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


Sprite::Sprite(int left = 0, int top = 0, int width = 200, int height = 100)
{
	this->left = left;
	this->top = top;
	this->right = left + width;
	this->bottom = top + height;
	this->width = width;
	this->height = height;
}

void Sprite::MoveLeft(int offset)
{
	if (this->left - offset > 0)
	{
		this->left -= offset;
		this->right -= offset;
	}
	else
	{
		this->right -= this->left + MOVE_OFFSET_MULTIPLIER * offset;
		this->left = MOVE_OFFSET_MULTIPLIER * offset;
	}
}

void Sprite::MoveUp(int offset)
{
	if (this->top - offset > 0)
	{
		this->top -= offset;
		this->bottom -= offset;
	}
	else
	{
		this->bottom -= this->top + MOVE_OFFSET_MULTIPLIER * offset;
		this->top = MOVE_OFFSET_MULTIPLIER * offset;
	}
}

void Sprite::MoveRight(int offset, int windowWidth)
{
	if (this->right + offset < windowWidth)
	{
		this->left += offset;
		this->right += offset;
	}
	else
	{
		this->left += windowWidth - this->right - MOVE_OFFSET_MULTIPLIER * offset;
		this->right += windowWidth - this->right - MOVE_OFFSET_MULTIPLIER * offset;
	}
}

void Sprite::MoveDown(int offset, int windowHeight)
{
	if (this->bottom + offset < windowHeight)
	{
		this->top += offset;
		this->bottom += offset;
	}
	else
	{
		this->top += windowHeight - this->bottom - MOVE_OFFSET_MULTIPLIER * offset;
		this->bottom += windowHeight - this->bottom - MOVE_OFFSET_MULTIPLIER * offset;
	}
}

RECT window;
Sprite *sprite = new Sprite(200, 200, SPRITE_WIDTH, SPRITE_HEIGHT);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPRITEMOVEMENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPRITEMOVEMENT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPRITEMOVEMENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPRITEMOVEMENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
		
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BITMAP bitmap;
			HDC hDC = BeginPaint(hWnd, &ps);
			HANDLE hBitmap = LoadImage(nullptr, SPRITE_FILENAME, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmap, sizeof(BITMAP), &bitmap);
			HDC hCompatibleDc = CreateCompatibleDC(hDC);
			HGDIOBJ hOldBitmap = SelectObject(hCompatibleDc, hBitmap);
			StretchBlt(hDC, sprite->left, sprite->top, sprite->width, sprite->height, hCompatibleDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
			SelectObject(hCompatibleDc, hOldBitmap);
			DeleteObject(hBitmap);
			DeleteDC(hCompatibleDc);
			EndPaint(hWnd, &ps);
		}
        break;

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_LEFT:
				{
					sprite->MoveLeft(MOVE_OFFSET);
				}
				break;

				case VK_UP:
				{
					sprite->MoveUp(MOVE_OFFSET);
				}
				break;

				case VK_RIGHT:
				{
					if (GetClientRect(hWnd, &window)) sprite->MoveRight(MOVE_OFFSET, window.right);
				}
				break;
				
				case VK_DOWN:
				{
					if (GetClientRect(hWnd, &window)) sprite->MoveDown(MOVE_OFFSET, window.bottom);
				}
				break;
				
				case VK_SHIFT:
				{
					shiftDown = true;
				}
				break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

		case WM_KEYUP:
		{
			case VK_SHIFT:
			{
				shiftDown = false;
			}
		}
		break;

		case WM_MOUSEWHEEL:
		{
			if ((int)wParam > 0)
			{
				if (shiftDown) sprite->MoveLeft(MOVE_OFFSET);
				else sprite->MoveUp(MOVE_OFFSET);
			}
			else if (GetClientRect(hWnd, &window))
			{
				if (shiftDown) sprite->MoveRight(MOVE_OFFSET, window.right);
				else sprite->MoveDown(MOVE_OFFSET, window.bottom);
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
    
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

	    case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
    }
    return (INT_PTR)FALSE;
}
