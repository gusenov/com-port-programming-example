// COMAPIc.cpp 
// www.softelectro
//Electron18

#include "stdafx.h"
#define STYLBTN WS_CHILD|BS_DEFPUSHBUTTON|WS_VISIBLE
#define STYLEDT WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP 

 		//data main window
	HINSTANCE hInst;											
	LPCTSTR szTitle="COMAPIc v.1.00";						
	LPCTSTR szWindowClass="SimpleWinClass";
		//data control
	LPCTSTR CLSBUTN="BUTTON";
	LPCTSTR CPBUT1="Open port";
	LPCTSTR CPBUT2="Close port";
	LPCTSTR CPBUT3="Read port";
	LPCTSTR CPBUT4="Write port";
	LPCTSTR CLSEDIT="EDIT";
	LPCTSTR CPEDT1;
	LPCTSTR CPEDT2="Hello World!";
	HWND HWNDBTN1;
	HWND HWNDBTN2;
	HWND HWNDBTN3;
	HWND HWNDBTN4;
	HWND HWNDEDT1;
	HWND HWNDEDT2;  
	HMENU ID_BTN1=(HMENU) 101;
	HMENU ID_BTN2=(HMENU) 102;
	HMENU ID_BTN3=(HMENU) 103;
	HMENU ID_BTN4=(HMENU) 104;
	
        //App message
	LPCTSTR  lpCapApp = "App message";
	char  lpApp1[40]   = "Open port HANDLE:";
	LPCTSTR  lpApp2    ="Close port";
	LPCTSTR  lpApp3    ="Data read: Ok";
	LPCTSTR  lpApp4    ="Data write: Ok";
    
        //error message
	LPCTSTR  lpCapERR  ="Error Message";       
	char lpERR1[40]={"Open port Error:"};
	char lpERR2[40]={"DCB Structure ERROR:"};
	char lpERR3[40]={"SetComm Function ERROR:"};
	char lpERR4[40]={"Read port ERROR:"};
	char lpERR5[40]={"Write port ERROR:"};
	char lpERR6[40]={"Close port ERROR:"};
	DWORD Mem1;
	LPCTSTR Par1        ="%lu";

		//data com port	
	HANDLE HWNDCOM;
	LPCTSTR lpNumCOM="COM1:";
	LPCTSTR COMSETTING="Com1: baud=1200 parity=N data=8 stop=1";
	char Buf[255];
	DWORD LenBuf;
	DCB DCB1;

	
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void PurgeBuf()
{
	int a;
	for (a=0;a<255;a++)
	{
		Buf[a]=0x20;
	}
	return;
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	
	MSG msg;
	WNDCLASSEX wcex;
	HWND hWnd;
    hInst=hInstance;

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm		= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(4);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	RegisterClassEx(&wcex);

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,20, 20, 800, 200, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_SHOWNORMAL);
   UpdateWindow(hWnd);


	

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}





	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)

{	
	int wmId, wmEvent;
	int Retval;
	switch (message)

	{
		case WM_COMMAND:
				wmId    = LOWORD(wParam); 
				wmEvent = HIWORD(wParam); 
				switch (wmId)
			{
				case 101: //open port
					HWNDCOM=CreateFile(lpNumCOM, GENERIC_READ|GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL,NULL);
               
					if (HWNDCOM==INVALID_HANDLE_VALUE)
						{
							Mem1= GetLastError();
							wsprintf(&lpERR1[16],Par1,Mem1);
							MessageBox(0,lpERR1,lpCapERR,0);
						}
					else
						{   
							wsprintf(&lpApp1[17],Par1,HWNDCOM);
							MessageBoxA(0,lpApp1,lpCapApp,0);
							Retval=BuildCommDCB(COMSETTING,&DCB1);
							if (Retval==0)	//error DCB
								{
									Mem1= GetLastError();
									wsprintf(&lpERR2[20],Par1,Mem1);
									MessageBox(0,lpERR2,lpCapERR,0);
								}
							Retval=SetCommState(HWNDCOM,&DCB1);
							if (Retval==0)	//error SetCom
								{
									Mem1= GetLastError();
									wsprintf(&lpERR3[23],Par1,Mem1);
									MessageBox(0,lpERR3,lpCapERR,0);
								}
			   

						}
							break;

				case 102:			//close port
					Retval=CloseHandle(HWNDCOM);
					if (Retval==0) //error close port
						{
							Mem1= GetLastError();
							wsprintf(&lpERR6[17],Par1,Mem1);
							MessageBox(0,lpERR6,lpCapERR,0);
						}
					else
						{
							MessageBox(0,lpApp2,lpCapApp,0);
						}
					break;

				case 103:		//read port
					PurgeBuf();
					Retval= ReadFile(HWNDCOM,&Buf, 255,&LenBuf, NULL);
					if (Retval==0) //error read port
						{
							Mem1= GetLastError();
							wsprintf(&lpERR4[16],Par1,Mem1);
							MessageBox(0,lpERR4,lpCapERR,0);
						}
					else
						{
							SendMessageA(HWNDEDT1,WM_SETTEXT,sizeof Buf,(LPARAM) Buf);
							MessageBox(0,lpApp3,lpCapApp,0);
						}
           
						break;
				case 104:		//write port
					PurgeBuf();
					SendMessage(HWNDEDT2,WM_GETTEXT,sizeof Buf,(LPARAM) Buf);
					Retval= WriteFile(HWNDCOM,&Buf, sizeof Buf,&LenBuf, NULL);
					if (Retval==0) //error write port
						{
							Mem1= GetLastError();
							wsprintf(&lpERR5[17],Par1,Mem1);
							MessageBox(0,lpERR5,lpCapERR,0);
						}
					else
						{
							MessageBox(0,lpApp4,lpCapApp,0);
						}
						break;
				default:
						{ 
							return DefWindowProc(hWnd, message, wParam, lParam);
						}
			}
			break;
		case WM_CREATE:
			HWNDBTN1= CreateWindowEx( 0,CLSBUTN,CPBUT1,STYLBTN,10,10,100,20,hWnd,ID_BTN1,hInst,0);
			HWNDBTN2= CreateWindowEx( 0,CLSBUTN,CPBUT2,STYLBTN,10,40,100,20,hWnd,ID_BTN2,hInst,0);
			HWNDBTN3= CreateWindowEx( 0,CLSBUTN,CPBUT3,STYLBTN,10,70,100,20,hWnd,ID_BTN3,hInst,0);
			HWNDBTN4= CreateWindowEx( 0,CLSBUTN,CPBUT4,STYLBTN,10,100,100,20,hWnd,ID_BTN4,hInst,0);
			HWNDEDT1= CreateWindowEx(0,CLSEDIT,CPEDT1,STYLEDT,120,70,600,20,hWnd,0,hInst,0);
			HWNDEDT2= CreateWindowEx(0,CLSEDIT,CPEDT2,STYLEDT,120,100,600,20,hWnd,0,hInst,0);

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

