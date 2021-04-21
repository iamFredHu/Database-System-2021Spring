// query.cpp: 实现文件
//

#include "stdafx.h"
#include "testMySQL2.h"
#include "query.h"
#include "afxdialogex.h"
#include "mysql.h"


// query 对话框

IMPLEMENT_DYNAMIC(query, CDialogEx)

query::query(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_ename(_T(""))
{

}

query::~query()
{
}

void query::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_pListCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_ename);
}


BEGIN_MESSAGE_MAP(query, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &query::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &query::OnLvnItemchangedList3)
END_MESSAGE_MAP()


// query 消息处理程序


void query::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	MYSQL m_sqlCon;
	//初始化数据库对象  
	mysql_init(&m_sqlCon);

	//服务器地址：localhost; 账号、密码：root、MySQL1234;
	//数据库名:company; 端口号：3306;    
	if (!mysql_real_connect(&m_sqlCon, "localhost", "root",
		"MySQL1234", "company", 3306, NULL, 0))
	{
		AfxMessageBox(_T("数据库连接失败!"));
		return;
	}
	else
	{
		UpdateData(true);								//用输入框的值更新m_ename变量		
		mysql_query(&m_sqlCon, "set names 'gb2312'");	//设置数据库字符格式，解决中文乱码问题  
				
		char sql[100];        
		sprintf_s(sql, "select * from employee ");
		if(strlen(m_ename) != 0)
		{ 
			sprintf_s(sql, "select * from employee where ename = \'%s\' ", m_ename);
		}
		AfxMessageBox(_T(sql));
		
		mysql_query(&m_sqlCon, sql);
		MYSQL_RES *result = mysql_store_result(&m_sqlCon);

		CRect rect;
		m_pListCtrl.DeleteAllItems();		//删除原来的数据行，重新初始化列表
		while (m_pListCtrl.DeleteColumn(0));//删除原来的列，重新初始化列表
		m_pListCtrl.GetClientRect(&rect);

		// 为列表视图控件添加6列   
		m_pListCtrl.InsertColumn(0, _T("员工姓名"), LVCFMT_CENTER, rect.Width() / 6, 0);
		m_pListCtrl.InsertColumn(1, _T("员工编号"), LVCFMT_CENTER, rect.Width() / 6, 1);
		m_pListCtrl.InsertColumn(2, _T("员工住址"), LVCFMT_CENTER, rect.Width() / 6, 2);
		m_pListCtrl.InsertColumn(3, _T("员工薪水"), LVCFMT_CENTER, rect.Width() / 6, 3);
		m_pListCtrl.InsertColumn(4, _T("领导编号"), LVCFMT_CENTER, rect.Width() / 6, 4);
		m_pListCtrl.InsertColumn(5, _T("部门编号"), LVCFMT_CENTER, rect.Width() / 6, 5);

		int i = 0;
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(result)))
		{    
			int nRow = m_pListCtrl.InsertItem(i, row[0]);	//插入行                 
			m_pListCtrl.SetItemText(nRow, 1, row[1]);	//设置数据                
			m_pListCtrl.SetItemText(nRow, 2, row[2]);	//设置数据                
			m_pListCtrl.SetItemText(nRow, 3, row[3]);	//设置数据                
			m_pListCtrl.SetItemText(nRow, 4, row[4]);	//设置数据     
			m_pListCtrl.SetItemText(nRow, 5, row[5]);	//设置数据                
			  
			i++;
		}       
		mysql_free_result(result);	//释放内存空间  

	}
	mysql_close(&m_sqlCon);			//关闭Mysql连接  
}


void query::OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
