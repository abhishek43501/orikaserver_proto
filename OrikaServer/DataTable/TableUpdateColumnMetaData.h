//#include "stdafx.h"
#ifndef		_TableUpdateColumnMetaData
#define		_TableUpdateColumnMetaData
#include <atldbcli.h>
class CTableUpdateColumnMetaData
{
public:
	TCHAR m_ColumnKey[40];
	TCHAR m_TabName[40];
	TCHAR m_SqlTable[50];
	TCHAR m_SqlTableColumn[50];
	TCHAR m_MTModuleName[50];
	TCHAR m_MtColumnName[50];
	int m_DataIndexForUpdate;
	TCHAR m_DataSeprator[1];



	DBSTATUS s_ColumnKey;
	DBSTATUS s_TabName;
	DBSTATUS s_SqlTable;
	DBSTATUS s_SqlTableColumn;
	DBSTATUS s_MTModuleName;
	DBSTATUS s_MtColumnName;
	DBSTATUS s_DataIndexForUpdate;
	DBSTATUS s_DataSeprator;


	DBLENGTH l_ColumnKey;
	DBLENGTH l_TabName;
	DBLENGTH l_SqlTable;
	DBLENGTH l_SqlTableColumn;
	DBLENGTH l_MTModuleName;
	DBLENGTH l_MtColumnName;
	DBLENGTH l_DataIndexForUpdate;
	DBLENGTH l_DataSeprator;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableUpdateColumnMetaData)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_ColumnKey, s_ColumnKey, l_ColumnKey);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_TabName, s_TabName, l_TabName);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_SqlTable, s_SqlTable, l_SqlTable);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_SqlTableColumn, s_SqlTableColumn, l_SqlTableColumn);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_MTModuleName, s_MTModuleName, l_MTModuleName);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_MtColumnName, s_MtColumnName, l_MtColumnName);
		COLUMN_ENTRY_LENGTH_STATUS(7, m_DataIndexForUpdate, s_DataIndexForUpdate, l_DataIndexForUpdate);
		COLUMN_ENTRY_LENGTH_STATUS(8, m_DataSeprator, s_DataSeprator, l_DataSeprator);
	END_COLUMN_MAP()

};


class CTableauthKey
{
public:
	TCHAR   m_loginUser[20];
	TCHAR   m_spreadSheetId[300];
	TCHAR   m_authKey[300];

	DBSTATUS s_loginUser;
	DBSTATUS s_spreadSheetId;
	DBSTATUS s_authKey;

	DBLENGTH l_loginUser;
	DBLENGTH l_spreadSheetId;
	DBLENGTH l_authKey;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableauthKey)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_loginUser, s_loginUser, l_loginUser);
	COLUMN_ENTRY_LENGTH_STATUS(2, m_spreadSheetId, s_spreadSheetId, l_spreadSheetId);
	COLUMN_ENTRY_LENGTH_STATUS(2, m_authKey, s_authKey, l_authKey);
	END_COLUMN_MAP()

};

class CTableStringCol
{
public:
	TCHAR m_colData[40];	
	DBSTATUS s_colData;
	DBLENGTH l_colData;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableStringCol)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_colData, s_colData, l_colData);
	END_COLUMN_MAP()

};



class CTableColumnTemplate
{
public:
	TCHAR m_template_name[40];
	TCHAR m_userlogin[20];
	int m_templetID;
	int m_sequence;
	TCHAR m_color[40];
	TCHAR m_value[40];
	TCHAR m_fontColor[40];
	TCHAR m_colorDark[40];
	TCHAR m_fontColorDark[40];




	DBSTATUS s_template_name;
	DBSTATUS s_userlogin;
	DBSTATUS s_templetID;
	DBSTATUS s_sequence;
	DBSTATUS s_color;
	DBSTATUS s_value;
	DBSTATUS s_fontColor;
	DBSTATUS s_colorDark;
	DBSTATUS s_fontColorDark;




	DBLENGTH l_template_name;
	DBLENGTH l_userlogin;
	DBLENGTH l_templetID;
	DBLENGTH l_sequence;
	DBLENGTH l_color;
	DBLENGTH l_value;
	DBLENGTH l_fontColor;
	DBLENGTH l_colorDark;
	DBLENGTH l_fontColorDark;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableColumnTemplate)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_template_name, s_template_name, l_template_name);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_userlogin, s_userlogin, l_userlogin);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_templetID, s_templetID, l_templetID);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_sequence, s_sequence, l_sequence);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_color, s_color, l_color);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_value, s_value, l_value);
		COLUMN_ENTRY_LENGTH_STATUS(7, m_fontColor, s_fontColor, l_fontColor);
		COLUMN_ENTRY_LENGTH_STATUS(8, m_colorDark, s_colorDark, l_colorDark);
		COLUMN_ENTRY_LENGTH_STATUS(9, m_fontColorDark, s_fontColorDark, l_fontColorDark);
	END_COLUMN_MAP()

};



class CTableColDesc
{
public:
	int   m_messagecode;
	TCHAR m_desc[255];

	DBSTATUS s_messagecode;
	DBSTATUS s_desc;

	DBLENGTH l_messagecode;
	DBLENGTH l_desc;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableColDesc)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_messagecode, s_messagecode, l_messagecode);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_desc, s_desc, l_desc);
	END_COLUMN_MAP()

};
class CTableColumns
{
public:	
	TCHAR m_column[255];

	DBSTATUS s_column;	

	DBLENGTH l_column;	
	// Column binding map
	BEGIN_COLUMN_MAP(CTableColumns)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_column, s_column, l_column);
	END_COLUMN_MAP()

};






class CTableTradeQtyTOT
{
public:
	TCHAR m_login[15];
	TCHAR m_symbol[32];
	double m_TotalTradedLot;
	double m_TotalTradedTO;
	double m_TotalBuyLot;
	double m_TotalSellLot;
	double m_TotalButTO;
	double m_TotalSellTO;

	DBSTATUS s_login;
	DBSTATUS s_symbol;
	DBSTATUS s_TotalTradedVolume;
	DBSTATUS s_TotalTradedLot;
	DBSTATUS s_BuyBolume;
	DBSTATUS s_SellVolume;
	DBSTATUS s_ButTOT;
	DBSTATUS s_SellTOT;

	DBLENGTH l_login;
	DBLENGTH l_symbol;
	DBLENGTH l_TotalTradedVolume;
	DBLENGTH l_TotalTradedLot;
	DBLENGTH l_BuyBolume;
	DBLENGTH l_SellVolume;
	DBLENGTH l_ButTOT;
	DBLENGTH l_SellTOT;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableTradeQtyTOT)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_login, s_login, l_login);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_symbol, s_symbol, l_symbol);	
		COLUMN_ENTRY_LENGTH_STATUS(3, m_TotalTradedLot, s_TotalTradedVolume,l_TotalTradedVolume);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_TotalTradedTO,s_TotalTradedLot,l_TotalTradedLot);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_TotalBuyLot,s_BuyBolume,l_BuyBolume);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_TotalSellLot,s_SellVolume,l_SellVolume);
		COLUMN_ENTRY_LENGTH_STATUS(7, m_TotalButTO,s_ButTOT,l_ButTOT);
		COLUMN_ENTRY_LENGTH_STATUS(8, m_TotalSellTO,s_SellTOT,l_SellTOT);
	END_COLUMN_MAP()
};



class CTableOrderCount
{
public:
	TCHAR m_login[15];
	TCHAR m_symbol[32];
	int   m_Type;
	int   m_TotalOrder;
	int   m_TotalCancelledOrder;
	int   m_TotalExecutedOrder;

	DBSTATUS s_login;
	DBSTATUS s_symbol;
	DBSTATUS s_Type;
	DBSTATUS s_TotalOrder;
	DBSTATUS s_TotalCancelledOrder;
	DBSTATUS s_TotalExecutedOrder;

	DBLENGTH l_login;
	DBLENGTH l_symbol;
	DBLENGTH l_Type;
	DBLENGTH l_TotalOrder;
	DBLENGTH l_TotalCancelledOrder;
	DBLENGTH l_TotalExecutedOrder;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableOrderCount)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_login, s_login, l_login);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_symbol, s_symbol, l_symbol);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_Type,s_Type,l_Type);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_TotalOrder,s_TotalOrder,l_TotalOrder);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_TotalCancelledOrder,s_TotalCancelledOrder,l_TotalCancelledOrder);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_TotalExecutedOrder,s_TotalExecutedOrder,l_TotalExecutedOrder);
	END_COLUMN_MAP()
};



class CTableColumnsubscription
{
public:
	TCHAR m_userlogin[30];
	TCHAR m_ColumnKey[200];
	int   m_subscribe;
	TCHAR m_RequestKey[200];

	DBSTATUS s_userlogin;
	DBSTATUS s_ColumnKey;
	DBSTATUS s_subscribe;
	DBSTATUS s_RequestKey;

	DBLENGTH l_userlogin;
	DBLENGTH l_ColumnKey;
	DBLENGTH l_subscribe;
	DBLENGTH l_RequestKey;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableColumnsubscription)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_userlogin, s_userlogin, l_userlogin);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_ColumnKey, s_ColumnKey, l_ColumnKey);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_subscribe, s_subscribe, l_subscribe);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_RequestKey, s_RequestKey, l_RequestKey);
		END_COLUMN_MAP()
};





class CTableClientSymbolWiseLastTrade
{
public:
	TCHAR m_login[15];
	TCHAR m_symbol[32];
	int		m_volume;
	double   m_Price;
	int		m_Position;
	

	DBSTATUS s_login;
	DBSTATUS s_symbol;
	DBSTATUS s_volume;
	DBSTATUS s_Price;
	DBSTATUS s_Position;

	DBLENGTH l_login;
	DBLENGTH l_symbol;
	DBLENGTH l_volume;
	DBLENGTH l_Price;
	DBLENGTH l_Position;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableClientSymbolWiseLastTrade)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_login, s_login, l_login);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_symbol, s_symbol, l_symbol);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_volume, s_volume, l_volume);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_Price, s_Price, l_Price);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_Position, s_Position, l_Position);
	END_COLUMN_MAP()
};



class CTableBadTradeGoodTrade
{
public:
	TCHAR	m_login[15];
	TCHAR   m_symbol[32];
	int		m_TotalTrade;
	int     m_BadTrade;
	int		m_GoodTrade;
	int     m_badTradeIgnoringPosition;
	int     m_TradeIgnoringPosition;


	DBSTATUS s_login;
	DBSTATUS s_symbol;
	DBSTATUS s_TotalTrade;
	DBSTATUS s_BadTrade;
	DBSTATUS s_GoodTrade;
	DBSTATUS s_badTradeIgnoringPosition;
	DBSTATUS s_TradeIgnoringPosition;

	DBLENGTH l_login;
	DBLENGTH l_symbol;
	DBLENGTH l_TotalTrade;
	DBLENGTH l_BadTrade;
	DBLENGTH l_GoodTrade;
	DBLENGTH l_badTradeIgnoringPosition;
	DBLENGTH l_TradeIgnoringPosition;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableBadTradeGoodTrade)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_login, s_login, l_login);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_symbol, s_symbol, l_symbol);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_TotalTrade, s_TotalTrade, l_TotalTrade);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_BadTrade, s_BadTrade, l_BadTrade);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_GoodTrade, s_GoodTrade, l_GoodTrade);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_badTradeIgnoringPosition, s_badTradeIgnoringPosition, l_badTradeIgnoringPosition);
		COLUMN_ENTRY_LENGTH_STATUS(7, m_TradeIgnoringPosition, s_TradeIgnoringPosition, l_TradeIgnoringPosition);
	END_COLUMN_MAP()
};

#endif