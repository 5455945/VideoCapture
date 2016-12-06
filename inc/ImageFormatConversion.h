#pragma once

class CImageFormatConversion
{
public:
	CImageFormatConversion(void);
	virtual ~CImageFormatConversion(void);

	bool ToJpg(const char* szBmpFileName, const char* szJpgFileName, long iQuality);
	bool ToJpg(const wchar_t* szBmpFileName, const wchar_t* szJpgFileName, long iQuality);

	bool ToBmp(const char* szJpgFileName, const char* szBmpFileName);
	bool ToBmp(const wchar_t* szJpgFileName, const wchar_t* szBmpFileName);

	bool ToPng(const char* szBmpFileName, const char* szPngFileName);
	bool ToPng(const wchar_t* szBmpFileName, const wchar_t* szPngFileName);

	bool AddFont(const char* szSrcFileName, const char* szDstFileName, const char* szText, int iXPos, int iYPos);
	bool AddFont(const wchar_t* szSrcFileName, const wchar_t* szDstFileName, const wchar_t* szText, int iXPos, int iYPos);

private:
	wchar_t *BStr2WStr(const char* pSource, int &iDstLen);
};
