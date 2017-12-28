#include "KrCore.h"
#include "KrButton.h"
namespace KrUI
{
	KrWindow::KrWindow()
	{
		m_pKrWindow = nullptr;
		m_UIType = KrUIType::KrWindow_t;
		m_bMouseDown = false;
	}

	LPCWSTR KrWindow::GetWindowName()
	{
		return m_lpName;
	}

	void KrWindow::SetWindowName(LPCWSTR lpWindowName)
	{
		if (IsCreated())SetWindowText(m_hwnd, lpWindowName);
		m_lpName = lpWindowName;
	}

	void KrWindow::SetHWND(HWND hwnd)
	{
		m_hwnd = hwnd;
	}

	HWND KrWindow::GetHWND()
	{
		return m_hwnd;
	}

	KrUIBase* KrWindow::AddControl(KrUIType t, LPCWSTR lpName, int x, int y, int width, int height)
	{
		KrUIBase* pui = nullptr;
		switch (t)
		{
		case KrUI::KrButton_t:
			pui = new KrButton(this);
			pui->SetSize(x, y, width, height);
			pui->SetName(lpName);
			m_UIVec.push_back(pui);
			break;
		}
		return pui;
	}

	void KrWindow::SetRect(RECT* pRect)
	{
		KrUIBase::SetRect(pRect);
		if (IsCreated())MoveWindow(m_hwnd, m_rect.left, m_rect.top, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, TRUE);

	}

	void KrWindow::SetStyle(DWORD dwStyle)
	{
		m_dwStyle = dwStyle;
		if (IsCreated())SetWindowLong(m_hwnd, GWL_STYLE, dwStyle);
	}

	DWORD KrWindow::GetStyle()
	{
		return m_dwStyle;
	}

	void KrWindow::SetX(UINT x)
	{
		KrUIBase::SetX(x);
		if (IsCreated())MoveWindow(m_hwnd, m_rect.left, m_rect.top, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, TRUE);

	}

	void KrWindow::SetY(UINT y)
	{
		KrUIBase::SetY(y);
		if (IsCreated())MoveWindow(m_hwnd, m_rect.left, m_rect.top, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, TRUE);

	}

	void KrWindow::SetXY(UINT x, UINT y)
	{
		KrUIBase::SetXY(x, y);
		if (IsCreated())MoveWindow(m_hwnd, m_rect.left, m_rect.top, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, TRUE);
	}

	void KrWindow::SetWidth(UINT width)
	{
		KrUIBase::SetWidth(width);
		if (IsCreated())MoveWindow(m_hwnd, m_rect.left, m_rect.top, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, TRUE);
	}

	void KrWindow::SetHeight(UINT height)
	{
		KrUIBase::SetHeight(height);
		if (IsCreated())MoveWindow(m_hwnd, m_rect.left, m_rect.top, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, TRUE);
	}

	void KrWindow::Show()
	{
		if (IsCreated())
		{
			ShowWindow(m_hwnd, SW_SHOW);
			KrUIBase::Show();
			UpdateWindow(m_hwnd);
		}
	}

	void KrWindow::Hide()
	{
		if (IsCreated())
		{
			ShowWindow(m_hwnd, SW_HIDE);
			KrUIBase::Hide();
		}
	}

	bool KrWindow::IsCreated()
	{
		if (m_hwnd == NULL)
		{
			return false;
		}
		return true;
	}

	LRESULT  KrWindow::HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam)
	{
		switch (Message)
		{
		case WM_DESTROY:
			//�����ڱ�����ʱ���������Ƿ���ڴ���
			KrUIManager::GetpKrUIManager()->DeleteWindow(this);
			break;
		case WM_MOVE:
		case WM_SIZE:
			GetWindowRect(m_hwnd, GetRect());
			break;
		}
		//���ô�����Ϣ��������
		CallMsgProc(Message, wParam, lParam);
		//������Ϣ���ؼ�
		for (auto p :m_UIVec)
		{
			dynamic_cast<KrMessageHandler*>(p)->HandleMessage(Message, wParam, lParam);
		}
		//
		KrUIBase::HandleMessage(Message, wParam, lParam);
		return DefWindowProc(m_hwnd, Message, wParam, lParam);
	}

	void KrWindow::UpdateDc()
	{
		cout << "timer" << endl;
	}

} //!KrUI