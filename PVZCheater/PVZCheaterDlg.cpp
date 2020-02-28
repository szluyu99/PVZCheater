
// PVZCheaterDlg.cpp: 实现文件

#include "pch.h"
#include "framework.h"
#include "PVZCheater.h"
#include "PVZCheaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CPVZCheaterDlg * g_dlg;
static HANDLE g_processHandle;

// 将某个值写入植物大战僵尸内存（后面的可变参数是地址链，要以-1结尾）
void WriteMemory(void *value, DWORD valueSize, ...) {
	if (value == NULL || valueSize == 0 || g_processHandle == NULL) return;

	DWORD tempValue = 0;

	va_list addresses;
	va_start(addresses, valueSize);
	DWORD offset = 0;
	DWORD lastAddress = 0;
	while ((offset = va_arg(addresses, DWORD)) != -1) {
		lastAddress = tempValue + offset;
		::ReadProcessMemory(g_processHandle, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);
	}
	va_end(addresses);

	::WriteProcessMemory(g_processHandle, (LPVOID)lastAddress, value, valueSize, NULL);
}

void WriteMemory(void *value, DWORD valueSize, DWORD address) {
	WriteMemory(value, valueSize, address, -1);
}

// 用来监控游戏的线程
DWORD monitorThreadFunc(LPVOID lpThreadParameter) {
	while (1) {
	
		// 获得植物大战僵尸的句柄
		HWND windowHandle = FindWindow(CString("MainWindow"), CString("植物大战僵尸中文版"));
		
		if (windowHandle == NULL) { // 程序如果没有运行
			g_dlg->m_kill.SetCheck(FALSE);
			g_dlg->m_kill.EnableWindow(FALSE);
			g_dlg->m_sun.SetCheck(FALSE);
			g_dlg->m_sun.EnableWindow(FALSE);
			g_dlg->m_noCd.SetCheck(FALSE);
			g_dlg->m_noCd.EnableWindow(FALSE);
			g_dlg->m_pnd.SetCheck(FALSE);
			g_dlg->m_pnd.EnableWindow(FALSE);
			g_dlg->m_backstage.SetCheck(FALSE);
			g_dlg->m_backstage.EnableWindow(FALSE);
			g_dlg->m_znd.SetCheck(FALSE);
			g_dlg->m_znd.EnableWindow(FALSE);

			g_processHandle = NULL;
		}else if (g_processHandle == NULL) {
			g_dlg->m_kill.EnableWindow(TRUE);
			g_dlg->m_sun.EnableWindow(TRUE);
			g_dlg->m_noCd.EnableWindow(TRUE);
			g_dlg->m_pnd.EnableWindow(TRUE);
			g_dlg->m_backstage.EnableWindow(TRUE);
			g_dlg->m_znd.EnableWindow(TRUE);

			// 获得植物大战僵尸的进程ID
			DWORD processPid;
			GetWindowThreadProcessId(windowHandle, &processPid);
			// 获得植物大战僵尸的进程句柄
			g_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processPid);
		}
		// 睡眠1s
		Sleep(1000);

	}
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPVZCheaterDlg 对话框


CPVZCheaterDlg::CPVZCheaterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PVZCHEATER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPVZCheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KILL, m_kill);
	DDX_Control(pDX, IDC_SUN, m_sun);
	DDX_Control(pDX, IDC_NOCD, m_noCd);
	DDX_Control(pDX, IDC_PlantNoDie, m_pnd);
	DDX_Control(pDX, IDC_BACKSTAGE, m_backstage);
	DDX_Control(pDX, IDC_ZombieNoDie, m_znd);
}

BEGIN_MESSAGE_MAP(CPVZCheaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LINK, &CPVZCheaterDlg::OnBnClickedLink)
	ON_BN_CLICKED(IDC_KILL, &CPVZCheaterDlg::OnBnClickedKill)
	ON_BN_CLICKED(IDC_SUN, &CPVZCheaterDlg::OnBnClickedSun)
	ON_BN_CLICKED(IDC_NOCD, &CPVZCheaterDlg::OnBnClickedNocd)
	ON_BN_CLICKED(IDC_PlantNoDie, &CPVZCheaterDlg::OnBnClickedPlantnodie)
	ON_BN_CLICKED(IDC_BACKSTAGE, &CPVZCheaterDlg::OnBnClickedBackstage)
	ON_BN_CLICKED(IDC_ZombieNoDie, &CPVZCheaterDlg::OnBnClickedZombienodie)
	ON_BN_CLICKED(IDC_MONEY, &CPVZCheaterDlg::OnBnClickedMoney)
	ON_BN_CLICKED(IDC_CANCEL, &CPVZCheaterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPVZCheaterDlg 消息处理程序

BOOL CPVZCheaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 创建一条子线程，监控游戏的打开或者关闭
	m_monitorThread = CreateThread(NULL, NULL, monitorThreadFunc, NULL, NULL, NULL);
	// 保存对话框
	g_dlg = this;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPVZCheaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPVZCheaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPVZCheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 宏定义log 
#define log(fmt, ...)\
CString str;\
str.Format(CString(fmt), __VA_ARGS__);\
AfxMessageBox(str);	

/****************************按钮**************************/
// 跳转页面
void CPVZCheaterDlg::OnBnClickedLink() {
	ShellExecute(
		NULL,
		CString("open"),
		CString("http://hlzy.xyz/"),
		NULL,
		NULL,
		SW_NORMAL);
}
// 无限金币
void CPVZCheaterDlg::OnBnClickedMoney() {
	/*DWORD value = 99990;
	WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x82C, 0x28, -1);*/
	log("未完成");
}
// 一键取消
void CPVZCheaterDlg::OnBnClickedCancel() {
	if (m_kill.GetCheck()) {
		g_dlg->m_kill.SetCheck(FALSE);
	}
	if (m_sun.GetCheck()) {
		g_dlg->m_sun.SetCheck(FALSE);
	}
	if (m_noCd.GetCheck()) {
		g_dlg->m_noCd.SetCheck(FALSE);
	}
	if (m_pnd.GetCheck()) {
		g_dlg->m_pnd.SetCheck(FALSE);
	}
	if (m_backstage.GetCheck()) {
		g_dlg->m_backstage.SetCheck(FALSE);
	}
	if (m_znd.GetCheck()) {
		g_dlg->m_znd.SetCheck(FALSE);
	}
}
/****************************按钮**************************/

// 无限阳光
void CPVZCheaterDlg::OnBnClickedSun() {
	log("未完成");
}

// 秒杀僵尸
void CPVZCheaterDlg::OnBnClickedKill() {
	/*
	00531319 - 89 BD C8000000 -		mov[ebp + 000000C8], edi
	x*/
	if (m_znd.GetCheck()) { // 秒杀僵尸与僵尸不死不可兼得
		g_dlg->m_znd.SetCheck(FALSE);
	}
	if (m_kill.GetCheck()) { // 需要秒杀僵尸
		BYTE data[] = { 0xFF, 0x90, 0x90 };
		WriteMemory(data, sizeof(data), 0x00531310);
	}else { // 不需要秒杀僵尸
		BYTE data[] = { 0x7c, 0x24, 0x20 };
		WriteMemory(data, sizeof(data), 0x00531310);
	}
}

// 无CD
void CPVZCheaterDlg::OnBnClickedNocd() {
	log("未完成");
}

// 植物不死
void CPVZCheaterDlg::OnBnClickedPlantnodie() {
	/*
	修改前:
	0052FCF0 - 83 46 40 FC - add dword ptr [esi+40],-04 <<
	*/
	/*
	修改后:
	0052FCF0      90            nop
	0052FCF1      90            nop
	0052FCF2      90            nop
	0052FCF3      90            nop
	*/
	if (g_dlg->m_pnd.GetCheck()){ // 选中植物不死
		BYTE date[] = {0x90, 0x90, 0x90, 0x90};
		WriteMemory(date, sizeof(date), 0x0052FCF0);
	}else { // 未选中植物不死
		BYTE date[] = { 0x83, 0x43, 0x40, 0xFC};
		WriteMemory(date, sizeof(date), 0x0052FCF0);
	}

}

// 后台运行
void CPVZCheaterDlg::OnBnClickedBackstage() {
	log("未完成");
}

// 僵尸不死
void CPVZCheaterDlg::OnBnClickedZombienodie() {
	/*
	修改前： 
	0053130F  2B 7C 24 20     sub edi,dword ptr ss:[esp+0x20]
	修改后：
	0053130F      90            nop
	00531310      90            nop
	00531311      90            nop
	00531312      90            nop
	*/
	if (m_kill.GetCheck()) { // 秒杀僵尸与僵尸不死不可兼得
		g_dlg->m_kill.SetCheck(FALSE);
	}
	if (m_znd.GetCheck()) { // 选中僵尸不死
		BYTE date[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(date, sizeof(date), 0x53130F);
	}else { // 未选中僵尸不死
		BYTE date[] = { 0x2B, 0x7C, 0x24, 0x20};
		WriteMemory(date, sizeof(date), 0x53130F);
	}

}