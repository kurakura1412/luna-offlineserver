// StorageManager.h: interface for the CStorageManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STORAGEMANAGER_H__A316F76A_DB8B_4009_B69E_ECF2DCD2F0EC__INCLUDED_)
#define AFX_STORAGEMANAGER_H__A316F76A_DB8B_4009_B69E_ECF2DCD2F0EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct STORAGELISTINFO
{
	BYTE MaxCellNum;
	MONEYTYPE MaxMoney;
	MONEYTYPE BuyPrice;
};

#define STORAGEMGR CStorageManager::GetInstance()
class CStorageManager
{
	CStorageManager();
	STORAGELISTINFO m_StorageListInfo[MAX_STORAGELIST_NUM];
public:
	GETINSTANCE(CStorageManager);
	virtual ~CStorageManager();
	
	
	STORAGELISTINFO* GetStorageInfo(int i); //i°³ ÀÏ ¶§ÀÇ Á¤º¸¸¦ ¸®ÅÏ
	
	void LoadStorageList();
	void SendStorageItemInfo(CPlayer* pPlayer);
	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	void BuyStorageSyn(CPlayer* pPlayer);
	void PutInMoneyStorage(CPlayer* pPlayer, MONEYTYPE Money);
	void PutOutMoneyStorage(CPlayer* pPlayer, MONEYTYPE Money);
// --- skr : warehouse
	void ChangeWarehouseSet( CPlayer* pPlayer, DWORD dwdata );
	
};

#endif // !defined(AFX_STORAGEMANAGER_H__A316F76A_DB8B_4009_B69E_ECF2DCD2F0EC__INCLUDED_)
