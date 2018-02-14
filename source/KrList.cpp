#include "KrList.h"

namespace KrUI
{
	KrList::KrList()
	{
		m_Position = 0;
		m_TotalHeight = 0;
		m_MouseWheelDelta = 10;
		m_ScrollBarRect.Width = 20;
		m_SelectedItem = -1;
		m_MouseHoverItem = -1;
		m_MouseDownOnScrollBarPos = -1;
		m_FontColor = Gdiplus::Color::Black;
		m_StringFormat.SetAlignment(Gdiplus::StringAlignment::StringAlignmentNear);
		m_StringFormat.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
	}

	void KrList::SetPosition(int position)
	{
		if (position < 0)
		{
			m_Position = 0;
			return;
		}
		else if (position > static_cast<int>(m_TotalHeight - GetHeight()))
		{
			m_Position = m_TotalHeight - GetHeight();
			return;
		}
		m_Position = position;
	}

	KrListItem KrList::GetSelectedItem()
	{
		if (m_SelectedItem == -1)return KrListItem{ L"",0,0 };
		return m_ListItems[m_SelectedItem];
	}

	LRESULT KrList::HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam)
	{
		switch (Message)
		{
		case WM_LBUTTONDOWN:
			//�����갴��ʱ���б��ڣ��Ͱѵ�ǰѡ�е���Ŀ����Ϊ���ͣ���ڵ���һ��
			if (!(static_cast<int>(m_pKrWindow->m_ptMouse.x) >= static_cast<int>(GetX()) && static_cast<int>(m_pKrWindow->m_ptMouse.y) >= static_cast<int>(GetY()) &&
				static_cast<int>(m_pKrWindow->m_ptMouse.x) <= static_cast<int>(GetX() + GetWidth()) && static_cast<int>(m_pKrWindow->m_ptMouse.y) <= static_cast<int>(GetY() + GetHeight())))break;
			{
				m_SelectedItem = m_MouseHoverItem;
				if (m_SelectedItem != -1)CallMsgProc(KM_SELECTEDITEMCHANGE, wParam, lParam);
			}
			//�����갴��ʱ�ڹ������ϣ��ͱ������͹��������������λ��
			if (static_cast<int>(m_pKrWindow->m_ptMouse.x) >= static_cast<int>(GetX() + GetWidth() - m_ScrollBarRect.Width) &&
				static_cast<int>(m_pKrWindow->m_ptMouse.y) >= static_cast<int>(GetY() + m_ScrollBarRect.Y) &&
				static_cast<int>(m_pKrWindow->m_ptMouse.x) <= static_cast<int>(GetX() + GetWidth()) &&
				static_cast<int>(m_pKrWindow->m_ptMouse.y) <= static_cast<int>(GetY() + m_ScrollBarRect.GetBottom()))
				m_MouseDownOnScrollBarPos = m_pKrWindow->m_ptMouse.y - GetY() - m_ScrollBarRect.GetTop();
			break;
		case WM_MOUSEMOVE:
			//�����갴�ڹ�������ʱ���������λ�������б�����ʾλ��
			if (m_MouseDownOnScrollBarPos != -1)
			{
				//m_ScrollBarRect.Y = (GetHeight() - m_ScrollBarRect.Height)*static_cast<float>(m_Position) / (m_TotalHeight - GetHeight());
				//�����Ϸ��̵ã�
				SetPosition(static_cast<int>((m_pKrWindow->m_ptMouse.y - m_MouseDownOnScrollBarPos - GetY())*
					(static_cast<float>(m_TotalHeight) - GetHeight()) / (GetHeight() - m_ScrollBarRect.Height)));
			}
			break;
		case WM_MOUSEWHEEL:
			//���������ʱ���б������ڣ���������ʾλ�������ӻ����
			if (!(static_cast<int>(m_pKrWindow->m_ptMouse.x) >= static_cast<int>(GetX()) && static_cast<int>(m_pKrWindow->m_ptMouse.y) >= static_cast<int>(GetY()) &&
				static_cast<int>(m_pKrWindow->m_ptMouse.x) <= static_cast<int>(GetX() + GetWidth()) && static_cast<int>(m_pKrWindow->m_ptMouse.y) <= static_cast<int>(GetY() + GetHeight())))break;
			if (static_cast<int>(wParam) > 0)
			{
				if (m_Position > 0)SetPosition(m_Position - m_MouseWheelDelta);
			}
			else
			{
				if (m_TotalHeight - m_Position >= GetHeight())SetPosition(m_Position + m_MouseWheelDelta);
			}
			break;
		case WM_LBUTTONUP:
			m_MouseDownOnScrollBarPos = -1;
			break;
		}
		return KrUIBase::HandleMessage(Message, wParam, lParam);
	}

	int KrList::AddItem(std::wstring wStr, int nIndex /* = -1 */, unsigned int nHeight /* = 35*/)
	{
		if (nIndex == -1)
		{
			while (nIndex == -1)
			{
				nIndex = abs(rand());
				for (auto& li : m_ListItems)
				{
					if (li.m_Index == nIndex) nIndex = -1;
				}
			}
			m_ListItems.push_back(KrListItem(wStr, nIndex, nHeight));
			m_TotalHeight += nHeight;
			return nIndex;
		}

		for (auto& li : m_ListItems)
		{
			if (li.m_Index == nIndex)return -1;
		}
		m_ListItems.push_back(KrListItem(wStr, nIndex, nHeight));
		m_TotalHeight += nHeight;
		return nIndex;
	}

	int KrList::RemoveItem(std::wstring wStr)
	{
		int ret = 0;
		auto it = m_ListItems.begin();
		while (it != m_ListItems.end())
		{
			if (it->m_ItemName == wStr)
			{
				m_TotalHeight -= it->m_Height;
				it = m_ListItems.erase(it);
				if (m_Position > m_TotalHeight - GetHeight())SetPosition(m_TotalHeight - GetHeight());
				++ret;
			}
			else
			{
				++it;
			}
		}
		return ret;
	}

	bool KrList::RemoveItem(int nIndex)
	{
		auto it = m_ListItems.begin();
		while (it != m_ListItems.end())
		{
			if (it->m_Index == nIndex)
			{
				m_TotalHeight -= it->m_Height;
				it = m_ListItems.erase(it);
				if (m_Position > m_TotalHeight - GetHeight())SetPosition(m_TotalHeight - GetHeight());
				return true;
			}
			else
			{
				++it;
			}
		}
		return false;
	}

	void KrList::Update()
	{
		m_pGraphics->FillRectangle(&Gdiplus::SolidBrush(m_BgColor), 0, 0, GetWidth(), GetHeight());
		if (m_ListItems.size() != 0)
		{
			//������Ҫ����Щ�
			//m_Position = 0;
			int current_total = 0, start_item = -1, end_item = -1, start_position = 0;
			m_MouseHoverItem = -1;
			for (unsigned int i = 0; i < m_ListItems.size(); i++)
			{
				//ȷ����ʼ��
				if (static_cast<int>(m_Position) >= current_total && m_Position <= (current_total + m_ListItems[i].m_Height))
				{
					start_item = i;
					start_position = current_total - m_Position;
				}
				//ȷ�����ͣ����
				if (m_pKrWindow->m_ptMouse.x >= static_cast<int>(GetX()) && m_pKrWindow->m_ptMouse.y >= static_cast<int>(GetY()) &&
					m_pKrWindow->m_ptMouse.x <= static_cast<int>(GetX() + GetWidth()) && m_pKrWindow->m_ptMouse.y <= static_cast<int>(GetY() + GetHeight()) &&
					static_cast<int>(m_pKrWindow->m_ptMouse.y - GetY()) >= static_cast<int>(current_total - m_Position) &&
					static_cast<int>(m_pKrWindow->m_ptMouse.y - GetY()) <= static_cast<int>(current_total - m_Position + m_ListItems[i].m_Height) &&
					static_cast<int>(m_pKrWindow->m_ptMouse.x - GetX()) <= static_cast<int>(GetWidth() - m_ScrollBarRect.Width))
					m_MouseHoverItem = i;
				//ȷ��������
				int end_position = m_Position + GetHeight();
				if (end_position >= current_total && end_position <= static_cast<int>(current_total + m_ListItems[i].m_Height))
				{
					end_item = i;
					break;//�ҵ�������ֱ���˳�ѭ��
				}
				current_total += m_ListItems[i].m_Height;
			}
			//���δ�ҵ����һ��ӿ�ʼλ�û���ĩβ
			if (end_item = -1)end_item = m_ListItems.size() - 1;
			for (int i = start_item; i <= end_item; i++)
			{
				//�����һ������ͣ������ѡ��״̬ʱ��������ɫ����
				if (m_MouseHoverItem == i || m_SelectedItem == i)m_pGraphics->FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(196, 218, 242)), 0, start_position, GetWidth() - m_ScrollBarRect.Width, m_ListItems[i].m_Height);
				//����������
				m_pGraphics->DrawString(m_ListItems[i].m_ItemName.c_str(), -1, m_pFont, Gdiplus::RectF(static_cast<Gdiplus::REAL>(10),
					static_cast<Gdiplus::REAL>(start_position), static_cast<Gdiplus::REAL>(GetWidth() - m_ScrollBarRect.Width), static_cast<Gdiplus::REAL>(m_ListItems[i].m_Height)),
					&m_StringFormat, &Gdiplus::SolidBrush(m_FontColor));
				//���߿�
				m_pGraphics->DrawRectangle(&Gdiplus::Pen(m_BorderColor), 0, start_position, GetWidth() - m_ScrollBarRect.Width, m_ListItems[i].m_Height);
				//��һ�λ���λ������
				start_position += m_ListItems[i].m_Height;
			}
		}
		//���б���͹������ֽ���
		m_pGraphics->DrawLine(&Gdiplus::Pen(m_BorderColor, 2), Gdiplus::Point(GetWidth() - m_ScrollBarRect.Width, 0), Gdiplus::Point(GetWidth() - m_ScrollBarRect.Width, GetHeight()));
		//������ʾλ�����ù������ľ���
		if (m_TotalHeight > GetHeight())
		{
			m_ScrollBarRect.Height = static_cast<int>(GetHeight()*static_cast<float>(GetHeight()) / m_TotalHeight);
			m_ScrollBarRect.X = GetWidth() - m_ScrollBarRect.Width + 1;
			m_ScrollBarRect.Y = static_cast<int>((GetHeight() - m_ScrollBarRect.Height)*static_cast<float>(m_Position) / (m_TotalHeight - GetHeight()));
		}
		else
		{
			m_ScrollBarRect.Height = GetHeight();
			m_ScrollBarRect.X = GetWidth() - m_ScrollBarRect.Width + 1;
			m_ScrollBarRect.Y = 0;
		}
		//��������
		m_pGraphics->FillRectangle(
			//���ͣ���ڹ������ϻ��������ڰ���״̬ʱ�������ɫ������ǳ��ɫ
			((static_cast<int>(m_pKrWindow->m_ptMouse.x) >= static_cast<int>(GetX() + GetWidth() - m_ScrollBarRect.Width) &&
				static_cast<int>(m_pKrWindow->m_ptMouse.y) >= static_cast<int>(GetY() + m_ScrollBarRect.Y) &&
				static_cast<int>(m_pKrWindow->m_ptMouse.x) <= static_cast<int>(GetX() + GetWidth()) &&
				static_cast<int>(m_pKrWindow->m_ptMouse.y) <= static_cast<int>(GetY() + m_ScrollBarRect.GetBottom())) || m_MouseDownOnScrollBarPos != -1) ?
			&Gdiplus::SolidBrush(Gdiplus::Color(150, 150, 150)) :
			&Gdiplus::SolidBrush(Gdiplus::Color(200, 200, 200)), m_ScrollBarRect);
		//���б��߿�
		m_pGraphics->DrawRectangle(&Gdiplus::Pen(m_BorderColor, 2), 1, 1, GetWidth() - 2, GetHeight() - 2);
		KrUIBase::Update();
	}
}