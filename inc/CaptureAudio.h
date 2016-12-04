#ifndef __CAPTUREAUDIO_H__
#define __CAPTUREAUDIO_H__
#include "common.h"

class CaptureAudio
{
public:
	CaptureAudio();
	~CaptureAudio();
	HRESULT InitializeEnv(); //initialize environment
	HRESULT EnumAllDevices(HWND hCombox);  //Enumeration all devices
	void CloseInterface(); //close all interface
	HRESULT OpenDevice(BSTR bstrDeviceName);
	HRESULT BindDeviceFilter(BSTR deviceName,GUID DEVICE_CLSID,IBaseFilter **pBaseFilter);
	HRESULT	DeviceConnect(IBaseFilter* pInputDevice,IBaseFilter* pOutputDevice, BSTR bstrInputPin, BSTR bstrOutputDevice);

private:
	IGraphBuilder *m_pGraphBuilder;
	ICaptureGraphBuilder2 *m_pCaptureGB;
	IBaseFilter *m_pDevFilter;
	IMediaControl *m_pMediaContrl;
public:
	int m_nCaptureDeviceNumber;      //Device Count
	//vector<string> m_vecCapDeviceName; //the Device name
	TCHAR m_pCapDeviceName[10][MAX_PATH]; //the Device name
	TCHAR m_pRendererDeviceName[10][MAX_PATH]; // Render Device name	
	HWND m_App;
};

#endif  //__CAPTUREAUDIO_H__