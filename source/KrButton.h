#ifndef KRBUTTON_H
#define  KRBUTTON_H
#pragma once
#include "KrUIBase.h"
#include "KrMessageHandler.h"
namespace KrUI
{
	class KrWindow;
	enum Button_Status{mouse_down,mouse_leave,mouse_hover};
	class KrButton :public KrUIBase
	{
	public:
		KrButton();
		~KrButton();
		void SetMouseHoverColor(Color color);
		void SetMouseDownColor(Color color);
		void SetMouseLeaveColor(Color color);
		void SetBorderColor(Color color);
		Color GetMouseHoverColor();
		Color GetMouseDownColor();
		Color GetMouseLeaveColor();
		Color GetBorderColor();
		virtual void DrawMouseDownBmp();
		virtual void DrawMouseHoverBmp();
		virtual void DrawMouseLeaveBmp();
		virtual void DrawContent();
		void UpdateDc();
		LRESULT HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam);
	protected:
		Color m_MouseHoverColor;
		Color m_MouseDownColor;
		Color m_MouseLeaveColor;
		Color m_BorderColor;
		Button_Status m_ButtonStatus;
	};



	//以下是特殊按钮：
	class KrCloseButton :public KrButton
	{
	public:
		friend KrWindow;
		virtual void DrawContent();
		virtual void UpdateDc();
		KrCloseButton();
	protected:
		UINT m_Margin;
	};


}//!KrUI

#endif

