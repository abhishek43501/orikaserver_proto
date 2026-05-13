#ifndef _ATA_LOGFILE_
#define _ATA_LOGFILE_
class CLogFile
{
public:
	
	void  CreateDirectories(CString filename);
	//	Constructor, open the logfile
	 CLogFile();

	//	Destructor, close if logfile if opened
	~CLogFile();

	void OpenFile(CString strFile, bool bAppend = TRUE, long lTruncate = 4096);
	void ChangeFile(CString strFile, bool bAppend = TRUE, long lTruncate = 4096);
	void CloseFile();

	// I7: set the shutdown gate so subsequent LogEvent/Write calls become
	// no-ops. Called from CStaticClass::Shutdown() before MFC tears down the
	// cross-TU statics so any late LogEvent (e.g. from another static's
	// dtor) cannot reach an already-destroyed CRITICAL_SECTION.
	void Shutdown();

	char* CstringToCharP(CString event);

	//	Write log info into the logfile, with printf like parameters support
	void CLogFile::Write(CString pszFormat);
	void CLogFile::LogEvent(CString event);
private:
	FILE*	m_pLogFile;
	long	m_lTruncate;
	CRITICAL_SECTION	m_cs;
	// I7: once set, LogEvent/Write return immediately. Plain bool is fine -
	// the only writer is CStaticClass::Shutdown() on the main UI thread,
	// and readers tolerate one transient miss.
	bool	m_shutdown;

	CString	m_filename;


	
};

#endif //_ATA_LOGFILE_
