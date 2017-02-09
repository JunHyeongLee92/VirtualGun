
// MyServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MyServer.h"
#include "MyServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyServerDlg ��ȭ ����



CMyServerDlg::CMyServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for(int i = 0; i < MAX_COUNT; i++) {
		m_client_list[i].h_socket = INVALID_SOCKET;
	}
	
	m_cursor_pos1.x = 0;
	m_cursor_pos1.y = 0;

	m_cursor_pos2.x = 0;
	m_cursor_pos2.y = 0;
}

void CMyServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_LIST, m_event_list);
}

BEGIN_MESSAGE_MAP(CMyServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(25001, &CMyServerDlg::On25001)
	ON_MESSAGE(25002, &CMyServerDlg::On25002)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CMyServerDlg �޽��� ó����

BOOL CMyServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	//���� ������ ���ῡ ���� �κ�
	mh_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = INADDR_ANY;
		//inet_addr("192.168.0.3");
	srv_addr.sin_port = htons(24670);

	bind(mh_socket, (LPSOCKADDR)&srv_addr, sizeof(srv_addr));

	listen(mh_socket, 1);

	WSAAsyncSelect(mh_socket, m_hWnd, 25001, FD_ACCEPT);

	AddEvent("���� ���񽺸� �����մϴ�.");

	//ShowWindow(SW_SHOWMAXIMIZED);

	CRect r;
	GetClientRect(r);

	m_bugs.SetArea(r.Width(), r.Height());

	SetTimer(1, 30, NULL);
	SetTimer(2, 1000, NULL);
	SetTimer(3, 3000, NULL);
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMyServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

		CPen line_pen;
		CBrush ellipse_brush;
		Bug *p_pos;
		int count = m_bugs.GetCount(), bug_size;
		const T_COLOR *p_color;

		for (int i = 0; i < count; i++) {
			p_pos = m_bugs.GetBug(i);
			p_color = p_pos->GetColor();
			line_pen.CreatePen(PS_SOLID, 1, RGB(p_color->red, p_color->green, p_color->blue));
			ellipse_brush.CreateSolidBrush(RGB(p_color->red, p_color->green, p_color->blue));

			CPen *p_old_pen = dc.SelectObject(&line_pen);
			CBrush *p_old_brush = dc.SelectObject(&ellipse_brush);

			dc.MoveTo(p_pos->GetStartPos()->x, p_pos->GetStartPos()->y);
			dc.LineTo(p_pos->GetEndPos()->x, p_pos->GetEndPos()->y);

			bug_size = p_pos->GetSize() / 2;
			dc.Ellipse(p_pos->GetPosX() - bug_size, p_pos->GetPosY() - bug_size,
				p_pos->GetPosX() + bug_size, p_pos->GetPosY() + bug_size);

			dc.SelectObject(p_old_pen);
			dc.SelectObject(p_old_brush);

			line_pen.DeleteObject();
			ellipse_brush.DeleteObject();
			p_pos++;
		}

		CString str;
		str.Format("Score = %d", m_bugs.GetScore());
		dc.TextOut(0, 0, str);

		str.Format("Life = %d", m_bugs.GetLife());  //������ ���
		dc.TextOut(0, 25, str);

		str.Format("Boom = %d", m_bugs.GetItem());  //������ ���
		dc.TextOut(0, 50, str);

		if (m_bugs.GetLife() <= 0) {  //����â ��� �� CLOSE �޼���

			SendMessage(WM_CLOSE, 0, 0);
		}

		line_pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		CPen *p_old_pen = dc.SelectObject(&line_pen);

		//������ �׸��� �̵���� ������ �׷��ݴϴ�.
		dc.MoveTo(m_cursor_pos1.x, m_cursor_pos1.y - 100);
		dc.LineTo(m_cursor_pos1.x, m_cursor_pos1.y + 100);
		dc.MoveTo(m_cursor_pos1.x - 100, m_cursor_pos1.y);
		dc.LineTo(m_cursor_pos1.x + 100, m_cursor_pos1.y);
		dc.SelectObject(p_old_pen);
		line_pen.DeleteObject();

		line_pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
		p_old_pen = dc.SelectObject(&line_pen);

		dc.MoveTo(m_cursor_pos2.x, m_cursor_pos2.y - 100);
		dc.LineTo(m_cursor_pos2.x, m_cursor_pos2.y + 100);
		dc.MoveTo(m_cursor_pos2.x - 100, m_cursor_pos2.y);
		dc.LineTo(m_cursor_pos2.x + 100, m_cursor_pos2.y);

		dc.SelectObject(p_old_pen);
		line_pen.DeleteObject();

		
		//CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMyServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//Ŭ���̾�Ʈ�� ��������� �߻��ϴ� �޼���
afx_msg LRESULT CMyServerDlg::On25001(WPARAM wParam, LPARAM lParam)
{
	SOCKET h_socket = (SOCKET)wParam;

	sockaddr_in client_addr;
	int sockaddr_in_size = sizeof(sockaddr_in), i;

	for(i = 0; i < MAX_COUNT; i++) {
		if(m_client_list[i].h_socket == INVALID_SOCKET) break;
	}
	
	if(i < MAX_COUNT) {
		m_client_list[i].h_socket = accept(h_socket,
									 (LPSOCKADDR)&client_addr,
									 &sockaddr_in_size);
		strcpy(m_client_list[i].ip_address, 
					inet_ntoa(client_addr.sin_addr));

		CString str = "���ο� ����ڰ� �����߽��ϴ� : ";
		str += m_client_list[i].ip_address;
		AddEvent(str);

		WSAAsyncSelect(m_client_list[i].h_socket,
					   m_hWnd, 25002, FD_READ | FD_CLOSE);

	} else {
		closesocket(h_socket);
	}



	return 0;
}

void CMyServerDlg::AddEvent(CString a_string)
{
	while(m_event_list.GetCount() > 1000) {
		m_event_list.DeleteString(0);
	}

	int index = m_event_list.InsertString(-1, a_string);
	m_event_list.SetCurSel(index);
}

//Ŭ���̾�Ʈ�� �����͸� �ְ������ ������ȭ ���ִ� �Լ�
void CMyServerDlg::SendFrameData(SOCKET ah_socket,
								 unsigned char a_msg_id,
								 const char *ap_data,
								 unsigned short int a_data_size)
{
	int send_data_size = a_data_size + 4;
	char *p_send_data = new char[send_data_size];

	*p_send_data = 27;
	*(p_send_data + 1) = a_msg_id;
	*(unsigned short int *)(p_send_data + 2) = a_data_size;
	memcpy(p_send_data + 4, ap_data, a_data_size);
	send(ah_socket, p_send_data, send_data_size, 0);
	delete[] p_send_data;
}

//Ŭ���̾�Ʈ�κ��� ���� ���� �� �� �޼��������� ������ ���ǹ�
afx_msg LRESULT CMyServerDlg::On25002(WPARAM wParam, LPARAM lParam)
{
	ClientSocket *p_user;

	for(int i = 0; i < MAX_COUNT; i++) {
		if(m_client_list[i].h_socket == wParam) {
			p_user = &m_client_list[i];
			break;
		}
	}

	if(WSAGETSELECTEVENT(lParam) == FD_READ) {
		WSAAsyncSelect(p_user->h_socket,
							   m_hWnd, 25002, FD_CLOSE);
		unsigned char check, msg_id;
		unsigned short int body_size;

		recv(p_user->h_socket, (char *)&check, 1, 0);
		if(check == 27) {
			
			recv(p_user->h_socket, (char *)&msg_id, 1, 0);
			recv(p_user->h_socket, (char *)&body_size, 2, 0);

			if(body_size > 0) {
				char *p_body_data = new char[body_size];
				recv(p_user->h_socket, p_body_data, body_size, 0);
				if(msg_id == 1) {
					/*ó�� Ŭ���̾�Ʈ�� ���ӵɶ� �߻��ϴ� �޼�����
					Ŭ���̾�Ʈ ������ ��ȭâ�� ���*/
					CString str;
					str = p_user->ip_address;
					str += " : ";
					str += p_body_data;

					AddEvent(str);

					for(int i = 0; i < MAX_COUNT; i++) {
						if(m_client_list[i].h_socket != INVALID_SOCKET) {
							SendFrameData(m_client_list[i].h_socket,
											  1, str, str.GetLength() + 1);
						}
					}
				}
				else  if (msg_id == 2) {
					switch (*(int*)p_body_data) {
					case 0:
						keybd_event(VK_F5, 0, 0, 0);
						keybd_event(VK_F5, 0, KEYEVENTF_KEYUP, 0);

						break;

					case 1:
						keybd_event(VK_ESCAPE, 0, 0, 0);
						keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);

						break;

					case 2:
						keybd_event(VK_LEFT, 0, 0, 0);
						keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
						break;

					case 3:
						keybd_event(VK_RIGHT, 0, 0, 0);
						keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
						break;

					case 4:
						if(m_bugs.GetItem() != 0)	m_bugs.Boom();
						break;
					
					default:
						if (p_user == &m_client_list[0]) {
							PostMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(m_cursor_pos1.x, m_cursor_pos1.y));
						}
						else if (p_user == &m_client_list[1]) {
							PostMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(m_cursor_pos2.x, m_cursor_pos2.y));
						}
						break;
					}
				}

				else if (msg_id == 22)
				{
					/*Ŭ���̾�Ʈ�� �������� ������ 
					ȭ�鿡 �ش� ��ǥ�� Ŀ���� ���*/

					float *p_sensor_data = (float *)p_body_data;
					//CString str;
					//str.Format("%8.4f %8.4f %8.4f", p_sensor_data[0], p_sensor_data[1], p_sensor_data[2]);

					if (m_start_flag) {
						float z_value = p_sensor_data[0] - m_base_z_value;
						if (z_value > 180.0f) z_value -= 360.0f;
						if (z_value < -180.0f) z_value += 360.0f;

						float x_value = p_sensor_data[1] - m_base_x_value;
						if (x_value > 180.0f) x_value -= 360.0f;
						if (x_value < -180.0f) x_value += 360.0f;

						int center_x_pos = 683, center_y_pos = 384;
						if (p_user == &m_client_list[0]) {
							m_cursor_pos1.x = center_x_pos + center_x_pos * z_value / 30;
							m_cursor_pos1.y = center_y_pos + center_y_pos * x_value / 15;

							ScreenToClient(&m_cursor_pos1);
						}

						else if (p_user == &m_client_list[1]) {
							m_cursor_pos2.x = center_x_pos + center_x_pos * z_value / 30;
							m_cursor_pos2.y = center_y_pos + center_y_pos * x_value / 15;

							ScreenToClient(&m_cursor_pos2);
						}
						//SetCursorPos(cursor_pos_x, cursor_pos_y);
					}

					else {
						if (m_check_time == 0) {
							m_check_time = GetTickCount();
						}
						else if (GetTickCount() - m_check_time >2000) {
							m_base_z_value = p_sensor_data[0];
							m_base_x_value = p_sensor_data[1];

							m_start_flag = 1;
						}
					}
				}
				delete[] p_body_data;
			}

			WSAAsyncSelect(p_user->h_socket, m_hWnd, 25002, FD_READ | FD_CLOSE);
		}
	} else {
		closesocket(p_user->h_socket); // m_client_list[i].h_socket
		p_user->h_socket = INVALID_SOCKET;
	}

	return 0;
}


void CMyServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		m_bugs.Move();
		Invalidate();
	}
	else if (nIDEvent == 2)
	{
		if (0 == m_bugs.Add()) KillTimer(2);
	}
	else if (nIDEvent == 3)
	{
		m_bugs.Turn();
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

//���콺�� ������ ���� Ŭ���Ҷ� �߻��ϴ� �Լ�
void CMyServerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// if (m_bugs.GetItem() != 0)	m_bugs.Boom();
	m_bugs.Check(point.x, point.y);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMyServerDlg::OnDestroy()
{

	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	CString buf;
	buf.Format("Score = %d", m_bugs.GetScore());
	AfxMessageBox(buf);

	CDialogEx::OnDestroy();
}
