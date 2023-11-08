// MSMQApiWrapper.cpp: implementation of the CMSMQApiWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSMQApiWrapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMSMQApiWrapper::CMSMQApiWrapper()
{
	//Initialise OLE Support
	OleInitialize(NULL); 
}

CMSMQApiWrapper::~CMSMQApiWrapper()
{

}

//	Creating A Queue [Public/Private]
	
int CMSMQApiWrapper::CreateQueue(LPCTSTR pszPathQueue,LPCTSTR pszQueueLabel,BOOL bPublic)
{
	try
	{
		IMSMQQueueInfoPtr qinfo("MSMQ.MSMQQueueInfo");
		CString szQueueName,szQueueLabel;
		szQueueLabel = pszQueueLabel;
		szQueueName = pszPathQueue;
		
		if(szQueueName.Find(L"\\") == -1)
		{
			//szQueueName.Format(".\\%s",pszPathQueue);
			//User Has not provided complete path so let
			//us do it for him but on the local machine
			//independent client
			if(bPublic)
				szQueueName.Format(L".\\%s",pszPathQueue);
			else
				szQueueName.Format(L".\\PRIVATE$\\%s",pszPathQueue);
			//szQueueName.Format(".\\PRIVATE$\\%s",pszPathQueue);		
		}		
		qinfo->PathName = szQueueName.AllocSysString();
		qinfo->Label = szQueueLabel.AllocSysString();
		try
		{
			qinfo->Create();
		}
		catch (_com_error comerr)
		{
			HRESULT hr = comerr.Error();
			if(hr == MQ_ERROR_QUEUE_EXISTS)
			{
				return MSMQ_QUEUEEXISTS;
			}
			else
			{
				throw comerr;
			}
		}
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr)
	{
	    return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::CreatePublicQueue(LPCTSTR pszPathQueue,LPCTSTR pszQueueLabel)
{
	return CreateQueue(pszPathQueue,pszQueueLabel,TRUE);
}

int CMSMQApiWrapper::CreatePrivateQueue(LPCTSTR pszPathQueue,LPCTSTR pszQueueLabel)
{
	return CreateQueue(pszPathQueue,pszQueueLabel,FALSE);
}

//	Deleting A Queue 

int CMSMQApiWrapper::DeletePublicQueue(LPCTSTR pszQueueLabel)
{
	IMSMQQueueInfoPtr qinfo = NULL;
	int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
	if(iResult == MSMQ_QUEUEEXISTS)
	{
		try
		{
			qinfo->Delete();
		}
		catch (_com_error comerr)
		{
			return MSMQ_FUNCTIONFAILURE;
		}
		return MSMQ_FUNCTIONSUCCESS;
	}
	else 
		return iResult;
	return MSMQ_FUNCTIONFAILURE;
}


//	Locating A Public Queue By Label
int CMSMQApiWrapper::LocatePrivateQueue(LPCTSTR pszQueueLabel,IMSMQQueueInfoPtr &qinfo)
{
	try
	{				
		CString szQueueName,szQueueLabel;
		szQueueLabel = pszQueueLabel;
		szQueueName = pszQueueLabel;
		
			
		szQueueName.Format(L".\\PRIVATE$\\%s",pszQueueLabel);					
		
		qinfo->PathName = szQueueName.AllocSysString();
		qinfo->Label = szQueueLabel.AllocSysString();
		

		if (qinfo == NULL)
		{
		   return MSMQ_NOQUEUEEXISTS;
		}

		return MSMQ_QUEUEEXISTS;
	}
	catch (_com_error comerr)
	{
	
	    return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

//	Locating A Public Queue By Label

int CMSMQApiWrapper::LocatePublicQueue(LPCTSTR pszQueueLabel,IMSMQQueueInfoPtr &qinfo)
{
	try
	{
		IMSMQQueryPtr query("MSMQ.MSMQQuery");
		IMSMQQueueInfosPtr qinfos;

		_variant_t vtLabel;
		CString szQueueLabel = pszQueueLabel;				
		vtLabel = szQueueLabel.AllocSysString();


		////////////////////////////////////////////////////////////////////
		// Run query to retrieve MSMQQueueInfos object.
		////////////////////////////////////////////////////////////////////

		//.\\PRIVATE$
		qinfos = query->LookupQueue(&vtMissing, &vtMissing, &vtLabel);

		////////////////////////////////////////////////////////////////////
		// Retrieve first MSMQQueueInfo object and exit if no queues found.
		////////////////////////////////////////////////////////////////////

		qinfo = qinfos->Next();

		if (qinfo == NULL)
		{
		   return MSMQ_NOQUEUEEXISTS;
		}

		return MSMQ_QUEUEEXISTS;
	}
	catch (_com_error comerr)
	{
	
	    return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

//	Synchronously Purging A Queue

int CMSMQApiWrapper::SynchronousPurgePublicQueue(LPCTSTR pszQueueLabel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult; 

		_variant_t vtReceiveTimeout;
		vtReceiveTimeout = (long)1000;

		IMSMQQueuePtr qRec;
		IMSMQMessagePtr msgRec("MSMQ.MSMQMessage");
		////////////////////////////////////////////////////////////
		// Open queue to retrieve message.
		////////////////////////////////////////////////////////////
		qRec = qinfo->Open(MQ_RECEIVE_ACCESS, MQ_DENY_NONE);


		////////////////////////////////////////////////////////////
		// Retrieve messages from queue.
		////////////////////////////////////////////////////////////
		msgRec = qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
		if (msgRec == NULL)
		{
			return MSMQ_NOMESSAGESINQUEUE;
		}

		while (msgRec != NULL)
		{
			//Receive The messages thereby removing it
			msgRec = qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
		}
		////////////////////////////////////////////////////////////
		// Close queue.
		////////////////////////////////////////////////////////////
		qRec->Close();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

//	Retrieving and Setting Queue Properties

int CMSMQApiWrapper::RetrieveAuthenticationLevelOfQueue(LPCTSTR pszQueueLabel,int &iAuthenticationLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iAuthenticationLevel = qinfo->Authenticate;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetAuthenticationLevelOfQueue(LPCTSTR pszQueueLabel,int iAuthenticationLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->Authenticate = iAuthenticationLevel;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrievePriorityLevelOfQueue(LPCTSTR pszQueueLabel,int &iPriorityLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iPriorityLevel = qinfo->BasePriority;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetPriorityLevelOfQueue(LPCTSTR pszQueueLabel,int iPriorityLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->BasePriority = iPriorityLevel;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveFormatNameOfQueue(LPCTSTR pszQueueLabel,CString &szFormatName)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		BSTR FAR FormatName = qinfo->FormatName;
		szFormatName.SetSysString(&FormatName);
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveTransactionLevelOfQueue(LPCTSTR pszQueueLabel,int &iTransactionLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iTransactionLevel = qinfo->IsTransactional;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveReadLevelOfQueue(LPCTSTR pszQueueLabel,int &iReadLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iReadLevel = qinfo->IsWorldReadable;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveJournalingLevelOfQueue(LPCTSTR pszQueueLabel,int &iJournalingLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iJournalingLevel = qinfo->Journal;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetJournalingLevelOfQueue(LPCTSTR pszQueueLabel,int iJournalingLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->Journal = iJournalingLevel;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveMaximumSizeOfQueueJournal(LPCTSTR pszQueueLabel,long &lMaximumSize)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		lMaximumSize = qinfo->JournalQuota;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetMaximumSizeOfQueueJournal(LPCTSTR pszQueueLabel,long lMaximumSize)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->JournalQuota = lMaximumSize;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveLabelOfQueue(LPCTSTR pszQueuePath,CString &szLabel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo("MSMQ.MSMQQueueInfo");
		try
		{
			qinfo->PathName = pszQueuePath;
			qinfo->Create();
		}
		catch (_com_error comerr)
		{
			HRESULT hr = comerr.Error();
			if(hr == MQ_ERROR_QUEUE_EXISTS)
			{
				szLabel = (LPSTR)qinfo->Label;
				return MSMQ_FUNCTIONSUCCESS;
			}
		}
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

//int CMSMQApiWrapper::SetLabelOfQueue(LPCTSTR pszQueueLabel,CString szLabel)
//{
//	try
//	{
//		IMSMQQueueInfoPtr qinfo = NULL;
//		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
//		if(iResult != MSMQ_QUEUEEXISTS)
//			return iResult;
//		qinfo->Refresh();
//		COleVariant vtLabel;
//		vtLabel.SetString(szLabel,VT_BSTR);
//		qinfo->Label = ((LPVARIANT)vtLabel)->bstrVal;
//		qinfo->Update();
//		return MSMQ_FUNCTIONSUCCESS;
//	}
//	catch (_com_error comerr) 
//	{
//		return MSMQ_FUNCTIONFAILURE;
//	}
//	return MSMQ_FUNCTIONFAILURE;
//}

int CMSMQApiWrapper::RetrievePrivacyLevelOfQueue(LPCTSTR pszQueueLabel,int &iPrivacyLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iPrivacyLevel = qinfo->PrivLevel;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetPrivacyLevelOfQueue(LPCTSTR pszQueueLabel,int iPrivacyLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->PrivLevel = iPrivacyLevel;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveMaximumSizeOfQueue(LPCTSTR pszQueueLabel,long &lMaximumSize)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		lMaximumSize = qinfo->Quota;//-1 indicates infinity
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetMaximumSizeOfQueue(LPCTSTR pszQueueLabel,long lMaximumSize)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->Quota = lMaximumSize;//-1 indicates infinity
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrievePathNameOfQueue(LPCTSTR pszQueueLabel,CString &szPathName)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		szPathName = (LPSTR)qinfo->PathName;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SendStringMessage(LPCTSTR pszQueueLabel,CString szLabel,char* szMessage)
{


	IMSMQQueueInfoPtr qinfo("MSMQ.MSMQQueueInfo");
		CString szQueueName,szQueueLabel;
		szQueueLabel = pszQueueLabel;
		szQueueName = pszQueueLabel;
		
		if(szQueueName.Find(L"\\") == -1)
		{			
				szQueueName.Format(L".\\PRIVATE$\\%s",pszQueueLabel);					
		}		
		qinfo->PathName = szQueueName.AllocSysString();
		qinfo->Label = szQueueLabel.AllocSysString();

		int iResult=MSMQ_QUEUEEXISTS;

	if(iResult == MSMQ_QUEUEEXISTS)
	{
		try
		{
			IMSMQQueuePtr qSend;
			IMSMQMessagePtr m("MSMQ.MSMQMessage");
			qSend = qinfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
			//COleVariant vtMessage,vtLabel;
			_variant_t  vtMessage,vtLabel;

			_bstr_t tmp=szMessage;
			LPCTSTR m_mes=tmp;

			_bstr_t tmp_label=szLabel;
			char* chat_label=tmp_label;

			vtMessage.SetString(szMessage);			
			vtLabel.SetString(chat_label);
			m->Body = vtMessage.bstrVal;
			m->Label = vtLabel.bstrVal;
			m->Send(qSend);
      
		}
		catch (_com_error comerr)
		{
			return MSMQ_FUNCTIONFAILURE;
		}
		return MSMQ_FUNCTIONSUCCESS;
	}
	else 
		return iResult;
	return MSMQ_FUNCTIONFAILURE;
}



//int CMSMQApiWrapper::SendByteMessage(LPCTSTR pszQueueLabel,CString szLabel,char* szMessage)
//{
//
//
//	IMSMQQueueInfoPtr qinfo("MSMQ.MSMQQueueInfo");
//		CString szQueueName,szQueueLabel;
//		szQueueLabel = pszQueueLabel;
//		szQueueName = pszQueueLabel;
//		
//		if(szQueueName.Find(L"\\") == -1)
//		{			
//				szQueueName.Format(L".L\\PRIVATE$\\%s",pszQueueLabel);					
//		}		
//		qinfo->PathName = szQueueName.AllocSysString();
//		qinfo->Label = szQueueLabel.AllocSysString();
//
//		int iResult=MSMQ_QUEUEEXISTS;
//
//	if(iResult == MSMQ_QUEUEEXISTS)
//	{
//		try
//		{
//			IMSMQQueuePtr qSend;
//			IMSMQMessagePtr m("MSMQ.MSMQMessage");
//			qSend = qinfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
//			_variant_t  vtMessage,vtLabel;
//			_bstr_t tmp=szMessage;
//			char* m_mes=tmp;
//
//
//			CString temp_str_message=m_mes;
//
//			int message_len=temp_str_message.GetLength();
//
//			byte* m_byte_message=NULL ;
//			CharToByte(szMessage,m_byte_message,message_len);
//
//
//			vtMessage.SetString(m_mes);		
//
//			
//
//
//
//
//			vtLabel.SetString(szLabel,VT_BSTR);
//			m->Body = ((LPVARIANT)vtMessage)->bstrVal;
//			m->Label = ((LPVARIANT)vtLabel)->bstrVal;
//			m->Send(qSend);
//      
//		}
//		catch (_com_error comerr)
//		{
//			return MSMQ_FUNCTIONFAILURE;
//		}
//		return MSMQ_FUNCTIONSUCCESS;
//	}
//	else 
//		return iResult;
//	return MSMQ_FUNCTIONFAILURE;
//}






void CMSMQApiWrapper::CharToByte(char* chars, byte* bytes, unsigned int count)
{
    for(unsigned int i = 0; i < count; i++)
    	bytes[i] = (byte)chars[i];
}

void CMSMQApiWrapper::ByteTochar( byte* bytes,char* chars, unsigned int count)
{
    for(unsigned int i = 0; i < count; i++)
    	chars[i] = (char)bytes[i];
}


int CMSMQApiWrapper::ReadStringMessage(LPCTSTR pszQueueLabel,CString &szLabel,CString &szMessage)
{
	_variant_t vtReceiveTimeout;
	vtReceiveTimeout = (long)1000;
	//IMSMQQueueInfoPtr qinfo = NULL;
	IMSMQQueueInfoPtr qinfo("MSMQ.MSMQQueueInfo");
	int iResult = LocatePrivateQueue(pszQueueLabel,qinfo);
	if(iResult == MSMQ_QUEUEEXISTS)
	{
		IMSMQQueuePtr qRec;
		IMSMQMessagePtr msgRec("MSMQ.MSMQMessage");
    	try
		{
			qRec = qinfo->Open(MQ_RECEIVE_ACCESS, MQ_DENY_NONE);
			msgRec = qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
			if (msgRec != NULL)
			{
				VARIANT vtValue;
				vtValue = msgRec->GetBody();				

				if(vtValue.vt = VT_BSTR)
					szMessage = vtValue.bstrVal;
				szLabel = (LPSTR) msgRec->GetLabel();
			}
		}
		catch (_com_error comerr)
		{
			
			return MSMQ_FUNCTIONFAILURE;
		}
		return MSMQ_FUNCTIONSUCCESS;
	}
	else 
		return iResult;
	return MSMQ_FUNCTIONFAILURE;
}


int CMSMQApiWrapper::ReadByteMessage(LPCTSTR pszQueueLabel,CString &szLabel,CString &szMessage)
{
	_variant_t vtReceiveTimeout;
	vtReceiveTimeout = (long)1000;
	//IMSMQQueueInfoPtr qinfo = NULL;
	IMSMQQueueInfoPtr qinfo("MSMQ.MSMQQueueInfo");
	int iResult = LocatePrivateQueue(pszQueueLabel,qinfo);
	if(iResult == MSMQ_QUEUEEXISTS)
	{
		IMSMQQueuePtr qRec;
		IMSMQMessagePtr msgRec("MSMQ.MSMQMessage");
    	try
		{
			qRec = qinfo->Open(MQ_RECEIVE_ACCESS, MQ_DENY_NONE);
			msgRec = qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
			if (msgRec != NULL)
			{
				VARIANT vtValue;
				vtValue = msgRec->GetBody();								
				if(vtValue.vt = VT_BSTR)
					szMessage = vtValue.bstrVal;
				szLabel = (LPSTR) msgRec->GetLabel();
			}
		}
		catch (_com_error comerr)
		{
			return MSMQ_FUNCTIONFAILURE;
		}
		return MSMQ_FUNCTIONSUCCESS;
	}
	else 
		return iResult;
	return MSMQ_FUNCTIONFAILURE;
}
