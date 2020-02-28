
// PVZCheaterDlg.h: 头文件
//

#pragma once


// CPVZCheaterDlg 对话框
class CPVZCheaterDlg : public CDialogEx
{
// 构造	
public:
	CPVZCheaterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PVZCHEATER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持	

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLink();
	afx_msg void OnBnClickedKill();
	afx_msg void OnBnClickedSun();
	afx_msg void OnBnClickedNocd();
	afx_msg void OnBnClickedPlantnodie();
	afx_msg void OnBnClickedBackstage();
	afx_msg void OnBnClickedZombienodie();
	afx_msg void OnBnClickedMoney();
	afx_msg void OnBnClickedCancel();
private:
	friend DWORD monitorThreadFunc(LPVOID); // 使得监控游戏的线程方法中可以访问
	// 秒杀僵尸
	CButton m_kill;
	// 无限阳光
	CButton m_sun;
	// 无CD
	CButton m_noCd;
	// 植物不死
	CButton m_pnd;
	// 后台运行
	CButton m_backstage;
	// 僵尸不死
	CButton m_znd;

	// 子线程句柄
	HANDLE m_monitorThread;
};
