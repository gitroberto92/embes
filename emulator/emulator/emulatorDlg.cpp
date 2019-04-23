
// emulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "emulator.h"
#include "emulatorDlg.h"
#include "afxdialogex.h"

#include "engine_main.h"
#include "SDTest.h"
#include "time_counter.h"
#include "ConfigFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



HANDLE g_hReadEvent;
HANDLE g_hProcEvent;
// CAboutDlg dialog used for App About


static int paCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	/* Cast data passed through stream to our structure. */
	//paTestData *data = (paTestData*)userData; 
	CemulatorDlg* pDlg = (CemulatorDlg*)userData;
	pDlg->AudioCallbackProc(inputBuffer, outputBuffer, framesPerBuffer, timeInfo);
	return 0;
}

DWORD _stdcall thread_proc(void* p_param)
{
	g_hProcEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	
	while (1)
	{
#ifdef WIN32
		WaitForSingleObject(g_hReadEvent, INFINITE);
#endif
		main_loop();
#ifdef WIN32
		SetEvent(g_hProcEvent);
#endif
	}

	main_finish();


	return 0;
}

// CemulatorDlg dialog



CemulatorDlg::CemulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EMULATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CemulatorDlg::~CemulatorDlg()
{
	main_finish();
}

void CemulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slVol);
	DDX_Control(pDX, IDC_SLIDER3, m_slPortamentoTime);
	DDX_Control(pDX, IDC_SLIDER2, m_slPortamentoDepth);
	DDX_Control(pDX, IDC_USEPORT, m_checkPort);
	DDX_Control(pDX, IDC_SLIDER5, m_sldAttackTime);
	DDX_Control(pDX, IDC_SLIDER4, m_sldReleaseTime);
	DDX_Control(pDX, IDC_SLIDER6, m_sldReleaseLevel);
	DDX_Control(pDX, IDC_KEY_NAME, m_lstKeyName);
	DDX_Control(pDX, IDC_LST_KEYSET, m_lstKeySet);
}

BEGIN_MESSAGE_MAP(CemulatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RECORD, OnBnClickedRecord)

	ON_NOTIFY(TB_THUMBTRACK, IDC_SLIDER1, OnSlVolChange)
	ON_NOTIFY(TB_THUMBPOSITION, IDC_SLIDER1, OnSlVolChange)

	ON_NOTIFY(TB_THUMBTRACK, IDC_SLIDER3, OnSlPortamentoTimeChange)
	ON_NOTIFY(TB_THUMBPOSITION, IDC_SLIDER3, OnSlPortamentoTimeChange)

	ON_NOTIFY(TB_THUMBTRACK, IDC_SLIDER2, OnSlPortamentoDepthChange)
	ON_NOTIFY(TB_THUMBPOSITION, IDC_SLIDER2, OnSlPortamentoDepthChange)

	ON_NOTIFY(TB_THUMBTRACK, IDC_SLIDER5, OnSlAttackTimeChange)
	ON_NOTIFY(TB_THUMBPOSITION, IDC_SLIDER5, OnSlAttackTimeChange)

	ON_NOTIFY(TB_THUMBTRACK, IDC_SLIDER4, OnSlReleaseTimeChange)
	ON_NOTIFY(TB_THUMBPOSITION, IDC_SLIDER4, OnSlReleaseTimeChange)

	ON_NOTIFY(TB_THUMBTRACK, IDC_SLIDER6, OnSlReleaseLevelChange)
	ON_NOTIFY(TB_THUMBPOSITION, IDC_SLIDER6, OnSlReleaseLevelChange)


	ON_BN_CLICKED(IDC_ENGINE, OnBnClickedEngine)
	ON_BN_CLICKED(IDC_USEPORT, OnBnClickedUseport)

	ON_BN_CLICKED(IDC_SAVE_PARAM, &CemulatorDlg::OnBnClickedSaveParam)
	ON_BN_CLICKED(IDC_BTN_CHANGEDIR, &CemulatorDlg::OnBnClickedBtnChangedir)
	ON_BN_CLICKED(IDC_BTN_CHANGE_KEYSET, &CemulatorDlg::OnBnClickedBtnChangeKeyset)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LST_KEYSET, &CemulatorDlg::OnLvnItemchangingLstKeyset)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_KEY_NAME, &CemulatorDlg::OnLvnItemchangedKeyName)
END_MESSAGE_MAP()


// CemulatorDlg message handlers

BOOL CemulatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	main_init();

	SetDlgItemTextA(this->m_hWnd, IDC_TXTDIR, recDirectory());

	m_slVol.SetInitVals(0.0, 1.0, 2, (double)getVolumn());
	m_slVol.SetCaption(L"Volumn");

	m_checkPort.SetCheck(usePortamento());

	m_slPortamentoDepth.SetInitVals(0.0, 1.0, 2, portamentoDepth());
	m_slPortamentoDepth.SetCaption(L"Portamento Depth");

	m_slPortamentoTime.SetInitVals(0.0, 0.5, 2, portamentoTime());
	m_slPortamentoTime.SetCaption(L"Portamento Time(s)");

	m_sldAttackTime.SetInitVals(0.0, 0.5, 2, (double)attackTime());
	m_sldAttackTime.SetCaption(L"Attack Time(s)");

	m_sldReleaseTime.SetInitVals(0.0, 0.5, 2, (double)releaseTime());
	m_sldReleaseTime.SetCaption(L"Release Time(s)");

	m_sldReleaseLevel.SetInitVals(0.1, 0.7, 2, (double)releaseLevel());
	m_sldReleaseLevel.SetCaption(L"Release Level");

	m_lstKeyName.InsertColumn(0, L"No", 0, 30);
	m_lstKeyName.InsertColumn(1, L"Name", 0, 120);
	m_lstKeyName.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_lstKeySet.InsertColumn(0, L"Finger Set", 0, 120);
	m_lstKeySet.InsertColumn(1, L"Note Name", 0, 120);
	m_lstKeySet.InsertColumn(2, L"Key", 0, 120);
	m_lstKeySet.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	initKeySetNames();

	InitAudio();
	startEngine();

	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CemulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CemulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CemulatorDlg::OnSlVolChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	setVolumn(m_slVol.GetCurrValue());
}

void CemulatorDlg::OnSlPortamentoTimeChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	setPortamentoTime(m_slPortamentoTime.GetCurrValue());
}


void CemulatorDlg::OnSlPortamentoDepthChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	setPortamentoDepth(m_slPortamentoDepth.GetCurrValue());
}


void CemulatorDlg::OnSlAttackTimeChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	setAttackTime(m_sldAttackTime.GetCurrValue());
}

void CemulatorDlg::OnSlReleaseTimeChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	setReleaseTime(m_sldReleaseTime.GetCurrValue());
}

void CemulatorDlg::OnSlReleaseLevelChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	setReleaseLevel(m_sldReleaseLevel.GetCurrValue());
}

void CemulatorDlg::AudioCallbackProc(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo)
{

	if (g_buffInit == false)
		return;
	WaitForSingleObject(g_hProcEvent, INFINITE);
	memcpy(outputBuffer, g_outBuff, AUDIO_FRAME_LEN * AUDIO_CHANNELS * sizeof(short));
	SetEvent(g_hReadEvent);

	if (m_wavWriter.IsOpened() == true)
	{
		m_mutexWriteFile.lock();
		m_wavWriter.AppendData(outputBuffer, AUDIO_FRAME_LEN * AUDIO_CHANNELS * sizeof(short));
		m_mutexWriteFile.unlock();
	}
}

void CemulatorDlg::InitAudio()
{
	PaStream* pa = PA_OpenDefAudioStream(paCallback, this, AUDIO_SAMPLING_RATE, AUDIO_FRAME_LEN, paInt16, 1, AUDIO_CHANNELS);
	m_paStream = pa;
}

void CemulatorDlg::DeinitAudio()
{
	if (m_paStream)
	{
		PA_CloseAudioStream(m_paStream);
		m_paStream = nullptr;
	}
}

bool CemulatorDlg::startEngine()
{
	m_hMainThread = CreateThread(NULL, 0, thread_proc, this, 0, NULL);
	if (m_hMainThread == NULL)
		return false;
	return true;
}


void CemulatorDlg::OnBnClickedOk()
{

}

void CemulatorDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DeinitAudio();
	CDialogEx::OnCancel();
}

void CemulatorDlg::initKeySetNames()
{
	KeySetGroup** groups = ConfigManager->keySetGroups();
	int row = 0;
	for (int i = 0; i < MAX_KEYSET_SIZE; i++)
	{
		if(groups[i] == NULL)
			continue;
		CString numberStr;
		CString nameStr(groups[i]->name);
		numberStr.Format(L"%d", row + 1);
		m_lstKeyName.InsertItem(row, numberStr);
		m_lstKeyName.SetItemText(row, 1, nameStr);
		++row;
	}
	initKeySet();
}

void CemulatorDlg::initKeySet()
{
	m_lstKeySet.DeleteAllItems();

	int row = 0;
	for (KeySetList* lstItem = ConfigManager->currentKeySet()->head; lstItem != NULL; lstItem = (KeySetList*)lstItem->next)
	{
		char tmp[NUM_KEYS + 1];
		for (int i = 0; i < NUM_KEYS; i++)
		{
			tmp[i] = ((lstItem->item->keyState >> i) & 1) + '0';
		}
		tmp[NUM_KEYS] = 0;
		CString fingerStr(tmp);
		ConfigFileManager::noteStrFromNumber(lstItem->item->note, tmp);
		CString noteStr(tmp);
		CString keyStr;
		keyStr.Format(L"%c", g_keyStrSet[row]);
		m_lstKeySet.InsertItem(0, fingerStr);
		m_lstKeySet.SetItemText(0, 1, noteStr);
		m_lstKeySet.SetItemText(0, 2, keyStr);
		++row;
	}
}

void CemulatorDlg::OnBnClickedRecord()
{
	if (m_wavWriter.IsOpened() == false)
	{
		if (m_wavWriter.Create("rec.wav", AUDIO_SAMPLING_RATE, 16, AUDIO_CHANNELS))
		{
			SetDlgItemText(IDC_RECORD, L"Stop");
		}
		else
		{
			MessageBox(L"Can not open record file.");
		}

	}
	else
	{
		m_mutexWriteFile.lock();
		m_wavWriter.Finish();
		m_mutexWriteFile.unlock();
		SetDlgItemText(IDC_RECORD, L"Start");
	}
}

void CemulatorDlg::OnBnClickedEngine()
{
	main_finish();
	DeinitAudio();
	//stopEngine();
	SDTestStart();

	main_init();
	InitAudio();
	startEngine();
	//MessageBox(L"Test Ended");
}

void CemulatorDlg::OnBnClickedUseport()
{
	if (m_checkPort.GetCheck())
	{
		setUsePortamento(true);
	}
	else
	{
		setUsePortamento(false);
	}
}


void CemulatorDlg::OnBnClickedSaveParam()
{
	saveParams();
}


void CemulatorDlg::OnBnClickedBtnChangedir()
{
	changeDirectory();
	main_finish();
	DeinitAudio();
	main_init();
	SetDlgItemTextA(this->m_hWnd, IDC_TXTDIR, recDirectory());
	InitAudio();
	startEngine();
}


void CemulatorDlg::OnBnClickedBtnChangeKeyset()
{
	changeKeySet();
	m_lstKeyName.SetItemState(ConfigManager->currentKeySetNumber(), LVIS_SELECTED, LVIS_SELECTED);
}


int g_uiPrevKey = -1;

int prevEvent;

void CemulatorDlg::sendKeyToEngine(int key, int event) {
	if (g_uiPrevKey != key || prevEvent != event)
	{
		CString keyStr;
		int rowIndex = -1;
		for (int i = 0; i < m_lstKeySet.GetItemCount(); i++) {
			CString str = m_lstKeySet.GetItemText(i, 2);
			if (str[0] == key) {
				rowIndex = i;
				break;
			}
		}
		if (rowIndex == -1)
		{
			return;
		}
		g_uiPrevKey = key;
		prevEvent = event;
		m_selectFromKey = true;
		if(event == NOTE_ON)
			m_lstKeySet.SetItemState(rowIndex, LVIS_SELECTED, LVIS_SELECTED);
		m_selectFromKey = false;
		selectedRow(rowIndex, event);
	}
}

void CemulatorDlg::OnLvnItemchangingLstKeyset(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_selectFromKey)
	{
		LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		// TODO: Add your control notification handler code here
		if ((pNMListView->uChanged & LVIF_STATE)
			&& (pNMListView->uOldState & LVIS_SELECTED) == 0
			&& (pNMListView->uNewState & LVIS_SELECTED))
		{
			int idx = pNMListView->iItem;
			if (idx >= 0) {
				CString itemStr = m_lstKeySet.GetItemText(idx, 2);
				selectedRow(idx, NOTE_ON);
			}
		}
	}
	*pResult = 0;
}

void CemulatorDlg::selectedRow(int rowIndex, int ev) {
	CString str = m_lstKeySet.GetItemText(rowIndex, 0);
	int state = 0;
	for (int i = 0; i < NUM_KEYS && str[i] != 0; i++)
	{
		if (str[i] == '1')
			state |= (1 << i);
	}
	if (ev == NOTE_ON)
	{
		SetDlgItemText(IDC_TEXT_KEY1, str);
	}
	raiseKeyEvent(state, ev);

}


void CemulatorDlg::OnLvnItemchangedKeyName(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if ((pNMListView->uChanged & LVIF_STATE)
		&& (pNMListView->uOldState & LVIS_SELECTED) == 0
		&& (pNMListView->uNewState & LVIS_SELECTED))
	{
		int idx = pNMListView->iItem;
		if (idx >= 0) {
			CString itemStr = m_lstKeyName.GetItemText(idx, 1);
			KeySetGroup** groups = ConfigManager->keySetGroups();
			for (int i = 0; i < MAX_KEYSET_SIZE; i++) {
				if (CString(groups[i]->name) == itemStr) {
					ConfigManager->setCurrentKeySet(i);
					initKeySet();
				}
			}
		}
	}
	*pResult = 0;
}
