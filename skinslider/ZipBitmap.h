#if !defined(AFX_ZIPBITMAP_H__13E1FC55_7116_4544_A74D_36486B7D3331__INCLUDED_)
#define AFX_ZIPBITMAP_H__13E1FC55_7116_4544_A74D_36486B7D3331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZipBitmap.h : header file
//

const iMarginWidth = 13;	// Possibly...
enum BGOptions {BG_CENTER = 1, BG_CENTER_ONTICK = 2, BG_STRETCH_HOR = 4, BG_STARTATICK = 8, 
				BG_SHOW_TOOLTIP = 16, BG_SHOW_TOOLTIP_TRACK = 32, BG_CENTER_ONTICK_VERT = 64, BG_STRETCH_VERT = 128, BG_STARTATICK_VERT = 256};

/////////////////////////////////////////////////////////////////////////////
// CZipBitmap window

class CZipBitmap : public ::CBitmap
{
public:
    CZipBitmap();
    ~CZipBitmap();

    int GetWidth();
    int GetHeight();

private:
    void GetMetrics();
    int m_iWidth;
    int m_iHeight;
};

class CZipBackGroundBitmap : public CZipBitmap
{
public:
	CZipBackGroundBitmap();
	~CZipBackGroundBitmap();

  void DrawBackGround(HDC hDC, const ::CRect& Area);
  void CreateBackBitmap(::CRect& Area, int uiBackGround, int Options, int TickWidth, int TickHeight);

	HDC GetDC() {return hDC;}

private:
	HDC hDC;
	HBITMAP hbmold;
};

class CZipTransBitmap : public CZipBitmap
{
public:
	CZipTransBitmap();
	~CZipTransBitmap();

	void DrawTrans(HDC hDC, int x, int y);
	void CreateMask(UINT uiTickImage, COLORREF colTrans = -1);

private:
    HBITMAP m_hbmMask;    // Handle to mask bitmap
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZIPBITMAP_H__13E1FC55_7116_4544_A74D_36486B7D3331__INCLUDED_)
