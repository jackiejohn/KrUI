#ifndef _KR_UIBASE_H
#define _KR_UIBASE_H


#include "KrMessageHandler.h"
#include "KrDefine.h"
#include <windows.h>
#include <gdiplus.h>
#include <string>
//using namespace Gdiplus;
namespace KrUI
{
	class KrWindow;
	enum KrUIType { KrWindow_t, KrButton_t, KrCloseButton_t, KrLabel_t, KrEdit_t, KrProgressBar_t, KrList_t, KrRadio_t, KrCheckBox_t, KrScrollBar_t };
	enum Mouse_Status { mouse_down, mouse_leave, mouse_hover };
	class KrUIBase :public KrMessageHandler
	{
	protected:
		std::wstring m_strName;
		RECT m_rect;
		bool m_bVisible;
		Gdiplus::Bitmap* m_pBmp;
		Gdiplus::Graphics* m_pGraphics;
		KrUIType m_UIType;
		KrWindow* m_pKrWindow;
		HBITMAP m_hBmp;
		Gdiplus::StringFormat m_StringFormat;
		Gdiplus::Font* m_pFont;
		Gdiplus::Color m_FontColor;
		Gdiplus::Color m_BorderColor;
		Gdiplus::REAL m_BorderWidth;
		Gdiplus::Color m_BgColor;
		bool m_bMouseIn;
		bool m_bMouseDown;
		HCURSOR m_hCursor;
	public:
		friend class KrWindow;
		KrUIBase();
		virtual ~KrUIBase();
		void SetType(KrUIType ut);
		KrUIType GetType()const;
		virtual RECT* GetRect();
		virtual void SetRect(RECT* pRect);
		virtual unsigned int GetX()const;
		virtual unsigned int GetY()const;
		virtual unsigned int GetWidth()const;
		virtual unsigned int GetHeight()const;
		virtual void SetX(unsigned int x);
		virtual void SetY(unsigned int y);
		virtual void SetXY(unsigned int x, unsigned int y);
		virtual void SetWidth(unsigned int width);
		virtual void SetHeight(unsigned int height);
		virtual void SetSize(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
		virtual void Show();
		virtual void Hide();
		virtual bool IsVisible();
		virtual void SetDc(HDC hdc);
		virtual void SetParantWindow(KrWindow* pKrWindow);
		KrWindow* GetParantWindow();
		void SetFontColor(Gdiplus::Color color);
		Gdiplus::Color GetFontColor();
		Gdiplus::Color GetBgColor();
		void SetBgColor(Gdiplus::Color c);
		void SetBorderColor(Gdiplus::Color color);
		Gdiplus::Color GetBorderColor() const;
		void SetBorderWidth(Gdiplus::REAL width);
		Gdiplus::REAL GetBorderWidth();
		virtual void Update();
		virtual void ChangeBmpSize();
		Gdiplus::Graphics* GetBmpGraphics();
		virtual void CallMsgProc(UINT Message, WPARAM wParam, LPARAM lParam);
		virtual void SetName(std::wstring name);
		std::wstring GetName();
		virtual LRESULT HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam);
		void SetFont(const WCHAR* fontfamily, Gdiplus::REAL emSize);
		Gdiplus::Font* GetFont();
		void SetAlignment(Gdiplus::StringAlignment align);
		void SetLineAlignment(Gdiplus::StringAlignment align);
	};

}//!KrUI

#endif

