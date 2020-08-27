#ifndef __MAPDBPARSER__H
#define __MAPDBPARSER__H
#pragma once

//#include "..\[CC]ServerModule\DataBase.h"

class CPlayer;
class CParty;
class CSHFarm;
class CSHCrop;
class CAnimal;
class CObject;


enum   DBMESSAGEIDFUNC 
{
	eCharacterInfoQuery,
	eCharacterSkillTreeQuery,
	eCharacterItemQuery,
	eShopItemInfoQuery,			// ItemMall ItemÄõ¸®
	//ePetInfoQuery,				// CharacterPetInfo

	eSSItemInsert,
	
	/* New Item Work */
	eItemCombineUpdate,
	eItemMoveUpdate,
	eItemMoveStorageUpdate,
	eItemMoveGuildUpdate,
	eItemInsert,
	eItemDelete,
	eRegistLoginMapInfo,
	eUnRegistLoginMapInfo,

	/* New Skill Work */
	eSkillUpdate2,
	eSkillInsert,
	eSkillDelete,				/// ©ö¡ì¡Æ©ª¡íef
	eCharacterUpdate,			/// A©ø¢¬?d¨¬¢¬ ¨ú¡À¥ì¡ÍAI¨¡¢ç
	eHeroInfoUpdate,			/// HeroInfo ¨ú¡À¥ì¡ÍAI¨¡¢ç
	eTotalInfoUpdate,			/// TotalInfo ¨ú¡À¥ì¡ÍAI¨¡¢ç
	eBadFameUpdate,
	ePartyRegistMapServer,		//¢¬E¨ù¡©©öo A¡¿; ¢Ò¡× ¨¡A¨¡¨ù¢¬| ©ö¨­¨ú¨¡¢¯A¢¥U. 
	ePartyInfoByUserLogin,		///Au¡Æ¢® ¡¤I¡¾¡¿AI CI¢¬e ¨¡A¨¡¨ù d¨¬¢¬¢¬| ©ö¨­¨ú¨¡¢¯A¢¥U.
	ePartyCreate,
	ePartyBreakup,
	ePartyAddMember,			//¢¬a©öoA©¬¡Æ¢®
	ePartyDelMember,
	ePartyChangeMaster,

	eSavePoint,					// SavePoint 
	eSaveMapChangePoint,		// MapChange
	
	eStorageBuyStorage,
	eCharacterStorageInfo,
	eCharacterStorageItemInfo,
	eStorageMoneyUpdate,

	eFriendNotifyLogout,
	eBadFameCharacterUpdate,
	ePKCharacterUpdate,

	eQuestTotalInfo,	// AE¨¬¢¬AU ¡Æ¢®AI¥ìa
	eQuestUpdate,
	eQuestMainQuestLoad,
	eQuestSubQuestLoad,
	eQuestItemLoad,
	eQuestMainQuestInsert,
	eQuestSubQuestInsert,
	eQuestItemInsert,
	eQuestMainQuestUpdate,
	eQuestSubQuestUpdate,
	eQuestItemUpdate,
	eQuestMainQuestDelete,
	eQuestSubQuestDelete,
	eQuestSubQuestDeleteAll,
	eQuestItemDelete,
	eQuestItemDeleteAll,
	eQuestMainQuestUpdateCheckTime,
	eCharacterInfoByTime,
	eSkillExpByTime,
	eSpeedHackCheck,

	eGuildSelectForReward,	// ±æµåÀü º¸»óÀ» ÇÏ±â À§ÇØ ±æµå ¸ñ·ÏÀ» °¡Á®¿Â´Ù
	eGuildItemSelect,

	eGuildLoadGuild,		// ÀüÃ¼ ±æµå Á¤º¸¸¦ ÀÐ´Â´Ù
	eGuildLoadMember,
	eGuildBreakUp,
	eGuildDeleteMember,
	eGuildSecedeMember,
	eGuildChangeMemberRank,
	eGuildGiveNickName,
	eGuildAddScore,
	eGuildUpdateScore,
	eGuildMarkDelete,
	eConfirmUserOut,
	eLoadGuildWarRecord,
	eDeleteGuildWarRecord,
	eLoadGuildWar,
	eInsertGuildWar,
	eDeleteGuildWar,	
	eUpdateGuildWarRecord,
	eGuildUnionLoad,
	eGuildUnionCreate,
	eGuildUnionDestroy,
	eGuildUnionAddGuild,
	eGuildUnionRemoveGuild,
	eGuildUnionSecedeGuild,
	eChangeCharacterAttr,
	eGuildUpdateNotice,

	eQuestEventCheck,
	eQuickInfo,
	eQuickUpdate,

	// desc_hseos_³óÀå½Ã½ºÅÛ_01
	// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.04.23	2007.09.12	2008.01.16
	eFarm_LoadFarmState,		// ³óÀå »óÅÂ ·Îµå
	eFarm_LoadCropInfo,			// ³óÀÛ¹° Á¤º¸ ·Îµå
	eFarm_LoadTimeDelay,		// Å¸ÀÓ µô·¹ÀÌ Á¤º¸ ·Îµå
	eFarm_SetFarmState,			// ³óÀå »óÅÂ ÀúÀå
	eFarm_SetCropInfo,			// ³óÀå¹° Á¤º¸ ÀúÀå
	eFarm_SetTimeDelay,			// Å¸ÀÓ µô·¹ÀÌ Á¤º¸ ÀúÀå
	eFarm_SetTaxInfo,			// °ü¸®ºñ Á¤º¸ ÀúÀå
	// 091204 ONS ÆÐ¹Ð¸®´ÙÀÌ¾ó·Î±×¿¡ ³óÀå¼¼±Ý³³ºÎ±â´É Ãß°¡ : °³º° ³óÀå Á¤º¸¸¦ DB¿¡¼­ °¡Á®¿À±âÀ§ÇØ Ãß°¡
	eFarm_GetFarmInfo,
	// E ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.04.23	2007.09.12	2008.01.16

	// desc_hseos_¸ó½ºÅÍ¹ÌÅÍ01
	// S ¸ó½ºÅÍ¹ÌÅÍ Ãß°¡ added by hseos 2007.05.28
	eMonsterMeter_LoadInfo,
	eMonsterMeter_SetInfo,
	// E ¸ó½ºÅÍ¹ÌÅÍ Ãß°¡ added by hseos 2007.05.28

	eSkillPointUpdate,
	eJobUpdate,

	eCharacterLogoutPointUpdate,

	// 090413 ShinJS --- Ã§¸°Áö Á¸¿¡¼­ ³ª°¡´Â °æ¿ì
	eCharacterLogoutPointUpdate_ToChallengeSavedPoint,

	// 070917 LYW --- MapDBMsgParser : Add query.
	eSaveNpcMapChangePoint,

	// 071023 LYW --- MapDBMsgParser : Add query to load tutorial info.
	eTutorialLoad,

	// 071023 LYW --- MapDBMsgParser : Add query to update tutorial info.
	eTutorialUpdate,

	// 071106 LYW --- MapDBMsgParser : ±âº» ½ºÅÈÀ¸·Î ¾÷µ¥ÀÌÆ® ÇÏ´Â ÇÔ¼ö Æ÷ÀÎÅÍ.
	eUpdateToDefaultStats,

	// 071117 LYW --- MapDBMsgParser : Äù½ºÆ® °ü·Ã Ãß°¡ ÇÔ¼ö.
	eEndQuest_New,
	eEndSubQuest_New,
	eDeleteQuest_New,

	eCharacterBuffLoad,
	eCharacterBuffAdd,
	eCharacterBuffRemove,
	eCharacterBuffUpdate,
	eSkill_Reset,

	// 071129 LYW --- MapDBMsgParser : ´©Àû ½ºÅ³ Æ÷ÀÎÆ®¸¦ ¾÷µ¥ÀÌÆ®.
	eUpdateAccumulateSkillpoint,

	eConnectionCheck,

	eIncreaseCharacterInventory,
	eResetCharacterInventory,

	// desc_hseos_µ¥ÀÌÆ® Á¸_01
	// S µ¥ÀÌÆ® Á¸ Ãß°¡ added by hseos 2007.11.26	2007.11.27	2007.12.01	2008.01.23
	eChallengeZone_SavePos_SetInfo,

	eChallengeZone_EnterFreq_LoadInfo,
	eChallengeZone_EnterFreq_SetInfo,

	eChallengeZone_Success_SetInfo,

	// 091124 ONS Ã§¸°ÁöÁ¸ °æÇèÄ¡ºÐ¹è ÀúÀå/·Îµå Ã³¸®Ãß°¡
	eChallengeZone_ExpRate_Save,
	eChallengeZone_ExpRate_Load,			
	eChangeCharacterName,
	// 080111 KTH -- ÀÌ¸§º¯°æ ·Î±× Äõ¸® Ãß°¡
	//---KES AUTONOTE
	eAutoNoteList_Load,	//---ÀÚ½ÅÀÌ ÀâÀº ¿ÀÅä
	eAutoNoteList_Add,	//---ÀÚ½ÅÀÌ ÀâÀº ¿ÀÅä
	// 080204 KTH -- ÀÌº¥Æ® ÀÀ¸ð
	eJoinEvent,
	eResidentRegist_Reset,	// RResidentRegist_Reset
//---------------

	eGiftItemInsert,

	ePetInfoLoad,
	ePetInfoAdd,
	ePetInfoRemove,
	ePetInfoUpdate,
	ePetMasterUpdate,

	eItemMoveUpdatePetInven,
	ePetItemInfoLoad,

	eFarm_SetAnimalInfo,
	eFarm_LoadAnimalInfo,

	// 080813 LYW --- MapDBMsgParser : ¼ÒÈ¯ ¿ÀºêÁ§Æ® Á¤º¸¸¦ Ãß°¡ÇÏ´Â ºÐ·ù Ãß°¡.
    eSiegeREcallInsert,

    // 080811 LYW --- MapDBMsgParser : ¼ÒÈ¯ ¿ÀºêÁ§Æ® Á¤º¸¸¦ ¾÷µ¥ÀÌÆ® ÇÏ´Â ºÐ·ù Ãß°¡.
    eSiegeRecallUpdate,
    // 080811 LYW --- MapDBMsgParser : ¼ÒÈ¯ ¿ÀºêÁ§Æ® Á¤º¸¸¦ »èÁ¦ÇÏ´Â ºÐ·ù Ãß°¡.
    eSiegeRecallRemove,
    //// 080812 LYW --- MapDBMsgParser : ¼ÒÈ¯ ¿ÀºêÁ§Æ® Á¤º¸¸¦ ·ÎµåÇÏ´Â ºÐ·ù Ãß°¡.
    eSiegeRecallLoad,

	// 081108 LYW --- MapDBMsgParser : ¼ÒÈ¯ µÈ ¸÷ Á¤º¸¸¦ ÃÊ±âÈ­ ÇÏ´Â ºÐ·ù Ãß°¡.
	eSiegeRecallReset,
  
  	// 080814 KTH --- MapDBMsgParser : °ø¼º °ü·Ã Ãß°¡.
   	eSiegeWarfareInfoLoad,
   	eSiegeWarfareInfoUpdate,
	eSiegeWarWarterSeedComplete,
	eSiegeWarfareGuildBreakUp,
  
  	// 080822 LYW --- MapDBMsgParser : ¼ÒÈ¯ µÈ npc Á¤º¸¸¦ DB¿¡¼­ ·ÎµåÇÏ´Â ºÐ·ù Ãß°¡.
  	eNpcRemainTime_Load,
	eNpcRemainTime_Insert,
	eNpcRemainTime_Delete,
	eNpcRemainTime_Update,
  	eSavePointSiegeWarfare,
	eRequuestKillCount,

	// 090406 NYJ - ÇÏ¿ìÂ¡ °¡±¸¸ñ·Ï
	eHouseFurniture_Load,
	// NYJ - Æ®¸®°Å¸ñ·Ï
	eLoadTriggerPlayerList,
	eLoadTriggerServerList,

	// 091113 ONS ±æµå Åä³Ê¸Õµå ¿ì½Â/ÁØ¿ì½Â½Ã °ñµå Áö±Þ ºÐ·ù Ãß°¡.
	eGTRewardMoneyInsert,

	// 091230 ShinJS --- PC¹æ ¾ÆÀÌÅÛ Áö±Þ
	eProvidePCRoomItem,
	eGetPCRoomInfo,

	eConsignment_GetList,
	eConsignment_Search,
	eNote_GetPackage,

	eForbidChatUpdate,
	eForbidChatLoad,
	
	eInvenSort, // ---- skr sorting
	eWarehouseItemInfoSet, // --- skr : warehouse 2020agt27

	MaxQuery
};

#define STORED_CHARACTER_ITEMINFO				"dbo.MP_CHARACTER_ITEMINFO_NEW"				
#define STORED_CHARACTER_HEROINFOUPDATE			"dbo.MP_CHARACTER_HEROINFOUPDATE"
#define STORED_CHARACTER_TOTALINFOUPDATE		"dbo.MP_CHARACTER_TOTALINFOUPDATE"
#define STORED_CHARACTER_BADFAMEUPDATE			"dbo.MP_CHARACTER_BADFAMEUPDATE"
#define STORED_CHARACTER_REGISTLOGINMAPINFO		"dbo.MP_LOGINMAPINFO_REGIST"
#define STORED_CHARACTER_UNREGISTLOGINMAPINFO	"dbo.MP_LOGINMAPINFO_UNREGIST"

#define STORED_ITEM_COMBINEUPDATE				"dbo.MP_ITEM_COMBINEUPDATE"
#define STORED_ITEM_MOVEUPDATE_STORAGE			"dbo.MP_ITEM_MOVEUPDATESTORAGE"
#define STORED_ITEM_INSERT_INVEN				"dbo.MP_ITEM_INSERT"

/* New Skill */
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_SKILL_UPDATE						"dbo.MP_SKILLTREE_UPDATE"
#define STORED_SKILL_INSERT						"dbo.MP_SKILLTREE_INSERT"
#define STORED_PARTY_REGIST_MAPSERVER			"dbo.MP_PARTY_REGIST"
#define STORED_PARTY_INFO_BY_USERLOGIN			"dbo.MP_PARTY_PARTYINFOBYUSERLOGIN"
#define STORED_PARTY_CREATE						"dbo.MP_PARTY_CREATEPARTY"
#define STORED_PARTY_BREAK						"dbo.MP_PARTY_BREAKUPPARTY"
#define STORED_PARTY_UPDATE						"dbo.MP_PARTY_UPDATEMEMBER"
#define STORED_PARTY_ADDMEMBER					"dbo.MP_PARTY_ADDMEMBER"
#define STORED_PARTY_DELMEMBER					"dbo.MP_PARTY_DELMEMBER"
#define STORED_PARTY_CHANGEMASTER				"dbo.MP_PARTY_CHANGEMASTER"

//guild
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_GUILD_CREATE						"dbo.MP_GUILD_CREATE"
#define STORED_GUILD_BREAKUP					"dbo.MP_GUILD_BREAKUP"
#define STORED_GUILD_DELETEMEMBER				"dbo.MP_GUILD_DELETEMEMBER"
#define STORED_GUILD_LOADGUILD					"dbo.MP_GUILD_LOADGUILD"
#define STORED_GUILD_MARKUPDATE					"dbo.MP_GUILD_MARKUPDATE"
#define STORED_GUILD_LOADMARK					"dbo.MP_GUILD_LOADMARK"
#define STORED_GUILD_GIVENICKNAME				"dbo.MP_GUILD_GIVEMEMBERNICKNAME"
#define STORED_GUILD_MONEYUPDATE				"dbo.MP_GUILD_MONEYUPDATE"
#define STORED_GUILD_MARKREGIST					"dbo.MP_GUILD_MARKREGIST"
//091111 ONS ±æµå¹®Àå »èÁ¦ ÇÁ·Î½ÃÀú Ãß°¡
#define STORED_GUILD_MARKDELETE					"dbo.MP_GUILD_MARKDELETE"

#define STORED_STORAGE_BUYSTORAGE				"dbo.MP_STORAGE_BUY"
#define STORED_STORAGE_MONEYUPDATE				"dbo.MP_STORAGE_MONEYUPDATE"
#define STORED_STORAGE_INFO						"dbo.MP_STORAGE_INFO"
#define STORED_STORAGE_ITEMINFO					"dbo.MP_STORAGE_ITEMINFO"
#define STORED_STORAGE_MONEYUPDATE				"dbo.MP_STORAGE_MONEYUPDATE"

// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_FRIEND_NOTIFYLOGOUT				"dbo.MP_FRIEND_NOTIFYLOGOUT"
#define STORED_SHOPITEM_INFO					"dbo.MP_SHOPITEM_ITEMINFO"
#define STORED_BADFAME_CHARACTERUPDATE			"dbo.MP_BADFAME_CHARACTERUPDATE"
#define STORED_PK_CHARACTERUPDATE				"dbo.MP_PK_CHARACTERUPDATE"
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_QUEST_MAINQUEST_LOAD				"dbo.MP_MAINQUEST_LOAD"
#define STORED_QUEST_MAINQUEST_INSERT			"dbo.MP_MAINQUEST_INSERT"
#define STORED_QUEST_MAINQUEST_DELETE			"dbo.MP_MAINQUEST_DELETE"
#define STORED_QUEST_MAINQUEST_UPDATE			"dbo.MP_MAINQUEST_UPDATE"
#define STORED_QUEST_SUBQUEST_LOAD				"dbo.MP_SUBQUEST_LOAD"
// 071117 LYW --- QuestUpdate : DB ¾÷µ¥ÀÌÆ® ¼öÁ¤.
#define STORED_QUEST_ENDQUEST_NEW		"dbo.MP_QUEST_ENDQUEST_NEW"
#define STORED_QUEST_ENDSUBQUEST_NEW	"dbo.MP_QUEST_ENDSUBQUEST_NEW"
#define STORED_QUEST_DELETEQUEST_NEW	"dbo.MP_QUEST_DELETEQUEST_NEW"
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_QUEST_SUBQUEST_INSERT			"dbo.MP_SUBQUEST_INSERT"
#define STORED_QUEST_SUBQUEST_DELETE_ALL 		"dbo.MP_SUBQUEST_DELETE_ALL"
#define STORED_QUEST_SUBQUEST_DELETE			"dbo.MP_SUBQUEST_DELETE"
#define STORED_QUEST_SUBQUEST_UPDATE			"dbo.MP_SUBQUEST_UPDATE"
#define STORED_QUEST_ITEM_LOAD					"dbo.MP_QUESTITEM_LOAD"
#define STORED_QUEST_ITEM_INSERT				"dbo.MP_QUESTITEM_INSERT"
#define STORED_QUEST_ITEM_DELETE				"dbo.MP_QUESTITEM_DELETE"
#define STORED_QUEST_ITEM_DELETE_ALL			"dbo.MP_QUESTITEM_DELETE_ALL"
#define STORED_QUEST_ITEM_UPDATE				"dbo.MP_QUESTITEM_UPDATE"
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_MAPCHANGE_SKILLTREEINFO			"dbo.MP_CHARACTER_SKILLTREEINFO"
#define STORED_SAVE_MAPCHAGEPOINT				"dbo.MP_CHARACTER_MAPCHANGEPOINTUPDATE"
#define STORED_SAVE_MAPCHAGEPOINTRET			"dbo.MP_CHARACTER_MAPCHANGEPOINTRETURN"
#define STORED_SAVE_LOGINPOINT					"dbo.MP_CHARACTER_LOGINPOINTUPDATE"

#define STORED_FARM_LOAD_DELAYTIME				"dbo.MP_FARM_LOADTIMEDELAY"
#define STORED_FARM_SET_ANIMALINFO				"dbo.MP_FARM_SETANIMALINFO"
#define STORED_FARM_SET_FARMSTATE				"dbo.MP_FARM_SETFARMSTATE"
#define STORED_FARM_SET_CROPINFO				"dbo.MP_FARM_SETCROPINFO"
#define STORED_FARM_SET_DELAYTIME				"dbo.MP_FARM_SETTIMEDELAY"
#define STORED_FARM_SET_TAXINFO					"dbo.MP_FARM_SETTAXINFO"

// desc_hseos_¸ó½ºÅÍ¹ÌÅÍ01
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_MONSTERMETER_LOAD_INFO			"dbo.MP_MONSTERMETER_LOADINFO"
#define STORED_MONSTERMETER_SET_INFO			"dbo.MP_MONSTERMETER_SETINFO"

// desc_hseos_ÁÖ¹Îµî·Ï01
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_RESIDENTREGIST_LOAD_INFO				"dbo.MP_RESIDENTREGIST_LOADINFO"
#define STORED_RESIDENTREGIST_SAVE_INFO				"dbo.MP_RESIDENTREGIST_SAVEINFO"
#define STORED_RESIDENTREGIST_UPDATE_INTRODUCTION	"dbo.MP_RESIDENTREGIST_UPDATEINTRODUCTION"
// 080215 KTH -- RESET RESIDENT REGIST
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_RESIDENTREGIST_RESET				"dbo.MP_RESIDENTREGIST_RESET"

#define STORED_TUTORIAL_LOAD					"dbo.MP_TUTORIAL_LOAD"
#define STORED_TUTORIAL_UPDATE					"dbo.MP_TUTORIAL_UPDATE"
// 071106 LYW --- MapDBMsgParser : Define procedure name to update stats to default.
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_DEFAULT_STATS					"dbo.MP_DEFAULT_STATS_UPDATE"

// 071129 LYW --- MapDBMsgParser : Define procedure name to resetting skills to default.
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_DEFAULT_SKILL					"dbo.MP_SKILLTREE_RESET"

// 071129 LYW --- MapDBMsgParser : Define procedure name to update accmulate skill point.
// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
#define STORED_UPDATE_ACCSKILLPOINT				"dbo.MP_SKILLTREE_SKILLPOINT_UPDATE_ACCUMULATE"

#define STORED_CHALLENGEZONE_ENTERFREQ_SAVE_INFO	"dbo.MP_CHALLENGEZONE_ENTER_SAVEINFO"
#define STORED_CHALLENGEZONE_ENTERFREQ_LOAD_INFO	"dbo.MP_CHALLENGEZONE_ENTER_LOADINFO"
#define STORED_CHALLENGEZONE_SUCCESS_SAVE_INFO		"dbo.MP_CHALLENGEZONE_SUCCESS_SAVEINFO"
#define STORED_CHALLENGEZONE_CLEAR_NO1_LOAD_INFO	"dbo.MP_CHALLENGEZONE_CLEAR_NO1_LOADINFO"
// 091124 ONS Ã§¸°ÁöÁ¸ °æÇèÄ¡ºÐ¹è ÀúÀå/·Îµå Ã³¸®Ãß°¡
#define STORED_CHALLENGEZONE_EXP_RATE_LOAD			"dbo.MP_CHALLENGEZONE_EXP_RATE_LOAD"
#define STORED_CHALLENGEZONE_EXP_RATE_SAVE			"dbo.MP_CHALLENGEZONE_EXP_RATE_SAVE"
// 071210 KTH -- ÀÎº¥Åä¸® È®Àå
#define STORED_INCREASE_CHARACTER_INVENTORY		"dbo.MP_INCREASE_CHARACTER_INVENTORY"
#define STORED_RESET_CHARACTER_INVENTORY		"dbo.MP_RESET_CHARACTER_INVENTORY"
#define STORED_SIEGEWARFARE_INFO_LOAD			"dbo.MP_SIEGEWARFARE_INFO_LOAD"
#define STORED_SIEGEWARFARE_INFO_UPDATE			"dbo.MP_SIEGEWARFARE_INFO_UPDATE"

void CharacterNumSendAndCharacterInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterSkillTreeInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemInfo( DWORD CharacterIDX );
void SSItemInsert(DWORD CharacterIdx, DWORD wItemIdx, DURTYPE, POSTYPE, DWORD FromChrIdx, ITEM_SEAL_TYPE, int RemainSecond);
void ItemUpdateToDB(DWORD CharacterIdx, DWORD dwDBIdx, DWORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition, DWORD itemParam = 0);
void ItemCombineUpdateToDB(DWORD dwfromDBIdx, DURTYPE fromDur, DWORD dwToDBIdx, DURTYPE toDur, DWORD CharacterIdx);
void ItemMoveUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos, DWORD UserIdx, DWORD MunpaIdx);
void ItemMoveStorageUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMoveGuildUpdateToDB(DWORD CharacterIDX, DWORD MunpaIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemInsertToDB(DWORD CharacterIdx, DWORD wItemIdx, DURTYPE, POSTYPE, DWORD dwKey, ITEM_SEAL_TYPE);
void ItemDeleteToDB(DWORD dwDBIdx);
// 080402 LUJ, ÀÎÀÚ ÀÌ¸§À» ¾ÆÀÌÅÛ DBÀÎµ¦½º·Î º¯°æ
void ItemOptionDelete( DWORD dwDBIdx);
// 081118 LUJ, Ä³¸¯ÅÍ ¹øÈ£¸¦ Äõ¸®¸¦ ½ÇÇàÇÑ DB ½º·¹µå¸¦ ¼±ÅÃÇÒ Å°°ªÀ¸·Î »ç¿ëµÇµµ·Ï ÇÔ
void ItemOptionInsert( const ITEM_OPTION&, DWORD CharacterIdx );
void ItemDropOptionInsert( const ITEM_OPTION&, DWORD CharacterIdx );

/* New Skill Qurey Func */
// 071231 KTH -- CharacterIDX Add
void SkillUpdateToDB( SKILL_BASE* msg, DWORD CharacterIDX );
void SkillInsertToDB(DWORD CharacterIDX, DWORD SkillIDX, LEVELTYPE);

void PartyLoad(DWORD PartyIDX);
void PartyInfoByUserLogin(DWORD PartyIDX, DWORD CharacterIDX);
void PartyBreakup(DWORD PartyIDX, DWORD CharacterIDX);
void PartyCreate(DWORD MasterIDX, BYTE Option, BYTE SubOption);
void PartyMemberUpdate(CParty* pParty, DWORD PartyIDx);
void PartyAddMember(DWORD PartyIDX, DWORD TargetID);
void PartyDelMember(DWORD PartyIDX, DWORD TargetID);
void PartyChangeMaster(DWORD PartyID, DWORD PlayerID, DWORD TargetPlayerID);

void GuildLoadGuild(DWORD StartGuildIdx);
void GuildLoadMember(DWORD StartCharacterIdx);
void GuildCreate( DWORD CharacterIdx, const char* CharacterName, LEVELTYPE CharacterLvl, const char* GuildName, const char* Intro, int masterLevel  );
void GuildBreakUp(DWORD GuildIdx, DWORD MasterIdx);
void GuildDeleteMember(DWORD GuildIdx, DWORD MemberIDX);
void GuildSecedeMember(DWORD GuildIdx, DWORD PlayerIdx);
void GuildAddMember(DWORD GuildIdx, DWORD playerIndex, const char* guildName, BYTE rank );
void RGuildAddMember( LPMIDDLEQUERY, LPDBMESSAGE );
void GuildMarkRegist(DWORD GuildIdx, char* ImgData, DWORD CharacterIdx);
void GuildChangeMemberRank(DWORD GuildIdx, DWORD MemberIdx, BYTE Rank);

void GuildGiveMemberNickName(DWORD TargetId, char * NickName);
void GuildUpdateNotice( DWORD guildIndex, const char*, DWORD CharacterIDX );
void StorageBuyStorage(DWORD PlayerIDX);

void CharacterStorageInfo(DWORD UserIdx, DWORD CharacterIdx);
//void CharacterStorageItemInfo(DWORD CharacterIdx, DWORD StartDBIdx);
// 071220 KTH
void CharacterStorageItemInfo(DWORD CharacterIdx, DWORD UserIdx, DWORD StartDBIdx);
// 071231 KTH -- CharacterIDX Add
void StorageMoneyUpdateToDB(DWORD UserIdx, MONEYTYPE StorageMoney, DWORD CharacterIDX);

void FriendNotifyLogouttoClient(DWORD PlayerID);
void BadFameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val);
void PKCharacterUpdate(DWORD PlayerIDX, DWORD val);
//////////////////////////////////////////////////////////////////////////
// SavePoint
void SavePointUpdate(DWORD CharacterIDX, WORD LoginPoint_Idx, WORD MapNum);
void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx);
// 081006 LYW --- MapDBMsgParser : ¸Ê ÀÌµ¿ Á¤º¸ ¾÷µ¥ÀÌÆ® ÇÔ¼ö Ãß°¡.
void SaveMapChangePointUpdate_SiegeWarfare( DWORD CharacterIDX, WORD MapNum, float fXpos, float fZpos ) ;
 //¸Ê ÀÌµ¿½Ã ³Ê¹« ´À·Á ÇÃ·¹ÀÌ¾î°¡ ´Ù¸¥ ¸Ê¿¡ µé¾î°¡±â Àü¿¡ ¾÷µ¥ÀÌÆ® È®ÀÎ ÈÄ ¸Ê Ã¤ÀÎÁö ÇÁ·ÎÅäÄÝÀ» º¸³¿
// 081218 LUJ, ÇÃ·¹ÀÌ¾î ¼ÒÁö±Ý Ãß°¡
void SaveMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE inventoryMoney, MONEYTYPE StorageMoney);
// 070917 LYW --- MapDBMsgParser : Add function to save map change point to return.
// 081218 LUJ, ÇÃ·¹ÀÌ¾î ¼ÒÁö±Ý Ãß°¡
void SaveNpcMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE inventoryMoney, MONEYTYPE StorageMoney) ;
//////////////////////////////////////////////////////////////////////////
void CharacterHeroInfoUpdate(CPlayer*);
void CharacterTotalInfoUpdate(CPlayer*);
void StorageMoneyUpdate(DWORD PlayerID, BYTE StorageNum, DWORD Money);
void RegistLoginMapInfo(DWORD CharacterIDX, char* CharacterName, BYTE MapNum, WORD MapPort);
void UnRegistLoginMapInfo(DWORD CharacterIDX);

void QuestTotalInfo(DWORD PlayerID);
void QuestUpdateToDB(DWORD PlayerID, DWORD QuestIdx, QSTATETYPE State, BYTE bEnd);
//RaMa
void QuestItemload(DWORD PlayerID, int QuestIdx = -1 );
void QuestItemInsert(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD dwItemNum );
void QuestItemDelete(DWORD PlayerID, DWORD ItemIdx);
void QuestItemDeleteAll(DWORD PlayerID, DWORD dwQuestIdx);
void QuestItemUpdate(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD ItemNum);

void QuestMainQuestLoad(DWORD PlayerID, int QuestIdx = -1 );
void QuestMainQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, __time64_t registTime);
void QuestMainQuestDelete(DWORD PlayerID, DWORD mQuestIdx);
void QuestMainQuestInsert(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, __time64_t registTime, BOOL repeat);

void QuestSubQuestLoad(DWORD PlayerID, int QuestIdx = -1 );

// 071117 LYW --- MapDBMsgParser : Äù½ºÆ® °ü·Ã Ãß°¡ ÇÔ¼ö.
void EndQuest_New(DWORD dwCharacter_Idx, DWORD dwQuest_Idx, DWORD dwSubQuest_Do, DWORD dwEnd_Param, __time64_t registTime) ;
void EndSubQuest_New(DWORD dwCharacter_Idx, DWORD dwQuest_Idx, DWORD dwSubQuestIdx, DWORD dwSubQuest_Do, __time64_t registTime) ;
void DeleteQuest_New(DWORD dwCharacter_Idx, DWORD dwQuest_Idx, DWORD param) ;

void QuestSubQuestInsert(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time);
void QuestSubQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time);
void QuestSubQuestDelete(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx);
void QuestSubQuestDeleteAll(DWORD PlayerID, DWORD mQuestIdx);

void QuestMainQuestUpdateCheckTime(DWORD dwCharIdx, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime );

// 071023 LYW --- MapDBMsgParser : Add Load query function.
void TutorialLoad(DWORD dwPlayerID, int tutorialIdx = -1, BOOL isComplete = false) ;
// 071023 LYW --- MapDBMsgParser : Add Update query function.
void TutorialUpdateToDB(DWORD dwPlayerID, int tutorialIdx, BOOL isComplete) ;
// 071106 LYW --- MapDBMsgParser : ±âº» ½ºÅÈÀ¸·Î ¾÷µ¥ÀÌÆ® ÇÏ´Â ÇÔ¼ö.
void DB_UpdateToDefaultStats(DWORD dwPlayerID, int nStr, int nDex, int nVit, int nInt, int nWis, int nRemainPoint) ;

// 071129 LYW --- MapDBMsgParser : ½ºÅ³ ÃÊ±âÈ­ Äõ¸® ÇÔ¼ö Ãß°¡.
void DB_ResetToDefaultSkill(DWORD dwPlayerID) ;

// 071129 LYW --- MapDBMsgParser : ´©Àû ½ºÅ³ Æ÷ÀÎÆ® ¾÷µ¥ÀÌÆ® ÇÔ¼ö Ãß°¡.
void DB_UpdateAccumulateSkillPoint(DWORD dwPlayerID, BOOL bForce, int nPoint = 0) ;
void CharacterShopItemInfo(DWORD CharacterIDX, DWORD StartDBIdx);
void CharacterChangeName( DWORD CharacterIdx, char* Name, DWORD DBIdx );
// ·Î±×
void InsertLogCharacter( DWORD charIdx, LEVELTYPE level, HERO_TOTALINFO*  );
void InsertLogExp( BYTE bType, DWORD charIdx, LEVELTYPE level, EXPTYPE changeValue, EXPTYPE NowExp, WORD MurdererKind, DWORD MurdererIdx, DWORD CurAbilPoint);
void InsertLogMoney( BYTE type, DWORD charIdx, MONEYTYPE InvenMoney, MONEYTYPE ChangeValue, DWORD TargetID, MONEYTYPE TargetMoney );
void InsertLogWorngMoney( BYTE type, DWORD charIdx, MONEYTYPE changeValue, MONEYTYPE InvenMoney, MONEYTYPE StorageMoney, DWORD targetIdx );

//void InsertLogSkill( WORD type, DWORD charIdx, DWORD skillIdx, DWORD skillDBIdx, LEVELTYPE skillLevel, EXPTYPE skillExp );

// 071109 ¿õÁÖ
void InsertLogSkill( CPlayer*, const SKILL_BASE*, DWORD type );
void InsertLogGuildWarehouse( CPlayer*, POSTYPE previousPosition, const ITEMBASE*, MONEYTYPE, eLogitemmoney );
void InsertLogJob( CPlayer*, DWORD classIndex, DWORD jobIndex, DWORD jobGrade, char* jobMemo = "" );
void InsertLogGuildScore( DWORD guildIndex, float score, eGuildLog );
void InsertLogFarmCrop( CSHFarm*, CSHCrop*,  eFamilyLog );
// desc_hseos_³óÀå½Ã½ºÅÛ_01
// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2008.01.22
void InsertLogFarm( DWORD nZone, DWORD nFarm, DWORD nKind, DWORD nPlayerID, DWORD nValue01 = 0, DWORD nValue02 = 0);
void InsertLogQuest( CPlayer*, DWORD mainQuestIndex, DWORD subQuestIndex, int index, int variation, eQuestLog );

void InsertLogSkillExp( DWORD charIdx, EXPTYPE changeValue, DWORD skillIdx, DWORD skillDBIdx, LEVELTYPE skillLevel, EXPTYPE skillExp, POSTYPE skillPos );
void LogItemMoney(DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
				eLogitemmoney LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
				DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos,
				DURTYPE ItemDur, EXPTYPE ExpPoint);
void LogItemOption(DWORD CharacterIdx, DWORD ItemDBIdx, const ITEM_OPTION&, eLogitemmoney type );
// 080327 LUJ, µå·Ó ¿É¼Ç ·Î±× ÀúÀå
void LogItemDropOption( const ITEM_OPTION&, eLogitemmoney );
void LogCheat(DWORD Character_Idx, BYTE LogType, MONEYTYPE Param, DWORD ItemIdx, DWORD ItemDBIdx);
void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 );
void LogGuild(DWORD CharacterIdx, DWORD GuildIdx, WORD LogType, DWORD Param1 = 0, DWORD Param2 = 0);
void LogFamily(DWORD CharacterIdx, DWORD FamilyIdx, WORD LogType, DWORD Param1 = 0, DWORD Param2 = 0);
void UpdateCharacterInfoByTime(DWORD charIdx, EXPTYPE exp, /*¾îºô¸®Æ¼ »èÁ¦ - ÀÌ¿µÁØ EXPTYPE abilityExp, */MONEYTYPE money, DWORD time, DWORD LastPKEndTime );
void UpdateSkillExpByTime(DWORD charIdx, DWORD DBIdx, EXPTYPE exp);

void InsertSpeedHackCheck(DWORD charIdx, char* charName, DWORD Distance, DWORD skillDelay);

void UnSealItemByRealTime(DWORD dwPlayerID, DWORD dwDBIdx, DWORD dwTime);
void UnSealItemByGameTime(DWORD dwPlayerID, DWORD dwDBIdx, DWORD dwTime);
void UpdateRemainTime(DWORD dwPlayerID, DWORD dwDBIdx, ITEM_SEAL_TYPE, DWORD dwTime);

void RCharacterNumSendAndCharacterInfo(LPQUERY, LPDBMESSAGE);
void RCharacterSkillTreeInfo(LPQUERY, LPDBMESSAGE);
void RCharacterItemInfo(LPQUERY, LPDBMESSAGE);
void RSSItemInsert(LPQUERY, LPDBMESSAGE);
void RItemInsert(LPQUERY, LPDBMESSAGE);

/* New Skill Return Func */
void RSkillInsert(LPQUERY, LPDBMESSAGE);

void RPartyLoad(LPQUERY, LPDBMESSAGE);
void RPartyCreate(LPQUERY, LPDBMESSAGE);
void RPartyInfoByUserLogin(LPQUERY, LPDBMESSAGE);
void RPartyChangeMaster(LPQUERY, LPDBMESSAGE);
void RPartyDelMember(LPQUERY, LPDBMESSAGE);
// 090529 ShinJS --- ÆÄÆ¼¸â¹ö Ãß°¡½Ã ÆÄÆ¼¿ø Á¤º¸ °»½Å Ãß°¡
void RPartyAddMember(LPQUERY, LPDBMESSAGE);

void RGuildLoadGuild(LPQUERY, LPDBMESSAGE);
void RGuildLoadMember(LPQUERY, LPDBMESSAGE);
void RGuildLoadMark(LPLARGEQUERY pData, LPDBMESSAGE);
void RGuildCreate(LPMIDDLEQUERY, LPDBMESSAGE);
void RGuildBreakUp( LPQUERY, LPDBMESSAGE );
void RGuildDeleteMember( LPQUERY, LPDBMESSAGE );
void RGuildSecedeMember( LPQUERY, LPDBMESSAGE );
//void RGuildItemLoad( LPQUERY, LPDBMESSAGE );
//void RGuildItemOption(LPQUERY, LPDBMESSAGE);
void RGuildMarkRegist(LPLARGEQUERY pData, LPDBMESSAGE);
//void RGuildLoadNotice(LPMIDDLEQUERY pData, LPDBMESSAGE);
//////////////////////////////////////////////////////////////////////////

void RCharacterStorageInfo(LPQUERY, LPDBMESSAGE);
void RCharacterStorageItemInfo(LPQUERY, LPDBMESSAGE);

//////////////////////////////////////////////////////////////////////////
void RFriendNotifyLogouttoClient(LPQUERY, LPDBMESSAGE);
void RQuestTotalInfo(LPQUERY, LPDBMESSAGE);
void RQuestMainQuestLoad(LPQUERY, LPDBMESSAGE);
void RQuestSubQuestLoad(LPQUERY, LPDBMESSAGE);
void RQuestItemLoad(LPQUERY, LPDBMESSAGE);
//////////////////////////////////////////////////////////////////////////
void RSaveMapChangePointReturn(LPQUERY, LPDBMESSAGE);
//////////////////////////////////////////////////////////////////////////
//
void RCharacterShopItemInfo(LPQUERY, LPDBMESSAGE);

//void UpdateCharacterMoney( DWORD dwCharIdx, MONEYTYPE money, BYTE flag );
//void RUpdateCharacterMoney( LPQUERY, LPDBMESSAGE );

void ConfirmUserOut( DWORD dwConnectIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD dwKind );
void RConfirmUserOut( LPQUERY, LPDBMESSAGE );

void LoadGuildWarRecord( DWORD dwIdx );
void RLoadGuildWarRecord( LPQUERY, LPDBMESSAGE );
void DeleteGuildWarRecord( DWORD dwGuildIdx );
void LoadGuildWar( DWORD dwIdx );
void RLoadGuildWar( LPQUERY, LPDBMESSAGE );
void InsertGuildWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney );
void DeleteGuildWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 );
void UpdateGuildWarRecordToDB( DWORD dwGuildIdx, DWORD dwVitory, DWORD dwDraw, DWORD dwLoose );
// guildunion
void GuildUnionLoad( DWORD dwStartGuildUnionIdx );
void RGuildUnionLoad( LPQUERY, LPDBMESSAGE );
void GuildUnionLoadMark( DWORD dwMarkIdx );
void RGuildUnionLoadMark( LPLARGEQUERY pData, LPDBMESSAGE );

void GuildUnionCreate( DWORD dwCharacterIdx, DWORD dwGuildIdx, char* sUnionName );
void RGuildUnionCreate( LPQUERY, LPDBMESSAGE );
void GuildUnionDestroy( DWORD dwGuildUnionIdx, DWORD dwMarkIdx, DWORD dwMasterIdx, DWORD dwTime );
void GuildUnionAddGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx );
void GuildUnionRemoveGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx );
void GuildUnionSecedeGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx, DWORD dwTime );

void GuildUnionMarkRegist( DWORD dwCharacterIdx, BOOL isUpdate, DWORD dwGuildUnionIdx, char* pImgData );
void RGuildUnionMarkRegist( LPLARGEQUERY pData, LPDBMESSAGE );

//¦®¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¯
//¦­SiegeWar													  ¦­
//¦±¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦°
void SiegeWarInfoLoad( DWORD MapNum );
void RSiegeWarInfoLoad( LPQUERY, LPDBMESSAGE );
void SiegeWarInfoInsert( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum );
void SiegeWarInfoUpdate( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum );
void SiegeWarGuildInfoLoad( DWORD StartDBIdx, DWORD SiegeWarIdx, DWORD MapNum );
void RSiegeWarGuildInfoLoad( LPQUERY, LPDBMESSAGE );
void SiegeWarGuildInsert( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum );
void SiegeWarGuildUpdate( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum );
void SiegeWarGuildDelete( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD MapNum );

void ChangeCharacterAttr( DWORD dwPlayerIdx, DWORD dwAttr );
void QuestEventCheck( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwNpcIdx, DWORD dwQuestIdx, char* pTime );
void RQuestEventCheck( LPQUERY, LPDBMESSAGE );

void QuickInfo( DWORD characterIdx );
void RQuickInfo( LPQUERY, LPDBMESSAGE );
void QuickUpdate( DWORD characterIdx, BYTE sheet, SLOT_INFO* pSlot );

// desc_hseos_³óÀå½Ã½ºÅÛ_01
// S ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.04.23	2007.09.12	2008.01.16
void RFarm_LoadFarmState( LPQUERY, LPDBMESSAGE );
void RFarm_LoadCropInfo( LPQUERY, LPDBMESSAGE );

//--------------------------------------------------------------------------------------
// 080416 KTH -- °¡Ãà Á¤º¸
void RFarm_LoadAnimalInfo(LPQUERY pData, LPDBMESSAGE pMessage);
// 080425 KTH -- °¡Ãà ·Î±×
void InsertLogFarmAnimal( CSHFarm*, CAnimal*,  eFamilyLog );
//--------------------------------------------------------------------------------------

void Farm_LoadTimeDelay(DWORD nPlayerID);
void Farm_SetTimeDelay(DWORD nPlayerID, DWORD nKind, DWORD nRemainSecond);
void RFarm_LoadTimeDelay(LPQUERY pData, LPDBMESSAGE pMessage);

void Farm_SetTaxInfo(DWORD nFarmZone, DWORD nFarmID, DWORD nTaxArrearageFreq, DWORD nTaxPayPlayerID);
// 091204 ONS ÆÐ¹Ð¸®´ÙÀÌ¾ó·Î±×¿¡¼­ ³óÀå ¼¼±Ý³³ºÎ±â´É Ãß°¡
void Farm_GetFarmInfo(DWORD dwPlyaerID, DWORD dwFarmZone, DWORD dwFarmID);
void RFarm_GetFarmInfo(LPQUERY pData, LPDBMESSAGE pMessage);
// E ³óÀå½Ã½ºÅÛ Ãß°¡ added by hseos 2007.04.23	2007.09.12	2008.01.16

// desc_hseos_¸ó½ºÅÍ¹ÌÅÍ01
// S ¸ó½ºÅÍ¹ÌÅÍ Ãß°¡ added by hseos 2007.05.28
void MonsterMeter_Load(DWORD nPlayerID);
void MonsterMeter_Save(DWORD nPlayerID, DWORD nPlayTime, DWORD nKillMonNum, DWORD nPlayTimeTotal, DWORD nKillMonNumTotal);
void RMonsterMeter_LoadInfo( LPQUERY, LPDBMESSAGE );
// E ¸ó½ºÅÍ¹ÌÅÍ Ãß°¡ added by hseos 2007.05.28

// desc_hseos_ÁÖ¹Îµî·Ï01
// S ÁÖ¹Îµî·Ï Ãß°¡ added by hseos 2007.06.04
void ResidentRegist_LoadInfo(DWORD nPlayerID);
void ResidentRegist_SaveInfo(DWORD nPlayerID, void* _pInfo, int nIntroduction = 0);
void RResidentRegist_LoadInfo(LPMIDDLEQUERY pData, LPDBMESSAGE);
// 080215 KTH -- Reset Resident Regist
void ResidentRegist_Reset(DWORD nPlayerID);
void RResidentRegist_ResetResult(LPQUERY pData, LPDBMESSAGE pMessage);
// E ÁÖ¹Îµî·Ï Ãß°¡ added by hseos 2007.06.04

void SkillPointUpdate(DWORD CharacterIdx, DWORD SkillPoint);

void CharacterJobUpdate( DWORD CharacterIdx, WORD Grade, WORD Job1, WORD Job2, WORD Job3, WORD Job4, WORD Job5, WORD Job6 );

// 070731 ¿õÁÖ, ±æµå Á¡¼ö¸¦ Ãß°¡
void GuildAddScore( DWORD guildIndex, float score );
void RGuildAddScore( LPQUERY, LPDBMESSAGE );

// 080417 LUJ, ±æµå ½ºÅ³ È¹µæ
void RGuildAddSkill( LPMIDDLEQUERY, LPDBMESSAGE );

// 080417 LUJ, ±æµå ½ºÅ³ ·Îµù
void RGuildGetSkill( LPMIDDLEQUERY, LPDBMESSAGE );

// 071005 ¿õÁÖ, ´Ù¸¥ ±æµå¿øÀ» Á×ÀÌ°Å³ª Á×¾úÀ»¶§ ±æµå Æ÷ÀÎÆ® °»½Å
void GuildUpdateScore( CPlayer* killer, CPlayer* corpse, float killerPoint, float corpsePoint );
void RGuildUpdateScore( LPQUERY, LPDBMESSAGE );

// 070801 ¿õÁÖ, Á¢¼Ó ½Ã ÇØ´ç ÇÃ·¹ÀÌ¾î¿¡°Ô Àü¼ÛÇÒ ±æµå Á¤º¸¸¦ Äõ¸®ÇÑ´Ù.
//void GuildReadMember( DWORD playerIndex, DWORD guildIndex );
//void RGuildMemberSelect( LPQUERY, LPDBMESSAGE );

// ±æµå ±âº» Á¤º¸¸¦ °¡Á®¿È
void GuildReadData( DWORD playerIndex, DWORD guildIndex );
void RGuildReadData( LPMIDDLEQUERY, LPDBMESSAGE );
void RGuildSetLevel(LPMIDDLEQUERY, LPDBMESSAGE);
void GuildResetWarehouse(MAPTYPE);
void GuildOpenWarehouse( DWORD guildIndex, DWORD playerIndex, MAPTYPE );
void RGuildOpenWarehouse( LPMIDDLEQUERY, LPDBMESSAGE );

void GuildCloseWarehouse( DWORD guildIndex, DWORD playerIndex, MAPTYPE );
void RGuildCloseWarehouse( LPMIDDLEQUERY, LPDBMESSAGE );

// ±æµå ¾ÆÀÌÅÛ ¹× ¿É¼Ç Á¤º¸¸¦ ÀÐ´Â´Ù
void GuildItemSelect( DWORD playerIndex, DWORD guildIndex, DWORD beginItemDbIndex );
void RGuildItemSelect( LPQUERY, LPDBMESSAGE );

// ±æµå Ã¢°í »ç¿ë °¡´É ±ÇÇÑÀ» ºÎ¿©ÇÑ´Ù
void GuildSetWarehouseRank( DWORD guildIndex, DWORD playerIndex, BYTE rank );
void RGuildSetWarehouseRank( LPMIDDLEQUERY, LPDBMESSAGE );

// ±æµå ¸¶½ºÅÍ¸¦ ¹Ù²Û´Ù
void GuildChangeMaster( DWORD guildIndex, DWORD newMasterPlayerIndex, BYTE masterRank, BYTE exMasterRank );
void RGuildChangeMaster( LPMIDDLEQUERY, LPDBMESSAGE );

// 091105 ONS ±æµå¸¶Å© »èÁ¦
void GuildMarkDelete(DWORD GuildIdx, DWORD CharacterIdx);
void RGuildMarkDelete( LPQUERY, LPDBMESSAGE );

void GuildSelectForRewardByWar(DWORD winnerGuildIndex, DWORD winnerUnionIndex, DWORD loserGuildIndex, DWORD loserUnionIndex);
void RGuildSelectForReward(LPQUERY, LPDBMESSAGE);
void CharacterLogoutPointUpdate(DWORD playerIndex, MAPTYPE, float positionX, float positionZ);
void RSaveNpcMapChangePointReturn(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 071023 LYW --- MapDBMsgParser : Æ©Åä¸®¾ó ·Îµå Äõ¸®¹® Ã³¸®¿¡ ´ëÇÑ °á°ú Ã³¸®¸¦ ÇÏ±â À§ÇÑ ÇÔ¼ö.
void RTutorialLoad(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 071023 LYW --- MapDBMsgParser : Æ©Åä¸®¾ó ¾÷µ¥ÀÌÆ® Äõ¸®¹® Ã³¸®¿¡ ´ëÇÑ °á°ú Ã³¸®¸¦ ÇÏ±â À§ÇÑ ÇÔ¼ö.
void RTutorialUpdate(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 071207 LYW --- MapDBMsgParser : ½ºÅÈ ÃÊ±âÈ­ ÇÁ·Î½ÃÀú ½ÇÇà °á°ú ÇÔ¼ö.
void RUpdateToDefaultStats(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 071129 LYW --- MapDBMsgParser : ½ºÅ³ ÃÊ±âÈ­ ÇÁ·Î½ÃÀú ½ÇÇà °á°ú ÇÔ¼ö.
void RSkill_Reset(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 080602 LUJ, Àü¼­¹ö ´ë»ó ½ºÅ³ÀÌ »ç¿ë °¡´ÉÇÑÁö ¾Ë¸°´Ù
void RSkillCheckUsable( LPMIDDLEQUERY, LPDBMESSAGE );

// 080602 LUJ, ÆÐ¹Ð¸® ¸â¹ö ¸ñ·ÏÀ» Äõ¸®ÇØ¼­ ½ºÅ³À» Àû¿ëÇÑ´Ù. ÆÐ¹Ð¸® Á¤º¸°¡ ¸Ê ¼­¹ö¿¡ ¾ø°í
//				¿¡ÀÌÀüÆ®¿¡ ÀÖ±â ¶§¹®¿¡ ¹ø°Å·ÓÁö¸¸ DB¿¡¼­ Á¶È¸ÇÏ´Â ÆíÀÌ ÆíÇÏ´Ù.
//
//				ÁÖÀÇ: ¹Ìµé Äõ¸®ÀÌ¹Ç·Î ÃÖ´ë 20°³¹Û¿¡ Äõ¸®ÇÏÁö ¸øÇÑ´Ù. ±×·¯³ª ÆÐ¹Ð¸® ÃÖ´ë ÀÎ¿øÀÌ ´Ù¼¸¸íÀÌ¹Ç·Î »ç¿ëÇÑ´Ù
void RSkillAddToFamily( LPMIDDLEQUERY, LPDBMESSAGE );

void ChallengeZone_EnterFreq_Save(DWORD nPlayerID, DWORD nEnterFreq, DWORD nEnterBonusFreq);
void ChallengeZone_EnterFreq_Load(DWORD nPlayerID);
void RChallengeZone_EnterFreq_Load(LPQUERY pData, LPDBMESSAGE pMessage);
void ChallengeZone_Success_Save(DWORD nPlayerID, DWORD nPartnerID, UINT nSection, UINT nClearTime);
void ChallengeZone_ClearNo1_Load(DWORD nPlayerID, UINT nSection);
void RChallengeZone_ClearNo1_Load(LPQUERY pData, LPDBMESSAGE pMessage);
//091124 ONS Ã§¸°ÁöÁ¸ °æÇèÄ¡ºÐ¹è ÀúÀå/·Îµå Ã³¸®Ãß°¡
void ChallengeZone_ExpRate_Save(DWORD nPlayerID, DWORD dwExpRate);
void ChallengeZone_ExpRate_Load(DWORD nPlayerID, DWORD nPartnerID, UINT nSection);
void RChallengeZone_ExpRate_Load(LPQUERY pData, LPDBMESSAGE pMessage);
// E µ¥ÀÌÆ® Á¸ Ãß°¡ added by hseos 2007.11.26	2007.11.27	2007.12.01	2008.01.23

void CharacterBuffLoad( DWORD characteridx );
void RCharacterBuffLoad(LPQUERY pData, LPDBMESSAGE pMessage);
// 090204 LUJ, ¼öÄ¡ ¹üÀ§¸¦ È®ÀåÇÔ
void CharacterBuffAdd( DWORD characteridx, DWORD skillidx, DWORD remaintime, int count, BOOL isEndDate );
void CharacterBuffUpdate( DWORD characteridx, DWORD skillidx, DWORD remaintime, int count );
void CharacterBuffRemove( DWORD characteridx, DWORD skillidx );

void ConnectionCheck();

// 071210 KTH -- ÀÎº¥Åä¸® È®Àå
void IncreaseCharacterInventory( DWORD characteridx );
// 071210 KTH -- È®Àå ÀÎº¥Åä¸® °á°ú°ªÀ» ¹Þ¾Æ¿Â´Ù.
void RIncreaseCharacterInventory(LPQUERY pData, LPDBMESSAGE pMessage);
// 071212 KTH -- È®Àå ÀÎº¥Åä¸®¸¦ ÃÊ±âÈ­ ½ÃÄÑÁØ´Ù.
void ResetCharacterInventory( DWORD characteridx );

void ChangeCharacterName( char* szName, int characterIdx, DWORD itemIdx, POSTYPE pos);
void RChangeCharacterName(LPQUERY pData, LPDBMESSAGE pMessage);
//---KES AUTONOTE
void AutoNoteListAdd( DWORD dwCharacterIdx, char* pStrAutoName, DWORD dwAutoCharacterIdx, DWORD dwAutoUserIdx );
void RAutoNoteListAdd( LPQUERY pData, LPDBMESSAGE pMessage );
void AutoNoteListLoad( DWORD dwCharacterIdx  );
void RAutoNoteListLoad( LPQUERY pData, LPDBMESSAGE pMessage );
//---------------

// 080204 KTH -- ÀÌº¥Æ® °ü·Ã Äõ¸®
void UpdateEventFromGame( DWORD dwCharacterIdx, char* pCharacterName );

void GiftItemInsert( DWORD characterIdx, DWORD useridx, DWORD eventIdx, DWORD itemIdx, DWORD count, BOOL isStack);
void RGiftItemInsert( LPQUERY pData, LPDBMESSAGE pMessage );

// 080424 NYJ --- ³¬½Ã°ü·Ã
void FishingData_Load(DWORD dwPlayerID);
void RFishingData_Load(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);
// 080602 LYW --- Player : °æÇèÄ¡ ¼öÄ¡ (__int32) ¿¡¼­ (__int64) »çÀÌÁî·Î º¯°æ Ã³¸®.
//void FishingData_Update(DWORD dwPlayerID, WORD wFishingLevel, DWORD dwFishingExp, DWORD dwFishPoint)
void FishingData_Update(DWORD dwPlayerID, WORD wFishingLevel, EXPTYPE dwFishingExp, DWORD dwFishPoint) ;
// 080808 LUJ, ³¬½Ã °æÇèÄ¡/·¹º§ Ãß°¡, ´õ ÀÌ»ó ³¬½Ã Æ÷ÀÎÆ®¸¸ ÀúÀåÇÏ´Â ·Î±×°¡ ¾Æ´Ï¹Ç·Î ÇÔ¼ö¸íÀ» º¯°æ
void Log_Fishing(DWORD dwPlayerID, CHAR cLogType, DWORD dwDiffPoint, DWORD dwTotalPoint, DWORD dwBuyItemIdx, DWORD dwUseFish, EXPTYPE dwExp, DWORD dwLevel );

// ¿ä¸®°ü·Ã
void CookingData_Load(DWORD dwPlayerID);
void RCookingData_Load(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);
void Cooking_Update(CPlayer* pPlayer) ;

void Cooking_Recipe_Load(DWORD dwPlayerID);
void RCooking_Recipe_Load(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);
void Cooking_Recipe_Update(DWORD dwPlayerID, int nKind, DWORD dwRecipe, POSTYPE pos, DWORD dwRemainTime);

void Cooking_Log(DWORD dwPlayerID, int nKind, DWORD dwItemIdx, DWORD dwBeforeValue, DWORD dwDiffValue, DWORD dwAfterValue);
void CookRecipe_Log(DWORD dwPlayerID, DWORD dwRecipeIdx, POSTYPE slot, int nKind, DWORD dwRemainTime);

// ÇÏ¿ìÂ¡°ü·Ã
void HouseData_Load(DWORD dwPlayerID);
void RHouseData_Load(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void HouseCreate(DWORD dwConnectionIndex, DWORD dwPlayerMapNum, DWORD dwPlayerID, char* pCreateName);
void RHouseCreate(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void HouseUpdate(stHouseInfo* pHouseInfo);

void HouseEntrance(DWORD dwConnectionIndex, DWORD dwPlayerMapNum, CHAR cKind, DWORD dwValue1, DWORD dwValue2, DWORD dwPlayerID, char* pOwnerName);
void RHouseEntrance(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void HouseFurnitureLoad(DWORD dwOwnerIndex, DWORD dwStartIndex, DWORD dwQueryServerConnectionIndex, DWORD dwPlayerMapNum, CHAR cKind, DWORD dwValue1, DWORD dwValue2, DWORD dwQueryPlayerID);
void RHouseFurnitureLoad(LPQUERY pData, LPDBMESSAGE pMessage);

void HouseFurnitureUpdate(DWORD dwPlayerID, int nKind, DWORD dwUserIndex, DWORD dwFurnitureIndex, DWORD dwLinkItemIndex, BYTE byMtrlIndex,
						  float fPosX, float fPosY, float fPosZ, float fAngle, BYTE bySlot, BYTE byState, DWORD dwEndTime, BOOL bNotDelete=FALSE);

void HouseDelete(DWORD dwConnectionIndex, DWORD dwPlayerMapNum, DWORD dwPlayerID, char* pOwnerName);
void RHouseDelete(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void HouseRankLoad();
void HouseRankUpdate();
void RHouseRank(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void Log_Housing(DWORD dwOwnerIndex, char* pCharName, CHAR cLogType, DWORD dwItemDBIndex, DWORD dwItemIndex, DWORD dwItemValue, 
				 DWORD dwFurnitureIndex, BYTE bySlot, char* pHouseName, DWORD dwDecoPoint, DWORD dwHousePoint);
// ÀÎ´ø°ü·Ã
void DungeonEntrance(DWORD dwConnectionIndex, DWORD dwPlayerMapNum, DWORD dwPlayerIndex, DWORD dwPartyIndex, DWORD dwKeyIndex, DWORD dwItemIndex, DWORD dwItemSLot, WORD wDungeonMapNum, eDIFFICULTY difficulty);
void RDungeonEntrance(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);
void DungeonUpdate(DWORD dwPlayerIndex, DWORD wDungeonMapNum, DWORD wTotalVisit, DWORD wDailyVisit, BYTE byDoRollBack = 0);
// ±æµåÅä³Ê¸ÕÆ®
void RGTCheckRegist(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void GTRewardItemInsert(DWORD characterIdx, DWORD gtcount, DWORD itemIdx, DWORD count, BOOL isStack, DWORD endtime);
void RGTRewardItemInsert(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

// 091201 ONS ±æÅä°ñµå º¸»ó Ãß°¡
void GTRewardMoneyInsert(DWORD characterIdx, DWORD dwMoney);
void RGTRewardMoneyInsert(LPQUERY pData, LPDBMESSAGE pMessage);

void GTInfoLoad();
void RGTInfoLoad(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void GTInfoLoadAll(WORD wGTCount);
void RGTInfoLoadAll(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void GTInfoInsert(WORD wGTCount, DWORD dwGuildIdx, char* pGuildName, WORD wPosition, WORD wRound);
void GTInfoUpdate(WORD wGTCount, DWORD dwGuildIdx, WORD wPosition, WORD wRound);
void GTPlayerInsert(WORD wGTCount, DWORD dwGuildIdx, DWORD* pPlayers);
void GTPlayerLoad(WORD wGTCount, DWORD dwGuildIdx);
void RGTPlayerLoad(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void GTInfoReset(WORD wGTCount, WORD wRount);

void PetInfoLoad( DWORD characterIdx, DWORD userIdx );
// 090720 ONS Æê °íÁ¤½ºÅ³Á¤º¸¸¦ ÇÁ·Î½ÃÁ®¿¡ Ãß°¡ÇÑ´Ù.
void PetInfoAdd( DWORD characterIdx, DWORD userIdx, DWORD itemDBIdx, WORD kind, BYTE SkillSlot, DWORD SkillIdx, WORD Position );
void PetInfoRemove( DWORD characterIdx, DWORD itemDBIdx );
void PetInfoUpdate( DWORD characterIdx, DWORD itemDBIdx, BYTE level, BYTE grade, ePetType, BYTE slot, WORD exp, BYTE friendy, DWORD maxhp, DWORD hp, DWORD maxmp, DWORD mp, ePetAI, ePetState, DWORD time);
void PetMasterUpdate( DWORD characterIdx, DWORD userIdx, DWORD itemDBIdx );

void RPetInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void RPetInfoAdd( LPQUERY pData, LPDBMESSAGE pMessage );

void ItemMoveUpdatePetInven( DWORD characterIdx, DWORD userIdx, DWORD petIdx, DWORD fromItemIdx, DWORD fromItemPos, DWORD toItemIdx, DWORD toItemPos, BYTE param );

void PetItemInfoLoad( DWORD characterIdx, DWORD petIdx );
void RPetItemInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void LogPet( const PET_OBJECT_INFO&, ePetLog);
void Option_Load(DWORD dwPlayerIdx);
void ROption_Load(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);
void Option_Update(CPlayer&);
void CharSize_Update(CPlayer*);
void RSiegeRecallLoad(LPQUERY, LPDBMESSAGE);
void RSiegeWarfareInfoLoad(LPQUERY, LPDBMESSAGE);
void RSiegeWarWarterSeedComplete(LPQUERY, LPDBMESSAGE);
// 080822 LYW --- MapDBMsgParser : ¼ÒÈ¯ µÈ npc ·Îµå °á°ú Ã³¸® ÇÔ¼ö Ãß°¡.
void RNpcRemainTime_Load(LPQUERY pData, LPDBMESSAGE pMessage) ;
void RSiegeREcallInsert(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 081222 LYW --- MapDBMsgParser : Ä³ÅÍÆÞÆ® Å³ Ä«¿îÆ® °á°ú¸¦ ¹Þ´Â ÇÔ¼ö.
void RRequuestKillCount(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 081027 LUJ, °ø¼º ·Î±×
void InsertLogSiege( eSiegeLog, DWORD guildIndex = 0, DWORD playerIndex = 0, const char* memo = "" );

// 081031 LUJ, ¸®ÄÝÀ» À§ÇØ DB¿¡¼­ °¡Á®¿Í¾ßÇÏ´Â Á¤º¸¸¦ Äõ¸®ÇÑ´Ù
void RRecallDataLoad( LPMIDDLEQUERY, LPDBMESSAGE );
// 081218 LUJ, ¸Ê ÀÌµ¿ Àü¿¡ ÇÊ¼ö Á¤º¸¸¦ ¾÷µ¥ÀÌÆ®½ÃÅ²´Ù
void UpdatePlayerOnMapOut( DWORD playerIndex, DWORD userIndex, DWORD inventoryMoney, DWORD storageMoney, DWORD mapIndex, WORD channelIndex );
// 081218 LUJ, ÇÊ¼ö Á¤º¸°¡ Ã³¸®µÇ¾úÀ¸¸é ¸Ê ÀÌµ¿½ÃÅ²´Ù
void RUpdatePlayerOnMapOut( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage );
// 090115 LUJ, ½ºÅ©¸³Æ® ÇØÅ· ·Î±×¸¦ ÀÛ¼ºÇÑ´Ù
void LogScriptHack( CObject&, const TCHAR* scriptName = "", const TCHAR* memo = "" );

// 090413 ShinJS --- Ã§¸°Áö Á¸¿¡¼­ ³ª°¡´Â °æ¿ì
void CharacterLogoutPointUpdate_ToChallengeSavedPoint( DWORD dwCharacterIdx );
// 090316 LUJ, Å»°Í DB Äõ¸®
void LoadVehicleFromDb( const DWORD playerIndex, const MAPTYPE );
void RLoadVehicleFromDb( LPMIDDLEQUERY, LPDBMESSAGE );
void UpdateVehicleToDb( const DWORD playerIndex, const ICONBASE&, const MAPTYPE );
void LoadVehiclePassengerFromDb( const DWORD playerIndex, const MAPTYPE );
void RLoadVehiclePassengerFromDb( LPMIDDLEQUERY, LPDBMESSAGE );
void RemoveVehiclePassengerToDb( const DWORD playerIndex );
void ResetVehicle( MAPTYPE );

// 090609 ShinJS --- Add AutoNote Log
void LogAutoNote( eAutoNoteLogKind LogType, CPlayer* pFromPlayer, CPlayer* pToPlayer );
// 091113 LUJ, Æ®¸®°ÅÀÇ ¸ñ·ÏÀ» ÀÐ¾î¿Â´Ù
void LoadTriggerPlayerFromDb(DWORD ownerIndex, DWORD startindex);
void RLoadTriggerPlayerFromDb(LPQUERY, LPDBMESSAGE);
void LoadTriggerServerFromDb(DWORD channedIndex, DWORD startindex);
void RLoadTriggerServerFromDb(LPQUERY, LPDBMESSAGE);
void RGetTriggerRepeatCount(LPMIDDLEQUERY, LPDBMESSAGE);
// 091126 pdy ³óÀåº¸À¯¿©ºÎ Ã¤Å©
void RCheckHaveFarmNumToDB(LPMIDDLEQUERY, LPDBMESSAGE);
void RProvidePCRoomItem(LPQUERY, LPDBMESSAGE);
void RLoadPCRoomPoint(LPMIDDLEQUERY, LPDBMESSAGE);
// 100113 ONS PC¹æbuffÁ¤º¸ DBUpdate
void DBUpdatePCRoomBuff( DWORD dwPlayerIdx, WORD wBuffState, WORD wBuffStage = 0, BYTE byDay = 0, WORD byLoginMap = 0 );
//100118 ONS Ä£±¸°ü¸® ´ÙÀÌ¾ó·Î±×¿¡ PC¹æÁ¤º¸°ü·Ã ±â´ÉÃß°¡
void FriendGetPCRoomInfo( DWORD dwPlayerIdx, DWORD dwPCRoomIdx );
void RGetPCRoomInfo(LPQUERY pData, LPDBMESSAGE pMessage);

// À¥ÀÌº¥Æ®
void WebEvent( DWORD dwUserIdx, DWORD type );


// 100510 ÆÇ¸Å´ëÇà
void Consignment_CheckDate(DWORD dwPlayerID);
void RConsignment_CheckDate(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void Consignment_GetList(DWORD dwPlayerID);
void RConsignment_GetList(LPQUERY pData, LPDBMESSAGE pMessage);

void Consignment_Regist(DWORD dwPlayerID, DWORD dwItemDBIdx, DWORD dwItemIndex, char* pPlayerName, char* pItemName, int nCategory1, int nCategory2, int nRareLevel, int nItemLevel, int nEnchant, DWORD dwDeposit, DWORD dwCommission, DWORD dwPrice, POSTYPE FromPos, WORD wStack);
void RConsignment_Regist(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void Consignment_SearchIndex(DWORD dwPlayerID, WORD wSortType, WORD wStartPage, DWORD* pItemList);
void Consignment_Search(DWORD dwPlayerID, WORD wSortType, WORD wStartPage, int nCategory1, int nCategory2, int nMinLevel, int nMaxLevel, int nRareLevel);
void RConsignment_Search(LPQUERY pData, LPDBMESSAGE pMessage);
void Consignment_Update(DWORD dwPlayerID, DWORD dwConsignmentIdx, WORD wKind, DWORD dwDurability, ITEM_SEAL_TYPE);
void RConsignment_Update(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void Note_CheckDate(DWORD dwPlayerID);
void RNote_CheckDate(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

void Note_DelByDate(DWORD dwPlayerID, DWORD dwNoteIndex, DWORD dwPackageItemIdx, DWORD dwPackageMoney);

void Note_GetPackage(DWORD dwPlayerID, DWORD dwNoteID, WORD wEmptySlot, DWORD dwItemDBIdx, DWORD dwMoney);
void RNote_GetPackage(LPQUERY pData, LPDBMESSAGE pMessage);

void Consignment_Log(DWORD dwConsignmentIdx, DWORD dwPlayerID, int nKind, DWORD dwDBItemIdx, DWORD dwItemIndex, DWORD dwChangeDurability, DWORD dwPrice);

// 100607 ¾ÆÀÌÅÛÁö±Þ To ¿ìÆíÇÔ
void ItemInsertToNote(DWORD dwPlayerID, DWORD dwItemIndex, DWORD dwDurability, ITEM_SEAL_TYPE, DWORD dwMoney, WORD wKind, DWORD dwNameMsgIdx, DWORD dwTitleMsgIdx, char* pParam);
void RItemInsertToNote(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

// 100611 ONS Ã¤ÆÃ±ÝÁö ±â´É °ü·Ã 
void ForbidChatUpdate( DWORD dwPlayerIdx, DWORD dwTargetIdx, const char* pReason, DWORD dwForbidTime, BOOL bIsForbidAdd );
void RForbidChatUpdate( LPQUERY pData, LPDBMESSAGE pMessage );
void ForbidChatLoad( DWORD dwPlayerIdx );
void RForbidChatLoad( LPQUERY pData, LPDBMESSAGE pMessage );

// ---- skr sorting :
void InvenSort( DWORD dwPlayerID );
void RInvenSort( LPQUERY pData, LPDBMESSAGE pMessage );
// --- skr : warehouse 2020agt27
void WarehouseItemInfoSet(DWORD CharacterIdx, DWORD UserIdx, DWORD StartDBIdx, DWORD startpos, DWORD endpos, DWORD numset);
void RWarehouseItemInfoSet( LPQUERY pData, LPDBMESSAGE pMessage );

#endif