//#include "stdafx.h"
#ifndef		CNetpositionTable_HEADER
#define		CNetpositionTable_HEADER
#include <atldbcli.h>
class CNetpositionTable
{
public:
	TCHAR m_login[20];
	TCHAR m_name[50];
	TCHAR m_symbol[32];
	double m_volume;
	double m_prevVol;
	double m_volDiff;
	double m_avg;
	double m_balance;
	double m_lastRate;
	double m_clientBrokTotal;
	double m_subBrokerBrokTotal;
	double m_brokerBrokTotal;
	int m_lastUpdated;
	double m_compBal;
	double m_extraVolume;
	double m_subBrokerVolume;
	double m_brokerVolume;
	double m_companyVolume;
	double m_clientLots;
	double m_subBrokerLots;
	double m_brokerLots;
	double m_companyLots;
	double m_clientPL;
	double m_subBrokerPL;
	double m_brokerPL;
	double m_companyPL;
	double m_clientTotal;
	double m_subBrokerTotal;
	double m_brokerTotal;
	double m_companyTotal;
	double m_grossExposure;
	double m_subBrokerExposure;
	double m_brokerExposure;
	double m_companyExposure;
	double m_multiplayer;
	TCHAR m_brokerName[20];
	TCHAR m_subBrokerName[20];
	TCHAR m_extraGroup[20];
	double m_subbrokerRatio;
	double m_brokerRatio;
	double m_companyRation;
	TCHAR m_clientBrokerageType[10];
	double m_clientBrokerage;
	double m_subBrokerBrokerage;
	double m_brokerBrokerage;
	double m_companyBrokerage;
	double m_maxAllowedVolume;
		
	
	
	DBSTATUS s_login;
	DBSTATUS s_name;
	DBSTATUS s_symbol;
	DBSTATUS s_volume;
	DBSTATUS s_prevVol;
	DBSTATUS s_volDiff;
	DBSTATUS s_avg;
	DBSTATUS s_balance;
	DBSTATUS s_lastRate;
	DBSTATUS s_clientBrokTotal;
	DBSTATUS s_subBrokerBrokTotal;
	DBSTATUS s_brokerBrokTotal;
	DBSTATUS s_lastUpdated;
	DBSTATUS s_compBal;
	DBSTATUS s_extraVolume;
	DBSTATUS s_subBrokerVolume;
	DBSTATUS s_brokerVolume;
	DBSTATUS s_companyVolume;
	DBSTATUS s_clientLots;
	DBSTATUS s_subBrokerLots;
	DBSTATUS s_brokerLots;
	DBSTATUS s_companyLots;
	DBSTATUS s_clientPL;
	DBSTATUS s_subBrokerPL;
	DBSTATUS s_brokerPL;
	DBSTATUS s_companyPL;
	DBSTATUS s_clientTotal;
	DBSTATUS s_subBrokerTotal;
	DBSTATUS s_brokerTotal;
	DBSTATUS s_companyTotal;
	DBSTATUS s_grossExposure;
	DBSTATUS s_subBrokerExposure;
	DBSTATUS s_brokerExposure;
	DBSTATUS s_companyExposure;
	DBSTATUS s_multiplayer;
	DBSTATUS s_brokerName;
	DBSTATUS s_subBrokerName;
	DBSTATUS s_extraGroup;
	DBSTATUS s_subbrokerRatio;
	DBSTATUS s_brokerRatio;
	DBSTATUS s_companyRation;
	DBSTATUS s_clientBrokerageType;
	DBSTATUS s_clientBrokerage;
	DBSTATUS s_subBrokerBrokerage;
	DBSTATUS s_brokerBrokerage;
	DBSTATUS s_companyBrokerage;
	DBSTATUS s_maxAllowedVolume;


	DBLENGTH    l_login;
	DBLENGTH    l_name;
	DBLENGTH    l_symbol;
	DBLENGTH 	l_volume;
	DBLENGTH 	l_prevVol;
	DBLENGTH 	l_volDiff;
	DBLENGTH 	l_avg;
	DBLENGTH 	l_balance;
	DBLENGTH 	l_lastRate;
	DBLENGTH 	l_clientBrokTotal;
	DBLENGTH 	l_subBrokerBrokTotal;
	DBLENGTH 	l_brokerBrokTotal;
	DBLENGTH 	l_lastUpdated;
	DBLENGTH 	l_compBal;
	DBLENGTH 	l_extraVolume;
	DBLENGTH 	l_subBrokerVolume;
	DBLENGTH 	l_brokerVolume;
	DBLENGTH 	l_companyVolume;
	DBLENGTH 	l_clientLots;
	DBLENGTH 	l_subBrokerLots;
	DBLENGTH 	l_brokerLots;
	DBLENGTH 	l_companyLots;
	DBLENGTH 	l_clientPL;
	DBLENGTH 	l_subBrokerPL;
	DBLENGTH 	l_brokerPL;
	DBLENGTH 	l_companyPL;
	DBLENGTH 	l_clientTotal;
	DBLENGTH 	l_subBrokerTotal;
	DBLENGTH 	l_brokerTotal;
	DBLENGTH 	l_companyTotal;
	DBLENGTH 	l_grossExposure;
	DBLENGTH 	l_subBrokerExposure;
	DBLENGTH 	l_brokerExposure;
	DBLENGTH 	l_companyExposure;
	DBLENGTH 	l_multiplayer;
	DBLENGTH 	l_brokerName;
	DBLENGTH 	l_subBrokerName;
	DBLENGTH 	l_extraGroup;
	DBLENGTH 	l_subbrokerRatio;
	DBLENGTH 	l_brokerRatio;
	DBLENGTH 	l_companyRation;
	DBLENGTH 	l_clientBrokerageType;
	DBLENGTH 	l_clientBrokerage;
	DBLENGTH 	l_subBrokerBrokerage;
	DBLENGTH 	l_brokerBrokerage;
	DBLENGTH 	l_companyBrokerage;
	DBLENGTH 	l_maxAllowedVolume;
// Column binding map
BEGIN_COLUMN_MAP(CNetpositionTable)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_name,s_name,l_name);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_prevVol,s_prevVol,l_prevVol);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_volDiff,s_volDiff,l_volDiff);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_volume,s_volume,l_volume);	
	COLUMN_ENTRY_LENGTH_STATUS(7,m_avg,s_avg,l_avg);
	COLUMN_ENTRY_LENGTH_STATUS(8,m_balance,s_balance,l_balance);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_lastRate,s_lastRate,l_lastRate);
	COLUMN_ENTRY_LENGTH_STATUS(10, m_clientBrokTotal,s_clientBrokTotal,l_clientBrokTotal);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_subBrokerBrokTotal,s_subBrokerBrokTotal,l_subBrokerBrokTotal);
	COLUMN_ENTRY_LENGTH_STATUS(12,m_brokerBrokTotal,s_brokerBrokTotal,l_brokerBrokTotal);
	COLUMN_ENTRY_LENGTH_STATUS(13,m_lastUpdated,s_lastUpdated,l_lastUpdated);
	COLUMN_ENTRY_LENGTH_STATUS(14,m_compBal,s_compBal,l_compBal);
	COLUMN_ENTRY_LENGTH_STATUS(15,m_extraVolume,s_extraVolume,l_extraVolume);
	COLUMN_ENTRY_LENGTH_STATUS(16,m_subBrokerVolume,s_subBrokerVolume,l_subBrokerVolume);
	COLUMN_ENTRY_LENGTH_STATUS(17,m_brokerVolume,s_brokerVolume,l_brokerVolume);
	COLUMN_ENTRY_LENGTH_STATUS(18,m_companyVolume,s_companyVolume,l_companyVolume);
	COLUMN_ENTRY_LENGTH_STATUS(19,m_clientLots,s_clientLots,l_clientLots);
	COLUMN_ENTRY_LENGTH_STATUS(20,m_subBrokerLots,s_subBrokerLots,l_subBrokerLots);
	COLUMN_ENTRY_LENGTH_STATUS(21,m_brokerLots,s_brokerLots,l_brokerLots);
	COLUMN_ENTRY_LENGTH_STATUS(22,m_companyLots,s_companyLots,l_companyLots);
	COLUMN_ENTRY_LENGTH_STATUS(23,m_clientPL,s_clientPL,l_clientPL);
	COLUMN_ENTRY_LENGTH_STATUS(24,m_subBrokerPL,s_subBrokerPL,l_subBrokerPL);
	COLUMN_ENTRY_LENGTH_STATUS(25,m_brokerPL,s_brokerPL,l_brokerPL);
	COLUMN_ENTRY_LENGTH_STATUS(26,m_companyPL,s_companyPL,l_companyPL);
	COLUMN_ENTRY_LENGTH_STATUS(27,m_clientTotal,s_clientTotal,l_clientTotal);
	COLUMN_ENTRY_LENGTH_STATUS(28,m_subBrokerTotal,s_subBrokerTotal,l_subBrokerTotal);
	COLUMN_ENTRY_LENGTH_STATUS(29,m_brokerTotal,s_brokerTotal,l_brokerTotal);
	COLUMN_ENTRY_LENGTH_STATUS(30,m_companyTotal,s_companyTotal,l_companyTotal);
	COLUMN_ENTRY_LENGTH_STATUS(31,m_grossExposure,s_grossExposure,l_grossExposure);
	COLUMN_ENTRY_LENGTH_STATUS(32,m_subBrokerExposure,s_subBrokerExposure,l_subBrokerExposure);
	COLUMN_ENTRY_LENGTH_STATUS(33,m_brokerExposure,s_brokerExposure,l_brokerExposure);
	COLUMN_ENTRY_LENGTH_STATUS(34,m_companyExposure,s_companyExposure,l_companyExposure);
	COLUMN_ENTRY_LENGTH_STATUS(35,m_multiplayer,s_multiplayer,l_multiplayer);
	COLUMN_ENTRY_LENGTH_STATUS(36,m_brokerName,s_brokerName,l_brokerName);
	COLUMN_ENTRY_LENGTH_STATUS(37,m_subBrokerName,s_subBrokerName,l_subBrokerName);
	COLUMN_ENTRY_LENGTH_STATUS(38,m_extraGroup,s_extraGroup,l_extraGroup);
	COLUMN_ENTRY_LENGTH_STATUS(39,m_subbrokerRatio,s_subbrokerRatio,l_subbrokerRatio);
	COLUMN_ENTRY_LENGTH_STATUS(40,m_brokerRatio,s_brokerRatio,l_brokerRatio);
	COLUMN_ENTRY_LENGTH_STATUS(41,m_companyRation,s_companyRation,l_companyRation);
	COLUMN_ENTRY_LENGTH_STATUS(42,m_clientBrokerageType,s_clientBrokerageType,l_clientBrokerageType);
	COLUMN_ENTRY_LENGTH_STATUS(43,m_clientBrokerage,s_clientBrokerage,l_clientBrokerage);
	COLUMN_ENTRY_LENGTH_STATUS(44,m_subBrokerBrokerage,s_subBrokerBrokerage,l_subBrokerBrokerage);
	COLUMN_ENTRY_LENGTH_STATUS(45,m_brokerBrokerage,s_brokerBrokerage,l_brokerBrokerage);
	COLUMN_ENTRY_LENGTH_STATUS(46,m_companyBrokerage,s_companyBrokerage,l_companyBrokerage);
	COLUMN_ENTRY_LENGTH_STATUS(47,m_maxAllowedVolume,s_maxAllowedVolume,l_maxAllowedVolume);
END_COLUMN_MAP()
	
};



class CBrokerpositionTable
{
public:
	TCHAR m_broker[20];
	TCHAR m_name[50];
	TCHAR m_symbol[32];
	double m_volume;
	double m_prevVol;
	double m_volDiff;
	double m_avg;	
	double m_lastRate;
	double m_clientnetamount;
	double m_clientnetTotal;
	double m_brokergrossAmount;
	double m_brokernetAmount;
		
	
	
	DBSTATUS s_broker;
	DBSTATUS s_name;
	DBSTATUS s_symbol;
	DBSTATUS s_volume;
	DBSTATUS s_prevVol;
	DBSTATUS s_volDiff;
	DBSTATUS s_avg;	
	DBSTATUS s_lastRate;
	DBSTATUS s_clientnetamount;
	DBSTATUS s_clientnetTotal;
	DBSTATUS s_brokergrossAmount;
	DBSTATUS s_brokernetAmount;


	DBLENGTH    l_broker;
	DBLENGTH    l_name;
	DBLENGTH    l_symbol;
	DBLENGTH 	l_volume;
	DBLENGTH 	l_prevVol;
	DBLENGTH 	l_volDiff;
	DBLENGTH 	l_avg;
	DBLENGTH 	l_lastRate;
	DBLENGTH 	l_clientnetamount;
	DBLENGTH 	l_clientnetTotal;
	DBLENGTH 	l_brokergrossAmount;
	DBLENGTH 	l_brokernetAmount;
// Column binding map
BEGIN_COLUMN_MAP(CBrokerpositionTable)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_broker,s_broker,l_broker);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_name,s_name,l_name);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_prevVol,s_prevVol,l_prevVol);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_volDiff,s_volDiff,l_volDiff);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_volume,s_volume,l_volume);	
	COLUMN_ENTRY_LENGTH_STATUS(7,m_avg,s_avg,l_avg);
	
	COLUMN_ENTRY_LENGTH_STATUS(8,m_lastRate,s_lastRate,l_lastRate);

	COLUMN_ENTRY_LENGTH_STATUS(9,m_clientnetamount,s_clientnetamount,l_clientnetamount);	
	COLUMN_ENTRY_LENGTH_STATUS(10,m_clientnetTotal,s_clientnetTotal,l_clientnetTotal);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_brokergrossAmount,s_brokergrossAmount,l_brokergrossAmount);
	COLUMN_ENTRY_LENGTH_STATUS(12,m_brokernetAmount,s_brokernetAmount,l_brokernetAmount);
	
END_COLUMN_MAP()
	
};

class CBalanceDistributionTable
{
public:
	TCHAR m_login[30];
	TCHAR m_subBroker[30];
	TCHAR m_broker[30];
	TCHAR m_symbol[32];
	double m_subbrokerRatio;
	double m_brokerratio;
	double m_companyRatio;
	double m_clientNetAmount;
	double m_subbrokerNetAmount;
	double m_clientGrossAmount;
	double m_companyNetTotal;
		
	
	
	DBSTATUS  s_login;
	DBSTATUS  s_subBroker;
	DBSTATUS  s_broker;
	DBSTATUS  s_symbol;
	DBSTATUS  s_subbrokerRatio;
	DBSTATUS  s_brokerratio;
	DBSTATUS  s_companyRatio;
	DBSTATUS  s_clientNetAmount;
	DBSTATUS  s_subbrokerNetAmount;
	DBSTATUS  s_clientGrossAmount;
	DBSTATUS  s_companyNetTotal;


	DBLENGTH  l_login;
	DBLENGTH  l_subBroker;
	DBLENGTH  l_broker;
	DBLENGTH  l_symbol;
	DBLENGTH  l_subbrokerRatio;
	DBLENGTH  l_brokerratio;
	DBLENGTH  l_companyRatio;
	DBLENGTH  l_clientNetAmount;
	DBLENGTH  l_subbrokerNetAmount;
	DBLENGTH  l_clientGrossAmount;
	DBLENGTH  l_companyNetTotal;
// Column binding map
BEGIN_COLUMN_MAP(CBalanceDistributionTable)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_subBroker,s_subBroker,l_subBroker);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_broker,s_broker,l_broker);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_subbrokerRatio,s_subbrokerRatio,l_subbrokerRatio);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_brokerratio,s_brokerratio,l_brokerratio);	
	COLUMN_ENTRY_LENGTH_STATUS(7,m_companyRatio,s_companyRatio,l_companyRatio);	
	COLUMN_ENTRY_LENGTH_STATUS(8,m_clientNetAmount,s_clientNetAmount,l_clientNetAmount);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_subbrokerNetAmount,s_subbrokerNetAmount,l_subbrokerNetAmount);	
	COLUMN_ENTRY_LENGTH_STATUS(10,m_clientGrossAmount,s_clientGrossAmount,l_clientGrossAmount);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_companyNetTotal,s_companyNetTotal,l_companyNetTotal);	
	
END_COLUMN_MAP()
	
};



class CGrossExposureDistributionTable
{
public:
	TCHAR m_login[30];
	TCHAR m_subBroker[30];
	TCHAR m_broker[30];
	TCHAR m_symbol[32];
	double m_subbrokerRatio;
	double m_brokerratio;
	double m_companyRatio;
	double m_volume;	
	double m_clientExposure;
	double m_subbrokerExposure;
	double m_brokerExposure;
	double m_companyExposure;
		
	
	
	DBSTATUS  s_login;
	DBSTATUS  s_subBroker;
	DBSTATUS  s_broker;
	DBSTATUS  s_symbol;
	DBSTATUS  s_subbrokerRatio;
	DBSTATUS  s_brokerratio;
	DBSTATUS  s_companyRatio;
	DBSTATUS  s_volume;	
	DBSTATUS  s_clientExposure;
	DBSTATUS  s_subbrokerExposure;
	DBSTATUS  s_brokerExposure;
	DBSTATUS  s_companyExposure;


	DBLENGTH  l_login;
	DBLENGTH  l_subBroker;
	DBLENGTH  l_broker;
	DBLENGTH  l_symbol;
	DBLENGTH  l_subbrokerRatio;
	DBLENGTH  l_brokerratio;
	DBLENGTH  l_companyRatio;
	DBLENGTH  l_volume;	
	DBLENGTH  l_clientExposure;
	DBLENGTH  l_subbrokerExposure;
	DBLENGTH  l_brokerExposure;
	DBLENGTH  l_companyExposure;
// Column binding map
BEGIN_COLUMN_MAP(CGrossExposureDistributionTable)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_subBroker,s_subBroker,l_subBroker);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_broker,s_broker,l_broker);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_subbrokerRatio,s_subbrokerRatio,l_subbrokerRatio);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_brokerratio,s_brokerratio,l_brokerratio);	
	COLUMN_ENTRY_LENGTH_STATUS(7,m_companyRatio,s_companyRatio,l_companyRatio);	
	COLUMN_ENTRY_LENGTH_STATUS(8,m_volume,s_volume,l_volume);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_clientExposure,s_clientExposure,l_clientExposure);	
	COLUMN_ENTRY_LENGTH_STATUS(10,m_subbrokerExposure,s_subbrokerExposure,l_subbrokerExposure);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_brokerExposure,s_brokerExposure,l_brokerExposure);	
	COLUMN_ENTRY_LENGTH_STATUS(12,m_companyExposure,s_companyExposure,l_companyExposure);		
END_COLUMN_MAP()
	
};


class CBrokerageDistributionTable
{
public:
	TCHAR m_login[30];
	TCHAR m_subBroker[30];
	TCHAR m_broker[30];
	TCHAR m_symbol[32];
	TCHAR m_brokarageType[30];
	double m_clientBrokarageRate;
	double m_subbrokerBrokarageRate;
	double m_brokerBrokarageRate;
	double m_companyBrokarageRate;
	double m_clientBrokarage;
	double m_subbrokerBrokarage;
	double m_brokerBrokarage;
	double m_companyBrokarage;
		
	
	
	DBSTATUS  s_login;
	DBSTATUS  s_subBroker;
	DBSTATUS  s_broker;
	DBSTATUS  s_symbol;
	DBSTATUS  s_brokarageType;
	DBSTATUS  s_clientBrokarageRate;
	DBSTATUS  s_subbrokerBrokarageRate;
	DBSTATUS  s_brokerBrokarageRate;
	DBSTATUS  s_companyBrokarageRate;
	DBSTATUS  s_clientBrokarage;
	DBSTATUS  s_subbrokerBrokarage;
	DBSTATUS  s_brokerBrokarage;
	DBSTATUS  s_companyBrokarage;

	DBLENGTH  l_login;
	DBLENGTH  l_subBroker;
	DBLENGTH  l_broker;
	DBLENGTH  l_symbol;
	DBLENGTH  l_brokarageType;
	DBLENGTH  l_clientBrokarageRate;
	DBLENGTH  l_subbrokerBrokarageRate;
	DBLENGTH  l_brokerBrokarageRate;
	DBLENGTH  l_companyBrokarageRate;
	DBLENGTH  l_clientBrokarage;
	DBLENGTH  l_subbrokerBrokarage;
	DBLENGTH  l_brokerBrokarage;
	DBLENGTH  l_companyBrokarage;
// Column binding map
BEGIN_COLUMN_MAP(CBrokerageDistributionTable)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_subBroker,s_subBroker,l_subBroker);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_broker,s_broker,l_broker);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_brokarageType,s_brokarageType,l_brokarageType);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_clientBrokarageRate,s_clientBrokarageRate,l_clientBrokarageRate);	
	COLUMN_ENTRY_LENGTH_STATUS(7,m_subbrokerBrokarageRate,s_subbrokerBrokarageRate,l_subbrokerBrokarageRate);	
	COLUMN_ENTRY_LENGTH_STATUS(8,m_brokerBrokarageRate,s_brokerBrokarageRate,l_brokerBrokarageRate);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_companyBrokarageRate,s_companyBrokarageRate,l_companyBrokarageRate);	
	COLUMN_ENTRY_LENGTH_STATUS(10,m_clientBrokarage,s_clientBrokarage,l_clientBrokarage);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_subbrokerBrokarage,s_subbrokerBrokarage,l_subbrokerBrokarage);	
	COLUMN_ENTRY_LENGTH_STATUS(12,m_brokerBrokarage,s_brokerBrokarage,l_brokerBrokarage);		
	COLUMN_ENTRY_LENGTH_STATUS(13,m_companyBrokarage,s_companyBrokarage,l_companyBrokarage);		
END_COLUMN_MAP()
	
};

class CClientNetPositionTable
{
public:
	TCHAR m_login[30];
	TCHAR m_name[50];
	TCHAR m_symbol[32];
	double m_volume;
	double m_previousVolume;
	double m_difference;
	double m_average;
	double m_lastRate;
	double m_clientnetAmount;
	double m_clientnettotal;
	double m_maxallotedlots;
	TCHAR  m_subbroker[30];
	TCHAR  m_broker[30];
	double m_clientExposure;
	double m_extravolume;
	double m_freemargin;
		

	DBSTATUS  s_login;
	DBSTATUS  s_name;
	DBSTATUS  s_symbol;
	DBSTATUS  s_volume;
	DBSTATUS  s_previousVolume;
	DBSTATUS  s_difference;
	DBSTATUS  s_average;
	DBSTATUS  s_lastRate;
	DBSTATUS  s_clientnetAmount;
	DBSTATUS  s_clientnettotal;
	DBSTATUS  s_maxallotedlots;
	DBSTATUS  s_subbroker;
	DBSTATUS  s_broker;
	DBSTATUS  s_clientExposure;
	DBSTATUS  s_extravolume;
	DBSTATUS  s_freemargin;

	DBLENGTH   l_login;
	DBLENGTH   l_name;
	DBLENGTH   l_symbol;
	DBLENGTH   l_volume;
	DBLENGTH   l_previousVolume;
	DBLENGTH   l_difference;
	DBLENGTH   l_average;
	DBLENGTH   l_lastRate;
	DBLENGTH   l_clientnetAmount;
	DBLENGTH   l_clientnettotal;
	DBLENGTH   l_maxallotedlots;
	DBLENGTH   l_subbroker;
	DBLENGTH   l_broker;
	DBLENGTH   l_clientExposure;
	DBLENGTH   l_extravolume;
	DBLENGTH   l_freemargin;
// Column binding map
BEGIN_COLUMN_MAP(CClientNetPositionTable)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_name,s_name,l_name);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_volume,s_volume,l_volume);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_previousVolume,s_previousVolume,l_previousVolume);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_difference,s_difference,l_difference);	
	COLUMN_ENTRY_LENGTH_STATUS(7,m_average,s_average,l_average);	
	COLUMN_ENTRY_LENGTH_STATUS(8,m_lastRate,s_lastRate,l_lastRate);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_clientnetAmount,s_clientnetAmount,l_clientnetAmount);	
	COLUMN_ENTRY_LENGTH_STATUS(10,m_clientnettotal,s_clientnettotal,l_clientnettotal);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_maxallotedlots,s_maxallotedlots,l_maxallotedlots);	
	COLUMN_ENTRY_LENGTH_STATUS(12,m_subbroker,s_subbroker,l_subbroker);	
	COLUMN_ENTRY_LENGTH_STATUS(13,m_broker,s_broker,l_broker);	
	COLUMN_ENTRY_LENGTH_STATUS(14,m_clientExposure,s_clientExposure,l_clientExposure);	
	COLUMN_ENTRY_LENGTH_STATUS(15,m_extravolume,s_extravolume,l_extravolume);	
	COLUMN_ENTRY_LENGTH_STATUS(16,m_freemargin,s_freemargin,l_freemargin);		
END_COLUMN_MAP()	
};



class CComment
{
public:
	int   m_deal;
	TCHAR m_comment[100];			
	DBSTATUS  s_deal;	
	DBSTATUS  s_comment;	
	DBLENGTH   l_deal;	
	DBLENGTH   l_comment;	
BEGIN_COLUMN_MAP(CComment)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_deal,s_deal,l_deal);	
	COLUMN_ENTRY_LENGTH_STATUS(2,m_comment,s_comment,l_comment);	
END_COLUMN_MAP()	



};









class CNetpositionPreQtyTable
{
public:
	TCHAR m_login[20];
	TCHAR m_symbol[32];
	double m_volume;
	
				
	DBSTATUS s_login;	
	DBSTATUS s_symbol;
	DBSTATUS s_volume;



	DBLENGTH    l_login;
	DBLENGTH    l_symbol;
	DBLENGTH 	l_volume;
	
// Column binding map
BEGIN_COLUMN_MAP(CNetpositionPreQtyTable)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);	
	COLUMN_ENTRY_LENGTH_STATUS(2,m_symbol,s_symbol,l_symbol);	
	COLUMN_ENTRY_LENGTH_STATUS(3,m_volume,s_volume,l_volume);	
	
END_COLUMN_MAP()
	
};





class CClientWiseGrossTotal
{
public:
	TCHAR m_login[20];
	double m_totalGrossamount;
	double m_totalGrossBrokerage;
	
	
			 	
	DBSTATUS s_login;
	DBSTATUS s_totalGrossBrokerage;
	DBSTATUS s_totalGrossamount;


	DBLENGTH    l_login;
	DBLENGTH    l_totalGrossBrokerage;
	DBLENGTH 	l_totalGrossamount;
	
// Column binding map
BEGIN_COLUMN_MAP(CClientWiseGrossTotal)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);	
	COLUMN_ENTRY_LENGTH_STATUS(2,m_totalGrossamount,s_totalGrossamount,l_totalGrossamount);	
	COLUMN_ENTRY_LENGTH_STATUS(3,m_totalGrossBrokerage,s_totalGrossBrokerage,l_totalGrossBrokerage);		
	
END_COLUMN_MAP()
	
};


class CTelplateIDTable
{
public:
	int m_templateID;
	DBSTATUS s_templateID;	
	DBLENGTH    l_templateID;	
	// Column binding map
	BEGIN_COLUMN_MAP(CTelplateIDTable)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_templateID, s_templateID, l_templateID);
	END_COLUMN_MAP()
};




#endif