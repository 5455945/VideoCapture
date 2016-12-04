#include "CaptureVideo.h"

SampleGrabberCallback g_sampleGrabberCB;  //CallBack

CaptureVideo::CaptureVideo()
{
	//COM Library Initialize
	if (FAILED(CoInitialize(NULL)))
	{
		Msg(m_App,TEXT("CoInitialize Failed!\r\n"));
		return;
	}
	//initialize member variable
	m_nCaptureDeviceNumber = 0;
	m_pDevFilter = NULL;
	m_pCaptureGB = NULL;
	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pMediaEvent = NULL;
	m_pSampGrabber = NULL;
	m_pVideoWindow = NULL;
	m_App = NULL;
	m_bGetOneShot = FALSE;
	m_bConnect = FALSE;
	InitializeEnv();
}

CaptureVideo::~CaptureVideo()
{
	CloseInterface();
	CoUninitialize();
}

HRESULT CaptureVideo::EnumAllDevices(HWND hCombox)
{
	if (!hCombox)
		return S_FALSE;
	ICreateDevEnum *pDevEnum;
	IEnumMoniker   *pEnumMon;
	IMoniker	   *pMoniker;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC_SERVER,
			IID_ICreateDevEnum,(LPVOID*)&pDevEnum);
	if (SUCCEEDED(hr))
	{
		hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEnumMon, 0);
		if (hr == S_FALSE)
		{
			hr = VFW_E_NOT_FOUND;
			return hr;
		}
		pEnumMon->Reset();
		ULONG cFetched;
		while(hr=pEnumMon->Next(1,&pMoniker,&cFetched),hr == S_OK)
		{
			IPropertyBag *pProBag;
			hr = pMoniker->BindToStorage(0,0,IID_IPropertyBag,(LPVOID*)&pProBag);
			if (SUCCEEDED(hr))
			{
				VARIANT varTemp;
				varTemp.vt = VT_BSTR;
				hr = pProBag->Read(L"FriendlyName",&varTemp,NULL);
				if (SUCCEEDED(hr))
				{
					//int nStrSize = WideCharToMultiByte(CP_ACP,0,varTemp.bstrVal,-1,0,0,NULL,FALSE);
					//char *strName = new char[nStrSize];
					//WideCharToMultiByte(CP_ACP,0,varTemp.bstrVal,-1,strName,nStrSize,NULL,FALSE);
					//m_vecCapDeviceName.push_back(string(strName)); //push the device name to vector
					
					StringCchCopy(m_pCapDeviceName[m_nCaptureDeviceNumber],MAX_PATH,varTemp.bstrVal);
					m_nCaptureDeviceNumber++; //Device number add 1
					::SendMessage(hCombox, CB_ADDSTRING, 0,(LPARAM)varTemp.bstrVal);
					SysFreeString(varTemp.bstrVal);
				}
				pProBag->Release();
			}
			pMoniker->Release();
		}
		pEnumMon->Release();
	}
	return hr;
}

HRESULT CaptureVideo::InitializeEnv()
{
	HRESULT hr;

	//Create the filter graph
	hr = CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,
						  IID_IGraphBuilder,(LPVOID*)&m_pGraphBuilder);
	if(FAILED(hr))
		return hr;
	
	//Create the capture graph builder
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2,NULL,CLSCTX_INPROC_SERVER,
						  IID_ICaptureGraphBuilder2,(LPVOID*)&m_pCaptureGB);
	if(FAILED(hr))
		return hr;

	//Obtain interfaces for media control and Video Window
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl,(LPVOID*)&m_pMediaControl);
	if(FAILED(hr))
		return hr;

	hr = m_pGraphBuilder->QueryInterface(IID_IVideoWindow,(LPVOID*)&m_pVideoWindow);
	if(FAILED(hr))
		return hr;

	hr = m_pGraphBuilder->QueryInterface(IID_IMediaEventEx,(LPVOID*)&m_pMediaEvent);
	if(FAILED(hr))
		return hr;

	hr = m_pMediaEvent->SetNotifyWindow((OAHWND)m_App,WM_GRAPHNOTIFY,0);

	if(FAILED(hr))
		return hr;
	m_pCaptureGB->SetFiltergraph(m_pGraphBuilder);
	if(FAILED(hr))
		return hr;
	return hr;
}

void CaptureVideo::CloseInterface()
{
	m_bGetOneShot = FALSE;

	if (m_pMediaControl)
		m_pMediaControl->Stop();
	if(m_pVideoWindow)
	{
		m_pVideoWindow->get_Visible(FALSE);
		m_pVideoWindow->put_Owner(NULL);
	}

	if(m_pMediaEvent)
		m_pMediaEvent->SetNotifyWindow(NULL,WM_GRAPHNOTIFY,0);

	//release interface
	ReleaseInterface(m_pDevFilter);
	ReleaseInterface(m_pCaptureGB);
	ReleaseInterface(m_pGraphBuilder);
	ReleaseInterface(m_pMediaControl);
	ReleaseInterface(m_pMediaEvent);
	ReleaseInterface(m_pSampGrabber);
	ReleaseInterface(m_pVideoWindow);
}

HRESULT CaptureVideo::BindFilter(int deviceID, IBaseFilter **pBaseFilter)
{
	ICreateDevEnum *pDevEnum;
	IEnumMoniker   *pEnumMon;
	IMoniker	   *pMoniker;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum,(LPVOID*)&pDevEnum);
	if (SUCCEEDED(hr))
	{
		hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEnumMon, 0);
		if (hr == S_FALSE)
		{
			hr = VFW_E_NOT_FOUND;
			return hr;
		}
		pEnumMon->Reset();
		ULONG cFetched;
		int index = 0;
		while(hr=pEnumMon->Next(1,&pMoniker,&cFetched),hr == S_OK, index<= deviceID)
		{
			IPropertyBag *pProBag;
			hr = pMoniker->BindToStorage(0,0,IID_IPropertyBag,(LPVOID*)&pProBag);
			if (SUCCEEDED(hr))
			{
				if (index == deviceID)
				{
					pMoniker->BindToObject(0,0,IID_IBaseFilter,(LPVOID*)pBaseFilter);
				}
			}
			pMoniker->Release();
			index++;
		}
		pEnumMon->Release();
	}
	return hr;
}

HRESULT CaptureVideo::SetupVideoWindow(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight)
{
	HRESULT hr;
	hr = m_pVideoWindow->put_Owner((OAHWND)m_App);
	if (FAILED(hr))
		return hr;

	hr = m_pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
	if(FAILED(hr))
		return hr;

	ResizeVideoWindow(nLeft,nTop,nWidth,nHeight);

	hr = m_pVideoWindow->put_Visible(OATRUE);
	if(FAILED(hr))
		return hr;

	return hr;
}

void CaptureVideo::ResizeVideoWindow(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight)
{
	if(m_pVideoWindow)
	{
		m_pVideoWindow->SetWindowPosition(nLeft,nTop,nWidth,nHeight);
	}
}

HRESULT CaptureVideo::OpenDevice(int deviceID, LONG nLeft, LONG nTop, LONG nWide, LONG nHeight)
{
	HRESULT hr;
	IBaseFilter *pSampleGrabberFilter;
	if (m_bConnect)
	{
		CloseInterface();
		InitializeEnv();
	}

	hr = CoCreateInstance(CLSID_SampleGrabber,NULL,CLSCTX_INPROC_SERVER,
						  IID_IBaseFilter, (LPVOID*)&pSampleGrabberFilter);
	if(FAILED(hr))
		return hr;
	//bind device filter
	hr = BindFilter(deviceID,&m_pDevFilter);
	if (FAILED(hr))
		return hr;
	hr = m_pGraphBuilder->AddFilter(m_pDevFilter,L"Video Filter");
	if (FAILED(hr))
		return hr;

	hr = m_pGraphBuilder->AddFilter(pSampleGrabberFilter,L"Sample Grabber");
	if (FAILED(hr))
		return hr;

	hr = pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber,(LPVOID*)&m_pSampGrabber);
	if(FAILED(hr))
		return hr;

	//set media type
	AM_MEDIA_TYPE mediaType;
	ZeroMemory(&mediaType,sizeof(AM_MEDIA_TYPE));
	//Find the current bit depth
	HDC hdc=GetDC(NULL);
	int iBitDepth=GetDeviceCaps(hdc, BITSPIXEL);
	g_sampleGrabberCB.m_iBitCount = iBitDepth;
	ReleaseDC(NULL,hdc);
	//Set the media type
	mediaType.majortype = MEDIATYPE_Video;
	switch(iBitDepth)
	{
	case  8:
		mediaType.subtype=MEDIASUBTYPE_RGB8;
		break;
	case 16:
		mediaType.subtype=MEDIASUBTYPE_RGB555;
		break;
	case 24:
		mediaType.subtype=MEDIASUBTYPE_RGB24;
		break;
	case 32:
		mediaType.subtype=MEDIASUBTYPE_RGB32;
		break;
	default:
		return E_FAIL;
	}
	mediaType.formattype = FORMAT_VideoInfo;
	hr = m_pSampGrabber->SetMediaType(&mediaType);

	hr = m_pCaptureGB->RenderStream(&PIN_CATEGORY_PREVIEW,&MEDIATYPE_Video,
		m_pDevFilter,pSampleGrabberFilter,NULL);
	if(FAILED(hr))
		return hr;

	hr = m_pSampGrabber->GetConnectedMediaType(&mediaType);
	if(FAILED(hr))
		return hr;

	VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*) mediaType.pbFormat;
	g_sampleGrabberCB.m_lWidth = vih->bmiHeader.biWidth;
	g_sampleGrabberCB.m_lHeight = vih->bmiHeader.biHeight;
	// Configure the Sample Grabber
	hr = m_pSampGrabber->SetOneShot(FALSE);
	if (FAILED(hr))
		return hr;
	hr = m_pSampGrabber->SetBufferSamples(TRUE);
	if (FAILED(hr))
		return hr;
	// 1 = Use the BufferCB callback method.
	hr = m_pSampGrabber->SetCallback(&g_sampleGrabberCB,1);

	//set capture video Window
	SetupVideoWindow(nLeft,nTop,nWide,nHeight);
	hr = m_pMediaControl->Run();
	if(FAILED(hr))
		return hr;

	if (mediaType.cbFormat != 0)
	{
		CoTaskMemFree((PVOID)mediaType.pbFormat);
		mediaType.cbFormat = 0;
		mediaType.pbFormat = NULL;
	}
	if (mediaType.pUnk != NULL)
	{
		mediaType.pUnk->Release();
		mediaType.pUnk = NULL;
	}
	m_bConnect = TRUE;
	return hr;

}

void CaptureVideo::GrabOneFrame(BOOL bGrab)
{
	m_bGetOneShot = bGrab;
	g_sampleGrabberCB.m_bGetPicture = bGrab;
}

HRESULT CaptureVideo::HandleGraphCapturePicture(void)
{
	//////////////////////////////////////////////////////////////////////////
	HRESULT hr;
	long evCode = 0;
	long lBufferSize = 0;
	BYTE *p;
	hr = m_pMediaEvent->WaitForCompletion(INFINITE, &evCode); //
	if (SUCCEEDED(hr))
	{
		switch(evCode)
		{
		case EC_COMPLETE:
			m_pSampGrabber->GetCurrentBuffer(&lBufferSize, NULL);
			p = new BYTE[lBufferSize];
			m_pSampGrabber->GetCurrentBuffer(&lBufferSize, (LONG*)p);// get Current buffer
			g_sampleGrabberCB.SaveBitmap(p,lBufferSize); //save bitmap
			delete [] p;
			p = NULL;
			break;
		default:
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	return hr;
}
