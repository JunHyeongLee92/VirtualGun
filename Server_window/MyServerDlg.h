
// MyServerDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "BugsLife.h"

#define MAX_COUNT    100

struct ClientSocket
{
	SOCKET h_socket;
	char ip_address[16];
};

class CMyServerDlg : public CDialogEx
{
private:
	SOCKET mh_socket;
	ClientSocket m_client_list[MAX_COUNT];
	BugsLife m_bugs;

	int m_check_time = 0;
	int m_start_flag = 0;
	float m_base_z_value = (float)0.0, m_base_x_value = (float)0.0;
	
	POINT m_cursor_pos1, m_cursor_pos2 ;
// 생성입니다.
public:
	CMyServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	void AddEvent(CString a_string);
	void SendFrameData(SOCKET ah_socket,
									 unsigned char a_msg_id,
									 const char *ap_data,
									 unsigned short int a_data_size);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MYSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT On25001(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On25002(WPARAM wParam, LPARAM lParam);
private:
	CListBox m_event_list;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
