#include "CaptureVideo.h"
#include "CaptureAudio.h"
#include "resource.h"
#define ID_COMBOBOX  10000
#define ID_COMBOBOX2 10001
#define ID_TIMER     10002

CaptureVideo g_CaptureVideo;

CaptureAudio g_CaptureAudio;
HWND hwndCombo1;
HWND hwndCombo2;
BSTR bstrDeviceName;

HICON g_hIconLarge;
HICON g_hIconSmall;

int g_nTimerCount = 0;

INT_PTR CALLBACK WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
VOID CALLBACK TimerGetPicture(HWND hDlg, UINT message, UINT_PTR iTimerID, DWORD dwTimer);
VOID SetWindowPosCenter(HWND hDlg);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{


	g_hIconLarge = static_cast<HICON>(LoadImage(hInstance, TEXT("IDI_ICON1"), IMAGE_ICON,  //set large ico
		GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CXICON), 0));
	g_hIconSmall = static_cast<HICON>(LoadImage(hInstance, TEXT("IDI_ICON1"), IMAGE_ICON,   //set small ico
		GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON), 0));

    MSG msg;
	HWND hDlg = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL, WndProc);
	    
	ShowWindow(hDlg,iCmdShow);
	UpdateWindow(hDlg);


    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

	CoUninitialize();
	// Exit
	DestroyIcon(g_hIconLarge);
	DestroyIcon(g_hIconSmall);	
    return (int)msg.wParam;
}

INT_PTR CALLBACK WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nGetComboxCount = 0;
	int iGetCurSel = 0;
	DWORD dwBaseUnits;
	switch(message)
	{
	case WM_INITDIALOG:
        {
			SendMessage(hDlg,WM_SETICON,FALSE,reinterpret_cast<LPARAM>(g_hIconSmall));
			SendMessage(hDlg,WM_SETICON,TRUE,reinterpret_cast<LPARAM>(g_hIconLarge));
			//////////////////////////////////////////////////////////////////////////
			SetWindowPosCenter(hDlg); //set Dialog at window center
			//////////////////////////////////////////////////////////////////////////
			g_CaptureVideo.m_App = hDlg;
			g_CaptureAudio.m_App = hDlg;
			dwBaseUnits = GetDialogBaseUnits(); 
			hwndCombo1 = CreateWindow(TEXT("COMBOBOX"), TEXT(""), 
				CBS_DROPDOWN | WS_CHILD | WS_VISIBLE, 
				(6 * LOWORD(dwBaseUnits)) / 4, 
				(2 * HIWORD(dwBaseUnits)) / 8, 
				(100 * LOWORD(dwBaseUnits)) / 4, 
				(50 * HIWORD(dwBaseUnits)) / 8, 
				hDlg, (HMENU)ID_COMBOBOX, NULL, NULL);   

			hwndCombo2 = CreateWindow(TEXT("COMBOBOX"), TEXT(""), 
				CBS_DROPDOWN | WS_CHILD | WS_VISIBLE, 
				(110 * LOWORD(dwBaseUnits)) / 4, 
				(2 * HIWORD(dwBaseUnits)) / 8, 
				(100 * LOWORD(dwBaseUnits)) / 4, 
				(50 * HIWORD(dwBaseUnits)) / 8, 
				hDlg, (HMENU)ID_COMBOBOX2, NULL, NULL);   

			//Video
			g_CaptureVideo.EnumAllDevices(hwndCombo1); //Enum All Camera
			nGetComboxCount = ComboBox_GetCount(hwndCombo1);
			if (nGetComboxCount == 0)
				ComboBox_Enable(hwndCombo1,FALSE);
			else
				ComboBox_SetCurSel(hwndCombo1,0);

			if(g_CaptureVideo.m_nCaptureDeviceNumber == 0)
			{
				Msg(hDlg,TEXT("没有摄像头设备"));
				EnableWindow(GetDlgItem(hDlg,IDC_PREVIWE),FALSE);
			}
			EnableWindow(GetDlgItem(hDlg,IDONESHOT),FALSE);

			// Audio
			g_CaptureAudio.EnumAllDevices(hwndCombo2);
			nGetComboxCount = ComboBox_GetCount(hwndCombo2);
			if (nGetComboxCount == 0)
				ComboBox_Enable(hwndCombo2,FALSE);
			else
				ComboBox_SetCurSel(hwndCombo2,0);
			if(g_CaptureAudio.m_nCaptureDeviceNumber == 0)
			{
				Msg(hDlg,TEXT("没有音频设备"));
				EnableWindow(GetDlgItem(hDlg,IDC_PREVIWE),FALSE);
			}
        }
		return TRUE;
	case WM_DESTROY:
        {
			g_CaptureVideo.CloseInterface();
			PostQuitMessage(0);
        }
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
            {
                PostQuitMessage(0);
            }
           break;
		case IDONESHOT:
            {
                //g_CaptureVideo.GrabOneFrame(TRUE);
				SetTimer(hDlg,ID_TIMER,150, TimerGetPicture);
            }
			break;
		
		case IDC_PREVIWE:
			{
				//Video
				iGetCurSel = ComboBox_GetCurSel(hwndCombo1);
				g_CaptureVideo.OpenDevice(iGetCurSel,20,30,430,400);
				EnableWindow(GetDlgItem(hDlg,IDONESHOT),TRUE);

				//Audio
				iGetCurSel = ComboBox_GetCurSel(hwndCombo2);
				bstrDeviceName = SysAllocString(g_CaptureAudio.m_pCapDeviceName[iGetCurSel]);
				g_CaptureAudio.OpenDevice(bstrDeviceName);
			}
			break;
		default: break;
		}
	case WM_MOVE:
		g_CaptureVideo.m_pVideoWindow->NotifyOwnerMessage((OAHWND)hDlg, message, wParam, lParam);
		break;
	}
	return (FALSE);
}

VOID CALLBACK TimerGetPicture(HWND hDlg, UINT message, UINT_PTR iTimerID, DWORD dwTimer)
{
	if(g_nTimerCount < 25)
	{
		g_CaptureVideo.GrabOneFrame(TRUE);
		g_nTimerCount++;
	}else
	{
		KillTimer(hDlg,ID_TIMER);
		g_nTimerCount = 0;
	}
}

VOID SetWindowPosCenter(HWND hDlg)
{
	int cxWindow,cyWindow;  //window Screen width and height
	RECT hDlgRect;          //Dialog Rect
	int cxDialog,cyDialog;  //Dialog Screen width and height
	int cxSetPos,cySetPos;

	GetWindowRect(hDlg,&hDlgRect);
	//
	cxDialog = hDlgRect.right - hDlgRect.left;
	cyDialog = hDlgRect.bottom - hDlgRect.top;
	//
	cxWindow = GetSystemMetrics(SM_CXSCREEN);
	cyWindow = GetSystemMetrics(SM_CYSCREEN);
	//
	cxSetPos = (cxWindow-cxDialog)/2;
	cySetPos = (cyWindow-cyDialog)/2;

	SetWindowPos(hDlg,NULL,cxSetPos,cySetPos,0,0,SWP_NOSIZE);


}



//DWORD dwBaseUnits;
//int nGetComboxCount;
//LRESULT CALLBACK WndMainProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//	case WM_GRAPHNOTIFY:
//		g_CaptureVideo.HandleGraphEvent();
//		break;
//
//	case WM_CLOSE:            
//		// Hide the main window while the graph is destroyed
//		ShowWindow(hwnd, SW_HIDE);
//		g_CaptureVideo.CloseInterface();  // Stop capturing and release interfaces
//		break;
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	// Pass this message to the video window for notification of system changes
//	if (g_CaptureVideo.m_pVideoWindow)
//		g_CaptureVideo.m_pVideoWindow->NotifyOwnerMessage((LONG_PTR) hwnd, message, wParam, lParam);
//
//	return DefWindowProc (hwnd , message, wParam, lParam);
//}
//
//
//int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hInstP, LPSTR lpCmdLine, int nCmdShow)
//{
//	MSG msg={0};
//	WNDCLASS wc;
//
//	// Initialize COM
//	if(FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))   
//		exit(1);
//
//	// Register the window class
//	ZeroMemory(&wc, sizeof wc);
//	wc.lpfnWndProc   = WndMainProc;
//	wc.hInstance     = hInstance;
//	wc.lpszClassName = TEXT("video");
//	wc.lpszMenuName  = NULL;
//	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
//	wc.hIcon         = NULL;
//	if(!RegisterClass(&wc))
//	{
//		CoUninitialize();
//		exit(1);
//	}
//
//	// Create the main window.  The WS_CLIPCHILDREN style is required.
//	HWND ghApp = CreateWindow(TEXT("video"), TEXT("xxxx"),
//		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_CLIPCHILDREN,
//		CW_USEDEFAULT, CW_USEDEFAULT,
//		600, 600,
//		0, 0, hInstance, 0);
//
//	if(ghApp)
//	{
//		HRESULT hr;
//
//		// Create DirectShow graph and start capturing video
//		g_CaptureVideo.m_App = ghApp;
//		dwBaseUnits = GetDialogBaseUnits(); 
//		hwndCombo = CreateWindow(TEXT("COMBOBOX"), TEXT(""), 
//			CBS_DROPDOWN | WS_CHILD | WS_VISIBLE, 
//			(6 * LOWORD(dwBaseUnits)) / 4, 
//			(2 * HIWORD(dwBaseUnits)) / 8, 
//			(100 * LOWORD(dwBaseUnits)) / 4, 
//			(50 * HIWORD(dwBaseUnits)) / 8, 
//			ghApp, (HMENU)ID_COMBOBOX, NULL, NULL);   
//		hr = g_CaptureVideo.EnumAllDevices(hwndCombo); //Enum All Camera
//		if (FAILED (hr))
//		{
//			g_CaptureVideo.CloseInterface();
//			DestroyWindow(ghApp);
//		}
//		else
//		{
//			ShowWindow(ghApp, nCmdShow);
//		}       
//
//		nGetComboxCount = ComboBox_GetCount(hwndCombo);
//		if (nGetComboxCount == 0)
//			ComboBox_Enable(hwndCombo,FALSE);
//		else
//			ComboBox_SetCurSel(hwndCombo,0);
//
//		SetWindowPos(ghApp,NULL,200,200,0,0,SWP_NOSIZE);
//		if(g_CaptureVideo.m_nCaptureDeviceNumber == 0)
//		{
//			MessageBox(ghApp,TEXT("没有摄像头设备"),TEXT("Message"),MB_OK);
//		}
//		int iGetCurSel = ComboBox_GetCurSel(hwndCombo);
//		g_CaptureVideo.OpenDevice(iGetCurSel,20,30,430,400);
//
//		
//
//		// Main message loop
//		while(GetMessage(&msg,NULL,0,0))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}
//
//	// Release COM
//	CoUninitialize();
//
//	return (int) msg.wParam;
//}