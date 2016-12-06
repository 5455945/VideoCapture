#include "ImageFormatConversion.h"
#include <objbase.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib") 

using namespace Gdiplus;

int GetCodecClsid(const wchar_t* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
	{
		return -1;
	}

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
	{
		return -1;
	}

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);

	return -1;
}

CImageFormatConversion::CImageFormatConversion(void)
{

}

CImageFormatConversion::~CImageFormatConversion(void)
{
}

wchar_t *CImageFormatConversion::BStr2WStr(const char* pSource, int &iDstLen)
{
	iDstLen = MultiByteToWideChar(0, 0, pSource, (int)strlen(pSource), NULL, 0);
	wchar_t *pWDst = new wchar_t[iDstLen + 1];
	memset(pWDst, 0, sizeof(wchar_t) * (iDstLen + 1));
	MultiByteToWideChar(0, 0, pSource, (int)strlen(pSource), pWDst, iDstLen);
	return pWDst;
}

bool CImageFormatConversion::ToJpg(const char *szSrcFileName, const char *szJpgFileName, long iQuality)
{
	bool bRet = false;
	if (szSrcFileName == NULL || szJpgFileName == NULL) {
		return bRet;
	}
	int iLen = 0;
	wchar_t *pWSrcFile = BStr2WStr(szSrcFileName, iLen);
	wchar_t *pWDstFile = BStr2WStr(szJpgFileName, iLen);

	bRet = ToJpg(pWSrcFile, pWDstFile, iQuality);

	return bRet;
}

bool CImageFormatConversion::ToBmp(const char* szSrcFileName, const char* szBmpFileName)
{
	bool bRet = false;
	if (szSrcFileName == NULL || szBmpFileName == NULL) {
		return bRet;
	}

	int iLen = 0;
	wchar_t* pWSrcFile = BStr2WStr(szSrcFileName, iLen);
	wchar_t* pWDstFile = BStr2WStr(szBmpFileName, iLen);

	bRet = ToBmp(pWSrcFile, pWDstFile);

	return bRet;
}

bool CImageFormatConversion::ToPng(const char* szSrcFileName, const char* szPngFileName)
{
	bool bRet = false;
	if (szSrcFileName == NULL || szPngFileName == NULL) {
		return bRet;
	}

	int iLen = 0;
	wchar_t* pWSrcFile = BStr2WStr(szSrcFileName, iLen);
	wchar_t* pWDstFile = BStr2WStr(szPngFileName, iLen);

	bRet = ToPng(pWSrcFile, pWDstFile);

	return bRet;
}

// iQuality:Í¼ÏñÖÊÁ¿ 0~100
bool CImageFormatConversion::ToJpg(const wchar_t* szSrcFileName, const wchar_t* szJpgFileName, long iQuality)
{
	bool bRet = false;
	if (szSrcFileName == NULL || szJpgFileName == NULL) {
		return bRet;
	}

	try
	{
		CLSID codecClsid;
		EncoderParameters encoderParameters;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		GetCodecClsid(L"image/jpeg", &codecClsid);

		encoderParameters.Count = 1;
		encoderParameters.Parameter[0].Guid = EncoderQuality;
		encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
		encoderParameters.Parameter[0].NumberOfValues = 1;
		encoderParameters.Parameter[0].Value = &iQuality;

		Image *pImage = new Image(szSrcFileName);
		Status bStatus = pImage->Save(szJpgFileName, &codecClsid, &encoderParameters);
		delete pImage;

		GdiplusShutdown(gdiplusToken);
		if (bStatus == Ok)
		{
			bRet = true;
		}
	}
	catch (...)
	{
	}
	return bRet;
}

bool CImageFormatConversion::ToBmp(const wchar_t* szSrcFileName, const wchar_t* szBmpFileName)
{
	bool bRet = false;
	if (szSrcFileName == NULL || szBmpFileName == NULL) {
		return bRet;
	}
	try
	{
		CLSID codecClsid;
		EncoderParameters encoderParameters;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		GetCodecClsid(L"image/bmp", &codecClsid);

		encoderParameters.Count = 0;

		Image *pImage = new Image(szSrcFileName);
		Status bStatus = pImage->Save(szBmpFileName, &codecClsid, &encoderParameters);
		delete pImage;

		GdiplusShutdown(gdiplusToken);
		if (bStatus == Ok)
		{
			bRet = true;
		}
	}
	catch (...)
	{
	}
	return bRet;
}
        
bool CImageFormatConversion::ToPng(const wchar_t* szSrcFileName, const wchar_t* szPngFileName)
{
	bool bRet = false;
	if (szSrcFileName == NULL || szPngFileName == NULL) {
		return bRet;
	}

	try {
		CLSID codecClsid;
		//EncoderParameters encoderParameters;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		Status bStatus;
		Image* pImage = NULL;
		GetCodecClsid(L"image/png", &codecClsid);
		pImage = new Image(szSrcFileName, TRUE);
		bStatus = pImage->Save(szPngFileName, &codecClsid, NULL);
		delete pImage;
		GdiplusShutdown(gdiplusToken);
		if (bStatus == Ok)
		{
			bRet = true;
		}
	}
	catch (...)
	{
	}

	return bRet;
}

bool CImageFormatConversion::AddFont(const char* szSrcFileName, const char* szDstFileName, const char* szText, int iXPos, int iYPos)
{
	bool bRet = false;
	if (szSrcFileName == NULL || szDstFileName == NULL || szText) {
		return bRet;
	}

	int iLen = 0;
	wchar_t *pWSrcFileName = BStr2WStr(szSrcFileName, iLen);
	wchar_t *pWDstFileName = BStr2WStr(szDstFileName, iLen);
	wchar_t *pWText = BStr2WStr(szText, iLen);

	bRet = AddFont(pWSrcFileName, pWDstFileName, pWText, iXPos, iYPos);

	return bRet;
}

bool CImageFormatConversion::AddFont(const wchar_t* szSrcFileName, const wchar_t* szDstFileName, const wchar_t* szText, int iXPos, int iYPos)
{
	bool bRet = false;
	if (szSrcFileName == NULL || szDstFileName == NULL || szText) {
		return bRet;
	}

	CLSID codecClsid;
	EncoderParameters encoderParameters;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Image *pImage = new Image(szSrcFileName);
	Graphics *pGraph = new Graphics(pImage);
	SolidBrush *pBrush = new SolidBrush(Color::Black);
	FontFamily fontFamily(L"ËÎÌå");
	Gdiplus::Font font(&fontFamily, 16, FontStyleRegular, UnitPoint);
	PointF fontPoint((REAL)iXPos, (REAL)iYPos);

	pGraph->DrawString(szText, (INT)wcslen(szText), &font, fontPoint, pBrush);

	GetCodecClsid(L"image/bmp", &codecClsid);
	encoderParameters.Count = 0;
	Status bStatus = pImage->Save(szDstFileName, &codecClsid, &encoderParameters);
	
	delete pBrush;
	delete pGraph;
	delete pImage;

	bRet = true;

	return bRet;
}
