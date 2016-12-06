#ifndef __SAMPLEGRABBERCALLBACK_H__
#define __SAMPLEGRABBERCALLBACK_H__
#include "common.h"

class SampleGrabberCallback : public ISampleGrabberCB
{
public:
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void** ppvObject);

	HRESULT STDMETHODCALLTYPE SampleCB(double Time, IMediaSample *pSample);
	HRESULT STDMETHODCALLTYPE BufferCB(double Time, BYTE *pBuffer, long BufferLen);
	
	SampleGrabberCallback();
	BOOL SaveBitmap(BYTE * pBuffer, long lBufferSize ); //保存bitmap图片
	bool BitmapToJpg(const char* szSrcFileName, const char* szDstFileName);  // bitmap图片转换成jpg格式
public:
	BOOL m_bGetPicture;  // is get a picture
	long m_lWidth;       //存储图片的宽度
	long m_lHeight;		 //存储图片的长度
	int  m_iBitCount;    //the number of bits per pixel (bpp)
	TCHAR m_chTempPath[MAX_PATH];
	TCHAR m_chSwapStr[MAX_PATH];
	TCHAR m_chDirName[MAX_PATH];
};

#endif //__SAMPLEGRABBERCALLBACK_H__