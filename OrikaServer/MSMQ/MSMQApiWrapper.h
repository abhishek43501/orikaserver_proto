// MSMQApiWrapper.h: interface for the CMSMQApiWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSMQAPIWRAPPER_H__8F279E05_FAF5_11D3_AB6B_0050BAD05CD9__INCLUDED_)
#define AFX_MSMQAPIWRAPPER_H__8F279E05_FAF5_11D3_AB6B_0050BAD05CD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MSMQ_NOTIMPLEMENETED	-999
#define MSMQ_FUNCTIONFAILURE	-1
#define MSMQ_FUNCTIONSUCCESS	0
#define MSMQ_QUEUEEXISTS		1
#define MSMQ_NOQUEUEEXISTS		2
#define MSMQ_NOMESSAGESINQUEUE	3

//Import the MSMQ Api using the Native COM Support
#import "mqoa.dll" no_namespace


class CMSMQApiWrapper  
{
public:
	CMSMQApiWrapper();
	virtual ~CMSMQApiWrapper();
//Member Functions
public:

	//int SendByteMessage(LPCTSTR pszQueueLabel,CString szLabel,char* szMessage);

	void CharToByte(char* chars, byte* bytes, unsigned int count);

	int ReadByteMessage(LPCTSTR pszQueueLabel,CString &szLabel,CString &szMessage);
	void ByteTochar( byte* bytes,char* chars, unsigned int count);


	int LocatePrivateQueue(LPCTSTR pszQueueLabel,IMSMQQueueInfoPtr &qinfo);


//	Creating A Queue [Public/Private]
	
	int	CreateQueue(LPCTSTR pszPathQueue,LPCTSTR pszQueueLabel,BOOL bPublic = TRUE);
	int	CreatePublicQueue(LPCTSTR pszPathQueue,LPCTSTR pszQueueLabel);
	//Private Queues Only Creation Support Provided now
	int	CreatePrivateQueue(LPCTSTR pszPathQueue,LPCTSTR pszQueueLabel);

//	Deleting A Queue 
	
	int	DeletePublicQueue(LPCTSTR pszQueueLabel);

//	Locating A Public Queue By Label
	
	int	LocatePublicQueue(LPCTSTR pszPathQueue,IMSMQQueueInfoPtr &qinfo);

//	Synchronously Purging A Queue
	int	SynchronousPurgePublicQueue(LPCTSTR pszQueueLabel);

//	Retrieving and Setting Queue Properties

	int RetrieveAuthenticationLevelOfQueue(LPCTSTR pszQueueLabel,int &iAuthenticationLevel);
	int SetAuthenticationLevelOfQueue(LPCTSTR pszQueueLabel,int iAuthenticationLevel);

	int RetrievePriorityLevelOfQueue(LPCTSTR pszQueueLabel,int &iPriorityLevel);
	int SetPriorityLevelOfQueue(LPCTSTR pszQueueLabel,int iPriorityLevel);

	int RetrieveFormatNameOfQueue(LPCTSTR pszQueueLabel,CString &szFormatName);

	int RetrieveTransactionLevelOfQueue(LPCTSTR pszQueueLabel,int &iTransactionLevel);

	int RetrieveReadLevelOfQueue(LPCTSTR pszQueueLabel,int &iReadLevel);

	int RetrieveJournalingLevelOfQueue(LPCTSTR pszQueueLabel,int &iJournalingLevel);
	int SetJournalingLevelOfQueue(LPCTSTR pszQueueLabel,int iJournalingLevel);


	int RetrieveMaximumSizeOfQueueJournal(LPCTSTR pszQueueLabel,long &lMaximumSize);
	int SetMaximumSizeOfQueueJournal(LPCTSTR pszQueueLabel,long lMaximumSize);

	int RetrieveLabelOfQueue(LPCTSTR pszQueuePath,CString &szLabel);
	//int SetLabelOfQueue(LPCTSTR pszQueueLabel,CString szLabel);

	int RetrievePrivacyLevelOfQueue(LPCTSTR pszQueueLabel,int &iPrivacyLevel);
	int SetPrivacyLevelOfQueue(LPCTSTR pszQueueLabel,int iPrivacyLevel);

	int RetrieveMaximumSizeOfQueue(LPCTSTR pszQueueLabel,long &lMaximumSize);
	int SetMaximumSizeOfQueue(LPCTSTR pszQueueLabel,long lMaximumSize);

	int RetrievePathNameOfQueue(LPCTSTR pszQueueLabel,CString &szPathName);

//	Sending A Message [String other data types and persistent objects for files)
//	Sending A String Message is currentlu supported 
//	Other Data types , files and even persistent objects can be sent
//	using MSMQ
	int SendStringMessage(LPCTSTR pszQueueLabel,CString szLabel,char* szMessage);

//	Reading A Message[String other data types and persistent objects for files)
//	Reading A String Message is currentlu supported 
//	Other Data types , files and even persistent objects can be read
//	using MSMQ
//	Requesting Events Notification For A Queue is implemented in the 
//	CMSMQEventNotifier class
	int ReadStringMessage(LPCTSTR pszQueueLabel,CString &szLabel,CString &szMessage);


};

//	Note: Support for Transactional Queues is intended to
//			be added at an unspecified future date any enhancements
//			to the class or additional functionality for these
//			by anybody is welcome.

#endif // !defined(AFX_MSMQAPIWRAPPER_H__8F279E05_FAF5_11D3_AB6B_0050BAD05CD9__INCLUDED_)
