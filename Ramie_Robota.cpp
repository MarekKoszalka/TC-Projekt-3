//Autorzy: Marek Koszalka, Krystian Bielinski
// Ramie_Robota.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "Ramie_Robota.h"
#include "math.h"
#include <string>
#include <ctime>

using namespace std;

#define MAX_LOADSTRING		100/*----------------------------------------------------------------------------------------------STALE---------------------*/
#define PI					3.14159265
#define ARM1				350
#define ARM2				250
#define LOWLEVEL			400
#define TMR_1				1
#define ARM_SPEED			0.8
#define BOX_WIDE			30
#define BOX_HIGH			50
#define RAIL_WIDTH			5

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT value;

//buttons
HWND hwndButton;
/*----------------------------------------------------DEFINICJE ZMIENNYCH---------------------------------------*/
int stage;
int vertRail;
int horizontalRail;
int which_one;
int x2,x3;
int y2,y3;
int rx[4];
int ry[4];
int* wsk_rx[4];
int* wsk_ry[4];
int waga[4];
float degrees1;
float degrees2;
int sortBoxByWeight[4];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void WriteWeightOfBox(int i,HDC hdc)
{
	FontFamily  fontFamily(L"Times New Roman");
	Font        fontW(&fontFamily, 12, FontStyleRegular, UnitPixel);
	SolidBrush  solidBrush(Color(255, 0, 0, 255));

	PointF pointFW;

	pointFW.X = ((float)*wsk_rx[i]);
	pointFW.Y = ((float)*wsk_ry[i]);

	Graphics graphics(hdc);
		
	switch(waga[i])
	{
	case 0: 
		graphics.DrawString(L"5kg", -1, &fontW, pointFW, &solidBrush);
		break;
	case 1:
		graphics.DrawString(L"10kg", -1, &fontW, pointFW, &solidBrush);
		break;
	case 2:
		graphics.DrawString(L"15kg", -1, &fontW, pointFW, &solidBrush);
		break;
	case 3:
		graphics.DrawString(L"20kg", -1, &fontW, pointFW, &solidBrush);
		break;
	default:
		graphics.DrawString(L"Error!", -1, &fontW, pointFW, &solidBrush);
		break;
	}
}


int InWhichBoxArmIs(void)/*--------------------------------------------------------------------------------------IN WHICH BOX ARM IS-------------------*/
{ 
	bool czy_w_boxie = false;

	for(int i=0; i<=3; i++)
	{
		if(x3>=*wsk_rx[i] && x3<=(*wsk_rx[i]+BOX_WIDE) && y3 >= *wsk_ry[i] && y3 <= (*wsk_ry[i]+BOX_HIGH))
		{
			czy_w_boxie = true;
			return i;
		}
	}
	if(!czy_w_boxie)
	{
		return 100;
	}
}

void HorizontalShifting(int Xshift, int Direction)
{
	if(!(x3 <= Xshift + RAIL_WIDTH && x3 >= Xshift -RAIL_WIDTH))
	{
		if((y3 <= (horizontalRail + RAIL_WIDTH))&&(y3 >= (horizontalRail - RAIL_WIDTH)))
		{
			degrees1 -= Direction*ARM_SPEED;
		}else
		{
			if(y3 > horizontalRail)
			{
						degrees2 += ARM_SPEED;
				}else
				{
						degrees2 -= ARM_SPEED;
			}
		}
	}else stage++;
}


void DownShifting(int Yshift, int vertRail){
	if(y3 <= Yshift)
	{
		if((x3 <= (vertRail + RAIL_WIDTH))&&(x3 >= (vertRail - RAIL_WIDTH)))
		{
			degrees1 -= ARM_SPEED;
		}else
		{
			if(x3 > vertRail)
			{
						degrees2 -= ARM_SPEED;
				}else
				{
						degrees2 += ARM_SPEED;
			}
		}
	}else stage++;
};


void Catch(void)
{
	which_one= InWhichBoxArmIs();
	wsk_rx[which_one]= &x3;		
	wsk_ry[which_one]= &y3;
	stage++;
}

void PutDown(void)
{
	rx[which_one]= *wsk_rx[which_one];
	ry[which_one]= *wsk_ry[which_one];
	wsk_rx[which_one]= &rx[which_one];
	wsk_ry[which_one]= &ry[which_one];
	stage++;
}




void OnPaint(HDC hdc)/*-----------------------------------------------------------------------------------------------FUNKCJA RYSUJACA-------------------*/
{

	x2= (int)ARM1*sin(degrees1*PI / 180);
	y2= LOWLEVEL+(int)ARM1*cos(degrees1*PI / 180);
	x3= (int)(ARM2*sin((degrees1+degrees2)*PI / 180)+ARM1*sin(degrees1*PI / 180));
	y3= (int)(ARM2*cos((degrees1+degrees2)*PI/180)+LOWLEVEL+ARM1*cos(degrees1*PI / 180));

	int zabezpieczenie= 5;
		//rx[i]=400+ i*(BOX_WIDE+10);
		//ry[i]=LOWLEVEL - BOX_HIGH
		//sortBoxByWeight[i]

	switch(stage)   //-------------------------------------------------------------------PRACA AUTOMATYCZNA----------------*/
	{
		case 1:
			HorizontalShifting(	rx[sortBoxByWeight[0]]+zabezpieczenie, 1);
			break;
		case 2:
			DownShifting(ry[sortBoxByWeight[0]]+zabezpieczenie, rx[sortBoxByWeight[0]]+zabezpieczenie);
			break;
		case 3:
			Catch();
			break;
		case 4:
			HorizontalShifting(	180+zabezpieczenie, -1);
			break;
		case 5:
			DownShifting(LOWLEVEL-55, 180+zabezpieczenie);
			break;
		case 6: PutDown();
			break;
		case 7:	
			HorizontalShifting(	rx[sortBoxByWeight[1]]+zabezpieczenie, 1);
			break;
		case 8: 
			DownShifting(ry[sortBoxByWeight[1]]+zabezpieczenie, rx[sortBoxByWeight[1]]+zabezpieczenie);
			break;
		case 9:
			Catch();
			break;
		case 10:
			HorizontalShifting(	220+zabezpieczenie, -1);
			break;
		case 11:
			DownShifting(LOWLEVEL-55, 220+zabezpieczenie);
			break;
		case 12:
			PutDown();
			break;
		case 13:
			HorizontalShifting(	rx[sortBoxByWeight[2]]+zabezpieczenie, 1);
			break;
		case 14:
			DownShifting(ry[sortBoxByWeight[2]]+zabezpieczenie, rx[sortBoxByWeight[2]]+zabezpieczenie);
			break;
		case 15:
			Catch();
			break;
		case 16:
			HorizontalShifting(	260+zabezpieczenie, -1);
			break;
		case 17:
			DownShifting(LOWLEVEL-55, 260+zabezpieczenie);
			break;
		case 18:
			PutDown();
			break;
		case 19:
			HorizontalShifting(	rx[sortBoxByWeight[3]]+zabezpieczenie, 1);
			break;
		case 20:
			DownShifting(ry[sortBoxByWeight[3]]+zabezpieczenie, rx[sortBoxByWeight[3]]+zabezpieczenie);
			break;
		case 21:
			Catch();
			break;
		case 22:
			HorizontalShifting(	300+zabezpieczenie, -1);
			break;
		case 23:
			DownShifting(LOWLEVEL-55, 300+zabezpieczenie);
			break;
		case 24:
			PutDown();
			break;
		case 25:
			HorizontalShifting(	400, 1);
		default:
			break;

	}
	

	FontFamily  fontFamily(L"Times New Roman");
	Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	PointF      pointF(30.0f, 10.0f);
	SolidBrush  solidBrush(Color(255, 0, 0, 255));
	
	Graphics graphics(hdc);
	
	switch(InWhichBoxArmIs())
	{
	case 0:
		graphics.DrawString(L"ZERO", -1, &font, pointF, &solidBrush);
		break;
	case 1:
		graphics.DrawString(L"JEDEN", -1, &font, pointF, &solidBrush);
		break;
	case 2:
		graphics.DrawString(L"DWA", -1, &font, pointF, &solidBrush);
		break;
	case 3:
		graphics.DrawString(L"TRZY", -1, &font, pointF, &solidBrush);
		break;
	case 100:
		graphics.DrawString(L"POZA BOXAMI", -1, &font, pointF, &solidBrush);
		break;
	default:
		graphics.DrawString(L"COS SIE POPSULO", -1, &font, pointF, &solidBrush);
		break;
	}

	Pen blackpen(Color(255,0,0,0));
	Pen bluepen(Color(255,0,0,255),4);
	bluepen.SetEndCap(LineCapRoundAnchor);
	graphics.DrawLine(&bluepen, 0, LOWLEVEL, x2, y2);
	graphics.DrawLine(&bluepen, x2, y2, x3, y3);
	graphics.DrawLine(&blackpen,0,LOWLEVEL,1400,LOWLEVEL);
	
	for(int i=0; i<=3; i++){
		graphics.DrawRectangle(&blackpen,*wsk_rx[i],*wsk_ry[i],BOX_WIDE,BOX_HIGH);
		WriteWeightOfBox(i, hdc);
	}
}


int OnCreate(HWND window)/*----------------------------------------------------------------------TIMER---------------------------------------------------*/
{
   SetTimer(window, TMR_1, 25, 0);
   return 0;
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);



 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	srand(time(NULL));
	/*-------------------------------------------------------------------------------------------ZMIENNE--------------------------WARTOSCI POCZATKOWE-----*/
	for(int i= 0; i<=3; i++)
	{
		rx[i]=400+ i*(BOX_WIDE+10);
		ry[i]=LOWLEVEL - BOX_HIGH;
		wsk_rx[i]= &rx[i];
		wsk_ry[i]= &ry[i];
		waga[i]= rand()% 4;
		sortBoxByWeight[i]= i;
	}

	for(int n=2; n>=0; n--)
	{
		for(int i= 0; i<=n; i++)
		{
			if(waga[sortBoxByWeight[i]]< waga[sortBoxByWeight[i+1]])
			{
				int container;
				container= sortBoxByWeight[i+1];
				sortBoxByWeight[i+1]= sortBoxByWeight[i];
				sortBoxByWeight[i]= container;
			}
		}
	}

	stage= 1;
	horizontalRail= LOWLEVEL -120;
	value= 10;
	degrees1= 170.0;
	degrees2= -90.0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAMIE_ROBOTA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAMIE_ROBOTA));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAMIE_ROBOTA));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAMIE_ROBOTA);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   OnCreate(hWnd);
/*-----------------------------------------------------------------------------------------------------------------WYGLAD PRZYCISKOW---------------------*/
//UP BUTTON
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("UP"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		680, 10,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON_UP,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
//DOWN BUTTON
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("DOWN"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		680, 60,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON_DOWN,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
//LEFT BUTTON
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("LEFT"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		600, 60,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON_LEFT,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
//RIGHT BUTTON
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("RIGHT"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		760, 60,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON_RIGHT,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
//CATCH BUTTON
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("CATCH"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		600, 200,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON_CATCH,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
//PUT DOWN BUTTON
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("PUT DOWN"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		680, 200,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON_PUT_DOWN,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

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
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	//OnCreate(hWnd,wParam,lParam);
	//OnTimer(hWnd,wParam,lParam);
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON_UP:/*-------------------------------------------------------------------------FUNKCJE PRZYCISKOW-----------------------------------*/
			degrees2 += ARM_SPEED;
			break;
		case ID_BUTTON_DOWN:
			degrees2 -= ARM_SPEED;
			break;
		case ID_BUTTON_LEFT:
			degrees1 += ARM_SPEED;
			break;
		case ID_BUTTON_RIGHT:
			degrees1 -= ARM_SPEED;
			break;
		case ID_BUTTON_CATCH: /* -------------------------------TRZEBA ZROBIC FUNKCJE KTORA SPRAWDZA KTORY PODNOSIMY KLOCEK-------------------------------*/
			which_one= InWhichBoxArmIs();
			wsk_rx[which_one]= &x3;		
			wsk_ry[which_one]= &y3;
			break;
		case ID_BUTTON_PUT_DOWN:
			rx[which_one]= *wsk_rx[which_one];
			ry[which_one]= *wsk_ry[which_one];
			wsk_rx[which_one]= &rx[which_one];
			wsk_ry[which_one]= &ry[which_one];
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
			case TMR_1:
				//force window to repaint
				InvalidateRect(hWnd, NULL, TRUE);
				hdc = BeginPaint(hWnd, &ps);
				OnPaint(hdc);
				EndPaint(hWnd, &ps);
			break;
		}
		
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)/*---------------------------------NIE WIEM CO TO, NIE ZMIENIAC-------------*/
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
