#include "StdAfx.h"

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
//#include "Object.h"
//#include "ServerSystem.h"
#endif

#include "Object.h"
#include "..\[CC]Header\GameResourceManager.h"

#include "ServerSystem.h"

#include "Network.h"
#include "MapNetworkMsgParser.h"
#include "Usertable.h"
#include "Servertable.h"
#include "ObjectFactory.h"
#include "UserTable.h"
#include "GridSystem.h"
#include "[CC]ServerModule/DataBase.h"
#include "MapDBMsgParser.h"
#include "CharMove.h"
#include "Player.h"
#include "Monster.h"
#include "BossMonster.h"
#include "BossMonsterManager.h"
#include "BossRewardsManager.h"
#include "Npc.h"
#include "MHTimeManager.h"
#include "ItemManager.h"
#include "TileManager.h"
#include "MHFile.h"
#include "CharacterCalcManager.h"
#include "PartyManager.h"
#include "ObjectStateManager.h"
#include "ItemDrop.h"
//#include "MapDBMsgParser.h"
#include "AISystem.h"
#include "BattleSystem_Server.h"
#include "ChannelSystem.h"
#include "StreetSTallManager.h "
#include "StorageManager.h"
#include "BootManager.h"
#include "PathManager.h"
#include "RegenManager.h"
#include "ShowdownManager.h"
#include "PKManager.h"
#include "LootingManager.h"
#include "AIGroupManager.h"
#include "GuildFieldWarMgr.h"
#include "GuildTournamentMgr.h"
#include "QuestManager.h"
#include "..\[CC]Quest\QuestEvent.h"
#include "QuestRegenMgr.h"
#include "QuestMapMgr.h"
#include "GuildManager.h"
#include "cMonsterSpeechManager.h"
#include "MapObject.h"
//#include "..\[CC]ServerModule\MiniDumper.h"
#include "WeatherManager.h"
#include "FieldBossMonsterManager.h"
#include "FieldBossMonster.h"
#include "FieldSubMonster.h"
#include "[cc]skill/server/manager/skillmanager.h"
#include "[cc]skill/server/object/skillobject.h"
#include "hseos/SHMain.h"
#include "hseos/Farm/SHFarmManager.h"
#include "hseos/Monstermeter/SHMonstermeterManager.h"
#include "hseos/Debug/SHDebug.h"
#include "hseos/Date/SHDateManager.h"
#include "AutoNoteManager.h"
#include "FishingManager.h"
#include "cCookManager.h"
#include "Pet.h"
#include "PetManager.h"
#include "SiegeWarfareMgr.h"
#include "..\[CC]SiegeDungeon\SiegeDungeonMgr.h"
#include "SiegeRecallMgr.h"
#include "NpcRecallMgr.h"
#include "LimitDungeonMgr.h"
#include "HousingMgr.h"
#include "Dungeon/DungeonMgr.h"
#include "Trigger/Manager.h"
#include "NPCMoveMgr.h"
#include "PCRoomManager.h"
#include "Party.h"
#include "FiniteStateMachine/Machine.h"
#include "..\[CC]ServerModule\DataBase.h"
#include "..\[CC]ServerModule\Console.h"

// --- skr 12/01/2020
#include "Relife.h"

LPCTSTR g_SERVER_VERSION = "LTSV08070301";

typedef void (*MSGPARSER)(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
MSGPARSER g_pServerMsgParser[MP_MAX];
MSGPARSER g_pUserMsgParser[MP_MAX];

BOOL g_bCloseWindow = FALSE;
HWND g_hWnd = 0;

int	g_nHackCheckNum = 15;
int	g_nHackCheckWriteNum = 15;

// 071218 LUJ, 3Â¹Ã¸ Â¹Ã¶ÃÂ° Â¼Â±ÃÃ Â½Ã ÃÃÂ·ÃÂ¼Â¼Â½Âº ÃÂ¸ÃÃÃÂ» ÃÂ¥Â½ÃÃÃÂ±Ã¢ ÃÂ§ÃÃ Â¿Ã«ÂµÂµ
struct ProcessTime
{
	DWORD	mCurrentTick;
	DWORD	mPreviousTick;
	DWORD	mMaxSpace;
	float	mAverageSpace;
	DWORD	mSpace;
	DWORD	mTotalSpace;
	DWORD	mCount;
}
processTime;


// taiyo 
CServerSystem * g_pServerSystem = NULL;

CServerSystem::CServerSystem()
{
	srand(GetTickCount());
	//CoInitialize(NULL);
	
	g_pUserTable = new CUserTable;
	g_pUserTable->Init(2000);
	g_pServerTable = new CServerTable;
	g_pServerTable->Init(50);
	g_pObjectFactory = new CObjectFactory;
	g_pObjectFactory->Init();
	m_pGridSystem = new CGridSystem;
	m_pGridSystem->Init();

	m_Nation = eNATION_KOREA;
	m_dwQuestTime = 0;
	m_bQuestTime = FALSE;

	m_bCompletionChrUpdate = FALSE;

//-- for test
	m_dwMainProcessTime = 0;

	// 070809 Â¿ÃµÃÃ, Â±Ã¦ÂµÃ¥ ÃÂ¤ÂºÂ¸Â¸Â¦ Â¸Ã°ÂµÃ ÃÃÂ±Ã¢ ÃÃ¼Â±Ã®ÃÃ¶ ÃÃ Â°ÂªÃÃ FALSEÂ·Ã ÂµÃÂ¾Ã®ÃÃÂµÂµÂ·Ã ÂµÃÂ¾Ã® ÃÃÃÂ¸Â³Âª.. 
	//				Â¾ÃÃÂ¸Â·Ã Â±Ã¦ÂµÃ¥ ÃÂ¤ÂºÂ¸Â´Ã ÃÃÂ¿Ã¤ÃÃ Â½ÃÃÂ¡Â¿Â¡Â¸Â¸ Â·ÃÂµÃ¥ÃÃ Â°ÃÃÃÂ´Ã. ÂµÃ»Â¶Ã³Â¼Â­ ÃÃ Â°ÂªÃÂº ÃÃÂ±Ã¢ÃÂ­ ÃÃ ÃÃ¼ÃÃ ÂµÃÂ´Ã.
	//				Â±Ã¦ÂµÃ¥ ÃÂ¤ÂºÂ¸ÃÃ ÃÃ¶Â¿Â¬ Â·ÃÂµÃ¹ ÃÃÂ¾Ã·ÃÃ Â¸Ã°ÂµÃ Â³Â¡Â³ÂªÂ¸Ã© ÃÃ ÃÃÂ·Â¡Â±Ã ÃÃÃÂ¼Â°Â¡ ÃÃÂ¿Ã¤Â¾Ã¸Â´Ã.
	m_start	= TRUE;

	m_bTestServer = FALSE;
}

CServerSystem::~CServerSystem()
{
	//CoUninitialize();

	delete g_pUserTable;
	delete g_pServerTable;
	delete g_pObjectFactory;
	delete m_pGridSystem;
}

void CServerSystem::Start(WORD ServerNum)
{	
	m_bCheckProcessTime = FALSE;
	m_dwProcessCount = 0;

	m_wMapNum = ServerNum;
//AOÂ¨Ã¶A Â¡Â¤IAA
	SetNation();
	BOOTMNGR->AddSelfBootList(MAP_SERVER, ServerNum, g_pServerTable);

	/* MENU_CUSTOM_INFO menu[] = {
		{"Assert MsgBox", ButtonProc1},
		{"Load HackCheck", ButtonProc2},
		{"Put time", ButtonProc3},
		{"console", ButtonProc4},
		{"..toggle stat.", ButtonToggleStatOfTrigger},
		{"..toggle msg", ButtonTogglePeriodicMessageOfTrigger},
		{"..toggle Time", ButtonToggleProcessTime}
	};
	
	if(FALSE == g_Console.Init(sizeof(menu)/sizeof(*menu), menu, OnCommand))
	{
		MessageBox(
			0,
			"Console initializing Failed",
			0,
			0);
	} */

	g_hWnd = GetActiveWindow();

	//char TitleText[128];
	//sprintf(TitleText, "MAP%d(%s)", ServerNum, GetMapName(ServerNum) );
	//SetWindowText(g_hWnd, TitleText);

	// 080703 LYW --- ServerSystem : Â¼Â­Â¹Ã¶ ÃÃÂ¼ÃÃÂ¢Â¿Â¡ ÃÂ³ÃÂ½ÃÂ¸Â·Ã Â¼Â­Â¹Ã¶ Â¹Ã¶ÃÃ¼ÂºÃÃÃ ÃÃ¢Â·ÃÃÃÂ´Ã.
	//g_Console.LOG(4, "Â¡Â¡") ;
	g_Console.LOG(4, "[ Server Version : %s ]", g_SERVER_VERSION) ;
	//g_Console.LOG(4, "Â¡Â¡") ;
	
	//DWORD    dwProcessID = NULL;
    //DWORD    dwTreadID = ::GetWindowThreadProcessId(g_hWnd, &dwProcessID );

	// load hackcheck
	LoadHackCheck();

	FILE* fpstart = fopen("serverStart.txt","w");
#define STARTLOG(a)	fprintf(fpstart,#a);	a;
//	STARTLOG(CONDITIONMGR->Initial());
//	STARTLOG(CONDITIONMGR->LoadConditionList());
	STARTLOG(SKILLMGR->Init());

	STARTLOG(GAMERESRCMNGR->LoadMapChangeArea(GAMERESRCMNGR->m_MapChangeArea));
	STARTLOG(GAMERESRCMNGR->LoadLoginPoint(GAMERESRCMNGR->m_LoginPoint));
	STARTLOG(GAMERESRCMNGR->LoadMonsterList());
#ifdef _TESTCLIENT_
	STARTLOG(GAMERESRCMNGR->LoadSMonsterList());
#endif
	STARTLOG(BOSSMONMGR->LoadSummonInfoList());

	STARTLOG(GAMERESRCMNGR->LoadMonsterRewardList());

	STARTLOG(BOSSMONMGR->LoadBossMonsterInfoList());
	STARTLOG(BOSSREWARDSMGR->LoadBossRewardsInfo());
	
	// ÃÃÂµÃ¥ÂºÂ¸Â½Âº - 05.12 ÃÃÂ¿ÂµÃÃ
	STARTLOG(FIELDBOSSMONMGR->Init());
	STARTLOG(GAMERESRCMNGR->LoadNpcList());
	STARTLOG(GAMERESRCMNGR->LoadStaticNpc());
	// 06. 05 HIDE NPC - ÃÃÂ¿ÂµÃÃ
	STARTLOG(GAMERESRCMNGR->LoadHideNpcList());	
	STARTLOG(GAMERESRCMNGR->LoadSkillMoney());	
	STARTLOG(GAMERESRCMNGR->LoadExpPoint());
	STARTLOG(GAMERESRCMNGR->LoadFishingExpPoint());
	STARTLOG(GAMERESRCMNGR->LoadPlayerxMonsterPoint());
	// 080826 KTH -- Load Npc Buff List
	STARTLOG(NPCRECALLMGR->LoadNPCBuffList());

	STARTLOG(STORAGEMGR->LoadStorageList());
	STARTLOG(ITEMMGR->LoadItemList());	
	STARTLOG(ITEMMGR->LoadDealerItem());

	STARTLOG(ITEMMGR->LoadMonSummonItemInfo());
	STARTLOG(ITEMMGR->LoadNpcSummonItemInfo());
	STARTLOG(ITEMMGR->LoadScriptFileDataChangeItem());	
	STARTLOG(MON_SPEECHMGR->LoadMonSpeechInfoList());
	STARTLOG(PETMGR->LoadPetInfo());
	STARTLOG(m_Map.InitMap(ServerNum));
	STARTLOG(PATHMGR->SetMap(&m_Map, ServerNum, m_Map.GetTileWidth(ServerNum)));
	STARTLOG(PKMGR->Init(m_Map.IsPKAllow()));
	STARTLOG(LoadEventRate("./System/Resource/droprate.txt"));

	// quest
	STARTLOG(QUESTMGR->LoadQuestScript());
	STARTLOG(QUESTREGENMGR->LoadData());	
	AUTONOTEMGR->Init();

	g_pServerMsgParser[MP_POWERUP] = MP_POWERUPMsgParser;
	g_pServerMsgParser[MP_CHAR] = MP_CHARMsgParser;
	g_pServerMsgParser[MP_ITEM] = MP_ITEMMsgParser;
	g_pServerMsgParser[MP_CHAT] = MP_CHATMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pServerMsgParser[MP_MOVE] = MP_MOVEMsgParser;
	g_pServerMsgParser[MP_SKILLTREE] = MP_SKILLTREEMsgParser;
	g_pServerMsgParser[MP_CHEAT] = MP_CHEATMsgParser;
	g_pServerMsgParser[MP_QUICK] = MP_QUICKMsgParser;
	g_pServerMsgParser[MP_PARTY] = MP_PARTYMsgParser;
	g_pServerMsgParser[MP_SKILL] = MP_SkillMsgParser;
	g_pServerMsgParser[MP_STORAGE] = MP_STORAGEMsgParser;
	g_pServerMsgParser[MP_BATTLE] = MP_BattleMsgParser;
	g_pServerMsgParser[MP_CHAR_REVIVE] = MP_REVIVEMsgParser;
	g_pServerMsgParser[MP_EXCHANGE] = MP_EXCHANGEMsgParser;
	g_pServerMsgParser[MP_STREETSTALL] = MP_STREETSTALLMsgParser;
	g_pServerMsgParser[MP_NPC] = MP_NPCMsgParser;
	g_pServerMsgParser[MP_QUEST] = MP_QUESTMsgParser;
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;
	g_pServerMsgParser[MP_PK] = MP_PKMsgParser;
	g_pServerMsgParser[MP_HACKCHECK] = MP_HACKCHECKMsgParser;
	g_pServerMsgParser[MP_GUILD] = MP_GUILDMsgParser;
	g_pServerMsgParser[MP_GUILD_WAR] = MP_GUILDFIELDWARMsgParser;
	g_pServerMsgParser[MP_GTOURNAMENT] = MP_GTOURNAMENTMsgParser;
	g_pServerMsgParser[MP_GUILD_UNION] = MP_GUILUNIONMsgParser;
	g_pServerMsgParser[MP_FACIAL] = MP_FACIALMsgParser;
	g_pServerMsgParser[MP_EMOTION] = MP_EMOTIONMsgParser ;
	g_pServerMsgParser[MP_FAMILY] = MP_FAMILYMsgParser;
	g_pServerMsgParser[MP_FARM] = MP_FARM_MsgParser;
	g_pServerMsgParser[MP_RESIDENTREGIST] = MP_RESIDENTREGIST_MsgParser;
	g_pServerMsgParser[MP_TUTORIAL] = MP_TUTORIALMsgParser ;
	g_pServerMsgParser[MP_DATE] = MP_DATE_MsgParser;
	g_pServerMsgParser[MP_AUTONOTE] = MP_AUTONOTE_MsgParser;
	g_pServerMsgParser[MP_FISHING] = MP_FISHING_ServerMsgParser;
	g_pServerMsgParser[MP_PET] = MP_PET_MsgParser;
  	g_pServerMsgParser[MP_SIEGEWARFARE] = MP_SIEGEWARFAREMsgParser;
   	g_pServerMsgParser[MP_SIEGERECALL] = MP_SIEGERECALL_MsgParser ;
  	g_pServerMsgParser[MP_RECALLNPC] = MP_RECALLNPC_MsgParser ;
	g_pServerMsgParser[MP_LIMITDUNGEON] = MP_LIMITDUNGEON_MsgParser ;
	g_pServerMsgParser[MP_COOK] = MP_COOK_MsgParser;
	g_pServerMsgParser[MP_VEHICLE] = MP_VEHICLE_MsgParser;
	g_pServerMsgParser[MP_HOUSE] = MP_HOUSE_MsgParser;
	g_pServerMsgParser[MP_TRIGGER] = MP_TRIGGER_MsgParser;
	g_pServerMsgParser[MP_DUNGEON] = MP_DUNGEON_MsgParser;
	g_pServerMsgParser[MP_PCROOM] = MP_PCROOM_MsgParser;
	g_pServerMsgParser[MP_TRIGGER] = &(Trigger::CManager::NetworkMsgParser);
	g_pServerMsgParser[MP_CONSIGNMENT] = MP_CONSIGNMENT_MsgParser;
	g_pServerMsgParser[MP_NOTE] = MP_NOTE_MsgParser;
	STARTLOG( CHANNELSYSTEM->Init( ServerNum, m_Map.GetChannelNum() ) );

	STARTLOG(g_pAISystem.LoadAIGroupList());

	/* CUSTOM_EVENT customEvent[] = {
		{10, ProcessServer},
		{100, ProcessGame},
		{1000 * 60 * 5, ProcessCheck},
	};

	DESC_NETWORK desc = {0};
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectUser = OnDisconnectUser;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;
	desc.dwCustomDefineEventNum	= (sizeof(customEvent) / sizeof( *customEvent));
	desc.pEvent = customEvent;
	desc.dwMainMsgQueMaxBufferSize = 20480000;
	desc.dwMaxServerNum = 50;
	desc.dwMaxUserNum = 10;
	desc.dwServerBufferSizePerConnection = 512000;
	desc.dwServerMaxTransferSize = 65535;
	desc.dwUserBufferSizePerConnection = 65535;
	desc.dwUserMaxTransferSize = 65535;
	desc.dwConnectNumAtSameTime = 100;
	desc.dwFlag = 0; */

	//STARTLOG(g_Network.Init(&desc));
	
	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
		//ASSERT(0);
	}
	/*if(!BOOTMNGR->ConnectToMS(&g_Network, g_pServerTable))
	{
		SERVERINFO info;
		info.wServerKind = MONITOR_SERVER;
		OnConnectServerFail(&info);		
	}*/

	//////////////////////////////////////////////////////////////////////////
	// DB Â¡Â§uÂ¨ÃÂ©Âªd
	DWORD maxthread = g_pServerSystem->GetMap()->GetMaxDBThread();
	DWORD maxqueryinsametime = g_pServerSystem->GetMap()->GetMaxQueryInSameTime();

	//if(g_DB.Init(maxthread,maxqueryinsametime,FALSE) == FALSE)
	//	MessageBox(NULL,"DataBase Initializing Failed",0,0);


	// guildfieldwar
	GUILDWARMGR->Init();
	SIEGEWARFAREMGR->LoadMapInfo();
  	SIEGEWARFAREMGR->Init() ;
  	SIEGEDUNGEONMGR->LoadInfo();
  	SIEGERECALLMGR->Initialieze() ;
	
	// Guild Tournament
	GTMGR->Init();

	// Weather System
	// 080328 NYJ --- Â³Â¬Â½ÃÂ½ÃÂ½ÂºÃÃÃÃÂ°Â¡Â·Ã Â³Â¯Â¾Â¾Â°Ã¼Â¸Â®ÃÃ Â»Ã§Â¿Ã«
	WEATHERMGR->Init();
	FISHINGMGR->Init();
	GAMERESRCMNGR->LoadHousing_AllList();
	HOUSINGMGR->Init();
	DungeonMGR->Init();
	GAMERESRCMNGR->LoadDungeonKeyList();
	PartyLoad(0);
	GuildLoadGuild(0);
	// 090316 LUJ, Â»ÃµÂ·Ã Â½ÃÃÃÂµÃ Â¸ÃÂ¿Â¡Â¼Â­Â´Ã ÃÃÂµÂ¿ Â¼ÃÃÂ¯/ÃÂ¾Â½ÃÃÃÃÃ¶ Â¾ÃÂµÂµÂ·Ã ÃÃÂ±Ã¢ÃÂ­ÃÃÂ´Ã
	ResetVehicle( GetMapNum() );
	GuildResetWarehouse( m_wMapNum );
	// 100305 ONS Â¸ÃÂ±Ã¢ÂµÂ¿Â½Ã DBÃÃ PCÂ¹Ã¦ Â¹Ã¶ÃÃÃÂ¤ÂºÂ¸Â¸Â¦ ÃÃÂ±Ã¢ÃÂ­ÃÃÂ´Ã.
	PCROOMMGR->Init();
	
	// S Â³Ã³ÃÃ¥Â½ÃÂ½ÂºÃÃ ÃÃÂ°Â¡ added by hseos 2007.04.30	2007.09.07
	// 081017 LUJ, Â³Ã³ÃÃ¥ ÃÂ¤ÂºÂ¸Â¸Â¦ ÃÃÂ°Ã½ ÃÃµÂ¸Â®ÃÃÂµÂµÂ·Ã Â¼Ã¶ÃÂ¤
	{
		g_csFarmManager.Init( GetMapNum() );
		int farmZoneId = 0;
		
		// 090520 LUJ, Â°Â¢Â°Â¢ÃÃ Â³Ã³ÃÃ¥ ÃÂ¤ÂºÂ¸Â¸Â¦ ÃÃÂ¾Ã®Â¿Ã Â¼Ã¶ ÃÃÂµÂµÂ·Ã Â¼Ã¶ÃÂ¤
		if( TRUE == g_csFarmManager.GetFarmZoneID( GetMapNum(), &farmZoneId ) )
		{
			CSHFarmZone* const farmZone = g_csFarmManager.GetFarmZone( WORD( farmZoneId ) );

			if( farmZone )
			{
				g_DB.FreeQuery(
					eFarm_LoadFarmState,
					farmZoneId,
					"EXEC dbo.MP_FARM_SELECT %d",
					farmZoneId );

				// 091126 pdy 0Â¹Ã¸ Â³Ã³ÃÃ¥ÃÃ 0Â¹Ã¸ ÃÃÂ¹Â°,Â°Â¡ÃÃ ÃÃ Â·ÃÂµÃ¥Â°Â¡ Â¾ÃÂµÃÂ´Ã Â¹Ã¶Â±Ã Â¼Ã¶ÃÂ¤   
				g_DB.FreeQuery(
					eFarm_LoadCropInfo,
					farmZoneId,
					"EXEC dbo.MP_FARM_CROP_SELECT %d, 0, -1",
					farmZoneId );

				g_DB.FreeQuery(
					eFarm_LoadAnimalInfo,
					farmZoneId,
					"EXEC dbo.MP_FARM_ANIMAL_SELECT %d, 0, -1",
					farmZoneId );
			}
		}
	}

	g_csDateManager.LoadChallengeMonsterInfo(m_wMapNum);
	NPCMOVEMGR->Init();

	m_dwQuestTime = gCurTime;
	m_bQuestTime = FALSE;
	NPCRECALLMGR->Initialize();
	
// --- skr 12/01/2020
  RELIFEEMGR->Init();
  RELIFEEMGR->setmapnum(m_wMapNum);
  RELIFEEMGR->LoadSetup();

	//MiniDumper md(MiniDumper::DUMP_LEVEL_0);

	g_Console.LOG(4, "-----------   MAP SERVER START  -----------------");
	g_Console.LOG(4, "-----------   Map Number %d     -----------------", GAMERESRCMNGR->GetLoadMapNum());

	g_bReady = TRUE;

	fclose(fpstart);
	g_Console.WaitMessage();
	printf( "-----------   %s   ---------\n", GetMapName(ServerNum) );
}

void CServerSystem::End()
{
#ifdef _MYLUNA_
	TRIGGERMGR->ReleaseME();
#endif
	SetStart(FALSE);

	g_Network.Release();

	if(FALSE == GetCharUpdateCompletion())
	{
		HandlingBeforeServerEND();
	}

	SKILLMGR->Release();
	g_Console.Release();

	BATTLESYSTEM->Release();

	SAFE_DELETE( g_pUserTable );
	SAFE_DELETE( g_pServerTable );
	SAFE_DELETE( g_pObjectFactory );
	SAFE_DELETE( m_pGridSystem );
	AUTONOTEMGR->Release();
	m_Map.Release();
	//CoFreeUnusedLibraries();
}

void CServerSystem::Process()
{
	QueryPerformanceFrequency(&m_freq);

	// 071218 LUJ
	processTime.mPreviousTick	= GetTickCount();

	if(g_bCloseWindow)
	{
		SendMessage(g_hWnd, WM_CLOSE, 0,0);
		g_bCloseWindow = FALSE;
		SetStart(FALSE);
	}

	if(GetStart() == FALSE)
		return;

	MHTIMEMGR_OBJ->Process();

	if(!g_pUserTable)
		return;

	static DWORD dwLastProcessTime = 0;

	if( gCurTime > m_dwQuestTime+60000 )
	{
		m_dwQuestTime = gCurTime;
		m_bQuestTime = TRUE;
	}

	// 080602 NYJ --- Â³Â¯Â¾Â¾, Â³Â¬Â½Ã Process()
	WEATHERMGR->Process();
	FISHINGMGR->Process();
	COOKMGR->Process();
	
	// 091229 ShinJS --- PCÂ¹Ã¦ ÃÃÂºÂ¥ÃÂ® Process
	PCROOMMGR->Process();

	QueryPerformanceCounter(&m_ObjLoopProc[0]);
	g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while( (pObject = (CObject*)g_pUserTable->GetUserData()) != NULL )
	{
		if(pObject->GetInited() == FALSE)
			continue;
		
		pObject->StateProcess();
		pObject->ProceedToTrigger();

		if(TRUE == pObject->GetStateInfo().bEndState &&
			pObject->GetStateInfo().State_End_Time < gCurTime)
		{
			// EndObjectState() Â¾ÃÂ¿Â¡Â¼Â­ CObjectÂ¸Â¦ ÃÃÃÂ¦ÃÃÂ´Ã! Â°Ã¨Â¼Ã ÃÃ¸ÃÃ ÃÃ Â°Ã¦Â¿Ã¬ ÃÃÃÂ¦ÂµÃ Â¸ÃÂ¸Ã°Â¸Â®Â¸Â¦ Â¾ÃÂ¼Â¼Â½ÂºÃÃÂ°Ã ÂµÃÂ´Ã
			OBJECTSTATEMGR_OBJ->EndObjectState(
				pObject,
				pObject->GetState());
			continue;
		}

		CCharMove::MoveProcess(pObject);

		CCharacterCalcManager::UpdateLife(pObject);

		if(pObject->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer* pPlayer = (CPlayer*)pObject;
			
// --- skr 12/01/2020
      if( pPlayer->IsRelifeON() ) { pPlayer->UpdateRelife(); }
			
			CCharacterCalcManager::ProcessLife(pPlayer);
			CCharacterCalcManager::UpdateMana(pPlayer);	
			CCharacterCalcManager::ProcessMana(pPlayer);

			pPlayer->QuestProcess();
			pPlayer->CheckImmortalTime();
			if( m_bQuestTime )
			{
				// add quest event
				CQuestEvent QEvent( eQuestEvent_Time, 0, 0 );
				QUESTMGR->AddQuestEvent( pPlayer, &QEvent );
			}

			pPlayer->ProcessTimeCheckItem( FALSE );


			// Â³Â¬Â½ÃÂ°Ã¼Â·ÃÃÂ³Â¸Â® : FISHINGMGR::Process() Â°Â¡ ÃÂ£ÃÃ¢ÂµÃ ÃÃÃÃÂ¿Â¡ Â»Ã§Â¿Ã«ÂµÃÂ¾Ã®Â¾Ã ÃÃ.
			//                ÃÂ¯ÃÃºÃÃÃÃÂºÃ­ÃÂ» ÃÃÂºÂ¹Â¼Ã¸ÃÂ¸ ÃÃÃÃ¶ Â¾ÃÂ±Ã¢ ÃÂ§ÃÃ ÃÃÂ°Ã·Â¿Â¡Â¼Â­ ÃÂ³Â¸Â®ÃÃ.
			FISHINGMGR->ProcessPlayer(pPlayer);
		}
		else if(pObject->GetObjectKind() & eObjectKind_Monster)
		{
			CMonster* pMonster = (CMonster*)pObject;
			pMonster->Process();
		}
		else if(pObject->GetObjectKind() == eObjectKind_Npc)
		{
			CNpc* pNpc = (CNpc*)pObject;
			pNpc->Process();
		}
		else if(pObject->GetObjectKind() == eObjectKind_Pet)
		{
			// 091110 ONS ÃÃª Â¾ÃÂ´ÃÂ¸ÃÃÃÂ¼Ã ÃÃÂ°Â¡ : ÃÃªÃÃ ÃÃÂ¾ÃºÃÂ» Â°Ã¦Â¿Ã¬ ÂºÃ¼ÃÂ®Â³ÂªÂ°Â£Â´Ã.
			if(pObject->GetDieFlag() == TRUE)
				continue;

			CPet* pPet = (CPet*)pObject;
			CCharacterCalcManager::ProcessPetLife(pPet);
			CCharacterCalcManager::ProcessPetMana(pPet);
			pPet->FriendlyProcess();
		}
	}
	QueryPerformanceCounter(&m_ObjLoopProc[1]);

	FISHINGMGR->ChangeMissionState();	// ÃÂ¯ÃÃºÃÃÃÃÂºÃ­ Â¼Ã¸ÃÂ¸ÃÃÂ¿Â¡ Â¹ÃÂ¼ÃÂ»Ã³ÃÃ ÂºÂ¯Â°Ã¦ÃÂ» ÃÃÂ¾ÃÃÃ.
	FIELDBOSSMONMGR->Process();

	SKILLMGR->Process();
	BATTLESYSTEM->Process();
	g_pAISystem.Process();
	LOOTINGMGR->ProcessTimeOutCheck();
	QUESTMGR->Process();
	GUILDMGR->Process();
	GTMGR->Process();

	SIEGEWARFAREMGR->Process();
	
	m_bQuestTime = FALSE;

//--- Test
	m_dwMainProcessTime = gCurTime - dwLastProcessTime;
	dwLastProcessTime = gCurTime;

	// desc_hseos_Â³Ã³ÃÃ¥Â½ÃÂ½ÂºÃÃ_01
	// S Â³Ã³ÃÃ¥Â½ÃÂ½ÂºÃÃ ÃÃÂ°Â¡ added by hseos 2007.04.12
	CSHMain::MainLoop();
	// E Â³Ã³ÃÃ¥Â½ÃÂ½ÂºÃÃ ÃÃÂ°Â¡ added by hseos 2007.04.12

//---KES AUTONOTE
	AUTONOTEMGR->Process();
//---------------

	// desc_hseos_ÂµÂ¥ÃÃÃÂ® ÃÂ¸_01
	// S ÂµÂ¥ÃÃÃÂ® ÃÂ¸ ÃÃÂ°Â¡ added by hseos 2007.11.27
	// ..Â¸Ã Â¾ÃÂ·Â¡Â¿Â¡ ÃÃÃÃ¼ LocalTime ÃÂ» ÃÃºÃÃ¥ÃÃ ÂµÃÂ´Ã. Â²Ã Â¸Ã Â¾ÃÂ·Â¡Â¿Â¡ ÃÃÂ¾Ã®Â¾Ã ÃÃ. Â±ÃÂ·Â¸ÃÃ¶ Â¾ÃÂ°Ã­
	// ..ÃÃ ÃÃÂ¼Ã¶ÂºÂ¸Â´Ã Â¾ÃÂ·Â¡Â¿Â¡ ÃÂ§ÃÂ¡ÃÃÂ´Ã ÃÃÂ¼Ã¶Â°Â¡ ÃÃÃÂ» Â°Ã¦Â¿Ã¬ Â±Ã ÃÃÂ¼Ã¶Â¿Â¡Â¼Â­ OldLocalTime ÃÂ» Â»Ã§Â¿Ã«ÃÃÂ¸Ã© OldLocalTime Â°ÂªÃÃ Â¼Â³ÃÂ¤ÂµÃÃÃ¶ Â¾ÃÃÂ½.
	MHTIMEMGR_OBJ->ProcOldLocalTime();
	LIMITDUNGEONMGR->Process() ;
	
	QueryPerformanceCounter(&m_TriggerProc[0]);
	TRIGGERMGR->Process();
	QueryPerformanceCounter(&m_TriggerProc[1]);
	HOUSINGMGR->Process();
	QueryPerformanceCounter(&m_DungeonProc[0]);
	DungeonMGR->Process();
	QueryPerformanceCounter(&m_DungeonProc[1]);
	NPCRECALLMGR->Process() ;
	
	// 071218 LUJ
	{
		processTime.mCurrentTick	=	GetTickCount();
		processTime.mSpace			=	processTime.mCurrentTick - processTime.mPreviousTick;
		processTime.mMaxSpace		=	my_max( processTime.mSpace, processTime.mMaxSpace );
		processTime.mTotalSpace		+=	processTime.mSpace;
		processTime.mAverageSpace	=	float( processTime.mTotalSpace ) / ++processTime.mCount;
	}

	if(m_bCheckProcessTime)
	{
		static DWORD dwLastPutLogTime = gCurTime;

		double fTimeObjLoopProc = (double)(m_ObjLoopProc[1].QuadPart - m_ObjLoopProc[0].QuadPart) / m_freq.QuadPart;
		double fTimeTriggerProc = (double)(m_TriggerProc[1].QuadPart - m_TriggerProc[0].QuadPart) / m_freq.QuadPart;
		double fTimeDungeonProc = (double)(m_DungeonProc[1].QuadPart - m_DungeonProc[0].QuadPart) / m_freq.QuadPart;

		m_fAvrObjLoopProc += fTimeObjLoopProc;
		m_fAvrTriggerProc += fTimeTriggerProc;
		m_fAvrDungeonProc += fTimeDungeonProc;
		m_dwProcessCount++;

		if(m_fTimeObjLoopProc < fTimeObjLoopProc)
			m_fTimeObjLoopProc = fTimeObjLoopProc;

		if(m_fTimeTriggerProc < fTimeTriggerProc)
			m_fTimeTriggerProc = fTimeTriggerProc;

		if(m_fTimeDungeonProc < fTimeDungeonProc)
			m_fTimeDungeonProc = fTimeDungeonProc;

		
		if(gCurTime > dwLastPutLogTime + 5000)
		{
			if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
			{
				Trigger::PutLog("DungeonInfo : %dDungeons, %dUsers, %dMonsters, %dNpcs, %dExtras",
					DungeonMGR->GetDungeonNum(),
					DungeonMGR->GetPlayerNum(),
					DungeonMGR->GetMonsterNum(),
					DungeonMGR->GetNpcNum(),
					DungeonMGR->GetExtraNum());
			}

			Trigger::PutLog("	Object Proc (High:%f, Avr:%f)", m_fTimeObjLoopProc, m_fAvrObjLoopProc/=m_dwProcessCount);
			Trigger::PutLog("	TriggerProc (High:%f, Avr:%f)", m_fTimeTriggerProc, m_fAvrTriggerProc/=m_dwProcessCount);
			Trigger::PutLog("	DungeonProc (High:%f, Avr:%f)", m_fTimeDungeonProc, m_fAvrDungeonProc/=m_dwProcessCount);

			dwLastPutLogTime = gCurTime;
		}
	}
}

void CServerSystem::SetStart( BOOL state )
{
	m_start = state;
	if( m_start )
	{
		g_Console.LOG( 4, "-----------   MAP SERVER LOAD DATA FROM DB  -----------------");
	}
}

CPlayer* CServerSystem::AddPlayer(DWORD dwPlayerID,DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum, eUSERLEVEL userLevel)
{
	CObject* pPreObj = g_pUserTable->FindUser(dwPlayerID);
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		pPreObj->SetNotInited();
		RemovePlayer(dwPlayerID);
	}

	CHANNELSYSTEM->SetChallengeZoneAgentChannelID(ChannelNum);

	enum Alert
	{
		AlertNone,
		AlertDungeon,
		AlertHouse,
	}
	alertType = AlertNone;

	VECTOR3* pHouseStartPos = NULL;
	DWORD num = CHANNELSYSTEM->GetChannelID(ChannelNum);

	if( QUESTMAPMGR->IsQuestMap() )
	{
		num = CHANNELSYSTEM->CreateQuestMapChannel(dwPlayerID);
	}
	else if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	{
		num = DungeonMGR->GetChannelIDFromReservationList(dwPlayerID);
		if(0 == num)
		{
			// Â¿ÃÃÃºÂ¹Ã¶ÃÃ Â°Ã¦Â¿Ã¬ ÃÂ¤Â³ÃÂ¹Ã¸ÃÂ£Â¸Â¦ ÃÃÂ´Ã§Â¹ÃÂ°Ã­ ÂµÃ©Â¾Ã®Â¿ÃÂ´Ã.
			num = ChannelNum;

			if( ChannelNum == 0 )
				alertType = AlertDungeon;
		}
	}
	else if(HOUSINGMAPNUM == g_pServerSystem->GetMapNum())
	{
		num = HOUSINGMGR->GetChannelIDFromReservationList(dwPlayerID);

		if(0 == num)
		{
			alertType = AlertHouse;
		}
		else
		{
			pHouseStartPos = HOUSINGMGR->GetStartPosFromeservationList(dwPlayerID);
		}
	}

	if(0 == num)
	{
		if(eUSERLEVEL_USER <= userLevel)
		{
			return 0;
		}

		const int temporaryChannel = 1;
		num = temporaryChannel;
	}

	BASEOBJECT_INFO binfo;
	ZeroMemory(
		&binfo,
		sizeof(binfo));
	binfo.dwObjectID = dwPlayerID;

	CPlayer* pPlayer = (CPlayer*)g_pObjectFactory->MakeNewObject(eObjectKind_Player,dwAgentNum, &binfo);

	if(pHouseStartPos)
	{
		CCharMove::SetPosition(
		pPlayer,
		pHouseStartPos);
	}

	pPlayer->SetUniqueIDinAgent(
		UniqueIDinAgent);
	pPlayer->SetBattleID(
		num);
	pPlayer->SetGridID(
		num);
	pPlayer->SetChannelID(
		num);
	pPlayer->SetCurChannel(
		ChannelNum);
	pPlayer->InitClearData();
	pPlayer->SetInitState(
		PLAYERINITSTATE_ONLY_ADDED,
		0);
	pPlayer->SetUserLevel(
		userLevel);

	CHANNELSYSTEM->IncreasePlayerNum(pPlayer->GetChannelID());
	g_pUserTable->AddUser(pPlayer,pPlayer->GetID());

	switch(alertType)
	{
	case AlertDungeon:
		{
			TESTMSG message;
			ZeroMemory(
				&message,
				sizeof(message));
			message.Category = MP_SIGNAL;
			message.Protocol = MP_SIGNAL_COMMONUSER;
			SafeStrCpy(
				message.Msg,
				"This is a dungeon which you need to have specific items to enter. For detailed info, please ask programmer about them",
				_countof(message.Msg));

			pPlayer->SendMsg(
				&message,
				message.GetMsgLength());
			

			FILE* fpLog = NULL;
			fpLog = fopen( "./Log/DungeonEnterLog.txt", "a+" );
			if( fpLog )
			{
				SYSTEMTIME sysTime;
				GetLocalTime( &sysTime );

				fprintf( fpLog, "[%04d-%02d-%02d %02d:%02d:%02d] %s - AlertDungeon [PlayerID : %d, AgentNum : %d, ChannelNum : %d, UserLv : %d] \n", 
					sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond,
					__FUNCTION__,
					dwPlayerID, dwAgentNum, num, userLevel );

				fclose( fpLog );
			}

			break;
		}
	case AlertHouse:
		{
			TESTMSG message;
			ZeroMemory(
				&message,
				sizeof(message));
			message.Category = MP_SIGNAL;
			message.Protocol = MP_SIGNAL_COMMONUSER;
			SafeStrCpy(
				message.Msg,
				"This is a house which you need to have specific items to enter. For detailed info, please ask programmer about them",
				_countof(message.Msg));

			pPlayer->SendMsg(
				&message,
				message.GetMsgLength());
			break;
		}
	}

// --- skr 20012020
	pPlayer->SetRelifeTimer( RELIFEEMGR->getBuffRemainTime() );

	return pPlayer;
}

CPlayer* CServerSystem::InitPlayerInfo(BASEOBJECT_INFO* pBaseObjectInfo,CHARACTER_TOTALINFO* pTotalInfo,HERO_TOTALINFO* pHeroInfo)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID);
	if(pPlayer == NULL)
		return NULL;
	pBaseObjectInfo->BattleID = pPlayer->GetBattleID();
	ASSERT(pPlayer->GetID() == pBaseObjectInfo->dwObjectID);
	pPlayer->Init(eObjectKind_Player,pPlayer->GetAgentNum(),pBaseObjectInfo);
	pPlayer->InitCharacterTotalInfo(pTotalInfo);
	pPlayer->InitHeroTotalInfo(pHeroInfo);

	return pPlayer;
}

CMonster* CServerSystem::AddMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos,WORD wObjectKind)
{
	EObjectKind objectkind = eObjectKind_Monster;
	
	switch( wObjectKind )
	{
	case eObjectKind_SpecialMonster:
	case eObjectKind_ToghterPlayMonster:
	case eObjectKind_Trap:
	case eObjectKind_Vehicle:
		{
			objectkind = EObjectKind( wObjectKind );
			break;
		}
	}

	CMonster* pMonster = (CMonster*)g_pObjectFactory->MakeNewObject(objectkind,0, pBaseObjectInfo);
	pMonster->SetSubID(dwSubID);
	pMonster->InitMonster(pTotalInfo);
	pMonster->SetGridID(pBaseObjectInfo->BattleID);

	UpdateFiniteStateMachine(
		*pMonster,
		dwSubID);

	pMonster->SetInited();

	CCharMove::InitMove(pMonster,pPos);

	g_pUserTable->AddUser(pMonster,pMonster->GetID());

	// 100614 ShinJS --- ÃÂ»Â°ÃÃÂº AI ÂµÃ®Â·ÃÂ¾ÃÃÃ
	if( objectkind != eObjectKind_Vehicle )
	{
		g_pAISystem.AddObject(
			pMonster);
	}
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pMonster);

	AddMonsterCount(*pMonster);
	return pMonster;
}

CBossMonster* CServerSystem::AddBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CBossMonster* pBossMonster = (CBossMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_BossMonster,0, pBaseObjectInfo);
	pBossMonster->SetSubID(dwSubID);
	pBossMonster->InitMonster(pTotalInfo);
	pBossMonster->SetGridID(pBaseObjectInfo->BattleID);
	pBossMonster->SetInited();
	
	CCharMove::InitMove(pBossMonster,pPos);
	

	g_pUserTable->AddUser(pBossMonster,pBossMonster->GetID());
	g_pAISystem.AddObject(pBossMonster);
	BOSSMONMGR->SetBossInfo(pBossMonster);
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pBossMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pBossMonster);

	AddMonsterCount(*pBossMonster);
	return pBossMonster;
}

// ÃÃÂµÃ¥ÂºÂ¸Â½Âº - 05.12 ÃÃÂ¿ÂµÃÃ
// ÃÃÂµÃ¥ÂºÂ¸Â½Âº ÃÃÂ°Â¡ ÃÃÂ¼Ã¶
CFieldBossMonster* CServerSystem::AddFieldBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CFieldBossMonster* pFieldBossMonster = (CFieldBossMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_FieldBossMonster,0, pBaseObjectInfo);
	pFieldBossMonster->SetSubID(dwSubID);
	pFieldBossMonster->InitMonster(pTotalInfo);
	pFieldBossMonster->SetGridID(pBaseObjectInfo->BattleID);
	pFieldBossMonster->SetInited();
	
	CCharMove::InitMove(pFieldBossMonster,pPos);

	g_pUserTable->AddUser(pFieldBossMonster,pFieldBossMonster->GetID());
	g_pAISystem.AddObject(pFieldBossMonster); 
		
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pFieldBossMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pFieldBossMonster);

	AddMonsterCount(*pFieldBossMonster);
	return pFieldBossMonster;
}

// ÃÃÂµÃ¥ÂºÂ¸Â½Âº - 05.12 ÃÃÂ¿ÂµÃÃ
// ÃÃÂµÃ¥ÂºÂ¸Â½Âº ÂºÃÃÃ ÃÃÂ°Â¡ ÃÃÂ¼Ã¶
CFieldSubMonster* CServerSystem::AddFieldSubMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CFieldSubMonster* pFieldSubMonster = (CFieldSubMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_FieldSubMonster,0, pBaseObjectInfo);
	pFieldSubMonster->SetSubID(dwSubID);
	pFieldSubMonster->InitMonster(pTotalInfo);
	pFieldSubMonster->SetGridID(pBaseObjectInfo->BattleID);
	pFieldSubMonster->SetInited();
	
	CCharMove::InitMove(pFieldSubMonster,pPos);

	g_pUserTable->AddUser(pFieldSubMonster,pFieldSubMonster->GetID());

	g_pAISystem.AddObject(pFieldSubMonster); 
		
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pFieldSubMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pFieldSubMonster);

	AddMonsterCount(*pFieldSubMonster);
	return pFieldSubMonster;
}

CNpc* CServerSystem::AddNpc(BASEOBJECT_INFO* pBaseObjectInfo,NPC_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CNpc* pNpc = (CNpc*)g_pObjectFactory->MakeNewObject(eObjectKind_Npc,0, pBaseObjectInfo);

	// 081012 LYW --- ServerSystem : npc null ÃÂ¼ÃÂ©Â¸Â¦ ÃÃÂ´Ã.
	if( !pNpc ) return NULL ;

	pNpc->InitNpc(pTotalInfo);
	pNpc->SetGridID(pBaseObjectInfo->BattleID);
	pNpc->SetInited();
	CCharMove::InitMove(pNpc,pPos);

	g_pUserTable->AddUser(pNpc,pNpc->GetID());
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pNpc->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pNpc);

	return pNpc;
}

cSkillObject* CServerSystem::AddSkillObject(cSkillObject* pSkillObj,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pSkillObj->GetID()) == NULL);

	pSkillObj->SetInited();
	CCharMove::InitMove(pSkillObj,pPos);

	g_pUserTable->AddUser(pSkillObj,pSkillObj->GetID());
	
	return pSkillObj;
}

CMapObject* CServerSystem::AddMapObject(DWORD Kind, BASEOBJECT_INFO* pBaseObjectInfo, MAPOBJECT_INFO* pMOInfo, VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CMapObject* pMapObject = (CMapObject*)g_pObjectFactory->MakeNewObject((EObjectKind)Kind,0, pBaseObjectInfo);
	
	pMapObject->InitMapObject( pMOInfo );
	pMapObject->SetGridID( pBaseObjectInfo->BattleID );
	pMapObject->SetInited();
	CCharMove::InitMove( pMapObject, pPos );

	g_pUserTable->AddUser( pMapObject, pMapObject->GetID() );
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pMapObject->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle( pBattle, pMapObject );

	return pMapObject;
}

void CServerSystem::HandlingBeforeServerEND()
{
	//SetUnableProcess(TRUE);				//1. Process Â¸Â®ÃÃ & DBReturnMsg ÃÂ³Â¸Â® Â¾ÃÃÃ.

	//g_Console.LOG(4, "[0/4]Start Working For Map Server Shutdown" );

	RemoveServerForKind(AGENT_SERVER);	//2. AgentSrv Â²Ã·Â±Ã¢.
	//g_Console.LOG(4, "[1/4]Agent Disconnecting...Done" );

	//3. =======Object ÃÂ³Â¸Â®
	g_pUserTable->SetPositionHead();
	CObject* pObject;
	while( (pObject = g_pUserTable->GetData()) != NULL )
	{	
		if(pObject->GetObjectKind() == eObjectKind_Player)
			g_pServerSystem->RemovePlayer(pObject->GetID(), FALSE);
			//FALSE: Â½ÃÃÂ¦Â·Ã UserTableÂ¿Â¡Â¼Â­Â´Ã ÃÃ¶Â¿Ã¬ÃÃ¶ Â¾ÃÂ´ÃÂ´Ã.
		else
		{
			CCharMove::ReleaseMove(pObject);
			g_pObjectFactory->ReleaseObject(pObject);
			if(pObject->GetObjectKind() == eObjectKind_Monster ||
				pObject->GetObjectKind() == eObjectKind_BossMonster )
			{
				g_pAISystem.RemoveObject(pObject->GetID());
			}
		}
	}
	//g_Console.LOG(4, "[2/4]Start Object Handling (Update/Delete)...Done" );

	//g_pUserTable->RemoveAll();
	//g_Console.LOG(4, "[3/4]Removing All Object on UserTable...Done" );

	//g_Console.LOG(4, "[4/4]Start DB Release..." );
	g_DB.Release();
	//g_Console.LOG(4, "[4/4]Release...Done" );

	SetCharUpdateCompletion(TRUE);
}


void CServerSystem::RemovePlayer(DWORD dwPlayerID, BOOL bRemoveFromUserTable )
{	
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwPlayerID);
	if(!pPlayer)
		return;
	ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);
	if( pPlayer->GetObjectKind() != eObjectKind_Player ){
		return;
	}

	EXCHANGEMGR->UserLogOut(pPlayer);
	STREETSTALLMGR->UserLogOut(pPlayer);
	SHOWDOWNMGR->UserLogOut(pPlayer);
	LOOTINGMGR->LootingCancel(pPlayer);	//PKLOOT
	BOSSMONMGR->UserLogOut(pPlayer->GetID(), pPlayer->GetGridID());
	// 080725 KTH - ÃÃÃÂºÂ°ÃÂ·Ã ÃÂ³Â¸Â®ÃÃÃÃ;
	SIEGEWARFAREMGR->CancelWaterSeedUsing(pPlayer);

	// 071122
	pPlayer->ProcessTimeCheckItem( TRUE );

	if(pPlayer->GetInited() == TRUE)
	{
		if(pPlayer->GetState() == eObjectState_Die)
		{
			if( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_NONE &&
				pPlayer->IsPenaltyByDie() )
			{
				pPlayer->ReviveLogInPenelty(); 
			}
			else
			{
				pPlayer->ReviveAfterShowdown( FALSE );	//Â¡ÃaÂ¡ÃuAÂ©Ã¸Â¢Â¬Â¢Ã§Â¢Â¬Â¢Â¬.. msgÂ¨Â¬Â¢Â¬Â©Ã¸Â¡Ã­Ao Â¨ÃºEÂ¢Â¥AÂ¢Â¥U.
			}
		}

		if( pPlayer->IsPKMode() )
			pPlayer->SetPKModeEndtime();

		CharacterHeroInfoUpdate(pPlayer);
		CharacterTotalInfoUpdate(pPlayer);
		Option_Update(*pPlayer);
		CCharMove::ReleaseMove(pPlayer);

		BATTLESYSTEM->DeleteObjectFromBattle(pPlayer);

		MonsterMeter_Save(pPlayer->GetID(), 0, 0, pPlayer->GetMonstermeterInfo()->nPlayTimeTotal, pPlayer->GetMonstermeterInfo()->nKillMonsterNumTotal);

		if(g_pServerSystem->GetMapNum() != GTMAPNUM && pPlayer->IsEmergency())
		{
			VECTOR3* ppos;
			VECTOR3 pos;

			ppos = GAMERESRCMNGR->GetRevivePoint();

			if(ppos)	// NYJ ÃÃ·ÃÃÃÃÂ¹Â«ÃÂ¿Â»Ã³ÃÃÂ·Ã ÃÂ¢Â±ÃÃÃÂ¸Ã© Â´ÃÂ¿Ã®.
			{
				int temp;
				temp = rand() % 500 - 250;
				pos.x = ppos->x + temp;
				temp = rand() % 500 - 250;
				pos.z = ppos->z + temp;
				pos.y = 0;

				pPlayer->SetMapMoveInfo( GAMERESRCMNGR->GetLoadMapNum(), ( DWORD )( pos.x ), ( DWORD )( pos.z ) );
			}
		}
	}
	else		
	{
		CCharMove::ReleaseMove(pPlayer);
	}

	CHANNELSYSTEM->DecreasePlayerNum(
		pPlayer->GetChannelID());
	DWORD playerSizeInChannel = CHANNELSYSTEM->GetPlayerNumInChannel(
		pPlayer->GetChannelID());

	// 100111 LUJ, ÃÃÂ¿Ã¬Â½Âº/Â´Ã¸ÃÂ¯ Â¸ÃÃÂº ÃÃÂ·ÃÂ¼Â¼Â½Âº Â»Ã³ÃÂ¸Â·Ã ÃÂ«Â¿Ã®ÃÂ®Â°Â¡ Â´ÃÂ°Ã ÂµÃÂ¼Â­ Â°Â¨Â¼ÃÃÂ³Â¸Â® ÃÃÃÃ Â¾ÃÃÃÂ´Ã
	//			ÃÃÃÃ ÃÂ¤Â³Ã Â°Â³Â¼Ã¶Â¿Â¡ Â°Ã¼Â°Ã¨Â¾Ã¸ÃÃ ÃÂ³Â¸Â®ÃÃ Â¼Ã¶ ÃÃÂµÂµÂ·Ã Â¼Ã¶ÃÂ¤ ÃÃÂ¾Ã·ÃÃ Â¿Â¹ÃÂ¤
	if(HOUSINGMAPNUM == GetMapNum())
	{
		--playerSizeInChannel;
	}
	else if(DungeonMGR->IsDungeon(GetMapNum()) &&
		! pPlayer->IsDungeonObserver())	// Â´Ã¸ÃÃ¼Â¿ÃÃÃºÂ¹Ã¶Â´Ã ÃÃÂ¿Ã¸ÃÂ¼ÃÂ© ÃÃÃÃ¶ Â¾ÃÂ´ÃÂ´Ã.
	{
		--playerSizeInChannel;
	}

	if(0 == playerSizeInChannel)
	{
		TRIGGERMGR->StopServerTrigger(pPlayer->GetChannelID());
	}

	UnRegistLoginMapInfo(
		dwPlayerID);
	QUESTMAPMGR->RemovePlayer(
		pPlayer);

	BOOL bUpdateLogPos = TRUE;
	MAPTYPE map;
	DWORD	pos_x;
	DWORD	pos_z;

	pPlayer->GetMapMoveInfo( map, pos_x, pos_z );

	BOOL bMapIsZeroValue = ( map == 0 ) ? TRUE : FALSE ;

	if( map == 0 && pos_x == 0 && pos_z == 0 )
	{
		VECTOR3 pos;
		
		pPlayer->GetPosition( &pos );

		map = GAMERESRCMNGR->GetLoadMapNum();
		
		pos_x = (DWORD)pos.x;
		pos_z = (DWORD)pos.z;
	}
	else
	{
		/// Â¸Ã Â¹Ã¸ÃÂ£Â°Â¡ 0ÃÃ Â°Ã¦Â¿Ã¬ Â±ÃÃÂ¯ÃÃ¶Â·Ã ÂºÂ¸Â³Â»Â¹Ã¶Â¸Â®ÃÃ?
		if( map == 0 )
		{
			LOGINPOINT_INFO* ReturnInfo = NULL;

			ReturnInfo = GAMERESRCMNGR->GetLoginPointInfo( pPlayer->GetPlayerLoginPoint() );

			/// Â±ÃÃÂ¯ÃÃ¶Â°Â¡ Â¾Ã¸Â´Ã? Â±ÃÂ·Â³ Â¾ÃÃÃ¼ÃÃ¶Â´Ã«Â·Ã...
			if( !ReturnInfo )
			{
				VECTOR3* ppos;
				VECTOR3 pos;

				ppos = GAMERESRCMNGR->GetRevivePoint();

				int temp;
				temp = rand() % 500 - 250;
				pos.x = ppos->x + temp;
				temp = rand() % 500 - 250;
				pos.z = ppos->z + temp;
				pos.y = 0;		

				map = GAMERESRCMNGR->GetLoadMapNum();
				pos_x = ( DWORD )( pos.x );
				pos_z = ( DWORD )( pos.z );
			}
			else
			{
				VECTOR3 RandPos;

				int temp;
				temp = rand() % 500 - 250;
				RandPos.x = ReturnInfo->CurPoint[0].x + temp;
				temp = rand() % 500 - 250;
				RandPos.z = ReturnInfo->CurPoint[0].z + temp;
				RandPos.y = 0;

				map = ReturnInfo->MapNum;
				pos_x = ( DWORD )( RandPos.x );
				pos_z = ( DWORD )( RandPos.z );
			}
		}

		/// ÃÃÂµÂ¿ ÃÃÃÂ¥Â°Â¡ ÃÃÂ»Ã³ÃÃÂ´ÃÂ¸Ã©... Â¾ÃÃÃ¼ÃÃ¶Â´Ã«Â·Ã...
		if( pos_x == 0 || pos_x > 51200 || pos_z == 0 || pos_z > 51200 )
		{
			VECTOR3* ppos;
			VECTOR3 pos;

			ppos = GAMERESRCMNGR->GetRevivePoint();

			int temp;
			temp = rand() % 500 - 250;
			pos.x = ppos->x + temp;
			temp = rand() % 500 - 250;
			pos.z = ppos->z + temp;
			pos.y = 0;		

			map = GAMERESRCMNGR->GetLoadMapNum();
			pos_x = ( DWORD )( pos.x );
			pos_z = ( DWORD )( pos.z );
		}
	}

	if(g_pServerSystem->GetMapNum() == GTMAPNUM)
	{		
		VECTOR3* ppos = GAMERESRCMNGR->GetRevivePoint(GTRETURNMAPNUM);
		VECTOR3 pos = {0};
		int temp;
		temp = rand() % 500 - 250;
		pos.x = ppos->x + temp;
		temp = rand() % 500 - 250;
		pos.z = ppos->z + temp;
		
		CharacterLogoutPointUpdate(
			pPlayer->GetID(),
			GTRETURNMAPNUM,
			pos.x,
			pos.z);

		bUpdateLogPos = FALSE;
	}
	else if((SIEGEWARFAREMGR->IsSiegeWarfareZone(g_pServerSystem->GetMapNum(), FALSE) && 
				SIEGEWARFAREMGR->Is_CastleMap() == FALSE)
			|| SIEGEDUNGEONMGR->IsSiegeDungeon(g_pServerSystem->GetMapNum()))
	{
		const VillageWarp* const pVillageWarp = SIEGEWARFAREMGR->GetVilageInfo();

		// 100216 pdy Â°Ã¸Â¼ÂºÃÃ¶Â¿Âª ÃÃÂµÂ¿ÃÃÃÂ¥Â¹Â®ÃÂ¦ Â¼Ã¶ÃÂ¤
		if( map == pVillageWarp->MapNum || bMapIsZeroValue == TRUE || pPlayer->IsEmergency() )
		{
			if(pVillageWarp)
			{
				CharacterLogoutPointUpdate(
					pPlayer->GetID(),
					WORD(pVillageWarp->MapNum),
					pVillageWarp->PosX * 100.0f,
					pVillageWarp->PosZ * 100.0f);
			}

			bUpdateLogPos = FALSE;
		}
	}
	else if(g_csDateManager.IsChallengeZone(g_pServerSystem->GetMapNum()))
	{
		bUpdateLogPos = FALSE;
	}
	else if(g_pServerSystem->GetMapNum() == HOUSINGMAPNUM)
	{
		DWORD dwReserveChannel = HOUSINGMGR->GetChannelIDFromReservationList(pPlayer->GetID());
		if(dwReserveChannel == MAX_HOUSE_NUM+1)
		{
			HOUSINGMGR->ExitHouse(pPlayer->GetID(), FALSE);
		}
		else
		{
			// ÃÃÂ¿Ã¬ÃÂ¡Â¸ÃÂ¿Â¡Â¼Â­ Â´ÃÂ¸Â¥ ÃÃÂ¿Ã¬Â½Âº Â¹Ã¦Â¹Â®
			HOUSINGMGR->ExitHouse(pPlayer->GetID(), TRUE);
		}

		bUpdateLogPos = FALSE;
	}
	else if(DungeonMGR->IsDungeon(g_pServerSystem->GetMapNum()))
	{
		DungeonMGR->Exit(pPlayer->GetID());
	}
	
	if(PARTYMGR->CanUseInstantPartyMap(g_pServerSystem->GetMapNum()))
	{
		CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());
		if(pParty)
		{
			if(pParty->GetMasterID() == pPlayer->GetID())
					PARTYMGR->BreakupInstantParty(pParty->GetPartyIdx(), pPlayer->GetID());
			else
				PARTYMGR->DelMemberInstantParty(pPlayer->GetID(), pParty->GetPartyIdx());
		}
	}

	if(map == HOUSINGMAPNUM || DungeonMGR->IsDungeon(map) || g_csDateManager.IsChallengeZone(map))
	{
		bUpdateLogPos = FALSE;
	}

	if(bUpdateLogPos)
	{
		CharacterLogoutPointUpdate(
			pPlayer->GetID(),
			map,
			float(pos_x),
			float(pos_z));
	}

	// desc_hseos_ÂµÂ¥ÃÃÃÂ® ÃÂ¸_01
	// S ÂµÂ¥ÃÃÃÂ® ÃÂ¸ ÃÃÂ°Â¡ added by hseos 2007.11.29	2007.12.05
	// ..Â¸Ã ÃÃÃÂ»Â½Ã DBÃÃ ÃÂ§Â¸Â°ÃÃ¶ ÃÂ¸ ÃÃÃÃ¥ ÃÂ¸Â¼Ã¶ ÃÃÂ±Ã¢ÃÂ­Â°Â¡ ÃÃÂ¿Ã¤ÃÃ Â°Ã¦Â¿Ã¬.. CSHDateManager::SRV_ProcChallengeZoneEnterFreq ÃÃÂ¼Ã¶ÃÃ Â¼Â³Â¸Ã­ ÃÃ¼ÃÂ¶
	if (pPlayer->GetChallengeZoneNeedSaveEnterFreq())
	{
		ChallengeZone_EnterFreq_Save(pPlayer->GetID(), 0, pPlayer->GetChallengeZoneEnterBonusFreq());
		pPlayer->SetChallengeZoneNeedSaveEnterFreq(FALSE);
	}
	// ..ÃÂ§Â¸Â°ÃÃ¶ ÃÂ¸ Â³Â¡
	g_csDateManager.SRV_EndChallengeZone(pPlayer, CSHDateManager::CHALLENGEZONE_END_PARTNER_OUT);
	// E ÂµÂ¥ÃÃÃÂ® ÃÂ¸ ÃÃÂ°Â¡ added by hseos 2007.11.29	2007.12.05

	// 091231 ShinJS --- PCÂ¹Ã¦ ÂµÂ¥ÃÃÃÃ ÃÂ¦Â°Ã
	PCROOMMGR->RemovePlayer( dwPlayerID );

	// 080804 LUJ, Â°Â´ÃÂ¼ ÃÃÃÂ¦Â´Ã ÃÃÂ»Ã³ ÃÃÃÃÂ¿Â¡ Â¼Ã¶ÃÃ ÃÃÂ¾Ã ÃÃÂ´Ã. pPlayerÂ·Ã Â¼Ã¶ÃÃ ÃÃ ÃÃÂµÃ¥Â°Â¡ ÃÃÃÂ» Â°Ã¦Â¿Ã¬ ÃÃ ÃÃÂµÃ¥ Â¾ÃÂ¿Â¡ Â³ÃÂ¾Ã®Â¾Ã ÃÃÂ´Ã
	{
		g_pObjectFactory->ReleaseObject( pPlayer );

		if( bRemoveFromUserTable )
		{
			g_pUserTable->RemoveUser( dwPlayerID );
		}
	}
}


CPet* CServerSystem::AddPet(BASEOBJECT_INFO& pBaseObjectInfo, const PET_OBJECT_INFO& pPetObjectInfo, CPlayer* pPlayer)
{
	CPet* const previousPetObject = PETMGR->GetPet(
		pPlayer->GetPetItemDbIndex());

	if(previousPetObject)
	{
		previousPetObject->SetPetObjectState(
			ePetState_None);
		RemovePet(
			previousPetObject->GetID());
	}

	CPet* pPet = ( CPet* )g_pObjectFactory->MakeNewObject(
		eObjectKind_Pet,
		0,
		&pBaseObjectInfo);

	if(0 == pPet)
	{
		return 0;
	}

	pPet->SetGridID(
		pPlayer->GetGridID());
	pPet->SetOwnerIndex(
		pPlayer->GetID());
	pPet->SetObjectInfo(
		pPetObjectInfo);
	PETMGR->AddObject(
		pPetObjectInfo.ItemDBIdx,
		pPet->GetID());
	pPet->SetInited();

	g_pUserTable->AddUser(
		pPet,
		pPet->GetID());

	return pPet;
}

// 091214 ONS ÃÃª Â¸ÃÃÃÂµÂ¿Â½Ã Â¼ÃÃÂ¯/ÂºÃÃÃ Â°Ã¼Â·Ã Â¸ÃÂ¼Â¼ÃÃ¶Â¸Â¦ ÃÃ¢Â·ÃÃÃÃÃ¶ Â¾ÃÂµÂµÂ·Ã ÃÂ³Â¸Â®.
// Â¾ÃÃÃÃÃÂ¼ÃÃÂ¯/Â¸ÃÃÃÂµÂ¿ÃÂ» Â±Â¸ÂºÃÃÃÂ±Ã¢ ÃÂ§ÃÃ Â°ÂªÃÂ» ÃÂ¬Â¶Ã³ÃÃÂ¾Ã°ÃÂ®Â¿Â¡ ÃÃ¼Â´ÃÃÃÂ´Ã.
void CServerSystem::RemovePet( DWORD dwPetObjectID, BOOL bSummoned )
{
	CObject* pObject = g_pUserTable->FindUser(dwPetObjectID);
	
	if(!pObject)	return;

	CCharMove::ReleaseMove(pObject);
	CPet* pRemovePet =  ( CPet* )pObject;
	pRemovePet->DBInfoUpdate();
	PETMGR->RemoveObject(
		pRemovePet->GetObjectInfo().ItemDBIdx);
	PETMGR->Update(
		pRemovePet->GetObjectInfo());

	if(CObject* const ownerObject = g_pUserTable->FindUser(pRemovePet->GetOwnerIndex()))
	{
		MSG_DWORD_WORD  Msg;
		ZeroMemory( &Msg, sizeof(Msg) );
		Msg.Category = MP_USERCONN;
		Msg.Protocol = MP_USERCONN_HEROPET_REMOVE;
		Msg.dwData = pRemovePet->GetID();
		Msg.wData = WORD(bSummoned);

		ownerObject->SendMsg(
			&Msg,
			sizeof(Msg));
	}

	g_pObjectFactory->ReleaseObject(pObject);
	g_pUserTable->RemoveUser(dwPetObjectID);
}

void CServerSystem::RemoveMonster(DWORD dwMonster)
{
	CObject* pObject = g_pUserTable->FindUser(dwMonster);
	ASSERT(pObject);
	if(!pObject)	return;

	RemoveMonsterCount(*pObject);
	CCharMove::ReleaseMove(pObject);
	BATTLESYSTEM->DeleteObjectFromBattle(pObject);

	g_pAISystem.RemoveObject(dwMonster);
	g_pObjectFactory->ReleaseObject(pObject);
	g_pUserTable->RemoveUser(dwMonster);
}


void CServerSystem::RemoveBossMonster(DWORD dwMonster)
{
	CObject* pObject = g_pUserTable->FindUser(dwMonster);
	ASSERT(pObject);
	if(!pObject)	return;

	RemoveMonsterCount(*pObject);
	CCharMove::ReleaseMove(pObject);
	BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	g_pObjectFactory->ReleaseObject(pObject);

	g_pAISystem.RemoveObject(dwMonster);
	g_pUserTable->RemoveUser(dwMonster);


//CONDITIONMGR->RemoveListToPool(dwMonster);

}

void CServerSystem::RemoveNpc(DWORD dwNpcID)
{
	CObject* pObject = g_pUserTable->FindUser(dwNpcID);
	ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	
	if(!pObject)	return;

	g_pObjectFactory->ReleaseObject(pObject);
	g_pUserTable->RemoveUser(dwNpcID);
}

void CServerSystem::RemoveSkillObject(DWORD SkillObjectID)
{
	CObject* pObject = g_pUserTable->FindUser(SkillObjectID);
	//ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	g_pObjectFactory->ReleaseObject(pObject);
	g_pUserTable->RemoveUser(SkillObjectID);
}

void CServerSystem::RemoveMapObject( DWORD MapObjID )
{
	CObject* pObject = g_pUserTable->FindUser(MapObjID);
	ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	BATTLESYSTEM->DeleteObjectFromBattle(pObject);

	if(!pObject)	return;

	g_pObjectFactory->ReleaseObject(pObject);
	g_pUserTable->RemoveUser(MapObjID);
}


void CServerSystem::SendToOne(CObject* pObject,void* pMsg,int MsgLen)
{
	g_Network.Send2Server(pObject->GetAgentNum(),(char*)pMsg,MsgLen);
}

void CServerSystem::ReloadResourceData()	// case MP_CHEAT_RELOADING:
{
	SKILLMGR->Release();
	SKILLMGR->Init();
	GAMERESRCMNGR->LoadPlayerxMonsterPoint();
	ITEMMGR->ReloadItemList();

	g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while( (pObject = (CObject*)g_pUserTable->GetUserData()) != NULL )
	{
		if(pObject->GetObjectKind() == eObjectKind_Monster)
		{
			RemoveMonster(pObject->GetID());
		}
		else if(pObject->GetObjectKind() == eObjectKind_BossMonster)
		{
			RemoveBossMonster(pObject->GetID());
		}
		else if(pObject->GetObjectKind() == eObjectKind_Npc)
		{
			RemoveNpc(pObject->GetID());
		}
		
	}

	g_pAISystem.RemoveAllList();
	GAMERESRCMNGR->ResetMonsterList();
	GAMERESRCMNGR->LoadMonsterList();
	GAMERESRCMNGR->LoadMonsterRewardList();

	//SW050901
	MON_SPEECHMGR->LoadMonSpeechInfoList();
	g_pAISystem.LoadAIGroupList();

}

void CServerSystem::SetNation()
{
	CMHFile file;
	if( file.Init( "LocalizingInfo.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{

		if( strcmp( file.GetString(), "*NATION" ) == 0 )
		{
			if( strcmp( file.GetString(), "CHINA" ) == 0 )
			{
				m_Nation = eNATION_CHINA;
			}
		}

		file.Release();
	}

	CMHFile file2;
	if( file2.Init( "_TESTSERVER", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		m_bTestServer = TRUE;
		file2.Release();
	}
}

void CServerSystem::RemoveServerForKind( WORD wSrvKind )
{
	g_pServerTable->RemoveServerForKind(wSrvKind);
}


void CServerSystem::LoadHackCheck()
{
	CMHFile file;
	if( !file.Init( "ServerSet/HackCheck.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		g_Console.LOG(4, "HackCheckNum : %d, HackCheckWriteNum : %d", g_nHackCheckNum, g_nHackCheckWriteNum );
		return;
	}

	char temp[256] = {0, };
	while( 1 )
	{
		if( file.IsEOF() )	break;

		file.GetString( temp );
		if( strcmp( temp, "*HACKCHECKNUM" ) == 0 )
		{
			g_nHackCheckNum = file.GetInt();
		}
		else if( strcmp( temp, "*HACKCHECKWRITENUM" ) == 0 )
		{
			g_nHackCheckWriteNum = file.GetInt();
		}
		// 090205 LUJ, Â±Â¤Â¿Âª Â½ÂºÃÂ³ Â¿ÃÃÃ· Â¹Ã¼ÃÂ§
		else if( strcmp( temp, "*ALLOW_RANGE_FOR_WIDE_SKILL" ) == 0 )
		{
			cSkillObject::SetAllowRangeForWideSkill( file.GetFloat() );
		}
	}

	file.Release();

	g_Console.LOG(4, "HackCheckNum : %d, HackCheckWriteNum : %d", g_nHackCheckNum, g_nHackCheckWriteNum );
}

void CServerSystem::AddMonsterCount(CObject& object)
{
	if(IsUncountable(EObjectKind(object.GetObjectKind())))
	{
		return;
	}

	MonsterSet& monsterSet = mChannelMonsterMap[object.GetGridID()];
	monsterSet.insert(object.GetID());
}

void CServerSystem::RemoveMonsterCount(CObject& object)
{
	if(IsUncountable(EObjectKind(object.GetObjectKind())))
	{
		return;
	}

	MonsterSet& monsterSet = mChannelMonsterMap[object.GetGridID()];
	monsterSet.erase(object.GetID());
}

BOOL CServerSystem::IsUncountable(EObjectKind kind) const
{
	switch(kind)
	{
	case eObjectKind_Monster:
	case eObjectKind_BossMonster:
	case eObjectKind_SpecialMonster:
	case eObjectKind_FieldBossMonster:
	case eObjectKind_FieldSubMonster:
	case eObjectKind_ToghterPlayMonster:
	case eObjectKind_ChallengeZoneMonster:
		{
			return FALSE;
		}
	}

	return TRUE;
}

DWORD CServerSystem::GetMonsterCount(DWORD gridIndex) const
{
	const ChannelMonsterMap::const_iterator iter = mChannelMonsterMap.find(gridIndex);

	if(mChannelMonsterMap.end() == iter)
	{
		return 0;
	}

	const MonsterSet& monsterSet = iter->second;
	return monsterSet.size();
}

void CServerSystem::RemoveMonsterInGrid(DWORD gridIndex)
{
	const ChannelMonsterMap::const_iterator channelIterr = mChannelMonsterMap.find(gridIndex);

	if(mChannelMonsterMap.end() == channelIterr)
	{
		return;
	}

	const MonsterSet& monsterSet = channelIterr->second;

	for(MonsterSet::const_iterator monsterIter = monsterSet.begin();
		monsterSet.end() != monsterIter;
		++monsterIter)
	{
		const DWORD objectIndex = *monsterIter;
		CMonster* const monster = (CMonster*)g_pUserTable->FindUser(objectIndex);

		if(0 == monster)
		{
			continue;
		}

		const BOOL isNotMonster = !(monster->GetObjectKind() & eObjectKind_Monster);

		if(isNotMonster)
		{
			continue;
		}

		// 100204 LUJ, Â¸Ã³Â½ÂºÃÃÂ¸Â¦ Â°Â­ÃÂ¦Â·Ã Â»Ã¨ÃÂ¦ÃÃ Â¶Â§Â¿Â¡Â´Ã Â¸Â®ÃÂ¨ÂµÃÃÃ¶ Â¾ÃÂµÂµÂ·Ã Â±ÃÂ·Ã¬ Â¹Ã¸ÃÂ£Â¸Â¦ Â¹Â«ÃÂ¿ Â°ÂªÃÂ¸Â·Ã Â¼Â³ÃÂ¤ÃÃÂ´Ã
		MONSTER_TOTALINFO& totalInfo = monster->GetMonsterTotalInfo();
		totalInfo.Group = USHRT_MAX;
		monster->Die(0);
	}
}

void CServerSystem::ToggleCheckProcessTime()
{
	m_bCheckProcessTime = ! m_bCheckProcessTime;

	if(m_bCheckProcessTime)
	{
		m_fTimeObjLoopProc = 0.0f;
		m_fTimeTriggerProc = 0.0f;
		m_fTimeDungeonProc = 0.0f;

		m_fAvrObjLoopProc = 0.0f;
		m_fAvrTriggerProc = 0.0f;
		m_fAvrDungeonProc = 0.0f;
		m_dwProcessCount = 0;
	}
}

BOOL CServerSystem::IsNoRecallMap(CObject& object)
{
	if(GuildTournamentStadium == GetMapNum())
	{
		return TRUE;
	}
	else if(DungeonMGR->IsDungeon(GetMapNum()))
	{
		return TRUE;
	}
	else if(g_csDateManager.IsChallengeZone(GetMapNum()))
	{
		return TRUE;
	}

	const LimitDungeonScript& script = GAMERESRCMNGR->GetLimitDungeonScript(
		GetMapNum(),
		object.GetGridID());

	if(GetMapNum() == script.mMapType)
	{
		return TRUE;
	}

	return FALSE;
}

// 080813 LUJ, Â¼Ã¶ÃÂ¤ÂµÃ inetwork ÃÂ£ÃÃ¢ ÃÃ¼Â½ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ ÂºÂ¯Â°Ã¦
void __stdcall OnConnectServerSuccess(DWORD dwConnectionIndex, void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	info->dwConnectionIndex = dwConnectionIndex;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->NotifyBootUpToMS(&g_Network);
		g_Console.LOG(4, "Connected to the MS : %s, %d, (%d)", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}
	else
	{
		BOOTMNGR->SendConnectSynMsg(&g_Network, dwConnectionIndex, g_pServerTable);
		g_Console.LOG(4, "Connected to the Server : %s, %d, (%d)", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}

	if(g_pServerTable->GetMaxServerConnectionIndex() < dwConnectionIndex)
			g_pServerTable->SetMaxServerConnectionIndex(dwConnectionIndex);
}

// 080813 LUJ, Â¼Ã¶ÃÂ¤ÂµÃ inetwork ÃÂ£ÃÃ¢ ÃÃ¼Â½ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ ÂºÂ¯Â°Ã¦
void __stdcall OnConnectServerFail(void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->AddBootListINI(MAP_SERVER, GAMERESRCMNGR->GetLoadMapNum(), g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
		g_Console.LOG(4, "Failed to Connect to the MS : %s, %d", info->szIPForServer, info->wPortForServer);
	}
	else
	{
		g_Console.LOG(4, "Failed to Connect to the Server : %s, %d", info->szIPForServer, info->wPortForServer);
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}	
}

// 080813 LUJ, Â¼Ã¶ÃÂ¤ÂµÃ inetwork ÃÂ£ÃÃ¢ ÃÃ¼Â½ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ ÂºÂ¯Â°Ã¦
void __stdcall OnAcceptServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "Server Connected : ConnectionIndex %d", dwConnectionIndex);
}

// 080813 LUJ, Â¼Ã¶ÃÂ¤ÂµÃ inetwork ÃÂ£ÃÃ¢ ÃÃ¼Â½ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ ÂºÂ¯Â°Ã¦
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	if( g_pServerSystem->GetStart() == FALSE ) return;

	g_Console.LOG(4, "Server Disconnected : ConnectionIndex %d", dwConnectionIndex);

	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		ASSERT(0);
	}
	delete delInfo;


	// taiyo
	g_pUserTable->SetPositionUserHead();
	CObject * info = NULL;
	cPtrList list;

	while( (info = g_pUserTable->GetUserData()) != NULL )
	{
		if(info->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer * ply = (CPlayer *)info;
			if(ply->GetAgentNum() == dwConnectionIndex)
			{
				SaveMapChangePointUpdate(ply->GetID(), 0);
				//g_pServerSystem->RemovePlayer(ply->GetID());
				list.AddTail( ply );
			}
		}
	}
	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		CPlayer* p = (CPlayer*)list.GetNext(pos);
		g_pServerSystem->RemovePlayer(p->GetID());
	}
	list.RemoveAll();
}

// 080813 LUJ, Â¼Ã¶ÃÂ¤ÂµÃ inetwork ÃÂ£ÃÃ¢ ÃÃ¼Â½ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ ÂºÂ¯Â°Ã¦
void __stdcall OnAcceptUser(DWORD dwConnectionIndex)
{
}

// 080813 LUJ, Â¼Ã¶ÃÂ¤ÂµÃ inetwork ÃÂ£ÃÃ¢ ÃÃ¼Â½ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ ÂºÂ¯Â°Ã¦
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{

}

// 080813 LUJ, Â¼Ã¶ÃÂ¤ÂµÃ inetwork ÃÂ£ÃÃ¢ ÃÃ¼Â½ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ ÂºÂ¯Â°Ã¦
void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	ASSERT(pTempMsg->Category < MP_MAX);
	
	if( g_pServerMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	if (CSHDebug::GetExceptionProcGrade())
	{
		try
		{
			g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
		}
		catch(...)
		{
			char szTxt[256];
			sprintf(szTxt, "M:%02d  ReceivedMsgFromServer, %u, %u, %u, %u", g_pServerSystem->GetMapNum(), dwConnectionIndex, pTempMsg->Category, pTempMsg->Protocol, dwLength);
			CSHDebug::LogExceptionError(szTxt);
		}
	}
	else
	{
		g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
	}
}

// 080813 LUJ, Â¼Ã¶ÃÂ¤ÂµÃ inetwork ÃÂ£ÃÃ¢ ÃÃ¼Â½ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ ÂºÂ¯Â°Ã¦
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	ASSERT(pTempMsg->Category < MP_MAX);
	ASSERT(g_pUserMsgParser[pTempMsg->Category]);
	
	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	if (CSHDebug::GetExceptionProcGrade())
	{
		try
		{
			g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
		}
		catch(...)
		{
			char szTxt[256];
			sprintf(szTxt, "M:%02d  ReceivedMsgFromUser, %u, %u, %u, %u", g_pServerSystem->GetMapNum(), dwConnectionIndex, pTempMsg->Category, pTempMsg->Protocol, dwLength);
			CSHDebug::LogExceptionError(szTxt);
		}
	}
	else
	{
		g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
	}
}

void __stdcall ProcessServer(DWORD)
{
	g_DB.ProcessingDBMessage();
}

void __stdcall ProcessGame(DWORD)
{
	g_pServerSystem->Process();
}

void __stdcall ProcessCheck(DWORD)
{
	ConnectionCheck();
	NPCMOVEMGR->Process();
}

extern BOOL g_bWriteQuery;

BOOL LoadEventRate(char* strFileName)
{
	for(int i=0; i<eEvent_Max; ++i)	
	{
		gEventRate[i] = 1.f;
		gEventRateFile[i] = 1.f;
	}

	CMHFile file;
	// 080118 KTH -- ÃÃÂ¹Ã Â¸Ã°ÂµÃ¥Â·Ã ÃÃÃÃÃÂ» Â¿Â¬Â´Ã.
	if( !file.Init( strFileName, "r", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR) )
		return FALSE;

	char Token[256];
	
// RaMa -04.11.24
	while( !file.IsEOF() )
	{
		file.GetString(Token);

		if(strcmp(Token,"#EXP") == 0)	
		{			
			//gExpRate = file.GetFloat();
			gEventRateFile[eEvent_ExpRate] = file.GetFloat();
		}
		else if(strcmp(Token, "#ABIL") == 0)
		{
			//gAbilRate = file.GetFloat();
			gEventRateFile[eEvent_AbilRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#ITEM") == 0)
		{
			//gItemRate = file.GetFloat();
			gEventRateFile[eEvent_ItemRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#MONEY") == 0)
		{
			//gMoneyRate = file.GetFloat();
			gEventRateFile[eEvent_MoneyRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#GETMONEY") == 0)
		{
			//gGetMoney = file.GetFloat();
			gEventRateFile[eEvent_GetMoney] = file.GetFloat();
		}
		else if(strcmp(Token,"#DAMAGERECIVE") == 0)
		{
			//gDamageReciveRate = file.GetFloat();
			gEventRateFile[eEvent_DamageReciveRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#DAMAGERATE") == 0)
		{
			//gDamageRate = file.GetFloat();
			gEventRateFile[eEvent_DamageRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#MANASPEND") == 0)
		{
			//gNaeRuykRate = file.GetFloat();
			gEventRateFile[eEvent_ManaRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#UNGISPEED") == 0)
		{
			//gUngiSpeed = file.GetFloat();
			gEventRateFile[eEvent_UngiSpeed] = file.GetFloat();
		}
		else if(strcmp(Token,"#PARTYEXP") == 0)
		{
			//gPartyExpRate = file.GetFloat();
			gEventRateFile[eEvent_PartyExpRate] = file.GetFloat();
		}
		else if(strcmp(Token,"#SKILLEXPRATE") == 0)
		{
			gEventRateFile[eEvent_SkillExp] = file.GetFloat();
		}		
	}

	file.Release();

	for(int i=0; i<eEvent_Max; ++i)	
		gEventRate[i] = gEventRateFile[i];

	return TRUE;
}

DWORD CServerSystem::AddGameRoom(LPCTSTR address, LPCTSTR name, LPCTSTR upperIndex, LPCTSTR lowerIndex)
{
	GameRoom& gameRoom = mGameRoomContainer[GetGameRoomKey(address)];
	ZeroMemory(
		&gameRoom,
		sizeof(gameRoom));
	SafeStrCpy(
		gameRoom.mName,
		name,
		sizeof(gameRoom.mName) / sizeof(*gameRoom.mName));
	_stprintf(
		gameRoom.mDbIndex,
		"%s%s",
		upperIndex,
		lowerIndex);

	if(0 < _tcslen(gameRoom.mDbIndex))
	{
		gameRoom.mIndex = TRIGGERMGR->GetHashCode(gameRoom.mDbIndex);
	}

	// 100216 ShinJS --- Â´ÃÃÂ½ DB Load Â½ÃÂ°Â¢ ÃÃºÃÃ¥(1Â½ÃÂ°Â£ÃÃ)
	const DWORD dwCheckTime = 60*60*1000;
	gameRoom.mDBLoadTime = gCurTime + dwCheckTime;

	return gameRoom.mIndex;
}

ULONGLONG CServerSystem::GetGameRoomKey(LPCTSTR address) const
{
	TCHAR buffer[MAX_PATH] = {0};
	SafeStrCpy(
		buffer,
		address,
		sizeof(buffer) / sizeof(*buffer));
	ULONGLONG key = 0;
	LPCTSTR seperator = ".";

	// 091228 LUJ, Â¹Â®ÃÃÂ¿Â­ÃÂ» Â¼Ã¶ÃÂ¡Â·Ã ÂºÂ¯Â°Ã¦ÃÃÂ´Ã(Â¿Â¹: 192.168.1.130 -> 192168001130)
	for(LPCTSTR token = _tcstok(buffer, seperator);
		0 < token;
		token = _tcstok(0, seperator))
	{
		key = key * 1000;
		key = key + _ttoi(token);
	}

	return key;
}

void CServerSystem::UpdateFiniteStateMachine(CMonster& monster, DWORD subIndex) const
{
	CAIGroup* const aiGroup = GROUPMGR->GetGroup(
		monster.GetMonsterGroupNum(),
		monster.GetGridID());

	if(0 == aiGroup)
	{
		return;
	}

	const CAIGroup::Parameter* const parameter = aiGroup->GetRegenObject(
		subIndex);

	if(0 == parameter)
	{
		return;
	}
	else if(FALSE == monster.GetFiniteStateMachine().Initialize(
		parameter->mMachine,
		monster.GetID(),
		monster.GetGridID()))
	{
		return;
	}

	MONSTER_TOTALINFO& monsterTotalInfo = monster.GetMonsterTotalInfo();
	SafeStrCpy(
		monsterTotalInfo.mScriptName,
		parameter->mMachine,
		_countof(monsterTotalInfo.mScriptName));
}
void CServerSystem::StartRE(DWORD dwmapnum)
{
	printf("[ On Map Change %i -> %i ]\n",m_wMapNum,dwmapnum );
// RELEASING
	TRIGGERMGR->ReleaseME();
	SKILLMGR->ReleaseME();
	
	NPCRECALLMGR->ReleaseME();
	NPCMOVEMGR->ReleaseME();
	g_csDateManager.ReleaseME();
	DungeonMGR->ReleaseME();
	HOUSINGMGR->ReleaseME();
	FISHINGMGR->ReleaseME();
	GTMGR->ReleaseME();
	SIEGEWARFAREMGR->ReleaseME();
	m_Map.ReleaseME();
	//NPCRECALLMGR->ReleaseME();
	GAMERESRCMNGR->ReleaseME();
	
	g_pUserTable->SetPositionHead();
	CObject* pObject;
	while( (pObject = g_pUserTable->GetData()) != NULL )
	{
		//if(pObject->GetObjectKind() == eObjectKind_Player)
		//	g_pServerSystem->RemovePlayer(pObject->GetID(), FALSE);
		//	//FALSE: Â½ÃÃÂ¦Â·Ã UserTableÂ¿Â¡Â¼Â­Â´Ã ÃÃ¶Â¿Ã¬ÃÃ¶ Â¾ÃÂ´ÃÂ´Ã.
		//else
		//{
		//	CCharMove::ReleaseMove(pObject);
		//	g_pObjectFactory->ReleaseObject(pObject);
		//	if(pObject->GetObjectKind() == eObjectKind_Monster ||
		//		pObject->GetObjectKind() == eObjectKind_BossMonster )
		//	{
		//		g_pAISystem.RemoveObject(pObject->GetID());
		//	}
		//}
		switch(pObject->GetObjectKind()){
			case eObjectKind_Player: RemovePlayer(pObject->GetID(), FALSE);				break;
			case eObjectKind_Npc: RemoveNpc( pObject->GetID() ); break;
			//case eObjectKind_Item: RemoveItemObject( pObject->GetID() ); break;
			case eObjectKind_SkillObject: RemoveSkillObject( pObject->GetID()); break;
			case eObjectKind_Monster: 
			case eObjectKind_BossMonster:
			case eObjectKind_SpecialMonster:
			case eObjectKind_FieldBossMonster:
			case eObjectKind_FieldSubMonster:
			case eObjectKind_ToghterPlayMonster:
			case eObjectKind_ChallengeZoneMonster:
			case eObjectKind_Trap: 
			case eObjectKind_Vehicle:					RemoveMonster( pObject->GetID() ); break;
			case eObjectKind_Pet: {
				CPet* pPet = (CPet*)pObject;
				RemovePet( pObject->GetID(),pPet->GetSummonFlag() );				}
				break;
			case eObjectKind_MapObject: RemoveMapObject(pObject->GetID() ); break;
			default: 
				CCharMove::ReleaseMove(pObject);
				g_pObjectFactory->ReleaseObject(pObject);
				g_pUserTable->Remove(pObject->GetID()); 
				break;
		}
	}
	g_pAISystem.ReleaseME();
// CHANGING
	m_wMapNum = dwmapnum;
// RELOADING
	GAMERESRCMNGR->LoadStaticNpc(); // << Reload
	GAMERESRCMNGR->LoadHideNpcList();	// << Reload

	m_Map.InitME(m_wMapNum);// << Reload
	PATHMGR->SetMap(&m_Map, m_wMapNum, m_Map.GetTileWidth(m_wMapNum)); // << Reload
	PKMGR->Init(m_Map.IsPKAllow()); // << Reload

	SIEGEWARFAREMGR->InitME();//SIEGEWARFAREMGR->LoadMapInfo(); // << Reload
  //SIEGEWARFAREMGR->Init() ;
  SIEGERECALLMGR->Initialieze() ; // << Reload
	
	// Guild Tournament
	GTMGR->InitME();// GTMGR->Init(); // << Reload

	FISHINGMGR->InitME(); //FISHINGMGR->Init();//HOUSINGMGR->Init(); // << Reload
	HOUSINGMGR->InitME();//HOUSINGMGR->Init(); // << Reload
	DungeonMGR->InitME();//DungeonMGR->Init(); // << Reload

	// 090316 LUJ, Â»ÃµÂ·Ã Â½ÃÃÃÂµÃ Â¸ÃÂ¿Â¡Â¼Â­Â´Ã ÃÃÂµÂ¿ Â¼ÃÃÂ¯/ÃÂ¾Â½ÃÃÃÃÃ¶ Â¾ÃÂµÂµÂ·Ã ÃÃÂ±Ã¢ÃÂ­ÃÃÂ´Ã
	ResetVehicle( GetMapNum() ); // << Reload
	GuildResetWarehouse( m_wMapNum ); // << Reload
	// 100305 ONS Â¸ÃÂ±Ã¢ÂµÂ¿Â½Ã DBÃÃ PCÂ¹Ã¦ Â¹Ã¶ÃÃÃÂ¤ÂºÂ¸Â¸Â¦ ÃÃÂ±Ã¢ÃÂ­ÃÃÂ´Ã.
	PCROOMMGR->Init(); // << Reload
	{
		g_csFarmManager.InitME( GetMapNum() );
		int farmZoneId = 0;
		
		// 090520 LUJ, Â°Â¢Â°Â¢ÃÃ Â³Ã³ÃÃ¥ ÃÂ¤ÂºÂ¸Â¸Â¦ ÃÃÂ¾Ã®Â¿Ã Â¼Ã¶ ÃÃÂµÂµÂ·Ã Â¼Ã¶ÃÂ¤
		if( TRUE == g_csFarmManager.GetFarmZoneID( GetMapNum(), &farmZoneId ) )
		{
			CSHFarmZone* const farmZone = g_csFarmManager.GetFarmZone( WORD( farmZoneId ) );

			if( farmZone )
			{
				g_DB.FreeQuery(
					eFarm_LoadFarmState,
					farmZoneId,
					"EXEC dbo.MP_FARM_SELECT %d",
					farmZoneId );

				// 091126 pdy 0Â¹Ã¸ Â³Ã³ÃÃ¥ÃÃ 0Â¹Ã¸ ÃÃÂ¹Â°,Â°Â¡ÃÃ ÃÃ Â·ÃÂµÃ¥Â°Â¡ Â¾ÃÂµÃÂ´Ã Â¹Ã¶Â±Ã Â¼Ã¶ÃÂ¤   
				g_DB.FreeQuery(
					eFarm_LoadCropInfo,
					farmZoneId,
					"EXEC dbo.MP_FARM_CROP_SELECT %d, 0, -1",
					farmZoneId );

				g_DB.FreeQuery(
					eFarm_LoadAnimalInfo,
					farmZoneId,
					"EXEC dbo.MP_FARM_ANIMAL_SELECT %d, 0, -1",
					farmZoneId );
			}
		}
	}
	//g_csDateManager.LoadChallengeMonsterInfo(m_wMapNum);
	g_csDateManager.InitME(m_wMapNum);
	NPCMOVEMGR->InitME(); // NPCMOVEMGR->Init(); // << Reload
	NPCRECALLMGR->InitME();//NPCRECALLMGR->Initialize();
	g_pAISystem.InitME();
	TRIGGERMGR->InitME();
	printf("[ On Map Change END ]\n" );
}
