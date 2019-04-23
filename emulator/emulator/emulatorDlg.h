
// emulatorDlg.h : header file
//

#pragma once
#include "pa.h"
#include "WaveFile.h"
#include "mutex"
#include "afxcmn.h"
#include "afxwin.h"
#include "SliderGDICtrl.h"

using namespace SliderGdi;

const char g_keyStrSet[] = "1234567890QWERTYUIOPASDFGHJKLZXCVBNM";



// CemulatorDlg dialog
class CemulatorDlg : public CDialogEx
{
// Construction
public:
	CemulatorDlg(CWnd* pParent = NULL);	// standard constructor
	~CemulatorDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMULATOR_DIALOG };
#endif
	void AudioCallbackProc(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
private:
	void InitAudio();
	void DeinitAudio();
	bool startEngine();
// Implementation
protected:
	HICON m_hIcon;

	PaStream* m_paStream;

	WaveFileWriter m_wavWriter;
	HANDLE m_hMainThread;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void  OnSlVolChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void  OnSlPortamentoTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void  OnSlPortamentoDepthChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void  OnSlAttackTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void  OnSlReleaseTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void  OnSlReleaseLevelChange(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedEngine();
	afx_msg void OnBnClickedUseport();
	afx_msg void OnBnClickedSaveParam();
	afx_msg void OnBnClickedBtnChangedir();
	afx_msg void OnBnClickedBtnChangeKeyset();
	void sendKeyToEngine(int key, int event);
	virtual void OnCancel();
	void initKeySetNames();
	void initKeySet();
	void selectedRow(int rowIndex, int ev);
	CSliderGdiCtrlT<double>  m_slVol;
	mutex m_mutexWriteFile;

	CSliderGdiCtrlT<double> m_slPortamentoTime;
	CSliderGdiCtrlT<double> m_slPortamentoDepth;
	CButton m_checkPort;
	CSliderGdiCtrlT<double> m_sldAttackTime;
	CSliderGdiCtrlT<double> m_sldReleaseTime;
	CSliderGdiCtrlT<double> m_sldReleaseLevel;
	CListCtrl m_lstKeyName;
	CListCtrl m_lstKeySet;
	bool m_selectFromKey;
	afx_msg void OnLvnItemchangingLstKeyset(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedKeyName(NMHDR *pNMHDR, LRESULT *pResult);
};
