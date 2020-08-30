// StorageManager.cpp: implementation of the CStorageManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MAP00_
//#include "[lib]yhlibrary/YHLibrary.h"
#include "[lib]yhlibrary/HashTable.h"
#include "[lib]yhlibrary/PtrList.h"
//#include "[lib]yhlibrary/cLooseLinkedList.h"
#include "[lib]yhlibrary/cLinkedList.h"
#include "[lib]yhlibrary/IndexGenerator.h"
//#include "[lib]yhlibrary/cConstLinkedList.h"
//#include "[CC]Header/vector.h"
#include "[CC]Header/protocol.h"
#include "[CC]Header/CommonDefine.h"
#include "[CC]Header/CommonGameDefine.h"
#include "[CC]Header/ServerGameDefine.h"
#include "[CC]Header/CommonStruct.h"
#include "[CC]Header/ServerGameStruct.h"
//#include "[CC]Header/CommonGameFunc.h"
#include "[CC]Header/GameResourceStruct.h"
//#include "[CC]Header/CommonCalcFunc.h"
//#include "[CC]ServerModule/DataBase.h"
//#include "[CC]ServerModule/Console.h"
//#include "Object.h"
//#include "ServerSystem.h"
#endif

#include "StorageManager.h"
#include "[CC]ServerModule/DataBase.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "UserTable.h"
#include "Network.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStorageManager::CStorageManager()
{
}

CStorageManager::~CStorageManager()
{
}

void CStorageManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_STORAGE_BUY_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;
			
			BuyStorageSyn(pPlayer);
		}
		break;
	case MP_STORAGE_PUTIN_MONEY_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;

			PutInMoneyStorage(pPlayer, (DWORD)pmsg->dwData);
		}
		break;
	case MP_STORAGE_PUTOUT_MONEY_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)	return;

			PutOutMoneyStorage(pPlayer, (DWORD)pmsg->dwData);
		}
		break;
// --- skr : warehouse
	case MP_STORAGE_WAREHOUSE_SYN :
	{
		MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
		if(!pPlayer)	return;
		ChangeWarehouseSet( pPlayer, (DWORD)pmsg->dwData );
	}
	break;
		
	}

}

void CStorageManager::LoadStorageList()
{
	CMHFile file;
	if(file.Init("./System/Resource/StorageListInfo.bin","rb") == FALSE)
		return;
	
	int count = 0;
	for(int n=0;n<MAX_STORAGELIST_NUM;++n)
	{
		m_StorageListInfo[count].MaxCellNum = BYTE( 30*(count+1) );
		m_StorageListInfo[count].MaxMoney = file.GetDword();
		m_StorageListInfo[count].BuyPrice = file.GetDword();
		
		++count;
	}
	
	file.Release();

}

void CStorageManager::SendStorageItemInfo(CPlayer* pPlayer)
{
	SEND_STORAGE_ITEM_INFO StorageItem;
	memset(&StorageItem, 0, sizeof(SEND_STORAGE_ITEM_INFO));
	StorageItem.Category = MP_ITEM;
	StorageItem.Protocol = MP_ITEM_STORAGE_ITEM_INFO_ACK;

	// 081010 LUJ, ¿É¼ÇÀ» Àü¼ÛÇÒ ±¸Á¶Ã¼ ¼±¾ð
	MSG_ITEM_OPTION optionMessage;
	ZeroMemory( &optionMessage, sizeof( optionMessage ) );
	optionMessage.Category	= MP_ITEM;
	optionMessage.Protocol	= MP_ITEM_OPTION_ADD_ACK;

	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Storage);
	StorageItem.money = pSlot->GetPurse()->GetPurseCurMoney();
	
	POSTYPE StartPos = TP_STORAGE_START;
	
	for(POSTYPE i=0;i<SLOT_STORAGE_NUM;++i)
	{
		if( pSlot->IsEmpty(StartPos+i) == FALSE )
		{
			 StorageItem.StorageItem[i]= *pSlot->GetItemInfoAbs( POSTYPE( StartPos+i ) );

			ITEMBASE& item = StorageItem.StorageItem[i];

			const ITEM_OPTION& option = ITEMMGR->GetOption( item );

			if( option.mItemDbIndex )
			{
				optionMessage.mOption[ optionMessage.mSize++ ] = option;
			}

			const DWORD maxOptionSize = ( sizeof( optionMessage.mOption ) / sizeof( *( optionMessage.mOption ) ) );

			// 081010 LUJ, Ã¢°í ½½·ÔÀÌ ¿É¼Ç ¸Þ½ÃÁö ¼öº¸´Ù Å©¹Ç·Î ÃÖ´ë °³¼ö¿¡ µµ´ÞÇßÀ» ¶§ Àü¼Û½ÃÅ°°í ÀçÀü¼ÛÇÒ ÁØºñ¸¦ ÇÑ´Ù
			if( maxOptionSize == optionMessage.mSize )
			{
				pPlayer->SendMsg( &optionMessage, optionMessage.GetSize() );
				ZeroMemory( &optionMessage.mOption, sizeof( optionMessage.mOption ) );

				optionMessage.mSize = 0;
			}
		}
	}
	
	// 081010 LUJ, ³²¾ÆÀÖ´Â ¿É¼ÇÀ» Àü¼ÛÇÑ´Ù
	if( optionMessage.mSize )
	{
		pPlayer->SendMsg( &optionMessage, optionMessage.GetSize() );
	}

	pPlayer->SendMsg(&StorageItem, StorageItem.GetSize());
}

void CStorageManager::BuyStorageSyn(CPlayer* pPlayer)
{
	WORD storagenum = pPlayer->GetStorageNum();

	STORAGELISTINFO* pBuyStorageInfo = GetStorageInfo(storagenum+1);

	if( storagenum >= TAB_BASE_STORAGE_NUM || pPlayer->GetMoney() < pBuyStorageInfo->BuyPrice )
	{
		MSGBASE msg;
		msg.Category = MP_STORAGE;
		msg.Protocol = MP_STORAGE_BUY_NACK;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	else
	{
		pPlayer->SetMoney(pBuyStorageInfo->BuyPrice, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseStorageBuy, 0);
		pPlayer->SetStorageNum( BYTE( storagenum+1 ) );
		pPlayer->SetMaxPurseMoney(eItemTable_Storage, GetStorageInfo(pPlayer->GetStorageNum())->MaxMoney);
		StorageBuyStorage(pPlayer->GetID()); //db update		
		
		MSG_BYTE msg;
		msg.Category = MP_STORAGE;
		msg.Protocol = MP_STORAGE_BUY_ACK;
		msg.bData	= BYTE(storagenum+1);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}

void CStorageManager::PutInMoneyStorage(CPlayer* pPlayer, MONEYTYPE setMoney)
{
	MONEYTYPE storagemon = pPlayer->GetMoney(eItemTable_Storage);
	MONEYTYPE maxmon = pPlayer->GetMaxPurseMoney(eItemTable_Storage);
	
	if(pPlayer->GetMoney() < setMoney)
	{
		setMoney = pPlayer->GetMoney();
	}
	if( maxmon - storagemon < setMoney )
	{
		setMoney = maxmon - storagemon;
	}	
	
	if(setMoney == 0)
	{
		MSGBASE msg;
		msg.Category = MP_STORAGE;
		msg.Protocol = MP_STORAGE_PUTIN_MONEY_NACK;
		
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
		
	setMoney = pPlayer->SetMoney(setMoney, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseStorage, pPlayer->GetUserID());
	if(setMoney == 0)
	{
		ASSERTMSG(0, "Must Check!");
		return;
	}
	pPlayer->SetMoney(setMoney, MONEY_ADDITION, 0, eItemTable_Storage);
	

	StorageMoneyUpdateToDB(pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage), pPlayer->GetID());	//dbUpdate

	InsertLogMoney( eMoneyLog_LoseStorage, pPlayer->GetID(), pPlayer->GetMoney(), setMoney, pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage) );
	
	//YH
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetUserID(), "#Storage",
				eLog_ItemMoveInvenToStorage, pPlayer->GetMoney(), pPlayer->GetMoney(eItemTable_Storage),
				setMoney, 0, 0, 0, 0, 0, pPlayer->GetPlayerExpPoint());

	MSG_DWORD msg;
	msg.Category = MP_STORAGE;
	msg.Protocol = MP_STORAGE_PUTIN_MONEY_ACK;
	msg.dwData	= pPlayer->GetMoney(eItemTable_Storage);

	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CStorageManager::PutOutMoneyStorage(CPlayer* pPlayer, MONEYTYPE getMoney)
{
	DWORDEX getmoney = getMoney;
	DWORDEX storagemon = pPlayer->GetMoney(eItemTable_Storage);
	DWORDEX maxmon = pPlayer->GetMaxPurseMoney(eItemTable_Inventory); //ÀÎº¥Åä¸®·Î µ· ¿Å±â´Ï±î.
	
	if( storagemon < getmoney)
	{
		getmoney = storagemon;
	}
	
	if(getmoney + pPlayer->GetMoney() > MAX_INVENTORY_MONEY)
	{
		MSG_DWORD msg;
		msg.Category = MP_STORAGE;
		msg.Protocol = MP_STORAGE_PUTOUT_MONEY_NACK;
		msg.dwData	= pPlayer->GetMoney(eItemTable_Storage);

		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	else if(getmoney + pPlayer->GetMoney() > maxmon)
	{
		getmoney = maxmon - pPlayer->GetMoney();
	}

	if(getmoney == 0)
		return;
	getmoney = pPlayer->SetMoney(MONEYTYPE(getmoney), MONEY_SUBTRACTION, 0, eItemTable_Storage);
	if(getmoney == 0) //purse¿¡¼­ Á¤¸» ºüÁ³´ÂÁö 
	{
		ASSERTMSG(0, "Must Check!");
		return;
	}
	pPlayer->SetMoney(MONEYTYPE(getmoney), MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetStorage, pPlayer->GetUserID());
	

	StorageMoneyUpdateToDB(pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage), pPlayer->GetID());	//dbUpdate
	
	InsertLogMoney( eMoneyLog_GetStorage, pPlayer->GetID(), pPlayer->GetMoney(), MONEYTYPE(getmoney), pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage) );

	//YH
	LogItemMoney( pPlayer->GetUserID(), "#Storage", pPlayer->GetID(), pPlayer->GetObjectName(), 
				eLog_ItemMoveStorageToInven, pPlayer->GetMoney(eItemTable_Storage), pPlayer->GetMoney(),
				MONEYTYPE(getmoney), 0, 0, 0, 0, 0, pPlayer->GetPlayerExpPoint());

	MSG_DWORD msg;
	msg.Category = MP_STORAGE;
	msg.Protocol = MP_STORAGE_PUTOUT_MONEY_ACK;
	msg.dwData	= pPlayer->GetMoney(eItemTable_Storage);

	pPlayer->SendMsg(&msg, sizeof(msg));
}

STORAGELISTINFO* CStorageManager::GetStorageInfo(int i)
{
	ASSERT(i>0);
	//ASSERT(i<=MAX_STORAGELIST_NUM);
	ASSERT(i<=MAX_STORAGELIST_NUMSET4);
	if( i <= MAX_STORAGELIST_NUM ){
		return &m_StorageListInfo[i-1]; 
	}
	else {
		return &m_StorageListInfo[MAX_STORAGELIST_NUM-1]; 
	}
}
// --- skr : warehouse 2020/agt/27
void CStorageManager::ChangeWarehouseSet( CPlayer* pPlayer, DWORD dwdata )
{
	ITEMBASE* aItem = NULL;
	
	POSTYPE startpos = 0, endpos = 0, i = 0;
	DWORD dwstartpos = startpos, dwendpos = endpos;
	DWORD dwnum = (DWORD)pPlayer->GetStorageNum();
	DWORD currentset = pPlayer->GetWarehouseSet();
	CItemSlot* pslot = pPlayer->GetSlot( eItemTable_Storage );
	MSG_DWORD msg;
	msg.dwData = pPlayer->GetID();
	msg.Category = MP_STORAGE;
	if( dwdata > dwnum )
	{
		msg.Protocol = MP_STORAGE_WAREHOUSE_NACK;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if( !pslot )
	{
		msg.Protocol = MP_STORAGE_WAREHOUSE_NACK;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	pPlayer->SetWarehouseSet(dwdata);
	pPlayer->GetWarehouseStartEnd( dwstartpos, dwendpos );
	startpos = dwstartpos;
	endpos = dwendpos;
	if ( startpos == 0 || endpos == 0)
	{
		pPlayer->SetWarehouseSet(currentset);
		msg.Protocol = MP_STORAGE_WAREHOUSE_NACK;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
		
	msg.Protocol = MP_STORAGE_WAREHOUSE_ACK;
	pPlayer->SendMsg(&msg, sizeof(msg));
	
	for( i = startpos; i < endpos; i++)
	{
		aItem = pslot->GetItemInfoAbs( i );
		if( aItem != NULL ){
			ITEMMGR->RemoveOption( aItem->dwDBIdx );
		}
		pslot->ClearItemBaseAndSlotInfo( i );
	}
	WarehouseItemInfoSet( pPlayer->GetID(), pPlayer->GetUserID(), 0, startpos - 1,endpos, dwdata );
}