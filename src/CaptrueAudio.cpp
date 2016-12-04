#include "CaptureAudio.h"


CaptureAudio::CaptureAudio()
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
	m_pMediaContrl = NULL;
	m_App = NULL;
	InitializeEnv();
}

CaptureAudio::~CaptureAudio()
{
	CloseInterface();
	CoUninitialize();
}

HRESULT CaptureAudio::EnumAllDevices(HWND hCombox)
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
		// get all Audio input Device Friendly Name
		hr = pDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory,&pEnumMon, 0);
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

		//get All Audio Renderer Device Friendly Name
		hr = pDevEnum->CreateClassEnumerator(CLSID_AudioRendererCategory,&pEnumMon, 0);
		if (hr == S_FALSE)
		{
			hr = VFW_E_NOT_FOUND;
			return hr;
		}
		pEnumMon->Reset();
		int index = 0;
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
					StringCchCopy(m_pRendererDeviceName[index],MAX_PATH,varTemp.bstrVal);
					index++; //Device number add 1
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

HRESULT CaptureAudio::InitializeEnv()
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

	hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl,(LPVOID*)&m_pMediaContrl);
	if(FAILED(hr))
		return hr;
	m_pCaptureGB->SetFiltergraph(m_pGraphBuilder);
	if(FAILED(hr))
		return hr;
	return hr;
}

void CaptureAudio::CloseInterface()
{
	if (m_pMediaContrl)
		m_pMediaContrl->Stop();
	//release interface
	ReleaseInterface(m_pDevFilter);
	ReleaseInterface(m_pCaptureGB);
	ReleaseInterface(m_pGraphBuilder);
	ReleaseInterface(m_pMediaContrl);
}

HRESULT CaptureAudio::OpenDevice(BSTR bstrDeviceName)
{
	HRESULT hr;
	IBaseFilter  *pWaveDest = NULL, *pWriter = NULL, *pOutputDev = NULL;
	IFileSinkFilter *pSink= NULL;
	BSTR bstrTempDeviceName;
	BSTR bstrOutPin,bstrInPin;
	IBaseFilter *pPinTeeFilter = NULL;
	
	hr = BindDeviceFilter(bstrDeviceName,CLSID_AudioInputDeviceCategory, &m_pDevFilter);
	if (FAILED(hr))
		return hr;
	// Add the audio capture filter to the filter graph. 
	hr = m_pGraphBuilder->AddFilter(m_pDevFilter, L"Capture");
	if (FAILED(hr))
		return hr;
	/************************************************************************/
	/* Audio Prieview                                           */
	/************************************************************************/
	bstrTempDeviceName = SysAllocString(L"Infinite Pin Tee Filter");
	hr = BindDeviceFilter(bstrTempDeviceName,CLSID_LegacyAmFilterCategory,&pPinTeeFilter);
	if (FAILED(hr))
		return hr;
	hr = m_pGraphBuilder->AddFilter(pPinTeeFilter,bstrTempDeviceName);
	bstrInPin = SysAllocString(L"Capture");
	bstrOutPin = SysAllocString(L"Input");
	
	hr = DeviceConnect(m_pDevFilter,pPinTeeFilter,bstrInPin,bstrOutPin);
	if(FAILED(hr))
		return hr;
	//////////////////////////////////////////////////////////////////////////
	// output the Rendered Device
	SysReAllocString(&bstrTempDeviceName,m_pRendererDeviceName[0]); //Rendered Device Name;
	hr = BindDeviceFilter(bstrTempDeviceName,CLSID_AudioRendererCategory,&pOutputDev);
	if(FAILED(hr))
		return hr;
	hr = m_pGraphBuilder->AddFilter(pOutputDev,bstrTempDeviceName);
	if(FAILED(hr))
		return hr;
	SysReAllocString(&bstrInPin,L"Output1");
	SysReAllocString(&bstrOutPin,L"Audio Input pin (rendered)");
	hr = DeviceConnect(pPinTeeFilter,pOutputDev,bstrInPin,bstrOutPin);
	if(FAILED(hr))
		return hr;
	//////////////////////////////////////////////////////////////////////////


	//Add AudioRecorder WAV Dest
	SysReAllocString(&bstrTempDeviceName,TEXT("AudioRecorder WAV Dest"));
	hr = BindDeviceFilter(bstrTempDeviceName,CLSID_LegacyAmFilterCategory, &pWaveDest);
	if (FAILED(hr))
		return hr;
	hr = m_pGraphBuilder->AddFilter(pWaveDest,bstrTempDeviceName);
	if (FAILED(hr))
		return hr;
	//Connect the AudioRecorder WAV Dest
	SysReAllocString(&bstrInPin,L"Output2");
	SysReAllocString(&bstrOutPin,L"In");
	hr = DeviceConnect(pPinTeeFilter,pWaveDest,bstrInPin,bstrOutPin);
	if (FAILED(hr))
		return hr;
	
	// output Filter Wirter
	SysReAllocString(&bstrTempDeviceName,L"File writer");
	hr = BindDeviceFilter(bstrTempDeviceName,CLSID_LegacyAmFilterCategory,&pWriter);	
	if (FAILED(hr))
		return hr;

	IFileSinkFilter *pFileSink= NULL;
	SysReAllocString(&bstrTempDeviceName,L"test.wav");
	hr = pWriter->QueryInterface(IID_IFileSinkFilter, (void**)&pFileSink);
	if(FAILED(hr))
		return hr;
	hr = pFileSink->SetFileName((LPCOLESTR)bstrTempDeviceName,NULL);
	if(FAILED(hr))
		return hr;
	hr = m_pGraphBuilder->AddFilter(pWriter,bstrTempDeviceName);
	if(FAILED(hr))
		return hr;

	SysReAllocString(&bstrInPin,L"Out");
	SysReAllocString(&bstrOutPin,L"in");

	hr = DeviceConnect(pWaveDest,pWriter,bstrInPin,bstrOutPin);
	if(FAILED(hr))
		return hr;
	SysFreeString(bstrInPin);
	SysFreeString(bstrOutPin);
	SysFreeString(bstrTempDeviceName);

	hr = m_pMediaContrl->Run();
	if(FAILED(hr))
		return hr;
	return hr;

}

HRESULT CaptureAudio::DeviceConnect(IBaseFilter* pInputDevice,IBaseFilter* pOutputDevice, BSTR bstrInputPin, BSTR bstrOutputDevice)
{
	HRESULT hr;
	IEnumPins *pInputPins = NULL, *pOutputPins = NULL;
	IPin *pIn = NULL, *pOut = NULL;

	hr = pInputDevice->EnumPins(&pInputPins);
	if (SUCCEEDED(hr))
	{
		hr = pInputDevice->FindPin(bstrInputPin,&pIn);
		if(FAILED(hr))
			return hr;
	}else
		return hr;

	hr = pOutputDevice->EnumPins(&pOutputPins);
	if (SUCCEEDED(hr))
	{
		hr = pOutputDevice->FindPin(bstrOutputDevice,&pOut);
		if(FAILED(hr))
			return hr;
	}

	hr = pIn->Connect(pOut, NULL);
	if (FAILED(hr))
		return hr;
	return hr;
}

HRESULT CaptureAudio::BindDeviceFilter(BSTR deviceName,GUID DEVICE_CLSID,IBaseFilter **pBaseFilter)
{
	ICreateDevEnum *pDevEnum;
	IEnumMoniker   *pEnumMon;
	IMoniker	   *pMoniker;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum,(LPVOID*)&pDevEnum);
	if (SUCCEEDED(hr))
	{
		hr = pDevEnum->CreateClassEnumerator(DEVICE_CLSID,&pEnumMon, 0);
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
					if (Bstr_Compare(varTemp.bstrVal,deviceName) == true)
					{
						hr = pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(LPVOID*)pBaseFilter);
						if (SUCCEEDED(hr))
							return hr;
					}
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
