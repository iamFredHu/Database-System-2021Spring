#pragma once


// query 对话框

class query : public CDialogEx
{
	DECLARE_DYNAMIC(query)

public:
	query(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~query();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CListCtrl m_pListCtrl;
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_ename;
};
