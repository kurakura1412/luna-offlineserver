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
#include "[CC]Header/CommonGameFunc.h"
//#include "[CC]Header/GameResourceStruct.h"
#include "[CC]Header/CommonCalcFunc.h"
//#include "[CC]ServerModule/DataBase.h"
//#include "[CC]ServerModule/Console.h"
#include "Object.h"
#include "ServerSystem.h"
#endif
 
#include "..\[CC]Header\GameResourceManager.h"
#include "Player.h"
#include "PartyManager.h"
#include "Party.h"
#include "CharMove.h"
#include "PackedData.h"
#include "[CC]ServerModule/DataBase.h"
#include "MapDBMsgParser.h"
#include "ItemManager.h"
#include "[cc]skill/server/manager/skillmanager.h"
#include "ObjectStateManager.h"
#include "streetstallmanager.h"
#include "CharacterCalcManager.h"
#include "GuildManager.h"
#include "SkillTreeManager.h"
#include "GridSystem.h"
#include "GridTable.h"
#include "[cc]skill/Server/info/BuffSkillInfo.h"
#include "[cc]skill/Server/info/ActiveSkillInfo.h"
#include "Showdown/Battle_Showdown.h"
#include "StateMachinen.h"
#include "MHError.h"
#include "CQuestBase.h"
#include "Quest.h"
#include "TileManager.h"
#include "mhtimemanager.h"
#include "LootingManager.h"
#include "PKManager.h"
#include "QuestManager.h"
#include "StorageManager.h"
#include "UserTable.h"
#include "GuildFieldWarMgr.h"
#include "QuestMapMgr.h"
#include "GuildTournamentMgr.h"
#include "StreetStall.h"
#include "QuickManager.h"
#include "FishingManager.h"
#include "[CC]BattleSystem/BattleSystem_Server.h"
#include "[cc]skill/server/tree/SkillTree.h"
#include "[CC]Skill\Server\Delay\Delay.h"
#include "AttackManager.h"
#include "hseos/Common/SHMath.h"
#include "hseos/Monstermeter/SHMonstermeterManager.h"
#include "hseos/Farm/SHFarmManager.h"
#include "hseos/Family/SHFamilyManager.h"
#include "hseos/Date/SHDateManager.h"
#include "[cc]skill/server/Object\BuffSkillObject.h"
#include "[cc]skill/server/Object/ActiveSkillObject.h"
#include "Guild.h"
#include "Network.h"
#include "MapNetworkMsgParser.h"
#include "Pet.h"
#include "petmanager.h"
#include "LimitDungeonMgr.h"
#include "SiegeWarfareMgr.h"
#include "[CC]SiegeDungeon/SiegeDungeonMgr.h"
#include "Vehicle.h"
#include "HousingMgr.h"
#include "Trigger\Manager.h"
#include "Trigger\Message.h"
#include "PCRoomManager.h"
#include "Dungeon\DungeonMgr.h"
#include "..\hseos\ResidentRegist\SHResidentRegistManager.h"
#include "..\[CC]ServerModule\Console.h"
#include "ServerTable.h"


// --- skr 12/01/2020
#include "Relife.h"


#define INSERTLOG_TIME			600000 // 10 min
#define INSERTLOG_TIME_CHINA	1800000 // 30 min
#define INSERTLOG_TIME_HK		1800000 // 30 min

extern int	g_nHackCheckNum;
extern int	g_nHackCheckWriteNum;

CPlayer::CPlayer() :
m_SkillTree(new cSkillTree)
{
	m_SkillFailCount = 0;
	mpBattleStateDelay = new cDelay;
	mpBattleStateDelay->Init( 10000 );

	m_God = FALSE;

	mIsEmergency = FALSE;

	mTargetMap = 0;
	mTargetPosX = 0;
	mTargetPosZ = 0;

	// 071128 LYW --- Player : HPMP ÃÃ»Â¿Ã«.
	m_byHP_Point = 0;
	m_byMP_Point = 0;

	m_bResetSkill	=	false;
	m_bResetStat	=	false;
	m_ItemArrayList.Initialize(10);
	m_FollowMonsterList.Initialize(5);
	m_QuestList.Initialize(30);
	m_dweFamilyRewardExp = 0;
	m_byCurFamilyMemCnt	 = 0;
	m_dwCurrentResurrectIndex = 0;
}

CPlayer::~CPlayer()
{
	delete mpBattleStateDelay;
	SAFE_DELETE(m_SkillTree);
}

void CPlayer::InitClearData()
{
	m_ItemContainer.Init();
	//m_ItemContainer.SetInit(eItemTable_Inventory,	TP_INVENTORY_START,		SLOT_INVENTORY_NUM,		&m_InventorySlot);
	m_ItemContainer.SetInit(eItemTable_Inventory,	TP_INVENTORY_START,		SLOT_MAX_INVENTORY_NUM,		&m_InventorySlot);

	m_ItemContainer.SetInit(eItemTable_Weared,		TP_WEAR_START,			SLOT_WEAR_NUM,			&m_WearSlot);
	m_ItemContainer.SetInit(eItemTable_Storage,		TP_STORAGE_START,		SLOT_STORAGE_NUM,		&m_StorageSlot);
	m_ItemContainer.SetInit(eItemTable_Shop,		TP_SHOPITEM_START,		SLOT_SHOPITEM_NUM,		&m_ShopItemSlot);	
	memset(&m_HeroCharacterInfo,0,sizeof(CHARACTER_TOTALINFO));
	memset(&m_HeroInfo,0,sizeof(HERO_TOTALINFO));
	// 090701 LUJ, Â¸ÃÂ¸Ã°Â¸Â®ÃÂ®ÃÃ Â»Ã½Â¼ÂºÃÃ/Â¼ÃÂ¸ÃªÃÃÂ¸Â¦ ÃÂ£ÃÃ¢ÃÃÃÃ¶ Â¾ÃÂ±Ã¢ Â¶Â§Â¹Â®Â¿Â¡ Init()Â¿Â¡Â¼Â­ ÃÃÂ±Ã¢ÃÂ­ÃÃÃÃ¶ Â¾ÃÃÂ» Â°Ã¦Â¿Ã¬, ÃÃÂ±Ã¢ÃÂ­Â°Â¡
	//		ÃÃÂ·Ã¯ÃÃ¶ÃÃ¶ Â¾ÃÂ´ÃÂ´Ã. purseÂ´Ã Â±Ã ÂµÂ¿Â¾Ã ÃÃÂ±Ã¢ÃÂ­Â¸Â¦ ÃÃÃÃ¶ Â¾ÃÂ¾ÃÃÂ¸Â¹ÃÂ·Ã ÃÃÂ·Â¹ÃÃÂ¾Ã®Â°Â¡ ÃÂ¢Â¼ÃÃÂ» ÃÃÃÂ¦ÃÃÂµÂµ ÃÂ¤ÂºÂ¸(Â¾Â²Â·Â¹Â±Ã¢Â°Âª)Â¸Â¦
	//		Â°Â®Â°Ã­ ÃÃÂ´Ã. ÃÃÂ¸Â¦ ÃÃÂ¿Ã«ÃÃÂ¼Â­ DBÂ¿Â¡Â¼Â­ Â°ÂªÃÂ» Â°Â¡ÃÂ®Â¿ÃÂ±Ã¢ ÃÃ¼Â¿Â¡ ÂºÃ¼Â¸Â£Â°Ã Â¸Ã ÃÃÂµÂ¿ÃÂ» Â¹ÃÂºÂ¹ÃÃ Â°Ã¦Â¿Ã¬, Â¾Â²Â·Â¹Â±Ã¢Â°ÂªÃÂ» DBÂ¿Â¡ ÃÃºÃÃ¥ÃÃÂ°ÃÂµÃÂ´Ã.
	//		ÃÃÂ¸Â¦ Â¸Â·Â±Ã¢ ÃÂ§ÃÃ Â°Â´ÃÂ¼ ÃÃÂ±Ã¢ÃÂ­ Â¼Ã¸Â°Â£Â¿Â¡ Â°ÂªÂµÂµ ÃÃÂ±Ã¢ÃÂ­Â½ÃÃÂ°ÂµÂµÂ·Ã ÃÃÂ´Ã
	m_InventoryPurse.SetZeroMoney();
	m_StoragePurse.SetZeroMoney();

	m_QuestGroup.Initialize( this );
	m_wKillMonsterLevel = 0;
	m_bGotWarehouseItems = FALSE;
	m_SkillFailCount = 0;
	mGravity = 0;

	memset( &m_DateMatching, 0, sizeof(DATE_MATCHING_INFO));			// ÂµÂ¥ÃÃÃÂ® Â¸ÃÃÂª ÃÃÂ¹ÃÃÃµ.
	// desc_hseos_ÃÃÂ¹ÃÂµÃ®Â·Ã01
	// S ÃÃÂ¹ÃÂµÃ®Â·Ã ÃÃÂ°Â¡ added by hseos 2007.06.09
	m_DateMatching.nSerchTimeTick = gCurTime;
	m_DateMatching.nRequestChatTimeTick = gCurTime;
	// E ÃÃÂ¹ÃÂµÃ®Â·Ã ÃÃÂ°Â¡ added by hseos 2007.06.09
	memset( &mPassiveStatus, 0, sizeof( Status ) );
	memset( &mBuffStatus, 0, sizeof( Status ) );
	memset( &mRatePassiveStatus, 0, sizeof( Status ) );
	memset( &mRateBuffStatus, 0, sizeof( Status ) );
	memset( &mAbnormalStatus, 0, sizeof( AbnormalStatus ) );

	// desc_hseos_Â¸Ã³Â½ÂºÃÃÂ¹ÃÃÃ01
	// S Â¸Ã³Â½ÂºÃÃÂ¹ÃÃÃ ÃÃÂ°Â¡ added by hseos 2007.05.23
	ZeroMemory(&m_stMonstermeterInfo, sizeof(m_stMonstermeterInfo));
	m_stMonstermeterInfo.nPlayTimeTick = gCurTime;
	m_pcFamilyEmblem = NULL;
	ZeroMemory(&m_stFarmInfo, sizeof(m_stFarmInfo));
	m_stFarmInfo.nCropPlantRetryTimeTick  = gCurTime;
	m_stFarmInfo.nCropManureRetryTimeTick = gCurTime;
	// 080519 KTH
	m_stFarmInfo.nAnimalFeedRetryTimeTick = gCurTime;
	m_stFarmInfo.nAnimalCleanRetryTimeTick = gCurTime;
	m_nChallengeZoneEnterFreq = 0;
	m_nChallengeZoneEnterBonusFreq = 0;
	m_nChallengeZonePartnerID = 0;
	m_nChallengeZoneSection = 0;
	m_nChallengeZoneStartState = 0;
	m_nChallengeZoneStartTimeTick = 0;
	m_nChallengeZoneMonsterNum = 0;
	m_nChallengeZoneKillMonsterNum = 0;
	m_bChallengeZoneNeedSaveEnterFreq = FALSE;
	m_bChallengeZoneCreateMonRightNow = FALSE;
	m_nChallengeZoneMonsterNumTillNow = 0;
	m_nChallengeZoneClearTime = 0;
	m_nChallengeZoneExpRate   = 0;

	m_God = FALSE;
	mIsEmergency = FALSE;

	mTargetMap = 0;
	mTargetPosX = 0;
	mTargetPosZ = 0;

	m_bResetSkill	=	false;
	m_bResetStat	=	false;

	m_dwReturnSkillMoney = 0;

//---KES AUTONOTE
	m_dwAutoNoteIdx = 0;
	m_dwLastActionTime = 0;
//---------------

	FishingInfoClear();
	m_dwFM_MissionCode = 1000000;

	int i;
	for(i=0; i<eFishItem_Max; i++)
	{
		m_fFishItemRate[i] = 0.0f;
	}
	m_lstGetFishList.clear();
	

	m_wFishingLevel = 1;
	m_dwFishingExp = 0;
	m_dwFishPoint = 0;
	mReviveFlag = ReviveFlagNone;
	m_dwKillCountFromGT = 0;
	m_dwKillPointFromGT = 0;
	m_dwRespawnTimeOnGTMAP = 0;
	m_dwImmortalTimeOnGTMAP = 0;
	m_wCookLevel = 1;
	m_wCookCount = 0;
	m_wEatCount = 0;
	m_wFireCount = 0;
	m_dwLastCookTime = 0;
	memset(m_MasterRecipe, 0, sizeof(m_MasterRecipe));
	// 090316 LUJ, ÃÂ» Â°Ã ÃÃÂ±Ã¢ÃÂ­
	SetSummonedVehicle( 0 );
	SetMountedVehicle( 0 );
	m_initState = 0;

	mWeaponEnchantLevel = 0;
	mPhysicDefenseEnchantLevel = 0;
	mMagicDefenseEnchantLevel = 0;

	strcpy(m_szHouseName, "");

	m_dwCurrentResurrectIndex = 0;
	mPetIndex = 0;
	m_bDungeonObserver = FALSE;

	m_dwConsignmentTick = 0;
	ForbidChatTime = 0;
}

BOOL CPlayer::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	m_StreetStallTitle[0] = 0;
	m_MurdererIDX = 0;
	m_MurdererKind = 0;
	m_bPenaltyByDie = FALSE;

	m_bReadyToRevive = FALSE;
	m_bShowdown	= FALSE;
	m_bExit = FALSE;
	m_bNormalExit = FALSE;
//
	CObject::Init(kind, AgentNum, pBaseObjectInfo); //Â¢Â¯Â¨ÃÂ¡Â¾aÂ¨Ã¹Â¡Â©Â¢Â¥A eObjectState_NoneAÂ¢Â¬Â¡Â¤I Â¢Â¬Â¢Â¬Â¥Ã¬cÂ¢Â¥U.

//KES 040827
	OBJECTSTATEMGR_OBJ->StartObjectState( this, eObjectState_Immortal, 0 );
	// 06.08.29. RaMa.
	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 30000 );
//

	m_CurComboNum = 0;
	m_pGuetStall = 0;
	m_bNeedRevive = TRUE;
	ZeroMemory(
		&m_GameOption,
		sizeof(m_GameOption));
	m_ContinuePlayTime = 0;

//HACK CHECK INIT
	m_nHackCount = 0;
	m_dwHackStartTime = gCurTime;
	m_wKillMonsterLevel = 0;
	m_bDieForGFW = FALSE;
	m_bWaitExitPlayer = TRUE;
	m_bWaitExitPlayer = TRUE;

//	m_SkillList.RemoveAll();
	m_SkillFailCount = 0;

	for( BYTE i = 0; i < 8; i++ )
	{
		m_QuickSlot[i].Init( GetID(), i );
	}

	m_SkillTree->Init( this );
	m_stMonstermeterInfo.nPlayTimeTick = gCurTime;
	mIsEmergency = FALSE;

	mTargetMap = 0;
	mTargetPosX = 0;
	mTargetPosZ = 0;

	m_dwLastTimeCheckItemDBUpdate = gCurTime;
	m_dwLastTimeCheckItem = gCurTime;

	m_bNoExpPenaltyByPK = FALSE;

	// 080515 LUJ, ÃÃ°ÃÂ¸ÃÃ ÃÂ¼ÃÂ©Â¿Ã« Â±Â¸ÃÂ¶ÃÂ¼ ÃÃÂ±Ã¢ÃÂ­
	ZeroMemory( &mCheckCoolTime, sizeof( mCheckCoolTime ) );

	m_dwSkillCancelCount = 0;
	m_dwSkillCancelLastTime = 0;

	m_dwAutoNoteLastExecuteTime = 0;
	
	mWeaponEnchantLevel = 0;
	mPhysicDefenseEnchantLevel = 0;
	mMagicDefenseEnchantLevel = 0;

	m_dwCurrentResurrectIndex = 0;

	ZeroMemory( &m_YYManaRecoverTime, sizeof(m_YYManaRecoverTime) );
	m_ManaRecoverDirectlyAmount = 0;

	return TRUE;
}
void CPlayer::Release()
{
	RemoveAllAggroed();
//-------------------

	FishingData_Update(GetID(), GetFishingLevel(), GetFishingExp(), GetFishPoint());
	Cooking_Update(this);

	if(FISHINGMGR->GetActive())		// Â³Â¬Â½ÃÂ½ÂºÃÂ©Â¸Â³ÃÂ®Â°Â¡ ÃÂ°Â¼ÂºÂµÃÂ¾Ã® ÃÃÃÂ»Â¶Â§(==2Â¹Ã¸ Â³Ã³ÃÃ¥Â¸Ã)Â¸Â¸ Â±Ã¢Â·Ã.
	{
		// 080808 LUJ, Â³Â¬Â½Ã ÃÂ¤ÂºÂ¸Â¸Â¦ Â·ÃÂ±ÃÂ·Ã ÃÃºÃÃ¥
		Log_Fishing(
			GetID(),
			eFishingLog_Regular,
			0,
			GetFishPoint(),
			0,
			0,
			GetFishingExp(),
			GetFishingLevel() );
	}

	LogOnRelease();
	GUILDMGR->RemovePlayer( this );
	ITEMMGR->RemoveCoolTime( GetID() );
	mCoolTimeMap.clear();
	mSkillCoolTimeMap.clear();
	mSkillAnimTimeMap.clear();

	{
		for( POSTYPE position = 0; position < m_ItemContainer.GetSize(); ++position )
		{
			const ITEMBASE* item = m_ItemContainer.GetItemInfoAbs( position );

			if( ! item )
			{
				continue;
			}

			ITEMMGR->RemoveOption( item->dwDBIdx );
		}
	}

	{
		m_ItemArrayList.SetPositionHead();

		for(
			ITEMOBTAINARRAYINFO* pInfo;
			(pInfo = m_ItemArrayList.GetData())!= NULL; )
		{
			ITEMMGR->Free(this, pInfo);
		}

		m_ItemArrayList.RemoveAll();
	}

	m_FollowMonsterList.RemoveAll();
	
	{		
		m_QuestList.SetPositionHead();

		for(
			CQuestBase* pQuest;
			(pQuest = m_QuestList.GetData())!= NULL; )
		{
			delete pQuest;
		}

		m_QuestList.RemoveAll();
	}
	

	m_InventoryPurse.Release();
	m_StoragePurse.Release();
	m_QuestGroup.Release();
	CObject::Release();

	mSpecialSkillList.clear();
	m_SkillFailCount = 0;
	m_SkillTree->Release();

	SAFE_DELETE_ARRAY(m_pcFamilyEmblem);	

	{
		CPet* const petObject = PETMGR->GetPet(
			GetPetItemDbIndex());

		g_pServerSystem->RemovePet(
			petObject ? petObject->GetID() : 0,
			FALSE);
	}

	// 090316 LUJ, ÃÃÃÃ· ÃÂ³Â¸Â®ÃÃÂ´Ã
	{
		CVehicle* const vehicleObject = ( CVehicle* )g_pUserTable->FindUser( GetMountedVehicle() );

		if( vehicleObject &&
			vehicleObject->GetObjectKind() == eObjectKind_Vehicle )
		{
			vehicleObject->Dismount( GetID(), TRUE );
		}
	}

	mWeaponEnchantLevel = 0;
	mPhysicDefenseEnchantLevel = 0;
	mMagicDefenseEnchantLevel = 0;
}

void CPlayer::UpdateGravity()
{
	mGravity = 0;
	m_FollowMonsterList.SetPositionHead();

	while(CObject* const pObject = m_FollowMonsterList.GetData())
	{
		mGravity += pObject->GetGravity();
	}
}

BOOL CPlayer::AddFollowList(CMonster * pMob)
{
	if( m_FollowMonsterList.GetDataNum() < 50 )		//max 50Â¸Â¶Â¸Â®
	{
		m_FollowMonsterList.Add(pMob, pMob->GetID());
		UpdateGravity();
		return TRUE;
	}

	return FALSE;
}
BOOL CPlayer::RemoveFollowAsFarAs(DWORD GAmount, CObject* pMe )
{	
	VECTOR3 * ObjectPos	= CCharMove::GetPosition(this);
	BOOL bMe = FALSE;

	while(GAmount > 100)
	{	
		CMonster * MaxObject = NULL;
		float	MaxDistance	= -1;
		float	Distance	= 0;

		m_FollowMonsterList.SetPositionHead();
		while(CMonster* pObject = m_FollowMonsterList.GetData())
		{
			VECTOR3 * TObjectPos = CCharMove::GetPosition(pObject);
			if((Distance = CalcDistanceXZ( ObjectPos, TObjectPos )) > MaxDistance)
			{
				MaxDistance = Distance;
				MaxObject = pObject;
			}
		}

		if(MaxObject)
		{
			if(GAmount > MaxObject->GetGravity())
				GAmount -= MaxObject->GetGravity();
			else
				GAmount = 0;

			MaxObject->SetTObject(NULL);

			GSTATEMACHINE.SetState(MaxObject, eMA_WALKAROUND);

			if( pMe == MaxObject )
				bMe = TRUE;
		}
		else
		{
			MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("amount != 0"));
			GAmount = 0;
		}
	}

	return bMe;	
}

void CPlayer::RemoveFollowList(DWORD ID)
{
	m_FollowMonsterList.Remove(ID);
	UpdateGravity();
}

ITEMOBTAINARRAYINFO * CPlayer::GetArray(WORD id)
{
	return m_ItemArrayList.GetData(id);
}

void CPlayer::AddArray(ITEMOBTAINARRAYINFO * pInfo)
{
	m_ItemArrayList.Add(pInfo, pInfo->wObtainArrayID);
}
void CPlayer::RemoveArray(DWORD id)
{
	m_ItemArrayList.Remove(id);
}
void CPlayer::InitCharacterTotalInfo(CHARACTER_TOTALINFO* pTotalInfo)
{
	memcpy(&m_HeroCharacterInfo,pTotalInfo,sizeof(CHARACTER_TOTALINFO));
	if( GetUserLevel() == eUSERLEVEL_GM )
		m_HeroCharacterInfo.bVisible = FALSE;
	else
		m_HeroCharacterInfo.bVisible = TRUE;

	// 071226 KTH -- ÃÃÂºÂ¥ÃÃ¤Â¸Â®ÃÃ ÃÂ©Â±Ã¢Â¸Â¦ Â¼Â³ÃÂ¤ÃÃÂ¿Â© ÃÃÂ´Ã.
	m_InventorySlot.SetSlotNum( POSTYPE( SLOT_INVENTORY_NUM + GetInventoryExpansionSize() ) );

}

void CPlayer::InitHeroTotalInfo(HERO_TOTALINFO* pHeroInfo)
{
	memcpy(&m_HeroInfo,pHeroInfo,sizeof(HERO_TOTALINFO));
	m_ItemContainer.GetSlot(eItemTable_Inventory)->CreatePurse(&m_InventoryPurse, this, m_HeroInfo.Money, MAX_INVENTORY_MONEY);
}

void CPlayer::InitItemTotalInfo(ITEM_TOTALINFO* pItemInfo)
{
	m_ItemContainer.GetSlot(eItemTable_Inventory)->SetItemInfoAll(pItemInfo->Inventory);	
	m_ItemContainer.GetSlot(eItemTable_Weared)->SetItemInfoAll(pItemInfo->WearedItem);
}

void CPlayer::AddStorageItem(ITEMBASE * pStorageItem)
{
	CStorageSlot * pSlot = (CStorageSlot *)m_ItemContainer.GetSlot(eItemTable_Storage);
	pSlot->InsertItemAbs(this, pStorageItem->Position, pStorageItem);
}

void CPlayer::InitStorageInfo(BYTE Storagenum,MONEYTYPE money)
{
	CStorageSlot * pSlot = (CStorageSlot *)m_ItemContainer.GetSlot(eItemTable_Storage);
	pSlot->SetStorageNum(Storagenum);

	MONEYTYPE maxmoney = 0;
	if(Storagenum)
	{		
		STORAGELISTINFO* pInfo = STORAGEMGR->GetStorageInfo(Storagenum);
		ASSERT(pInfo);
		maxmoney = pInfo ? pInfo->MaxMoney : 10;
	}
	else
	{
		ASSERT(money == 0);
		maxmoney = 0;
	}
	pSlot->CreatePurse(&m_StoragePurse, this, money, maxmoney);	
}

void CPlayer::InitShopItemInfo(SEND_SHOPITEM_INFO& message)
{
	CItemSlot* const pSlot = m_ItemContainer.GetSlot(
		eItemTable_Shop);

	for(DWORD i = 0; i < _countof(message.Item); ++i)
	{
		pSlot->ClearItemBaseAndSlotInfo(
			POSTYPE(TP_SHOPITEM_START + i));

		ITEMBASE& itemBase = message.Item[i];

		if(0 == itemBase.dwDBIdx)
		{
			continue;
		}

		pSlot->InsertItemAbs(
			this,
			itemBase.Position,
			&itemBase);
	}
}

void CPlayer::SetStorageNum(BYTE n)
{
	CStorageSlot * pSlot = (CStorageSlot *)m_ItemContainer.GetSlot(eItemTable_Storage);
	pSlot->SetStorageNum(n);
}

BYTE CPlayer::GetStorageNum()
{
	CStorageSlot * pSlot = (CStorageSlot *)m_ItemContainer.GetSlot(eItemTable_Storage);
	return pSlot->GetStorageNum();
}

MONEYTYPE CPlayer::GetMoney(eITEMTABLE tableIdx)
{
	CItemSlot* pSlot = m_ItemContainer.GetSlot(tableIdx);

	return pSlot ? pSlot->GetMoney() : 0;
}

void CPlayer::GetItemtotalInfo(ITEM_TOTALINFO& pRtInfo,DWORD dwFlag)
{
	if(dwFlag & GETITEM_FLAG_INVENTORY)
	{
		m_ItemContainer.GetSlot(eItemTable_Inventory)->GetItemInfoAll(
			pRtInfo.Inventory,
			_countof(pRtInfo.Inventory));
	}

	if(dwFlag & GETITEM_FLAG_WEAR)
	{
		m_ItemContainer.GetSlot(eItemTable_Weared)->GetItemInfoAll(
			pRtInfo.WearedItem,
			_countof(pRtInfo.WearedItem));
	}
}

// 091026 LUJ, Â¹Ã¶ÃÃ ÃÂ©Â±Ã¢Â¿Â¡ Â¹Â«Â°Ã¼ÃÃÂ°Ã ÃÃ¼Â¼ÃÃÃ Â¼Ã¶ ÃÃÂµÂµÂ·Ã Â¼Ã¶ÃÂ¤
DWORD CPlayer::SetAddMsg(DWORD dwReceiverID, BOOL isLogin, MSGBASE*& sendMessage)
{
	if(eUSERLEVEL_GM >= GetUserLevel() &&
		FALSE == IsVisible())
	{
		return 0;
	}

	static SEND_CHARACTER_TOTALINFO message;
	ZeroMemory( &message, sizeof( message ) );
	message.Category = MP_USERCONN;
	message.Protocol = MP_USERCONN_CHARACTER_ADD;
	message.dwObjectID = dwReceiverID;
	GetSendMoveInfo( &message.MoveInfo,&message.AddableInfo );
	GetBaseObjectInfo( &message.BaseObjectInfo);
	GetCharacterTotalInfo( &message.TotalInfo );
	message.TotalInfo.DateMatching = m_DateMatching;
	message.bLogin = BYTE(isLogin);
	message.mMountedVehicle.mVehicleIndex = GetSummonedVehicle();

	// 090316 LUJ, ÃÂ¾Â½Ã ÃÂ¤ÂºÂ¸ ÂºÂ¹Â»Ã§
	{
		CVehicle* const vehicle = ( CVehicle* )g_pUserTable->FindUser( GetMountedVehicle() );

		if( vehicle && vehicle->GetObjectKind() == eObjectKind_Vehicle )
		{
			message.mMountedVehicle.mVehicleIndex = vehicle->GetID();
			message.mMountedVehicle.mSeatIndex = vehicle->GetMountedSeat( GetID() );
		}
	}

	// ÃÃÂ¿Ã¬ÃÂ¡ ÃÂ¾Â½Ã ÃÂ¤ÂºÂ¸ ÂºÂ¹Â»Ã§
	{
		const stHouseRiderInfo* const houseRiderInfo = HOUSINGMGR->GetRiderInfo(GetID());

		if(houseRiderInfo)
		{
			message.mRiderInfo.dwFurnitureObjectIndex = houseRiderInfo->dwFurnitureObjectIndex;
			message.mRiderInfo.wSlot = houseRiderInfo->wSlot;
		}
	}

	cStreetStall* pStall = STREETSTALLMGR->FindStreetStall(this);

	if( pStall != NULL)
	{
		char StallTitle[MAX_STREETSTALL_TITLELEN+1] = {0};
		GetStreetStallTitle(StallTitle);
		CAddableInfoList::AddableInfoKind kind = CAddableInfoList::None;

		switch(pStall->GetStallKind())
		{
		case eSK_SELL:
			{

				kind = CAddableInfoList::StreetStall;
				break;
			}
		case eSK_BUY:
			{
				kind = CAddableInfoList::StreetBuyStall;
				break;
			}
		}
		
		message.AddableInfo.AddInfo(
			BYTE(kind),
			WORD(strlen(StallTitle)+1),
			StallTitle,
			__FUNCTION__);
	}

	switch(GetBattle()->GetBattleKind())
	{
	case eBATTLE_KIND_SHOWDOWN:
		{
			COMPRESSEDPOS ShowdownPos;
			BATTLE_INFO_SHOWDOWN info;
			WORD wSize = 0;
			GetBattle()->GetBattleInfo( (char*)&info, &wSize );
			ShowdownPos.Compress(&info.vStgPos);
			message.AddableInfo.AddInfo(CAddableInfoList::ShowdownInfo,sizeof(COMPRESSEDPOS),&ShowdownPos, __FUNCTION__ );
			break;
		}
	case eBATTLE_KIND_GTOURNAMENT:
		{
			int nTeam = GetBattle()->GetBattleTeamID(this);
			message.AddableInfo.AddInfo(CAddableInfoList::GTournament, sizeof(nTeam), &nTeam, __FUNCTION__ );
			break;
		}
	}

	sendMessage = &message;
	return message.GetMsgLength();
}

void CPlayer::SetStreetStallTitle(char* title)
{
	SafeStrCpy( m_StreetStallTitle, title, MAX_STREETSTALL_TITLELEN+1 );
}

void CPlayer::GetStreetStallTitle(char* title)
{
	SafeStrCpy( title, m_StreetStallTitle, MAX_STREETSTALL_TITLELEN+1);
}

void CPlayer::CalcState()
{
	//CHARCALCMGR->CalcItemStats(this);
	CHARCALCMGR->CalcCharStats( this );

	SetLife(m_HeroCharacterInfo.Life);
	SetMana(m_HeroInfo.Mana);
}

void CPlayer::MoneyUpdate( MONEYTYPE money )
{	
	m_HeroInfo.Money = money;
}

void CPlayer::SetStrength(DWORD val)
{
	m_HeroInfo.Str = val;

	CHARCALCMGR->CalcCharStats(this);

	// DBÂ¡Ã?Â¡ÃÂ¢Ã§ Â¡Â§uÂ¢Â®AÂ¡ÃiÂ¢Â®IAIÂ¡Â§Â¢Â®Â¡Ãc
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_STR_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

void CPlayer::SetDexterity(DWORD val)
{
	m_HeroInfo.Dex = val;

	CHARCALCMGR->CalcCharStats(this);

	// DBÂ¡Ã?Â¡ÃÂ¢Ã§ Â¡Â§uÂ¢Â®AÂ¡ÃiÂ¢Â®IAIÂ¡Â§Â¢Â®Â¡Ãc
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_DEX_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetVitality(DWORD val)
{
	m_HeroInfo.Vit = val;

	// Â¢Â®iyÂ¡ÃÂ¡Ã¾iÂ¢Â®Â¢Â´A, Â¨ÃoÂ¨ÃÂ¢Â®Â¡Â§uiÂ¢Â®Â¢Â´A; Â¡ÃÂ¡ÃUÂ¡Â§oA Â¢Â®Â¨Â¡eÂ¢Â®ie
	CHARCALCMGR->CalcCharStats(this);

	// DBÂ¡Ã?Â¡ÃÂ¢Ã§ Â¡Â§uÂ¢Â®AÂ¡ÃiÂ¢Â®IAIÂ¡Â§Â¢Â®Â¡Ãc
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_VIT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetWisdom(DWORD val)
{
	m_HeroInfo.Wis = val;

	// Â¨ÃÂ©ÂªÂ¢Â®iÂ¢Â®Â¢Â´A; Â¡ÃÂ¡ÃUÂ¡Â§oA Â¢Â®Â¨Â¡eÂ¢Â®ie
	CHARCALCMGR->CalcCharStats(this);

	// DBÂ¡Ã?Â¡ÃÂ¢Ã§ Â¡Â§uÂ¢Â®AÂ¡ÃiÂ¢Â®IAIÂ¡Â§Â¢Â®Â¡Ãc
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_WIS_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

// 070411 LYW --- Player : Add function to setting intelligence.
void CPlayer::SetIntelligence( DWORD val )
{
	m_HeroInfo.Int = val ;

	CHARCALCMGR->CalcCharStats(this);

	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_INT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

CItemSlot * CPlayer::GetSlot(POSTYPE absPos)
{
	if( TP_INVENTORY_START <= absPos && absPos < TP_SHOPITEM_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END )
	{
		return (CItemSlot*)GUILDMGR->GetSlot( GetGuildIdx());
	}
	else if( TP_SHOPITEM_START <= absPos && absPos < TP_SHOPITEM_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}

	return NULL;
}

CItemSlot * CPlayer::GetSlot(eITEMTABLE tableIdx)
{
	if(tableIdx < eItemTable_TableMax)
		return m_ItemContainer.GetSlot(tableIdx);
	else if( tableIdx == eItemTable_MunpaWarehouse)
	{
		return (CItemSlot*)GUILDMGR->GetSlot(GetGuildIdx());
	}
	return NULL;
}

//-------------------------------------------------------------------------------------------------
//	NAME : SetLifeForce
//	DESC : 080625 LYW 
//		   ÃÂ³Â¸Â¯ÃÃÂ°Â¡ ÃÃÃÂº Â»Ã³ÃÃÂ¿Â¡Â¼Â­ Â°Â­ÃÂ¾ÃÃÂ³Âª ÃÂ¨Â±Ã¨ ÃÃ¶Â»Ã³ÃÃ Â¹ÃÂ»Ã½ ÃÃ Â¶Â§, 
//		   ÃÂ³Â¸Â¯ÃÃÃÃ Â»Ã½Â¸Ã­Â·Ã 50%Â¸Â¦ ÂºÂ¹Â±Â¸ÃÃ ÃÃÂ¾Ã®Â¾Ã ÃÃÂ´Ã. Â±ÃÂ·Â±ÂµÂ¥ Â»Ã³ÃÃÂ°Â¡ ÃÃÃÂº Â»Ã³ÃÃÂ¸Ã©, 
//		   Â±Ã¢ÃÂ¸ ÃÃÂ¼Ã¶Â´Ã return ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ¹ÃÂ·Ã, Â°Â­ÃÂ¦Â·Ã ÂºÂ¹Â±Â¸ Â¿Â©ÂºÃÂ¸Â¦ Â¼Â¼ÃÃÃÃ Â¼Ã¶ ÃÃÂ´Ã ÃÃÂ¼Ã¶Â¸Â¦ ÃÃÂ°Â¡ÃÃÂ´Ã.
//-------------------------------------------------------------------------------------------------
void CPlayer::SetLifeForce(DWORD Life, BYTE byForce, BOOL bSendMsg) 
{
	// Â°Â­ÃÂ¦ Â¼Â¼ÃÃ Â¿Â©ÂºÃÂ¸Â¦ ÃÂ®ÃÃÃÃÂ´Ã.
	if(byForce == FALSE)
	{
		// ÃÂ³Â¸Â¯ÃÃÂ°Â¡ ÃÃÃÂº Â»Ã³ÃÃÂ¶Ã³Â¸Ã©, return ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ´Ã.
		if(GetState() == eObjectState_Die) return ;
	}


	// ÃÃ¶ÃÃ§ ÃÂ³Â¸Â¯ÃÃ Â·Â¹ÂºÂ§Â¿Â¡ Â¸ÃÂ´Ã ÃÃÂ´Ã« Â»Ã½Â¸Ã­Â·ÃÃÂ» Â¹ÃÂ´ÃÂ´Ã.
	DWORD maxlife = 0 ;
	maxlife = GetMaxLife() ;


	// Â»Ã½Â¸Ã­Â·Ã Â¼Ã¶ÃÂ¡ ÃÂ¯ÃÂ¿ ÃÂ¼ÃÂ©.
	if(Life > maxlife) Life = maxlife ;


	// ÃÃÃÃÂ·Ã Â³ÃÂ¾Ã®Â¿Ã Â»Ã½Â¸Ã­Â·ÃÃÃ Â±Ã¢ÃÂ¸ Â»Ã½Â¸Ã­Â·Ã ÂºÂ¸Â´Ã ÃÃÃÂ¸Â¸Ã©, return ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ´Ã.
	if( m_HeroCharacterInfo.Life >= Life ) return ;

	
	// Â±Ã¢ÃÂ¸ Â»Ã½Â¸Ã­Â·Ã / ÃÃÃÃÂ·Ã Â³ÃÂ¾Ã®Â¿Ã Â»Ã½Â¸Ã­Â·ÃÃÃ Â°Â°ÃÃ¶ Â¾ÃÃÂ¸Â¸Ã©,
	if(m_HeroCharacterInfo.Life != Life)
	{
		// Â¸ÃÂ½ÃÃÃ¶ ÃÃ¼Â¼Ã ÃÃÂ´Ã Â»Ã³ÃÂ²ÃÃÂ¶Ã³Â¸Ã©,
		if(bSendMsg == TRUE)
		{
			//// Â»ÃµÂ·ÃÂ¿Ã® Â»Ã½Â¸Ã­Â·ÃÃÂ» Â°Ã¨Â»ÃªÃÃÂ´Ã.
			//DWORD dwNewLife = 0 ;
			//dwNewLife = Life - GetLife() ;

			// Â¸ÃÂ½ÃÃÃ¶Â¸Â¦ ÃÃ¼Â¼ÃÃÃÂ´Ã.
			MSG_INT msg ;
			msg.Category = MP_CHAR ;
			msg.Protocol = MP_CHAR_LIFE_ACK ;
			msg.dwObjectID = GetID() ;
			//msg.nData = dwNewLife ;
			msg.nData = Life ;

			SendMsg(&msg,sizeof(msg)) ;
		}
		
		SendLifeToParty(
			Life);
	}
		

	// ÃÂ³Â¸Â¯ÃÃÃÃ Â»Ã½Â¸Ã­Â·ÃÃÂ» Â¼Â¼ÃÃÃÃÂ´Ã.
	m_HeroCharacterInfo.Life = Life ;
}





void CPlayer::SetLife(DWORD val,BOOL bSendMsg) 
{
	if(GetState() == eObjectState_Die)
		return;

	DWORD maxlife = GetMaxLife();
	if(val > maxlife)
		val = maxlife;
	
	if(m_HeroCharacterInfo.Life != val)	// Â¡ÃiÂ¢Â®IÂ¨ÃoIAo AIÂ¢Â®Â¨Â¡Â¨ÃÂ¢Â®Â¡Ã?iÂ¡Ã?Â¡ÃÂ¢Ã§Â¡ÃÂ¡ÃA Â¡Ã?Â¢Â®Â©Â­AaÂ¡Ã?Â¡ÃÂ¢Ã§Â¡Â§uÂ¢Â®Â¨Ã Â¢Â®iÂ¡Â§IAÂ¡Â§Â¨Â£Â¡ÃÂ¡ÃU.
	{
		if(bSendMsg == TRUE)
		{
			// To AÂ¢Â®Â©Â­Â¡ÃOoAIÂ¡Â§uÂ¨ÃÂ¡ÃÂ¡Â§Â¢Â®Â¡Ãc -------------------------------
			MSG_INT msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_LIFE_BROADCAST;
			msg.dwObjectID = GetID();
			// 070419 LYW --- Player : Modified function SetLife.
			msg.nData = val - GetLife();
			PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof( msg ) );
		}
		
		SendLifeToParty(
			val);
	}
		
	m_HeroCharacterInfo.Life = val;
}

void CPlayer::SendLifeToParty(DWORD val)
{	
	if(CParty* pParty = PARTYMGR->GetParty(GetPartyIdx()))
	{
		MSG_DWORD2 message;
		ZeroMemory(
			&message,
			sizeof(message));
		message.Category = MP_PARTY;
		message.Protocol = MP_PARTY_MEMBERLIFE;
		message.dwData1 = GetID();
		message.dwData2 = val * 100 / GetMaxLife();

		pParty->SendMsgExceptOneinChannel(
			&message,
			sizeof(message),
			GetID(),
			GetGridID());
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetLifeForce
//	DESC : 080625 LYW 
//		   ÃÂ³Â¸Â¯ÃÃÂ°Â¡ ÃÃÃÂº Â»Ã³ÃÃÂ¿Â¡Â¼Â­ Â°Â­ÃÂ¾ÃÃÂ³Âª ÃÂ¨Â±Ã¨ ÃÃ¶Â»Ã³ÃÃ Â¹ÃÂ»Ã½ ÃÃ Â¶Â§, 
//		   ÃÂ³Â¸Â¯ÃÃÃÃ Â¸Â¶Â³Âª 30%Â¸Â¦ ÂºÂ¹Â±Â¸ÃÃ ÃÃÂ¾Ã®Â¾Ã ÃÃÂ´Ã. Â±ÃÂ·Â±ÂµÂ¥ Â»Ã³ÃÃÂ°Â¡ ÃÃÃÂº Â»Ã³ÃÃÂ¸Ã©, 
//		   Â±Ã¢ÃÂ¸ ÃÃÂ¼Ã¶Â´Ã return ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ¹ÃÂ·Ã, Â°Â­ÃÂ¦Â·Ã ÂºÂ¹Â±Â¸ Â¿Â©ÂºÃÂ¸Â¦ Â¼Â¼ÃÃÃÃ Â¼Ã¶ ÃÃÂ´Ã ÃÃÂ¼Ã¶Â¸Â¦ ÃÃÂ°Â¡ÃÃÂ´Ã.
//-------------------------------------------------------------------------------------------------
void CPlayer::SetManaForce(DWORD Mana, BYTE byForce, BOOL bSendMsg) 
{
	// Â°Â­ÃÂ¦ Â¼Â¼ÃÃ Â¿Â©ÂºÃÂ¸Â¦ ÃÂ®ÃÃÃÃÂ´Ã.
	if(byForce == FALSE)
	{
		// ÃÂ³Â¸Â¯ÃÃÂ°Â¡ ÃÃÃÂº Â»Ã³ÃÃÂ¶Ã³Â¸Ã©, return ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ´Ã.
		if(GetState() == eObjectState_Die) return ;
	}


	// ÃÂ³Â¸Â¯ÃÃÃÃ ÃÃ¶ÃÃ§ Â·Â¹ÂºÂ§Â»Ã³ ÃÃÂ´Ã« Â¸Â¶Â³Âª Â¼Ã¶ÃÂ¡Â¸Â¦ Â¹ÃÂ´ÃÂ´Ã.
	DWORD MaxMana = 0 ;
	MaxMana = GetMaxMana() ;


	// ÃÃÃÃÂ·Ã Â³ÃÂ¾Ã®Â¿Ã Â¸Â¶Â³ÂªÃÃ ÃÂ¯ÃÂ¿ Â¹Ã¼ÃÂ§Â¸Â¦ ÃÂ¼ÃÂ©.
	if(Mana > MaxMana) Mana = MaxMana ;


	// ÃÃÃÃÂ·Ã Â³ÃÂ¾Ã®Â¿Ã Â¸Â¶Â³ÂªÂ·Â®ÃÃ Â±Ã¢ÃÂ¸ Â¸Â¶Â³ÂªÂ·Â® ÂºÂ¸Â´Ã ÃÃÃÂ¸Â¸Ã©, return ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ´Ã.
	if( m_HeroInfo.Mana >= Mana ) return ;


	// Â±Ã¢ÃÂ¸Â¸Â¶Â³Âª / ÃÃÃÃÂ·Ã Â³ÃÂ¾Ã®Â¿Ã Â¸Â¶Â³ÂªÂ°Â¡ Â°Â°ÃÃ¶ Â¾ÃÃÂ¸Â¸Ã©,
	if( m_HeroInfo.Mana != Mana)
	{
		// Â¸ÃÂ½ÃÃÃ¶ ÃÃ¼Â¼Ã Â¿Â©ÂºÃÂ°Â¡ TRUE ÃÃÂ¸Ã©,
		if(bSendMsg)
		{
			// Â¸Â¶Â³ÂªÂ¸Â¦ ÃÃ¼Â¼ÃÃÃÂ´Ã.
			MSG_DWORD msg ;
			msg.Category = MP_CHAR ;
			msg.Protocol = MP_CHAR_MANA_GET_ACK;
			msg.dwObjectID = GetID() ;
			msg.dwData = Mana ;
			SendMsg(&msg,sizeof(msg)) ;
		}
		
		SendManaToParty(
			Mana);
	}
	

	// ÃÂ³Â¸Â¯ÃÃÃÃ Â¸Â¶Â³ÂªÂ¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
	m_HeroInfo.Mana = Mana ; 
}





void CPlayer::SetMana(DWORD val,BOOL bSendMsg)
{ 
	if(GetState() == eObjectState_Die)
		return;

	DWORD MaxMana = GetMaxMana();
	if(val > MaxMana)
		val = MaxMana;

	if( m_HeroInfo.Mana != val)
	{
		// 100223 ShinJS --- Â¸Â¶Â³Âª ÂµÂ¥Â¹ÃÃÃ¶ ÃÃÂ°Â¡Â·Ã ÃÃÃÃ Â¸Â¶Â³Âª ÃÂ¤ÂºÂ¸Â¸Â¦ ÂºÂ¯ÃÂ­Â·Â®ÃÂ¸Â·Ã Â¼Ã¶ÃÂ¤.
		if(bSendMsg)
		{
			MSG_INT msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_MANA_ACK;
			msg.dwObjectID = GetID();
			msg.nData = val - GetMana();
			SendMsg(&msg,sizeof(msg));
		}
		
		SendManaToParty(
			val);
	}
	
	m_HeroInfo.Mana = val; 
}

void CPlayer::SendManaToParty(DWORD mana)
{
	if(CParty* pParty = PARTYMGR->GetParty(GetPartyIdx()))
	{
		MSG_DWORD2 message;
		ZeroMemory(
			&message,
			sizeof(message));
		message.Category = MP_PARTY;
		message.Protocol = MP_PARTY_MEMBERMANA;
		message.dwData1 = GetID();
		message.dwData2 = mana * 100 / GetMaxMana();

		pParty->SendMsgExceptOneinChannel(
			&message,
			sizeof(message),
			GetID(),
			GetGridID());
	}
}

void CPlayer::SetMaxLife(DWORD val)
{
	m_HeroCharacterInfo.MaxLife = val;

	// To AÂ¢Â®Â©Â­Â¡ÃOoAIÂ¡Â§uÂ¨ÃÂ¡ÃÂ¡Â§Â¢Â®Â¡Ãc -------------------------------
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MAXLIFE_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof( msg ) );
}

void CPlayer::SetMaxMana(DWORD val)
{
	m_HeroInfo.MaxMana= val;

	// To AÂ¢Â®Â©Â­Â¡ÃOoAIÂ¡Â§uÂ¨ÃÂ¡ÃÂ¡Â§Â¢Â®Â¡Ãc -------------------------------
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MAXMANA_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::AddStrength(int val)
{
	SetStrength(m_HeroInfo.Str+val);
}
void CPlayer::AddWisdom(int val)
{
	SetWisdom(m_HeroInfo.Wis+val);
}
void CPlayer::AddDexterity(int val)
{
	SetDexterity(m_HeroInfo.Dex+val);
}
void CPlayer::AddVitality(int val)
{
	SetVitality(m_HeroInfo.Vit+val);
}
// 070411 LYW --- Player : Add function to setting intelligence.
void CPlayer::AddIntelligence( int val )
{
	SetIntelligence( m_HeroInfo.Int+ val ) ;
}
void CPlayer::SetPlayerLevelUpPoint(LEVELTYPE val) 
{ 
	m_HeroInfo.LevelUpPoint=val;
	
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_LEVELUPPOINT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
/*****************************************************************/
/* 1. AEÂ¢Â®Â¨ÃºaÂ¡Ã?Â¡ÃÂ¢Ã§ AÂ¨ÃÂ©ÂªÂ¡ÃÂ¡Ã¾?AIÂ¢Â®Â¨Â¡Â¡ÃÂ¢Ã§ Â¢Â®iyÂ¡Â§uÂ¡Â§Â¡Ã¾Â¡ÃiCÂ¡Â§uu;Â¡ÃOÂ¢Â®Â¡Â¿ from DBResultQuery
/* 2. SetPlayerExpPoint()Â¡Ã?Â¡ÃÂ¢Ã§Â¡Â§uÂ¢Â®Â¨Ã AOÂ¡ÃÂ¡Ãe Â¢Â®Â¨Â¡Â¨ÃÂ¢Â®CeAÂ¡ÃÂ¢Ã§Â¡ÃÂ¡Ã¾| Â¨ÃÂ©ÂªNÂ¡Â§ui Â¡Â§uÂ¨Ão; Â¡ÃOÂ¢Â®Â¡Â¿ callÂ¡ÃiE
/*
/*
/*****************************************************************/

void CPlayer::SetLevel(LEVELTYPE level)
{
	if(level >= MAX_PLAYERLEVEL_NUM)
	{
		ASSERT(0);
		return;
	}

	if(m_HeroCharacterInfo.Level == level) return;

	LEVELTYPE AddPoint = 0;
	if(level > m_HeroCharacterInfo.Level)
	{
		if(m_HeroInfo.MaxLevel < level)
		{
			m_HeroInfo.MaxLevel = level;
			AddPoint += 5;

			DWORD skillpoint = (DWORD)( ceil( level / 10.f ) + 19 );

			if(m_HeroCharacterInfo.Race == RaceType_Devil)
			{
				skillpoint = (DWORD)( ceil( level / 10.f ) + 21 );
			}

			SetSkillPoint( skillpoint, FALSE );

			// ÃÂ¥ÃÃÂºÂ¥ÃÂ®
			if( level == 10 )
			{
				WebEvent( GetUserID(), 1 );
			}
		}
	}

	SetLife(
		GetMaxLife());
	SetMana(
		GetMaxMana());

	m_HeroCharacterInfo.Level = level;
	CHARCALCMGR->CalcCharStats( this );

	MSG_LEVEL message;
	ZeroMemory(
		&message,
		sizeof(message));
	message.Category = MP_CHAR;
	message.Protocol = MP_CHAR_LEVEL_NOTIFY;
	message.dwObjectID = GetID();
	message.Level = GetLevel();
	message.MaxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(level);
	message.CurExpPoint = GetPlayerExpPoint();
	PACKEDDATA_OBJ->QuickSend(
		this,
		&message,
		sizeof(message));

	SetPlayerLevelUpPoint(GetPlayerLevelUpPoint() + AddPoint);

	CQuestEvent QEvent(
		eQuestEvent_Level,
		level,
		1);
	QUESTMGR->AddQuestEvent(
		this,
		&QEvent);

	PARTYMGR->MemberLevelUp(
		GetPartyIdx(),
		GetID(),
		GetLevel());
	GUILDMGR->MemberLevelUp(
		GetGuildIdx(),
		GetID(),
		GetLevel());
	g_csFamilyManager.SRV_UpdateMapPlayerInfo(
		this);
}

// 080611 LYW --- Player : Â½ÂºÃÂ³ÃÃ·ÃÃÃÂ® Â¾Ã·ÂµÂ¥ÃÃÃÂ® ÃÂ³Â¸Â®Â¸Â¦ Â¼Ã¶ÃÂ¤ÃÃ.
// (Â¾ÃÃÃÃÃÃÂ¸Â·Ã Â½ÂºÃÂ³ ÃÃ·ÃÃÃÂ®Â¸Â¦ ÃÃÂ°Â¡ÃÃÂ´Ã Â±Ã¢Â´ÃÃÃ Â»Ã½Â°Ã¥Â±Ã¢ Â¶Â§Â¹Â®.)
//void CPlayer::SetSkillPoint( DWORD point )
void CPlayer::SetSkillPoint( DWORD point, BYTE byForced )
{
	// Â°Â­ÃÂ¦ Â¾Ã·ÂµÂ¥ÃÃÃÂ® Â¿Â©ÂºÃ ÃÂ¼ÃÂ©.
	ASSERT(byForced <= TRUE) ;
	if(byForced > TRUE) return ;


	// Â½ÂºÃÂ³ ÃÃ·ÃÃÃÂ® Â¾Ã·ÂµÂ¥ÃÃÃÂ®.
	GetHeroTotalInfo()->SkillPoint += point;


	// ÃÂ¬Â¶Ã³ÃÃÂ¾Ã°ÃÂ® ÃÃ¼Â¼Ã
	MSG_DWORD msg;

	msg.Category = MP_SKILLTREE;
	msg.Protocol = MP_SKILLTREE_POINT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = point;

	SendMsg(&msg, sizeof(msg));


	// DB Â¾Ã·ÂµÂ¥ÃÃÃÂ®
	SkillPointUpdate( GetID(), GetSkillPoint() );


	// 071129 LYW --- Player : Â´Â©ÃÃ» Â½ÂºÃÂ³ ÃÃ·ÃÃÃÂ® Â¾Ã·ÂµÂ¥ÃÃÃÂ®.
	//DB_UpdateAccumulateSkillPoint(GetID(), FALSE, point) ;
	DB_UpdateAccumulateSkillPoint(GetID(), byForced, point) ;


	// 071114 Â¿ÃµÃÃ. Â·ÃÂ±Ã
	{
		const SKILL_BASE emptyData = { 0 };

		InsertLogSkill( this, &emptyData, eLog_SkillGetPoint );
	}
}

DWORD CPlayer::GetSkillPoint()
{
	return GetHeroTotalInfo()->SkillPoint;
}

void CPlayer::SetPlayerExpPoint(EXPTYPE point)
{
	// 071119 Â¿ÃµÃÃ, ÃÃÂ¹Ã¸Â¿Â¡ Â¿Â©Â·Â¯ Â´ÃÂ°Ã¨Â¸Â¦ Â·Â¹ÂºÂ§Â¾Ã·ÃÃ Â¼Ã¶ ÃÃÂµÂµÂ·Ã Â¼Ã¶ÃÂ¤ÃÃÂ°Ã­ ÃÃÂµÃ¥Â¸Â¦ Â´ÃÂ¼Ã¸ÃÂ­ÃÃ

	const LEVELTYPE& level = m_HeroCharacterInfo.Level;
	
	ASSERT( level <= MAX_CHARACTER_LEVEL_NUM );

	if( level == MAX_CHARACTER_LEVEL_NUM )
	{
		const EXPTYPE& BeforeExp = m_HeroInfo.ExpPoint;
		// MaxÂ·Â¹ÂºÂ§ÃÃÂ¶Â§ Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ¶Ã´ÃÃ ÂµÃÃÃ¶ Â¾ÃÂ´Ã¸ Â¹Ã¶Â±Ã Â¼Ã¶ÃÂ¤
		// Â°Ã¦ÃÃ¨ÃÂ¡ Â»Ã³Â½ÃÃÃÂ¶Â§Â¸Â¸ Â¸Â®ÃÃÂ½ÃÃÂ²Â´Ã.
		if( point > BeforeExp )
		{
			return;
		}
	}	
	
	// Â°Ã¦ÃÃ¨ÃÂ¡Â°Â¡ Â´ÃÃÂ½ Â´ÃÂ°Ã¨Â¿Â¡Â¼Â­ Â¿Ã¤Â±Â¸ÃÃÂ´Ã Â°ÃÂºÂ¸Â´Ã ÃÃÂ¾Ã Â¸Â¹ÃÂ» Â¼Ã¶ ÃÃÃÂ¸Â¹ÃÂ·Ã,
	// Â°Ã¨Â¼Ã ÃÂ¼ÃÂ©ÃÃÂ¼Â­ Â·Â¹ÂºÂ§Â¾Ã·ÃÃÃÃ
	{
		EXPTYPE nextPoint = GAMERESRCMNGR->GetMaxExpPoint( level );

		while( point >= nextPoint )
		{
			SetLevel( level + 1 );

			CharacterHeroInfoUpdate( this );
			CharacterTotalInfoUpdate( this );

			InsertLogCharacter( GetID(), m_HeroCharacterInfo.Level, &m_HeroInfo );
			InsertLogExp( eExpLog_LevelUp, GetID(), level, 0, GetPlayerExpPoint(), 0, 0, 0 );

			point		-=	nextPoint;
			nextPoint	=	GAMERESRCMNGR->GetMaxExpPoint( level );
		}

		m_HeroInfo.ExpPoint = point;
	}

	{
		MSG_EXPPOINT message;
		message.Category	= MP_CHAR;
		message.Protocol	= MP_CHAR_EXPPOINT_ACK;
		message.ExpPoint	= point;

		SendMsg( &message, sizeof( message ) );
	}
}
void CPlayer::AddPlayerExpPoint(EXPTYPE Exp)
{
	if( Exp == 0 ) return;

	EXPTYPE NewExp = 0 ;

	// 090213 LYW --- Player : ÃÃÂ¹ÃÂ¸Â® Â¸Ã¢Â¹Ã¶ ÃÂ¢Â¼ÃÂ¿Â¡ ÂµÃ»Â¸Â¥ ÃÃÂ°Â¡ Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ ÃÃ»Â¿Ã«ÃÃÂ´Ã. ( Â¼Â­Â¹Ã¶ ÃÃ»Â¿Ã« Â°Ã¦ÃÃ¨ÃÂ¡ Â¼Â³ÃÂ¤ )
	if( GetFamilyIdx() )
	{
		NewExp = GetPlayerExpPoint() + Exp + m_dweFamilyRewardExp ;
	}
	else
	{
		NewExp = GetPlayerExpPoint() + Exp ;
	}

	SetPlayerExpPoint(NewExp);

	MSG_DWORDEX2 msg ;

	msg.Category	= MP_USERCONN ;
	msg.Protocol	= MP_USERCONN_CHARACTER_APPLYEXP_NOTICE ;
	msg.dwObjectID	= GetID() ;
	msg.dweData1	= GetPlayerExpPoint() ;

	// 090213 LYW --- Player : ÃÃÂ¹ÃÂ¸Â® Â¸Ã¢Â¹Ã¶ ÃÂ¢Â¼ÃÂ¿Â¡ ÂµÃ»Â¸Â¥ ÃÃÂ°Â¡ Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ ÃÃ»Â¿Ã«ÃÃÂ´Ã. ( ÃÃ¼Â¼ÃÂ¿Ã« Â°Ã¦ÃÃ¨ÃÂ¡ Â¼Â³ÃÂ¤ )
	if( GetFamilyIdx() )
	{
		msg.dweData2	= Exp + m_dweFamilyRewardExp ;
	}
	else
	{
		msg.dweData2	= Exp ;
	}

	SendMsg(&msg, sizeof(msg)) ;
}

void CPlayer::ReduceExpPoint(EXPTYPE minusExp, BYTE bLogType)
{
	LEVELTYPE minuslevel = 0;

	// 080602 LYW --- Player : Â°Ã¦ÃÃ¨ÃÂ¡ Â¼Ã¶ÃÂ¡ (__int32) Â¿Â¡Â¼Â­ (__int64) Â»Ã§ÃÃÃÃ®Â·Ã ÂºÂ¯Â°Ã¦ ÃÂ³Â¸Â®.
	//DWORD CurExp = GetPlayerExpPoint();
	EXPTYPE CurExp = GetPlayerExpPoint();

	if(GetLevel() <= 1 && CurExp < minusExp)	//Â¡Â¤Â©Ã¶Â¨Â¬Â¡Ã1AÂ¨Â¬ 0Â¡Â¾iAoÂ¢Â¬Â¢Â¬ Â¡Â¾iAIÂ¢Â¥U.
		minusExp = CurExp;

	InsertLogExp( bLogType, GetID(), GetLevel(), minusExp, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, 0/*Â¾Ã®ÂºÃ´Â¸Â®ÃÂ¼ Â»Ã¨ÃÂ¦ - GetPlayerAbilityExpPoint()*/);
	
	while(1)
	{
		if(CurExp < minusExp)
		{
			minusExp -= CurExp;
			++minuslevel;
			CurExp = GAMERESRCMNGR->GetMaxExpPoint(GetLevel()-minuslevel) - 1;
			ASSERT(minuslevel<2);	//EÂ¢Â´Â¨Ã¶AÂ©Ã¸Â¨Â£ CÂ¨ÂªÂ¨Ã¹Â¡Â©
			if(minuslevel > 3)		//EÂ¢Â´Â¨Ã¶AÂ©Ã¸Â¨Â£ CIÂ¢Â¥A... Â©Ã¶Â¡Ã¬CNÂ¡Â¤cCA Â©Ã¶Â©Â¡AoÂ¢Â¯e
				break;
		}
		else
		{
			CurExp -= minusExp;
			break;
		}
	}

	if(minuslevel > 0)
	{
		SetLevel(m_HeroCharacterInfo.Level-minuslevel);

		// character info log
		InsertLogCharacter( GetID(), m_HeroCharacterInfo.Level, &m_HeroInfo );
	}

	SetPlayerExpPoint(CurExp);
}

BYTE CPlayer::GetLifePercent()
{
	return BYTE(GetLife() / (float)GetMaxLife() * 100);
}

BYTE CPlayer::GetManaPercent()
{
	return BYTE(GetMana() / (float)GetMaxMana() * 100);
}

void CPlayer::OnEndObjectState(EObjectState State)
{
	switch(State)
	{
	case eObjectState_Die:
		GetBattle()->OnTeamMemberRevive(GetBattleTeam(),this);
		m_bNeedRevive = TRUE;
		break;
	}

}

// 090204 LUJ, ÃÂ¸ÃÃÃÂ» Â¸Ã­ÃÂ®ÃÃ· ÃÃ
eWeaponType CPlayer::GetWeaponEquipType()
{
	const ITEM_INFO* const pItemInfo = ITEMMGR->GetItemInfo( GetWearedWeapon() );

	return pItemInfo ? eWeaponType( pItemInfo->WeaponType ) : eWeaponType_None;
}

// 080703 LUJ, Â¹ÃÃÂ¯ ÃÂ¸ÃÃÃÂ» enumÃÂ¸Â·Ã ÂºÂ¯Â°Ã¦
eWeaponAnimationType CPlayer::GetWeaponAniType()
{
	const ITEM_INFO* leftInfo	= ITEMMGR->GetItemInfo( GetWearedItemIdx( eWearedItem_Weapon ) );
	const ITEM_INFO* rightInfo	= ITEMMGR->GetItemInfo( GetWearedItemIdx( eWearedItem_Shield ) );

	const eWeaponAnimationType	leftType	= eWeaponAnimationType( leftInfo ? leftInfo->WeaponAnimation : eWeaponAnimationType_None );
	const eWeaponAnimationType	rightType	= eWeaponAnimationType( rightInfo ? rightInfo->WeaponAnimation : eWeaponAnimationType_None );

	// 080703 LUJ, Â¾Ã§Â¼ÃÃÃ Â¹Â«Â±Ã¢Â°Â¡ Â´ÃÂ¸Â£Â¸Ã© ÃÃÂµÂµÂ·Ã¹Â°Â¡ Â¾ÃÂ´ÃÂ´Ã. Â¿ÃÂ¼ÃÂ¿Â¡ Â¹Â«Â±Ã¢Â¸Â¦ Â¾Ã ÂµÃ©Â¾ÃºÃÂ» Â°Ã¦Â¿Ã¬ÂµÂµ Â¸Â¶ÃÃ¹Â°Â¡ÃÃ¶ÃÃÂ´Ã.
	if( leftType != rightType ||
		leftType == eWeaponAnimationType_None )
	{
		return leftType;
	}

	return eWeaponAnimationType_TwoBlade;
}

void CPlayer::ReviveAfterShowdown( BOOL bSendMsg )
{
	ClearMurderIdx();
	m_bNeedRevive = TRUE;

	if( bSendMsg )
	{
		MOVE_POS msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
		msg.dwObjectID = GetID();
		msg.dwMoverID = GetID();
	
		msg.cpos.Compress(CCharMove::GetPosition(this));
		
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	}

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);

	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYManaRecoverTime.bStart = FALSE;
	SetLife( GetMaxLife() * 30 / 100 );	//Â¡Â§uoAÂ¡ÃÂ¢Ã§ Â¡Â§uiÂ¡ÃOÂ¢Â®iÂ¢Â®Â¨Â¡O COÂ¢Â®Â¨Ãºi.
}
	
// 080602 LYW --- Player : Â°Ã¦ÃÃ¨ÃÂ¡ Â¼Ã¶ÃÂ¡ (__int32) Â¿Â¡Â¼Â­ (__int64) Â»Ã§ÃÃÃÃ®Â·Ã ÂºÂ¯Â°Ã¦ ÃÂ³Â¸Â®.
//DWORD CPlayer::RevivePenalty(BOOL bAdditionPenalty)								// ÃÂ¦ÃÃÂ¸Â® ÂºÃÃÂ°Â½Ã ÃÃÂ°Â¡ Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ¶Ã´ÃÂ» ÃÂ³Â¸Â®ÃÃÂ´Ã ÃÃÂ¼Ã¶.
EXPTYPE CPlayer::RevivePenalty(BOOL bAdditionPenalty)								// ÃÂ¦ÃÃÂ¸Â® ÂºÃÃÂ°Â½Ã ÃÃÂ°Â¡ Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ¶Ã´ÃÂ» ÃÂ³Â¸Â®ÃÃÂ´Ã ÃÃÂ¼Ã¶.
{
	// desc_hseos_ÂµÂ¥ÃÃÃÂ® ÃÂ¸_01
	// S ÂµÂ¥ÃÃÃÂ® ÃÂ¸ ÃÃÂ°Â¡ added by hseos 2007.11.30
	// ..ÃÂ§Â¸Â°ÃÃ¶ ÃÂ¸Â¿Â¡Â¼Â­ ÃÃÂ¾Ã®Â¼Â­ Â¸ÃÂ¾ÃÂ¿Ã´ÃÃ Â¶Â§Â´Ã ÃÃ¤Â³ÃÃÂ¼ Â¾Ã¸ÃÂ½
	if (g_csDateManager.IsChallengeZoneHere())
	{
		return FALSE;
	}
	// E ÂµÂ¥ÃÃÃÂ® ÃÂ¸ ÃÃÂ°Â¡ added by hseos 2007.11.30

	// Â±Ã¦ÃÂ® ÃÃ¤Â³ÃÂ¸ÃÃÂ®Â½Ã ÃÃÃÂ»Â°Ã¦Â¿Ã¬ ÃÃÂ³ÃÃÂ¼ Â¾Ã¸ÃÂ½.
	if( g_pServerSystem->GetMapNum() == GTMAPNUM)
	{
		return FALSE;
	}

	DWORD PenaltyNum = 0 ;														// ÃÃÂ°Â¡ Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ¶Ã´ÃÂ» 2%Â·Ã Â°Ã­ÃÂ¤ÃÃÂ´Ã.

	if( bAdditionPenalty )
	{
		PenaltyNum = 1 ;
	}
	else
	{
		PenaltyNum = random(1, 3) ;
	}
	
	// 071217 KTH --- StatusÂ¿Â¡ ProtectExpÃÃ ÃÂ¿Â°ÃºÂ°Â¡ ÃÂ¸ÃÃ§ÃÃÂ¸Ã© Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ Â°Â¨Â¼Ã Â½ÃÃÂ°ÃÃ¶ Â¾ÃÂ´ÃÂ´Ã.//
	Status* pStatus;
	pStatus = this->GetBuffStatus();

	if( pStatus->IsProtectExp )
	{
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////////////////

	LEVELTYPE CurLevel = GetLevel() ;											// ÃÃÂ·Â¹ÃÃÂ¾Ã®ÃÃ ÃÃ¶ÃÃ§ Â·Â¹ÂºÂ§ÃÂ» Â±Â¸ÃÃÂ´Ã.

	EXPTYPE CurExp	= GetPlayerExpPoint() ;										// ÃÃÂ·Â¹ÃÃÂ¾Ã®ÃÃ ÃÃ¶ÃÃ§ Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ Â±Â¸ÃÃÂ´Ã.
	
	EXPTYPE GoalExp	= GAMERESRCMNGR->GetMaxExpPoint(CurLevel) ;					// ÃÃÂ·Â¹ÃÃÂ¾Ã®ÃÃ Â·Â¹ÂºÂ§Â¾Ã· Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ Â±Â¸ÃÃÂ´Ã. 

	//---KES CHECK : GoalExpÂ´Ã Â¸ÃÂ¿Ã¬ ÃÂ« Â¼Ã¶ÃÃÂ´Ã. * PeanltyNumÃÂ» ÃÃÃÂ» Â°Ã¦Â¿Ã¬ DWORDÂ¸Â¦ Â³ÃÂ¾Ã®Â°Â¥ Â¼Ã¶ ÃÃÂ´Ã.
	// 080602 LYW --- Player : Â°Ã¦ÃÃ¨ÃÂ¡ Â¼Ã¶ÃÂ¡ (__int32) Â¿Â¡Â¼Â­ (__int64) Â»Ã§ÃÃÃÃ®Â·Ã ÂºÂ¯Â°Ã¦ ÃÂ³Â¸Â®.
	//DWORD dwExpA = GoalExp * PenaltyNum ;										// ÃÃÂ³ÃÃÂ¼ Â¼Ã¶ÃÂ¡Â¸Â¦ Â±Â¸ÃÃÂ´Ã.
	EXPTYPE dwExpA = GoalExp * PenaltyNum ;										// ÃÃÂ³ÃÃÂ¼ Â¼Ã¶ÃÂ¡Â¸Â¦ Â±Â¸ÃÃÂ´Ã.
	
	EXPTYPE PenaltyExp = (EXPTYPE)(dwExpA / 100) ;								// ÃÃÂ³ÃÃÂ¼ Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ Â±Â¸ÃÃÂ´Ã.

	// 080602 LYW --- Player : Â°Ã¦ÃÃ¨ÃÂ¡ Â¼Ã¶ÃÂ¡ (__int32) Â¿Â¡Â¼Â­ (__int64) Â»Ã§ÃÃÃÃ®Â·Ã ÂºÂ¯Â°Ã¦ ÃÂ³Â¸Â®.
	//DWORD dwExp = 0 ;															// ÃÃ¼Â¼ÃÃÃ Â°Ã¦ÃÃ¨ÃÂ¡ ÂºÂ¯Â¼Ã¶Â¸Â¦ Â¼Â±Â¾Ã°ÃÃÂ°Ã­ 0ÃÂ¸Â·Ã Â¼Â¼ÃÃÃÃÂ´Ã.
	EXPTYPE dwExp = 0 ;															// ÃÃ¼Â¼ÃÃÃ Â°Ã¦ÃÃ¨ÃÂ¡ ÂºÂ¯Â¼Ã¶Â¸Â¦ Â¼Â±Â¾Ã°ÃÃÂ°Ã­ 0ÃÂ¸Â·Ã Â¼Â¼ÃÃÃÃÂ´Ã.

	BOOL bLevelDown = FALSE ;													// Â·Â¹ÂºÂ§ Â´ÃÂ¿Ã®Â¿Â©ÂºÃ ÂºÂ¯Â¼Ã¶Â¸Â¦ Â¼Â±Â¾Ã°ÃÃÂ°Ã­ FALSE Â¼Â¼ÃÃÃÂ» ÃÃÂ´Ã.

	if( CurExp >= PenaltyExp )													// ÃÃ¶ÃÃ§ Â°Ã¦ÃÃ¨ÃÂ¡Â°Â¡ ÃÃÂ³ÃÃÂ¼ Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ»Ã³ÃÃ Â°Ã¦Â¿Ã¬.
	{
		dwExp = CurExp - PenaltyExp ;											// ÃÃ¼Â¼ÃÃÃ Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.

		ASSERT( dwExp >= 0 ) ;													// Â°Ã¦ÃÃ¨ÃÂ¡Â´Ã 0ÃÃÂ»Ã³ÃÃÂ¾Ã®Â¾ÃÂ¸Â¸ ÃÃÂ´Ã.

		SetPlayerExpPoint( dwExp ) ;

		MSG_DWORDEX3 msg ;

		msg.Category	= MP_USERCONN ;											// ÃÂ«ÃÃÂ°Ã­Â¸Â®Â¸Â¦ MP_USERCONNÂ·Ã Â¼Â¼ÃÃÃÃÂ´Ã.
		msg.Protocol	= MP_USERCONN_CHARACTER_DOWNEXP_NOTICE ;				// ÃÃÂ·ÃÃÃ¤ÃÃÃÂ» Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ¶Ã´ÃÂ¸Â·Ã Â¼Â¼ÃÃÃÃÂ´Ã.
		msg.dwObjectID	= GetID() ;												// ÃÃÂ·Â¹ÃÃÂ¾Ã®ÃÃ Â¾ÃÃÃÂµÃ°Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
		msg.dweData1	= (DWORD)PenaltyNum ;									// ÃÃÂ³ÃÃÂ¼ Â¼Ã¶ÃÂ¡Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
		msg.dweData2	= dwExp ;												// ÃÃ¼Â¼ÃÃÃ Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.

		if( bAdditionPenalty )
		{
			msg.dweData3		= TRUE ;												// ÃÃÂ°Â¡ Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ¶Ã´ Â¿Â©ÂºÃÂ¸Â¦ TRUEÂ·Ã Â¼Â¼ÃÃÃÃÂ´Ã.
		}
		else
		{
			msg.dweData3		= FALSE ;												// ÃÃÂ°Â¡ Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ¶Ã´ Â¿Â©ÂºÃÂ¸Â¦ FALSEÂ·Ã Â¼Â¼ÃÃÃÃÂ´Ã.
		}

		SendMsg(&msg, sizeof(msg)) ;											// ÃÃÂ·Â¹ÃÃÂ¾Ã®Â¿Â¡Â°Ã Â¸ÃÂ½ÃÃÃ¶Â¸Â¦ ÃÃ¼Â¼ÃÃÃÂ´Ã.
	}
	else																		// ÃÃ¶ÃÃ§ ÃÃÂ·Â¹ÃÃÂ¾Ã®ÃÃ Â°Ã¦ÃÃ¨ÃÂ¡Â°Â¡ ÃÃÂ³ÃÃÂ¼ Â°Ã¦ÃÃ¨ÃÂ¡ÂºÂ¸Â´Ã ÃÃÃÂ»Â°Ã¦Â¿Ã¬.
	{
		bLevelDown = TRUE ;														// Â·Â¹ÂºÂ§ Â´ÃÂ¿Ã® Â¿Â©ÂºÃÂ¸Â¦ TRUEÂ·Ã Â¼Â¼ÃÃÃÃÂ´Ã.

		dwExp = PenaltyExp - CurExp ;											// ÃÃ¼Â¼ÃÃÃ Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.

		ASSERT( dwExp >= 0 ) ;													// Â°Ã¦ÃÃ¨ÃÂ¡Â´Ã 0ÃÃÂ»Ã³ÃÃÂ¾Ã®Â¾ÃÂ¸Â¸ ÃÃÂ´Ã.

		GoalExp = GAMERESRCMNGR->GetMaxExpPoint(CurLevel-1) ;					// ÃÃÂ´ÃÂ°Ã¨ Â³Â·ÃÂº Â·Â¹ÂºÂ§ÃÃ Â·Â¹ÂºÂ§Â¾Ã· Â¸Ã±ÃÂ¥ Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ Â±Â¸ÃÃÂ´Ã.
		SetLevel( CurLevel -1 ) ;												// ÃÃÂ·Â¹ÃÃÂ¾Ã®ÃÃ Â·Â¹ÂºÂ§ÃÂ» Â´ÃÂ¿Ã®ÃÃ Â·Â¹ÂºÂ§Â·Ã Â¼Â¼ÃÃÃÃÂ´Ã.
		SetPlayerExpPoint(GoalExp-dwExp) ;										// ÃÃÂ·Â¹ÃÃÂ¾Ã®ÃÃ Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.

		MSG_DWORDEX4 msg ;

		msg.Category	= MP_USERCONN ;											// ÃÂ«ÃÃÂ°Ã­Â¸Â®Â¸Â¦ MP_USERCONNÂ·Ã Â¼Â¼ÃÃÃÃÂ´Ã.
		msg.Protocol	= MP_USERCONN_CHARACTER_DOWNLEVEL_NOTICE ;				// ÃÃÂ·ÃÃÃ¤ÃÃÃÂ» Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ¶Ã´ÃÂ¸Â·Ã Â¼Â¼ÃÃÃÃÂ´Ã.
		msg.dwObjectID	= GetID() ;												// ÃÃÂ·Â¹ÃÃÂ¾Ã®ÃÃ Â¾ÃÃÃÂµÃ°Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
		msg.dweData1	= (DWORDEX)GetLevel() ;									// ÃÃÂ·Â¹ÃÃÂ¾Ã®ÃÃ Â·Â¹ÂºÂ§ÃÂ» Â¼Â¼ÃÃÃÃÂ´Ã.
		msg.dweData2	= GoalExp-dwExp ;										// ÃÃÂ·Â¹ÃÃÂ¾Ã®ÃÃ Â°Ã¦ÃÃ¨ÃÂ¡Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
		msg.dweData3	= (DWORDEX)PenaltyNum ;									// ÃÃÂ³ÃÃÂ¼ Â¼Ã¶ÃÂ¡Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.


		if( bAdditionPenalty )
		{
			msg.dweData4		= TRUE ;												// ÃÃÂ°Â¡ Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ¶Ã´ Â¿Â©ÂºÃÂ¸Â¦ TRUEÂ·Ã Â¼Â¼ÃÃÃÃÂ´Ã.
		}
		else
		{
			msg.dweData4		= FALSE ;												// ÃÃÂ°Â¡ Â°Ã¦ÃÃ¨ÃÂ¡ ÃÃÂ¶Ã´ Â¿Â©ÂºÃÂ¸Â¦ FALSEÂ·Ã Â¼Â¼ÃÃÃÃÂ´Ã.
		}

		SendMsg(&msg, sizeof(msg)) ;											// ÃÃÂ·Â¹ÃÃÂ¾Ã®Â¿Â¡Â°Ã Â¸ÃÂ½ÃÃÃ¶Â¸Â¦ ÃÃ¼Â¼ÃÃÃÂ´Ã.
	}

	// 080414 LUJ, Â°Ã¦ÃÃ¨ÃÂ¡ Â¼ÃÂ½Ã Â¶Â§ Â¼ÃÂ½ÃÂµÃ Â°Ã¦ÃÃ¨ÃÂ¡ Â¸Â¸ÃÂ­ Â·ÃÂ±ÃÂ¸Â¦ Â³Â²Â±Ã¤Â´Ã
	InsertLogExp(
		eExpLog_LosebyRevivePresent,
		GetID(),
		GetLevel(),
		PenaltyExp,
		dwExp,
		0,
		0,
		0 );

	return PenaltyExp;
}


void CPlayer::RevivePresentSpot()
{	
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	//Â¢Â®Â¢Â´cÂ¡Â§Â¢Â®AAÂ¨ÃÂ¡Ã¾Â¡Ã?Â¢Â®I
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}
	// 100111 LUJ, ÂºÃÃÂ° ÃÃÂ·Â¡Â±ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ ÂºÃÃÂ° ÂºÃÂ°Â¡Â´ÃÃÃ Â¼Ã¶ÂµÂµ ÃÃÂ´Ã
	else if(ReviveFlagTown == mReviveFlag)
	{
		MSG_BYTE message;
		ZeroMemory(
			&message,
			sizeof(message));
		message.Category = MP_USERCONN;
		message.Protocol = MP_USERCONN_CHARACTER_REVIVE_NACK;
		message.bData = 3;
		SendMsg(
			&message,
			sizeof(message));
		return;
	}

	m_bNeedRevive = TRUE;
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	msg.cpos.Compress(CCharMove::GetPosition(this));
		
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));

	LEVELTYPE curLevel = GetLevel() ;

	if( curLevel >= 10 && m_bNoExpPenaltyByPK == FALSE )
	{
		RevivePenalty(FALSE) ;
// --- skr 20012020
		//RevivePenalty(TRUE) ;
	
		if( !g_csDateManager.IsChallengeZoneHere() )
		{
			// 090204 LUJ, Â°Â¨Â¼Ã ÃÂ¸Â¼Ã¶Â¸Â¦ ÃÃ¶ÃÂ¤ÃÃ
			RemoveBuffCount( eBuffSkillCountType_Dead, 1 );
		}
	}

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);
	
	DWORD MaxLife = GetMaxLife();
	DWORD MaxMana = GetMaxMana();

	int nReviveVal = (int)(MaxLife*0.3) ;

	MSG_INT ReviveLife;
	ReviveLife.Category = MP_CHAR;
	ReviveLife.Protocol = MP_CHAR_LIFE_ACK;
	ReviveLife.dwObjectID = GetID();
	ReviveLife.nData = my_max(1, nReviveVal);
	SendMsg(&ReviveLife,sizeof(ReviveLife));			
		
	SendLifeToParty(
		nReviveVal);
		
	m_HeroCharacterInfo.Life = nReviveVal;

	// 070417 LYW --- Player : Modified setting mana when the character revived.
	DWORD dwManaRate = (DWORD)(MaxMana*0.3) ;
	if( GetMana() < dwManaRate )
	{
		SetMana(dwManaRate);
	}
	
	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYManaRecoverTime.bStart = FALSE;
	ClearMurderIdx();

	m_bDieForGFW = FALSE;
  
// --- skr  12/01/2020
  SetRelifeStart();
}

// 080602 LYW --- Player : Â°Ã¦ÃÃ¨ÃÂ¡ Â¼Ã¶ÃÂ¡ (__int32) Â¿Â¡Â¼Â­ (__int64) Â»Ã§ÃÃÃÃ®Â·Ã ÂºÂ¯Â°Ã¦ ÃÂ³Â¸Â®.
//DWORD CPlayer::ReviveBySkill()
void CPlayer::ReviveBySkill( cSkillObject* pSkillObject )
{	
	if( !pSkillObject )
		return;

	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	//Â¢Â®Â¢Â´cÂ¡Â§Â¢Â®AAÂ¨ÃÂ¡Ã¾Â¡Ã?Â¢Â®I
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	// 100211 ONS ÂºÃÃÂ°Â´Ã«Â»Ã³Â¿Â¡Â°Ã ÂºÃÃÂ°Â¿Â©ÂºÃÂ¸Â¦ Â¹Â¯Â´ÃÂ´Ã.
	// ÂºÃÃÂ°Â½ÂºÃÂ³ÃÂ» Â¼Â³ÃÂ¤ÃÃÂ´Ã.
	SetCurResurrectIndex( pSkillObject->GetSkillIdx() );
	
	// Â½ÂºÃÂ³OperatorÃÃÂ¸Â§ÃÂ» ÃÃ¼Â¼ÃÃÃÂ´Ã.
	CObject* pOperator = pSkillObject->GetOperator();
	if( !pOperator || 
		pOperator->GetObjectKind() != eObjectKind_Player )
	{
		return;
	}

	MSG_NAME msg;
	ZeroMemory(&msg, sizeof(MSG_NAME));
	msg.Category	= MP_SKILL;
	msg.Protocol	= MP_SKILL_RESURRECT_SYN;
	msg.dwObjectID	= GetID();
	SafeStrCpy(msg.Name, pOperator->GetObjectName(), MAX_NAME_LENGTH+1);
	SendMsg( &msg, sizeof(msg) );
}

// 100211 ONS ÂºÃÃÂ°Â´Ã«Â»Ã³ÃÃÂ°Â¡ Â¼Ã¶Â¶Ã´ÃÃ Â°Ã¦Â¿Ã¬ ÂºÃÃÂ°ÃÂ³Â¸Â®Â¸Â¦ Â½ÃÃÃ ÃÃÂ´Ã.
EXPTYPE CPlayer::OnResurrect()
{
	EXPTYPE exp = 0;
	m_bNeedRevive = TRUE;

	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	msg.cpos.Compress(CCharMove::GetPosition(this));
		
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));

	LEVELTYPE curLevel = GetLevel() ;

	if( curLevel >= 10 && m_bNoExpPenaltyByPK == FALSE )
	{
		exp = RevivePenalty(FALSE) ;

		if( !g_csDateManager.IsChallengeZoneHere() && g_pServerSystem->GetMapNum()!=GTMAPNUM)
		{
			// 090204 LUJ, Â°Â¨Â¼Ã ÃÂ¸Â¼Ã¶Â¸Â¦ ÃÃ¶ÃÂ¤ÃÃ
			RemoveBuffCount( eBuffSkillCountType_Dead, 1 );
		}
	}

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);
	
	DWORD MaxLife = GetMaxLife();
	DWORD MaxMana = GetMaxMana();

	int nReviveVal = (int)(MaxLife*0.3) ;

	MSG_INT ReviveLife;
	ReviveLife.Category = MP_CHAR;
	ReviveLife.Protocol = MP_CHAR_LIFE_ACK;
	ReviveLife.dwObjectID = GetID();
	ReviveLife.nData = my_max(1, nReviveVal);
	SendMsg(&ReviveLife,sizeof(ReviveLife));			
		
	SendLifeToParty(
		nReviveVal);
		
	m_HeroCharacterInfo.Life = nReviveVal;

	// 070417 LYW --- Player : Modified setting mana when the character revived.
	DWORD dwManaRate = (DWORD)(MaxMana*0.3) ;
	if( GetMana() < dwManaRate )
	{
		SetMana(dwManaRate);
	}
	
	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYManaRecoverTime.bStart = FALSE;
	ClearMurderIdx();

	m_bDieForGFW = FALSE;
  
// --- skr  12/01/2020
  SetRelifeStart();
  
	return exp;
}

void CPlayer::ReviveLogIn()
{	
	// ÃÂ³Â¸Â¯ÃÃÂ°Â¡ ÃÃÃÂº Â»Ã³ÃÃÂ°Â¡ Â¾ÃÂ´ÃÂ¸Ã©, ÂºÃÃÂ° Â½ÃÃÃ ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ´Ã.
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0) ;
		MSG_BYTE msg ;
		msg.Category	= MP_USERCONN ;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK ;
		msg.bData		= 1 ; //errorcode
		SendMsg( &msg, sizeof(msg) ) ;

		return ;
	}
	

	// Â·Ã§ÃÃ Â»Ã³ÃÃÂ¶Ã³Â¸Ã©, ÂºÃÃÂ° Â½ÃÃÃÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ´Ã.
	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )
	{
		MSG_BYTE msg ;
		msg.Category	= MP_USERCONN ;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK ;
		msg.bData		= 2 ; //errorcode
		SendMsg( &msg, sizeof(msg) ) ;

		return ;
	}


	// Â¾ÃÂ¿Ã´ÃÂ³Â¸Â®Â°Â¡ Â½ÃÃÃ ÂµÃÂ¾ÃºÃÂ¸Â¸Ã©, Â½ÃÃÃ ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ´Ã.
	if( IsExitStart() )
	{
		MSG_BYTE msg ;
		msg.Category	= MP_USERCONN ;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK ;
		msg.bData		= 4 ; //errorcode
		SendMsg( &msg, sizeof(msg) ) ;

		return ;
	}
	// 100111 LUJ, ÂºÃÃÂ° ÃÃÂ·Â¡Â±ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ ÂºÃÃÂ° ÂºÃÂ°Â¡Â´ÃÃÃ Â¼Ã¶ÂµÂµ ÃÃÂ´Ã
	else if(ReviveFlagHere == mReviveFlag)
	{
		MSG_BYTE message;
		ZeroMemory(
			&message,
			sizeof(message));
		message.Category = MP_USERCONN;
		message.Protocol = MP_USERCONN_CHARACTER_REVIVE_NACK;
		message.bData = 3;
		SendMsg(
			&message,
			sizeof(message));
		return;
	}

	// Â°Ã¸Â¼Âº Â±Ã¦ÂµÃ¥ Â´Ã¸ÃÃ¼ÃÃÃÃ¶ ÃÂ®ÃÃÃÃÂ´Ã.
	if( SIEGEDUNGEONMGR->IsSiegeDungeon(g_pServerSystem->GetMapNum()) )
	{
		ReviveLogIn_GuildDungeon() ;
	}
	else
	{
		ReviveLogIn_Normal() ;
	}
}





// 081210 LYW --- Player : Â°Ã¸Â¼ÂºÃÃ¼ Â±Ã¦ÂµÃ¥ Â´Ã¸ÃÃ¼Â¿Â¡Â¼Â­ÃÃ ÂºÃÃÂ° Â¹Â®ÃÂ¦Â·Ã ÃÃÃÃ ÂµÃÂ°Â¡ÃÃ¶ ÃÃÂ¼Ã¶Â¸Â¦ ÃÃÂ°Â¡ÃÃÂ´Ã.
//-------------------------------------------------------------------------------------------------
//	NAME		: ReviveLogIn_Normal
//	DESC		: ÃÃÂ¹ÃÃÃ»ÃÃ Â¾ÃÃÃ¼ÃÃ¶Â´Ã« ÂºÃÃÂ° ÃÃÂ¼Ã¶.
//	PROGRAMMER	: Yongs Lee
//	DATE		: December 10, 2008
//-------------------------------------------------------------------------------------------------
void CPlayer::ReviveLogIn_Normal()
{
	// ÂºÃÃÂ°ÃÂ¢ÃÃ ÃÃÂ¿Ã¤ÃÃÂ´ÃÂ°Ã­ Â¼Â¼ÃÃÃÃÂ´Ã.
	m_bNeedRevive = TRUE ;


	// ÂºÃÃÂ° Â¸ÃÂ½ÃÃÃ¶Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
	MOVE_POS msg ;

	msg.Category	= MP_USERCONN ;
	msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE ;

	msg.dwObjectID	= GetID() ;
	msg.dwMoverID	= GetID() ;


	// ÂºÃÃÂ° ÃÂ§ÃÂ¡Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
	VECTOR3* ppos ;
	VECTOR3 pos ;

	if(g_pServerSystem->GetMapNum() == GTMAPNUM)
	{
		const DWORD dwTeam = GetBattle()->GetBattleTeamID( this );
		ppos = GetBattle()->GetRevivePoint(dwTeam);
	}
	else
	{
		ppos = GAMERESRCMNGR->GetRevivePoint() ;
	}

	int temp ;
	temp	= rand() % 500 - 250 ;
	pos.x	= ppos->x + temp ;
	temp	= rand() % 500 - 250 ;
	pos.z	= ppos->z + temp ;
	pos.y	= 0 ;

	msg.cpos.Compress(&pos) ;
	
	CCharMove::SetPosition(this,&pos) ;

	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg)) ;
		

	// PlayerÃÃ ÃÃÃÂ½ Â»Ã³ÃÃÂ¸Â¦ ÃÃÃÂ¦ÃÃÂ´Ã.
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die) ;


	// ÂºÃÃÂ° ÃÃÂ³ÃÃÂ¼Â¸Â¦ ÃÃ»Â¿Ã«ÃÃÂ´Ã.
	const LEVELTYPE curLevel = GetLevel() ;
	
	if(	curLevel >= 10 && !m_bDieForGFW && m_bNoExpPenaltyByPK == FALSE )
	{
		RevivePenalty(FALSE) ;
		
		if( !g_csDateManager.IsChallengeZoneHere() && g_pServerSystem->GetMapNum()!=GTMAPNUM )
		{
			// 090204 LUJ, Â°Â¨Â¼Ã ÃÂ¸Â¼Ã¶Â¸Â¦ ÃÃ¶ÃÂ¤ÃÃ
            RemoveBuffCount( eBuffSkillCountType_Dead, 1 );
		}
	}


	// Â±Ã¦ÂµÃ¥ ÃÃ¤Â³ÃÂ¸ÃÃÂ® Â¿Â¹Â¿Ã ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ´Ã.
	m_bDieForGFW = FALSE ;
	m_dwRespawnTimeOnGTMAP = 0 ;
	m_dwImmortalTimeOnGTMAP = 0 ;
	

	// Â»Ã³ÃÂ²Â¿Â¡ ÂµÃ»Â¸Â¥ Â»Ã½Â¸Ã­Â·ÃÃÂ» Â¼Â¼ÃÃÃÃÂ´Ã.
	DWORD CurLife = GetMaxLife() ;

	int nReviveVal = 0 ;
	if(g_pServerSystem->GetMapNum() == GTMAPNUM)
		nReviveVal = (int)(CurLife*1.0) ;
	else
		nReviveVal = (int)(CurLife*0.3) ;

	MSG_INT ReviveLife ;
	ReviveLife.Category = MP_CHAR ;
	ReviveLife.Protocol = MP_CHAR_LIFE_ACK ;
	ReviveLife.dwObjectID = GetID() ;
	ReviveLife.nData = my_max(1, nReviveVal);
	SendMsg(&ReviveLife,sizeof(ReviveLife)) ;

	m_HeroCharacterInfo.Life = nReviveVal ;


	// Â»Ã³ÃÂ²Â¿Â¡ ÂµÃ»Â¸Â¥ Â¸Â¶Â³ÂªÂ·ÃÃÂ» Â¼Â¼ÃÃÃÃÂ´Ã.
	DWORD MaxMana = GetMaxMana() ;

	DWORD dwManaRate = 0 ;
	if(g_pServerSystem->GetMapNum() == GTMAPNUM)
		SetMana(MaxMana) ;
	else
	{
		dwManaRate = (DWORD)(MaxMana*0.3) ;
		if( GetMana() < dwManaRate )
		{
			SetMana(dwManaRate) ;
		}
	}


	// Â±Ã¦ÂµÃ¥ ÃÃ¤Â³ÃÂ¸ÃÃÂ® Â¿Â¹Â¿ÃÂ¸Â¦ ÃÂ³Â¸Â®ÃÃÂ´Ã.
	if(g_pServerSystem->GetMapNum() == GTMAPNUM)
	{
		WORD wCode = GetJobCodeForGT() ;
		m_dwImmortalTimeOnGTMAP = GTMGR->GetImmortalTimeByClass(wCode) ;
	}


	// Â¹Â«ÃÃ»Â»Ã³ÃÃ ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ´Ã.
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Immortal,0) ;
	// 06.08.29. RaMa.
	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 30000 ) ;
	
	m_YYLifeRecoverTime.bStart = FALSE ;
	m_YYManaRecoverTime.bStart = FALSE ;

	if(CParty* const pParty = PARTYMGR->GetParty(m_HeroInfo.PartyID))
	{
		SEND_PARTYICON_REVIVE msg;
		ZeroMemory(
			&msg,
			sizeof(msg));
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_REVIVEPOS;
		msg.dwMoverID = m_BaseObjectInfo.dwObjectID;
		msg.Pos.posX = (WORD)pos.x;
		msg.Pos.posZ = (WORD)pos.z;

		pParty->SendMsgToAll(
			&msg,
			sizeof(msg));
		SendLifeToParty(
			nReviveVal);
	}

	ClearMurderIdx();
  
// --- skr  12/01/2020
  SetRelifeStart();
  
}





//-------------------------------------------------------------------------------------------------
//	NAME		: ReviveLogIn_GuildDungeon
//	DESC		: Â°Ã¸Â¼Âº Â±Ã¦ÂµÃ¥Â´Ã¸ÃÃ¼Â¿Â¡Â¼Â­ÃÃ Â¾ÃÃÃ¼ÃÃ¶Â´Ã« ÂºÃÃÂ° ÃÃÂ¼Ã¶.
//	PROGRAMMER	: Yongs Lee
//	DATE		: December 10, 2008
//-------------------------------------------------------------------------------------------------
void CPlayer::ReviveLogIn_GuildDungeon()
{
	BYTE byCheckRevivePoint = TRUE ;

	// ÂºÃÃÂ°ÃÂ¢ÃÃ ÃÃÂ¿Ã¤ÃÃÂ´ÃÂ°Ã­ Â¼Â¼ÃÃÃÃÂ´Ã.
	m_bNeedRevive = TRUE ;


	// ÂºÃÃÂ° Â¸ÃÂ½ÃÃÃ¶Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
	MOVE_POS msg ;

	msg.Category	= MP_USERCONN ;
	msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE ;

	msg.dwObjectID	= GetID() ;
	msg.dwMoverID	= GetID() ;

	
	// PlayerÂ°Â¡ Â·Ã§Â½Â¨Â¼Âº Â±Ã¦ÂµÃ¥ Â¼ÃÂ¼ÃÃÃÃÃ¶ ÃÂ¦ÂºÃ¡Â¼Âº Â±Ã¦ÂµÃ¥ Â¼ÃÂ¼ÃÃÃÃÃ¶ ÃÂ®ÃÃÃÃÂ´Ã.
	VillageWarp* pRevivePoint	= NULL ;

	DWORD dwGuildID				= GetGuildIdx() ;
	if( dwGuildID == 0 )
	{
		char szMsg[512] = {0, } ;
		sprintf( szMsg, "This player is not in guild! - %s,%d", GetObjectName(), GetID() ) ;
		SIEGEWARFAREMGR->WriteLog(szMsg) ;

		// 081217 LYW --- Player : Â°Ã¸Â¼Âº Â±Ã¦ÂµÃ¥ Â´Ã¸ÃÃ¼Â¿Â¡Â¼Â­ Â±Ã¦ÂµÃ¥ Â¼ÃÂ¼Ã Â¾Ã¸ÃÃ Â»Ã§Â¸ÃÂ½Ã, ÃÂ³Â¸Â¯ÃÃ Â¼Â±ÃÃ ÃÂ­Â¸Ã©ÃÂ¸Â·Ã ÃÃÂµÂ¿ÃÃÂ´Ã ÃÂ³Â¸Â® ÃÃÂ°Â¡.
		MSGBASE msg ;

		msg.Category	= MP_SIEGEWARFARE ;
		msg.Protocol	= MP_SIEGEWARFARE_DIEINGUILDDUNGEON_NOTICE ;

		msg.dwObjectID	= GetID() ;

		SendMsg( &msg, sizeof(MSGBASE) ) ;

		byCheckRevivePoint = FALSE ;
	}

	DWORD dwGuildID_Rushen		= SIEGEWARFAREMGR->GetCastleGuildIdx(eNeraCastle_Lusen) ;
	DWORD dwGuildID_Zevyn		= SIEGEWARFAREMGR->GetCastleGuildIdx(eNeraCastle_Zebin) ;

	// Â·Ã§Â½Â¨Â¼Âº Â±Ã¦ÂµÃ¥Â¶Ã³Â¸Ã©,
	if( dwGuildID_Rushen == dwGuildID )
	{
		pRevivePoint = SIEGEWARFAREMGR->GetDGRP_Rushen() ;
	}
	// ÃÂ¦ÂºÃ¡Â¼Âº Â±Ã¦ÂµÃ¥Â¶Ã³Â¸Ã©,
	else
	{
		if( dwGuildID_Zevyn == dwGuildID )
		{
			pRevivePoint = SIEGEWARFAREMGR->GetDGRP_Zevyn() ;
		}
		else
		{
			char szMsg[512] = {0, } ;
			sprintf( szMsg, "Invalid guild idx! \n PLAYER_GUILD:%d / RUSHEN_GUILD:%d / ZEVYN_GUILD:%d", 
			dwGuildID, dwGuildID_Rushen, dwGuildID_Zevyn ) ;
			SIEGEWARFAREMGR->WriteLog(szMsg) ;

			// 081217 LYW --- Player : Â°Ã¸Â¼Âº Â±Ã¦ÂµÃ¥ Â´Ã¸ÃÃ¼Â¿Â¡Â¼Â­ Â±Ã¦ÂµÃ¥ Â¼ÃÂ¼Ã Â¾Ã¸ÃÃ Â»Ã§Â¸ÃÂ½Ã, ÃÂ³Â¸Â¯ÃÃ Â¼Â±ÃÃ ÃÂ­Â¸Ã©ÃÂ¸Â·Ã ÃÃÂµÂ¿ÃÃÂ´Ã ÃÂ³Â¸Â® ÃÃÂ°Â¡.
			MSGBASE msg ;

			msg.Category	= MP_SIEGEWARFARE ;
			msg.Protocol	= MP_SIEGEWARFARE_DIEINGUILDDUNGEON_NOTICE ;

			msg.dwObjectID	= GetID() ;

			SendMsg( &msg, sizeof(MSGBASE) ) ;

			byCheckRevivePoint = FALSE ;
		}
	}


	// ÂºÃÃÂ° ÃÂ§ÃÂ¡Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
	VECTOR3 pos ;
	int temp ;

	if( byCheckRevivePoint )
	{
		if( !pRevivePoint )
		{
			char szMsg[512] = {0, } ;
			sprintf( szMsg, "Failed to receive dungeon revive point! \n PLAYER_GUILD:%d / RUSHEN_GUILD:%d / ZEVYN_GUILD:%d", 
				dwGuildID, dwGuildID_Rushen, dwGuildID_Zevyn ) ;
			SIEGEWARFAREMGR->WriteLog(szMsg) ;
			return ;
		}

		temp	= rand() % 100 ;
		pos.x	= (pRevivePoint->PosX*100) + temp ;
		temp	= rand() % 100 ;
		pos.z	= (pRevivePoint->PosZ*100) + temp ;
		pos.y	= 0 ;
	}
	else
	{
		pos = *CCharMove::GetPosition(this) ;
	}

	msg.cpos.Compress(&pos) ;
	
	CCharMove::SetPosition(this,&pos) ;

	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg)) ;
		

	// PlayerÃÃ ÃÃÃÂ½ Â»Ã³ÃÃÂ¸Â¦ ÃÃÃÂ¦ÃÃÂ´Ã.
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die) ;


	// ÂºÃÃÂ° ÃÃÂ³ÃÃÂ¼Â¸Â¦ ÃÃ»Â¿Ã«ÃÃÂ´Ã.
	const LEVELTYPE curLevel = GetLevel() ;
	
	if(	curLevel >= 10 && !m_bDieForGFW && m_bNoExpPenaltyByPK == FALSE )
	{
		RevivePenalty(FALSE) ;
		
		if( !g_csDateManager.IsChallengeZoneHere() && g_pServerSystem->GetMapNum()!=GTMAPNUM )
		{
			// 090204 LUJ, Â°Â¨Â¼Ã ÃÂ¸Â¼Ã¶Â¸Â¦ ÃÃ¶ÃÂ¤ÃÃ
            RemoveBuffCount( eBuffSkillCountType_Dead, 1 );
		}
	}


	// Â»Ã³ÃÂ²Â¿Â¡ ÂµÃ»Â¸Â¥ Â»Ã½Â¸Ã­Â·ÃÃÂ» Â¼Â¼ÃÃÃÃÂ´Ã.
	DWORD CurLife = GetMaxLife() ;
	int nReviveVal = (int)(CurLife*0.3) ;

	MSG_INT ReviveLife ;
	ReviveLife.Category = MP_CHAR ;
	ReviveLife.Protocol = MP_CHAR_LIFE_ACK ;
	ReviveLife.dwObjectID = GetID() ;
	ReviveLife.nData = my_max(1, nReviveVal);
	SendMsg(&ReviveLife,sizeof(ReviveLife)) ;

	m_HeroCharacterInfo.Life = nReviveVal ;


	// Â»Ã³ÃÂ²Â¿Â¡ ÂµÃ»Â¸Â¥ Â¸Â¶Â³ÂªÂ·ÃÃÂ» Â¼Â¼ÃÃÃÃÂ´Ã.
	DWORD MaxMana = GetMaxMana() ;

	DWORD dwManaRate = (DWORD)(MaxMana*0.3) ;
	if( GetMana() < dwManaRate )
	{
		SetMana(dwManaRate) ;
	}


	// Â¹Â«ÃÃ»Â»Ã³ÃÃ ÃÂ³Â¸Â®Â¸Â¦ ÃÃÂ´Ã.
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Immortal,0) ;
	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 30000 ) ;
	
	m_YYLifeRecoverTime.bStart = FALSE ;
	m_YYManaRecoverTime.bStart = FALSE ;

	if(CParty* const pParty = PARTYMGR->GetParty(m_HeroInfo.PartyID))
	{
		SEND_PARTYICON_REVIVE msg;
		ZeroMemory(
			&msg,
			sizeof(msg));
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_REVIVEPOS;
		msg.dwMoverID = m_BaseObjectInfo.dwObjectID;
		msg.Pos.posX = (WORD)pos.x;
		msg.Pos.posZ = (WORD)pos.z;

		pParty->SendMsgToAll(
			&msg,
			sizeof(msg));
		SendLifeToParty(
			nReviveVal);
	}

	ClearMurderIdx();

// --- skr  12/01/2020
  SetRelifeStart();

}





void CPlayer::ReviveLogInPenelty()
{
	if( GetLevel() >= 10 && m_bNoExpPenaltyByPK == FALSE )
	{
		RevivePenalty(FALSE) ;
// --- skr 20012020
		//RevivePenalty(TRUE) ;
		
		if( !g_csDateManager.IsChallengeZoneHere() )
		{
			// 090204 LUJ, Â°Â¨Â¼Ã ÃÂ¸Â¼Ã¶Â¸Â¦ ÃÃ¶ÃÂ¤?			RemoveBuffCount( eBuffSkillCountType_Dead, 1 );
		}
	}

	DWORD CurLife = GetMaxLife();
	DWORD CurMana = GetMaxMana();

	// 080625 LYW --- Player : Â»Ã½Â¸Ã­Â·Ã Â¼Â¼ÃÃÃÂ» ÃÃÂ¶Ã³Â°Ã­ ÃÃÂ³Âª, ÃÂ³Â¸Â¯ÃÃÂ°Â¡ ÃÃÃÂº Â»Ã³ÃÃÃÃÂ±Ã¢ Â¶Â§Â¹Â®Â¿Â¡, 
	// Â»Ã½Â¸Ã­Â·Ã Â¼Â¼ÃÃ ÃÃÂ¼Ã¶Â°Â¡ Â±Ã¢Â´ÃÃÂ» ÃÂ¦Â´Ã«Â·Ã ÃÃÃÃ¶ Â¾ÃÂ´ÃÂ´Ã. Â¶Â§Â¹Â®Â¿Â¡, Â°Â­ÃÂ¦Â·Ã Â»Ã½Â¸Ã­Â·ÃÃÂ» Â¼Â¼ÃÃÃÃÂ´Ã 
	// ÃÃÂ¼Ã¶Â·Ã ÂºÂ¯Â°Ã¦ÃÃÂ´Ã.
	//SetLife((DWORD)(CurLife*0.3));
	//SetMana(0);

	// Â»Ã½Â¸Ã­Â·Ã Â¼Â¼ÃÃ.
	DWORD dwNewLife = 0 ;
	// 080710 LYW --- Player : Â»Ã½Â¸Ã­Â·Ã ÃÂ¸ÂºÂ¹ 50%Â·Ã Â¼Ã¶ÃÂ¤.
	//dwNewLife = (DWORD)(CurLife * 0.3f) ;
	dwNewLife = (DWORD)(CurLife * 0.5f) ;

	SetLifeForce(dwNewLife, TRUE) ;

	// Â¸Â¶Â³Âª Â¼Â¼ÃÃ.
	DWORD dwNewMana = 0 ;
	// 080710 LYW --- Player : Â¸Â¶Â³Âª ÃÂ¸ÂºÂ¹ 50%Â·Ã Â¼Ã¶ÃÂ¤.
	//dwNewMana = (DWORD)(CurMana* 0.3f) ;
	dwNewMana = (DWORD)(CurMana* 0.5f) ;

	SetManaForce(dwNewMana, TRUE) ;

	m_bDieForGFW = FALSE;
}


void CPlayer::DoDie(CObject* pAttacker)
{
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,pAttacker->GetID());

	m_bNoExpPenaltyByPK = FALSE;

	if( pAttacker->GetObjectKind() == eObjectKind_Pet )
	{
		CObject* const object = g_pUserTable->FindUser(
			pAttacker->GetOwnerIndex());

		if(0 == object)
		{
			return;
		}

		pAttacker = object;
	}

	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
		CBattle* pBattle = pAttacker->GetBattle();
		if(pBattle->GetBattleKind() == eBATTLE_KIND_NONE || pAttacker->GetBattleID() != GetBattleID() )
		{
			// for pk
			if( LOOTINGMGR->IsLootingSituation( this, pAttackPlayer ) )
			{
				LOOTINGMGR->CreateLootingRoom( this, (CPlayer*)pAttacker );
			}

			if(pAttackPlayer->IsPKMode() == TRUE)
			{
				m_MurdererIDX = pAttacker->GetID();
				m_MurdererKind = WORD(pAttacker->GetObjectKind());
				m_bNoExpPenaltyByPK = TRUE;
			}
			else if( GUILDWARMGR->JudgeGuildWar( this, (CPlayer*)pAttacker ) &&
				! m_bDieForGFW )
			{
				SetDieForGFW( TRUE );

				const CGuildManager::ScoreSetting& setting = GUILDMGR->GetScoreSetting();

				GuildUpdateScore( pAttackPlayer, this, setting.mKillerScore, setting.mCorpseScore );
			}

			SetReadyToRevive(TRUE);

		}
	}
	else if(pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		m_MurdererKind = ((CMonster*)pAttacker)->GetMonsterKind();
			
		// 080616 LUJ, ÃÃÃÂ¤ÃÃ Â°Ã¦Â¿Ã¬Â¿Â¡Â´Ã ÃÃ¤Â³ÃÃÂ¼Â¸Â¦ ÃÃÃÃ¶ Â¾ÃÂµÂµÂ·Ã ÃÃÂ´Ã
		if( pAttacker->GetObjectKind() != eObjectKind_Trap )
		{
			//SW060831 Â¹Ã¦ÃÃ ÂºÃ±Â¹Â« Â½Ã //Â°Â­ÃÂ¦ ÃÂ¾Â·Ã¡Â½Ã ÃÂ¼ÃÂ© ÂºÂ¯Â¼Ã¶
			SetPenaltyByDie(TRUE);
		}

		SetReadyToRevive(TRUE);

		// add quest event
		CQuestEvent QEvent( eQuestEvent_Die, g_pServerSystem->GetMapNum(), 0 );
		QUESTMGR->AddQuestEvent( this, &QEvent, 0 );
	}

	//KES EXIT
	ExitCancel();
	//KES EXCHANGE 031002
	EXCHANGEMGR->CancelExchange( this );
	STREETSTALLMGR->UserLogOut( this );
	//KES PKLOOTING
	LOOTINGMGR->LootingCancel( this );
	//KES PKPlayerPanelty
	PKMGR->DiePanelty( this, pAttacker );

//---KES Aggro 070918
//---Â¾Ã®Â±ÃÂ·Ã Â¸Â®Â½ÂºÃÂ® Â¸Â±Â¸Â®Â½Âº
	RemoveAllAggroed();	//*ÃÃÃÃ: Â¾ÃÂ·Â¡ FollowMonsterListÃÃÃÂ¦ÃÃÂ±Ã¢ ÃÃÃÃ¼Â¿Â¡ ÃÃÃÃÂ¾Ã®Â¾Ã, Â´ÃÃÂ½ Â¾Ã®Â±ÃÂ·ÃÂ¸Â¦ ÃÂ¸Â°ÃÃÂ¸Â·Ã ÃÃ¢ÃÂ» Â¼Ã¶ ÃÃÂ´Ã.
//-------------------

	CMonster * pObject = NULL;
	m_FollowMonsterList.SetPositionHead();
	while((pObject = (CMonster *)m_FollowMonsterList.GetData())!= NULL)
	{
		pObject->SetTObject(NULL);
	}
	m_FollowMonsterList.RemoveAll();

	//---KES ÃÃÃÂ¸Â¸Ã© ÃÃÂµÂ¿ÃÂ» Â¸ÃÃÃ§ÃÃÂ¾Ã®Â¾Ã ÃÃÂ´Ã.
	if( CCharMove::IsMoving(this) )
	{
		VECTOR3 pos;
		GetPosition( &pos );
		CCharMove::EndMove( this, gCurTime, &pos );
	}

	QUESTMAPMGR->DiePlayer( this );

	// desc_hseos_ÂµÂ¥ÃÃÃÂ® ÃÂ¸_01
	// S ÂµÂ¥ÃÃÃÂ® ÃÂ¸ ÃÃÂ°Â¡ added by hseos 2007.11.29
	g_csDateManager.SRV_EndChallengeZone(this, CSHDateManager::CHALLENGEZONE_END_ALL_DIE);
	// E ÂµÂ¥ÃÃÃÂ® ÃÂ¸ ÃÃÂ°Â¡ added by hseos 2007.11.29

	// 080725 KTH
	SIEGEWARFAREMGR->CancelWaterSeedUsing(this);

	{
		CPet* const petObject = PETMGR->GetPet(
			GetPetItemDbIndex());

		if(petObject)
		{
			petObject->DoDie(
				pAttacker);
		}
	}

	// 081020 LYW --- Player : Â°Ã¸Â¼ÂºÃÃ ÃÂ³Â¸Â¯ÃÃ Â»Ã§Â¸ÃÂ½Ã, AÂ°Â¡ BÂ¸Â¦ ÃÃÂ¿Â´Â´ÃÂ´Ã Â¸Ã Â°Ã¸ÃÃ¶ ÂºÃªÂ·ÃÂµÃ¥ÃÂ³Â½ÂºÃÃ ÃÂ³Â¸Â® ÃÃÂ°Â¡. - Â¼ÃÂ°Â¡Â¶Ã·.
	if( SIEGEWARFAREMGR->IsSiegeWarfareZone(g_pServerSystem->GetMapNum()) )
	{
		MSG_DWORD2 msg ;

		msg.Category	= MP_SIEGEWARFARE ;
		msg.Protocol	= MP_SIEGEWARFARE_NOTICE_KILLANDKILLER_SYN ;

		msg.dwObjectID	= 0 ;

		msg.dwData1		= pAttacker->GetID() ;
		msg.dwData2		= this->GetID() ;

		g_Network.Broadcast2AgentServer((char*)&msg, sizeof(MSG_DWORD2)) ;
	}

	SetSummonedVehicle( 0 );
	SetMountedVehicle( 0 );

	// 100621 ShinJS Â»Ã§Â¸ÃÂ½Ã ÃÃ¶ÃÃ§ Â½ÃÃÃ¼ÃÃÃÃ Â½ÂºÃÂ³ÃÂ» ÃÃ«Â¼ÃÂ½ÃÃÂ²Â´Ã.
	CancelCurrentCastingSkill( FALSE );
}

float CPlayer::DoGetMoveSpeed()
{
	if( GetAbnormalStatus()->IsMoveStop )
	{
		return 0;
	}

	// 090422 ShinJS --- ÃÂ»Â°ÃÃÃ MasterÃÃÂ°Ã­ ÃÂ¾Â½ÃÃÃÃÃ Â°Ã¦Â¿Ã¬ ÃÂ»Â°ÃÃÃ ÃÃÂµÂ¿Â¼ÃÂµÂµ ÃÃÂ¿Ã«
	{
		CObject* const vehicleObject = g_pUserTable->FindUser( GetSummonedVehicle() );

		if( vehicleObject &&
			GetSummonedVehicle() == GetMountedVehicle() )
		{
			return vehicleObject->GetMoveSpeed();
		}
	}

	float speed = float( m_MoveInfo.MoveMode == eMoveMode_Run ? RUNSPEED : WALKSPEED );

	// 080630 LUJ, Â¼Â¼ÃÂ® Â¾ÃÃÃÃÃ Â¼Ã¶ÃÂ¡Â°Â¡ ÃÃ»Â¿Ã«ÂµÃÂµÂµÂ·Ã ÃÃ
	float addrateval = ( GetRateBuffStatus()->MoveSpeed + GetRatePassiveStatus()->MoveSpeed ) / 100.f + m_itemBaseStats.mMoveSpeed.mPercent + m_itemOptionStats.mMoveSpeed.mPercent + m_SetItemStats.mMoveSpeed.mPercent;
	float addval = GetBuffStatus()->MoveSpeed + GetPassiveStatus()->MoveSpeed + m_itemBaseStats.mMoveSpeed.mPlus + m_itemOptionStats.mMoveSpeed.mPlus + m_SetItemStats.mMoveSpeed.mPlus;

	m_MoveInfo.AddedMoveSpeed = speed * addrateval + addval;

	speed += m_MoveInfo.AddedMoveSpeed;
	return my_max( 0, speed );
}


void CPlayer::SetInitedGrid()
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_GRIDINIT;
	SendMsg(&msg,sizeof(msg));

	CGridUnit::SetInitedGrid();
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(this->GetBattleID());
	if(pBattle && pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, this);

	{
		CQuestEvent QEvent( eQuestEvent_GameEnter, 0, 1 );
		QUESTMGR->AddQuestEvent( this, &QEvent );
	}

	{
		CQuestEvent QEvent( eQuestEvent_MapChange, 0, g_pServerSystem->GetMapNum() );
		QUESTMGR->AddQuestEvent( this, &QEvent );
	}

	// 090316 LUJ, ÃÂ»Â°ÃÂ¿Â¡ ÃÂ¾Â½ÃÃÃ ÃÂ¤Â·Ã Â¸Ã ÃÃÂµÂ¿ÃÃ Â°Ã¦Â¿Ã¬ ÃÃÂµÂ¿ÃÂ¸Â·Ã ÃÃÂ¿Ã¬Â±Ã¢ ÃÂ§ÃÃ ÃÂ¤ÂºÂ¸Â¸Â¦ Â°Â¡ÃÂ®Â¿ÃÂ´Ã
	LoadVehicleFromDb( GetID(), g_pServerSystem->GetMapNum() );
}
// RaMa - 04.11.10    -> ShopItemOption ÃÃÂ°Â¡   AvatarOptionÃÃÂ°Â¡(05.02.16)
DWORD CPlayer::DoGetCritical()
{	
	return (DWORD)mCriticalRate;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 06. 07 Â³Â»Â°Ã¸ ÃÃ»ÃÃ(ÃÃÂ°Ã) - ÃÃÂ¿ÂµÃÃ
DWORD CPlayer::DoGetDecisive()
{	
	return (DWORD)mCriticalRate;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CPlayer::DoGetPhyAttackPowerMin()
{
	return (DWORD)mPhysicAttackMin;
}

DWORD CPlayer::DoGetPhyAttackPowerMax()
{
	return (DWORD)mPhysicAttackMax;
}

void CPlayer::DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife)
{
	SetObjectBattleState( eObjectBattleState_Battle );
	if ( pAttacker )
	{
		pAttacker->SetObjectBattleState( eObjectBattleState_Battle );
		AddToAggroed(
			pAttacker->GetID());
	}

	if( GetAbnormalStatus()->IsSlip )
	{
		EndBuffSkillByStatus( eStatusKind_Slip );
	}

	// 090109 LUJ, ÃÃÂ°Ã Â½Ã ÃÂ³Â½ÂºÃÃ ÃÃÃÃ Â½ÂºÃÂ³ÃÃ ÃÃ«Â¼ÃÂµÃ Â¼Ã¶ ÃÃÂ´Ã
	CancelCurrentCastingSkill( TRUE );
}

void CPlayer::DoManaDamage( CObject* pAttacker, RESULTINFO* pDamageInfo, DWORD beforeMana )
{
	SetObjectBattleState( eObjectBattleState_Battle );
	if ( pAttacker )
	{
		pAttacker->SetObjectBattleState( eObjectBattleState_Battle );
	}

	if( GetAbnormalStatus()->IsSlip )
	{
		EndBuffSkillByStatus( eStatusKind_Slip );
	}

	// 090109 LUJ, ÃÃÂ°Ã Â½Ã ÃÂ³Â½ÂºÃÃ ÃÃÃÃ Â½ÂºÃÂ³ÃÃ ÃÃ«Â¼ÃÂµÃ Â¼Ã¶ ÃÃÂ´Ã
	CancelCurrentCastingSkill( TRUE );
}

void CPlayer::InitBaseObjectInfo(BASEOBJECT_INFO* pBaseInfo)
{
	ASSERT(GetInitState() == PLAYERINITSTATE_ONLY_ADDED);
	memcpy(&m_BaseObjectInfo,pBaseInfo,sizeof(BASEOBJECT_INFO));
}

/* Â¡Â§oCÂ¡Â§Â¡Ã¾?EÂ¢Â®Â¨ÃÂ¢Â®Â¢Â´Â¡Ãc Return */
MONEYTYPE CPlayer::SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag, eITEMTABLE tableIdx, BYTE LogType, DWORD TargetIdx )
{
	CPurse* pPurse = m_ItemContainer.GetPurse(tableIdx);
	if( !pPurse) return 0;

	MONEYTYPE Real = 0;
	if( bOper == MONEY_SUBTRACTION )
	{
		Real = pPurse->Subtraction( ChangeValue, MsgFlag );
	}
	else
	{
		Real = pPurse->Addition( ChangeValue, MsgFlag );
	}

	if(tableIdx == eItemTable_Inventory)
	{
		if( Real >= 10000 )
			InsertLogWorngMoney( LogType, GetID(), Real, GetMoney(), GetMoney(eItemTable_Storage), TargetIdx );
	}

	return Real;
}

/* Â¡ÃiÂ¢Â®Â¢Â´; Â¨ÃÂ©ÂªO; AÂ¨ÃÂ¢Â®Â¡Â§Â¡Ã¾Â¡Â§Â¡ÃCN Â¢Â®Â¨Â¡Â¨ÃÂ¨Â£Â¢Â®Â¨Â¡Â¢Â®IAI AOÂ¡ÃÂ¡ÃAAoÂ¡ÃÂ¡Ã¾| Â¨Ão?Â¡ÃÂ¡ÃAÂ¡ÃÂ¡ÃU.*/
BOOL CPlayer::IsEnoughAdditionMoney(MONEYTYPE money, eITEMTABLE tableIdx )
{
	CPurse* pPurse = m_ItemContainer.GetPurse(tableIdx);
	if(!pPurse) return FALSE;

	return pPurse->IsAdditionEnough( money );
}


MONEYTYPE CPlayer::GetMaxPurseMoney(eITEMTABLE TableIdx)
{
	CPurse* pPurse = m_ItemContainer.GetPurse(TableIdx);
	if( !pPurse) return FALSE;
	return pPurse->GetMaxMoney();
}

void CPlayer::SetMaxPurseMoney(eITEMTABLE TableIdx, DWORD MaxMoney)
{
	//CÂ¢Â®IÂ¢Â®Â¨ÃºÂ¨Ão; Â¢Â®icÂ¡ÃÂ¡Ã¾e Â¡ÃiÂ¢Â®Â¢Â´ Â¡Â§Â¡Ã¾Â¡ÃÂ¡Ã¾Â¢Â®Â¨Â¡u Â¡ÃÂ¡Ã¾Â¡Â§Â¢Â®Â¡Â§oÂ¡Â§Â¡Ã¾AÂ¡ÃÂ¢Ã§Â¢Â®Â¨Â¡Â¡ÃÂ¢Ã§ Â¡ÃÂ¡ÃAÂ¡Â§uiÂ¨ÃÂ©ÂªÂ¢Â®Â¨ÃÂ¡ÃÂ¡ÃU.
	if(TableIdx != eItemTable_Storage)
	{
		ASSERT(0);
		return;
	}
	CPurse* pPurse = m_ItemContainer.GetPurse(TableIdx);
	if( !pPurse) return;
	pPurse->SetMaxMoney(MaxMoney);
}

void CPlayer::AddQuest(CQuestBase* pQuest)
{
	if(CQuestBase* questBase = m_QuestList.GetData(pQuest->GetQuestIdx()))
	{
		m_QuestList.Remove(
			pQuest->GetQuestIdx());
		SAFE_DELETE(
			questBase);
	}
	m_QuestList.Add( pQuest, pQuest->GetQuestIdx() );
}

BOOL CPlayer::SetQuestState(DWORD QuestIdx, QSTATETYPE value)
{
	CQuestBase* pQuest;
	pQuest = m_QuestList.GetData(QuestIdx);

	if( !pQuest ) 
	{
//		char buff[256] = {0,};
//		sprintf(buff, "Â¡Ã?aÂ¢Â®Â¨ÃºaÂ¡ÃÂ¡ÃA AuÂ¡Â§oÂ¡Â§Â¡Ã¾Â¡Â§Â¢Â®Â¡ÃcÂ¢Â®Â¨Â¡Â¡ÃÂ¢Ã§ xAc Â¡Â§uECNÂ¡ÃÂ¡ÃUÂ¨ÃÂ©ÂªÂ¨ÃÂ¡Ã¾ CIÂ¡Ã?Â¡Â§IÂ¢Â®Â¨Â¡Â¢Â®I Â¨ÃoyÂ¡Â§uÂ¡ÃcÂ¡Ã?Â¡ÃÂ¢Ã§Â¢Â®Â¨Â¡O Â¡Â§uEÂ¢Â®Â¢Â´AAaÂ¡Ã?Â¢Â®Â¨Â¡ [QUEST ID : %d]", QuestIdx);
//		ASSERTMSG(0, buff);
		return FALSE;
	}

	pQuest->SetValue(value);
	BOOL bEnd = pQuest->IsComplete();

	// DBÂ¡Ã?Â¡ÃÂ¢Ã§ Â¡Â§uAÂ¡Â§Â¢Â®ACNÂ¡ÃÂ¡ÃU.
	QuestUpdateToDB( GetID(), QuestIdx, value, (BYTE)bEnd );

	if( bEnd )
	{
		m_QuestList.Remove(QuestIdx);

		MSG_DWORD msg;
		msg.Category = MP_QUEST;
		msg.Protocol = MP_QUEST_REMOVE_NOTIFY;
		msg.dwObjectID = GetID();
		msg.dwData = QuestIdx;
		SendMsg(&msg, sizeof(msg));

		SAFE_DELETE(pQuest);
	}

	return TRUE;
}

void CPlayer::SetInitState(int initstate,DWORD protocol)
{
	m_initState |= initstate;


	// 091106 LUJ, ÂºÂ¹ÃÃ¢ÃÃ ifÂ¹Â® ÃÂ³Â¸Â®Â¸Â¦ Â°Â£Â°Ã¡ÃÂ­
	if(FALSE == (m_initState & PLAYERINITSTATE_ONLY_ADDED))
	{
		return;
	}
	else if(FALSE == (m_initState & PLAYERINITSTATE_SKILL_INFO))
	{
		return;
	}
	else if(FALSE == (m_initState & PLAYERINITSTATE_ITEM_INFO))
	{
		return;
	}
	else if(FALSE == (m_initState & PLAYERINITSTATE_QUICK_INFO))
	{
		return;
	}
	else if(FALSE == (m_initState & PLAYERINITSTATE_HERO_INFO))
	{
		return;
	}
	else if(FALSE == (m_initState & PLAYERINITSTATE_QEUST_INFO))
	{
		return;
	}
	else if(FALSE == (m_initState & PLAYERINITSTATE_FARM_INFO))
	{
		return;
	}
	else if(FALSE == (m_initState & PLAYERINITSTATE_CHALLENGEZONE_INFO))
	{
		return;
	}
	else if(FALSE == (m_initState & PLAYERINITSTATE_STORAGE_INFO))
	{
		CharacterStorageInfo(GetUserID(), GetID());
		return;
	}
	else if(FALSE == (m_initState & PLAYERINITSTATE_OPTION_INFO))
	{
		return;
	}

	SetPeaceMode(g_pServerSystem->GetMap()->IsVillage());

	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(this, part);

		if( pTargetItemBase && 
			pTargetItemBase->dwDBIdx )
		{
			m_HeroCharacterInfo.WearedItemIdx[part-TP_WEAR_START] = pTargetItemBase->wIconIdx;
		}
		else
		{
			m_HeroCharacterInfo.WearedItemIdx[part-TP_WEAR_START] = 0;
		}
	}

	const ITEMBASE * pWeaponItemBase = ITEMMGR->GetItemInfoAbsIn(this, TP_WEAR_START + eWearedItem_Weapon );

	if( pWeaponItemBase && pWeaponItemBase->dwDBIdx )
	{
		const ITEM_OPTION& option = ITEMMGR->GetOption( *pWeaponItemBase );

		if( option.mItemDbIndex )
		{
			m_HeroCharacterInfo.WeaponEnchant = option.mEnchant.mLevel;
		}
	}

	m_dwProgressTime = gCurTime;

	// LUJ, ÃÂ¬Â¶Ã³ÃÃÂ¾Ã°ÃÂ®Â¿Â¡ ÃÃ¼Â¼ÃÃÃÂ±Ã¢ ÃÃ¼Â¿Â¡ Â°Ã¨Â»ÃªÃÃÂ¾ÃÃÃÂ´Ã
	CHARCALCMGR->Initialize(
		this);

	SEND_HERO_TOTALINFO msg;
	memset( &msg, 0, sizeof(msg) );
	GetBaseObjectInfo(&msg.BaseObjectInfo);
	GetCharacterTotalInfo(&msg.ChrTotalInfo);
	GetHeroTotalInfo(&msg.HeroTotalInfo);
	GetItemtotalInfo(
		msg.ItemTotalInfo,
		GETITEM_FLAG_INVENTORY | GETITEM_FLAG_WEAR);
	GetSendMoveInfo(&msg.SendMoveInfo,NULL);


	msg.ChrTotalInfo.CurMapNum = GAMERESRCMNGR->GetLoadMapNum();
	msg.UniqueIDinAgent = GetUniqueIDinAgent();

	SKILL_BASE SkillTreeInfo[MAX_SKILL_TREE] = {0};	
	m_SkillTree->SetPositionHead();

	for(SKILL_BASE* skill = m_SkillTree->GetData();
		0 < skill;
		skill = m_SkillTree->GetData())
	{
		if(msg.SkillNum > sizeof(SkillTreeInfo) / sizeof(*SkillTreeInfo))
		{
			break;
		}

		SkillTreeInfo[msg.SkillNum] = *skill;
		++msg.SkillNum;
	}

	msg.AddableInfo.AddInfo(
		CAddableInfoList::SkillTree,
		sizeof(*SkillTreeInfo) * msg.SkillNum,
		SkillTreeInfo,
		__FUNCTION__);
	msg.ChrTotalInfo.DateMatching = m_DateMatching;

	srand( GetTickCount());
	GetLocalTime(&msg.ServerTime);

	// 080827 LYW --- Player : Â°Ã¸Â¼Âº Â»Ã³ÃÃÂ¸Â¦ ÃÂ¯ÃÃºÂ¿Â¡Â°Ã(Client)Â·Ã ÃÃ¼Â¼ÃÃÃÂ´Ã.
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_GAMEIN_ACK;

	// 071227 LUJ
	msg.StorageSize	= GetStorageNum();
	SendMsg( &msg, msg.GetMsgLength() );

	MSG_DWORDEX2 msgFishingExp;
	msgFishingExp.Category = MP_FISHING;
	msgFishingExp.Protocol = MP_FISHING_EXP_ACK;
	msgFishingExp.dweData1 = (DWORDEX)m_wFishingLevel;
	msgFishingExp.dweData2 = m_dwFishingExp;
	SendMsg( &msgFishingExp, sizeof(msgFishingExp) );

	// 080424 NYJ --- Â¹Â°Â°Ã­Â±Ã¢ÃÃ·ÃÃÃÂ®
	MSG_DWORD msgFishPoint;
	msgFishPoint.Category = MP_FISHING;
	msgFishPoint.Protocol = MP_FISHING_POINT_ACK;
	msgFishPoint.dwData   = m_dwFishPoint;
	SendMsg( &msgFishPoint, sizeof(msgFishPoint) );

	// Â¿Ã¤Â¸Â®Â¼Ã·Â·ÃÂµÂµ
	MSG_DWORD4 msgCookState;
	msgCookState.Category = MP_COOK;
	msgCookState.Protocol = MP_COOK_STATE;
	msgCookState.dwData1 = GetCookLevel();
	msgCookState.dwData2 = GetCookCount();
	msgCookState.dwData3 = GetEatCount();
	msgCookState.dwData4 = GetFireCount();
	SendMsg( &msgCookState, sizeof(msgCookState) );

	// Â¿Ã¤Â¸Â®Â´ÃÃÃÂ·Â¹Â½ÃÃÃ
	int i;
	for(i=0; i<MAX_RECIPE_LV4_LIST; i++)
	{
		MSG_DWORD4	msgRecipe;
		msgRecipe.Category = MP_COOK;
		msgRecipe.Protocol = MP_COOK_UPDATERECIPE;
		msgRecipe.dwData1 = eCOOKRECIPE_ADD;
		msgRecipe.dwData2 = m_MasterRecipe[i].dwRecipeIdx;
		msgRecipe.dwData3 = i;
		msgRecipe.dwData4 = m_MasterRecipe[i].dwRemainTime;
		SendMsg( &msgRecipe, sizeof(msgRecipe) );
	}

	VECTOR3 pos;
	msg.SendMoveInfo.CurPos.Decompress(&pos);

	MSG_WORD3 msgMapDesc;
	msgMapDesc.Category		= MP_USERCONN;
	msgMapDesc.Protocol		= MP_USERCONN_MAPDESC;
	msgMapDesc.wData1		= (WORD)g_pServerSystem->GetMap()->IsVillage();
	// 090824 ONS GMÃÃ¸Â¿Â¡Â¼Â­ PKÃÂ¦Â¾Ã®Â½Ã Â¼Â³ÃÂ¤ÂµÃ PKÃÃ£Â¿Ã«Â°Âª.
	msgMapDesc.wData2		= (WORD)PKMGR->IsPKAllow();
	msgMapDesc.wData3		= (WORD)GetCurChannel();
	SendMsg( &msgMapDesc, sizeof(msgMapDesc) );

	const MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();

	RegistLoginMapInfo(
		GetID(),
		GetObjectName(),
		BYTE(GTMAPNUM == MapNum ? GTRETURNMAPNUM : MapNum),
		g_pServerTable->GetSelfServer()->wPortForServer);
	SetInited();
	if(PARTYMGR->CanUseInstantPartyMap(g_pServerSystem->GetMapNum()))
		PARTYMGR->ProcessReservationList(GetID());

	PARTYMGR->UserLogIn(
		this,
		MP_USERCONN_GAMEIN_SYN == protocol);
	QUICKMNGR->SendQuickInfo(
		this);

	// S Â¸Ã³Â½ÂºÃÃÂ¹ÃÃÃ ÃÃÂ°Â¡ added by hseos 2007.05.29
	{
		MSG_DWORD2 msg;
		msg.Category	= MP_CHAR;
		msg.Protocol	= MP_CHAR_MONSTERMETER_PLAYTIME;
		msg.dwData1		= m_stMonstermeterInfo.nPlayTime;
		msg.dwData2		= m_stMonstermeterInfo.nPlayTimeTotal;
		SendMsg(&msg, sizeof(msg));

		msg.Category	= MP_CHAR;
		msg.Protocol	= MP_CHAR_MONSTERMETER_KILLMONSTER;
		msg.dwData1		= m_stMonstermeterInfo.nKillMonsterNum;
		msg.dwData2		= m_stMonstermeterInfo.nKillMonsterNumTotal;
		SendMsg(&msg, sizeof(msg));
		// ÃÂ®Â¸Â®Â°ÃÂ¸Â¦ ÃÃÂ¾Ã®ÂµÃ©ÃÃÂ´Ã
		TRIGGERMGR->LoadTrigger(*this);
	}

	g_csFarmManager.SRV_SendPlayerFarmInfo(this);
	g_csDateManager.SRV_SendChallengeZoneEnterFreq(this);
	// 091106 LUJ, Â¸Â®Â¹ÃÃÂ® Â´Ã¸ÃÂ¯Â¿Â¡ ÂµÃ®Â·ÃÂ½ÃÃÂ²Â´Ã
	LIMITDUNGEONMGR->AddPlayer(*this);
	GUILDMGR->AddPlayer( this );
	GUILDWARMGR->AddPlayer( this );
	PassiveSkillCheckForWeareItem();
	PetInfoLoad(
		GetID(),
		GetUserID());
	CharacterBuffLoad(
		GetID());

	CCharMove::InitMove(
		this,
		&m_MoveInfo.CurPosition);

	if(CGridTable* const gridTable = g_pServerSystem->GetGridSystem()->GetGridTable(this))
	{
		gridTable->AddObject(
			this,
			CCharMove::GetPosition(this));
	}

	// 100408 ShinJS --- Â¼Â­Â¹Ã¶ Â½ÃÂ°Â£ ÃÃ¼Â¼Ã
	stTime64t serverTimeMsg;
	ZeroMemory( &serverTimeMsg, sizeof(serverTimeMsg) );
	serverTimeMsg.Category = MP_USERCONN;
	serverTimeMsg.Protocol = MP_USERCONN_GETSERVERTIME_ACK;
	serverTimeMsg.dwObjectID = GetID();

	_time64( &serverTimeMsg.time64t );

	SendMsg( &serverTimeMsg, sizeof(serverTimeMsg) );

	if(const DWORD gameRoomIndex = PCROOMMGR->GetGameRoomIndex(GetID()))
	{
		g_DB.FreeMiddleQuery(
			RLoadPCRoomPoint,
			GetID(),
			"EXEC dbo.MP_GAMEROOM_LOAD_POINT %d, %d, %d",
			GetID(),
			gameRoomIndex,
			g_pServerSystem->GetMapNum());
	}

	// 100525 NYJ - ÃÃÂ¸ÃÂ´Ã«ÃÃ  ÂµÃ®Â·ÃÂ»Ã³ÃÂ°Â¿Â¡ Â´Ã«ÃÃ Â½ÃÂ°Â£Â°Ã¦Â°ÃºÃÂ¼ÃÂ© Â¼Ã¶ÃÃ 
	Consignment_CheckDate(GetID());
	Note_CheckDate(GetID());

	// 100611 ONS Â·ÃÂ±ÃÃÃÂ½Ã ÃÂ¤ÃÃÂ±ÃÃÃ¶ ÃÂ¤ÂºÂ¸Â¸Â¦ Â·ÃÂµÃ¥ÃÃÂ´Ã.
	ForbidChatLoad(GetID());
}

int CPlayer::CanExitStart()	//~Â¢Â®Â¢Â´avÂ¢Â®Â¨Â¡C
{
//	if( GetState() != eObjectState_None && GetState() != eObjectState_Move )
//		return FALSE;
	if( IsPKMode() )
		return eEXITCODE_PKMODE;
	if( LOOTINGMGR->IsLootedPlayer(GetID()) )	//PKÂ¢Â®Â¢Â´cÂ¡Â§Â¢Â®A; Â¡ÃÂ¡ÃcCIÂ¡ÃÂ¡ÃA AÂ¨ÃÂ¡Ã¾AIÂ¢Â®Â¨Â¡Â¡ÃÂ¢Ã§?
		return eEXITCODE_LOOTING;

	if( GetState() == eObjectState_Exchange )	//Â¡Â¾Â©Ã¸E?AÂ©Â¬Â¢Â¯Â¡Ã AÂ¨ÃºÂ¡Â¤aCO Â¨Ã¹o Â¨ÃºÂ©ÂªÂ¢Â¥U.
		return eEXITCODE_NOT_ALLOW_STATE;

	if( GetState() == eObjectState_StreetStall_Owner ||
		GetState() == eObjectState_StreetStall_Guest )	//Â©Ã¸eAÂ¢Â®AÂ©Â¬Â¢Â¯Â¡Ã AÂ¨ÃºÂ¡Â¤aCO Â¨Ã¹o Â¨ÃºÂ©ÂªÂ¢Â¥U.
		return eEXITCODE_NOT_ALLOW_STATE;

	if( GetState() == eObjectState_Deal )	//Â¡Ã­oAÂ¢Â® AIÂ¢Â¯eAÂ©Â¬Â¢Â¯Â¡Ã AÂ¨ÃºÂ¡Â¤aCO Â¨Ã¹o Â¨ÃºÂ©ÂªÂ¢Â¥U.
		return eEXITCODE_NOT_ALLOW_STATE;
	
	return eEXITCODE_OK;
}

void CPlayer::SetExitStart( BOOL bExit )
{
	m_bExit				= bExit;
	m_dwExitStartTime	= gCurTime;
}

int CPlayer::CanExit()
{
	DWORD lCurTime = MHTIMEMGR_OBJ->GetNewCalcCurTime();
	if( lCurTime - m_dwExitStartTime < EXIT_COUNT*1000 - 2000 )	//8.0	//Â¹Ã¶ÃÃÂ¸Â¦ ÃÃ¦ÂºÃÃÃ· ÃÃ¢ÃÃ
		return eEXITCODE_SPEEDHACK;

	if( IsPKMode() )							//PKÂ¡ÃÂ¡Ã¾Â¨ÃÂ¡ÃÂ¡ÃiaAIÂ¢Â®Â¨Â¡Â¡ÃÂ¢Ã§?
		return eEXITCODE_PKMODE;
	if( LOOTINGMGR->IsLootedPlayer(GetID()) )	//PKÂ¢Â®Â¢Â´cÂ¡Â§Â¢Â®A; Â¡ÃÂ¡ÃcCIÂ¡ÃÂ¡ÃA AÂ¨ÃÂ¡Ã¾AIÂ¢Â®Â¨Â¡Â¡ÃÂ¢Ã§?
		return eEXITCODE_LOOTING;

//---KES AUTONOTE
	if( GetAutoNoteIdx() )
		return eEXITCODE_NOT_ALLOW_STATE;
//---------------
	
	return eEXITCODE_OK;
}

void CPlayer::ExitCancel()
{
	if( IsExitStart() )
	{
		SetExitStart( FALSE );
		MSG_BYTE msgNack;
		msgNack.Category	= MP_CHAR;
		msgNack.Protocol	= MP_CHAR_EXIT_NACK;
		msgNack.bData		= eEXITCODE_DIEONEXIT;
		SendMsg(&msgNack, sizeof(msgNack));		
	}
}

int CPlayer::PKModeOn()
{
	if( IsPKMode() ) return ePKCODE_ALREADYPKMODEON;		//AIÂ¨ÃoI PKÂ¡ÃÂ¡Ã¾Â¨ÃÂ¡ÃÂ¡Ãia
	if( IsShowdown() ) return ePKCODE_SHOWDOWN;		//Â¡Â§Â¡Ã¾nÂ¨ÃoÂ¢Â®iAÂ¨ÃÂ¡Ã¾AIÂ¡ÃÂ¡Ã¾e Â¡Â§uEÂ¡ÃiEÂ¡ÃÂ¡ÃU

	//pkÂ¢Â¬Â©Â£Â¥Ã¬a AÂ¡ÃÂ¢Â¬e Â©Ã¶Â¡Ã¬AuÂ¡Ã­oAA CÂ¨ÂªA|
	if( GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal );

	if( GetState() == eObjectState_Die )
		return ePKCODE_STATECONFLICT;	//Â¡ÃÂ¡ÃUÂ¡ÃÂ¡Ã¾Â¢Â®IÂ¢Â®ioAAAIÂ¡ÃOÂ¢Â®Â¡Â¿Â¡ÃÂ¡ÃA Â¡Â§uEÂ¡ÃiEÂ¡ÃÂ¡ÃU.
	
	m_HeroCharacterInfo.bPKMode = TRUE;
	m_dwPKModeStartTime			= gCurTime;

//---KES PK 071124
	m_dwPKContinueTime			= 20*60*1000 + ( GetBadFame() / 75 ) * 5*60*1000;	//Â±Ã¢ÂºÂ» 30ÂºÃ + Â¾ÃÂ¸Ã­ÃÂ¡ 75Â¸Â¶Â´Ã 5ÂºÃ
//----------------

	return ePKCODE_OK;
}

BOOL CPlayer::PKModeOff()
{
	if( !IsPKMode() ) return FALSE;

	if( gCurTime - m_dwPKModeStartTime >= m_dwPKContinueTime || ( GetUserLevel() <= eUSERLEVEL_GM && PKMGR->IsPKEvent() ) )
	{
		m_HeroCharacterInfo.bPKMode = FALSE;
		SetPKModeEndtime();
		PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );
		return TRUE;
	}

	return FALSE;
}

void CPlayer::PKModeOffForce()
{
	m_HeroCharacterInfo.bPKMode = FALSE;
}

void CPlayer::StateProcess()
{
	switch( GetState() )		
	{
	case eObjectState_None:
		{
			if( m_BaseObjectInfo.ObjectBattleState )
			{
				if( !mpBattleStateDelay->Check() )
				{
					SetObjectBattleState( eObjectBattleState_Peace );
				}
			}

			//in Korea : per 10min
			//in China : per 30min
			DWORD dwInsertLogTime = INSERTLOG_TIME;

			if( gCurTime - m_dwProgressTime >= dwInsertLogTime )
			{
				{
					CPet* const petObject = (CPet*)g_pUserTable->FindUser(
						GetPetItemDbIndex());

					if(petObject &&
						eObjectKind_Pet == petObject->GetObjectKind())
					{
						LogPet(
							petObject->GetObjectInfo(),
							ePetLogRegular);
					}
				}

				InsertLogExp(
					eExpLog_Time,
					GetID(),
					GetLevel(),
					0,
					GetPlayerExpPoint(),
					0,
					0,
					0);
				InsertLogMoney(
					0,
					GetID(),
					GetMoney(),
					0,
					0,
					0);

				if( IsPKMode())
				{
					m_HeroInfo.LastPKModeEndTime = 0;
				}
				
				UpdateCharacterInfoByTime(
					GetID(),
					GetPlayerExpPoint(),
					GetMoney(),
					m_HeroInfo.Playtime,
					m_HeroInfo.LastPKModeEndTime);
				MonsterMeter_Save(
					GetID(),
					m_stMonstermeterInfo.nPlayTime,
					m_stMonstermeterInfo.nKillMonsterNum,
					m_stMonstermeterInfo.nPlayTimeTotal,
					m_stMonstermeterInfo.nKillMonsterNumTotal);
				FishingData_Update(
					GetID(),
					GetFishingLevel(),
					GetFishingExp(),
					GetFishPoint());

				m_dwProgressTime = gCurTime;
			}

			m_ContinuePlayTime += gTickTime;
			DWORD dwPT = m_ContinuePlayTime/1000;
			if(dwPT)
			{
				m_HeroInfo.Playtime += dwPT;
				m_ContinuePlayTime -= dwPT*1000;
			}
		}
		break;
	case eObjectState_Die:
		{
			//---KES PK 071202	ÃÃÃÂº Â°Ã¦Â¿Ã¬ Â½ÃÂ°Â£ÃÂ» Â°Ã¨Â¼Ã Â¸Â®Â¼Ã (Â½ÃÂ°Â£ÃÃ Â¾ÃÂ°Â¡ÂµÂµÂ·Ã)
			SetPKStartTimeReset();
			
			if(FALSE == m_bNeedRevive )
			{
				break;
			}
			else if(g_pServerSystem->GetMapNum() == GTMAPNUM)
			{
				if(gTickTime < m_dwRespawnTimeOnGTMAP)
					m_dwRespawnTimeOnGTMAP -= gTickTime;
				else
				{
					MSGBASE message;
					message.Category	= MP_USERCONN;
					message.Protocol	= MP_USERCONN_READY_TO_REVIVE_BY_GFW;
					SendMsg( &message, sizeof( message ) );
					m_bNeedRevive = FALSE;
				}

				break;
			}

			DWORD dwElapsed = gCurTime - m_ObjectState.State_Start_Time;

			if( dwElapsed > PLAYERREVIVE_TIME )
			{
				if(TRUE == m_bDieForGFW)
				{
					MSGBASE message;
					message.Category	= MP_USERCONN;
					message.Protocol	= MP_USERCONN_READY_TO_REVIVE_BY_GFW;					
					SendMsg( &message, sizeof( message ) );

					m_bNeedRevive = FALSE;

					break;
				}

				if( LOOTINGMGR->IsLootedPlayer( GetID() ) )
					break;
				if( IsReadyToRevive() != TRUE )
					break;
				
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_READY_TO_REVIVE;
				SendMsg( &msg, sizeof(msg) );

				m_bNeedRevive = FALSE;
			}
		}
		break;
	}

	AddBadFameReduceTime();
	ProcMonstermeterPlayTime();
	g_csResidentRegistManager.SRV_ProcDateMatchingChatTimeOut(this);
	ProcFarmTime();
	g_csDateManager.SRV_Process(this);
	ProcCoolTime();
	ProcSpecialSkill();
}

void CPlayer::SetWearedItemIdx(DWORD WearedPosition,DWORD ItemIdx)
{	
	const int size = sizeof( m_HeroCharacterInfo.WearedItemIdx ) / sizeof( DWORD );

	if( size > WearedPosition )
	{
		m_HeroCharacterInfo.WearedItemIdx[WearedPosition] = ItemIdx;
	}
	else
	{
		ASSERT( 0 );
	}	
}

void CPlayer::QuestProcess()
{
	m_QuestGroup.Process();
}


void CPlayer::SetPKModeEndtime()
{
	m_HeroInfo.LastPKModeEndTime = 0;
}


void CPlayer::AddBadFameReduceTime()
{
	if( g_pServerSystem->GetMap()->IsVillage() )	return;
	if( GetState() == eObjectState_Immortal )		return;
	if( IsPKMode() )								return;

	m_HeroInfo.LastPKModeEndTime += gTickTime;

	//---KES PK 071202
	if( m_HeroInfo.LastPKModeEndTime > 90*60*1000 )
	{
		m_HeroInfo.LastPKModeEndTime -= 30*60*1000;	//
		PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );

        if( GetBadFame() > 5 )
		{
			SetBadFame( GetBadFame()-5 );
		}
		else
		{
			SetBadFame( 0 );
			m_HeroInfo.LastPKModeEndTime = 0;
		}

		BadFameCharacterUpdate( GetID(), GetBadFame() );
	}
}



void CPlayer::SpeedHackCheck()
{
	++m_nHackCount;

	if( gCurTime - m_dwHackStartTime >= 60*1000 )	//1ÂºÃ
	{
		if( m_nHackCount >= g_nHackCheckWriteNum )
		{
			InsertSpeedHackCheck( GetID(), GetObjectName(), m_nHackCount, 1 );
		}
		if( m_nHackCount >= g_nHackCheckNum )
		{
			MSGBASE Msg;
			Msg.Category = MP_HACKCHECK;
			Msg.Protocol = MP_HACKCHECK_BAN_USER_TOAGENT;
			SendMsg( &Msg, sizeof(Msg) );
		}
		m_nHackCount		= 1;
		m_dwHackStartTime	= gCurTime;
	}
}

void CPlayer::ClearMurderIdx()
{
	SetReadyToRevive(FALSE);
	m_MurdererKind = 0;
	m_MurdererIDX = 0;
	m_bPenaltyByDie = FALSE;
}


DWORD CPlayer::Damage(CObject* pAttacker,RESULTINFO* pDamageInfo)
{
	DWORD life = GetLife();

// --- skr 12/01/2020
  //if( IsRelifeON() ){ return life; }
 
	DWORD beforelife = life;

	if(life > pDamageInfo->RealDamage)
	{
		life -= pDamageInfo->RealDamage;
	}
	else
	{
		if( GetUserLevel() == eUSERLEVEL_GM || m_God ) // && g_pServerSystem->GetNation() == eNATION_KOREA )
		{
			life = 1;		//gmÃÂº ÂµÂ¥Â¹ÃÃÃ¶Â´Ã Â¹ÃÂ¾ÃÂµÂµ ÃÃÃÃ¶ Â¾ÃÂµÂµÂ·Ã
		}
		else
		{
			life = 0;
		}
	}
	
	SetLife(life,FALSE);
 
	DoDamage(pAttacker,pDamageInfo,beforelife);

	return life;
}

DWORD CPlayer::ManaDamage( CObject* pAttacker, RESULTINFO* pDamageInfo )
{
	DWORD mana = GetMana();
	DWORD beforemana = mana;
	mana = (mana > pDamageInfo->ManaDamage ? mana - pDamageInfo->ManaDamage : 0);
	
	SetMana( mana, FALSE );
 
	// Â¸Â¶Â³Âª ÂµÂ¥Â¹ÃÃÃ¶Â¸Â¸ ÃÃÂ´Ã Â°Ã¦Â¿Ã¬
	if( pDamageInfo->RealDamage == 0 )
		DoManaDamage( pAttacker, pDamageInfo, beforemana );

	return mana;
}

void CPlayer::SetGuildInfo(DWORD GuildIdx, BYTE Grade, const char* GuildName, MARKNAMETYPE MarkName)
{
	m_HeroCharacterInfo.MunpaID = GuildIdx;
	m_HeroCharacterInfo.PositionInMunpa = Grade;
	SafeStrCpy(m_HeroCharacterInfo.GuildName, GuildName, sizeof( m_HeroCharacterInfo.GuildName ) );
	m_HeroCharacterInfo.MarkName = MarkName;
}

void CPlayer::RSetMoney( MONEYTYPE money, BYTE flag )
{
	CPurse* pPurse = m_ItemContainer.GetPurse(eItemTable_Inventory);
	if( !pPurse ) return;

	pPurse->RSetMoney( money, flag );
	m_HeroInfo.Money = money;
}

void CPlayer::SetGuildMarkName(MARKNAMETYPE MarkName)
{
	m_HeroCharacterInfo.MarkName = MarkName;
}

char* CPlayer::GetGuildCanEntryDate()
{ 
	return m_HeroInfo.MunpaCanEntryDate;
}

void CPlayer::SetGuildCanEntryDate(char* date)
{
	SafeStrCpy(m_HeroInfo.MunpaCanEntryDate, date, 11);
}

void CPlayer::ClrGuildCanEntryDate()
{
	SafeStrCpy(m_HeroInfo.MunpaCanEntryDate, "2004.01.01", 11);
}

void CPlayer::UpdateLogoutToDB(BOOL val)
{
	if(CPet* const petObject = PETMGR->GetPet(GetPetItemDbIndex()))
	{
		if(eObjectState_Die != petObject->GetState())
		{
			petObject->SetPetObjectState(
				val ? ePetState_None : ePetState_Summon);
		}
	}

	for(int i = 0; i < _countof(m_QuickSlot); ++i)
	{
		m_QuickSlot[i].DBUpdate();
	}
}

void CPlayer::CheckImmortalTime()
{
	if(g_pServerSystem->GetMapNum() != GTMAPNUM)
		return;

	if(gTickTime<m_dwImmortalTimeOnGTMAP)
		m_dwImmortalTimeOnGTMAP-=gTickTime;
	else
	{
		m_dwImmortalTimeOnGTMAP = 0;

		MSG_DWORD2 msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_TEAMMEMBER_MORTAL;
		msg.dwObjectID = GetID();
		msg.dwData1 = GetID();
		msg.dwData2 = 0;
		
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	}
}

void CPlayer::SetNickName(char* NickName)
{
	SafeStrCpy(m_HeroCharacterInfo.NickName,NickName, MAX_GUILD_NICKNAME+1);
}

void CPlayer::SetFamilyNickName(char* NickName)
{
	SafeStrCpy(m_HeroCharacterInfo.FamilyNickName,NickName, sizeof( m_HeroCharacterInfo.FamilyNickName ) );
}

LEVELTYPE CPlayer::GetLevel()
{ 
	return m_HeroCharacterInfo.Level; 
}

DWORD CPlayer::GetLife() 
{ 
	return m_HeroCharacterInfo.Life; 
}

DWORD CPlayer::GetMana()
{ 
	return m_HeroInfo.Mana; 
}

DWORD CPlayer::DoGetMaxLife()
{ 
	return m_HeroCharacterInfo.MaxLife; 
}

DWORD CPlayer::DoGetMaxMana()
{ 
	return m_HeroInfo.MaxMana; 
}

void CPlayer::SetStage( BYTE grade, BYTE index )
{
	MSG_BYTE2 msg ;

	msg.Category	= MP_CHAR ;
	msg.Protocol	= MP_CHAR_STAGE_NOTIFY ;
	msg.dwObjectID	= GetID() ;
	msg.bData1		= grade ;
	msg.bData2		= index ;
	PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof(msg) );

	CharacterTotalInfoUpdate( this );	
}

WORD CPlayer::GetJobCodeForGT ()
{
	WORD JobCategory = m_HeroCharacterInfo.Job[0];
	WORD JobGrade = m_HeroCharacterInfo.JobGrade;
	WORD JobIndex = m_HeroCharacterInfo.Job[JobGrade - 1];

	if(JobGrade == 1)
		JobIndex = 1;

	WORD JobCode = JobCategory*100 + JobGrade*10 + JobIndex;

	return JobCode;
}

// 070415 LYW --- Plsyer : Add function to setting job.
void CPlayer::SetJob( BYTE jobGrade, BYTE jobIdx )
{
	MSG_BYTE2 msg ;

	msg.Category	= MP_CHAR ;
	msg.Protocol	= MP_CHAR_STAGE_NOTIFY ;
	msg.dwObjectID	= GetID() ;
	msg.bData1		= jobGrade ;
	msg.bData2		= jobIdx ;

	// 070522 LYW --- Player : Modified send job notify.
	//PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof(msg) );
	SendMsg( &msg, sizeof(msg) );

	m_HeroCharacterInfo.JobGrade = jobGrade ;
	m_HeroCharacterInfo.Job[jobGrade - 1] = jobIdx ;

	CharacterJobUpdate( GetID(), jobGrade,
						m_HeroCharacterInfo.Job[0],
						m_HeroCharacterInfo.Job[1],
						m_HeroCharacterInfo.Job[2],
						m_HeroCharacterInfo.Job[3],
						m_HeroCharacterInfo.Job[4],
						m_HeroCharacterInfo.Job[5] );


	// 071112 Â¿ÃµÃÃ, ÃÂ¬Â·Â¡Â½Âº Â·ÃÂ±ÃÂ¸Â¦ Â³Â²Â±Ã¤Â´Ã
	InsertLogJob( this, m_HeroCharacterInfo.Job[0], jobGrade, jobIdx );

	// 081022 KTH -- 
	CHARCALCMGR->AddPlayerJobSkill(this);

	WebEvent( GetUserID(), 2 );


	// 100113 ONS ÃÃÂ¹ÃÂµÃ®Â·ÃÃÂ¤ÂºÂ¸ÃÃ ÃÂ¬Â·Â¡Â½ÂºÃÂ¤ÂºÂ¸Â°Â¡ ÂºÂ¯Â°Ã¦ÂµÃÂ¾ÃºÃÂ» Â°Ã¦Â¿Ã¬ Â¿Â¡ÃÃÃÃ¼ÃÂ®Â·Ã ÃÃ¼Â´ÃÃÃÂ´Ã.
	CHARACTER_TOTALINFO TotalInfo;
	ZeroMemory(&TotalInfo, sizeof(TotalInfo));
	GetCharacterTotalInfo(&TotalInfo);

	WORD idx = 1;
	if( TotalInfo.JobGrade > 1 )
	{
		idx = TotalInfo.Job[TotalInfo.JobGrade - 1];
	}
	DWORD dwClass = ( TotalInfo.Job[0] * 1000 ) + ( ( TotalInfo.Race + 1 ) * 100 ) + ( TotalInfo.JobGrade * 10 ) + idx;

	MSG_DWORD Packet;
	ZeroMemory(&Packet, sizeof(Packet));
	Packet.Category	= MP_RESIDENTREGIST;
	Packet.Protocol	= MP_RESIDENTREGIST_REGIST_CHANGE;
	Packet.dwObjectID = GetID();
	Packet.dwData = dwClass;
	g_Network.Broadcast2AgentServer( ( char* )&Packet, sizeof( Packet ) );

	// 080225 LUJ, Â±Ã¦ÂµÃ¥ ÃÂ¸Â¿Ã¸ÃÃ Â°Ã¦Â¿Ã¬ ÃÃ·Â¾Ã· ÃÂ¤ÂºÂ¸ ÂºÂ¯Â°Ã¦ÃÂ» ÃÃ¼ Â¼Â­Â¹Ã¶Â¿Â¡ ÃÃ¼ÃÃÃÃÂ¾ÃÃÃÂ´Ã
	{
		CGuild* guild = GUILDMGR->GetGuild( GetGuildIdx() );

		if( ! guild )
		{
			return;
		}

		GUILDMEMBERINFO* member = guild->GetMemberInfo( GetID() );

		if( ! member )
		{
			return;
		}

		member->mJobGrade	= jobGrade;
		member->mRace		= m_HeroCharacterInfo.Race;
		memcpy( member->mJob, m_HeroCharacterInfo.Job, sizeof( member->mJob ) );

		SEND_GUILD_MEMBER_INFO message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_SET_MEMBER_TO_MAP;
		message.GuildIdx	= GetGuildIdx();
		message.MemberInfo	= *member;

		g_Network.Send2AgentServer( ( char* )&message, sizeof( message ) );

		GUILDMGR->NetworkMsgParse( message.Protocol, &message );
	}	
}

void CPlayer::SendPlayerToMap(MAPTYPE mapNum, float xpos, float zpos)
{
	MSG_DWORD3 msg ;														// Â¸ÃÂ½ÃÃÃ¶ Â±Â¸ÃÂ¶ÃÂ¼Â¸Â¦ Â¼Â±Â¾Ã°ÃÃÂ´Ã.
	memset(&msg, 0, sizeof(MSG_DWORD3)) ;									// Â¸ÃÂ½ÃÃÃ¶ ÃÃÂ±Ã¢ÃÂ­.

	msg.Category	= MP_USERCONN ;
	msg.Protocol	= MP_USERCONN_QUEST_CHANGEMAP_SYN ;						// ÃÂ«ÃÃÂ°Ã­Â¸Â®Â¿Ã ÃÃÂ·ÃÃÃ¤ÃÃÃÂ» Â¼Â¼ÃÃÃÃÂ´Ã.

	msg.dwObjectID	= GetID() ;												// Â¿ÃÂºÃªÃÂ§ÃÂ® Â¾ÃÃÃÂµÃ°Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.

	msg.dwData1		= (DWORD)mapNum ;										// ÂµÂµÃÃ¸ ÃÃ¶Â¿ÂªÃÃ Â¸Ã Â¹Ã¸ÃÂ£Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
	msg.dwData2		= (DWORD)xpos ;											// XÃÃÃÂ¥Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.
	msg.dwData3		= (DWORD)zpos ;											// ZÃÃÃÂ¥Â¸Â¦ Â¼Â¼ÃÃÃÃÂ´Ã.

	SendMsg( &msg, sizeof(MSG_DWORD3) ) ;									// Â¸ÃÂ½ÃÃÃ¶Â¸Â¦ ÃÃ¼Â¼ÃÃÃÂ´Ã.
}

DOUBLE CPlayer::GetPlayerTotalExpPoint()
{
	DOUBLE exp = 0;

	for(LEVELTYPE i=1; i<GetLevel(); ++i)
	{
		exp = exp + GAMERESRCMNGR->GetMaxExpPoint( i ) ;
	}

	exp += GetPlayerExpPoint();

	return exp;
}

void CPlayer::InitGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx )
{
	m_HeroCharacterInfo.dwGuildUnionIdx = dwGuildUnionIdx;
	strncpy( m_HeroCharacterInfo.sGuildUnionName, pGuildUnionName, MAX_GUILD_NAME+1 );
	m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx;
}

void CPlayer::SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx )
{
	m_HeroCharacterInfo.dwGuildUnionIdx = dwGuildUnionIdx;
	strncpy( m_HeroCharacterInfo.sGuildUnionName, pGuildUnionName, sizeof( m_HeroCharacterInfo.sGuildUnionName ) );
	m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx;

	MSG_NAME_DWORD3 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_PLAYER_INFO;
	Msg.dwData1 = GetID();
	Msg.dwData2 = dwGuildUnionIdx;
	Msg.dwData3 = dwMarkIdx;
	strncpy( Msg.Name, pGuildUnionName, sizeof( Msg.Name ) );

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &Msg, sizeof(Msg) );
}

float CPlayer::GetAccuracy()
{
	return mAccuracy;
}

float CPlayer::GetAvoid()
{
	return mAvoid;
}

// 100221 ShinJS --- ÂºÃ­Â·Â°Â°Ã¸Â½Ã Â¼Ã¶ÃÂ¤
float CPlayer::GetBlock()
{
	const float rate	= mRateBuffStatus.Block + mRatePassiveStatus.Block;
	float		bonus	= 0;

	switch( GetJop( 0 ) )
	{
		// 080910 LUJ, ÃÃÃÃÃÃ
	case 1:
		{
			bonus = 15.f;
			break;
		}
		// 080910 LUJ, Â·ÃÂ±Ã
	case 2:
		{
			bonus = 10.f;
			break;
		}
		// 080910 LUJ, Â¸ÃÃÃÃÃ¶
	case 3:
		{
			bonus = 5.f;
			break;
		}
		// 100218 ShinJS --- Â¸Â¶ÃÂ·
	case 4:
		{
			bonus = 9.f;
			break;
		}
	}

	return ( float( GetDexterity() / 27.f ) + rate + bonus );
}

float CPlayer::GetPhysicAttackMax()
{
	return mPhysicAttackMax;
}

float CPlayer::GetPhysicAttackMin()
{
	return mPhysicAttackMin;
}

float CPlayer::GetPhysicAttack()
{
	return float( random( mPhysicAttackMin, mPhysicAttackMax ) );;
}

float CPlayer::GetMagicAttackMax()
{
	return mMagicAttackMax;
}

float CPlayer::GetMagicAttackMin()
{
	return mMagicAttackMin;
}

float CPlayer::GetMagicAttack()
{
	return float( random( mMagicAttackMin, mMagicAttackMax ) );;
}

float CPlayer::GetPhysicDefense()
{
	return mPhysicDefense;
}

float CPlayer::GetMagicDefense()
{
	return mMagicDefense;
}

float CPlayer::GetCriticalRate()
{
	return mCriticalRate;
}

float CPlayer::GetCriticalDamageRate()
{
	return mCriticalDamageRate;
}

float CPlayer::GetCriticalDamagePlus()
{
	return mCriticalDamagePlus;
}

float CPlayer::GetMagicCriticalRate()
{
	return mMagicCriticalRate;
}

float CPlayer::GetMagicCriticalDamageRate()
{
	return mMagicCriticalDamageRate;
}

float CPlayer::GetMagicCriticalDamagePlus()
{
	return mMagicCriticalDamagePlus;
}

float CPlayer::GetLifeRecover()
{
	return mLifeRecover;
}

float CPlayer::GetManaRecover()
{
	return mManaRecover;
}

////////////////////////////////////////////////////////////////////////////
// 06. 07. Â»Ã³ÃÃÂ°Â­ÃÂ¦ÂºÂ¯Â°Ã¦ - ÃÃÂ¿ÂµÃÃ
// ÃÃÃÂ¤ÃÃÂ»Ã³ Â½ÂºÃÂ³ Â»Ã§Â¿Ã«Â½ÃÃÃÂ½Ã ÃÂ¯ÂºÂ°ÃÃ Â»Ã³ÃÃÂ¸Â¦ ÃÂ¦Â¿ÃÃÃ Â³ÂªÂ¸ÃÃÃ¶ Â»Ã³ÃÃÂ´Ã Â¸Ã°ÂµÃ ÃÃÂ±Ã¢ÃÂ­
// ÃÂ¤Â»Ã³ÃÃ»ÃÃ Â»Ã³ÃÃÂ¿Â¡Â¼Â­Â´Ã Â°ÂªÃÂ» ÃÃÂ±Ã¢ÃÂ­
// Â½ÃÃÃÂ°ÂªÃÂ» Â´Â©ÃÃ»ÃÃÃÃ¶ Â¾ÃÂ´ÃÂ´Ã.
// Â¿Â¬Â¼Ã Â½ÃÃÃÂ½ÃÂ¿Â¡Â¸Â¸ Â»Ã³ÃÃÃÃÃÂ¦
BOOL CPlayer::CanSkillState()
{
	//Â¿Â¬Â¼Ã 5ÃÂ¸ ÃÃÂ»Ã³ Â½ÃÃÃÂ½Ã
	if(m_SkillFailCount >= 5)
	{
		switch(m_BaseObjectInfo.ObjectState)
		{
		// Â½ÂºÃÂ³ÃÂ» Â»Ã§Â¿Ã«ÃÃÂ¼Ã¶ ÃÃÂ´Ã Â»Ã³ÃÃÂ¸Ã© Â°ÂªÃÂ» ÃÃÂ±Ã¢ÃÂ­ÃÃÂ°Ã­ TRUE Â¸Â®ÃÃ
		case eObjectState_None:
		case eObjectState_Move:
		case eObjectState_TiedUp_CanSkill:
			{
				m_SkillFailCount = 0;
				return TRUE;
			}
			break;
		// Â½ÂºÃÂ³ÃÂ» Â»Ã§Â¿Ã«ÃÃÂ¼Ã¶ Â¾Ã¸Â´Ã Â»Ã³ÃÃÃÃ ÃÂ®Â¾Ã®ÃÃ ÂµÂµ Â¹Â«Â¹Ã¦ÃÃ Â»Ã³ÃÃÂ¸Ã© Â°ÂªÃÂ» ÃÃÂ±Ã¢ÃÂ­ ÃÃÂ°Ã­
		// Â»Ã³ÃÃÂ¸Â¦ ÃÃÂ±Ã¢ÃÂ­ ÃÃÂµÃ TRUE Â¸Â®ÃÃ
		case eObjectState_SkillStart:
		case eObjectState_SkillSyn:	
		case eObjectState_SkillBinding:
		case eObjectState_SkillUsing:
		case eObjectState_SkillDelay:
		case eObjectState_Society:
		case eObjectState_Enter:
		case eObjectState_Rest:
			{
				m_SkillFailCount = 0;
				m_BaseObjectInfo.ObjectState = eObjectState_None;
				return TRUE;
			}
			break;
		// Â±Ã Â¿ÃÃÃ Â°Ã¦Â¿Ã¬Â¿Â£ Â°ÂªÃÂ» ÃÃÂ±Ã¢ÃÂ­ ÃÃÂ°Ã­ FALSE Â¸Â®ÃÃ
		default:
			{
				m_SkillFailCount = 0;
				return FALSE;
			}
			break;
		}
	}

	// 5ÃÂ¸ ÃÃÃÃ ÃÃÂ¶Â§ Â½ÂºÃÂ³ÃÂ» Â»Ã§Â¿Ã«ÃÃÂ¼Ã¶ Â¾Ã¸Â´Ã Â»Ã³ÃÃÂ¸Ã©
	// Â°ÂªÃÂ» ÃÃµÂ°Â¡ÃÃÂ°Ã­ FALSE Â¸Â®ÃÃ
	if(m_BaseObjectInfo.ObjectState != eObjectState_None &&
	   m_BaseObjectInfo.ObjectState != eObjectState_Move &&
	   m_BaseObjectInfo.ObjectState != eObjectState_TiedUp_CanSkill )
	{
		m_SkillFailCount++;
		return FALSE;
	}

	// ÃÂ¤Â»Ã³ Â»Ã³ÃÃÃÃÂ¶Â§ Â°ÂªÃÂ» ÃÃÂ±Ã¢ÃÂ­ÃÃÂ°Ã­ TRUE Â¸Â®ÃÃ
	m_SkillFailCount = 0;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////

BOOL CPlayer::AddQuick( BYTE sheet, WORD pos, SLOT_INFO* pSlot )
{
	return m_QuickSlot[ sheet ].Update( pos, pSlot );
}

SLOT_INFO*	CPlayer::GetQuick( BYTE sheet, WORD pos )
{
	return m_QuickSlot[ sheet ].GetQuick( pos );
}

// desc_hseos_Â¸Ã³Â½ÂºÃÃÂ¹ÃÃÃ01
// S Â¸Ã³Â½ÂºÃÃÂ¹ÃÃÃ ÃÃÂ°Â¡ added by hseos 2007.05.23	2007.07.08
void CPlayer::ProcMonstermeterPlayTime()
{
	if (gCurTime - m_stMonstermeterInfo.nPlayTimeTick > SHMath_MINUTE(1))
	{
		m_stMonstermeterInfo.nPlayTimeTick = gCurTime;
		m_stMonstermeterInfo.nPlayTime++;
		m_stMonstermeterInfo.nPlayTimeTotal++;

		// DBÂ¿Â¡ ÃÃºÃÃ¥
		//MonsterMeter_Save(GetID(), m_stMonstermeterInfo.nPlayTime, m_stMonstermeterInfo.nKillMonsterNum, m_stMonstermeterInfo.nPlayTimeTotal, m_stMonstermeterInfo.nKillMonsterNumTotal);

		// ÃÃÂ·Â¹ÃÃÂ½ÃÂ°Â£ÃÂº ÃÂ¬Â¶Ã³ÃÃÂ¾Ã°ÃÂ®Â¿Â¡Â¼Â­ÂµÂµ Â°Ã¨Â»Ãª Â°Â¡Â´ÃÃÃÂ¹ÃÂ·Ã ÃÂ¬Â¶Ã³ÃÃÂ¾Ã°ÃÂ®Â¿Â¡Â¼Â­ Â°Ã¨Â»ÃªÃÃÂµÂµÂ·Ã ÃÃÂ°Ã­
		// ÃÂ¤Â½Ã Â¸Ã°Â¸Â¦ Â¿ÃÃÃ·ÂÂÂ¹Â®Â¿Â¡ 10ÂºÃÂ¸Â¶Â´Ã Â¼Â­Â¹Ã¶ÃÃ Â¼Ã¶ÃÂ¡Â¸Â¦ ÂºÂ¸Â³Â»ÃÃÂ´Ã.
		if ((m_stMonstermeterInfo.nPlayTime%10) == 0)
		{
			// ÃÂ¬Â¶Ã³ÃÃÂ¾Ã°ÃÂ®Â¿Â¡ Â¾ÃÂ¸Â®Â±Ã¢
			MSG_DWORD2 msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_MONSTERMETER_PLAYTIME;
			msg.dwObjectID = GetID();
			msg.dwData1 = m_stMonstermeterInfo.nPlayTime;
			msg.dwData2 = m_stMonstermeterInfo.nPlayTimeTotal;
			SendMsg(&msg, sizeof(msg));
		}

		if( m_stMonstermeterInfo.nPlayTimeTotal == 30 * 60 )
		{
			WebEvent( GetUserID(), 6 );
		}
		// ÂºÂ¸Â»Ã³ ÃÂ³Â¸Â®
		g_csMonstermeterManager.ProcessReward(this, CSHMonstermeterManager::RBT_PLAYTIME, m_stMonstermeterInfo.nPlayTimeTotal);
	}
}
// E Â¸Ã³Â½ÂºÃÃÂ¹ÃÃÃ ÃÃÂ°Â¡ added by hseos 2007.05.23	2007.07.08

// desc_hseos_Â¸Ã³Â½ÂºÃÃÂ¹ÃÃÃ01
// S Â¸Ã³Â½ÂºÃÃÂ¹ÃÃÃ ÃÃÂ°Â¡ added by hseos 2007.05.23	2007.07.08
void CPlayer::ProcMonstermeterKillMon()
{
	m_stMonstermeterInfo.nKillMonsterNum++;
	m_stMonstermeterInfo.nKillMonsterNumTotal++;

	// DBÂ¿Â¡ ÃÃºÃÃ¥
	//MonsterMeter_Save(GetID(), m_stMonstermeterInfo.nPlayTime, m_stMonstermeterInfo.nKillMonsterNum, m_stMonstermeterInfo.nPlayTimeTotal, m_stMonstermeterInfo.nKillMonsterNumTotal);

	// ÃÂ¬Â¶Ã³ÃÃÂ¾Ã°ÃÂ®Â¿Â¡ Â¾ÃÂ¸Â®Â±Ã¢
	MSG_DWORD2 msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MONSTERMETER_KILLMONSTER;
	msg.dwObjectID = GetID();
	msg.dwData1 = m_stMonstermeterInfo.nKillMonsterNum;
	msg.dwData2 = m_stMonstermeterInfo.nKillMonsterNumTotal;
	SendMsg(&msg, sizeof(msg));

	if( m_stMonstermeterInfo.nKillMonsterNumTotal == 1000 )
	{
		WebEvent( GetUserID(), 9 );
	}

	// ÂºÂ¸Â»Ã³ ÃÂ³Â¸Â®
	g_csMonstermeterManager.ProcessReward(this, CSHMonstermeterManager::RBT_MONSTERKILL, m_stMonstermeterInfo.nKillMonsterNumTotal);
}

void CPlayer::ProcFarmTime()
{
	if (m_stFarmInfo.nCropPlantRetryTime)
	{
		if (gCurTime - m_stFarmInfo.nCropPlantRetryTimeTick > SHMath_SECOND(1))
		{
			m_stFarmInfo.nCropPlantRetryTimeTick = gCurTime;
			m_stFarmInfo.nCropPlantRetryTime--;

			// DBÂ¿Â¡ ÃÃºÃÃ¥
			Farm_SetTimeDelay(GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_PLANT, m_stFarmInfo.nCropPlantRetryTime);
		}
	}

	if (m_stFarmInfo.nCropManureRetryTime)
	{
		if (gCurTime - m_stFarmInfo.nCropManureRetryTimeTick > SHMath_SECOND(1))
		{
			m_stFarmInfo.nCropManureRetryTimeTick = gCurTime;
			m_stFarmInfo.nCropManureRetryTime--;

			// DBÂ¿Â¡ ÃÃº?			Farm_SetTimeDelay(GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_MANURE, m_stFarmInfo.nCropManureRetryTime);
		}
	}

	// 080430 KTH Animal Delay Add  (ÂºÃ)Â´ÃÃÂ§Â´Ã Â¿Â©Â±Ã¢Â¼Â­ Â¼Ã¶ÃÂ¡Â¸Â¦ Decrease ÃÃÃÃÂ´ÃÂ±Âº...
	if( m_stFarmInfo.nAnimalCleanRetryTime )
	{
		if( gCurTime - m_stFarmInfo.nAnimalCleanRetryTimeTick > SHMath_MINUTE(1) )
		{
			m_stFarmInfo.nAnimalCleanRetryTimeTick = gCurTime;
			m_stFarmInfo.nAnimalCleanRetryTime--;

			Farm_SetTimeDelay(GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_CLEAN, m_stFarmInfo.nAnimalCleanRetryTime);
		}
	}

	if( m_stFarmInfo.nAnimalFeedRetryTime )
	{
		if( gCurTime - m_stFarmInfo.nAnimalFeedRetryTimeTick > SHMath_SECOND(1) )
		{
			m_stFarmInfo.nAnimalFeedRetryTimeTick = gCurTime;
			m_stFarmInfo.nAnimalFeedRetryTime--;

			Farm_SetTimeDelay(GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_FEED, m_stFarmInfo.nAnimalFeedRetryTime);
		}
	}
}
// E Â³Ã³ÃÃ¥Â½ÃÂ½ÂºÃÃ ÃÃÂ°Â¡ added by hseos 2007.08.23

BOOL CPlayer::IsInventoryPosition( POSTYPE position )
{
	const CItemSlot* slot = GetSlot( eItemTable_Inventory );

    const POSTYPE begin = slot->GetStartPos();
	const POSTYPE end	= slot->GetSlotNum() + begin;

	return begin <= position && end >= position;
}


void CPlayer::ResetSetItemStatus()
{
	mSetItemLevel.clear();
	ZeroMemory(
		&m_SetItemStats,
		sizeof(m_SetItemStats));
}


const CPlayer::SetItemLevel& CPlayer::GetSetItemLevel() const
{
	return mSetItemLevel;
}


CPlayer::SetItemLevel& CPlayer::GetSetItemLevel()
{
	return mSetItemLevel;
}

void CPlayer::AddJobSkill( DWORD skillIndex, BYTE level )
{
	SKILL_BASE SkillBase;
	SkillBase.dwDBIdx = 0;
	SkillBase.wSkillIdx = skillIndex;
	SkillBase.Level = level;
	m_JobSkillList.push_back(SkillBase);

	AddSetSkill(skillIndex, level);
}

void CPlayer::ClearJobSkill()
{
	for(std::list< SKILL_BASE >::iterator iterator = m_JobSkillList.begin();
		iterator != m_JobSkillList.end();
		++iterator)
	{
		SKILL_BASE* skill = m_SkillTree->GetData(iterator->wSkillIdx);

		if( skill )
		{
			RemoveSetSkill(
				skill->wSkillIdx,
				skill->Level);
		}
	}

	m_JobSkillList.clear();
}

void CPlayer::AddSetSkill(DWORD skillIndex, LEVELTYPE level)
{
	SKILL_BASE skill = {0};
	skill.wSkillIdx = skillIndex;
	skill.Level = level;

	const SKILL_BASE* const oldSkill = m_SkillTree->GetData(
		skillIndex);

	if(oldSkill)
	{
		skill = *oldSkill;
		skill.Level = skill.Level + level;
	}

	m_SkillTree->Update(
		skill);
}


void CPlayer::RemoveSetSkill(DWORD skillIndex, LEVELTYPE level)
{
	SKILL_BASE* const oldSkill = m_SkillTree->GetData(
		skillIndex);

	if(0 == oldSkill)
	{
		return;
	}

	SKILL_BASE skill = *oldSkill;
	skill.Level = skill.Level - level;

	m_SkillTree->Update(
		skill);
}

// 090217 LUJ, Â¸Ã±ÃÃ»Â¿Â¡ Â¸ÃÂµÂµÂ·Ã ÃÃÂ¼Ã¶ ÃÃÂ¸Â§ ÂºÂ¯Â°Ã¦
void CPlayer::SetHideLevel( WORD level )
{ 
	m_HeroCharacterInfo.HideLevel = level; 
	
	if( level )
	{
		m_HeroCharacterInfo.bVisible = false;
	}
	else
	{
		m_HeroCharacterInfo.bVisible = true;
	}

	MSG_WORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_HIDE_NOTIFY;
	msg.dwObjectID = GetID();
	msg.wData = level;

	PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof( msg ) );	
}

// 090217 LUJ, Â¸Ã±ÃÃ»Â¿Â¡ Â¸ÃÂ°Ã ÃÃÂ¼Ã¶ ÃÃÂ¸Â§ ÂºÂ¯Â°Ã¦
void CPlayer::SetDetectLevel( WORD level )
{ 
	m_HeroCharacterInfo.DetectLevel = level; 
	
	MSG_WORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_DETECT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.wData = level;

	PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof( msg ) );	
}	

void CPlayer::RemoveAllAggroed()
{
	while(false == mAggroObjectContainer.empty())
	{
		const ObjectIndex objectIndex = *(mAggroObjectContainer.begin());
		mAggroObjectContainer.erase(
			objectIndex);

		CMonster* const monster = (CMonster*)g_pUserTable->FindUser(
			objectIndex);

		if(0 == monster ||
			FALSE == (eObjectKind_Monster & monster->GetObjectKind()))
		{
			continue;
		}

		monster->RemoveFromAggro(
			GetID());
		monster->GetAbnormalStatus()->Attract = 0;
	}
}

void CPlayer::AddAggroToMyMonsters(int nAggroAdd, DWORD targetObjectIndex, DWORD skillIndex)
{
	for(ObjectIndexContainer::const_iterator iterator = mAggroObjectContainer.begin();
		mAggroObjectContainer.end() != iterator;
		++iterator)
	{
		const ObjectIndex objectIndex = *iterator;
		CMonster* const monster = (CMonster*)g_pUserTable->FindUser(
			objectIndex);

		if(0 == monster ||
			FALSE == (eObjectKind_Monster & monster->GetObjectKind()))
		{
			continue;
		}

		monster->AddAggro(
			targetObjectIndex,
			nAggroAdd,
			skillIndex);
	}
}

// 080910 LUJ, Â¹Ã¦ÃÃÃÃ Â¹Ã¦Â¾Ã®Â·ÃÃÂ» Â¹ÃÃÂ¯ÃÃÂ´Ã
DWORD CPlayer::GetShieldDefense()
{
	return mShieldDefense;
}

// 080910 LUJ, Â¹Ã¦ÃÃÃÃ Â¹Ã¦Â¾Ã®Â·ÃÃÂ» Â¼Â³ÃÂ¤ÃÃÂ´Ã
void CPlayer::SetShieldDefence( DWORD shieldDefense )
{
	mShieldDefense = shieldDefense;
}

BOOL CPlayer::AddCoolTime( DWORD coolTimeGroupIndex, DWORD coolTime )
{
	ProcCoolTime();

	if(mCoolTimeMap.end() != mCoolTimeMap.find( coolTimeGroupIndex ))
	{
		return FALSE;
	}

	CoolTime& time = mCoolTimeMap[ coolTimeGroupIndex ];
	const DWORD tick = GetTickCount();

	time.mBeginTick = tick;
	time.mEndTick	= tick + coolTime;

	return TRUE;
}

void CPlayer::RemoveCoolTime( DWORD coolTimeGroupIndex)
{
	mCoolTimeMap.erase(
		coolTimeGroupIndex);
}


void CPlayer::ProcCoolTime()
{
	if(true == mCoolTimeMap.empty())
	{
		return;
	}

	const DWORD tick = GetTickCount();

	std::set< DWORD > group;

	for(CoolTimeMap::iterator it = mCoolTimeMap.begin(); mCoolTimeMap.end() != it; ++it)
	{
		const CoolTime& time = it->second;

		if( ( time.mBeginTick < time.mEndTick && time.mEndTick < tick ) ||
			( time.mBeginTick > time.mEndTick && time.mBeginTick > tick && time.mEndTick < tick ) )
		{
			// ÃÃÃÃ: Â»Ã¨ÃÂ¦ÃÃ ÃÃÃÃÂ¿Â¡Â´Ã Â¹ÃÂºÂ¹ÃÃÂ°Â¡ ÃÃÂ¸Ã¸ÂµÃÂ´ÃÂ´Ã Â»Ã§Â½ÃÃÂ» Â¸Ã­Â½Ã. ÂµÃ»Â¶Ã³Â¼Â­ Â´ÃÃÂ½ ÃÃÂ·ÃÂ¼Â¼Â½ÂºÂ¿Â¡Â¼Â­ ÃÂ³Â¸Â®ÃÃÂ¾Ã ÃÃÂ´Ã.
			group.insert( it->first );
		}
	}

	for( std::set< DWORD > ::const_iterator it = group.begin(); group.end() != it; ++it )
	{
		mCoolTimeMap.erase(*it);
	}
}

DWORD CPlayer::GetVitality() 
{ 
	const float rate =
		mRatePassiveStatus.Vit +
		mRateBuffStatus.Vit +
		m_itemBaseStats.mVitality.mPercent +
		m_itemOptionStats.mVitality.mPercent +
		m_SetItemStats.mVitality.mPercent * 100.f;
	const float plus =
		mPassiveStatus.Vit +
		mBuffStatus.Vit +
		m_itemBaseStats.mVitality.mPlus +
		m_itemOptionStats.mVitality.mPlus +
		m_SetItemStats.mVitality.mPlus;
	float Result = m_charStats.mVitality.mPlus * (1.f + rate / 100.f) + plus;

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)( Round( Result, 1 ) );
}

DWORD CPlayer::GetWisdom() 
{ 
	const float	rate =
		mRatePassiveStatus.Wis +
		mRateBuffStatus.Wis +
		m_itemBaseStats.mWisdom.mPercent +
		m_itemOptionStats.mWisdom.mPercent +
		m_SetItemStats.mWisdom.mPercent * 100.f;
	const float plus =
		mPassiveStatus.Wis +
		mBuffStatus.Wis +
		m_itemBaseStats.mWisdom.mPlus +
		m_itemOptionStats.mWisdom.mPlus +
		m_SetItemStats.mWisdom.mPlus;
	float Result = m_charStats.mWisdom.mPlus * (1.f + rate / 100.f) + plus;

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)( Round( Result, 1 ) );
}

DWORD CPlayer::GetStrength() 
{
	const float	rate =
		mRatePassiveStatus.Str +
		mRateBuffStatus.Str +
		m_itemBaseStats.mStrength.mPercent +
		m_itemOptionStats.mStrength.mPercent +
		m_SetItemStats.mStrength.mPercent * 100.f;
	const float	plus =
		mPassiveStatus.Str +
		mBuffStatus.Str +
		m_itemBaseStats.mStrength.mPlus +
		m_itemOptionStats.mStrength.mPlus +
		m_SetItemStats.mStrength.mPlus;
	float Result = m_charStats.mStrength.mPlus * (1.f + rate / 100.f) + plus;

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)( Round( Result, 1 ) );
}

DWORD CPlayer::GetDexterity() 
{ 
	const float rate =
		mRatePassiveStatus.Dex +
		mRateBuffStatus.Dex +
		m_itemBaseStats.mDexterity.mPercent +
		m_itemOptionStats.mDexterity.mPercent +
		m_SetItemStats.mDexterity.mPercent * 100.f;
	const float plus =
		mPassiveStatus.Dex +
		mBuffStatus.Dex +
		m_itemBaseStats.mDexterity.mPlus +
		m_itemOptionStats.mDexterity.mPlus +
		m_SetItemStats.mDexterity.mPlus;
	float Result = m_charStats.mDexterity.mPlus * (1.f + rate / 100.f) + plus;

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)( Round( Result, 1 ) );
}

DWORD CPlayer::GetIntelligence() 
{
	const float rate =
		mRatePassiveStatus.Int +
		mRateBuffStatus.Int +
		m_itemBaseStats.mIntelligence.mPercent +
		m_itemOptionStats.mIntelligence.mPercent +
		m_SetItemStats.mIntelligence.mPercent * 100.f;
	const float plus =
		mPassiveStatus.Int +
		mBuffStatus.Int +
		m_itemBaseStats.mIntelligence.mPlus +
		m_itemOptionStats.mIntelligence.mPlus +
		m_SetItemStats.mIntelligence.mPlus;
	float Result = m_charStats.mIntelligence.mPlus * (1.f + rate / 100.f) + plus;

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)( Round( Result, 1 ) );
}

void CPlayer::SetObjectBattleState(eObjectBattleState state)
{ 
	m_BaseObjectInfo.ObjectBattleState = state; 

	if( state )	//eObjectBattleState_Battle
	{
		mpBattleStateDelay->Start();
	}
}

void CPlayer::ProcessTimeCheckItem( BOOL bForceDBUpdate )
{
	DWORD dwElapsedMili = gCurTime - m_dwLastTimeCheckItem;
	if(  dwElapsedMili < 60 * 1000 && !bForceDBUpdate )
		return;

	DWORD dwElapsedSecond = dwElapsedMili / 1000;

	m_dwLastTimeCheckItem = gCurTime - ( dwElapsedMili - dwElapsedSecond * 1000 ) ;

	BOOL bDBUpdate = bForceDBUpdate ? TRUE : FALSE;

	if( gCurTime - m_dwLastTimeCheckItemDBUpdate > 5 * 60 * 1000 )
	{
		bDBUpdate = TRUE;
		m_dwLastTimeCheckItemDBUpdate = gCurTime;
	}

	int iCheckItemMaxNum = TP_WEAR_END;
	if( IsGotWarehouseItems() )
	{
		iCheckItemMaxNum = TP_STORAGE_END;
	}

	for( POSTYPE i = TP_INVENTORY_START ; i <= iCheckItemMaxNum ; ++i )
	{
		ITEMBASE* pItemBase = (ITEMBASE*)m_ItemContainer.GetItemInfoAbs( i );
		const ITEM_INFO* info = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );

		if(0 == info)
		{
			continue;
		}
		else if(info->SupplyType == ITEM_KIND_COOLTIME )
		{
			pItemBase->nRemainSecond = my_max( 0, int( pItemBase->nRemainSecond - dwElapsedSecond ) );
			pItemBase->ItemParam = gCurTime;

			UpdateRemainTime(
				GetID(),
				pItemBase->dwDBIdx,
				pItemBase->nSealed,
				pItemBase->nRemainSecond);
		}
		else if( pItemBase->nSealed == eITEM_TYPE_UNSEAL )
		{
			if( pItemBase->nRemainSecond > (int)dwElapsedSecond )
			{
				pItemBase->nRemainSecond -= dwElapsedSecond;

				// 071125 KTH --- Player "RemainSecondÂ°Â¡ 1ÂºÃ Â¹ÃÂ¸Â¸ÃÃ Â°Ã¦Â¿Ã¬ ÃÂ¬Â¶Ã³ÃÃÂ¾Ã°ÃÂ®Â¿Â¡Â°Ã ÃÂ¤ÂºÂ¸Â¸Â¦ ÂºÂ¸Â³Â»ÃÃÂ´Ã."
				if( pItemBase->nRemainSecond <= 60 )
				{
					MSG_DWORD2 msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_TIMELIMT_ITEM_ONEMINUTE;
					msg.dwData1 = pItemBase->wIconIdx;
					msg.dwData2 = pItemBase->Position;
	
					SendMsg(&msg, sizeof(msg));
				}

				if( bDBUpdate )
				{
					if( info->nTimeKind == eKIND_PLAYTIME )
					{
						UpdateRemainTime(
							GetID(),
							pItemBase->dwDBIdx,
							pItemBase->nSealed,
							pItemBase->nRemainSecond);
					}
				}
			}
			else
			{
				POSTYPE	position = pItemBase->Position;
				DWORD	iconIdx = pItemBase->wIconIdx;
				int returnValue = ITEMMGR->DiscardItem(
					this,
					pItemBase->Position,
					pItemBase->wIconIdx,
					pItemBase->Durability);

				if( EI_TRUE == returnValue )
				{
					CVehicle* const vehicle = ( CVehicle* )g_pUserTable->FindUser( GetSummonedVehicle() );

					// 090316 LUJ, ÃÃÂ´Ã§ Â¾ÃÃÃÃÃÃÂ¸Â·Ã Â¼ÃÃÂ¯ÂµÃ ÃÂ» Â°ÃÃÂ» Â¼ÃÃÂ¯ ÃÃÃÂ¦ÃÃÂ´Ã
					if( vehicle &&
						vehicle->GetObjectKind() == eObjectKind_Vehicle )
					{
						const ICONBASE& useItem = vehicle->GetUseItem();

						if( useItem.wIconIdx == iconIdx )
						{
							vehicle->DoDie( 0 );
						}
					}

					MSG_ITEM_DISCARD_SYN msg;
					ZeroMemory(&msg, sizeof(msg));
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_DISCARD_ACK;
					msg.TargetPos = position;
					msg.wItemIdx = iconIdx;
					msg.ItemNum = 1;
					SendMsg(&msg, sizeof(msg));	

					LogItemMoney(
						GetID(),
						GetObjectName(),
						0,
						"",
						eLog_ShopItemUseEnd,
						GetMoney(),
						0,
						0,
						iconIdx,
						pItemBase->dwDBIdx,
						pItemBase->Position,
						0,
						1,
						GetPlayerExpPoint() );
				}
			}
		}
	}

	// NYJ - Â½ÃÂ°Â£ÃÂ¦ Â¿Ã¤Â¸Â®Â·Â¹Â½ÃÃÃ Â½ÃÂ°Â£Â¼ÃÂ¸Ã°
	ProcessRecipeTimeCheck(dwElapsedMili);

	{
		CPet* const petObject = PETMGR->GetPet(
			GetPetItemDbIndex());

		if(petObject)
		{
			petObject->ProcessTimeCheckItem(
				bDBUpdate,
				dwElapsedMili);
		}
	}

	// 100525 NYJ - ÃÃÂ¸ÃÂ´Ã«ÃÃ  ÂµÃ®Â·ÃÂ»Ã³ÃÂ°Â¿Â¡ Â´Ã«ÃÃ Â½ÃÂ°Â£Â°Ã¦Â°ÃºÃÂ¼ÃÂ© Â¼Ã¶ÃÃ 
	Consignment_CheckDate(GetID());
	Note_CheckDate(GetID());
}

// desc_hseos_Â°Ã¡ÃÂ¥_01
// S Â°Ã¡ÃÂ¥ ÃÃÂ°Â¡ added by hseos 2008.01.29
BOOL CPlayer::RemoveItem(DWORD nItemID, DWORD nItemNum, eLogitemmoney eLogKind)
{
	int iCheckItemMaxNum = TP_WEAR_END;
	if( IsGotWarehouseItems() )
	{
		iCheckItemMaxNum = TP_STORAGE_END;
	}

	for( POSTYPE i = TP_INVENTORY_START ; i <= iCheckItemMaxNum ; ++i )
	{
		ITEMBASE* pItemBase = (ITEMBASE*)m_ItemContainer.GetItemInfoAbs( i );

		if( pItemBase->wIconIdx == nItemID )
		{
			BOOL dbidx = pItemBase->dwDBIdx;
			POSTYPE	position = pItemBase->Position;
			DWORD	iconIdx = pItemBase->wIconIdx;
			int nResult = ITEMMGR->DiscardItem( this, pItemBase->Position, pItemBase->wIconIdx, nItemNum );
			if (nResult != EI_TRUE)
			{
				return FALSE;
			}

			LogItemMoney(
				GetID(),
				GetObjectName(),
				0,
				"",
				eLogKind,
				GetMoney(),
				0,
				0,
				iconIdx,
				dbidx,
				position,
				0,
				nItemNum,
				0);

			MSG_ITEM_DISCARD_SYN msg;
			ZeroMemory(&msg, sizeof(msg));
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_DISCARD_ACK;
			msg.TargetPos = position;
			msg.wItemIdx = iconIdx;
			msg.ItemNum = nItemNum;
			SendMsg(&msg, sizeof(msg));	
		}
	}

	return TRUE;
}
// E Â°Ã¡ÃÂ¥ ÃÃÂ°Â¡ added by hseos 2008.01.29


BOOL CPlayer::IncreaseInventoryExpansion()
{
	if( GetInventoryExpansion() >= 2 )
		return FALSE;

	IncreaseCharacterInventory(this->GetID());

	return TRUE;
}

void CPlayer::PassiveSkillCheckForWeareItem()
{
	memset( &mPassiveStatus, 0, sizeof( Status ) );
	memset( &mRatePassiveStatus, 0, sizeof( Status ) );

	m_SkillTree->SetPositionHead();

	for(SKILL_BASE* pSkillBase = m_SkillTree->GetData();
		0 < pSkillBase;
		pSkillBase = m_SkillTree->GetData())
	{
		const DWORD skillLevel = my_min(
			pSkillBase->Level,
			SKILLMGR->GetSkillSize(pSkillBase->wSkillIdx));
		const cActiveSkillInfo* const pSkill = SKILLMGR->GetActiveInfo( 
			pSkillBase->wSkillIdx - 1 + skillLevel);
		
		if(0 == pSkill)
		{
			continue;
		}
		else if(SKILLKIND_PASSIVE != pSkill->GetKind())
		{
			continue;
		}

		for(DWORD index = 0; index < MAX_BUFF_COUNT; ++index)
		{
			const cBuffSkillInfo* const pSkillInfo = ( cBuffSkillInfo* )SKILLMGR->GetBuffInfo(
				pSkill->GetInfo().Buff[index]);

			if(0 == pSkillInfo)
			{
				break;
			}
			else if(FALSE == IsEnable(pSkillInfo->GetInfo()))
			{
				continue;
			}

			pSkillInfo->AddPassiveStatus(
				this);
		}
	}
}

BOOL CPlayer::ClearInventory()
{
	for( POSTYPE i = TP_INVENTORY_START ; i < TP_WEAR_START ; ++i )
	{
		ITEMBASE* pItemBase = (ITEMBASE*)m_ItemContainer.GetItemInfoAbs( i );
		if( pItemBase->dwDBIdx == 0 ) continue;

		int returnValue = ITEMMGR->DiscardItem( this, pItemBase->Position, pItemBase->wIconIdx, pItemBase->Durability );
		if( EI_TRUE == returnValue )
		{
			continue;
		}
	}
	return TRUE;
}

void CPlayer::FishingInfoClear()
{
	SetFishingPlace(0);
	SetFishingStartTime(0);
	SetFishingProcessTime(0);
	SetFishingBait(0);
	ZeroMemory(
		m_fFishBaitRate,
		sizeof(m_fFishBaitRate));
}

void CPlayer::SetFishingExp(EXPTYPE dwExp)
{
	const LEVELTYPE& level = m_wFishingLevel;

	if(MAX_FISHING_LEVEL <= level)
	{
		m_wFishingLevel = MAX_FISHING_LEVEL;
		m_dwFishingExp = 0;
		return;
	}
	
	// Â°Ã¦ÃÃ¨ÃÂ¡Â°Â¡ Â´ÃÃÂ½ Â´ÃÂ°Ã¨Â¿Â¡Â¼Â­ Â¿Ã¤Â±Â¸ÃÃÂ´Ã Â°ÃÂºÂ¸Â´Ã ÃÃÂ¾Ã Â¸Â¹ÃÂ» Â¼Ã¶ ÃÃÃÂ¸Â¹ÃÂ·Ã,
	// Â°Ã¨Â¼Ã ÃÂ¼ÃÂ©ÃÃÂ¼Â­ Â·Â¹ÂºÂ§Â¾Ã·ÃÃÃÃ
	{
		EXPTYPE nextPoint = 0 ;
		nextPoint = GAMERESRCMNGR->GetFishingMaxExpPoint( level ) ;

		if(nextPoint == 0)
		{
			return ;
		}

		while( dwExp >= nextPoint )
		{
			if(nextPoint==0)
				break;

			m_wFishingLevel++;

			if(GetLevel() < GAMERESRCMNGR->GetFishingLevelUpRestrict(level))
			{
				m_wFishingLevel--;
				m_dwFishingExp = dwExp = nextPoint;

				MSG_WORD msg;
				msg.Category = MP_FISHING;
				msg.Protocol = MP_FISHING_LEVELUP_NACK;
				msg.wData = m_wFishingLevel;
				SendMsg(&msg, sizeof(msg));	
				break;
			}

			Log_Fishing(
				GetID(),
				eFishingLog_SetLevel,
				0,
				GetFishPoint(),
				0,
				0,
				dwExp - nextPoint,
				GetFishingLevel() );

			// 100607 NYJ ÂºÂ¸ÃÂ¶Â±Ã¢Â¼Ãº Â·Â¹ÂºÂ§Â¾Ã·Â½Ã Â¾ÃÃÃÃÃÃÃ¶Â±Ã
			DWORD dwRewardItem = GAMERESRCMNGR->GetFishingLevelUpReward(level);
			if(dwRewardItem)
			{
				ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(dwRewardItem);
				if(pInfo)
				{
					// 2286, 2287, "2288" Â´Ã SystemMsg.binÃÃ ÃÃÂµÂ¦Â½Âº
					ItemInsertToNote(GetID(), dwRewardItem, 1, pInfo->wSeal, 0, eNoteParsing_FishingLevelUp, 2286, 2287, "2288");
				}
			}

			if( m_wFishingLevel == 2 )
			{
				WebEvent( GetUserID(), 8 );
			}

			MSG_WORD msg;
			msg.Category = MP_FISHING;
			msg.Protocol = MP_FISHING_LEVELUP_ACK;
			msg.wData = m_wFishingLevel;
			SendMsg(&msg, sizeof(msg));	

			dwExp		-=	nextPoint;
			nextPoint	=	GAMERESRCMNGR->GetFishingMaxExpPoint( level );
		}

		m_dwFishingExp = dwExp;
	}
}

// 080509 LUJ, Â½ÂºÃÂ³ ÃÃ°ÃÂ¸ÃÃÃÃ ÃÃ¶Â³ÂªÃÃ¶ Â¾ÃÂ¾ÃÃÂ¸Â¸Ã© ÃÃ¼ÃÂ» Â¹ÃÃÂ¯ÃÃÂ´Ã
// 080514 LUJ, Â½ÂºÃÂ³ Â¾ÃÂ´ÃÂ¸ÃÃÃÂ¼Ã Â½ÃÂ°Â£ ÃÂ¼ÃÂ©
// 080515 LUJ, ÃÃ°ÃÂ¸ÃÃÂ°Ãº Â¾ÃÂ´ÃÂ¸ÃÃÃÂ¼Ã Â½ÃÂ°Â£ ÃÂ¼ÃÂ©Â·Ã ÃÃÃÃ Â½ÂºÃÂ³ÃÃ Â¾Ã³Â¸Â¶Â³Âª Â½ÃÃÃÃÃÂ´ÃÃÃ¶ ÃÂ¡Â°ÃÃÃÂ±Ã¢ ÃÂ§ÃÃ Â·ÃÂ±ÃÂ¸Â¦ ÃÃÂ¼ÂºÃÃÂ´Ã
// 080516 LUJ, ÃÃ°ÃÂ¸ÃÃ ÃÂ¼ÃÂ© Â½ÃÃÃÂ°Â¡ ÃÃ£Â¿Ã« Â¹Ã¼ÃÂ§ ÃÃÂ»Ã³ÃÃ Â¶Â§ ÃÂ¢Â¼ÃÃÂ» ÃÂ¾Â·Ã¡Â½ÃÃÂ´
BOOL CPlayer::IsCoolTime( const ACTIVE_SKILL_INFO& skill )
{
	// 080516 LUJ, ÃÃ°ÃÂ¸ÃÃ Â½ÃÃÃÂ°Â¡ Â¹ÃÂ»Ã½ÃÃÂµÂµ ÃÃÃÂ¤ ÃÂ¸Â¼Ã¶ ÃÃÂ»Ã³ÃÂº ÃÃ£Â¿Ã«ÃÃÂ´Ã. Â±Ã ÃÃÂ»Ã³ÃÃ Â¹ÃÂ»Ã½ÃÃÂ¸Ã© ÃÂ¢Â¼ÃÃÂ» Â°Â­ÃÂ¦Â·Ã ÃÂ¾Â·Ã¡Â½ÃÃÂ²Â´Ã
	struct
	{
		void operator() ( CPlayer& player, CPlayer::CheckCoolTime& checkCoolTime )
		{
			const DWORD maxCheckTick = 1000 * 60;

			// 080516 LUJ, ÃÃ°ÃÂ¸ÃÃ Â½ÃÃÃÂ°Â¡ Â¹ÃÂ»Ã½ÃÃ ÃÃ¶ ÃÃÃÂ¤ÃÃ Â½ÃÂ°Â£ÃÃ ÃÃ¶Â³ÂµÃÂ¸Â¸Ã© ÃÂ¼ÃÂ© ÂµÂ¥ÃÃÃÃÂ¸Â¦ ÃÃÂ±Ã¢ÃÂ­ÃÃÂ´Ã
			// 080519 LUJ, maxCheckTick Â³Â»Â¿Â¡ Â¹ÃÂ»Ã½ÃÃ Â¿ÃÂ·Ã¹ ÃÂ¼ÃÂ©Â¸Â¦ ÃÃÃÃ¶ Â¸Ã¸ÃÃÂ´Ã¸ Â°Ã Â¼Ã¶ÃÂ¤
			if( maxCheckTick < ( checkCoolTime.mCheckedTick - gCurTime ) )
			{
				// 080519 LUJ, ÃÂ¼ÃÂ© Â½ÃÂ°Â£ÃÂ» ÃÃ¶Â±ÃÂºÃÃÃ maxCheckTickÂµÂ¿Â¾ÃÃÂ¸Â·Ã ÃÃÂ´Ã
				checkCoolTime.mCheckedTick	= gCurTime + maxCheckTick;
				checkCoolTime.mFailureCount	= 0;
			}

			const DWORD maxCheckCount = 10;

			// 080516 LUJ, ÃÃ°ÃÂ¸ÃÃ Â½ÃÃÃÂ°Â¡ ÃÃ£Â¿Ã« ÃÂ¸Â¼Ã¶ ÃÃÃÃÃÃÂ¸Ã©, ÃÂ³Â¸Â®ÃÃÃÃ¶ Â¾ÃÂ´ÃÂ´Ã
			if( maxCheckCount > ++checkCoolTime.mFailureCount )
			{
				return;
			}

			// 080516 LUJ, ÃÃÃÂ¤ ÃÂ¸Â¼Ã¶ÃÃÂ»Ã³ Â½ÃÃÃÃÃ Â°Ã¦Â¿Ã¬ ÃÂ¢Â¼ÃÃÂ» ÃÂ¾Â·Ã¡Â½ÃÃÂ²Â´Ã
			{
				MSG_DWORD message;
				ZeroMemory( &message, sizeof( message ) );
				message.Category	= MP_USERCONN;
				message.Protocol	= MP_USERCONN_GAMEIN_NACK;
				message.dwData		= player.GetID();
				
				g_Network.Broadcast2AgentServer( (char*)&message, sizeof( message ) );

				// 100812 NYJ - Â°Â­ÃÂ¦ÃÂ¾Â·Ã¡ ÃÃÂ¼ÃÂ·ÃÂ±ÃÂ¸Â¦ Â³Â²Â±Ã¢ÃÃ.
				g_Console.LOG(4, "Force KickOut!! (CoolTime Check Failed.) : ID: %d, NAME: %s",  player.GetID(), player.GetObjectName() );
			}
		}
	}
	Punish;

	// 080514 LUJ, Â¾ÃÂ´ÃÂ¸ÃÃÃÂ¼ÃÃÃ ÃÂ¥Â½ÃÂµÃÂ´Ã Â¼Ã¸Â°Â£Â¿Â¡Â´Ã Â½ÂºÃÂ³ÃÂ» Â»Ã§Â¿Ã«ÃÃ Â¼Ã¶ Â¾Ã¸Â´Ã
	const SkillAnimTimeMap::const_iterator itAnim = mSkillAnimTimeMap.find( skill.Index );
	if(mSkillAnimTimeMap.end() != itAnim &&
		itAnim->second > gCurTime)
	{
		// 080516 LUJ, ÃÃ°ÃÂ¸ÃÃ ÃÂ¼ÃÂ©Â°Â¡ ÃÃÃÂ¤ Â±Ã¢ÃÃ ÃÃÂ»Ã³ Â½ÃÃÃÃÃÃÂ» Â¶Â§ Â¹ÃºÃÂ¢ÃÂ» ÂºÃÂ¿Â©ÃÃÂ´Ã
		Punish( *this, mCheckCoolTime );
		// 080519 LUJ, ÃÃ°ÃÂ¸ÃÃ ÃÂ¼ÃÂ© Â½ÃÃÃ Â¶Â§ÂµÂµ Â»Ã§Â¿Ã« Â°Â¡Â´ÃÃÃÂ°Ã Â¹ÃÃÂ¯ÃÃÂ´Ã. ÃÃÃÂ¤ ÃÂ¸Â¼Ã¶ ÃÃÂ»Ã³ Â½ÃÃÃ Â½Ã Â¹ÃºÃÂ¢ÃÂ» ÂºÃÂ¿Â©ÃÃÂ±Ã¢ Â¶Â§Â¹Â®
		return FALSE;
	}

	const SkillCoolTimeMap::const_iterator it = mSkillCoolTimeMap.find( skill.Index );

	if(mSkillCoolTimeMap.end() == it)
	{
		return FALSE;
	}

	const DWORD endTime		= it->second;
	const BOOL	isCoolTime	=  endTime > gCurTime;

	if( isCoolTime )
	{
		// 080516 LUJ, ÃÃ°ÃÂ¸ÃÃ ÃÂ¼ÃÂ©Â°Â¡ ÃÃÃÂ¤ Â±Ã¢ÃÃ ÃÃÂ»Ã³ Â½ÃÃÃÃÃÃÂ» Â¶Â§ Â¹ÃºÃÂ¢ÃÂ» ÂºÃÂ¿Â©ÃÃÂ´Ã
		Punish( *this, mCheckCoolTime );
	}
	
	// 080519 LUJ, ÃÃ°ÃÂ¸ÃÃ ÃÂ¼ÃÂ© Â½ÃÃÃ Â¶Â§ÂµÂµ Â»Ã§Â¿Ã« Â°Â¡Â´ÃÃÃÂ°Ã Â¹ÃÃÂ¯ÃÃÂ´Ã. ÃÃÃÂ¤ ÃÂ¸Â¼Ã¶ ÃÃÂ»Ã³ Â½ÃÃÃ Â½Ã Â¹ÃºÃÂ¢ÃÂ» ÂºÃÂ¿Â©ÃÃÂ±Ã¢ Â¶Â§Â¹Â®
	return FALSE;
}

// 080511 LUJ, Â½ÂºÃÂ³ ÃÃ°ÃÂ¸ÃÃÃÂ» ÃÃÂ°Â¡ÃÃÂ´Ã
// 080514 LUJ, Â½ÂºÃÂ³ Â¾ÃÂ´ÃÂ¸ÃÃÃÂ¼Ã ÃÂ¾Â·Ã¡ Â½ÃÂ°Â£ Â¼Â³ÃÂ¤
// 080605 LUJ, Â½ÂºÃÂ³ ÃÂ¾Â·Ã¹Â¿Â¡ ÂµÃ»Â¶Ã³ Â¾ÃÂ´ÃÂ¸ÃÃÃÂ¼Ã Â½ÃÂ°Â£ÃÂ» ÂºÂ¯Â°Ã¦ÃÃÂ´Ã
void CPlayer::SetCoolTime( const ACTIVE_SKILL_INFO& skill )
{
	// 080605 LUJ, Â¾ÃÂ´ÃÂ¸ÃÃÃÂ¼Ã ÃÂ¸ÃÃÃÂ» Â½ÂºÃÂ³ ÃÂ¾Â·Ã¹Â¿Â¡ ÂµÃ»Â¶Ã³ Â°Â¡ÂºÂ¯Â½ÃÃÂ°Â±Ã¢ ÃÂ§ÃÃ ÂºÂ¯Â¼Ã¶ ÃÂ¤ÃÃ
	float animationTime = float( skill.AnimationTime );

	// 080605 LUJ, Â½ÂºÃÂ³ ÃÂ¾Â·Ã¹Â¿Â¡ ÂµÃ»Â¶Ã³ Â¾ÃÂ´ÃÂ¸ÃÃÃÂ¼Ã ÃÂ¸ÃÃÃÂ» ÃÃ»ÃÃ½ÃÃ· Â°Â¡Â°Â¨ÃÃÂ´Ã.
	{
		const Status* ratePassiveStatus = GetRatePassiveStatus();
		const Status* rateBuffStatus	= GetRateBuffStatus();

		if( rateBuffStatus &&
			ratePassiveStatus )
		{
			if( ( skill.Index / 100000 ) % 10 )
			{
				switch( skill.Unit )
				{
				case UNITKIND_MAGIC_ATTCK:
					{
						animationTime = animationTime * ( 1.0f - ( rateBuffStatus->MagicSkillSpeedRate + ratePassiveStatus->MagicSkillSpeedRate ) / 100.0f );
						break;
					}
				case UNITKIND_PHYSIC_ATTCK:
					{
						animationTime = animationTime * ( 1.0f - ( rateBuffStatus->PhysicSkillSpeedRate + ratePassiveStatus->PhysicSkillSpeedRate ) / 100.0f );
						break;
					}
				}
			}
			else
			{
				animationTime = animationTime * ( 1.0f - ( rateBuffStatus->NormalSpeedRate + ratePassiveStatus->NormalSpeedRate ) / 100.0f );
			}
		}
	}
	
	// 080514 LUJ, Â¾ÃÂ´ÃÂ¸ÃÃÃÂ¼ÃÃÃ Â³Â¡Â³ÂªÂ´Ã Â½ÃÂ°Â£ÃÂ» Â¼Â³ÃÂ¤ÃÃÂ´Ã. Â³ÃÃÂ®Â¿Ã¶ÃÂ© ÃÃ¶Â¿Â¬ÃÂ» Â°Â¨Â¾ÃÃÃÂ¿Â© 0.1ÃÃ Â¿ÃÃÃ·Â´Ã ÃÃ£Â¿Ã«ÃÃÂ´Ã
	// 080520 LUJ, ÃÃÂ½ÂºÃÂ® Â°Ã¡Â°ÃºÂ·Ã 0.1->0.3ÃÃÂ·Ã ÃÃ£Â¿Ã« Â½ÃÂ°Â£ Â¿Â¬ÃÃ¥
	// 080605 LUJ, ÃÃÂ¼Ã 0ÃÃ Â°ÂªÂ¸Â¸ ÃÃ£Â¿Ã«ÃÃÂ´Ã. animationTimeÃÃ Â½ÃÂ¼Ã¶Â·Ã ÂºÂ¯Â°Ã¦ÂµÃÂ¾Ã® Â¿ÃÂ¹Ã¶ÃÃÂ·Ã Â¿Â©ÃÃ¶Â°Â¡ ÃÃÂ±Ã¢ Â¶Â§Â¹Â®
	mSkillAnimTimeMap[ skill.Index ] = DWORD( my_max( 0, animationTime ) ) + gCurTime - 300;
	mSkillCoolTimeMap[ skill.Index ] = skill.CoolTime + gCurTime - 300;	
}

void CPlayer::ResetCoolTime( const ACTIVE_SKILL_INFO& skill )
{
	mSkillAnimTimeMap[skill.Index] = 0;
	mSkillCoolTimeMap[skill.Index] = 0;
}

BOOL CPlayer::IsCanCancelSkill()
{
	// 100618 ShinJS ÃÃÃÂ¤Â½ÃÂ°Â£Â³Â»Â¿Â¡ ÃÂ¯ÃÂ¤ÃÂ¸Â¼Ã¶(ÃÃ¶ÃÃ§3ÃÂ¸)ÃÃÂ»Ã³ Â¿Ã¤ÃÂ»Â½Ã ÃÃ«Â¼ÃÂ¿Ã¤ÃÂ»Â¿Â¡ ÃÂ¦ÃÃÃÂ» ÃÃÂ´Ã.
	if( m_dwSkillCancelLastTime > gCurTime )
	{
		if( ++m_dwSkillCancelCount >= eSkillCancelLimit_Count )
		{
			m_dwSkillCancelLastTime = gCurTime + eSkillCancelLimit_CheckTime;
			return FALSE;
		}

		return TRUE;
	}
	
	m_dwSkillCancelCount = 0;
	m_dwSkillCancelLastTime = gCurTime + eSkillCancelLimit_CheckTime;

	return TRUE;
}

const DWORD CPlayer::GetSkillCancelDelay() const
{
	return eSkillCancelLimit_CheckTime;
}

// 100621 ShinJS ÃÃ¶ÃÃ§ ÃÂ³Â½ÂºÃÃÃÃÃÃ Â½ÂºÃÂ³ ÃÃ«Â¼Ã
void CPlayer::CancelCurrentCastingSkill( BOOL bUseSkillCancelRate )
{
	cActiveSkillObject* const activeSkillObject = ( cActiveSkillObject* )SKILLMGR->GetSkillObject( mCurrentSkillID );

	// 090109 LUJ, Â¾ÃÃÂ¼ÂºÃª Â½ÂºÃÂ³Â¸Â¸ ÃÃ«Â¼ÃÂµÃ Â¼Ã¶ ÃÃÂ´Ã
	// 090109 LUJ, ÃÂ³Â½ÂºÃÃ ÃÃÂ¿Â¡Â¸Â¸ ÃÃ«Â¼ÃÂµÃ Â¼Ã¶ ÃÃÂµÂµÂ·Ã ÃÂ¼ÃÂ©ÃÃÂ´Ã
	if( ! activeSkillObject || 
		cSkillObject::TypeActive != activeSkillObject->GetType() ||
		! activeSkillObject->IsCasting() )
	{
		return;
	}

	if( bUseSkillCancelRate && 
		activeSkillObject->GetInfo().Cancel <= (rand() % 100) ) 
	{
		return;
	}

	MSG_DWORD message;
	ZeroMemory( &message, sizeof( message ) );
	message.Category	= MP_SKILL;
	message.Protocol	= MP_SKILL_CANCEL_NOTIFY;
	message.dwObjectID	= GetID();
	message.dwData		= mCurrentSkillID;
	PACKEDDATA_OBJ->QuickSend( this, &message, sizeof( MSG_DWORD ) );

	cSkillObject* const skillObject = SKILLMGR->GetSkillObject( mCurrentSkillID );
	if( skillObject )
	{
		skillObject->SetEndState();
		skillObject->EndState();
	}
}

eArmorType CPlayer::GetArmorType(EWEARED_ITEM wearType) const
{
	const ITEM_INFO* const itemInfo = ITEMMGR->GetItemInfo( GetWearedItemIdx( wearType ) );

	return itemInfo ? eArmorType( itemInfo->ArmorType ) : eArmorType_None;
}

void CPlayer::AddSpecialSkill(const cBuffSkillInfo* buffSkillInfo)
{
	// 090204 LUJ, ÃÂ³ÃÂ½Â¿Â¡Â´Ã ÃÃÂ»Ã³ ÃÃ»Â¿Ã«ÃÃÂ´Ã. ÃÃÂ·ÃÂ¼Â¼Â½Âº Â¶Â§ Â°ÃÂ»Ã§ÃÃÂ¸Ã©Â¼Â­ ÃÂ¶Â°ÃÂ¿Â¡ Â¸ÃÃÃ¶ Â¾ÃÃÂ» Â°Ã¦Â¿Ã¬ ÃÃÃÂ¦ÃÃÂ´Ã
	buffSkillInfo->AddBuffStatus( this );

	SpecialSkillData specialSkillData = { 0 };
	specialSkillData.mBuffSkillInfo	= buffSkillInfo;
	specialSkillData.mIsTurnOn = TRUE;
	mSpecialSkillList.push_back( specialSkillData );
}

void CPlayer::RemoveSpecialSkill(const cBuffSkillInfo* buffSkillInfo)
{
	SpecialSkillList::iterator it = mSpecialSkillList.end();

	for(; mSpecialSkillList.end() != it; ++it )
	{
		const SpecialSkillData& specialSkillData = *it;

		if( specialSkillData.mBuffSkillInfo == buffSkillInfo )
		{
			break;
		}
	}

	if(mSpecialSkillList.end() == it)
	{
		return;
	}

	const SpecialSkillData& specialSkillData = *it;

	// 090204 LUJ, ÃÂ¶Â°ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ Â¹ÃÃÃ»Â¿Ã« Â»Ã³ÃÃÃÃ Â¼Ã¶ ÃÃÃÂ¸Â¹ÃÂ·Ã Â°ÃÂ»Ã§ ÃÃ ÃÃ«Â¼ÃÃÃÂ¾Ã ÃÃÂ´Ã
	if( specialSkillData.mIsTurnOn )
	{
		buffSkillInfo->RemoveBuffStatus( this );
	}

	mSpecialSkillList.erase(it);
}

// 090204 LUJ, ÃÂ¯Â¼Ã¶ Â½ÂºÃÂ³ÃÂ» ÃÃÂ·ÃÂ¼Â¼Â½Âº ÃÂ¸ÃÃÂ¿Â¡ ÃÂ¼ÃÂ©ÃÃÂ´Ã
void CPlayer::ProcSpecialSkill()
{
	if(true == mSpecialSkillList.empty())
	{
		return;
	}

	// 090204 LUJ, ÃÂ¿ÃÂ²Â¼ÂºÃÂ» ÃÂ§ÃÃ ÃÃÃÃÃÃÂ³Ã Â¸Ã Â¾ÃÃÃ Â½ÂºÃÂ³ÃÂ» Â°ÃÂ»Ã§ÃÃ ÃÃ Â¸Ã ÂµÃÂ·Ã ÂµÂ¹Â¸Â°Â´Ã
	SpecialSkillData specialSkillData = mSpecialSkillList.front();
	mSpecialSkillList.pop_front();

	const cBuffSkillInfo* const buffSkillInfo = specialSkillData.mBuffSkillInfo;
	const BOOL validCondition = IsEnable(buffSkillInfo->GetInfo());
	const BOOL isTurnOn = ( ! specialSkillData.mIsTurnOn && validCondition );
	const BOOL isTurnOff = ( specialSkillData.mIsTurnOn && ! validCondition );

	if( isTurnOn )
	{
		buffSkillInfo->AddBuffStatus( this );
		specialSkillData.mIsTurnOn = TRUE;
	}
	else if( isTurnOff )
	{
		buffSkillInfo->RemoveBuffStatus( this );
		specialSkillData.mIsTurnOn = FALSE;
	}

	// 090204 LUJ, Â´ÃÃÂ½ Â°ÃÂ»Ã§Â¸Â¦ ÃÂ§ÃÃ ÃÃÂ°Â¡ÃÃÂ´Ã
	mSpecialSkillList.push_back(
		specialSkillData);
}

void CPlayer::SetMasterRecipe(POSTYPE pos, DWORD dwRecipe, DWORD dwRemainTime)
{
	if(pos<0 || MAX_RECIPE_LV4_LIST<=pos)	return;

	m_MasterRecipe[pos].dwRecipeIdx = dwRecipe;
	m_MasterRecipe[pos].dwRemainTime = dwRemainTime;
}

stRecipeLv4Info* CPlayer::GetMasterRecipe(POSTYPE pos)
{
	if(pos<0 || MAX_RECIPE_LV4_LIST<=pos)	return NULL;

	return &m_MasterRecipe[pos];
}

int CPlayer::CanAddRecipe(DWORD dwRecipe)
{
	int i, nEmptySlot = -1;
	for(i=0; i<MAX_RECIPE_LV4_LIST; i++)
	{
		if(nEmptySlot<0 && 0==m_MasterRecipe[i].dwRecipeIdx)
			nEmptySlot = i;

		if(m_MasterRecipe[i].dwRecipeIdx == dwRecipe)
			return -2;
	}

	return nEmptySlot;
}

void CPlayer::ProcessRecipeTimeCheck(DWORD dwElapsedTime)
{
	DWORD dwElapsedMili = dwElapsedTime;

	for(POSTYPE i =0; i<MAX_RECIPE_LV4_LIST; i++)
	{
		if(0!=m_MasterRecipe[i].dwRecipeIdx && 0!=m_MasterRecipe[i].dwRemainTime)
		{
			if(dwElapsedMili > m_MasterRecipe[i].dwRemainTime)
			{
				// Â»Ã¨ÃÂ¦
				DWORD dwRecipeIdx = m_MasterRecipe[i].dwRecipeIdx;
				SetMasterRecipe(i, 0, 0);
				Cooking_Recipe_Update(GetID(), eCOOKRECIPE_DEL, dwRecipeIdx, i, 0);
				CookRecipe_Log(GetID(), dwRecipeIdx, i, eCOOKRECIPE_DEL, 0);

				MSG_DWORD4 msg;
				msg.Category	= MP_COOK;
				msg.Protocol	= MP_COOK_UPDATERECIPE;
				msg.dwObjectID	= GetID();
				msg.dwData1		= eCOOKRECIPE_DEL;
				msg.dwData2		= dwRecipeIdx;
				msg.dwData3		= i;
				msg.dwData4		= 0;
				SendMsg(&msg, sizeof(msg));
			}
			else
			{
				// Â°Â»Â½Ã
				DWORD dwRemainTime = m_MasterRecipe[i].dwRemainTime - dwElapsedMili;
				SetMasterRecipe(i, m_MasterRecipe[i].dwRecipeIdx, dwRemainTime);
				Cooking_Recipe_Update(GetID(), eCOOKRECIPE_UPDATE, m_MasterRecipe[i].dwRecipeIdx, i, dwRemainTime);

				MSG_DWORD4 msg;
				msg.Category	= MP_COOK;
				msg.Protocol	= MP_COOK_UPDATERECIPE;
				msg.dwObjectID	= GetID();
				msg.dwData1		= eCOOKRECIPE_UPDATE;
				msg.dwData2		= m_MasterRecipe[i].dwRecipeIdx;
				msg.dwData3		= i;
				msg.dwData4		= dwRemainTime;
				SendMsg(&msg, sizeof(msg));
			}
		}
	}
}

void CPlayer::ProceedToTrigger()
{
	if(gCurTime < mNextCheckedTick)
	{
		return;
	}

	// Â´Ã¸ÃÃ¼Â¿ÃÃÃºÂ¹Ã¶Â´Ã Â¸ÃÂ½ÃÃÃ¶Â¸Â¦ Â¹ÃÂ»Ã½Â½ÃÃÂ°ÃÃ¶ Â¾ÃÂ´ÃÂ´Ã.
	if(m_bDungeonObserver)
		return;

	// 091116 LUJ, ÃÃÂ±Ã¢ÃÃ»ÃÂ¸Â·Ã Â¹ÃÂ¼ÃÃÃÂ´Ã Â¸ÃÂ½ÃÃÃ¶ Â°Â£Â°ÃÃÂ» Â´ÃÂ¸Â²(0.5 -> 1.0ÃÃ)
	const DWORD stepTick = 1000;
	mNextCheckedTick = gCurTime + stepTick;
	// 091116 LUJ, ÃÂ¤Â³ÃÂ¿Â¡ ÃÃÂ´Ã§ÃÃÂ´Ã Â¸ÃÂ½ÃÃÃ¶Â¸Â¦ ÃÃÂ´Ã§Â¹ÃÂµÂµÂ·Ã ÃÃÂ´Ã
	Trigger::CMessage* const message = TRIGGERMGR->AllocateMessage(GetGridID());
	message->AddValue(Trigger::eProperty_ObjectIndex, GetID());
	message->AddValue(Trigger::eProperty_ObjectKind, GetObjectKind());
	message->AddValue(Trigger::eProperty_Event, Trigger::eEvent_CheckSelf);
}

float CPlayer::GetBonusRange() const
{	
	const float value = mPassiveStatus.Range + mBuffStatus.Range;
	const float percent = mRatePassiveStatus.Range + mRateBuffStatus.Range;

	return value * (1.0f + percent);
}

BOOL CPlayer::IsNoEquip(eArmorType armorType, eWeaponType weaponType, eWeaponAnimationType weaponAnimationType)
{
	switch(armorType)
	{
	case eArmorType_Robe:
	case eArmorType_Leather:
	case eArmorType_Metal:
		{
			if(GetArmorType(eWearedItem_Dress) != armorType)
			{
				return TRUE;
			}

			break;
		}
	case eArmorType_Shield:
		{
			if(GetArmorType(eWearedItem_Shield) != armorType)
			{
				return TRUE;
			}

			break;
		}
	}

	if(eWeaponType_None != weaponType)
	{
		if(GetWeaponEquipType() != weaponType)
		{
			return TRUE;
		}
	}

	switch( weaponAnimationType )
	{
	case eWeaponAnimationType_None:
		break;
	case eWeaponAnimationType_OneHand:
		{
			switch( GetWeaponAniType() )
			{
			case eWeaponAnimationType_OneHand:
			case eWeaponAnimationType_Dagger:
			case eWeaponAnimationType_TwoBlade:
				break;
			default:
				return TRUE;
			}
		}
		break;

	case eWeaponAnimationType_TwoHand:
		{
			switch( GetWeaponAniType() )
			{
			case eWeaponAnimationType_TwoHand:
			case eWeaponAnimationType_Staff:
			case eWeaponAnimationType_Bow:
			case eWeaponAnimationType_Gun:
			case eWeaponAnimationType_Spear:
				break;
			default:
				return TRUE;
			}
		}
		break;

	default:
		{
			if( GetWeaponAniType() != weaponAnimationType )
				return TRUE;
		}
		break;
	}

	return FALSE;
}

BOOL CPlayer::IsEnable(const BUFF_SKILL_INFO& info)
{
	if(IsNoEquip(
		info.mArmorType,
		info.mWeaponType,
		info.mWeaponAnimationType))
	{
		return FALSE;
	}

	float checkValue = 0;
	const BUFF_SKILL_INFO::Condition& condition = info.mCondition;

	switch(condition.mType)
	{
	case BUFF_SKILL_INFO::Condition::TypeLifePercent:
		{
			checkValue = float(GetLifePercent());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeLife:
		{
			checkValue = float(GetLife());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeLifeMax:
		{
			checkValue = float(GetMaxLife());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeLifeRecovery:
		{
			checkValue = float(GetLifeRecover());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeManaPercent:
		{
			checkValue = float(GetManaPercent());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeMana:
		{
			checkValue = float(GetMana());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeManaMax:
		{
			checkValue = float(GetMaxMana());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeManaRecovery:
		{
			checkValue = float(GetManaRecover());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeStrength:
		{
			checkValue = float(GetStrength());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeIntelligence:
		{
			checkValue = float(GetIntelligence());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeDexterity:
		{
			checkValue = float(GetDexterity());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeWisdom:
		{
			checkValue = float(GetWisdom());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeVitality:
		{
			checkValue = float(GetVitality());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypePhysicalAttack:
		{
			checkValue = float(GetPhysicAttackMax());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeMagicalAttack:
		{
			checkValue = float(GetMagicAttackMax());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypePhysicalDefense:
		{
			checkValue = float(GetPhysicDefense());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeMagicalDefense:
		{
			checkValue = float(GetMagicDefense());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeCriticalRate:
		{
			checkValue = float(GetCriticalRate());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeCriticalDamage:
		{
			checkValue = float(GetCriticalDamageRate());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeAccuracy:
		{
			checkValue = float(GetAccuracy());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeEvade:
		{
			checkValue = float(GetAvoid());
			break;
		}
	case BUFF_SKILL_INFO::Condition::TypeMoveSpeed:
		{
			checkValue = float(GetMoveSpeed());
			break;
		}
	}

	BOOL isEnable = TRUE;

	switch(condition.mOperation)
	{
	case BUFF_SKILL_INFO::Condition::OperationMore:
		{
			isEnable = (checkValue > condition.mValue);
			break;
		}
	case BUFF_SKILL_INFO::Condition::OperationMoreEqual:
		{
			isEnable = (checkValue >= condition.mValue);
			break;
		}
	case BUFF_SKILL_INFO::Condition::OperationLess:
		{
			isEnable = (checkValue < condition.mValue);
			break;
		}
	case BUFF_SKILL_INFO::Condition::OperationLessEqual:
		{
			isEnable = (checkValue <= condition.mValue);
			break;
		}
	case BUFF_SKILL_INFO::Condition::OperationEqual:
		{
			isEnable = (checkValue == condition.mValue);
			break;
		}
	}

	return isEnable;
}

cSkillTree& CPlayer::GetSkillTree()
{
	return *m_SkillTree;
}

void CPlayer::SetPartyIdx( DWORD PartyIDx )
{
	m_HeroInfo.PartyID = PartyIDx; 

	if(m_HeroInfo.PartyID)
	{
		return;
	}

	// ÃÃÃÂ¼ Â»Ã³ÃÃÃÃ Â°Ã¦Â¿Ã¬ ÃÂ¯ÃÂ¤ Â½ÂºÃÂ³ÃÂ» Â»Ã¨ÃÂ¦ÃÃÂ´Ã
	{
		cPtrList templist;
		m_BuffSkillList.SetPositionHead();

		while(cBuffSkillObject* pSObj = (cBuffSkillObject*)m_BuffSkillList.GetData())
		{
			if(GetPartyIdx() != 0 && pSObj->GetInfo().Party == 1 )
			{
				pSObj->SetEndState();
				pSObj->EndState();
				templist.AddTail( pSObj );
			}
		}

		PTRLISTPOS pos = templist.GetHeadPosition();
		while( pos )
		{
			cSkillObject* const pSObj = ( cSkillObject* )templist.GetNext( pos );
			m_BuffSkillList.Remove( pSObj->GetSkillIdx() );
		}
		templist.RemoveAll();
	}
}

CObject* CPlayer::GetTObject() const
{
	if(mAggroObjectContainer.empty())
	{
		return 0;
	}

	ObjectIndexContainer::const_iterator iterator = mAggroObjectContainer.begin();

	std::advance(
		iterator,
		rand() % mAggroObjectContainer.size());

	return g_pUserTable->FindUser(
		*iterator);
}

void CPlayer::AddToAggroed(DWORD objectIndex)
{
	// Â¹Ã¶ÃÃÂ·Ã ÂµÂ¥Â¹ÃÃÃ¶Â¸Â¦ ÃÃÃÃº Â°Ã¦Â¿Ã¬ Â°Ã¸Â°ÃÃÃÂ°Â¡ ÃÃÂ±Ã¢ ÃÃÂ½ÃÃÃ ÂµÃÂ´Ã. ÃÃÂ½ÃÃÂ» Â¾Ã®Â±ÃÂ·Ã ÃÃÃÃÃÃÂ³ÃÂ¿Â¡ ÃÃºÃÃ¥ÃÃ ÃÃÂ¿Ã¤Â´Ã Â¾Ã¸Â´Ã
	if(GetID() == objectIndex)
	{
		return;
	}

	CObject* const pObject = g_pUserTable->FindUser( objectIndex );
	if( !pObject )
		return;

	if( pObject->GetObjectKind() & eObjectKind_Monster )
	{
		// 100616 ShinJS --- Â»Ã³Â´Ã«Â¿Â¡Â°Ã ÃÃÂ½ÃÃÂ» ÂµÃ®Â·ÃÃÃÂµÂµÂ·Ã ÃÃÂ¿Â© Die/ReleaseÂ½Ã Â¾Ã®Â±ÃÂ·ÃÂ¸Â¦ ÃÂ¦Â°ÃÃÃÂ¼Ã¶ ÃÃÂµÂµÂ·Ã ÃÃÂ´Ã.
		((CMonster*)pObject)->AddToAggroed( GetID() );
	}

	mAggroObjectContainer.insert(objectIndex);
}

void CPlayer::LogOnRelease()
{
	if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	{
		TCHAR text[MAX_PATH] = {0};
		_sntprintf(
			text,
			_countof(text),
			"map:%d",
			g_pServerSystem->GetMapNum());
		LogItemMoney(
			GetID(),
			GetObjectName(),
			0,
			text,
			eLog_DungeonEnd,
			GetMoney(),
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0);
		return;
	}
	else if(g_csDateManager.IsChallengeZone(g_pServerSystem->GetMapNum()))
	{
		TCHAR text[MAX_PATH] = {0};
		_sntprintf(
			text,
			_countof(text),
			"map:%d",
			g_pServerSystem->GetMapNum());
		LogItemMoney(
			GetID(),
			GetObjectName(),
			0,
			text,
			eLog_DateMatchEnd,
			GetMoney(),
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0);
		return;
	}

	const LimitDungeonScript& script = g_CGameResourceManager.GetLimitDungeonScript(
		g_pServerSystem->GetMapNum(),
		GetChannelID());

	if(script.mMapType == g_pServerSystem->GetMapNum())
	{
		TCHAR text[MAX_PATH] = {0};
		_sntprintf(
			text,
			_countof(text),
			"%map:%d(%d)",
			g_pServerSystem->GetMapNum(),
			GetChannelID());
		LogItemMoney(
			GetID(),
			GetObjectName(),
			0,
			text,
			eLog_LimitDungeonEnd,
			GetMoney(),
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0);
	}
}

// 100624 ONS HPÂ¾Ã·ÂµÂ¥ÃÃÃÂ®Â°Ã¼Â·Ã ÃÂ³Â¸Â® ÃÃÂ°Â¡
void CPlayer::AddLifeRecoverTime( const YYRECOVER_TIME& recoverTime )
{
	m_YYLifeRecoverTimeQueue.push( recoverTime );
}

void CPlayer::UpdateLife()
{
	if( 0 != m_LifeRecoverDirectlyAmount )
	{
		AddLife( m_LifeRecoverDirectlyAmount, NULL );
		m_LifeRecoverDirectlyAmount = 0;
		return;
	}

	if( 0 == m_YYLifeRecoverTime.count )
	{
		if( FALSE == m_YYLifeRecoverTimeQueue.empty() )
		{
			// ÃÃÂ³ÂªÂ¾Â¿ ÃÂ¥Â¿Â¡Â¼Â­ Â²Â¨Â³Â»Â¼Â­ Â¾Ã·ÂµÂ¥ÃÃÃÂ®Â½ÃÃÂ°ÂµÂµÂ·Ã ÃÃÂ´Ã.
			m_YYLifeRecoverTime = m_YYLifeRecoverTimeQueue.front();
			m_YYLifeRecoverTimeQueue.pop();
		}
	}

	if( m_YYLifeRecoverTime.count > 0 )
	{
		if( m_YYLifeRecoverTime.lastCheckTime < gCurTime )
		{
			// HPÂ°Â¡ Â²ÃÃÃ·Â¸Ã© ÃÂ¥Â¿Â¡ ÃÃºÃÃ¥ÂµÃ ÂµÂ¥ÃÃÃÃÂ¸Â¦ Â¸Ã°ÂµÃ Â»Ã¨ÃÂ¦ÃÃÂ´Ã.
			if( GetMaxLife() <= GetLife() )
			{
				while( !m_YYLifeRecoverTimeQueue.empty() )
				{
					m_YYLifeRecoverTimeQueue.pop();
				}
				m_YYLifeRecoverTime.count = 0;
				return;
			}

			// HPÂ¾Ã·ÂµÂ¥ÃÃÃÂ®ÃÃÂ´Ã.
			m_YYLifeRecoverTime.lastCheckTime = gCurTime + m_YYLifeRecoverTime.recoverDelayTime;
			AddLife( m_YYLifeRecoverTime.recoverUnitAmout, NULL );
			--m_YYLifeRecoverTime.count;
		}
	}
}

// 100624 ONS MPÂ¾Ã·ÂµÂ¥ÃÃÃÂ®Â°Ã¼Â·Ã ÃÂ³Â¸Â® ÃÃÂ°Â¡
void CPlayer::AddManaRecoverTime( const YYRECOVER_TIME& recoverTime )
{
	m_YYManaRecoverTimeQueue.push( recoverTime );
}

void CPlayer::UpdateMana()
{
	if( 0 != m_ManaRecoverDirectlyAmount )
	{
		AddMana( m_ManaRecoverDirectlyAmount, NULL );
		m_ManaRecoverDirectlyAmount = 0;
		return;
	}

	if( 0 == m_YYManaRecoverTime.count )
	{
		if( FALSE == m_YYManaRecoverTimeQueue.empty() )
		{
			m_YYManaRecoverTime = m_YYManaRecoverTimeQueue.front();
			m_YYManaRecoverTimeQueue.pop();
		}
	}

	if( m_YYManaRecoverTime.count > 0 )
	{
		if( m_YYManaRecoverTime.lastCheckTime < gCurTime )
		{
			if( GetMaxMana() <= GetMana() )
			{
				while( !m_YYManaRecoverTimeQueue.empty() )
				{
					m_YYManaRecoverTimeQueue.pop();
				}
				m_YYManaRecoverTime.count = 0;
				return;
			}

			m_YYManaRecoverTime.lastCheckTime = gCurTime + m_YYManaRecoverTime.recoverDelayTime;
			AddMana( m_YYManaRecoverTime.recoverUnitAmout, NULL );
			--m_YYManaRecoverTime.count;
		}
	}
}

// 100611 ONS ÃÂ¤ÃÃÂ±ÃÃÃ¶Â»Ã³ÃÃ Â¿Â©ÂºÃ ÃÃÂ´Ã.
BOOL CPlayer::IsForbidChat() const
{

	__time64_t time = 0;
	_time64( &time );
	if( time > ForbidChatTime || 0 == ForbidChatTime )
		return FALSE;

	return TRUE;
}

void	CPlayer::SetGuildName( char* GuildName ) { SafeStrCpy(m_HeroCharacterInfo.GuildName, GuildName, _countof(m_HeroCharacterInfo.GuildName)); }


// --- skr 12/01/2020
void CPlayer::UpdateRelife()
{
  if( RelifeStartTime ){
    DWORD spend = gCurTime - RelifeStartTime;
    if( RelifeTimer  <= spend ){
      RelifeStartTime = 0;
      RelifeON = FALSE;
    }
  }
}
void CPlayer::SetRelifeTimer(DWORD anum)
{
  RelifeTimer = anum;
}
void CPlayer::SetRelifeStart()
{
  if( RELIFEEMGR->isRelifeMod() && !RelifeON ){
    RelifeStartTime = gCurTime;
    RelifeON = TRUE;
    if( RELIFEEMGR->getBuffIdx() != 0){
    SKILLMGR->BuffSkillStart(
      GetID(),
      RELIFEEMGR->getBuffIdx(),
      RELIFEEMGR->getBuffRemainTime(),
      RELIFEEMGR->getBuffCount()
      );
    }
  }
}
BOOL CPlayer::CheckReLifeBuff(cBuffSkillInfo* abuff)
{
  BOOL ret = FALSE;
  DWORD biffindex = abuff->GetIndex();
  if( biffindex == 0 ){}
  else{
    ret = RELIFEEMGR->isAllowBuff( biffindex );
  }
  return ret;
}
BOOL CPlayer::CheckReLifeSkill(DWORD abuff)
{
  BOOL ret = FALSE;
  if( abuff == 0 ){}
  else{
    ret = RELIFEEMGR->isAllowSkill( abuff );
  }
  return ret;
}
