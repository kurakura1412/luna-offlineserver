#ifndef __COMMONGAMEDEFINE_H__
#define __COMMONGAMEDEFINE_H__


enum RaceType
{
	RaceType_Human,
	RaceType_Elf,
	// 090504 ONS ½Å±ÔÁ¾Á· Ãß°¡ÀÛ¾÷
	RaceType_Devil,

	RaceType_Max,

	RaceType_GM,
};

//for GMPower
enum eGM_POWER
{
	eGM_POWER_MASTER = 0,
	eGM_POWER_MONITOR,
	eGM_POWER_PATROLLER,
	eGM_POWER_AUDITOR,
	eGM_POWER_EVENTER,
	eGM_POWER_QA,
	eGM_POWER_MAX,
};

#define EXIT_COUNT	5
enum eEXITCODE
{
	eEXITCODE_OK,
	eEXITCODE_PKMODE,
	eEXITCODE_LOOTING,
	eEXITCODE_SPEEDHACK,
	eEXITCODE_DIEONEXIT,
	eEXITCODE_NOT_ALLOW_STATE,
};
//

#define SPEEDHACK_CHECKTIME	60000	//1¢®¡×¢®©­¢®¡×¢®E

//for PK
#define PKLOOTING_ITEM_NUM			20
#define PKLOOTING_DLG_DELAY_TIME	2000
#define PKLOOTING_LIMIT_TIME		10000

enum ePKCODE
{
	ePKCODE_OK,
	ePKCODE_ALREADYPKMODEON,
	ePKCODE_SHOWDOWN,
	ePKCODE_STATECONFLICT,
	ePKCODE_NOTALLAW,
};

enum eLOOTINGERROR
{
	eLOOTINGERROR_OK,
	eLOOTINGERROR_NOMORE_CHANCE,
	eLOOTINGERROR_NOMORE_ITEMLOOTCOUNT,
	eLOOTINGERROR_INVALID_POSITION,
	eLOOTINGERROR_ALREADY_SELECTED,
	eLOOTINGERROR_OVER_DISTANCE,
	eLOOTINGERROR_NO_LOOTINGROOM,
};

#define NPC_TALKING_DISTANCE 700
#define PK_LOOTING_DISTANCE	1000
#define eDROPITEMKIND_MAX 16

enum eUSERLEVEL
{
	eUSERLEVEL_GOD = 1,
	eUSERLEVEL_PROGRAMMER,
	eUSERLEVEL_DEVELOPER,
	eUSERLEVEL_GM,
	eUSERLEVEL_SUPERUSER,
	eUSERLEVEL_USER,
	eUSERLEVEL_SUBUSER,	
};

// 091125 ONS ¾ÆÀÌÅÛÃë±Þ ÃÖ´ë°¹¼ö 100 -> 1000À¸·Î ¼öÁ¤
#define MAX_ITEMBUY_NUM		1000
#define MAX_INVENTORY_MONEY	4000000000

enum eSTATUSICON
{
	eStatusIcon_None,
	eStatusIcon_QuestTime,
	eStatusIcon_Max,
};

enum eBOSSMONSTERKIND
{
	eBOSSKIND_TARINTUS = 261,
	eBOSSKIND_KIERRA = 267,
	eBOSSKIND_ARACH = 280,
	eBOSSKIND_LEOSTEIN = 721,
	eBOSSKIND_DRAGONIAN = 762,
};

#define RECALL_GROUP_ID			-1
#define EVENTRECALL_GROUP_ID	-2
#define QUESTRECALL_GROUP_ID	-3
// desc_hseos_µ¥ÀÌÆ® Á¸_01
// S µ¥ÀÌÆ® Á¸ Ãß°¡ added by hseos 2007.11.29
#define CHALLENGERECALL_GROUP_ID	-4
// E µ¥ÀÌÆ® Á¸ Ãß°¡ added by hseos 2007.11.29

#define ITEMRECALL_GROUP_ID	1001	// ¸ó½ºÅÍ¼ÒÈ¯ ¾ÆÀÌÅÛ »ç¿ë
#define DUNGEON_MONSTER_GROUP_ID	2001	// ÀÎ½ºÅÏ½º´øÀü º¸½º

// 100603 ONS Å»°Í ±×·ì¾ÆÀÌµð Ãß°¡
#define VEHICLE_RECALL_GROUP_ID	3001

#define eOPT_HIDEPARTS_NONE				0x00000000
#define eOPT_HIDEPARTS_EQUIP_BAND		0x00000001
#define eOPT_HIDEPARTS_EQUIP_GLASSES	0x00000002
#define eOPT_HIDEPARTS_EQUIP_WING		0x00000004
#define eOPT_HIDEPARTS_COSTUME_HEAD		0x00000008
#define eOPT_HIDEPARTS_COSTUME_DRESS	0x00000010
#define eOPT_HIDEPARTS_COSTUME_GLOVE	0x00000020
#define eOPT_HIDEPARTS_COSTUME_SHOES	0x00000040
#define eOPT_HIDEPARTS_ALL				( eOPT_HIDEPARTS_EQUIP_BAND			|\
											eOPT_HIDEPARTS_EQUIP_GLASSES	|\
											eOPT_HIDEPARTS_EQUIP_WING		|\
											eOPT_HIDEPARTS_COSTUME_HEAD		|\
											eOPT_HIDEPARTS_COSTUME_DRESS	|\
											eOPT_HIDEPARTS_COSTUME_GLOVE	|\
											eOPT_HIDEPARTS_COSTUME_SHOES )
#define MAXPLAYER_IN_CHANNEL	300

enum eBATTLE_KIND
{
	eBATTLE_KIND_NONE = 0,		// Player vs Monster ¢®¡×¡Ë¢ço¢®¡×uOAC ¡Ë¢çic¡§I¨Ï¨£ECO¢®EO¡Ë¢ç¢®¢¯AC Battle
	eBATTLE_KIND_SHOWDOWN,
	eBATTLE_KIND_GTOURNAMENT,
	eBATTLE_KIND_MAX,
};

enum GRID_TYPE
{
	eGridType_General,
	eGridType_Murim
};

enum Friend_ERROR_CODE
{
	//add friend
	eFriend_AddSuccess=0,
	eFriend_AddInvalidUser,
	eFriend_AddFullList,
	eFriend_AddDeny,
	eFriend_NotLogin,
	eFriend_AlreadyRegist,
//	eFriend_RegistSelf,
	//delete friend
	eFriend_DelInvaliedUser,
//----
	eFriend_OptionNoFriend,
};

enum {
LOGIN_SUCCESS = 0,
LOGIN_ERROR_INVALID_VERSION,
LOGIN_ERROR_OVERLAPPEDLOGIN,
LOGIN_ERROR_NOAGENTSERVER,
LOGIN_ERROR_NODISTRIBUTESERVER,
LOGIN_ERROR_INVALIDUSERLEVEL,
LOGIN_ERROR_WRONGIDPW,
LOGIN_ERROR_INVALIDDISTAUTHKEY,
LOGIN_ERROR_DISTSERVERISBUSY,
LOGIN_ERROR_AGENTSERVERISBUSY,
LOGIN_ERROR_AGENTMAXCONNECTION,
LOGIN_ERROR_BLOCKUSERLEVEL,
LOGIN_ERROR_INVALID_IP,	//ipcheck
LOGIN_ERROR_DISTCONNET_ERROR,
LOGIN_ERROR_MAXUSER,
LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER,
LOGIN_ERROR_MINOR_INADULTSERVER,
LOGIN_ERROR_NOT_CLOSEBETAWINNER,
LOGIN_ERROR_SECEDEDACCOUNT,
LOGIN_ERROR_NOREMAINTIME,
LOGIN_ERROR_NOIPREGEN,
LOGIN_ERROR_NONACCADMINIP,
// 080111 LYW --- CommonGameDefine : ·Î±×ÀÎ º¸¾È ºñ¹Ð¹øÈ£ °ü·Ã ¿¡·¯ Ã³¸® Ãß°¡.
LOGIN_ERROR_INVALID_SEC_PASS,				// º¸¾È ºñ¹Ð¹øÈ£ ÀÎÁõ¿¡ ½ÇÆÐÇß´Ù´Â ¿¡·¯¸Þ½ÃÁö.
LOGIN_ERROR_CHANGE_SEC_PASS,				// Àß¸øµÈ º¸¾È ºñºô¹øÈ£, È¨ÆäÀÌÁö¿¡¼­ º¸¾È ºñ¹Ð¹øÈ£¸¦ Àç µî·Ï ÇÏ½Ã¿À.
LOGIN_ERROR_FILE_CRC,
LOGIN_ERROR_LOGINSESSION_INVALID,
};

#define MHSHADOW_OFF	0
#define MHSHADOW_CIRCLE	1
#define MHSHADOW_DETAIL	2
// E¢®E¢®©­¢®¡×¢®©­¡§Io ¢®¡×oA¡Ë¢ç¡§¢®¡Ë¢çI
#define RECOVERTIMEUNIT					3000
// E¢®E¢®©­¢®¡×¢®©­¡§Io ¡Ë¢ç¡§¢®¡Ë¢çI¡Ë¢ç¡§¢®Y
#define RECOVERINTERVALUNIT				10

// A¢®E¢®©­AI ¡Ë¢çicAIAi(CN A¢®E¢®©­AIAC A¢®¡×I¡Ë¢ç¡§ua)
#define fTILE_SIZE 50.f

#define SKILLAREA_ATTR_DAMAGE			0x00000001
#define SKILLAREA_ATTR_BLOCK			0x00000002
#define SKILLAREA_ATTR_SAFETYZONE		0x00000003
#define SKILLAREA_ATTR_OTHEREFFECT		0x00000004

typedef BYTE AREATILE;
#define GetAreaTile	GetByte

#define MAX_ITEM_NUM 3000	//confirm
#define MAX_ITEM_OPTION_NUM	90

// INDEXGENERATER ID START INDEX
#define MAX_ITEMICON_NUM	999

// 080203 LYW --- Çàµ¿ °ü·Ã ¾ÆÀÌÄÜ ÃÖ´ë °³¼ö Á¤ÀÇ.
#define MAX_CONDUCTICON_NUM	999

#define IG_ITEM_STARTINDEX				10000
#define IG_MSGBOX_STRARTINDEX			12000
#define IG_STREETSTALLITEM_STARTINDEX	13000	//LBS 03.09.19
#define IG_EXCHANGEITEM_STARTINDEX		14000	//KES EXCHANGE 030920
#define IG_MENUSLOTITEM_STARTINDEX		15000	//KES 040510	//Ao¡Ë¢ç¡§uY ¢®¡×uE¢®¡×u¢®E¢®I¢®E¢®IU. Ao¢®E?i¡Ë¢ç¡§ui.
#define IG_PKLOOTINGDLG_START			16000	//KES PKLOOTING 040720
#define IG_DEBUG_START					20000	//KES DEBUG 040720
#define IG_QUESTITEM_STARTINDEX			25000	//RaMa
#define IG_SHOPITEM_STARTINDEX			28000	//RaMa 04.11.17
#define IG_DEALITEM_START				30000
#define IG_MAINBARICON_START			40000
//#define IG_ABILITYICON_START			31000
//#define IG_ABILITYPOINT_START			40000	//RaMa Æ¯±âÄ¡ ÀçºÐ¹è
#define IG_QUICKITEM_STARTINDEX			60000
// 070216 LYW --- CommonGameDefine : Define start index of conduct part.
#define IG_CONDUCT_STARTINDEX			70000

// 070608 LYW --- CommonGameDefine : Define start index of matching favor icon part.
#define IG_FAVOR_STARTINDEX				150000
#define IG_IDENTIFICATION_STARTINDEX	151000
#define IG_MATCH_MYINFO_STARTINDEX		152000
#define IG_MATCH_PARTNERINFO_STARTINDEX	153000
#define IG_MATCH_ENDINDEX				154000

//090423 pdy ÇÏ¿ìÂ¡ ¾ÆÀÌÄÜ ÀÎµ¦½º 
#define IG_HS_STOREDICON_START 155000
#define HOUSING_FURNITURE_STARTINDEX	(UINT_MAX - 2000000)
#define MONSTER_INDEX_SIZE				200000
#define MONSTER_STARTINDEX				HOUSING_FURNITURE_STARTINDEX - MONSTER_INDEX_SIZE
#define VEHICLE_INSTALL_ID				MONSTER_STARTINDEX - 1

enum NPC_ROLE
{
	OBJECT_ROLE = 0,
	DEALER_ROLE = 1,
	MUNPA_ROLE = 3,
	CHANGGO_ROLE = 4,
	FYOKUK_ROLE = 5,
	TALKER_ROLE = 6,
	MAP_MOVE_ROLE = 8,
	FAMILY_ROLE = 15,
 	// 080901 LYW --- CommonGameDefine :
	CANDLESTAND_ROLE = 20,
	// 080821 LYW --- CommonGameDefine : ¸Ê ÀÌµ¿À» ÇÏ´Âµ¥, È®ÀÎ ¸Þ½ÃÁö ¹Ú½º¸¦ ¶ç¿ì´Â Å¸ÀÔÀÌ´Ù.(°ø¼ºÀü¿¡¼­ »ç¿ë)
	PORTAL_ROLE = 21,
	// 080901 LYW --- CommonGameDefine : 
	SIEGEWELL_ROLE = 22,
	BOMUL_ROLE	= 23,
	// 080901 LYW --- CommonGameDeinfe : ¹öÇÁ NPC Á÷¾÷ Ãß°¡.
	BUFFNPC_ROLE = 24,
	// 080903 LYW --- CommonGameDefine : °ø¼º ±ê¹ß Ãß°¡.
	SIEGEFLAGROLE_RUSHEN = 25,
	SIEGEFLAGROLE_ZEVYN = 26,
	MAPCHANGE_ROLE = 27,	
	FISHINGPLACE_ROLE = 28,
	CAMPFIRE_ROLE = 29,
	// 070605 LYW --- CommonGameDefine : Add identification role.
	IDENTIFICATION_ROLE = 38,

	// 081125 LYW --- CommonGameDefine : ¸®¹ÌÆ® ´øÀü ÀÔÀå Àü¿ë Á÷¾÷ Ãß°¡.
	LIMITDUNGEON_ROLE	= 40,

	//090522 pdy  --- CommonGameDefine : ÇÏ¿ìÂ¡ NPC Á÷¾÷ Ãß°¡.
	HOUSING_ROLE		= 50,
	HOUSING_RANK1_ROLE	= 51,
	HOUSING_RANK2_ROLE	= 52,
	HOUSING_RANK3_ROLE	= 53,
	HOUSING_PORTAL_ROLE	= 54,
	DUNGEON_WARP_ROLE	= 55,
	DUNGEON_SWITCH_ROLE = 56,
	
	NOTE_ROLE			= 60,	// ¿ìÆíÇÔ
	CONSIGNMENT_ROLE	= 61,	// ÆÇ¸Å´ëÇà
};


enum eMoveMode
{
	eMoveMode_Run = 0,
	eMoveMode_Walk = 1,
};

#define MAX_PACKEDDATA_SIZE 60000 // for SEND_PACKED_DATA

enum eMONSTER_ACTION
{
	eMA_STAND,
	eMA_WALKAROUND,
	eMA_PERSUIT,
	eMA_RUNAWAY,
	eMA_WALKAWAY,
	eMA_ATTACK,
	eMA_PAUSE,
	eMA_SCRIPT_RUN,
	eMA_SCRIPT_WALK,
};

enum LIMIT_RACE
{
	eLimitRace_Common = 0,
	eLimitRace_HumanElf,
	eLimitRace_Devil,
};

enum ITEM_KIND
{
	ITEM_KIND_HP_RECOVER		= 1,	// »ý¸í·Â È¸º¹Á¦
	ITEM_KIND_MP_RECOVER		= 2,	// ±â·Â È¸º¹Á¦
//---KES 
	ITEM_KIND_HP_RECOVER_INSTANT	= 3,	// »ý¸í·Â È¸º¹Á¦
	ITEM_KIND_MP_RECOVER_INSTANT	= 4,	// ±â·Â È¸º¹Á¦
	ITEM_KIND_HPMP_RECOVER			= 5,	// »ý¸í±â·Â µ¿½Ã È¸º¹Á¦
	ITEM_KIND_HPMP_RECOVER_INSTANT	= 6,	// »ý¸í±â·Â µ¿½Ã È¸º¹Á¦
//-----
	ITEM_KIND_FARM_PLANT		= 7,	// ³óÀåÀÇ ³óÀÛ¹° ½É±â
	ITEM_KIND_FARM_MANURE		= 8,	// ³óÀåÀÇ ³óÀÛ¹° ºñ·á ÁÖ±â
	ITEM_KIND_BUFF_ITEMS		= 11,	// ¹öÇÁÀÇ ±â´ÉµéÀ» »ç¿ëÇÑ À¯·á ¾ÆÀÌÅÛµé.
	ITEM_KIND_FARM_HARVEST		= 12,	// ³óÀåÀÇ ³óÀÛ¹° ¼öÈ®
	ITEM_KIND_COOLTIME			= 13,	// 080820 LUJ, È¹µæ ½Ã¿¡ ÄðÅ¸ÀÓÀ» °®°í ÀÖ´Â ¾ÆÀÌÅÛ

	ITEM_KIND_CHANGESIZE_UPWEIGHT	= 20,	// Ä³¸¯ÅÍ Å°º¯°æ - Ä¿ÁúÈ®·ü °¡ÁßÄ¡
	ITEM_KIND_CHANGESIZE_DNWEIGHT	= 21,	// Ä³¸¯ÅÍ Å°º¯°æ - ÀÛ¾ÆÁúÈ®·ü °¡ÁßÄ¡
	ITEM_KIND_CHANGESIZE_DEFALUT	= 22,	// Ä³¸¯ÅÍ Å°º¯°æ - ¿ø·¡´ë·Î

	ITEM_KIND_ADDRECIPE			= 23,	// ¿ä¸® - ·¹½ÃÇÇ¹è¿ì±â
	//ITEM_KIND_DELRECIPE			= 24,	// ¿ä¸® - ·¹½ÃÇÇÁö¿ì±â
	ITEM_KIND_FOODSKILLBOOK		= 25,	// ¿ä¸® - ½ºÅ³ºÏ
	ITEM_KIND_FOOD				= 26,	// ¿ä¸® - ¿ä¸®

	//090527 pdy ÇÏ¿ìÂ¡ 
	ITEM_KIND_FURNITURE						= 30,
	ITEM_KIND_FURNITURE_WALLPAPER			= 31,
	ITEM_KIND_FURNITURE_FLOORPAPER			= 32,
	ITEM_KIND_FURNITURE_CEILINGPAPER		= 33,
	ITEM_KIND_VISIT_HOUSE					= 34,	// ÇÏ¿ì½º¹æ¹®
	ITEM_KIND_VISIT_HOME					= 35,	// myÇÏ¿ì½ºÀÔÀå
	ITEM_KIND_EXTEND_HOUSE					= 36,	// ÇÏ¿ì½º È®Àå

	ITEM_KIND_CHANGE_CLASS		= 49,	// 100511 ONS ÀüÁ÷ÃÊ±âÈ­ ¾ÆÀÌÅÛ	
	ITEM_KIND_RESET_STATS		= 50,	// ½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ
	ITEM_KIND_RESET_SKILL		= 51,	// ½ºÅ³ ÃÊ±âÈ­ ¾ÆÀÌÅÛ

	// 080611 LYW --- CommonGameDefine : ¾ÆÀÌÅÛ Å¸ÀÔ Ãß°¡.
	ITEM_KIND_GET_SKILLPOINT	= 52,	// ½ºÅ³ Æ÷ÀÎÆ® È¹µæ.

	ITEM_KIND_PROTECT_EXP		= 53,	// °æÇèÄ¡ º¸È£ ¾ÆÀÌÅÛ
	ITEM_KIND_RECALL_MOVE		= 54,	// ±â¾ïÀÌµ¿ ÁÖ¹®¼­
	ITEM_KIND_EXTEND_INVEN		= 55,	// ÀÎº¥Åä¸® È®Àå ¾ÆÀÌÅÛ
	ITEM_KIND_EXTEND_STORAGE	= 56,	// Ã¢°í È®Àå ¾ÆÀÌÅÛ
	ITEM_KIND_MEMORY_STONE		= 57,	// ±â¾ï¼®

	// 080607 LYW --- CommonGameDefine : ¾ÆÀÌÅÛ Å¸ÀÔ Ãß°¡.
	ITEM_KIND_OPENSTORAGE_BYITEM = 58,	// Ã¢°í ¼ÒÈ¯ ¾ÆÀÌÅÛ.

	ITEM_KIND_CHANGE_NAME		= 66,	// ÀÌ¸§ º¯°æ ¾ÆÀÌÅÛ
	ITEM_KIND_MAP_MOVE_SCROLL	= 67,	// 071201 ¸ÊÀÌµ¿ ÁÖ¹®¼­
	ITEM_KIND_GTBUFF_ITEMS		= 76,		// ±æµåÅä³Ê¸ÕÆ® Àü¿ë ¹öÇÁ¾ÆÀÌÅÛ (Å¸°Ù´ë»ó¿¡°Ô¸¸ ¾µ ¼ö ÀÖ´Ù)
	ITEM_KIND_JOIN_EVENT		= 77,	// 080204 KTH -- ÀÌº¥Æ® ÀÀ¸ð °ü·Ã
	ITEM_KIND_GTHP_RECOVER_INSTANT	= 78,	// ±æµåÅä³Ê¸ÕÆ® Àü¿ë HPÆ÷¼Ç
	ITEM_KIND_GTMP_RECOVER_INSTANT	= 79,	// ±æµåÅä³Ê¸ÕÆ® Àü¿ë MPÆ÷¼Ç
	ITEM_KIND_WORLDSHOUT		= 80,	// 080312 NYJ -- ÀüÃ¼¿ÜÄ¡±â(¿ùµåÈ®¼º±â) ¾ÆÀÌÅÛ
	ITEM_KIND_FISHING_BAIT		= 81,	// 080410 NYJ -- ³¬½Ã ¹Ì³¢¾ÆÀÌÅÛ
	ITEM_KIND_FISHING_FISH		= 82,	// 080410 NYJ -- ³¬½Ã ¹°°í±â
	ITEM_KIND_RESET_REGIDENT_REGIST	= 88,// 080215 KTH -- ÁÖ¹Îµî·Ï ÃÊ±âÈ­ ¾ÆÀÌÅÛ
	ITEM_KIND_RETURN_SCROLL		= 99,	// ±ÍÈ¯ ÁÖ¹®¼­

 	// 080826 LYW --- CommonGameDefine : ¿öÅÍ½Ãµå ¾ÆÀÌÅÛ Å¸ÀÔ Ãß°¡.
	ITEM_KIND_SIEGEWARFARE_WATERSEED = 100,

	ITEM_KIND_CHALLENGEZONE_ENTER_FREQ	= 200,			// Ã§¸°Áö Á¸ ÀÔÀå È½¼ö Áõ°¡
	ITEM_KIND_ANIMAL			= 500,	// 080415 KTH -- °¡Ãà
	ITEM_KIND_ANIMAL_FOOD		= 501,	// 080507 KTH -- ¸ÔÀÌ
	ITEM_KIND_ANIMAL_CLEANER	= 502,	// 080514 KTH -- Ãà»ç Ã»¼Ò ¼¼Àç
	ITEM_KIND_PET							= 600,
	ITEM_KIND_PET_SKILL						= 601,
	ITEM_KIND_PET_COSTUME					= 602,
	ITEM_KIND_PET_EQUIP						= 603,

	ITEM_KIND_PET_EGG						= 700,
	ITEM_KIND_PET_FOOD						= 701,
	ITEM_KIND_PET_NORMAL_RESURRECTION		= 702,
	ITEM_KIND_PET_SPECIAL_RESURRECTION		= 703,
	ITEM_KIND_PET_HP_RECOVER				= 704,
	ITEM_KIND_PET_MP_RECOVER				= 705,
	ITEM_KIND_PET_EVOLUTION_PYHSIC_TYPE		= 706,
	ITEM_KIND_PET_EVOLUTION_MAGIC_TYPE		= 707,
	ITEM_KIND_PET_EVOLUTION_SUPPORT_TYPE	= 708,
	ITEM_KIND_SUMMON_NPC					= 800,
	ITEM_KIND_SUMMON_MONSTER				= 801,
	ITEM_KIND_SUMMON_EFFECT					= 802,
	ITEM_KIND_SUMMON_VEHICLE				= 803,
	ITEM_KIND_DUNGEON_KEY					= 900,
	ITEM_KIND_LEVEL_UP						= 910,
	ITEM_KIND_GUILDPOINT_UP					= 911,
	ITEM_KIND_FAMILYPOINT_UP				= 912,
	ITEM_KIND_EXP_UP						= 913,
	ITEM_KIND_PCROOMPOINT_UP				= 914,
	ITEM_KIND_FISHINGEXP_UP					= 915,
	ITEM_KIND_FISHPOINT_UP					= 916,
	ITEM_KIND_COOKLEVEL_UP					= 917,
	ITEM_KIND_DECOUSEPOINT_UP				= 918,
	ITEM_KIND_PKPOINT_UP					= 919,
	ITEM_KIND_PKPOINT_DN					= 920,
// --- skr : warehouse
	ITEM_KIND_EXTEND_STORAGE_SET1 = 921,
	ITEM_KIND_EXTEND_STORAGE_SET2 = 922,
	ITEM_KIND_EXTEND_STORAGE_SET3 = 923,
	ITEM_KIND_EXTEND_STORAGE_SET4 = 924,
};
// E ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.05.14

/// ¾ÆÀÌÅÛ ±¸ºÐ ÀçÁ¤ÀÇ
enum eItemCategory
{
	eItemCategory_Etc,
	eItemCategory_Equip,
	eItemCategory_Expend,
	eItemCategory_Material,
	eItemCategory_Quest,
	eItemCategory_Max,
};

enum eEquipType
{
	eEquipType_None,
	eEquipType_Weapon,
	eEquipType_Armor,
	eEquipType_Accessary,
	eEquipType_Pet,
	eEquipType_Max,
};

enum ePetEquipSlot
{
	ePetEquipSlot_Helmet,
	ePetEquipSlot_Armor,
	ePetEquipSlot_Glove,
	ePetEquipSlot_Accessaries,
	ePetEquipSlot_Weapon,
	
	ePetEquipSlot_Face,
	ePetEquipSlot_Head,
	ePetEquipSlot_Back,
	ePetEquipSlot_Tail,
	
	ePetEquipSlot_Skill,

	ePetEquipSlot_Skill1 = ePetEquipSlot_Skill,
	ePetEquipSlot_Skill2,
	ePetEquipSlot_Skill3,
	ePetEquipSlot_Skill4,
	ePetEquipSlot_Skill5,
	ePetEquipSlot_Skill6,


	ePetEquipSlot_Max,
};

enum eWeaponAnimationType
{
	eWeaponAnimationType_None,
	eWeaponAnimationType_OneHand,
	eWeaponAnimationType_TwoHand,
	eWeaponAnimationType_Dagger,
	eWeaponAnimationType_Staff,
	eWeaponAnimationType_Bow,	
	eWeaponAnimationType_Gun,
	eWeaponAnimationType_TwoBlade,
	// 090528 ONS ¸¶Á· ¾ç¼Õ¹«±â ¼Ó¼º Ãß°¡
	eWeaponAnimationType_BothDagger,
	// 090204 LUJ, ¼Ó¼º Ãß°¡
	eWeaponAnimationType_Spear,
	eWeaponAnimationType_Max,
};

enum eWeaponType
{
	eWeaponType_None,
	eWeaponType_Sword,
	eWeaponType_Mace,
	eWeaponType_Axe,
	eWeaponType_Dagger,
	eWeaponType_Staff,
	eWeaponType_Bow,
	eWeaponType_Gun,
	eWeaponType_FishingPole,
	eWeaponType_CookUtil,
	// 090528 ONS ½Å±ÔÁ¾Á· ¹«±â Å¸ÀÔ ¼Ó¼º Ãß°¡
	eWeaponType_BothDagger,
	eWeaponType_Max,
};

enum eArmorType
{
	eArmorType_None,
	eArmorType_Robe,
	eArmorType_Leather,
	eArmorType_Metal,
	// 090819 ShinJS --- Á¶ÇÕ ÀÎÅÍÆäÀÌ½º ¼öÁ¤, ¹æÆÐ/Çï¸ä ±¸ºÐÀ» À§ÇØ Armor Type Ãß°¡
	eArmorType_Helmet,
	eArmorType_Shield,
	eArmorType_Max,
};

enum eAccessaryType
{
	eAccessaryType_None,
	eAccessaryType_Ring,
	// 090820 ShinJS --- Á¶ÇÕ ÀÎÅÍÆäÀÌ½º ¼öÁ¤, ¸ñ°ÉÀÌ¿Í ±Í°ÉÀÌ°¡ ¾ÆÀÌÅÛ¸®½ºÆ®ÀÇ °ª°ú ¼­·Î ¹Ù²î¾î ÀÖ¾î ¼öÁ¤
	eAccessaryType_Necklace,
	eAccessaryType_Earring,
	eAccessaryType_Belt,
	eAccessaryType_Band,
	eAccessaryType_Max,
};

// 100104 ShinJS --- ¾ÆÀÌÅÛ Point Type
enum eItemPointType
{
	eItemPointType_None,
	eItemPointType_Item,
	eItemPointType_PCRoomPoint,
	eItemPointType_GuildPoint,
	eItemPointType_FamilyPoint,
	eItemPointType_Exp,
	eItemPointType_PKPoint,
	eItemPointType_FishPoint,
};

#define PET_EQUIP_SLOT_START 70
#define MAX_JOB	3
#define MAX_NPC_NUM 200
#define	MAX_MOVENPC_NUM	10						// 090227 ShinJS --- ÀÌµ¿NPC ÃÖ´ë °³¼ö 
#define MAX_DROPITEM_NUM 20
#define MAX_CHARACTER_LEVEL_NUM		150			// A¡§I¨Ï¨£¢®E¢®©­?AI ¡Ë¢ç¡Ë¢¥¡§Ii¨Ïo¢®Ei¡§u¡Ë¢ç¢®¢¯ ¢®¡×uo

#define MAX_PLAYERLEVEL_NUM	151
#define MAX_MONSTERLEVELPOINTRESTRICT_NUM	9
#define MONSTERLEVELRESTRICT_LOWSTARTNUM	6

#define MAX_COMBO_NUM		3
#define SKILL_COMBO_NUM		100
#define SKILLRESULTKIND_NONE 0

enum ReviveFlag
{
	ReviveFlagNone,
	ReviveFlagHere,
	ReviveFlagTown,
};

enum eItemUse_Err
{
	eItemUseSuccess,
	eItemUseSuccess_ResetRegidentCard,
	eItemUseSuccess_ChangeItem,
	// E Ã¼ÀÎÁö¾ÆÀÌÅÛ Ãß°¡ added by hseos 2007.08.29

	eItemUseErr_PreInsert,
	eItemUseErr_Move,
	eItemUseErr_Combine,
	eItemUseErr_Divide,
	eItemUseErr_Discard,
	eItemUseErr_Quest,
	eItemUseErr_Err,
	eItemUseErr_Upgrade,
	eItemUseErr_Lock,
	eItemUseErr_AlreadyUse,
	eItemUseErr_Unabletime,
	eItemUseErr_AlreadyPoint,
	eItemUseErr_AlredyChange,
	eItemuseErr_DontUseToday,
	eItemUseErr_DontDupUse,
	eItemUseErr_UseFull,
	// desc_hseos_³óÀå½Ã½ºÅÛ_01
	// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.08.29
	eItemUseErr_FullInven,
	eItemUseErr_WithNoneMsg,
	// E ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.08.29

	eItemUseUnsealed,	//080317 NYJ ºÀÀÎÇØÁ¦
	eItemUseErr_NoSummonInfo,

 	// 080607 LYW --- CommonGameDefine : Ã¢°í ¼ÒÈ¯ ¾ÆÀÌÅÛ »ç¿ë ½ÇÆÐ ¿¡·¯ Ãß°¡.
	eItemUse_OpenStorage_byItem_Failed_Storage_IsZero,
	// 080609 LYW --- CommonGameDefine : Ã¢°í ¼ÒÈ¯ ¾ÆÀÌÅÛ »ç¿ë ¼º°ø Å¸ÀÔ Ãß°¡.
	eItemUse_OpenStorage_Success,
	// 080902 LYW --- CommonGameDefine : 
	eItemUseErr_WaterseedErr,

	// 080904 LYW --- CommonGameDefine : °ø¼º Áß »ç¿ëÇÒ ¼ö ¾ø´Â ¾ÆÀÌÅÛ ¸Þ½ÃÁö Ãâ·Â.
	eItemUseErr_SiegeWarfareForbiddenItem,

	eItemUseErr_ForbiddenSummon,	// 081027 NYJ ¼ÒÈ¯±ÝÁö

	eItemUseErr_InvalidCookGrade,
	eItemUseErr_LowerLevelForEat,
	eItemUseErr_AlreadyLearnRecipe,
	eItemUseErr_FullLearnRecipe,

	eItemUseErr_HaveNoHouse,

	// 100218 ONS ÁÖ¹Îµî·Ï ÃÊ±âÈ­ ¾ÆÀÌÅÛ »ç¿ë ½ÇÆÐ ¿¡·¯ Ãß°¡
	eItemUseErr_NotRegist,
	eItemUseErr_PlayerIsGod,
	eItemUseErr_SkillCanNotStart,
	eltemUseErr_Stun,
};

/// GameResource¢®E?¢®E¡Ëc ¡Ë¢çic¢®E?e¢®IiC¢®E¢®IA define ///////////////////////////////////////////////
// 091125 ONS ¾ÆÀÌÅÛÃë±Þ ÃÖ´ë°¹¼ö 100 -> 1000À¸·Î ¼öÁ¤
#define MAX_YOUNGYAKITEM_DUPNUM			1000
// 061211 LYW --- Adjust max character count.
//#define MAX_CHARACTER_NUM	4
#define MAX_CHARACTER_NUM	5
#define MAX_STREETSTALL_TITLELEN 66

#define MAX_FILE_NAME		32
enum { STR_POINT, WIS_POINT, DEX_POINT, VIT_POINT, INT_POINT, MAX_BTN_POINT};

#define GENDER_MALE 0
#define GENDER_FEMALE 1
#define GENDER_MAX 2

#define MAX_PARTY_LISTNUM 7
#define MAX_PARTY_LOAD_NUM 100

// 070105 LYW --- Define max count for family member.
#define MAX_FAMILY_LISTNUM 5
#define MAX_BASESTORAGELIST_NUM 2
#define MAX_STORAGELIST_NUM 5
// --- skr : warehouse
#define MAX_STORAGELIST_NUMSET1 10
#define MAX_STORAGELIST_NUMSET2 15
#define MAX_STORAGELIST_NUMSET3 20
#define MAX_STORAGELIST_NUMSET4 25
#define MAX_STORAGELIST_MAXSET 5

#define MAX_FRIEND_NUM 30
#define MAX_FRIEND_LIST 10
#define MAX_FRIEND_PAGE MAX_FRIEND_NUM/MAX_FRIEND_LIST // 30/10 = 10

// 100118 ONS Ä£±¸°ü¸® ´ÙÀÌ¾ó·Î±×¿¡ PC¹æÁ¤º¸°ü·Ã ±â´ÉÃß°¡
#define MAX_PCROOM_MEMBER_NUM 30
#define MAX_PCROOM_MEMBER_LIST 10

#define MAX_NOTENUM 88 //40
#define MAX_NOTE_PAGE 8 //4
#define NOTENUM_PER_PAGE MAX_NOTENUM/MAX_NOTE_PAGE // 30/3 = 10
#define MAX_NOTE_LENGTH 600//255

#define MAX_CHANNEL_NUM 10
#define MAX_CHANNEL_NAME 16

enum EWEARED_ITEM
{
	eWearedItem_Weapon,
	eWearedItem_Shield,
	eWearedItem_Dress,
	eWearedItem_Hat,
	eWearedItem_Glove,
	eWearedItem_Shoes,

	eWearedItem_Ring1,
	eWearedItem_Ring2,
	eWearedItem_Necklace,
	eWearedItem_Earring1,
	eWearedItem_Earring2,
	eWearedItem_Belt,
	eWearedItem_Band,

	eWearedItem_Glasses,
	eWearedItem_Mask,
	eWearedItem_Wing,

	eWearedItem_Costume_Head,
	eWearedItem_Costume_Dress,
	eWearedItem_Costume_Glove,
	eWearedItem_Costume_Shoes,

	eWearedItem_Max,
};

enum ERROR_GUILD
{
	eGuildErr_None	= 0,
	eGuildErr_Err	= 1,
			
	eGuildErr_Create_Invalid_Name,
	eGuildErr_Create_Already,
	eGuildErr_Create_Low_Level,
	eGuildErr_Create_Insufficient_Money,
	eGuildErr_Create_Unable_Entry_Data,
	
	eGuildErr_BreakUp,
	
	eGuildErr_DeleteMember,
	eGuildErr_DeleteMember_NothaveAuthority,
	eGuildErr_DeleteMember_NotMember,
	
	eGuildErr_AddMember_NotPlayer,
	eGuildErr_AddMember_OtherGuild,
	eGuildErr_AddMember_AlreadyMember,
	eGuildErr_AddMember_FullMember,	
	eGuildErr_AddMember_TargetNotDay,
	eGuildErr_AddMember_NothaveAuthority,
	
	eGuildErr_InviteApt_NoMaster,

	eGuildErr_Mark_NoGuild,
	eGuildErr_Mark_NoMark,

	eGuildErr_LevelUp_NotAbil,
	eGuildErr_LevelUp_NotMoney,
	eGuildErr_LevelUp_Complete,

	eGuildErr_ChangeRank_NoGuild,
	eGuildErr_ChangeRank_NotMember,
	eGuildErr_ChangeRank_LowerLevel,
	eGuildErr_ChangeRank_Master,
	eGuildErr_ChangeRank_AlreadyHave,
	eGuildErr_ChangeRank_NoAuthority,
	eGuildErr_ChangeRank_NoGraduatedStudent,
	eGuildErr_ChangeRank_Student,

	eGuildErr_NoGuild,

	eGuildErr_Nick_Filter,
	eGuildErr_Nick_NotMember,
	eGuildErr_Nick_NotAvailableName,
	eGuildErr_Nick_NotLogIn,

	eGuildErr_AddStudent_TooHighLevelAsStudent,
	eGuildErr_AddStudent_Full,

	// 080417 LUJ, ±æµå ½ºÅ³ Ãß°¡ ¿¡·¯
	eGuildErr_InsufficentMoney	=	100,
	eGuildErr_InsufficentScore,
	eGuildErr_NoMaster,
	eGuildErr_NoScript,
	eGuildErr_Skill_Max,
	eGuildErr_Skill_InvalidLevel,
	eGuildErr_Skill_NotExist,

	// 090317 NYJ - ±æµå°øÁö¿¡ ÇÊÅÍ¸µ¹®ÀÚ °ËÃâ
	eGuildErr_Notice_FilterCharacter,
};
enum
{
	eGuild_Delete,
	eGuild_Secede,		
};

enum ERROR_FAMILY
{
	eFamilyErr_Err = 1,
			
	eFamilyErr_Create_Name,
	eFamilyErr_Create_Intro,
	
	eFamilyErr_BreakUp,
	
	eFamilyErr_DeleteMember,
	eFamilyErr_DeleteMember_NothaveAuthority,
	eFamilyErr_DeleteMember_NotMember,
	
	eFamilyErr_AddMember_NotPlayer,
	eFamilyErr_AddMember_OtherFamily,
	eFamilyErr_AddMember_AlreadyMember,
	eFamilyErr_AddMember_FullMember,	
	eFamilyErr_AddMember_TargetNotDay,
	eFamilyErr_AddMember_NothaveAuthority,

	eFamilyErr_InviteApt_NoMaster,

	eFamilyErr_Mark_NoFamily,
	eFamilyErr_Mark_NoMark,

	eFamilyErr_LevelUp_NotAbil,
	eFamilyErr_LevelUp_NotMoney,
	eFamilyErr_LevelUp_Complete,

	eFamilyErr_ChangeRank_NoFamily,
	eFamilyErr_ChangeRank_NotMember,

	eFamilyErr_NoFamily,

	eFamilyErr_Nick_Filter,
	eFamilyErr_Nick_NotMember,
	eFamilyErr_Nick_NotAvailableName,
	eFamilyErr_Nick_NotLogIn,

	eFamilyErr_AddStudent_TooHighLevelAsStudent,
};
enum
{
	eFamily_Delete,
	eFamily_Secede,		
};

//--------¢®E?¢®E¡Ëc¡Ë¢ç¡Ë¢¥? ¢®E¢®©­¢®¡×¢®¨Ï¢®¡×oAAo----------------------------------------------------------
// 090227 ShinJS --- CoinºÎÁ· ¿¡·¯ Ãß°¡(NOT_COIN)
enum DEAL_BUY_ERROR { NOT_ERROR, NOT_MONEY = 101, NOT_SPACE, NOT_EXIST, NOT_PLAYER, NOT_OWNERMONEY, NOT_SAMEPRICE, NOT_SAMEINFO, NOT_FISHINGPOINT, OVER_INVENMONEY, NOT_COIN, NOT_PCROOMPOINT, NOT_PCROOM };

// 090107 LYW --- CommonGameDefine : ¾ÆÀÌÅÛ »óÁ¡ ÆÇ¸Å ¿¡·¯ ºÐ·ù Ãß°¡.
enum DEAL_SELL_ERROR
{
	ERR_NONE = 0,			// ¿¡·¯¾Æ´Ô.
	ERR_NOTSELLITEM,		// ºñ¸ÅÇ° ¾ÆÀÌÅÛ.
	ERR_NOTINVENITEM,		// ÀÎº¥Åä¸®ÀÇ ¾ÆÀÌÅÛÀÌ ¾Æ´Ô.
	ERR_INVALSLOT,			// ºñÁ¤»ó SLOT.
	ERR_INVALITEMINFO,		// ºñÁ¤»ó ITEM.
	ERR_INVALPLAYER,		// ºñÁ¤»ó PLAYER.
	ERR_LOOTPLAYER,			// ·çÆÃÁßÀÎ PLAYER.
	ERR_INVALNPC,			// ºñÁ¤»ó NPC.
	ERR_INVALIDENTITY,		// ¾ÆÀÌÅÛ µ¿ÀÏ¼º Ã¼Å© ½ÇÆÐ.
	ERR_OVERSTACKCOUNT,		// ½ºÅÃ Ä«¿îÆ® ¿À¹ö.
	ERR_MAXMONEY,			// ¼ÒÀ¯°¡´É±Ý¾× ¿À¹ö.
	ERR_FAILDISCARD,		// ¾ÆÀÌÅÛ »èÁ¦Ã³¸® ½ÇÆÐ.
} ;

enum ERROR_MUNPABOARDCONTENTS {
	ERROR_MUNPABOARDCONTENTS_NOCONTENTS = 1,
	ERROR_MUNPABOARDCONTENTS_NOAUTHORITY,
};

enum ERROR_PARTY 
{
	eErr_Create_DB,
	eErr_Add_NoPlayer,
	eErr_Add_AlreadyinParty,
	eErr_Add_OptionNoParty,
	eErr_Add_DiedPlayer,
	eErr_ChangingMaster,
	eErr_Add_Full, //can not join cause party is full!
	eErr_BrokenParty, // Party has already broken.
	eErr_Create_MapHasParty, //map has party but client doesn't have party
	eErr_Add_NotOurGuild,		// During Guild Tournament
	// 070110 LYW --- Add error message for party.
	eErr_Fail_Change_PickupItem,
	eErr_Success_Change_PickupItem,
	eErr_Fail_Change_DiceGrade,
	eErr_Success_Change_DiceGrade,
	// 090528 ShinJS --- ÀÌ¸§°Ë»öÀ¸·Î ÆÄÆ¼ÃÊ´ë½Ã ¿¡·¯ Ãß°¡
	eErr_AlreadySearchingByName,	// °Ë»öÁßÀÎ °æ¿ì
	eErr_Add_NotConnectedPlayer,	// ÃÊ´ë´ë»óÀÌ Á¢¼ÓÁßÀÌÁö ¾ÊÀº °æ¿ì
	eErr_SettingLimitTime,			// ÃÖ´ë ÃÊ´ë¿äÃ» ÃÊ°ú½Ã Á¦ÇÑ½Ã°£ ¼³Á¤
};

enum PARTY_OPTION
{
	ePartyOpt_Damage,
	ePartyOpt_Sequence,
};

#define PARTY_DICEGRADE_DEFAULT 3

// 090528 ShinJS --- ÀÌ¸§À¸·Î ÆÄÆ¼ÃÊ´ë¿äÃ»¿¡ ´ëÇÑ Á¦ÇÑ¼³Á¤
enum PARTY_INVITE_BYNAME_SETTING
{
	ePartyInvite_LimitCnt = 2,									// ÃÖ´ë ÃÊ´ë¿äÃ» ´©ÀûÈ¸¼ö (ÃÊ°ú½Ã eOverCntLimitTime ¸¸Å­ ±â´ÉÁ¦ÇÑ)
	ePartyInvite_CheckSearchTimeNoPlayer = 5000,				// ÀÌ¸§°Ë»ö½Ã °Ë»ç½Ã°£ - ÇÃ·¹ÀÌ¾î°¡ Á¸ÀçÇÏÁö ¾Ê´Â°æ¿ì
	ePartyInvite_CheckSearchTimeNotConnectPlayer = 5000,		// ÀÌ¸§°Ë»ö½Ã °Ë»ç½Ã°£ - ÇÃ·¹ÀÌ¾î°¡ Á¢¼ÓÇÏÁö ¾ÊÀº°æ¿ì
	ePartyInvite_CheckSearchTimeAlreadyInParty = 5000,			// ÀÌ¸§°Ë»ö½Ã °Ë»ç½Ã°£ - ÇÃ·¹ÀÌ¾î°¡ ÀÌ¹Ì ´Ù¸¥ÆÄÆ¼¿¡ ¼ÓÇÑ °æ¿ì
	ePartyInvite_CheckSearchTimeSuccess = 5000,					// ÀÌ¸§°Ë»ö½Ã °Ë»ç½Ã°£ - °Ë»ö¿¡ ¼º°øÇÑ °æ¿ì
	ePartyInvite_OverCntLimitTimeNoPlayer = 5000,				// ´©Àû ÃÊ°ú½Ã Á¦ÇÑ½Ã°£ ¼³Á¤ - ÇÃ·¹ÀÌ¾î°¡ Á¸ÀçÇÏÁö ¾Ê´Â°æ¿ì
	ePartyInvite_OverCntLimitTimeAlreadyInParty = 5000,			// ´©Àû ÃÊ°ú½Ã Á¦ÇÑ½Ã°£ ¼³Á¤ - ÇÃ·¹ÀÌ¾î°¡ ÀÌ¹Ì ´Ù¸¥ÆÄÆ¼¿¡ ¼ÓÇÑ °æ¿ì
	ePartyInvite_OverCntLimitTimeNotConnectPlayer = 5000,		// ´©Àû ÃÊ°ú½Ã Á¦ÇÑ½Ã°£ ¼³Á¤ - ÇÃ·¹ÀÌ¾î°¡ Á¢¼ÓÇÏÁö ¾ÊÀº°æ¿ì
	ePartyInvite_OverCntLimitTimeSuccess = 5000,				// ´©Àû ÃÊ°ú½Ã Á¦ÇÑ½Ã°£ ¼³Á¤ - °Ë»ö¿¡ ¼º°øÇÑ °æ¿ì
};

//--------¢®E?¢®E¡Ëc¡Ë¢ç¡Ë¢¥? ¢®E¢®©­¢®¡×¢®¨Ï¢®¡×oAAo----------------------------------------------------------

enum TABNUMBER
{
	TAB_INVENTORY_NUM		= 2,
	TAB_MUNPAWAREHOUSE_NUM	= 7,
	TAB_BASE_STORAGE_NUM	= 2,
	TAB_STORAGE_NUM			= TAB_BASE_STORAGE_NUM + 3,
	TAB_GUILDWAREHOUSE_NUM	= 2,
};

enum CELLPERTAB
{
	TABCELL_INVENTORY_NUM		= 20,
	TABCELL_MUNPAWAREHOUSE_NUM	= 45,
	TABCELL_STORAGE_NUM			= 36,
	TABCELL_GUILDWAREHOUSE_NUM	= 36,
};

enum
{
	SLOT_INVENTORY_NUM			= TAB_INVENTORY_NUM * TABCELL_INVENTORY_NUM,
	SLOT_EXTENDED_INVENTORY1_NUM = TABCELL_INVENTORY_NUM,
	SLOT_EXTENDED_INVENTORY2_NUM = TABCELL_INVENTORY_NUM,
	SLOT_MAX_INVENTORY_NUM		= SLOT_INVENTORY_NUM + SLOT_EXTENDED_INVENTORY1_NUM + SLOT_EXTENDED_INVENTORY2_NUM,	// 80
	SLOT_WEAR_NUM				= 20,
	SLOT_MUNPAWAREHOUSE_NUM		= TAB_MUNPAWAREHOUSE_NUM * TABCELL_MUNPAWAREHOUSE_NUM,	//315
	SLOT_STORAGE_NUM			= TAB_STORAGE_NUM * TABCELL_STORAGE_NUM,				//180
	SLOT_GUILDWAREHOUSE_NUM		= TAB_GUILDWAREHOUSE_NUM * TABCELL_GUILDWAREHOUSE_NUM,	//72
	SLOT_SHOPITEM_NUM			= 36,
	SLOT_STREETSTALL_NUM		= 24,
	SLOT_STREETBUYSTALL_NUM		= 5,	
};

enum
{
	TP_INVENTORY_START				= 0,
	TP_INVENTORY_END				= TP_INVENTORY_START		+ SLOT_INVENTORY_NUM,		// 40
	TP_EXTENDED_INVENTORY1_START	= TP_INVENTORY_END,
	TP_EXTENDED_INVENTORY1_END		= TP_EXTENDED_INVENTORY1_START	+ SLOT_EXTENDED_INVENTORY1_NUM,	// 60
	TP_EXTENDED_INVENTORY2_START	= TP_EXTENDED_INVENTORY1_END,
	TP_EXTENDED_INVENTORY2_END		= TP_EXTENDED_INVENTORY2_START	+ SLOT_EXTENDED_INVENTORY2_NUM,	// 80
	TP_WEAR_START					= TP_EXTENDED_INVENTORY2_END,
	TP_WEAR_END						= TP_WEAR_START				+ SLOT_WEAR_NUM,			// 100
	TP_STORAGE_START				= TP_WEAR_END,											// 100
	TP_STORAGE_END					= TP_STORAGE_START			+ SLOT_STORAGE_NUM,			// 280
	TP_SHOPITEM_START				= TP_STORAGE_END,										// 280
	TP_SHOPITEM_END					= TP_SHOPITEM_START			+ SLOT_SHOPITEM_NUM,		// 316
	TP_MAX							= TP_SHOPITEM_END,										// 316
	TP_GUILDWAREHOUSE_START			= 500,
	TP_GUILDWAREHOUSE_END			= TP_GUILDWAREHOUSE_START + SLOT_GUILDWAREHOUSE_NUM,	// 572
// --- skr : warehouse
	TP_STORAGE_START_SET1				= 1000,
	TP_STORAGE_END_SET1					= TP_STORAGE_START_SET1			+ SLOT_STORAGE_NUM,			// 280
	TP_STORAGE_START_SET2				= TP_STORAGE_END_SET1,											// 100
	TP_STORAGE_END_SET2					= TP_STORAGE_START_SET2			+ SLOT_STORAGE_NUM,			// 280
	TP_STORAGE_START_SET3				= TP_STORAGE_END_SET2,											// 100
	TP_STORAGE_END_SET3					= TP_STORAGE_START_SET3			+ SLOT_STORAGE_NUM,			// 280
	TP_STORAGE_START_SET4				= TP_STORAGE_END_SET3,											// 100
	TP_STORAGE_END_SET4					= TP_STORAGE_START_SET4			+ SLOT_STORAGE_NUM,			// 280
};

enum eITEMTABLE
{
	eItemTable_Inventory,
	eItemTable_Weared,
	eItemTable_Storage,
	eItemTable_Shop,
	eItemTable_TableMax,
	eItemTable_MunpaWarehouse,
	eItemTable_Deal,
	eItemTable_Max,
};

enum MONEY_FLAG {
	MF_NOMAL = 0, MF_OBTAIN, MF_LOST, MF_NONE, MF_PUTINGUILD = 20, MF_PUTOUTGUILD, MF_PUTINFAMILY, MF_PUTOUTFAMILY, MF_FEE,
};

enum EAPPEAR_PART
{
	eAppearPart_Body,
	eAppearPart_Hair,
	eAppearPart_Face,
	eAppearPart_Hand,
	eAppearPart_Foot,
	eAppearPart_Weapon,
	
	eAppearPart_Max
};

#define WALKSPEED		225
#define RUNSPEED		450


#define MOVE_LIMIT_ERRORDISTANCE	1000
#define MAX_NAME_LENGTH				16
#define MAX_SKILL_NAME_LENGTH		32
#define MAX_NPC_NAME_LENGTH		32
#define MAX_MAP_NAME_LENGTH		32
#define MAX_MONSTER_NAME_LENGTH		60
#define MAX_CHAT_LENGTH				127
#define	MAX_CHANNELTITLE_LENGTH		64
#define	MAX_PLAYROOMTITLE_LENGTH	64
#define MAX_ITEMNAME_LENGTH			32
#define MAX_TARGET_NUM		300

#define MAX_SENDDATE_LENGTH		16
#define MAX_NOTE_TITLE			36


#define MIN_CHAR_CHANGESIZE	0.3f
#define MAX_CHAR_CHANGESIZE	3.0f
#define POINT_VALID_DISTANCE		6500


#define REMOVEFADE_START_TIME		2500
#define MONSTERREMOVE_TIME			5000
#define PLAYERREVIVE_TIME			5000
// ³¬½Ã°ü·Ã Define
#define MAX_FISHING_LEVEL			4			// ³¬½Ã¼÷·Ãµµ ´Ü°è
#define MAX_FISHING_MISSION			3			// ³¬½Ã¹Ì¼Ç°³¼ö
#define MAX_FISHLIST_FORPOINT		12			// ¹°°í±â¸®½ºÆ® : Æ÷ÀÎÆ®º¯È¯½Ã »ç¿ë
#define MAX_FISHINGMISSIONCODE		99			// 
#define MAX_FISHING_DISTANCE		2300.0f		// ³¬½Ã °¡´ÉÇÑ °Å¸®
#define MAX_FISHPOINT				9999999		// ÃÖ´ë ¹°°í±âÆ÷ÀÎÆ® 9,999,999
#define FISHINGTIME_STARTPULLING	2000		// ³¬½ÃÁú ½ÃÀÛ½Ã°£
#define FISHINGMISSION_INVALID		1000000		// ¹Ì¼ÇÄÚµå´Â 000000 ~ 999999 ¹üÀ§ÀÌ¹Ç·Î ÀÌ¸¦ ÃÊ°úÇÏ¸é ¹«È¿°ª.

enum eCOOKLEVEL{eCOOKLEVEL1=1, eCOOKLEVEL2=2, eCOOKLEVEL3=3, eCOOKLEVEL4=4};
enum eCOOKRECIPE{eCOOKRECIPE_ADD=1, eCOOKRECIPE_DEL=2, eCOOKRECIPE_UPDATE=3};
#define ERR_FULLLEANRN_RECIPE		-2
#define ERR_ALREADYLEARN_RECIPE		-1
#define MAX_COOKING_LEVEL			4					// ÃÖ´ë ¿ä¸®¼÷·Ã´Ü°è 4°³
#define MAX_INGREDIENT_LIST			5					// ÃÖ´ë Àç·á¸ñ·Ï 5°³
#define MAX_RECIPE_LV4_LIST			10					// DB¿¡ µî·ÏµÇ´Â ÃÖ´ë ´ÞÀÎ·¹½ÃÇÇ¸ñ·Ï 10°³
#define MAX_RECIPE_DISPLIST			99					// UI¿¡ Ãâ·ÂµÇ´Â ÃÖ´ë ·¹½ÃÇÇ¸ñ·Ï 99°³
#define MAX_COOKING_DISTANCE		300.0f				// ¿ä¸® °¡´ÉÇÑ °Å¸®
#define LV4FOOD_EAT_LEVEL			75					// ´ÞÀÎ ¿ä¸®¸¦ ¸ÔÀ» ¼ö ÀÖ´Â ·¹º§

// ÇÏ¿ìÂ¡°ü·Ã Define
enum {eHOUSEFURNITURE_ADD=1, eHOUSEFURNITURE_DEL=2, eHOUSEFURNITURE_UPDATE=3};
enum eHOUSEFURNITURE_STATE
{
	eHOUSEFURNITURE_STATE_UNKEEP	= 0,		// ÀÎº¥Åä¸®¿¡ º¸°üÁß (RemainTime ¼ÒÁø¾øÀ½)
	eHOUSEFURNITURE_STATE_KEEP		= 1,		// ³»ÁýÃ¢°í¿¡ º¸°üÁß (RemainTime ¼ÒÁø¾øÀ½)
	eHOUSEFURNITURE_STATE_INSTALL	= 2,		// ÇÏ¿ì½º¿¡ ¼³Ä¡»óÅÂ (RemainTime ¼ÒÁøµÊ)
	eHOUSEFURNITURE_STATE_UNINSTALL	= 3,		// ÇÏ¿ì½º¿¡ ¼³Ä¡Çß´Ù°¡ ³»ÁýÃ¢°í¿¡ º¸°üÁß (RemainTime ¼ÒÁøµÊ)
};
enum {
	eHOUSEERR_NONE			= 0,	// ¼º°ø(¿¡·¯¾øÀ½)
	eHOUSEERR_NOOWNER		= 1,	// ¼ÒÀ¯ÀÚ¾øÀ½
	eHOUSEERR_NOHOUSE		= 2,	// ÇÏ¿ì½º¾øÀ½
	eHOUSEERR_HAVEHOUSE		= 3,	// ÇÏ¿ì½º¼ÒÀ¯
	eHOUSEERR_ENTRANCEFAIL	= 4,	// ÀÔÀå½ÇÆÐ
	eHOUSEERR_LOADING		= 5,	// DB¿¡¼­ µ¥ÀÌÅÍ·ÎµùÁß
	eHOUSEERR_FULL			= 6,	// ÇÏ¿ì½º»ý¼ºÃÊ°ú
	eHOUSEERR_DONOT_HOUSE	= 7,	// ÇÏ¿ì½º¿¡¼­ Ã³¸®ºÒ°¡´É
	eHOUSEERR_DO_HOUSE		= 8,	// ÇÏ¿ì½º¿¡¼­ °¡´É
	eHOUSEERR_NOTOWNER		= 9,	// ¼ÒÀ¯ÀÚ°¡ ¾Æ´Ô
	eHOUSEERR_MAX_EXTEND	= 10,	// ÇÏ¿ì½ºÈ®Àå ÃÖ´ë
	eHOUSEERR_DECOMODE		= 11,	// ²Ù¹Ì±â¸ðµåÁß ºÒ°¡
	eHOUSEERR_NOTENOUGHPOINT = 12,	// DecoUseÆ÷ÀÎÆ® ºÎÁ·
	eHOUSEERR_INVAILDSTATE	= 13,	// À¯È¿ÇÏÁö ¾ÊÀº »óÅÂ
	eHOUSEERR_NOFURNITURE	= 14,	// °¡±¸°¡¾øÀ½
	eHOUSEERR_HASRIDER		= 15,	// Å¾½ÂÀÚ ÀÖÀ½
	eHOUSEERR_NOTRIDING		= 16,	// Å¾½Â»óÅÂ ¾Æ´Ô
	eHOUSEERR_RIDING		= 17,	// Å¾½ÂÁß
	eHOUSEERR_RESERVATING	= 18,	// ¹æ¹®¿¹¾àÁß
	eHOUSEERR_DISTANCE_FAR	= 19,	// °Å¸®°¡ ¸Ö´Ù
	eHOUSEERR_NOTREGIST		= 20,	// ÁÖ¹Îµî·Ï ¾ÈÇßÀ½.
	eHOUSEERR_HOME2HOME_FAIL = 21,	// È¨->È¨ ÀÌµ¿½ÇÆÐ
	eHOUSEERR_ONVEHICLE		= 22,	// Å»°Í¼ÒÈ¯ or Å¾½ÂÁß
	eHOUSEERR_NORANKER		=23,	// ·©Ä¿°¡ ¾Æ´Ô 		

	eHOUSEERR_ERROR			   ,
};

enum eHouseLog
{
	// ¾ÆÀÌÅÛ(ÀÎº¥)<->°¡±¸(³»ÁýÃ¢°í)
	eHouseLog_Keep	=	1,
	eHouseLog_UnKepp,

	// ³»ÁýÃ¢°í
	eHouseLog_Install,
	eHouseLog_UnInstall,
	eHouseLog_EndTime,	//½Ã°£¼Ò¸ê
	eHouseLog_Destroy,

	// ÇÏ¿ìÂ¡°ü·Ã ¾ÆÀÌÅÛ»ç¿ë(ÀÎº¥)
	eHouseLog_WallPaper,
	eHouseLog_FloorPaper,
	eHouseLog_CeillingPaper,
	eHouseLog_Extend,
	eHouseLog_ChangeHouseName,
};

enum {eHouseVisitByCheat=0, eHouseVisitByLink=1, eHouseVisitByItemRandom=2, eHouseVisitByItem=3, eHouseVisitByName=4};
enum {eHOUSEEXTEND_NONE=0, eHOUSEEXTEND_LV1=1, eHOUSEEXTEND_LV2=2, eHOUSEEXTEND_LV3=3, eHOUSEEXTEND_LV4=4};
#define MAX_HOUSING_CATEGORY_NUM	8		// Ä«Å×°í¸® : [0]ÀÓ½Ã [1]°¡±¸ [2]°¡Àü [3]¹® [4]º® [5]ÀåÆÇ [6]¼ÒÇ° [7]Àå½Ä
#define MAX_HOUSING_SLOT_NUM		60		// ³»ÁýÃ¢°í ½½·Ô°¹¼ö
#define MAX_HOUSING_NAME_LENGTH		32		// ÇÏ¿ìÂ¡°ü·Ã ÀÌ¸§
#define MAX_FURNITURE_STORAGE_NUM	(MAX_HOUSING_CATEGORY_NUM * MAX_HOUSING_SLOT_NUM)	// ³»ÁýÃ¢°í ÃÖ´ëº¸°ü¸ñ·Ï
//#define MAX_BONUS_LIST				20		// ÃÖ´ë ²Ù¹Ì±âÈ¿°ú °³¼ö
#define MAX_HOUSE_DECOUSEPOINT		100		// ²Ù¹Ì±â ¼Ò¸ðÆ÷ÀÎÆ®

#define MAX_HOUSE_NUM				1000	// ¿­¸± ¼ö ÀÖ´Â ÃÖ´ë ÇÏ¿ì½º°³¼ö (½ÇÁ¦·Î´Â MAX_HOUSE_NUM - 1 ¸¸Å­ ¿­¸°´Ù)

#define HOUSE_INSTALL_MARGIN		1
#define	CHECK_PECENT				50.0f
#define	MAX_HOUSE_EXTEND_LEVEL		eHOUSEEXTEND_LV4
#define MAX_HOUSE_ACTION_SLOT		5
#define MAX_HOUSE_ATTATCH_SLOT		5
#define MAX_HOUSE_ACTION_DISTANCE	500.0f
#define MAX_START_FURNITURE_HEIGHT	6.0f

#define HOUSE_ACTIONINDEX_GETOFF	7		// ³»¸®±â ¾×¼ÇÀÎµ¦½º 

enum HOUSE_ACTION_TYPE
{
	eHOUSE_ACTION_TYPE_INVALID	= 0,		// ¿¡·¯°ËÃâ¿ë
	eHOUSE_ACTION_TYPE_RIDE		= 1,		// Å¾½Â
	eHOUSE_ACTION_TYPE_GETOFF	= 2,		// ³»¸®±â
	eHOUSE_ACTION_TYPE_BUFF		= 3,		// ¹öÇÁ
	eHOUSE_ACTION_TYPE_NORMAL	= 4,		// ¾Ö´Ï,ÀÌÆåÆ®
	eHOUSE_ACTION_TYPE_STORAGE	= 5,		// Ã¢°í

	eHOUSE_ACTION_TYPE_DOOR_EXITHOUSE	= 1000,
	eHOUSE_ACTION_TYPE_DOOR_VOTEHOUSE	= 1001,

	// 091105 pdy ÇÏ¿ìÂ¡ °¡±¸ ¾×¼ÇÃß°¡ ( ¹Ì´ÏÈ¨ÇÇ À¥ ºê¶ó¿ìÀú ¸µÅ© ) 
	eHOUSE_ACTION_TYPE_OPEN_HOMEPAGE = 2000,	
};

// ÇÏ¿ìÂ¡ÆÄÀÏ ³×ÀÌ¹Ö ·ê
enum HOUSE_HIGHCATEGORY
{
	eHOUSE_HighCategory_Furniture		= 1,
	eHOUSE_HighCategory_Electric		= 2,
	eHOUSE_HighCategory_Door			= 3,
	eHOUSE_HighCategory_Wall			= 4,
	eHOUSE_HighCategory_Carpet			= 5,
	eHOUSE_HighCategory_Properties		= 6,
	eHOUSE_HighCategory_Decoration		= 7,
	
	eFN_HighCatefory_Max,
};

enum HOUSE_LOWCATEGORY_FURNITURE
{
	eHOUSE_LowCategory_Furniture_Etc		= 1,
	eHOUSE_LowCategory_Furniture_Table		= 2,
	eHOUSE_LowCategory_Furniture_Chair		= 3,
	eHOUSE_LowCategory_Furniture_Sofa		= 4,
	eHOUSE_LowCategory_Furniture_Closet		= 5,
	eHOUSE_LowCategory_Furniture_Chest		= 6,
	eHOUSE_LowCategory_Furniture_Drawer		= 7,
	eHOUSE_LowCategory_Furniture_Bed		= 8,
	
	eHOUSE_LowCategory_Furniture_Max,
};

enum HOUSE_LOWCATEGORY_ELECTRIC
{
	eHOUSE_LowCategory_Electric_Etc		= 1,
	eHOUSE_LowCategory_Electric_Ref		= 2,
	eHOUSE_LowCategory_Electric_Tv		= 3,
	
	eHOUSE_LowCategory_Electric_Max,
};

enum HOUSE_LOWCATEGORY_DOOR
{
	eHOUSE_LowCategory_Door_Etc			= 1,
	eHOUSE_LowCategory_Door_Frame		= 2,
	eHOUSE_LowCategory_Door_Window		= 3,
	eHOUSE_LowCategory_Door_Watch		= 4,
	eHOUSE_LowCategory_Door_Door		= 5,

	eHOUSE_LowCategory_Door_Max,
};

enum HOUSE_LOWCATEGORY_WALL
{
	eHOUSE_LowCategory_Wall_Etc			= 1,
	eHOUSE_LowCategory_Wall_Wall		= 2,

	eHOUSE_LowCategory_Wall_Max,
};

enum HOUSE_LOWCATEGORY_CARPET
{
	eHOUSE_LowCategory_Carpet_Etc		= 1,
	eHOUSE_LowCategory_Carpet_Tile		= 2,

	eHOUSE_LowCategory_Carpet_Max,
};

enum HOUSE_LOWCATEGORY_PROPERTIES
{
	eHOUSE_LowCategory_Properties_Etc		= 1,
	eHOUSE_LowCategory_Properties_Frame		= 2,
	eHOUSE_LowCategory_Properties_FlowerPot	= 3,
	eHOUSE_LowCategory_Properties_Cushion	= 4,
	eHOUSE_LowCategory_Properties_Clock		= 5,
	eHOUSE_LowCategory_Properties_Doll		= 6,
	eHOUSE_LowCategory_Properties_Basket	= 7,
	
	eHOUSE_LowCategory_Properties_Max,
};

enum HOUSE_LOWCATEGORY_DECORATION
{
	eHOUSE_LowCategory_Decoration_Etc		= 1,
	eHOUSE_LowCategory_Decoration_FirePlace	= 2,
	eHOUSE_LowCategory_Decoration_Mark		= 3,
	eHOUSE_LowCategory_Decoration_Lamp		= 4,
	eHOUSE_LowCategory_Decoration_Bronze	= 5,
	eHOUSE_LowCategory_Decoration_CCover	= 6,
	
	eHOUSE_LowCategory_Decoration_Max,
};

enum HOUSE_WORKINDEX
{
	eHOUSE_WorkIndex_A0		= 1,
	eHOUSE_WorkIndex_B0		= 2,
	eHOUSE_WorkIndex_C0		= 3,
	eHOUSE_WorkIndex_D0		= 4,
	eHOUSE_WorkIndex_E0		= 5,
	eHOUSE_WorkIndex_F0		= 6,

	eHOUSE_WorkIndex_Max,
};

enum HOUSE_FUNCTION
{
	eHOUSE_Function_None	= 0,
	eHOUSE_Function_Wall	= 1,
	eHOUSE_Function_Floor	= 2,
	eHOUSE_Function_Door	= 3,
	eHOUSE_Function_Start	= 4,
	eHOUSE_Function_Action	= 5,

	eHOUSE_Function_Max,
};

enum HOUSE_FURNITUREINDEX_PARTS
{
	eHOUSE_FurnitureIndex_Part_HighCategory,
	eHOUSE_FurnitureIndex_Part_LowCategory,
	eHOUSE_FurnitureIndex_Part_Work,
	eHOUSE_FurnitureIndex_Part_Index,
	eHOUSE_FurnitureIndex_Part_Action,
	
	eHOUSE_FurnitureIndex_Part_Max,
};

#define HOUSE_HIGHCATEGORY_CHAR_FURINITURE			"FU"
#define HOUSE_HIGHCATEGORY_CHAR_ELECTRIC			"EL"
#define HOUSE_HIGHCATEGORY_CHAR_DOOR				"DO"
#define HOUSE_HIGHCATEGORY_CHAR_WALL				"WA"
#define HOUSE_HIGHCATEGORY_CHAR_CARPET				"CA"
#define HOUSE_HIGHCATEGORY_CHAR_PROPERTIES			"PR"
#define HOUSE_HIGHCATEGORY_CHAR_DECORATION			"DE"

#define HOUSE_LOWCATEGORY_CHAR_FURNITURE_ETC		"ETC"
#define HOUSE_LOWCATEGORY_CHAR_FURNITURE_TABLE		"TABLE"
#define HOUSE_LOWCATEGORY_CHAR_FURNITURE_CHAIR		"CHAIR"
#define HOUSE_LOWCATEGORY_CHAR_FURNITURE_SOFA		"SOFA"
#define HOUSE_LOWCATEGORY_CHAR_FURNITURE_CLOSET		"CLOSET"
#define HOUSE_LOWCATEGORY_CHAR_FURNITURE_CHEST		"CHEST"
#define HOUSE_LOWCATEGORY_CHAR_FURNITURE_DRAWER		"DRAWER"
#define HOUSE_LOWCATEGORY_CHAR_FURNITURE_BED		"BED"

#define HOUSE_LOWCATEGORY_CHAR_ELECTRIC_ETC			"ETC"
#define HOUSE_LOWCATEGORY_CHAR_ELECTRIC_REF			"REF"
#define HOUSE_LOWCATEGORY_CHAR_ELECTRIC_TV			"TV"

#define HOUSE_LOWCATEGORY_CHAR_DOOR_ETC				"ETC"
#define HOUSE_LOWCATEGORY_CHAR_DOOR_FRAME			"FRAME"
#define HOUSE_LOWCATEGORY_CHAR_DOOR_WINDOWS			"WINDOW"//"WINDOWS"
#define HOUSE_LOWCATEGORY_CHAR_DOOR_WATCH			"WATCH"
#define HOUSE_LOWCATEGORY_CHAR_DOOR_DOOR			"DOOR"

#define HOUSE_LOWCATEGORY_CHAR_WALL_ETC				"ETC"
#define HOUSE_LOWCATEGORY_CHAR_WALL_WALL			"WALL"

#define HOUSE_LOWCATEGORY_CHAR_CARPET_ETC			"ETC"
#define HOUSE_LOWCATEGORY_CHAR_CARPET_TILE			"TILE"

#define HOUSE_LOWCATEGORY_CHAR_PROPERTIES_ETC		"ETC"
#define HOUSE_LOWCATEGORY_CHAR_PROPERTIES_FRAME		"FRAME"
#define HOUSE_LOWCATEGORY_CHAR_PROPERTIES_FLOWERPOT	"FLOWERPOT"
#define HOUSE_LOWCATEGORY_CHAR_PROPERTIES_CUSHION	"CUSHION"
#define HOUSE_LOWCATEGORY_CHAR_PROPERTIES_CLOCK		"CLOCK"
#define HOUSE_LOWCATEGORY_CHAR_PROPERTIES_DOLL		"DOLL"
#define HOUSE_LOWCATEGORY_CHAR_PROPERTIES_BASKET	"BASKET"

#define HOUSE_LOWCATEGORY_CHAR_DECORATION_ETC		"ETC"
#define HOUSE_LOWCATEGORY_CHAR_DECORATION_FIREPLACE	"FIREPLACE"
#define HOUSE_LOWCATEGORY_CHAR_DECORATION_MARK		"MARK"
#define HOUSE_LOWCATEGORY_CHAR_DECORATION_LAMP		"LAMP"
#define HOUSE_LOWCATEGORY_CHAR_DECORATION_BRONZE	"BRONZE"
#define HOUSE_LOWCATEGORY_CHAR_DECORATION_CCOVER	"CCOVER"

#define HOUSE_WORK_CHAR_A0							"A0"
#define HOUSE_WORK_CHAR_B0							"B0"
#define HOUSE_WORK_CHAR_C0							"C0"
#define HOUSE_WORK_CHAR_D0							"D0"
#define HOUSE_WORK_CHAR_E0							"E0"
#define HOUSE_WORK_CHAR_F0							"F0"

#define HOUSE_FUNCTION_CHAR_NONE					"N"
#define HOUSE_FUNCTION_CHAR_WALL					"W"
#define HOUSE_FUNCTION_CHAR_FLOOR					"F"
#define HOUSE_FUNCTION_CHAR_DOOR					"D"
#define HOUSE_FUNCTION_CHAR_START					"S"
#define HOUSE_FUNCTION_CHAR_ACTION					"A"

enum eDIFFICULTY
{
	eDIFFICULTY_NONE	= 0,
	eDIFFICULTY_EASY	= 1,
	eDIFFICULTY_NORMAL	= 2,
	eDIFFICULTY_HARD	= 3,
	eDIFFICULTY_MAX,
};


#define MAX_DUNGEON_NUM	100

enum {
	eDungeonERR_None			= 0,	// ¼º°ø(¿¡·¯¾øÀ½)
	eDungeonERR_NoKeyInfo		= 1,	// Å°Á¤º¸¾øÀ½
	eDungeonERR_NoParty			= 2,	// ÆÄÆ¼¾øÀ½
	eDungeonERR_NotEnoughLevel	= 3,	// ·¹º§¹Ì´Þ
	eDungeonERR_OverLevel		= 4,	// ·¹º§ÃÊ°ú
	eDungeonERR_EntranceDelay	= 5,	// ÀÔÀåµô·¹ÀÌ½Ã°£
	eDungeonERR_OverCount		= 6,	// ÀÔÀåÈ½¼öÃÊ°ú
	eDungeonERR_DungeonMapOff	= 7,	// ´øÀü¸Ê¼­¹ö Off
	eDungeonERR_DungeonFull		= 8,	// ´øÀü¸Ê Full
	eDungeonERR_NoNPCInfo		= 9,	// NPC Á¤º¸ ¾øÀ½

	eDungeonERR_WrongParam		= 10,	// ÆÄ¶ó¹ÌÅÍ Àß¸ø
	eDungeonERR_NoChannel		= 11,	// Ã¤³Î ¾øÀ½

	eDungeonERR_WrongDiffculty	= 12,	// ´Ù¸¥ ³­ÀÌµµÀÇ ÀÎ´øÀÔÀå
	eDungeonERR_CreateFailed	= 13,	// ´øÀü »ý¼º ½ÇÆÐ

	eDungeonERR_ERROR			= 99,	// ¹ÌÁöÁ¤ ¿¡·¯
};


enum eTimerType
{
	eTimerType_CountDown	= 0,
	eTimerType_CountUp		= 1,
};

enum eTimerState
{
	// µ¿ÀÛ°ú °ü·ÃÀÖ´Â »óÅÂ´Â Ready, Run, Pause, StopÀÇ ³×°¡ÁöÀÌ´Ù.
	eTimerState_Ready		= 0,
	eTimerState_Run			= 1,

	// µ¿ÀÛÀ» º¯°æÇÏ±â À§ÇØ ÀÇ·ÚµÇ´Â »óÅÂÀÌ´Ù. °á°ú´Â Run, Pause, StopÀ¸·Î ³ªÅ¸³­´Ù.
	eTimerState_Pause		= 2,	// °á°ú : Pause
	eTimerState_Resume		= 3,	// °á°ú : Run
	eTimerState_Stop		= 4,	// °á°ú : Stop
};

#define CONSIGNMENT_DEPOSIT_RATE		0.001
#define CONSIGNMENT_COMMISSION_RATE		0.03
#define CONSIGNMENT_SEARCHINDEX_NUM		10

enum {
	eConsignmentERR_None			= 0,	// ¼º°ø(¿¡·¯¾øÀ½)
	eConsignmentERR_WrongState		= 1,	// Àß¸øµÈ »óÅÂ
	eConsignmentERR_WrongItem		= 2,	// Àß¸øµÈ ¾ÆÀÌÅÛ
	eConsignmentERR_WrongParam		= 3,	// Àß¸øµÈ ÀÎÀÚ
	eConsignmentERR_PoorMoney		= 4,	// °ñµåºÎÁ·
	eConsignmentERR_PoorSlot		= 5,	// ÀÎº¥ºÎÁ·
	eConsignmentERR_FastRequest		= 6,	// ºü¸¥¿äÃ»
	eConsignmentERR_GoldOver		= 7,	// °ñµåÃÊ°ú

	eConsignmentERR_ERROR			= 99,	// ¹ÌÁöÁ¤ ¿¡·¯
};

enum {
	eConsignmentSORT_NameAsc		= 0,
	eConsignmentSORT_NameDesc		= 1,
	eConsignmentSORT_LevelAsc		= 2,
	eConsignmentSORT_LevelDesc		= 3,
	eConsignmentSORT_PriceAsc		= 4,
	eConsignmentSORT_PriceDesc		= 5,
};

enum {
	eConsignmentUPDATEKIND_CancelByUser	= 0,
	eConsignmentUPDATEKIND_CancelByTime	= 1,
	eConsignmentUPDATEKIND_Buy			= 2,
};

enum {
	eConsignmentDBResult_RegistedOver		= -4, // µî·ÏÃÊ°ú
	eConsignmentDBResult_DurabilityErr		= -3, // ¼ö·®ºÒÀÏÄ¡
	eConsignmentDBResult_PoorMoney			= -2, // °ñµåºÎÁ·
	eConsignmentDBResult_Err				= -1, // ¿¡·¯
	eConsignmentDBResult_None				= 0,
	eConsignmentDBResult_SoldOut			= 1, // ¸ðµÎÆÇ¸ÅµÊ
	eConsignmentDBResult_UserCancel_Ack		= 2, // À¯ÀúÃë¼Ò¼º°ø
	eConsignmentDBResult_UserCancel_Nack	= 3, // À¯ÀúÃë¼Ò½ÇÆÐ
	eConsignmentDBResult_TimeCancel_Ack		= 4, // ±â°£Ãë¼Ò¼º°ø
	eConsignmentDBResult_TimeCancel_Nack	= 5, // ±â°£Ãë¼Ò½ÇÆÐ
	eConsignmentDBResult_Buy_Ack			= 6, // ±¸¸Å¼º°ø
	eConsignmentDBResult_Buy_Nack			= 7, // ±¸¸Å½ÇÆÐ
};

enum eConsignmentLog
{
	eConsignmentLog_Regist			= 0, // µî·Ï
	eConsignmentLog_Buy				= 1, // ±¸ÀÔ
	eConsignmentLog_SellUpdate		= 2, // °»½Å (ºÎºÐÆÇ¸Å½Ã ¿øº»¾ÆÀÌÅÛÀÇ ¼ö·®º¯°æ)
	eConsignmentLog_SoldOut			= 3, // ÆÇ¸Å¿Ï·á
	eConsignmentLog_UserCancel		= 4, // À¯ÀúÃë¼Ò
	eConsignmentLog_TimeCancel		= 5, // ±â°£Ãë¼Ò
	eConsignmentLog_GetPackage		= 6, // ¼ÒÆ÷È¹µæ
	eConsignmentLog_GetPackageFail	= 7, // ¼ÒÆ÷È¹µæ½ÇÆÐ
	eConsignmentLog_DelByDate		= 8, // ¼ÒÆ÷±â°£»èÁ¦
};

enum BASICEFFECT
{
	eEffect_NewCharacter = 1,	//A¡§I¨Ï¨£¢®E¢®©­?AI ¢®IiiAa
	eEffect_Standard = 2,		//A¡§I¨Ï¨£¢®E¢®©­?AI ¡Ë¢ç¡§ua¢®¡×¢®©­¡Ë¢çi(¡§Io¡Ë¢çi¡Ë¢ç¡§ua¢®¡×u¡§I¡§¡ÌAI)
	eEffect_Walk,	//¡Ë¢ç¡§¢®E¡Ë¢ç¡§ua (¡§Io¡Ë¢çi¡Ë¢ç¡§ua ¢®¡×u¡§I¡§¡ÌAI)
	eEffect_Run,	//¢®EOU¡Ë¢ç¡§ua (¡§Io¡Ë¢çi¡Ë¢ç¡§ua¢®¡×u¡§I¡§¡ÌAI)
	eEffect_BattleWalk_Gum,		//AuAo ¡Ë¢ç¡§¢®E¡Ë¢ç¡§ua(¡Ë¢ç¡§¢®E_¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleWalk_Gwun,		//AuAo ¡Ë¢ç¡§¢®E¡Ë¢ç¡§ua(¡Ë¢ç¡§uC¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleWalk_Do,		//AuAo ¡Ë¢ç¡§¢®E¡Ë¢ç¡§ua(¢®Ii¢®Ii_¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleWalk_Chang,		//AuAo ¡Ë¢ç¡§¢®E¡Ë¢ç¡§ua(A¡Ë¢çE_¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleWalk_Gung,		//AuAo ¡Ë¢ç¡§¢®E¡Ë¢ç¡§ua(¡Ë¢ç¡§uA_¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleWalk_Amgi,		//AuAo ¡Ë¢ç¡§¢®E¡Ë¢ç¡§ua(¢®¡×uI¡Ë¢ç¡§ua_¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleRun_Gum,		//AuAo ¢®EOU¡Ë¢ç¡§ua(¡Ë¢ç¡§¢®E_¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleRun_Gwon,		//AuAo ¢®EOU¡Ë¢ç¡§ua(¡Ë¢ç¡§uC_¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleRun_Do,		//AuAo ¢®EOU¡Ë¢ç¡§ua(¢®Ii¢®Ii_¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleRun_Chang,		//AuAo ¢®EOU¡Ë¢ç¡§ua(A¡Ë¢çE¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleRun_Gung,		//AuAo ¢®EOU¡Ë¢ç¡§ua(¡Ë¢ç¡§uA_¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_BattleRun_Amgi,		//AuAo ¢®EOU¡Ë¢ç¡§ua(¢®¡×uI¡Ë¢ç¡§ua_¡§Io¡Ë¢çi¡Ë¢ç¡§uaAaA¡§I¡§¡Ì)
	eEffect_UngijosikStart,	//¢®E?i¡Ë¢ç¡§uav¢®¡×oA(¢®¡×uE¢®E¢®IA¢®Ii¢®E?AU)
	eEffect_Ungijosik,	//¢®E?i¡Ë¢ç¡§uav¢®¡×oA
	eEffect_Ungijosik_End,	//¢®E?i¡Ë¢ç¡§uav¢®¡×oA (¡§I¨Ï¨£¢®E¡Ëc¡§I¨Ï¨£¡Ë¢ç¡§IEA)
	eEffect_UngijosikDamage,	//¢®E?i¡Ë¢ç¡§uav¢®¡×oA A¡§I¢®©­¢®E?¢®E¡Ëc ¢®Ii¡Ë¢çI¡§IoIAo
	eEffect_StallStart,	//¡§I¨Ï¨£ea¡Ë¢çio ¢®Ii¢®E?AU1 (¢®¡×uE¡Ë¢ç¡§ua)
	eEffect_Stall,	//¡§I¨Ï¨£ea¡Ë¢çio ¢®Ii¢®E?AU1 (¢®¡×uE¢®¡×u¢®¡×¡Ë¢ç AO¡Ë¢ç¡§ua)
	eEffect_StallEnd,	//¡§I¨Ï¨£ea¡Ë¢çio ¢®Ii¢®E?AU1 (¢®¡×u¡Ë¢ç¡§I¡Ë¢ç¡§ua)
	eEffect_NormalDamage = 13,		//¢®Ii¡Ë¢çI¡§IoIAo1 (AA)
	eEffect_CriticalDamage = 14,		//¢®Ii¡Ë¢çI¡§IoIAo2 (¢®E?i)
	eEffect_Die1 = 16,		//A¡Ë¢ç¡Ë?= 1
	eEffect_Died1 = 16,		//A¡Ë¢ç¡Ë?¢®¡×ui¢®¡×u¡Ë¢ç¡§I ¢®E¢®I¢®¡×I¢®E?o AO¢®E¢®IA ¢®E¢®©­¡§I¢®I¢®¡×oA 1
	eEffect_Die2 = 15,		//A¡Ë¢ç¡Ë?= 2
	eEffect_Died2 = 15,		//A¡Ë¢ç¡Ë?¢®¡×ui¢®¡×u¡Ë¢ç¡§I ¢®E¢®I¢®¡×I¢®E?o AO¢®E¢®IA ¢®E¢®©­¡§I¢®I¢®¡×oA 2
	eEffect_LevelUpSentence = 3,	//¡Ë¢ç¡Ë¢¥¡§Ii¨Ïo¢®Ei¡§u¡Ë¢ç¢®¢¯¢®¡×u¡Ë¢çA ¡Ë¢ç¡§uUAU
	eEffect_MouseMovePoint,	//¢®E¢®©­¢®EO¢®E?i¢®¡×o¢®¡×¢®©­ AI¢®Ii¢®E?¢®¡×¡Ë¢ç¡Ë¢çAAI¢®¡×¡Ë¢ç¢®Ec
	
	eEffect_GetMoney = 5,	// ¢®E¢®©­¡Ë¢çAA¡Ë¢ç¡Ë?¢®E?¢®¡×I¢®¡×u¡Ë¢ç¡§I ¢®Ii¡Ë¢ç¡Ë¢¥ ¢®¡×uo¡Ë¢ç¡§ua
	eEffect_GetItem_Accessory = 25,	// ¢®E¢®©­¡Ë¢çAA¡Ë¢ç¡Ë?¢®E?¢®¡×I¢®¡×u¡Ë¢ç¡§I ¢®¡×u¢®¡×¡Ë¢çAIA¢®¡×¡Ë¢ç ¢®¡×uo¡Ë¢ç¡§ua

	eEffect_Emoticon_Bow,	//AI¡Ë¢çic
	eEffect_Emoticon_Thanks,	//¡Ë¢ç¡§¢®¡Ë¢ç¢®¡¿¡Ë¢çic
	eEffect_Emoticon_Yes,	//¡Ë¢ç¡§uad 
	eEffect_Emoticon_No,	//¢®¡×¢®©­Id
	eEffect_Emoticon_Pleasure,	//¡Ë¢ç¡§ua¡Ë¢çiY
	eEffect_Emoticon_Sadness,	//¢®¡×o¢®¡×oCA
	eEffect_Emoticon_Love,	//¡Ë¢çic¢®EOu
	eEffect_Emoticon_Hostility,	//Au¢®E¢®Ie
	eEffect_Emoticon_Disappoint,	//¢®¡×oC¢®E¢®©­A
	eEffect_Emoticon_Angry,	//¢®¡×¢®©­¢®¡×¢®E¡§I¨Ï¨£e
	eEffect_Emoticon_Suprised,	//¡§I¨Ï¨£i¢®EO¡Ë¢çA
	eEffect_Emoticon_Evasion,	//E¢®E¢®©­CC
	eEffect_Emoticon_Declare,	//¢®¡×u¡Ë¢ç¡§u¢®¡×u¡§I¢®I
	eEffect_Defence_Gum,		//¡§Ii¨Ïo|i¢®¨ú¢®¡×ui¢®Ii¢®E?AU(¡Ë¢ç¡§¢®E)
	eEffect_Defence_Gwun,		//¡§Ii¨Ïo|i¢®¨ú¢®¡×ui¢®Ii¢®E?AU(¡Ë¢ç¡§uC)
	eEffect_Defence_Do,		//¡§Ii¨Ïo|i¢®¨ú¢®¡×ui¢®Ii¢®E?AU(¢®Ii¢®Ii)
	eEffect_Defence_Chang,		//¡§Ii¨Ïo|i¢®¨ú¢®¡×ui¢®Ii¢®E?AU(A¡Ë¢çE)
	eEffect_Defence_Gung,		//¡§Ii¨Ïo|i¢®¨ú¢®¡×ui¢®Ii¢®E?AU(¡Ë¢ç¡§uA)
	eEffect_Defence_Amgi,		//¡§Ii¨Ïo|i¢®¨ú¢®¡×ui¢®Ii¢®E?AU(¢®¡×uI¡Ë¢ç¡§ua)

	eEffect_GetItem_Weapon = 7,		//¡§Ii¨Ïo|i¢®¨ú¢®¡×ui¢®Ii¢®E?AU(¢®¡×uI¡Ë¢ç¡§ua)
	eEffect_GetItem_Dress = 8,
	eEffect_GetItem_Quest = 10,
	eEffect_GetItem_Youngyak = 6,

	eEffect_MonRegen = 80,				// ¾ÕÀ¸·Î ¸ó½ºÅÍ ¸®Á¨Àº eEffect_NewCharacter ÀÌÆåÆ®¸¦ »ç¿ëÇÔ
	eEffect_ChangeStage_Hwa = 98,
	eEffect_ChangeStage_Geuk = 99,
	eEffect_PetSummon = 3151,			// Æê¼ÒÈ¯
	eEffect_PetSeal		= 3152,			// ÆêºÀÀÎ
	eEffect_FeedUp		= 3153,			// Æê¸ÔÀÌ
	eEffect_PetWarp		= 3154,			// Æê¿öÇÁ

	eEffect_ShopItem_Life = 3200,		// ±ÝÃ¢¾à	
	eEffect_ShopItem_NaeRuyk,			// ¿ä»ó´Ü	
	eEffect_ShopItem_Revive,			// ºÎÈ°ÁÖ¹®¼­
	eEffect_ShopItem_Boom1,				// ÆøÁ×(³ªºñ)
	eEffect_ShopItem_Boom2,				// ÆøÁ×(º°)
	eEffect_ShopItem_Hair1,				// °¡¹ß1	
	eEffect_ShopItem_Hair2,				// °¡¹ß2	
	eEffect_ShopItem_Hair3,				// °¡¹ß3	
	eEffect_ShopItem_Teleport1,			// ÅÚ·¹Æ÷Æ®1
	eEffect_ShopItem_Teleport2,			// ÅÚ·¹Æ÷Æ®2
	eEffect_ShopItem_Teleport3,			// ÅÚ·¹Æ÷Æ®3
	eEffect_ShopItem_MaxLife,			// »ý¸íÀÇ ¹°¾à
	eEffect_ShopItem_MaxNaeRuyk,		// ³»·ÂÀÇ ¹°¾à

	// 070528 LYW CommonGameDefine : Add effect of emoticon part.
	eEffect_Emoticon1 = 51, 
	eEffect_Emoticon2 = 52, 
	eEffect_Emoticon3 = 53, 
	eEffect_Emoticon4 = 54, 
	eEffect_Emoticon5 = 55, 
	eEffect_Emoticon6 = 56, 

	eEffect_GetItem_Unique	= 40,
	eEffect_GetItem_Rare	= 41,

	// 071206 LUJ ±æµå Á¹¾÷ È¿°ú
	eEffect_Guild_Graduate	= 49,
};

enum MapName
{
	MapNameNone,
	Alker_Farm = 2,
	// 090921 LUJ, ³×¶ó ³óÀå Ãß°¡
	Nera_Farm = 3,
	Moon_Blind_Swamp = 13,
	Red_Orc_Outpost,	
	Moon_Blind_Forest,	
	Haunted_Mine_1F = 16,
	Haunted_Mine_2F = 17,
	The_Gate_Of_Alker = 19,
	Alker_Harbor,	

	Ruins_Of_Draconian,	
	Zakandia,	
	Tarintus,	
	Mont_Blanc_Port = 25,	
	Dryed_Gazell_Fall,		
	Zakandia_Outpost,		
	The_Dark_Portal,		
	Distorted_crevice = 29,
	The_Way_To_The_Howling_Ravine = 31,
	Howling_Ravine = 32,
	Howling_Cave_1F = 33,
	Howling_Cave_2F = 34,
	Ghost_Tree_Swamp = 41, 
	Lair_of_Kierra = 42,
	The_Valley_Of_Fairy = 51,
	The_Town_of_Nera_Castle = 52,
	The_Great_Garden = 53,
	// 080714 LUJ, "±â»ç´ÜÀÇ ¹¦Áö" ¸Ê Ãß°¡
	TheKnightsGrave	= 54,
	A_Harbor_of_Nera = 55,
	Temple_of_Greed = 56,
	Broken_Shipwrecked = 57,
	The_tower_of_wizardly_Low = 60,
	The_tower_of_wizardly_Middle = 61,
	The_tower_of_wizardly_High = 62,
	The_Death_tower = 63,
	The_Spirit_Forest = 64,
	The_Cristal_Empire = 65,
	TreeOfKenai = 67,
	TheCryingTavern = 68,
	TheUndergroundPassage = 69,
	SahelZone = 71,
	MirageInTheParasus = 72,
	RuinsOfAncientElpis = 73,
	DragoniansTomb1 = 74,
	DragoniansTomb2 = 75,
 	// 080905 LYW --- CommonGameDefine : °ø¼º °ü·Ã ¸Ê ÀÌ¸§ Ãß°¡.
	The_Castle_of_Rushen = 81,
	The_Culvert = 82,
	The_Castle_of_Zevyn = 83,
 	Advanced_Dungeon = 84,

	DateDungeon_Cemetery = 91,
	DateDungeon_BrokenHouse = 92,
	DateDungeon_DarkCave = 93,

	GuildTournamentStadium = 94,
	// 090609 LUJ, ½Å±Ô Áö¿ª Ãß°¡
	DesolatedGorge = 96,

	DateDungeon_CookieGarden = 97,

	AlkerResidence = 99,
	
	// 090626 NYJ ÃÖ´ë ¸Ê °¹¼ö 200°³·Î È®Àå
	MAX_MAP_NUM = 200,
	//MAX_MAP_NUM,
};

enum EObjectState
{
	eObjectState_None,						// ¢®¡×u¢®¡×¡Ë¢ç¡§Io¡Ë¢çi¡Ë¢çioAA ¢®¡×u¢®¡×¡Ë¢ç¢®E¢®IO..
	eObjectState_Enter,						// A¡§I¨Ï¨£¢®E¢®©­?AI¡§I¨Ï¨£¢®¡×¢®I ¢®E¢®©­o¢®¡×o¢®¡×¢®©­AI¡Ë¢ç¡§¢®¢®E¡Ëc A¡§I¨Ï¨£= ¢®IiiAaCO¢®EO¡Ë¢ç¢®¢¯ AC ¡Ë¢çioAA
	eObjectState_Move,						// AAC¡Ë¢çI AI¢®Ii¢®E? ¡Ë¢çioAA
	eObjectState_Rest,						// ¢®E¢®©­o¢®¡×o¢®¡×¢®©­AIAC E¢®¡×¢®¨Ï¢®¡×oA ¡Ë¢çioAA
	
//	eObjectState_Speech,					// Npc¢®E?I ¢®E¢®IeE¡Ë¢ç¡§I ¡Ë¢çioAA
	eObjectState_Deal,						// ¡Ë¢çioa Open ¡Ë¢çioAA

	eObjectState_Exchange,					// ¡Ë¢ç¡§u¡§I¨Ï¨£E? ¡Ë¢çioAA
	eObjectState_StreetStall_Owner,			// ¡§I¨Ï¨£ea¡Ë¢çio Open ¡Ë¢çioAA
	eObjectState_StreetStall_Guest,			// ¡§I¨Ï¨£ea¡Ë¢çio ¢®¡×uO¢®E¢®IO ¡Ë¢çioAA
	eObjectState_PrivateWarehouse,			// ¡Ë¢ç¡§¢®¡§I¨Ï¨£AIA¡Ë¢çE¡Ë¢ç¡§¢®i 10
	eObjectState_Munpa,						//¡§Io¢®Ec¢®¡×¡Ë¢çA ¢®E¢®©­¢®Ec¢®¡×i¨Ïo¢®Ei¡§u¢®¡×¡Ë¢ç¢®Ec¡§I¨Ï¨£¢®¡×¢®I ¡§Io¢®Ec¢®¡×¡Ë¢çAA¡Ë¢çE

	eObjectState_SkillStart,				// ¢®¡×o¢®¡×¢®©­A¡§I¨Ï¨£ ¢®¡×oAAu ¡Ë¢çioAA
	eObjectState_SkillSyn,					//
	eObjectState_SkillBinding,				//
	eObjectState_SkillUsing,				//
	eObjectState_SkillDelay,				// A¢®¡×¢®¨Ï¢®¡×¢®©­¢®E¢®©­¡Ë¢ç¡§¢®¢®E¡Ëc ¡§I¢®A¡Ë¢ç!¡§¢®a; ¢®EO¡Ë¢ç¢®¢¯, ¡§Io¡Ë¢çi¡Ë¢ç¡§¢®¡§I§¡Ì¢??¡§I¢®A¡Ë¢ç¡§¢®i¡§I¨Ï¨£¡Ë¢ç¡§IEAAC ¢®Iio¡Ë¢ç¡Ë¢¥¡§IoAI
	
	eObjectState_TiedUp_CanMove,			// ¡Ë¢ç¡§¢®a¡§IoU ¡Ë¢çioAA
	eObjectState_TiedUp_CanSkill,
	eObjectState_TiedUp,
	
	eObjectState_Die,						// A¡Ë¢ç¡Ë?: ¡Ë¢çioAA 20	
	
	eObjectState_BattleReady,
	eObjectState_Exit,						//KES 040707 A¡§I¨Ï¨£¢®E¢®©­?AI¡Ë¢ç¡§¢®¢®E¡Ëc ¡§I¨Ï¨£¢®¡×¢®I¡Ë¢ç¡§¢®¡Ë¢çI¢®EO¡Ë¢ç¢®¢¯
	eObjectState_Immortal,					//¢®¢´I¢®¨ú¢®¢¯AI, ¡Ë¡þEAuE?, AuAa'A¡Ë¢ç ¢®ii¡§u¡§¢®¨Ï©ª¡§¡Ì¢®¨úa¡§oA ¨Ïo¢®iAu¢®ioAA
	eObjectState_ImmortalDefense,			//

	eObjectState_Society,					//KES A¢¯©öA¢¥I¨¡¨ù ¥ì¢¯AU
	
	eObjectState_ItemUse,

	eObjectState_TournamentReady,
	eObjectState_TournamentProcess,
	eObjectState_TournamentEnd,
	eObjectState_TournamentDead,

	eObjectState_Engrave,

	// 070503 LYW --- CommonGameDefine : Add enum to setting rest damage.
	eObjectState_RestDamage,
	eObjectState_Connect,					// ¿õÁÖ 070823

	// 080428 NYJ --- ³¬½Ã»óÅÂ
	eObjectState_Fishing,
	eObjectState_FishingResult,

	eObjectState_Cooking,
	eObjectState_Housing,
	eObjectState_HouseRiding,
	
	eObjectState_MAX,	
};

enum eObjectBattleState
{
	eObjectBattleState_Peace,
	eObjectBattleState_Battle,
	eObjectBattleState_Max,
};

#define FARM_RENDER_OBJ_ID_START	0x8fffffff
// E ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.05.07
#define STATICNPC_ID_START		0xa0000000
#define STATICNPC_ID_MAX		0x10000000
#define SKILLOBJECT_ID_START	0xc0000000
#define SKILLOBJECT_ID_MAX		0x0000ffff
#define BATTLE_ID_START			0xd0000000
#define BATTLE_ID_MAX			0x0000ffff
#define TEMP_SKILLOBJECT_ID		0xffffffff
#define BATTLE_TEAMEFFECT_ID	0xfffffffe
#define BATTLE_STAGEEFFECT_ID	0xfffffffd
#define STATE_EFFECT_ID			0xfffffffc
#define PK_EFFECT_ID			0xfffffffb
#define NPCMARK_1_ID			0xfffffffa		// !
#define NPCMARK_2_ID			0xfffffff9		// ?
#define BATTLE_GTTEAM1_EFFECTID	0xfffffff7
#define BATTLE_GTTEAM2_EFFECTID	0xfffffff6
#define ENGRAVE_EFFECTID		0xfffffff4
#define ENGRAVE_ACK_EFFECTID	0xfffffff3
#define ENGRAVE_NACK_EFFECTID	0xfffffff2
#define LIFE_RECOVER_EFFECTID	0xfffffff1
#define MANA_RECOVER_EFFECTID	0xfffffff0
#define BATTLE_GTTEAM3_EFFECTID	0xffffffef
#define ALL_RECOVER_EFFECTID	0xffffffee
#define DETECT_EFFECT_ID		0xffffffea

#define HELMET_EFFECT			0xffffffe9
#define SHIELD_EFFECT			0xffffffe8
#define HEART_EFFECT			0xffffffe7
#define ITEM_WORKING_SUCCESS_EFFECT	0xffffffe6
#define ITEM_WORKING_FAIL_EFFECT	0xffffffe5
#define ITEM_WORKING_EFFECT			0xffffffe4

#define FISHING_START_EFFECT		0xffffffe3
#define FISHING_SUCCESS_EFFECT		0xffffffe2
#define FISHING_FAIL_EFFECT			0xffffffe1

#define BATTLE_GTIMMORTAL_ID		0xffffffe0

// 080827 LYW --- CommonGameDefine : ¿öÅÍ½Ãµå »ç¿ë ÀÌÆåÆ® ¹øÈ£¸¦ ÁöÁ¤ÇÑ´Ù.
#define SIEGEWARFARE_USING_WATERSEED	0xffffffdf

#define ITEM_SUMMON_EFFECTACTIONID	0xffffffde
#define ITEM_SUMMON_EFFECTID		0xffffffdd

#define COOKING_EFFECT				0xffffffdc
#define HOUSE_DECOMODE_EFFECT		0xffffffdb
// 100406 ShinJS --- ½Ã°£Á¦ÇÑ Äù½ºÆ® !, ? Effect ID
#define NPCMARK_3_ID			0xffffffda		// !
#define NPCMARK_4_ID			0xffffffd9		// ?
typedef BYTE DIRINDEX;
#define MAX_DIRECTION_INDEX		32
#define DEGTODIR(angleDeg)	(BYTE)((angleDeg)/360*MAX_DIRECTION_INDEX)
#define DIRTODEG(DirIndex)	(((float)DirIndex)*360/MAX_DIRECTION_INDEX)
#define DIRTORAD(DirIndex)	(DEGTORAD(DIRTODEG(DirIndex)))
#define RADTODIR(angleRad)	(DEGTODIR(RADTODEG(angleRad)))

// A¡Ë¢çi¢®E¢®©­¢®¡×¢®¨Ï¢®EOo ¢®¡×u¡§I¨Ï¨£d
#define CAMERA_NEAR		100
#define MAX_CHARTARGETPOSBUF_SIZE	15

//system
#define MAX_IPADDRESS_SIZE	16

// type : 0->¢®¡×oA¡Ë¢ç¡§¢®¡Ë¢çI¢®EO¡Ë¢ç¢®¢¯¢®¡×¢®©­¡Ë¢ç¡§¢® ¡§I¨Ï¨£¡§I¢®A¡Ë¢ç¡§ue. 1~100->AId¡Ë¢ç¡Ë¢¥¢®Ec AI¡Ë¢çio¢®¡×¢®©­?E¡Ë¢ç¡§I¢®¡×oA ¡§I¨Ï¨£¡§I¢®A¡Ë¢ç¡§ue(¡Ë¢ç¡§¢®¢®E¡Ëc¡Ë¢çie), 101~200->(¡Ë¢ç¡§¢®¡Ë¢ç¢®¡¿¡Ë¢çie)
// 1->¡Ë¢çioa¢®E?¢®E¡Ëc ¢®¡×¡Ë¢çC¢®E¢®©­A, 2->¡§I¨Ï¨£ea¢®E?¢®E¡Ëc¢®¡×u¡Ë¢ç¡§I ¢®¡×¡Ë¢çC¢®E¢®©­A, 3->¢®E¢®©­o¢®¡×o¢®¡×¢®©­AI¢®E?¢®E¡Ëc¡Ë¢ç¡§¢®O E¡§Io¢®Ii¡§I¡Ë¢ç, 4->¡Ë¢ç¡§u¡§I¨Ï¨£E?, 5->A¡Ë¢çE¡Ë¢ç¡§¢®i¢®E?¢®E¡Ëc¢®¡×u¡Ë¢ç¡§I ¡Ë¢çi¡Ë¢ç¨Ï¡©¢®EO¡Ë¢ç¢®¢¯, 6->¡§Io¢®Ec¢®¡×¡Ë¢çAA¡Ë¢çE¡Ë¢ç¡§¢®i¢®E?¢®E¡Ëc¢®¡×u¡Ë¢ç¡§I ¡Ë¢çi¡Ë¢ç¨Ï¡©¢®EO¡Ë¢ç¢®¢¯
// 101->¡Ë¢çioa¢®E?¢®E¡Ëc¢®¡×u¡Ë¢ç¡§I ¡Ë¢çii¢®EO¡Ë¢ç¢®¢¯, 102->¡§I¨Ï¨£ea¢®E?¢®E¡Ëc¢®¡×u¡Ë¢ç¡§I ¡Ë¢çii¢®EO¡Ë¢ç¢®¢¯, 103->A¡Ë¢ç¡Ë?¢®¡×uu;¢®EO¡Ë¢ç¢®¢¯, 104->¡Ë¢ç¡§u¡§I¨Ï¨£E?, 105->A¡Ë¢çE¡Ë¢ç¡§¢®i¢®E?¢®E¡Ëc ¡§I¨Ï¨£O=, 106->¡§Io¢®Ec¢®¡×¡Ë¢çAA¡Ë¢çE¡Ë¢ç¡§¢®i¢®E?¢®E¡Ëc ¡§I¨Ï¨£O=, 107->C¡Ë¢çI¡Ë¢ç¡§u¡§Io ¡Ë¢çii¢®EO¡Ë¢ç¢®¢¯
enum eMoneyLogtype
{
	eMoneyLog_Time				= 0,
// ¡Ë¢ç¡§¢®¢®E¡Ëc¡Ë¢çie
	eMoneyLog_GetStall			= 1,
	eMoneyLog_GetStreetStall	= 2,
	eMoneyLog_GetMonster		= 3,
	eMoneyLog_GetExchange		= 4,
	eMoneyLog_GetStorage			= 5,
	eMoneyLog_GetGuild			= 6,
	eMoneyLog_GetWantedPrize	= 7,
	eMoneyLog_GetPKLooting		= 8,
	eMoneyLog_GetSuryunFail		= 9,
	eMoneyLog_GetPrize			= 10,
	eMoneyLog_GetSWProfit		= 11,
	eMoneyLog_GetFromQuest		= 12,
	eMoneyLog_GetGMTool			= 13,
	eMoneyLog_GetFamily,
	// desc_hseos_¸ó½ºÅÍ¹ÌÅÍ01
	// S ¸ó½ºÅÍ¹ÌÅÍ Ãß°¡ added by hseos 2007.07.30
	eMoneyLog_GetMonstermeter,
	// E ¸ó½ºÅÍ¹ÌÅÍ Ãß°¡ added by hseos 2007.07.30

	
// ¡Ë¢ç¡§¢®¡Ë¢ç¢®¡¿¡Ë¢çie
	eMoneyLog_LoseStall			= 101,
	eMoneyLog_LoseStreetStall	= 102,
	eMoneyLog_LoseDie			= 103,
	eMoneyLog_LoseExchange		= 104,
	eMoneyLog_LoseStorage		= 105,
	eMoneyLog_LoseGuild			= 106,
	eMoneyLog_LoseStorageBuy		= 107,
	eMoneyLog_LoseWantedRegist	= 108,
	eMoneyLog_LoseWantedBuyRight = 109,
	eMoneyLog_LosePKLooted		= 110,
	eMoneyLog_LosePKModeExitPanelty = 111,
	eMoneyLog_LoseBadFrameBail	= 112,
	eMoneyLog_LoseSuryunFee		= 113,
	eMoneyLog_LoseGuildLevelUp	= 114,
	eMoneyLog_LoseChangeLocation	= 115,
	eMoneyLog_LoseGMTool		= 116,
	eMoneyLog_LoseFamily,
	eMoneyLog_LoseFamilyLevelUp,
	// desc_hseos_³óÀå½Ã½ºÅÛ_01
	// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.08.23	2007.09.10	2008.01.15
	eMoneyLog_LoseFarmBuy,
	eMoneyLog_LoseFarmUpgrade,
	eMoneyLog_LoseFarmTax,
	eMoneyLog_TollForVehicle,
	eMoneyLog_Enchant,
	eMoneyLog_TriggerAction,
	eMoneyLog_Skill,
	eMoneyLog_GetCheatMoney = 201,

	eMoneyLog_Consignment_Charge,
	eMoneyLog_MoneyFromNote,
};

enum eLogSkill
{
	eLog_SkillLearn = 0,
	eLog_SkillDiscard,
	eLog_SkillDestroyByWanted,
	eLog_SkillDestroyByBadFame,
	eLog_SkillLevelup,
	eLog_SkillCheatLevelup,
	eLog_SkillLevelDownByWanted,
	eLog_SkillLevelDownByBadFame,
	eLog_SkillDestroyByGetJulCho,
	eLog_SkillDestroyByBadFameJulChoDown,
	eLog_SkillDestroyByGetNextLevel,		// for japan

	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
	// ¹«°ø º¯È¯
	eLog_SkillOption,
	eLog_SkillOptionClear,
	eLog_SkillDestroyByOptionClear,
	////////////////////////////////////////////////////////////////////////////////

	eLog_Skill_RM	= 100,
	eLog_SkillLearn_RM,
	eLog_SkillLevel_RM,
	eLog_SkillRemove_RM,

	eLog_SkillGetPoint,
	// 080819 LUJ, Ä³½¬ ½ºÅ³ ·Î±×
	eLog_SkillCashAdd,
	eLog_SkillCashRemove,
};


enum eFamilyLog
{
	// Æ÷ÀÎÆ®
	eLog_FamilyPoint	= 0,
	eLog_FamilyPointAdd,
	// 080610 LUJ, Æ÷ÀÎÆ® º¯°æ
	eLog_FamilyPointUpdate,

	// ³óÀå
	eLog_FamilyFarm		= 100,
	eLog_FamilyFarmPutCrop,		// ÆÄÁ¾
	eLog_FamilyFarmDieCrop,		// °í»ç
	eLog_FamilyFarmGrowCrop,	// »ýÀå
	eLog_FamilyFarmCareCrop,	// ºñ·á
	eLog_FamilyFarmHarvestCrop,	// ¼öÈ®
	eLog_FamilyFarmLostRight,	// ±Ç¸® »ó½Ç(ÆÐ¹Ð¸® Å»Åð µîÀ¸·Î ÀÎÇÑ ±Ç¸® »ó½Ç)
	eLog_FamilyFarmDisband,		// ³óÀå ÇØÃ¼(ÆÐ¹Ð¸® ÇØ»êÀ¸·Î ÀÎÇÑ)
	// desc_hseos_³óÀå½Ã½ºÅÛ_01
	// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2008.01.16
	eLog_FamilyFarmTax,			// ³óÀå ÇØÃ¼(°ü¸®ºñ ¹Ì³³)
	eLog_FamilyFarmMystery,		// ³óÀå ÇØÃ¼(³óÀå ÇØÃ¼ ½Ã ·Î±×¸¦ Ãß°¡ÇÏÁö ¾Ê¾ÒÀ» °æ¿ì)
	// E ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2008.01.16
	// 080425 KTH -- Ãà»ç °ü·Ã ·Î±× Ãß°¡ --------------
	eLog_FamilyFarmInstallAnimal,
	eLog_FamilyFarmDieAnimal,
	eLog_FamilyFarmFeedAnimal,
	eLog_FamilyFarmCleanAnimal,
	eLog_FamilyFarmAnimalReward,
	// 081205 LUJ, ÆÐ¹Ð¸® Çàµ¿
	eLog_FamilyCreate = 200,
	eLog_FamilyAddMember,
	eLog_FamilyChangeNick,
	eLog_FamilyRegistEmblem,
	eLog_FamilyLeaveMember,
	eLog_FamilyExpelMember,
	eLog_FamilyBreakup,
	// 091111 ONS ÆÐ¹Ð¸® ¹®Àå »èÁ¦
	eLog_FamilyDelEmelem,
	// 091126 ONS ÆÐ¹Ð¸®Àå ÀÌ¾ç ±â´É Ãß°¡
	eLog_FamilyChangeMaster,
};

// desc_hseos_³óÀå½Ã½ºÅÛ_01
// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2008.01.23
enum FARM_LOG_KIND																			// ³óÀå ·Î±× Á¾·ù
{
	FARM_LOG_KIND_BUY = 0,																	// ..±¸ÀÔ
	FARM_LOG_KIND_UPGRADE,																	// ..¾÷±×·¹ÀÌµå
	FARM_LOG_KIND_PAY_TAX,																	// ..°ü¸®ºñ ³³ºÎ
	FARM_LOG_KIND_DEL_FAMILY_BREAKUP,														// ..»èÁ¦ - ÆÐ¹Ð¸® ÇØÃ¼
	FARM_LOG_KIND_DEL_TAX_ARREARAGE,														// ..»èÁ¦ - °ü¸®ºñ Ã¼³³
	FARM_LOG_KIND_MYSTERY = 9999,
};

enum FARM_UPGRADE_LOG_KIND																	// ³óÀå ¾÷±×·¹ÀÌµå ·Î±× Á¾·ù
{
	FARM_UPGRADE_LOG_KIND_GARDEN = 0,														// ..ÅÔ¹ç
	FARM_UPGRADE_LOG_KIND_FENCE,															// ..¿ïÅ¸¸®
	FARM_UPGRADE_LOG_KIND_HOUSE,															// ..Áý
	FARM_UPGRADE_LOG_KIND_WAREHOUSE,														// ..Ã¢°í
	FARM_UPGRADE_LOG_KIND_ANIMALCAGE,														// ..Ãà»ç
	FARM_UPGRADE_LOG_KIND_MYSTERY = 9999,
};

enum eLogitemmoney
{
	eLog_ItemNone		= 0,
	eLog_ItemDiscard	= 1,
	eLog_PetItemDiscard,
	
	//item move
	eLog_ItemMoveInvenToStorage = 100,
	eLog_ItemMoveStorageToInven,
	eLog_ItemMoveStorageToStorage,
	eLog_ItemMoveInvenToGuild,
	eLog_ItemMoveInvenToInven,
	eLog_ItemMoveGuildToInven,
	eLog_ItemMoveGuild,
	eLog_MoneyDepositToGuild,
	eLog_MoneyWithdrwaGuild,
	// 080716 LUJ, Æê<->ÇÃ·¹ÀÌ¾î ÀÌµ¿
	eLog_ItemMoveFromPlayerToPet,
	eLog_ItemMoveFromPetToPlayer,
	
	//item obtain/destory
	eLog_ItemObtainBuy = 200,
	eLog_ItemObtainDivide,
	eLog_ItemObtainCheat,
	eLog_ItemObtainMonster,
	eLog_ItemObtainQuest,
	eLog_ItemObtainPK,
	eLog_ItemObtainDissolve,
	eLog_ItemObtainMix,
	eLog_ItemObtainFromChangeItem,
	eLog_ItemRareObtainCheat,
	eLog_ItemOptionObtainCheat,
	eLog_ItemObtainMonstermeter,
	eLog_ItemObtainMarriage,
	eLog_ItemObtainFishing,
	eLog_ItemObtainGift,
	eLog_ItemObtainMerge,
	eLog_ItemObtainGTReward,	// ±æµåÅä³Ê¸ÕÆ® ¿ì½Â»óÇ°

	eLog_ItemObtainHouseRestore, // ÇÏ¿ì½º ¾ÆÀÌÅÛº¸°üÇØÁ¦
	eLog_ItemObtainHouseBonus,	// ÇÏ¿ì½º º¸³Ê½º(¾ÆÀÌÅÛÈ¹µæ)

	eLog_ItemSell = 300,
	eLog_ItemUse,
	
	eLog_StreetStallBuyAll = 400,
	eLog_StreetStallBuyDivide,
	eLog_StreetStallSellDivide,

	eLog_Exchange = 500,

	eLog_ItemReinforceSuccess = 600,
	eLog_ItemReinforceFail,
	eLog_ItemReinforceDataHaveTooManyMaterials,
	eLog_ItemReinforceSourceInvalid,
	eLog_ItemReinforceMaterialIsInvalid,
	eLog_ItemReinforceWhichMaterialHasZero,
	eLog_ItemReinfoceYouCannotOver100ToReinforce,
	eLog_ItemReinforceSourceItemNotExist,
	eLog_ItemRerinfoceServerHasNoReinforceScript,
	eLog_ItemReinforceMaterialSourceItemCannnotReinforce,
	eLog_ItemReinforceSourceItemCanDuplicate,
	eLog_ItemReinforceRemovingMaterialIsFailed,
	eLog_ItemReinforceUpdatingItemIsFailed,
	// 080929 LUJ, °­È­ º¸Á¶ ¾ÆÀÌÅÛ ¿À·ù Ãß°¡
	eLog_ItemReinforceInvalidSupportItem,
	eLog_ItemReinforceNoSupportScript,
	eLog_ItemReinforceInvalidMaterialForSupportItem,

	eLog_ItemMixSuccess = 700,
	eLog_ItemMixFail,
	eLog_ItemMixBigFail,
	eLog_ItemMixInsufficentSpace,
	eLog_ItemMixServerError,
	eLog_ItemMixUse,
	
	eLog_ItemDissolveSuccess,
	eLog_ItemDissolvePlayerNotExist,
	eLog_ItemDissolveSourceItemNotExist,
	eLog_ItemDissolveServerHaveNoDissolvingData,
	eLog_ItemDissolveItemDataNotMatched,
	eLog_ItemDissolveItNeedMoreLevel,
	eLog_ItemDissolveRemovingItemIsFailed,
	eLog_ItemDissolveAddingItemIsFailed,
	eLog_ItemDissolvePlayserHasNoInventory,
	eLog_ItemDissolveInsufficentSpace,
	eLog_ItemDissolveNoResult,
	eLog_ItemDissolveServerError,
	eLog_ItemDissolveScriptInvalid,
	eLog_ItemDissolveInsert,		// ºó ½½·Ô¿¡ ¾ÆÀÌÅÛÀ» ³ÖÀ½
	eLog_ItemDissolveAddDuplicated,	// ºÐÇØ½Ã ¾ÆÀÌÅÛÀÌ µç ½½·Ô¿¡ °ãÃÄÁö´Â ¾ÆÀÌÅÛÀ» Ãß°¡ÇÔ

	eLog_ItemEnchantSuccess,
	eLog_ItemEnchantItemInvalid,
	eLog_ItemEnchantServerError,
	eLog_ItemEnchantHacking,
	eLog_ItemEnchantItemUpdateFailed,
	eLog_ItemEnchantUse,
	eLog_ItemEnchantDestroy,
	eLog_ItemEnchantNotEnoughMoney,	// 090713 ShinJS --- ÀÎÃ¦Æ®½Ã ¼ÒÁö±ÝÀÌ ºÎÁ·ÇÑ °æ¿ì
	eLog_ItemEnchantLevelChanged,
	eLog_ItemEnchantProtected,

	eLog_ItemMixSystemFault,	// Á¶ÇÕ ½Ã ½Ã½ºÅÛ ¿À·ù
	eLog_ItemMixNotFound,		// Á¶ÇÕÇÒ ´ë»óÀ» Ã£Áö ¸øÇÔ
	eLog_itemMixBadRequest,		// Àß¸øµÈ ¿äÃ»
	eLog_ItemMixBadCondition,	// Á¶ÇÕÇÒ Á¶°ÇÀÌ µÇÁö ¾ÊÀ½
	eLog_ItemMixBadQuantity,	// °³¼ö°¡ Àß¸øµÊ
		
	eLog_ItemDestroyMix = 800,
	eLog_ItemDestroyReinforce,
	eLog_ItemDestroyUpgrade,
	eLog_ItemDestroyByWanted, // eLog_ItemDestroyByBadFame¢¬| AIAEA¢¬¡¤I ©ø¡í¡¤A¨ú©¬Ao
	eLog_ItemDestroyByBadFame, //die panelty with bad fame.
	eLog_ItemDestroyDissolution, // dissolve base item
	eLog_ItemDestroyGiveQuest, // by complete quest 
	eLog_ItemDestroyDeleteQuest,
	eLog_ItemDestroybyChangeItem,
	eLog_ItemDestroyReinforceWithShopItem,

	// 090207 LUJ, ÇÕ¼º ·Î±×
	eLog_ItemComposeSuccess = 900,
	eLog_ItemComposeSuccessWithConsume,
	eLog_ItemComposeFailureByMemoryMismatch,
	eLog_ItemComposeFailureByWrongScript,
	eLog_ItemComposeFailureByWrongUseItemSize,
	eLog_ItemComposeFailureByInvalidResult,
	eLog_ItemComposeFailureByInvalidKeyItem,
	eLog_ItemComposeFailureByNoExistedOption,
	eLog_ItemComposeFailureByNoResultItemInfo,

	eLog_MoneyObtainPK = 1000,
	//exp obtain
	eLog_ExpObtainPK,

	eLog_ShopItemUse = 1500,	
	eLog_ShopItemUseEnd,
	eLog_ShopItemMoveToInven,
	eLog_ShopItemProtectAll,
	eLog_ShopItemSeal,
	eLog_ShopItemUnseal,
	eLog_ShopItemUnseal_Failed,
	eLog_ShopItem_ReinforceReset,
	eLog_ShopItemGetCheat,
	// 090625 pdy Æê ±â°£Á¦ ¾ÆÀÌÅÛ Âø¿ëÃ³¸® Ãß°¡ 
	eLog_ShopItemUseEndFromPetInven,

	eLog_ShopItemUse_MixSuccess=1600,

	// 080602 LUJ, ½ºÅ³ »ç¿ë ½Ã ¼Ò¸ð
	eLog_SkillItemUse	= 1700,
	eLog_SkillMoneyUse,
	
	eLog_Item = 2000,
	eLog_ItemOptionAdd_RM,
	eLog_ItemOptionRemove_RM,
	eLog_ItemAdd,
	eLog_ItemRemove,
	eLog_ItemRemoveMarriageDivorce,
	eLog_ItemDropOptionAdd,
		
	eLog_ItemMoveInvenToPetInven = 2100,
	eLog_ItemMovePetInvenToInven,

	eLog_ItemSkillLearn	= 2200,

	eLog_GTournamentReg = 2300,		// ±æµåÅä³Ê¸ÕÆ® µî·Ïºñ
	// 081205 LUJ, ³óÀå
	eLog_FarmTaxPaid = 2400,

	eLog_ItemCookUse,
	eLog_ItemCookSuccess,
	eLog_ItemVehicleToll,
	eLog_ItemHouseKeep,
	eLog_ItemHouseUnKepp,
	eLog_ItemUseHouseItem,

	// 090227 ShinJS --- Coin ItemÀÇ ¼ö·® º¯°æ/»èÁ¦
	eLog_CoinDecreaseByItemBuy,					// Coin º¯°æ»çÇ×(Update½Ã)
	eLog_CoinDestroyByItemBuy,					// Coin º¯°æ»çÇ×(Delete½Ã)
	// Æ®¸®°Å·Î ÀÎÇÑ ¾ÆÀÌÅÛ »ý¼º/Ãß°¡
	eLog_AddItemByTrigger,
	eLog_RemoveItemByTrigger,
	eLog_ItemChangeClass,


	// ÆÇ¸Å´ëÇà
	eLog_Consignment_Regist = 2412,			// ÆÇ¸Å´ëÇàµî·Ï
	// ¿ìÆíÇÔ
	eLog_Note_Consignmnet_Buy,			// ÆÇ¸Å´ëÇà±¸¸Å
	eLog_Note_Consignmnet_SellUpdate,	// ÆÇ¸Å´ëÇà°»½Å (ºÎºÐÆÇ¸Å½Ã ¿øº»¾ÆÀÌÅÛÀÇ ¼ö·®º¯°æ)
	eLog_Note_Consignmnet_Receipt,		// ÆÇ¸Å´ëÇà¿µ¼öÁõ
	eLog_Note_Consignmnet_UserCancel,	// ÆÇ¸Å´ëÇàÃë¼Ò(À¯Àú)
	eLog_Note_Consignmnet_TimeCancel,	// ÆÇ¸Å´ëÇàÃë¼Ò(±â°£)
	eLog_Note_CombineDelete,			// ¿ìÆí¹Þ±â»èÁ¦(¼ö·®¼º ±âÁ¸¾ÆÀÌÅÛ¿¡ ÇÕÃÄÁ®¼­ Á¦°ÅÇÔ)
	eLog_Note_Obtain_Success,			// ¿ìÆí¹Þ±âÁö±Þ(¾ÆÀÌÅÛ)
	eLog_Note_Obtain_Money,				// ¿ìÆí¹Þ±âÁö±Þ(°ñµå)
	eLog_Note_UserDelete,				// ¿ìÆíÀ¯Àú»èÁ¦
	eLog_Note_TimeDelete,				// ¿ìÆí±â°£»èÁ¦
	eLog_ItemVehicleSummon,
	eLog_ItemVehicleUnsummon,
	eLog_DungeonBegin,
	eLog_DungeonEnd,
	eLog_LimitDungeonBegin,
	eLog_LimitDungeonEnd,
	eLog_DateMatchBegin,
	eLog_DateMatchEnd,
	eLog_Note_Obtain_Fail,				// ¿ìÆí¹Þ±â½ÇÆÐ
	eLog_Note_SendItemFromNPC,			// NPC°¡ ¿ìÆí¾ÆÀÌÅÛ¹ß¼Û

	eLog_ItemUse_GTBUFFITEM,			// ½ºÅ³ÀÌ ½ÃÀüµÇ´Â ¾ÆÀÌÅÛ
	eLog_Max,
};


enum eGuildLog
{
	// member
	eGuildLog_MemberSecede = 1,
	eGuildLog_MemberBan,
	eGuildLog_MemberAdd,
	eGuildLog_StudentAdd,

	// master 
	eGuildLog_MasterChangeRank = 100,

	// guild
	eLogGuild_GuildCreate = 200,
	eGuildLog_GuildBreakUp,
	eGuildLog_GuildLevelUp,
	eGuildLog_GuildLevelDown,
	eGuildLog_Retire,
	eGuildLog_SkillAdd,

	// guildunion
	eGuildLog_CreateUnion = 400,
	eGuildLog_DestroyUnion,
	eGuildLog_AddUnion,
	eGuildLog_RemoveUnion,	
	eGuildLog_SecedeUnion,

	// Á¡¼ö
	eGuildLog_Score	= 500,
	eGuildLog_TimeScoreAdd,
	eGuildLog_ScoreAddByKill,		// ±æµåÀü »ó´ë Á¦°Å
	eGuildLog_ScoreRemoveByMurder,	// ±æµåÀü¿¡¼­ »ç¸Á
	eGuildLog_ScoreAddByWin,		// ±æµåÀü ½Â¸®
	eGuildLog_ScoreRemoveByDefeat,	// ±æµåÀü ÆÐ¹è
	eGuildLog_ScoreCheat,			// Ä¡Æ®
	eGuildLog_ScoreLevelUp,			// ·¹º§¾÷¿¡ »ç¿ë
	eGuildLog_ScoreSkillUse,		// 080602 LUJ, ½ºÅ³ ¹ßµ¿¿¡ ¼Ò¸ð
	eGuildLog_ScoreRemoveByGTFee,	// ±æµåÅä³Ê¸ÕÆ® µî·Ï¼Ò¸ð
	eGuildLog_ScoreAddByGTWin,		// ±æµåÅä³Ê¸ÕÆ® ½Â¸®º¸»ó
	eGuildLog_ScoreAddByItem,		// ±æµåÆ÷ÀÎÆ®È¹µæ ¾ÆÀÌÅÛ

	// gm Åø
	eGuildLog_GM	= 600,
	eGuildLog_GM_ChangeRank,
	eGuildLog_GM_KIck_Member,
	// 081027 LUJ, ±æµå Åä³Ê¸ÕÆ® ·Î±×
	eGuildLog_TournamentAdd,
};


enum eQuestLog
{
	eQuestLog_None	= 0,

	eQuestLog_GM	= 1,

	eQuestLog_User	= 100,
	eQuestLog_Main_Begin,
	eQuestLog_Sub_Begin,
	eQuestLog_Main_End,
	eQuestLog_Sub_End,
	eQuestLog_Main_Delete,
	eQuestLog_Sub_Delete,
	eQuestLOg_ItemTake,
	eQuestLOg_ItemGive,
	eQuestLOg_ItemUpdate,
};


enum eLogExppoint
{
	eExpLog_Time,
		
	eExpLog_LevelUp = 10,
	// 081027 LUJ, °æÇèÄ¡ È¹µæ
	eExpLog_Get,

	eExpLog_LosebyBadFame = 50, //reduce 
	eExpLog_LosebyReviveLogIn,
	eExpLog_LosebyRevivePresent,
	eExpLog_LosebyLooting,
	eExpLog_LosebyBreakupGuild,
	eExpLog_LosebyReviveExpProtect,
	eExpLog_LosebyReviveReviveOther,
	eExpLog_LosebyGuildFieldWar,
	eExpLog_LosebyBreakupFamily,
	// desc_hseos_ÆÐ¹Ð¸®01
	// S ÆÐ¹Ð¸® Ãß°¡ added by hseos 2007.10.11
	eExpLog_LosebyLeaveFamily,
	eExpLog_LosebyExpleFamilyMember,
	// E ÆÐ¹Ð¸® Ãß°¡ added by hseos 2007.10.11
	eExpLog_LoseByPKMode,
	// 080602 LUJ, ½ºÅ³ »ç¿ëÀ¸·Î ÀÎÇÑ °æÇèÄ¡ ¼Ò¸ð
	eExpLog_LoseBySkillUse,

	eExpLog_GetbyQuest = 100,
	eExpLog_SExpGetbyQuest = 101,
	eExpLog_GetbyItem = 102,
	
	eExpLog_ProtectExp = 150,
	eExpLog_ReviveExp,
};

enum eLogToolType	// Tool A¨ú¡¤u
{
	eToolLog_GM = 1,
	eToolLog_RM,
	eToolLog_Server,
};

enum eGMLogKind		// GMTool ¡¤I¡¾¡¿ A¨ú¡¤u
{
	eGMLog_LogIn = 1,
	eGMLog_Move,	
	eGMLog_Item,
	eGMLog_Money,
	eGMLog_Summon,
	eGMLog_SummonDelete,
	eGMLog_MonsterKill,
	eGMLog_PKAllow,
	eGMLog_Disconnect_Map,
	eGMLog_Disconnect_User,
	eGMLog_Block,
	eGMLog_Event,
	eGMLog_PlusTime,

	eGMLog_Skill,
	eGMLog_Ability,
	eGMLog_Chat_Map,
	eGMLog_Chat_User,
	eGMLog_PK,
	eGMLog_Regen,
};

enum eFishingLog
{
	eFishingLog_SetPointFromCheat = 1,	// Ä¡Æ®·ÎºÎÅÍ ¹°°í±âÆ÷ÀÎÆ® ¼³Á¤
	eFishingLog_GetPointFromFish,		// ¹°°í±â·ÎºÎÅÍ ¹°°í±âÆ÷ÀÎÆ® È¹µæ
	eFishingLog_UsePointForBuyItem,		// ¾ÆÀÌÅÛÀ» »ç±âÀ§ÇØ ¹°°í±âÆ÷ÀÎÆ® »ç¿ë
	eFishingLog_SetLevel,				// 080808 LUJ, ³¬½Ã ·¹º§ º¯°æ
	eFishingLog_Regular,				// 080808 LUJ, Á¤±â ·Î±× ÀúÀå
	eFishingLog_GetExpFromItem,
	eFishingLog_GetPointFromItem,
};

enum eCookingLog
{
	eCookingLog_CookCount = 1,			// ¿ä¸®ÇÏ±â
	eCookingLog_EatCount,				// ¿ä¸®¸Ô±â
	eCookingLog_FireCount,				// ¸ð´ÚºÒ ÇÇ¿ì±â

	eCookingLog_RecipeReg,				// ·¹½ÃÇÇ µî·Ï
	eCookingLog_RecipeDel_by_Time,		// ·¹½ÃÇÇ »èÁ¦ (½Ã°£Á¾·á ÀÚµ¿»èÁ¦)
	eCookingLog_RecipeDel_by_User,		// ·¹½ÃÇÇ »èÁ¦ (À¯Àú Á÷Á¢»èÁ¦)
};

enum eHousingLog
{
	eHousingLog_Keep = 1,			// º¸°ü
	eHougingLog_UnKeep,				// º¸°üÇØÁ¦
	eHougingLog_Install,			// ¼³Ä¡
	eHougingLog_UnInstall,			// ¼³Ä¡ÇØÁ¦
	eHougingLog_Destroy,			// ÆÄ±«
	eHougingLog_EndTime,			// »ç¿ë½Ã°£Á¾·á

	eHougingLog_WallPaper,			// º®Áö»ç¿ë
	eHougingLog_FloorPaper,			// ¹Ù´ÚÁö»ç¿ë
	eHougingLog_CeilingPaper,		// ÃµÀåÁö»ç¿ë
	eHougingLog_Extend,				// È®Àå
	eHougingLog_Visit,				// ¹æ¹®
	eHougingLog_Exterior,			// ¿ÜÇü»ç¿ë
};

// 081028 LUJ, °ø¼º ·Î±×. ÀÌÈÄ 
enum eSiegeLog
{
	eSiegeLogNone,
	eSiegeLogChangeState,
	eSiegeLogDestoryCatapult,
	eSiegeLogGetCastle,
	eSiegeLogUseSeed,
	eSiegeLogSetSeedSucceed,
	eSiegeLogSetSeedFailed,
	eSiegeLogComplete,
};


// 080716 LUJ, Æê ·Î±×
enum ePetLog
{
	ePetLogSummon,
	ePetLogDie,
	ePetLogRevive,
	ePetLogChangeGrade,
	ePetLogChangeLevel,
	ePetLogRegular,		// 080716 LUJ, ÀÏÁ¤ °£°ÝÀ¸·Î »óÅÂ ÀúÀå
};


// ±æµåÅä³Ê¸ÕÆ® ·Î±×
enum eGTLogKind
{
	eGTLogKind_None=0,
	eGTLogKind_Regist,				// µî·Ï
	eGTLogKind_Win,					// ½Â
	eGTLogKind_Lose,				// ÆÐ
};


// °ø¼ºÀü ·Î±×
enum eSWLogKind
{
	eSWLogKind_None=0,
	eSWLogKind_DefenceProposal,			// ¼ö¼ºÂü¿© ½ÅÃ»
	eSWLogKind_Attack,					// °ø¼º¹®ÆÄ
	eSWLogKind_Defence,					// ¼ö¼ºÃø ¹®ÆÄ
	eSWLogKind_DefenceAccept,			// ¼ö¼ºÂü¿© Çã¶ôÀ¸·Î ¼ö¼º¹®ÆÄ Âü¿©
	eSWLogKind_DefenceUnion,			// µ¿¸Í°á¼ºÀ¸·Î ¼ö¼º¹®ÆÄ Âü¿©
	eSWLogKind_CancelByCastleGuild,		// ¼º¹®ÆÄ°¡ µ¿¸ÍÅ»Åð·Î ´Ù¸¥¼º¹®ÆÄÀÇ ¼ö¼ºÂü¿© Ãë¼Ò
	eSWLogKind_CancelByUnionSecession,	// ¼º¹®ÆÄ¿ÍÀÇ µ¿¸Í Å»Åð·Î ¼ö¼ºÂü¿© Ãë¼Ò
	eSWLogKind_StartSiegeWar,			// °ø¼º½ÃÀÛ
	eSWLogKind_SucceedCastleGuild,		// °ø¼º¼º°øÀ¸·Î ¼ºÀ» Â÷ÁöÇÑ ¹®ÆÄ
	eSWLogKind_SucceedMoveToDefence,	// °ø¼º¼º°øÀ¸·Î ¼ºÀ» Â÷ÁöÇÑ ¹®ÆÄÀÇ µ¿¸ÍÀº ¼ö¼ºÀ¸·Î ÀÌµ¿
	eSWLogKind_SucceedAttack,			// °ø¼º¼º°ø °ø¼ºÀº °ø¼º ±×´ë·ÎÀÎ ¹®ÆÄ
	eSWLogKind_SucceedMoveToAttack,		// °ø¼ºÃøÀÇ °ø¼º¼º°øÀ¸·Î ¼ö¼º->°ø¼ºÀ¸·Î ¿Å°Ü°£ ¹®ÆÄ
	eSWLogKind_EndCastleGuild,			// °ø¼ºÁ¾·áÈÄ ¼ºÀ» Â÷ÁöÇÑ ¹®ÆÄ
	eSWLogKind_EndDefenceUnionGuild,	// °ø¼ºÁ¾·áÈÄ µ¿¸ÍÀÌ¾î¼­ ¼ö¼ºÀ¸·Î µî·ÏµÇ´Â ¹®ÆÄ
	eSWLogKind_EngraveSyn,				// °¢ÀÎ½Ãµµ
	eSWLogKind_EngraveNack,				// °¢ÀÎ½ÇÆÐ ¸Â¾Æ Á×À½
	eSWLogKind_EngraveNackCancel,		// °¢ÀÎ½ÇÆÐ Ãë¼Ò
	eSWLogKind_EngraveNackLogOut,		// °¢ÀÎ½ÇÆÐ ·Î±×¾Æ¿ô
	eSWLogKind_SiegeWarSucceed,			// °¢ÀÎ¼º°ø
	eSWLogKind_EndSiegeWar,				// °ø¼ºÁ¾·á
};

// 090609 ShinJS --- AutoNote Log Ãß°¡
enum eAutoNoteLogKind
{
	eAutoNoteLogKind_None,
	eAutoNoteLogKind_Report,			// ½Å°í
	eAutoNoteLogKind_Regist,			// Auto µî·Ï
	eAutoNoteLogKind_Ban,				// Ban µî·Ï
};

#define MAX_KIND_PERMAP		150

enum
{
	ePreLoad_Monster,
	ePreLoad_Item,

	ePreLoad_Max,
};


struct sPRELOAD_INFO
{
	int		MapNum;
	int		Count[ePreLoad_Max];
	int		Kind[ePreLoad_Max][MAX_KIND_PERMAP];

	sPRELOAD_INFO()
	{
		MapNum		= 0;
		memset(Count, 0, sizeof(int)*ePreLoad_Max);
		memset(Kind, 0, sizeof(int)*ePreLoad_Max*MAX_KIND_PERMAP);
	}
};

#define MAX_GUILD_NAME 16
#define MAX_GUILD_INTRO		60
// 06. 03. ¹®ÆÄ°øÁö - ÀÌ¿µÁØ
#define MAX_GUILD_NOTICE	150
#define MAX_GUILD_UNION		7
#define MAX_GUILD_NICKNAME	16
#define GUILD_MASTER		60
#define GUILD_VICEMASTER	50
#define GUILD_SENIOR		40
#define GUILD_JUNIOR		30
#define GUILD_MEMBER		20
#define GUILD_STUDENT		10
#define GUILD_NOTMEMBER		0

#define GUILD_STUDENT_MAX_LEVEL	40
#define MAX_GUILD_MEMBER	80 //60 + 20(¼ö·Ã»ý)
#define MAX_SHOUT_LENGTH 60

enum
{
	eMode_NormalNote = 0,
	eMode_StoredNote,
};

#define MAXGUILD_INTOURNAMENT			32		//±æµåÅä³Ê¸ÕÆ® ÃÖ´ëÂü°¡ÆÀ
#define MAX_GTOURNAMENT_PLAYER			14		//±æµåÅä³Ê¸ÕÆ® Âü°¡¼±¼ö
#define GT_INVALID_BATTLEID				99999	//±æµåÅä³Ê¸ÕÆ® ¹èÆ²ID°¡ ¾øÀ»°æ¿ì

enum
{
	eGTError_NOERROR=0,					// No Error
	eGTError_NOTREGISTDAY,				// Not a Regist-day
	eGTError_NOTREGISTCANCELDAY,		// Not a Registcancel-day
	eGTError_DONTFINDGUILDINFO,			// Don't find a GuildInfo
	eGTError_MAXGUILDOVER,				// Over MAXGUILD_INTOURNAMENT
	eGTError_ALREADYREGISTE,			// Already Registed GuildTournament
	eGTError_ALREADYCANCELREGISTE,		// Already Cancel Registed GuildTournament
	eGTError_UNDERLIMITEMEMBER,			// Under LIMITMEMBER_GUILDTOURNAMENT
	eGTError_UNDERLEVEL,				// Under LIMITLEVEL_GUILDTOURNAMENT
	eGTError_DELETEAPPLIED,				// CancelError Regited GuildTournament
	eGTError_NOGUILDMASTER,				// Not a GuildMaster
	eGTError_DONTFINDPOSITION,			// GuildTournament-Position Find Error
	eGTError_DONTFINDBATTLE,			// Don't find a Battle	
	eGTError_BATTLEJOINFAIL,			// Failed a join in Battle
	eGTError_NOTENTERTIME,				// Not a time of enter the battle;
	eGTError_STUDENTCANTENTER,			// a guild student can't enter the battle
	eGTError_FAILTOREGIST,				// Fail to Regist
	eGTError_INVALIDROUND,
	eGTError_ALREADYSETMATCH,
	eGTError_NOTALLOWOBSERVER,
	eGTError_INVALIDDAY,
	eGTError_READYTONEXTMATCH,
	eGTError_NOTENOUGHFEE,
	eGTError_ERROR,
};

enum
{
	eGTState_BeforeRegist=0,
	eGTState_Regist,
	eGTState_BeforeEntrance,
	eGTState_Entrance,
	eGTState_Process,
	eGTState_Leave,

	eGTState_AutoSchedule,
	
	eGTState_Max,
};

enum
{
	eGTStanding_1ST = 1,
	eGTStanding_2ND = 2,
	eGTStanding_3RD = 3,
	eGTStanding_4TH = 4,
	eGTStanding_8TH = 8,
	eGTStanding_16TH = 16,
	eGTStanding_32TH = 32,
};

enum
{
	eGTFight_32		= 0,
	eGTFight_16		= 1,
	eGTFight_8		= 2,
	eGTFight_4		= 3,
	eGTFight_2		= 4,
	eGTFight_1		= 5,
	eGTFight_End,

	eGTT_MAX,
};

enum
{
	eGTNotify_RegistStart = 0,
	eGTNotify_RegistEnd,
	eGTNotify_TournamentStart,
	eGTNotify_TournamentEnd,
	eGTNotify_RoundStartforPlayer,
	eGTNotify_RoundStart,
	eGTNotify_RoundEnd,
	
	eGTNotify_EnteranceRemain,
	eGTNotify_RegistRemain,
	eGTNotify_BattleTableOpen,

	eGTNotify_WinLose,

	eGTNotify_Max,
};

enum
{
	eGTDay_SUNDAY=0,
	eGTDay_MONDAY,
	eGTDay_TUESDAY,
	eGTDay_WEDNESDAY,
	eGTDay_THURSDAY,
	eGTDay_FRIDAY,
	eGTDay_SATURDAY,
};

enum
{
	eGTWin_None = 0,
	eGTWin_Unearned,		// ºÎÀü½Â
	eGTWin_RemainPlayer,	// ³²Àº»ç¶÷¼ö
	eGTWin_LowLevel,		// ·¹º§ÇÕÀÌ ³·´Ù
	eGTWin_LowExp,			// °æÇèÄ¡°¡ ³·´Ù.
};

enum
{
	eNpcParam_ShowpStorage=1,
};

#define ITEM_PARAM_PRESENT	0x00000100

enum eITEM_PARAM
{
	eITEM_PARAM_NORMAL,
	eITEM_PARAM_SEAL,
	eITEM_PARAM_UNSEAL,
};

// 070414 LYW --- CommonGameDefine : Modified eCharacterStage.
enum eCharacterStage
{
	eStage_H_Fighter			= 1111,	// ÆÄÀÌÅÍ
	eStage_H_Guard				= 1121,	// °¡µå
	eStage_H_Warrior			= 1122,	// Àü»ç
	eStage_H_InfantryMan		= 1131,	// º¸º´
	eStage_H_SwordMan			= 1132,	// ¼Òµå¸Ç
	eStage_H_Mercenary			= 1133,	// ¿ëº´
	eStage_H_Phalanx			= 1141, // ÆÈ¶ûÅ©½º
	eStage_H_Knight				= 1142, // ±â»ç
	eStage_H_Gladiator			= 1143, // °ËÅõ»ç
	eStage_H_RunicKnight		= 1144, // ·é ³ªÀÌÆ®
	eStage_H_Paladin			= 1151, // ÆÓ·¯µò
	eStage_H_Panzer				= 1152, // ±â°©ºÎ´ë¿ø
	eStage_H_Crusader			= 1153, // ½ÊÀÚ±ºÀü»ç
	eStage_H_Destroyer			= 1154, // ÆÄ±«ÀÚ
	eStage_H_SwordMaster		= 1155, // ¼Òµå¸¶½ºÅÍ
	eStage_H_Magners			= 1156, // ¸Å±×³Ê½º
	eStage_H_Lord				= 1161, // ·Îµå
	eStage_H_DeathKNight		= 1162, // µ¥½º³ªÀÌÆ®
	eStage_H_ArcTemplar			= 1163, // ¾ÆÅ©ÅÛÇÃ·¯

	eStage_E_Fighter			= 1211,	// ÆÄÀÌÅÍ
	eStage_E_Guard				= 1221,	// °¡µå
	eStage_E_Warrior			= 1222,	// Àü»ç
	eStage_E_InfantryMan		= 1231,	// º¸º´
	eStage_E_SwordMan			= 1232,	// ¼Òµå¸Ç
	eStage_E_Mercenary			= 1233,	// ¿ëº´
	eStage_E_Phalanx			= 1241, // ÆÈ¶ûÅ©½º
	eStage_E_Knight				= 1242, // ±â»ç
	eStage_E_Gladiator			= 1243, // °ËÅõ»ç
	eStage_E_RunicKnight		= 1244, // ·é ³ªÀÌÆ®
	eStage_E_Paladin			= 1251, // ÆÓ·¯µò
	eStage_E_Panzer				= 1252, // ±â°©ºÎ´ë¿ø
	eStage_E_Crusader			= 1253, // ½ÊÀÚ±ºÀü»ç
	eStage_E_Destroyer			= 1254, // ÆÄ±«ÀÚ
	eStage_E_SwordMaster		= 1255, // ¼Òµå¸¶½ºÅÍ
	eStage_E_Magners			= 1256, // ¸Å±×³Ê½º
	eStage_E_Lord				= 1261, // ·Îµå
	eStage_E_DeathKNight		= 1262, // µ¥½º³ªÀÌÆ®
	eStage_E_ArcTemplar			= 1263, // ¾ÆÅ©ÅÛÇÃ·¯
	

	eStage_H_Rogue				= 2111, // ¹æ¶ûÀÚ
	eStage_H_Voyager			= 2121, // Ç×ÇØÀÚ
	eStage_H_Ruffian			= 2122, // ¹«¹ýÀÚ
	eStage_H_Archer				= 2131, // ±Ã¼ö
	eStage_H_Chief				= 2132, // ½ÃÇÁ
	eStage_H_Scout				= 2133, // Á¤Âûº´
	eStage_H_Ranger				= 2141, // ºÎ´ë¿ø
	eStage_H_TreasureHunter 	= 2142, // º¸¹°»ç³É²Û
	eStage_H_Assassin			= 2143, // ¾Ï»ìÀÚ
	eStage_H_RunicWorker		= 2144, // ·é¿öÄ¿
	eStage_H_Sniper				= 2151, // Àú°Ýº´
	eStage_H_Intraper			= 2152, // 
	eStage_H_BladeTaker			= 2153, //
	eStage_H_TemperMaster		= 2154, //

	// 080709 LYW --- CommonGameDefine : ¾ÆÅ©·¹ÀÎÀú Á÷¾÷À» Ãß°¡ÇÑ´Ù.
	eState_H_ArcRanger			= 2155,	// ¾ÆÅ©·¹ÀÎÀú

	eStage_H_SoulEye			= 2161, //
	eStage_H_BloodStalker		= 2162, //
	eStage_H_ArcBridger			= 2163, //

	eStage_E_Rogue				= 2211, // ¹æ¶ûÀÚ
	eStage_E_Voyager			= 2221, // Ç×ÇØÀÚ
	eStage_E_Ruffian			= 2222, // ¹«¹ýÀÚ
	eStage_E_Archer				= 2231, // ±Ã¼ö
	eStage_E_Chief				= 2232, // ½ÃÇÁ
	eStage_E_Scout				= 2233, // Á¤Âûº´
	eStage_E_Ranger				= 2241, // ºÎ´ë¿ø
	eStage_E_TreasureHunter 	= 2242, // º¸¹°»ç³É²Û
	eStage_E_Assassin			= 2243, // ¾Ï»ìÀÚ
	eStage_E_RunicWorker		= 2244, // ·é¿öÄ¿
	eStage_E_Sniper				= 2251, // Àú°Ýº´		
	eStage_E_Intraper			= 2252, // 
	eStage_E_BladeTaker			= 2253, //
	eStage_E_TemperMaster		= 2254, //

	// 080709 LYW --- CommonGameDefine : ¾ÆÅ©·¹ÀÎÀú Á÷¾÷À» Ãß°¡ÇÑ´Ù.
	eState_E_ArcRanger			= 2255,	// ¾ÆÅ©·¹ÀÎÀú

	eStage_E_SoulEye			= 2261, //
	eStage_E_BloodStalker		= 2262, //
	eStage_E_ArcBridger			= 2263, //

	eStage_H_Mage				= 3111, //
	eStage_H_Cleric				= 3121, //
	eStage_H_Wiard				= 3122, //
	eStage_H_Priest				= 3131, //
	eStage_H_Sorcerer			= 3132, //
	eStage_H_Monk				= 3133, //
	eStage_H_Bishop				= 3141, //
	eStage_H_Warlock			= 3142, //
	eStage_H_Inquirer			= 3143, //
	eStage_H_ElementalMaster	= 3144, //
	eStage_H_Cardinal			= 3151, //
	eStage_H_SoulArbiter		= 3152, //
	eStage_H_GrandMaster		= 3153, //
	eStage_H_Necromancer		= 3154, //
	eStage_H_RunicMaster		= 3155, //
	eStage_H_Saint				= 3161, //
	eStage_H_DarkArchon			= 3162, //
	eStage_H_ArcCelebrant		= 3163, //

	eStage_E_Mage				= 3211, //
	eStage_E_Cleric				= 3221, //
	eStage_E_Wiard				= 3222, //
	eStage_E_Priest				= 3231, //
	eStage_E_Sorcerer			= 3232, //
	eStage_E_Monk				= 3233, //
	eStage_E_Bishop				= 3241, //
	eStage_E_Warlock			= 3242, //
	eStage_E_Inquirer			= 3243, //
	eStage_E_ElementalMaster	= 3244, //
	eStage_E_Cardinal			= 3251, //
	eStage_E_SoulArbiter		= 3252, //
	eStage_E_GrandMaster		= 3253, //
	eStage_E_Necromancer		= 3254, //
	eStage_E_RunicMaster		= 3255, //
	eStage_E_Saint				= 3261, //
	eStage_E_DarkArchon			= 3262, //
	eStage_E_ArcCelebrant		= 3263, //

};

enum eGuildUnion_Err
{
	eGU_Not_ValidName,
	eGU_Not_InGuild,
	eGU_Not_GuildMaster,
	eGU_Not_Level,
	eGU_Not_GuildUnionMaster,
	eGU_Not_InGuildUnion,
	eGU_Aleady_InGuildUnion,
	eGU_Time,
	eGU_Other_Not_InGuild,
	eGU_Other_Not_InGuildUnion,
	eGU_No_Lone_Union,
	eGU_Other_Not_GuildMaster,
	eGU_Other_Aleady_InGuildUnion,
	eGU_Other_Time,
	eGU_Is_Full,	
	eGU_Is_GuildFieldWar,
	eGU_Not_ValidMap,
};

// ±â»ó»óÅÂ
enum eWeatherState
{
	//eWS_None,	// 0
	//eWS_Snow,	// 1
	eWS_Clear,	// ¸¼À½
	eWS_Cloudy,	// Èå¸²
	eWS_Rainy,	// ºñ
	eWS_Windy,	// ¹Ù¶÷
	eWS_Foggy,	// ¾È°³
	eWS_Typoon,	// ÅÂÇ³

	eWS_Max,	
};

// 080410 NYJ --- ³¬½Ã°ü·Ã ¿¡·¯ÄÚµå 
enum eFishingErrorCode
{
	eFishingError_None = 0,		// NotError, Success
	eFishingError_InvaildHero,	// À¯È¿ÇÏÁö ¾ÊÀº ÇÃ·¹ÀÌ¾î
	eFishingError_InvaildPlace,	// À¯È¿ÇÏÁö ¾ÊÀº ³¬½ÃÅÍ
	eFishingError_NoPole,		// ³¬½Ã´ë ¹ÌÂø¿ë
	eFishingError_NoBait,		// ¹Ì³¢ ¹Ì»ç¿ë
	eFishingError_OverTime,		// ½Ã°£ÃÊ°ú
	eFishingError_UserCancel,	// À¯ÀúÀÇ Ãë¼Ò¿äÃ»
	eFishingError_InvenFull,	// ÀÎº¥ÀÌ °¡µæÂù»óÅÂ
	eFishingError_InvalidGrade,	// ¼÷·Ãµî±Þ ºÒÀÏÄ¡
	eFishingError_EarlyTime,	// ½Ã°£¹ÌÃæÁ·
	eFishingError_InvaildState,	// À¯È¿ÇÏÁö ¾ÊÀº »óÅÂ
	eFishingError_OverMaxFishPoint,	// ÃÖ´ë ¹°°í±âÆ÷ÀÎÆ® ÃÊ°ú
	eFishingError_FailFishPoint,// ¹°°í±âÆ÷ÀÎÆ®º¯È¯ ½ÇÆÐ
	eFishingError_OverDistance,	// °Å¸®ÃÊ°ú
};

// 080414 NYJ --- ³¬½Ã°ü·Ã ¹°°í±â¾ÆÀÌÅÛÅ¸ÀÔ
enum eFishItemType
{
	eFishItem_Poor = 0,		// [0]ÇÇ¶ó¹Ì
	eFishItem_Common,		// [1]ºØ¾î
	eFishItem_UnCommon,		// [2]À×¾î
	eFishItem_Rare,			// [3]È²±ÝÀ×¾î, È²±ÝÀÚ¶ó
	eFishItem_Unique,		// [4]ºû³ª´Â ¾²·¹±â
	eFishItem_Event,		// [5]Ä£È¯°æ¹Ì³¢
	eFishItem_Reserve1,		// [6]¿¹¾à1
	eFishItem_Reserve2,		// [7]¿¹¾à1
	eFishItem_Reserve3,		// [8]¿¹¾à1
	eFishItem_Reserve4,		// [9]¿¹¾à1
	
	eFishItem_Max,
};

enum eCookErrorCode
{
	eCookError_None = 0,		// NotError, Success
	eCookError_InvaildRecipe,	// À¯È¿ÇÏÁö ¾ÊÀº ·¹½ÃÇÇ
	eCookError_InvaildFire,		// À¯È¿ÇÏÁö ¾ÊÀº ¸ð´ÚºÒ
	eCookError_InvaildMakeNum,	// À¯È¿ÇÏÁö ¾ÊÀº Á¦ÀÛ¼ö·®
	eCookError_InvaildUtil,		// À¯È¿ÇÏÁö ¾ÊÀº ¿ä¸®µµ±¸
	eCookError_LowerUtil,		// ·¹½ÃÇÇº¸´Ù µî±ÞÀÌ ³·Àº ¿ä¸®µµ±¸
	eCookError_LowerExpertPoint,// ·¹½ÃÇÇº¸´Ù Á¡¼ö°¡ ³·Àº ¼÷·Ãµµ
	eCookError_InvalidGrade,	// ¼÷·Ãµî±Þ ºÒÀÏÄ¡
	eCookError_InvaildState,	// À¯È¿ÇÏÁö ¾ÊÀº »óÅÂ
	eCookError_OverDistance,	// °Å¸®ÃÊ°ú
	eCookError_InvenFull,		// ÀÎº¥ÀÌ °¡µæÂù»óÅÂ
	eCookError_NeedIngredient,	// Àç·áºÎÁ·
};

enum PART_TYPE
{
	ePartType_None,
	ePartType_OneHand,
	ePartType_TwoHand,
	ePartType_TwoBlade,
	ePartType_Shield,

	ePartType_Body,
	ePartType_Hand = 7,
	ePartType_Foot,

	ePartType_Hair,
	ePartType_Face,
	ePartType_Head,

	ePartType_Helmet = 15,
	ePartType_Glasses,
	ePartType_Mask,

	ePartType_Head_Chicken = 20,
	ePartType_Head_Panda,
	ePartType_Head_Tights,
	ePartType_Head_Bag,
	ePartType_Head_Mask,
	//090506 pdy ÆÄÃ÷ Ãß°¡ 
	ePartType_Head_CatEar,		

	ePartType_Costume_Body = 30,
	ePartType_Costume_Foot,
	ePartType_Costume_Hand,

	ePartType_Wing = 40,
	ePartType_Wing2,

	//100406 pdy ÆÄÃ÷Ãß°¡ : Bip01 Pelvis¿¡ Attach
	ePartType_Pelvis,

	//090506 pdy ÆÄÃ÷Ãß°¡ : Bip01¿¡ Attach
	ePartType_Tail	= 50, 


	ePartType_Pet_Face = 70,
	ePartType_Pet_Head,
	ePartType_Pet_Wing,
	ePartType_Pet_Tail,
	
	ePartType_Job_Effect = 100,

	ePartType_Max,
};

enum	DATE_MATCHING_REGION				// µ¥ÀÌÆ® ¸ÅÄª ½Ã½ºÅÛ¿¡¼­ »ç´Â Áö¿ª ¼±¾ð. ·ÎÄÃ¶óÀÌÂ¡ ÇÏ·Á¸é ÆÄÀÏÇü½ÄÀ¸·Î ¹ÛÀ¸·Î »©¾ßÇÒ µí.?
{
	eDATE_MATCHING_REGION_KYUNGKI,			// °æ±â,
	eDATE_MATCHING_REGION_SEOUL,			// ¼­¿ï
	eDATE_MATCHING_REGION_INCHON,			// ÀÎÃµ
	eDATE_MATCHING_REGION_KANGWON,			// °­¿ø
	eDATE_MATCHING_REGION_CHUNGNAM,			// Ãæ³²
	eDATE_MATCHING_REGION_CHUNGBUK,			// ÃæºÏ
	eDATE_MATCHING_REGION_DAEJEON,			// ´ëÀü
	eDATE_MATCHING_REGION_KYUNGBUK,			// °æºÏ
	eDATE_MATCHING_REGION_KYUNGNAM,			// °æ³²
	eDATE_MATCHING_REGION_DAEGU,			// ´ë±¸
	eDATE_MATCHING_REGION_BUSAN,			// ºÎ»ê
	eDATE_MATCHING_REGION_ULSAN,			// ¿ï»ê
	eDATE_MATCHING_REGION_JEONBUK,			// ÀüºÏ
	eDATE_MATCHING_REGION_JEONNAM,			// Àü³²
	eDATE_MATCHING_REGION_KWANGJU,			// ±¤ÁÖ
	eDATE_MATCHING_REGION_JEJU,				// Á¦ÁÖ
	eDATE_MATCHING_REGION_OVERSEAS,			// ÇØ¿Ü
};

enum	DATE_MATCHING_FEELING				// µ¥ÀÌÆ® ¸ÅÄª ½Ã½ºÅÛÀÇ ¼ºÇâ.
{
	eDATE_MATCHING_FEELING_BLACKHAIR,		// °ËÀº ¸Ó¸®
	eDATE_MATCHING_FEELING_DYEDHAIR,			// ¿°»öÇÑ ¸Ó¸®
	eDATE_MATCHING_FEELING_FORMALDRESS,		// Á¤Àå
	eDATE_MATCHING_FEELING_CASUALWEAR,		// Ä³ÁÖ¾ó º¹Àå
	eDATE_MATCHING_FEELING_MAKEUP,			// È­Àå
	eDATE_MATCHING_FEELING_PERFUME,			// Çâ¼ö
	eDATE_MATCHING_FEELING_THIN,				// ¸¶¸§
	eDATE_MATCHING_FEELING_FAT,				// ¶×¶×ÇÔ
	eDATE_MATCHING_FEELING_BEARD,			// ¼ö¿°
	eDATE_MATCHING_FEELING_EYEGLASS,			// ¾È°æ
	eDATE_MATCHING_FEELING_HAT,				// ¸ðÀÚ
	eDATE_MATCHING_FEELING_ACCESSORIES,		// ¾×¼¼¼­¸®
};

enum eStatusKind
{
	eStatusKind_None,
	eStatusKind_Str,
	eStatusKind_Dex,
	eStatusKind_Vit,
	eStatusKind_Int,
	eStatusKind_Wis,
	eStatusKind_All,
	eStatusKind_PhysicAttack = 11,
	eStatusKind_PhysicDefense,
	eStatusKind_MagicAttack,
	eStatusKind_MagicDefense,
	eStatusKind_Accuracy,
	eStatusKind_Avoid,
	eStatusKind_CriticalRate,
	eStatusKind_Range,
	eStatusKind_MagicCriticalRate,
	eStatusKind_CriticalDamage = 22,
	eStatusKind_MoveSpeed,
	eStatusKind_Block,
	eStatusKind_CoolTime,
	eStatusKind_CastingProtect,

	eStatusKind_Hide,
	eStatusKind_Detect,

	eStatusKind_IncreaseDropRate = 29,
	eStatusKind_DecreaseManaRate = 30,
	eStatusKind_MaxLife = 31,
	eStatusKind_MaxMana,
	eStatusKind_LifeRecoverRate,
	eStatusKind_ManaRecoverRate,
	eStatusKind_LifeRecover,
	eStatusKind_ManaRecover,
	eStatusKind_Poison = 40,
	eStatusKind_Paralysis,
	eStatusKind_Stun,
	eStatusKind_Slip,
	eStatusKind_Freezing,
	eStatusKind_Stone,
	eStatusKind_Silence,
	eStatusKind_BlockAttack,
	eStatusKind_God,
	eStatusKind_Shield,
	eStatusKind_Bleeding,
	eStatusKind_Burning,
	eStatusKind_HolyDamage,
	eStatusKind_MoveStop,
	eStatusKind_Attract,
	// 090204 LUJ, ÇÇ°Ý ½Ã MP¸¦ °¨¼Ò½ÃÅ²´Ù
	eStatusKind_ManaBurn,

	eStatusKind_Reflect = 61,
	eStatusKind_Absorb,
	eStatusKind_DamageToLife,
	eStatusKind_DamageToMana,
	eStatusKind_GetLife,
	eStatusKind_GetMana,
	eStatusKind_GetExp,
	eStatusKind_GetGold,
	// 071217 KTH --- °æÇèÄ¡ º¸È£
	eStatusKind_ProtectExp = 70,
	eStatusKind_Attrib_None = 71,
	eStatusKind_Attrib_Earth,
	eStatusKind_Attrib_Water,
	eStatusKind_Attrib_Divine,
	eStatusKind_Attrib_Wind,
	eStatusKind_Attrib_Fire,
	eStatusKind_Attrib_Dark,
	eStatusKind_Sword = 81,
	eStatusKind_Mace,
	eStatusKind_Axe,
	eStatusKind_Staff,
	eStatusKind_Bow,
	eStatusKind_Gun,
	eStatusKind_Dagger,
	eStatusKind_Spear,
	eStatusKind_TwoHanded,
	eStatusKind_TwoBlade,
	eStatusKind_OneHanded,

	eStatusKind_RobeArmor,
	eStatusKind_LightArmor,
	eStatusKind_HeavyArmor,
	eStatusKind_ShieldArmor,
	eStatusKind_BothDagger,
	eStatusKind_FishingPole,
	eStatusKind_CookUtil,
	
	eStatusKind_NormalSpeedRate = 100,
	eStatusKind_PhysicSkillSpeedRate,
	eStatusKind_MagicSkillSpeedRate,	
	eStatusKind_NormalPhysicSkillSpeedRate = 105,
	// 080616 LUJ, 5Â÷ ½ºÅ³ Ãß°¡
	eStatusKind_ConvertHealthWithGivedDamage	= 106,
	eStatusKind_ConvertManaWithGivedDamage		= 107,
	eStatusKind_EventSkillToVictim				= 108,
	eStatusKind_EventSkillToKiller				= 109,
	// 080703 LUJ, ÀÌµµ·ù »ç¿ë °¡´É
	eStatusKind_EnableTwoBlade					= 110,
	// 081203 LUJ, ¾ÆÀÌÅÛ »ç¿ë °¡´É ¿©ºÎ
	eStatusKind_UnableUseItem					= 111,
	// 081203 LUJ, ¹öÇÁ »ç¿ë °¡´É ¿©ºÎ
	eStatusKind_UnableBuff						= 112,
	// 071204 KTH -- °æÇèÄ¡ Áõ°¡ %
	eStatusKind_IncreaseExpRate = 120,
	// 100221 ShinJS --- µ¥¹ÌÁö¸¦ ¸¶³ª µ¥¹ÌÁö·Î ÀüÈ¯½ÃÅ²´Ù
	eStatusKind_DamageToManaDamage = 121,

	// Push ±â´ÉÃß°¡
	eStatusKind_Push_Target,
	eStatusKind_Push_Operator,
	eStatusKind_PushPull_Target,
	eStatusKind_PushPull_Operator,

	eStatusKind_Max,
};

enum eSkillTargetKind
{
	eSkillTargetKind_None,
	eSkillTargetKind_OnlyOthers,
	eSkillTargetKind_Ground,
	eSkillTargetKind_OnlySelf,
	eSkillTargetKind_AllObject,
};

// 080602 LUJ, ½ºÅ³ Å¸°Ù ´ë»ó. Target.h¿¡¼­ ¿Å°Ü¿È
enum TARGET_KIND
{
	TARGET_KIND_SINGLE,
	TARGET_KIND_MULTI,
	TARGET_KIND_SINGLE_CIRCLE,
	TARGET_KIND_MULTI_CIRCLE,
	TARGET_KIND_MULTI_RECT,
	TARGET_KIND_MULTI_ARC,
	// 080602 LUJ, ¼¼°è ¹üÀ§ ½ºÅ³
	TARGET_KIND_WORLD,
	// 080708 LUJ, ¸ó½ºÅÍ ´ë»ó ¹üÀ§ ½ºÅ³
	TARGET_KIND_MULTI_MONSTER,
	// 090204 LUJ, ÀüÃ¼(¸ó½ºÅÍ/ÇÃ·¹ÀÌ¾î) ´ë»ó ¹üÀ§ ½ºÅ³
	TARGET_KIND_MULTI_ALL,
	TARGET_KIND_MAX,
};

// 080602 LUJ, ½ºÅ³ ¹üÀ§ À¯Çü Å¸°Ù. Target.h¿¡¼­ ¿Å°Ü¿È
enum eSkillAreaTarget
{
	eSkillAreaTargetNone,
	eSkillAreaTargetMonster,
	eSkillAreaTargetParty,
	eSkillAreaTargetFamily,
	eSkillAreaTargetGuild,
	eSkillAreaTargetAll,
	eSkillAreaTargetAllExceptMe,
};

// 080602 LUJ, ½ºÅ³ °á°ú. Å¬¶óÀÌ¾ðÆ®¿¡ ½ÇÆÐ ÀÌÀ¯¸¦ ³ªÅ¸³»±â À§ÇØ [cc]skill/server/manager/skillManager.h¿¡¼­ ¿Å°Ü¿È
enum SKILL_RESULT
{
	SKILL_SUCCESS = 0,
	SKILL_OPERATOR_INVALUED,
	SKILL_INFO_INVALUED,
	SKILL_LEVEL_INVALUED,
	SKILL_STATE_INVALUED,
	SKILL_TARGET_INVALUED,
	SKILL_TARGET_IMMORTALED,
	SKILL_RANGE_NOTENOUGH,
	SKILL_COOLING,
	SKILL_FAIL_ETC,
	SKILL_DESTROY,
};

enum eBuffSkillActiveRule
{
	eBuffSkillActiveRule_Active,
	eBuffSkillActiveRule_Delete,
	eBuffSkillActiveRule_Stop,
};

// 090204 LUJ, ¼Ó¼º º¯°æ
enum eBuffSkillCountType
{
	eBuffSkillCountType_None,
	eBuffSkillCountType_Dead,
	// 090204 LUJ, ¸ðµç À¯ÇüÀÇ ÇÇÇØ
	eBuffSkillCountType_AnyDamageVolumn = 10,
	eBuffSkillCountType_AnyDamage,
	eBuffSkillCountType_AnyHitVolumn,
	eBuffSkillCountType_AnyHit,
	// 090204 LUJ, ¹°¸® ÇÇÇØ
	eBuffSkillCountType_PhysicalDamageVolumn = 20,
	eBuffSkillCountType_PhysicalDamage,
	eBuffSkillCountType_PhysicalHitVolumn,
	eBuffSkillCountType_PhysicalHit,
	// 090204 LUJ, ¸¶¹ý ÇÇÇØ
	eBuffSkillCountType_MagicalDamageVolumn = 30,
	eBuffSkillCountType_MagicalDamage,
	eBuffSkillCountType_MagicalHitVolumn,
	eBuffSkillCountType_MagicalHit,
	// 090204 LUJ, Èú
	eBuffSkillCountType_HealVolumn = 40,
	eBuffSkillCountType_Heal,
	eBuffSkillCountType_BeHealedVolumn,
	eBuffSkillCountType_BeHealed,
};

enum eAutoNoteError
{
	eAutoNoteError_None,
	eAutoNoteError_CantFind,
	eAutoNoteError_NotProperState,
	eAutoNoteError_AlreadyAsked,
	eAutoNoteError_AlreadyAsking,
	eAutoNoteError_CantUseMap,
};

enum ePetAI
{
	ePetAI_Aggressive,
	ePetAI_Support,
	ePetAI_Stand,
};

enum ePetType
{
	ePetType_Basic,
	ePetType_Physic,
	ePetType_Magic,
	ePetType_Support,
	ePetType_Max,
};

enum ePetState
{
	ePetState_None,
	ePetState_Summon,
	ePetState_Die,
};

// 090701 ShinJS --- ´øÀü Á¾·ù ±¸ºÐ
enum eLIMITDUNGEON_TYPE
{
	eLIMITDUNGEON_TYPE_LOW,			// ÃÊ±Þ ´øÀü
	eLIMITDUNGEON_TYPE_MIDDLE,		// Áß±Þ ´øÀü
	eLIMITDUNGEON_TYPE_HIGH,		// °í±Þ ´øÀü
};


// 090909 ShinJS --- ±æÃ£±â Ãß°¡½Ã ±âÁ¸ ÇÊ¿äÁ¤º¸ Common Header ·Î ÀÌµ¿
#define MOUSE_MOVE_TICK		400
#define MOUSE_PRESS_TICK	400
#define MOUSE_ON_TICK		200
#define FOLLOW_TICK			500		//---KES µû¶ó°¡±â
 

//  ½Å±ÔÁ¾Á· »ý¼º °¡´ÉÇÑ ±âÁ¸Ä³¸¯ÅÍÀÇ ·¹º§¼öÄ¡
#define MAKE_DEVIL_LIMIT_LEVEL	50

// 100322 ONS Äù½ºÆ®¾Ë¸®¹Ì ÀÚµ¿µî·Ï °³¼öÁ¦ÇÑ º¯°æ
#define MAX_QUICKVIEW_QUEST 10
#define MAX_QUEST_COUNT 20

#define MAX_CLASS_LEVEL		6
enum ENUM_CLASS
{
	eClass_FIGHTER = 0,
	eClass_ROGUE,
	eClass_MAGE, 
	eClass_DEVIL,
	eClass_MAXCLASS
} ;

enum eNoteParsingKind
{
	eNoteParsing_None = 0,
	eNoteParsing_FishingLevelUp,
	eNoteParsing_CookLevelUp,
};

enum eNoteError
{
	eNoteErr_None = 0,
	eNoteErr_WrongState,
	eNoteErr_InsertFail,
};

#define DEFAULT_MONEY_TEXT	"0"
#define DEFAULT_VOLUME_TEXT	"1"

enum eBUFF_START_RESULT
{
	eBUFF_START_RESULT_SUCCESS,
	eBUFF_START_RESULT_BATTLE_RULE_DELETE,
	eBUFF_START_RESULT_PEACE_RULE_DELETE,
	eBUFF_START_RESULT_MOVE_RULE_DELETE,
	eBUFF_START_RESULT_STOP_RULE_DELETE,
	eBUFF_START_RESULT_REST_RULE_DELETE,
};

#define MIN_MAGIC_CASTING_TIME	700.f

#endif //__COMMONGAMEDEFINE_H__
