#include "stdafx.h"
#include "CommonDBMsgParser.h"
#include "AgentDBMsgParser.h"
#include "DataBase.h"
#include "UserTable.h"
#include "ServerTable.h"
#include "Console.h"
#include "Network.h"
#include "MsgTable.h"
#include "ServerSystem.h"
#include "GMPowerList.h"
// S �йи� �߰� added by hseos 2007.07.09
#include "hseos/Family/SHFamilyManager.h"
#include "hseos/Farm/SHFarmManager.h"
// E �йи� �߰� added by hseos 2007.07.09
#include "AgentNetworkMsgParser.h"


//---KES PUNISH
#include "PunishManager.h"
//-------------

// 080507 LYW --- AgentDBMsgParser : ä�ù� �Ŵ��� ��� ����.
#include "ChatRoomMgr.h"

// 080813 LYW --- AgentDBMsgParser : ���� ��ȯ�� �Ŵ��� ��� ����.
#include "SiegeRecallMgr.h"

// 080822 LYW --- AgentDBMsgParser : Npc ��ȯ �Ŵ��� ��� ����.
#include "NpcRecallMgr.h"

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif
#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif
#include ".\giftmanager.h"

//MSG_CHAT g_WisperTemp;
//MSG_CHAT g_MunpaTemp;
//MSG_CHAT g_PartyTemp;

extern int g_nServerSetNum;

//-----------------------------------------------------------------------
// DB?õ¸® ¸®?? ??¼ö°¡ ´?¾î³¯¶§¸¶´? ??°¡
// enum Query ?? ¼ø¼­°¡ ¹?µå½?!!!! ???¡??¾? ??´?.
DBMsgFunc g_DBMsgFunc[MaxQuery] =
{
	NULL,
	RUserIDXSendAndCharacterBaseInfo,	// ?³¸¯?? ¸®½º?® Query
	RCreateCharacter,
	NULL,
	RDeleteCharacter,
	RCharacterNameCheck,
	NULL,
	NULL,
	RSearchWhisperUserAndSend, /// ?³¸¯¸í?¸·? ?¢¼?¿©º?¹? ¸?¹ø?£ ¾ò¾î¿?±â
	NULL,							//SavePoint 
	NULL,							/// ¸?¿¡¼­ ³ª°¡¸é ½?????´?
	NULL,							/// ¸?¼­¹ö ?¾·á½? ½???
	RFriendIsValidTarget,		//FriendGetUserIDXbyName
	RFriendAddFriend,
	RFriendIsValidTarget,
	RFriendDelFriend,
	RFriendDelFriendID,
	NULL, //?£±¸¸®½º?® »è?¦(?³¸¯?? »è?¦½?)
	RFriendNotifyLogintoClient,
	RFriendGetFriendList,
	RFriendGetLoginFriends,
	RNoteIsNewNote,
	NULL,
	RNoteDelete,
	RNoteSave,
//For GM-Tool	
	RGM_BanCharacter,
	RGM_UpdateUserLevel,			/// eGM_UpdateUserLevel,
	RGM_WhereIsCharacter,
	RGM_Login,
	RGM_GetGMPowerList,
//	
	NULL,
	RCheckGuildMasterLogin,			// checkguildmasterlogin
	RCheckGuildFieldWarMoney,		// check guildfieldwarmoney
	RAddGuildFieldWarMoney,			// addd guildfieldwarmoney
	NULL,							// 051108 event
	NULL,							// eEventItemUse2

	RGM_UpdateUserState,

	NULL,							// eLogGMToolUse

	NULL,

	NULL,
	NULL,
	// desc_hseos_��������01
	// S �������� �߰� added by hseos 2007.06.15
	NULL,							// eInitMonstermeterInfoOfDB
	RGetUserSexKind,				// eGetUserSexKind
	// E �������� �߰� added by hseos 2007.06.15

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.09	2007.07.10	2007.07.14	2007.10.11
	RFamily_LoadInfo,					// eFamily_LoadInfo
	RFamily_SaveInfo,					// eFamily_SaveInfo
	RFamily_Member_LoadInfo,			// eFamily_Member_LoadInfo
	RFamily_Member_LoadExInfo01,		// eFamily_Member_LoadExInfo01,
	NULL,								// eFamily_Member_SaveInfo
	RFamily_CheckName,					// eFamily_CheckName
	RFamily_Leave_LoadInfo,				// eFamily_Leave_LoadInfo
	NULL,								// eFamily_Leave_SaveInfo
	NULL,								// 091111 ONS �йи� ������� �߰�
	RFarm_SendNote_DelFarm,
	NULL,								//ePunishList_Add,	//---������
	RPunishListLoad,					//ePunishList_Load,	//---������
	RPunishCountAdd,					//ePunishCount_Add, //---�ɸ� ȸ�� �߰�

	RGiftEvent,							//eGiftEvent,
//-------------

  	// 080812 LYW --- AgentDBMsgParser : ��ȯ�� db ������Ʈ ó�� �߰�.
  	RSiegeRecallUpdate,	// eSiegeRecallUpdate,
  
  	// 080813 LYW --- AgentDBMsgParser : ��ȯ�� db ���� �ε� ó�� �߰�.
  	RSiegeRecallLoad,	//eSiegeRecallLoad,
  
  	// 080831 LYW --- AgentDBMsgParser : ��ȯ�� db ���� �߰� ó�� �߰�.
  	RSiegeRecallInsert,
  
  	// 080831 LYW --- AgentDBMsgParser : ��ȯ�� db ���� ���� ó�� �߰�.
  	NULL,	// eSiegeRecallRemove,
  
	// 090525 ShinJS --- �̸����� ��Ƽ�ʴ�� DB �˻� ó�� �߰�
	RSearchCharacterForInvitePartyMember,
};	

// ?´¸¯?? ¸®½º?® °¡?®¿?´? DB?õ¸®
void UserIDXSendAndCharacterBaseInfo(DWORD UserIDX, DWORD AuthKey, DWORD dwConnectionIndex)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eCharacterBaseQuery, dwConnectionIndex, "EXEC MP_CHARACTER_SELECTBYUSERIDX %d, %d", UserIDX, AuthKey);
}

void CheatLog(DWORD CharacterIDX,DWORD CheatKind)
{
	char txt[256];
	sprintf(txt,"INSERT TB_LogHacking (Character_idx,CheatKind,LogDate) values(%d,%d,getdate())",
				CharacterIDX,CheatKind);
	g_DB.LogQuery(eQueryType_FreeQuery,0,0,txt);
}

void CreateCharacter(CHARACTERMAKEINFO* pMChar, WORD ServerNo, DWORD dwConnectionIndex)
{
	CHARACTERMAKEINFO* pMsg = pMChar;
	char txt[512];

	// 100301 ShinJS --- �⺻ ������ ���ҽ����� �е��� ����
	BYTE byRace				= pMsg->RaceType;
	BYTE byClass			= pMsg->JobType - 1;
	const PlayerStat stat	= g_pServerSystem->GetBaseStatus( (RaceType)byRace, byClass + 1 );

	BYTE Str = (BYTE)stat.mStrength.mPlus;
	BYTE Dex = (BYTE)stat.mDexterity.mPlus;
	BYTE Vit = (BYTE)stat.mVitality.mPlus;
	BYTE Int = (BYTE)stat.mIntelligence.mPlus;
	BYTE Wis = (BYTE)stat.mWisdom.mPlus;

	DWORD item[3][2];

	item[0][0] = 11000001;
	item[0][1] = 12000063;

	item[1][0] = 11000187;
	item[1][1] = 12000032;

	item[2][0] = 11000249;
	item[2][1] = 12000001;

	char ip[16];
	WORD port;
	g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

	int LoginPoint = 2019;
	BYTE bStartMap = 20;
	
	// 090325 ONS ���͸�����üũ
	char szBufName[MAX_NAME_LENGTH+1];
	char szBufIp[MAX_IPADDRESS_SIZE];

	SafeStrCpy(szBufName, pMsg->Name, MAX_NAME_LENGTH+1);
	SafeStrCpy(szBufIp, ip, MAX_IPADDRESS_SIZE);

	if(IsCharInString(szBufName, "'") || IsCharInString(szBufIp, "'"))	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, \'%s\', %u, %u", 
		"MP_CHARACTER_CREATECHARACTER", pMsg->UserID, Str, Dex, Vit, Int, Wis, 
		szBufName, pMsg->FaceType, pMsg->HairType, bStartMap, pMsg->SexType,
		pMsg->RaceType, pMsg->JobType, LoginPoint, ServerNo, szBufIp, item[byClass%3][0], item[byClass%3][1]);
		
	if(g_DB.Query(eQueryType_FreeQuery, eCreateCharacter, dwConnectionIndex, txt) == FALSE)
	{
	}

}

void CharacterNameCheck(char* pName, DWORD dwConnectionIndex)
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(pName, "'"))	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eNewCharacterCheckName, dwConnectionIndex, "EXEC %s \'%s\'", "MP_CHARACTER_NAMECHECK", pName);
}

void LoginCheckDelete(DWORD UserID)//, DWORD dwConnectionIndex)
{
	// 090710 LUJ, �α׾ƿ� ó�� �� ������ ��ȣ�� ������
	char txt[ MAX_PATH ] = { 0 };
	sprintf( txt, "EXEC DBO.UP_GAMELOGOUT %d, %d", UserID, g_nServerSetNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eLoginCheckDelete, 0, txt );
}

void DeleteCharacter(DWORD dwPlayerID, WORD ServerNo, DWORD dwConnectionIndex)
{
	USERINFO* pinfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pinfo)
		return;
	CHARSELECTINFO * SelectInfoArray = (CHARSELECTINFO*)pinfo->SelectInfoArray;
	
	for(int i = 0; i < MAX_CHARACTER_NUM; i++)
	{
		if(SelectInfoArray[i].dwCharacterID == dwPlayerID)
			break;
		if(i == MAX_CHARACTER_NUM - 1)	// ?¯?ú°¡ °¡?ö°í ??´? ?³¸¯°ú ´?¸¥ ?³¸¯?? ¼±??µ?
		{
			// ¿¬°áµ? ?³¸¯°ú ?ö¿ï?³¸¯?? ???¡???ö ¾?½?´?´?
			return;
		}
	}

	char ip[16];
	WORD port;
	g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

	// 090325 ONS ���͸�����üũ
	char szBufIp[MAX_IPADDRESS_SIZE];
	SafeStrCpy(szBufIp, ip, MAX_IPADDRESS_SIZE);
	if(IsCharInString(szBufIp, "'"))	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eDeleteCharacter, dwConnectionIndex, "EXEC %s %d, %d, \'%s\'", "MP_CHARACTER_DELETECHARACTER", dwPlayerID, ServerNo, szBufIp );
}

void SearchWhisperUserAndSend( DWORD dwPlayerID, char* CharacterName, DWORD dwKey )
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(CharacterName, "'"))	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eLoginMapInfoSearchForName, dwKey, "EXEC %s \'%s\', %d", "MP_LOGINCHARACTERSEARCHFORNAME", CharacterName, dwPlayerID );
}

void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eSavePoint, CharacterIDX, "EXEC  %s %d, %d", "MP_CHARACTER_MAPCHANGEPOINTUPDATE", CharacterIDX, MapChangePoint_Idx);
	
}

void LoadCharacterMap(DWORD CharacterIDX)
{
	g_DB.FreeMiddleQuery(RLoadCharacterMap, CharacterIDX, "EXEC %s %d", "MP_CHARACTER_CharacterMapLoad", CharacterIDX);
}

void RLoadCharacterMap(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage )
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	DWORD dwMapNum = (DWORD)atoi((char*)pData[0].Data[1]);

	MSG_DWORD3 stPacket;

	stPacket.Category	= MP_USERCONN;
	stPacket.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
	stPacket.dwObjectID	= pRecverInfo->dwCharacterID;
	stPacket.dwData1	= g_pServerSystem->GetMapChangeIndex(dwMapNum);
	UserConn_ChangeMap_Syn(pRecverInfo->dwConnectionIndex, (char*)&stPacket, sizeof(stPacket) );
}

void UnRegistLoginMapInfo(DWORD CharacterIDX)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eUnRegistLoginMapInfo, CharacterIDX, "EXEC %s %d", "MP_LOGINMAPINFO_UNREGIST", CharacterIDX);
}

void FriendGetUserIDXbyName(DWORD CharacterIDX, char* TargetName)
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(TargetName, "'"))	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eFriendGetTargetMemeberIDX, CharacterIDX, "EXEC %s \'%s\', %u", "MP_FRIEND_GETTARGETIDX", TargetName, CharacterIDX);
}

void FriendAddFriend(DWORD CharacterIDX, DWORD TargetID) //CharacterIDX : ½??»??, TargetID : ½?³«??
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eFriendAdd, CharacterIDX, "EXEC %s %u, %u", "MP_FRIEND_ADDFRIEND", CharacterIDX, TargetID);
}

// 100305 ONS ������θ� �����ʰ� ģ���� �߰��ϵ��� ����.
void FriendAddFriendByName(DWORD CharacterIDX, char* TargetName) //CharacterIDX : ½??»??, TargetID : ½?³«??
{
	if(IsCharInString(TargetName, "'"))	return;

	g_DB.FreeQuery(eFriendAdd, CharacterIDX, "EXEC %s %u, \'%s\'", "MP_FRIEND_ADDFRIEND", CharacterIDX, TargetName);
}

void FriendIsValidTarget(DWORD CharacterIDX, DWORD TargetID, char* FromName)
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(FromName, "'"))	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eFriendIsValidTarget, CharacterIDX, "EXEC %s %d, %d, \'%s\'", "MP_FRIEND_ISVALIDTARGET", CharacterIDX, TargetID, FromName);
}

void FriendDelFriend(DWORD CharacterIDX, char* TargetName)
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(TargetName, "'"))	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eFriendDel, CharacterIDX, "EXEC %s %u, \'%s\'", "MP_FRIEND_DELFRIEND", CharacterIDX, TargetName);
}

void FriendDelFriendID(DWORD CharacterIDX, DWORD TargetID, DWORD bLast)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eFriendDelID, CharacterIDX, "EXEC %s %u, %u, %d", "MP_FRIEND_DELFRIENDID", CharacterIDX, TargetID, bLast);
}

void FriendNotifyLogintoClient(DWORD CharacterIDX)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eFriendNotifyLogin, CharacterIDX, "EXEC %s %u", "MP_FRIEND_NOTIFYLOGIN", CharacterIDX); //³ª¸¦ µî·??? »ç¶÷µé
}

void FriendGetLoginFriends(DWORD CharacterIDX)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eFriendGetLoginFriends, CharacterIDX, "EXEC %s %u", "MP_FRIEND_LOGINFRIEND", CharacterIDX);//³»°¡ µî·??? »ç¶÷µé
}

void FriendGetFriendList(DWORD CharacterIDX)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eFriendGetFriendList, CharacterIDX, "EXEC %s %u", "MP_FRIEND_GETFRIENDLIST", CharacterIDX);
}

void NoteIsNewNote(DWORD PlayerID)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eNoteIsNewNote, PlayerID, "EXEC %s %u", "MP_NOTE_ISNEWNOTE", PlayerID);
}

void NoteSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Title, char* Note)
{
	// 090325 ONS ���͸�����üũ
	if( IsCharInString(ToName, "'") || IsCharInString(FromName, "'") || IsCharInString(Title, "'") || IsCharInString(Note, "'") )	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeLargeQuery(RNoteSendtoPlayer, FromIDX, "EXEC %s \'%s\', \'%s\', \'%s\', \'%s\', %d, %d", "MP_NOTE_SENDNOTE", ToName, FromName, Title, Note, 0, 0);	
}

void NoteServerSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Title, char* Note)
{
	// 090325 ONS ���͸�����üũ
	if( IsCharInString(ToName, "'") || IsCharInString(FromName, "'") || IsCharInString(Title, "'") || IsCharInString(Note, "'") )	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeLargeQuery(RNoteServerSendtoPlayer, FromIDX, "EXEC %s \'%s\', \'%s\', \'%s\', \'%s\', %d, %d", "MP_NOTE_SENDNOTE", ToName, FromName, Title, Note, 0, 0);	
}

void NoteSendtoPlayerID(DWORD FromIDX, char* FromName, DWORD ToIDX, char* Note)
{
	// 090325 ONS ���͸�����üũ
	if( IsCharInString(FromName, "'") || IsCharInString(Note, "'") )	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeLargeQuery(RNoteSendtoPlayer, FromIDX, "EXEC %s %u, \'%s\', %u, \'%s\'", "MP_NOTE_SENDNOTEID", FromIDX, FromName, ToIDX, Note);
}

void NoteDelAll(DWORD CharacterIDX)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eNoteDelAll, 0, "EXEC %s %u", "MP_NOTE_DELALLNOTE", CharacterIDX);
}

void NoteList(DWORD CharacterIDX, WORD Page, WORD Mode)
{	
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(CharacterIDX);
	if(!userinfo)
		return;
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeMiddleQuery(RNoteList, CharacterIDX, "EXEC %s %u, %d, %u, %d", "MP_NOTE_GETNOTELIST", CharacterIDX, 11, Page, Mode);
}

void NoteRead(DWORD CharacterIDX, DWORD NoteIDX, DWORD IsFront)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeLargeQuery(RNoteRead, CharacterIDX, "EXEC %s %u, %u, %u", "MP_NOTE_READNOTE", CharacterIDX, NoteIDX, IsFront);
}

void NoteDelete(DWORD PlayerID, DWORD NoteID, BOOL bLast)
{	
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eNoteDelete, PlayerID, "EXEC %s %d, %d, %d, %d", "MP_NOTE_DELNOTE", PlayerID, FALSE/*@bUserDelete*/, NoteID, bLast);
}

void NoteSave(DWORD PlayerID, DWORD NoteID, BOOL bLast)
{
	g_DB.FreeQuery(eNoteSave, PlayerID, "EXEC %s %u, %u, %d", "MP_NOTE_SAVENOTE", PlayerID, NoteID, bLast);
}

//---for GM_Tool
void GM_WhereIsCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID )
{
	// 090325 ONS ���͸�����üũ
	if( IsCharInString(CharacterName, "'") )	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eGM_WhereIsCharacter, dwID, "EXEC %s \'%s\', %d", "MP_LOGINCHARACTERSEARCHFORNAME", CharacterName, dwSeacherID );
}

void GM_BanCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID )
{
	// 090325 ONS ���͸�����üũ
	if( IsCharInString(CharacterName, "'") )	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery(eGM_BanCharacter, dwID, "EXEC %s \'%s\', %d", "MP_LOGINCHARACTERSEARCHFORNAME", CharacterName, dwSeacherID );
}

void GM_UpdateUserLevel(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserLevel)
{
	// 090325 ONS ���͸�����üũ
	if( IsCharInString(Charactername, "'") )	return;

    char txt[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC %s %d, \'%s\', %d", "MP_GMTOOL_UPDATEUSERLEVEL", dwServerGroup, Charactername, UserLevel);
	g_DB.LoginQuery(eQueryType_FreeQuery, eGM_UpdateUserLevel, dwID, txt);
}

void GM_Login( DWORD dwConnectionIdx, char* strID, char* strPW, char* strIP )
{
	// 090325 ONS ���͸�����üũ
	if( IsCharInString(strID, "'") || IsCharInString(strPW, "'") || IsCharInString(strIP, "'"))	return;

	char txt[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC RP_OPERATORLOGINCHECK \'%s\', \'%s\', \'%s\'", strID, strPW, strIP);	
	g_DB.LoginQuery(eQueryType_FreeQuery, eGM_Login, dwConnectionIdx, txt);
}

void GM_GetGMPowerList( DWORD dwStartIdx, DWORD dwFlag )
{
	char txt[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC %s %d, %d", "RP_OPERATORINFO", dwStartIdx, dwFlag );
	g_DB.LoginQuery(eQueryType_FreeQuery, eGM_GetGMPowerList, 0, txt);
}

void	RUserIDXSendAndCharacterBaseInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(atoi((char*)pData[0].Data[0]) == 0)
		count = 0;
	DWORD AgentAuthKey = atoi((char*)pData[0].Data[eCL_AuthKey]);
	USERINFO* pInfo = g_pUserTable->FindUser(pMessage->dwID);
	if(pInfo == NULL)		// ??¹? ³ª°¬?½
		return;
	if(pInfo->dwUniqueConnectIdx != AgentAuthKey)	// ³ª°¡°í ´?¸¥³??? µé¾î¿?
		return;

	count = min(
		MAX_CHARACTER_NUM,
		count);

	SEND_CHARSELECT_INFO msg;
	ZeroMemory(
		&msg,
		sizeof(msg));

	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTERLIST_ACK;

//---KES Crypt
#ifdef _CRYPTCHECK_ 
	msg.eninit = *pInfo->crypto.GetEnKey();
	msg.deinit = *pInfo->crypto.GetDeKey();
#endif
//--------
	if( !count ) /// ¾??÷ ¸¸µç ?³¸¯??°¡ ¾ø´?.
	{
		msg.CharNum = 0;			// ?? ?«¿î?®°¡ 0??¸é ?³¸¯?? ??³ªµ? ¾ø´?°???´?

		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(SEND_CHARSELECT_INFO));

#ifdef _CRYPTCHECK_
		pInfo->crypto.SetInit( TRUE );		// init on	
#endif

#ifdef _NPROTECT_
		//�̰��� ���������� ���ü��� �ִ�.(ù�α��ν�, ĳ���ͻ���â����, ���ӿ���)
		if( pInfo->m_nCSAInit == 0 )	//ù ������ �ȵǾ� �ִٸ�
		{
			pInfo->m_nCSAInit = 1;		//ù ���� ����
			NPROTECTMGR->SendAuthQuery(pInfo);

		}
#endif

		return;
	}

	msg.CharNum = (BYTE)(count);
		
	for( DWORD i=0; i<count; i++)
	{
		// ?³¸¯?? ±âº»?¤º¸ ¼?????´?°÷
		msg.BaseObjectInfo[i].dwObjectID = atoi((char*)pData[i].Data[eCL_ObjectID]);
		msg.StandingArrayNum[i] = (WORD)atoi((char*)pData[i].Data[eCL_StandIndex]);
		SafeStrCpy(
			msg.BaseObjectInfo[i].ObjectName,
			(char*)pData[i].Data[eCL_ObjectName],
			_countof(msg.BaseObjectInfo[i].ObjectName));
		msg.BaseObjectInfo[i].ObjectState = eObjectState_None;
		msg.ChrTotalInfo[i].FaceType = (BYTE)atoi((char*)pData[i].Data[eCL_BodyType]);
		msg.ChrTotalInfo[i].HairType = (BYTE)atoi((char*)pData[i].Data[eCL_HeadType]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Hat] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData[i].Data[eCL_Weapon]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData[i].Data[eCL_Dress]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData[i].Data[eCL_shoes]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Earring1] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Earring2] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Glove] = atoi((char*)pData[i].Data[eCL_Glove]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Necklace] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Ring1] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Ring2] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Belt] = 0;	
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Shield] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Band] = 0;
		msg.ChrTotalInfo[i].Level = (LEVELTYPE)atoi((char*)pData[i].Data[eCL_Grade]);
		msg.ChrTotalInfo[i].LoginMapNum = (MAPTYPE)atoi((char*)pData[i].Data[eCL_Map]);
		msg.ChrTotalInfo[i].Gender = (BYTE)atoi((char*)pData[i].Data[eCL_Gender]);
		msg.ChrTotalInfo[i].Race = (BYTE)atoi((char*)pData[i].Data[eCL_Race]);
		msg.ChrTotalInfo[i].JobGrade = (BYTE)atoi((char*)pData[i].Data[eCL_JobGrade]);
		msg.ChrTotalInfo[i].Job[0] = (BYTE)atoi((char*)pData[i].Data[eCL_Job1]);
		msg.ChrTotalInfo[i].Job[1] = (BYTE)atoi((char*)pData[i].Data[eCL_Job2]);
		msg.ChrTotalInfo[i].Job[2] = (BYTE)atoi((char*)pData[i].Data[eCL_Job3]);
		msg.ChrTotalInfo[i].Job[3] = (BYTE)atoi((char*)pData[i].Data[eCL_Job4]);
		msg.ChrTotalInfo[i].Job[4] = (BYTE)atoi((char*)pData[i].Data[eCL_Job5]);
		msg.ChrTotalInfo[i].Job[5] = (BYTE)atoi((char*)pData[i].Data[eCL_Job6]);
		msg.ChrTotalInfo[i].bVisible = TRUE;
		SafeStrCpy( msg.ChrTotalInfo[i].GuildName, (char*)pData[i].Data[eCL_GuildName], MAX_GUILD_NAME+1 );

		msg.ChrTotalInfo[i].Height = 1;
		msg.ChrTotalInfo[i].Width = 1;
		pInfo->SelectInfoArray[i].dwCharacterID = msg.BaseObjectInfo[i].dwObjectID;
		pInfo->SelectInfoArray[i].Level = msg.ChrTotalInfo[i].Level;
		pInfo->SelectInfoArray[i].MapNum = msg.ChrTotalInfo[i].LoginMapNum;
		pInfo->SelectInfoArray[i].Gender = msg.ChrTotalInfo[i].Gender;
		SafeStrCpy(pInfo->SelectInfoArray[i].name, msg.BaseObjectInfo[i].ObjectName, 32) ;
	}
	
	g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(SEND_CHARSELECT_INFO));

#ifdef _CRYPTCHECK_
	pInfo->crypto.SetInit( TRUE );		// init on	
#endif

#ifdef _HACK_SHIELD_
		HACKSHIELDMGR->SendGUIDReq(pInfo);
#endif

#ifdef _NPROTECT_
		//�̰��� ���������� ���ü��� �ִ�.(ù�α��ν�, ĳ���ͻ���â����, ���ӿ���)
		if( pInfo->m_nCSAInit == 0 )	//ù ������ �ȵǾ� �ִٸ�
		{
			pInfo->m_nCSAInit = 1;		//ù ���� ����
			NPROTECTMGR->SendAuthQuery(pInfo);
		}
#endif
}

void SkillInsertToDB(DWORD CharacterIDX, DWORD SkillIDX, BYTE level/*, POSTYPE SkillPos, BYTE bWeared, BYTE bLevel, WORD Option*/)
{
	char txt[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC %s %u, %d, %d", "MP_SKILLTREE_INSERT", CharacterIDX, SkillIDX, level);
	g_DB.Query(eQueryType_FreeQuery, eSkillInsert, CharacterIDX, txt);
}

// desc_hseos_���͹���01
// S ���͹��� �߰� added by hseos 2007.05.29
// ..�÷��̽ð�, ����ų �� �ʱ�ȭ
void InitMonstermeterInfoOfDB(DWORD nPlayerID)
{
	char txt[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC  dbo.MP_MONSTERMETER_INITINFO %d", nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eInitMonstermeterInfoOfDB, 0, txt);
}
// E ���͹��� �߰� added by hseos 2007.05.29

// desc_hseos_��������01
// S �������� �߰� added by hseos 2007.06.15
void GetUserSexKind(DWORD nUserID)
{
	char txt[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC  MP_USER_SEXKIND %d", nUserID);
	g_DB.LoginQuery(eQueryType_FreeQuery, eGetUserSexKind, nUserID, txt);
}

void RGetUserSexKind(LPQUERY pData, LPDBMESSAGE pMessage)
{
	int result = atoi((char*)pData->Data[0]);
	DWORD dwConnectionIndex = pMessage->dwID;

	USERINFO* pInfo = g_pUserTableForUserID->FindUser(dwConnectionIndex);
	if(pInfo == NULL) return;

	pInfo->nSexKind = result;

	// Ŭ���̾�Ʈ�� �˸���
	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_USER_SEXKIND;
	msg.dwData	 = result;
	g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}
// E �������� �߰� added by hseos 2007.06.15

void	RCreateCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	int result = atoi((char*)pData->Data[0]);
	DWORD dwConnectionIndex = pMessage->dwID;

	switch(result)
	{
	case 0:
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}

			ASSERT(pInfo->dwUserID);

			if(pInfo->dwUserID == 0)
			{
				ASSERTMSG(0, "UserID°¡ 0??´?.");

				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}
			
			/// �⺻ ���� ������

			DWORD idx = atoi((char*)pData->Data[1]);
			
			int job = atoi((char*)pData->Data[2]);

			if( job == 4 )
			{
				WebEvent( pInfo->dwUserID, 4 );
			}
/*			
			int job = atoi((char*)pData->Data[2]);

			switch( job )
			{
			case 1:
				{
					SkillInsertToDB( idx, 1101101, 1 );
					SkillInsertToDB( idx, 1101201, 1 );
				}
				break;
			case 2:
				{
					SkillInsertToDB( idx, 2101001, 1 );
				}
				break;
			case 3:
				{
					SkillInsertToDB( idx, 3200101, 1 );
				}
				break;
			case 4:
				{
					// 091102 ONS ���� �⺻ ��ų ����
					SkillInsertToDB( idx, 6100101, 1 );
				}
			}
*/
			//UserIDXSendAndCharacterBaseInfo(pInfo->dwUserID,pInfo->dwUniqueConnectIdx,dwConnectionIndex);
			pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1].dwCharacterID = idx;
			// 090622 ONS �ű����� ���۸� ���� ó���߰�
			MAPTYPE dwMapNum = (MAPTYPE)atoi((char*)pData->Data[4]);
			pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1].MapNum =  dwMapNum;

			// 080430 LYW --- AgentDBMsgParser : ä�ù� �ý������� ����, ĳ���� �̸��� �߰���.
			pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].Level = 1 ;
			SafeStrCpy(pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].name, (char*)pData->Data[3], 32) ;

			// 080507 LYW --- AgentDBMsgParser : ä�ù� �ý����� ����, ���� ������ Dist ������ �߰��Ѵ�.
			ST_CR_USER user ;
			memset(&user, 0, sizeof(ST_CR_USER)) ;

			user.byLevel	= (BYTE)pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].Level ;
			user.byMapNum	= (BYTE)pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1].MapNum ;
			user.dwPlayerID	= pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1].dwCharacterID ;
			user.dwUserID	= pInfo->dwUserID ;

			SafeStrCpy(user.name, pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].name, MAX_NAME_LENGTH) ;

			// ���� ������ Dist�� ��Ͻ�Ű��, ������ ��� �� ���·� �����Ѵ�.
			if(CHATROOMMGR->RegistPlayer_To_Lobby(&user))
			{
				pInfo->byAddedChatSystem = TRUE ;
			}
			// END

			// 090625 ONS ���۸��� �ű������� ���������� �����Ͽ� �����ϵ��� �޼����� �߰�.
			MSG_DWORD2 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_MAKE_ACK;
			msg.dwData1 = idx;
			msg.dwData2 = dwMapNum;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 1:
		{
			// ?³¸¯??°¡ ²??¡´?.
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 2:
		{
			// ??¸§?? ??º¹µ? ¿?·ù
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = (WORD)result;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 3:
		{
			// ??¸§?? NULL??¶§
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = (WORD)result;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 4:
		{
			// ¹®?? ??¸§°ú °ã?¥ ¶§
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = (WORD)result;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	default:
		ASSERT(0);
		return;
	}
}
void	RCharacterNameCheck(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(atoi((char*)pData->Data[0])==0)
	{
		// ??¸§ ??º¹ ¾?µ?
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_ACK;
		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
	}
	else
	{
		// ??¸§?? ??º¹µ? ¿?·ù
		MSG_WORD msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
		msg.wData = 2;
		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
	}
}

void RSearchWhisperUserAndSend(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{
		//return;
		ASSERT(0);
	}
	else
	{
		MSG_CHAT* pMsgChat = g_MsgTable.GetMsg( pMessage->dwID );
		if( pMsgChat == NULL ) return;

		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMsgChat->dwObjectID );
		if( !pSenderInfo ) 
		{
			g_MsgTable.RemoveMsg( pMessage->dwID );
			return;
		}

		int nError	= atoi((char*)pData->Data[0]);
		int nLenEr	= strlen((char*)pData->Data[0]);	//050118 fix Error for ID including '1'
		if( nLenEr == 1 && ( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED ) )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHAT;
			msg.Protocol	= MP_CHAT_WHISPER_NACK;
			msg.dwObjectID	= pMsgChat->dwObjectID;
			msg.bData		= (BYTE)nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwReceiverID = (DWORD)atoi((char*)pData->Data[1]);
			USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( dwReceiverID );
			
			if( pReceiverInfo )	//¹??» »ç¶÷?? ?? ¼­¹ö¿¡ ??´??ö °?»ç
			{
				if( pReceiverInfo->GameOption.bNoWhisper && pSenderInfo->UserLevel != eUSERLEVEL_GM )
				{
					MSG_BYTE msg;
					msg.Category	= MP_CHAT;
					msg.Protocol	= MP_CHAT_WHISPER_NACK;
					msg.dwObjectID	= pMsgChat->dwObjectID;	//º¸³½»ç¶÷ ¾???µð
					msg.bData		= CHATERR_OPTION_NOWHISPER;

					g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				}
				else
				{
					//º¸³½ »ç¶÷¿¡°? º¸³»°í,
					MSG_CHAT msgToSender = *pMsgChat;
					msgToSender.Category = MP_CHAT;
					msgToSender.Protocol = MP_CHAT_WHISPER_ACK;
					g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msgToSender, msgToSender.GetMsgLength() );	//CHATMSG 040324

					//¹?´? »ç¶÷¿¡°? º¸³»°í,
					MSG_CHAT msgToReceiver = *pMsgChat;
					msgToReceiver.Category = MP_CHAT;
					if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
						msgToReceiver.Protocol = MP_CHAT_WHISPER_GM;
					else
						msgToReceiver.Protocol = MP_CHAT_WHISPER;
					SafeStrCpy( msgToReceiver.Name, (char*)pData->Data[0], MAX_NAME_LENGTH + 1 );	//º¸³½»ç¶÷?? ??¸§?¸·? ¹?²?
					g_Network.Send2User( pReceiverInfo->dwConnectionIndex, (char*)&msgToReceiver, msgToReceiver.GetMsgLength() );
				}
			}
			else
			{
				MSG_WHISPER msgWhisper;
				msgWhisper.Category		= MP_CHAT;
				if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
					msgWhisper.Protocol		= MP_CHAT_WHISPER_GM_SYN;
				else
					msgWhisper.Protocol		= MP_CHAT_WHISPER_SYN;

				msgWhisper.dwObjectID	= pMsgChat->dwObjectID;					//º¸³½»ç¶÷
				msgWhisper.dwReceiverID	= (DWORD)atoi((char*)pData->Data[1]);	//¹?´?»ç¶÷
				SafeStrCpy( msgWhisper.SenderName, (char*)pData->Data[0], MAX_NAME_LENGTH + 1 ); //º¸³½»ç¶÷?? ??¸§
				SafeStrCpy( msgWhisper.ReceiverName, pMsgChat->Name, MAX_NAME_LENGTH + 1 ); //¹?´?»ç¶÷?? ??¸§
				SafeStrCpy( msgWhisper.Msg, pMsgChat->Msg, MAX_CHAT_LENGTH + 1 );	//?¤??³»¿ë

				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msgWhisper, msgWhisper.GetMsgLength() );	//CHATMSG 040324
			}
		}
	}

	g_MsgTable.RemoveMsg( pMessage->dwID );
}

void RFriendDelFriend(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(pRecverInfo)
	{
		if(atoi((char*)pData->Data[eFr_IsSuccess]) != 0)
		{
			MSG_NAME_DWORD msg;
			msg.Category = MP_FRIEND;
			msg.Protocol = MP_FRIEND_DEL_ACK;
			SafeStrCpy( msg.Name, (char*)pData->Data[eFr_targetname], MAX_NAME_LENGTH + 1 );
			msg.dwData = atoi((char*)pData->Data[eFr_IsSuccess]); //ack ??¶§ friendidx return
			msg.dwObjectID = pMessage->dwID;

			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_NAME_DWORD));
		}
		else
		{
			MSG_FRIEND_MEMBER_ADDDELETEID msg;
			msg.Category	= MP_FRIEND;
			msg.Protocol	= MP_FRIEND_DEL_NACK;
			SafeStrCpy( msg.Name, (char*)pData->Data[eFr_targetname], MAX_NAME_LENGTH + 1 );
			msg.dwObjectID	= pMessage->dwID;
			msg.PlayerID	= atoi((char*)pData->Data[eFr_IsSuccess]);

			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
		}
	}
}


void RFriendAddFriend(LPQUERY pData, LPDBMESSAGE pMessage)
{
	//µî·?µ?¾î?ö´? »ç¶÷
	MSG_FRIEND_MEMBER_ADDDELETEID bmsg;
	bmsg.Category = MP_FRIEND;
	SafeStrCpy(bmsg.Name, (char*)pData->Data[eFr_addFromName], MAX_NAME_LENGTH+1);
	bmsg.dwObjectID = atoi((char*)pData->Data[eFr_addToIDX]);
	bmsg.PlayerID = atoi((char*)pData->Data[eFr_addFromIDX]);
	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData->Data[eFr_addToIDX]));
	if(pToRecverInfo)
	{
		if(atoi((char*)pData->Data[eFr_addToErr]) == 0) //ack
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_ACK;
		else	//nack
		{
			bmsg.PlayerID = atoi((char*)pData->Data[eFr_addToErr]);
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_NACK;
		}
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&bmsg, sizeof(bmsg));
	}
	else //another agent
	{
		if(atoi((char*)pData->Data[eFr_addToErr]) == 0) //ack
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_TO_AGENT;
		else //nack
		{
			bmsg.PlayerID = atoi((char*)pData->Data[eFr_addToErr]);
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_NACK_TO_AGENT;
		}
		g_Network.Broadcast2AgentServerExceptSelf((char*)&bmsg, sizeof(bmsg));
	}
}

void RFriendIsValidTarget(LPQUERY pData, LPDBMESSAGE pMessage)
{
	MSG_FRIEND_MEMBER_ADDDELETEID msg;
	memset(&msg, 0, sizeof(msg));

	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData->Data[eFr_vtTargetid]));
	if(!pSenderInfo)
		return;

	if(atoi((char*)pData->Data[eFr_Err]) != 0)
	{		
		//nack 
		msg.Category = MP_FRIEND;
		msg.dwObjectID = pMessage->dwID;
		msg.Protocol = MP_FRIEND_ADD_NACK;
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtToname], MAX_NAME_LENGTH + 1 );
		msg.PlayerID = atoi((char*)pData->Data[eFr_Err]);	//errcode insert

		g_Network.Send2User(pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		if(pRecverInfo)
		{
			if(pRecverInfo->GameOption.bNoFriend == TRUE)
			{
				msg.Category = MP_FRIEND;
				msg.dwObjectID = pMessage->dwID;
				msg.Protocol = MP_FRIEND_ADD_NACK;
				SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtToname], MAX_NAME_LENGTH + 1 );
				msg.PlayerID = eFriend_OptionNoFriend;	//errcode insert

				g_Network.Send2User(pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
			else
			{
				msg.Category = MP_FRIEND;
				msg.dwObjectID = atoi((char*)pData->Data[eFr_vtTargetid]);
				msg.Protocol = MP_FRIEND_ADD_INVITE;
				SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtFromname], MAX_NAME_LENGTH + 1 );
				msg.PlayerID = pMessage->dwID;
				g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
		}
		else
		{
			// ´?¸¥ Agent¿¡ ¼?????´?.
			msg.Category = MP_FRIEND;
			msg.Protocol = MP_FRIEND_ADD_INVITE_TO_AGENT;
			SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtFromname], MAX_NAME_LENGTH + 1 );
			msg.PlayerID = pMessage->dwID;
			
			msg.dwObjectID = atoi((char*)pData->Data[eFr_vtTargetid]);

			g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
		}
	}
}

void RFriendDelFriendID(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(pRecverInfo)
	{
		MSG_DWORD_WORD msg;
		msg.Category = MP_FRIEND;
		msg.Protocol = MP_FRIEND_DELID_ACK;
		msg.wData = (WORD)atoi((char*)pData->Data[0]); //bLast
		msg.dwData = atoi((char*)pData->Data[1]); //targetid
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}

void RFriendNotifyLogintoClient(LPQUERY pData, LPDBMESSAGE pMessage)
{
	MSG_NAME_DWORD msg;
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_LOGIN_NOTIFY;
	SafeStrCpy( msg.Name, (char*)pData[0].Data[eFr_LLoggedname], MAX_NAME_LENGTH + 1 );
	msg.dwData = pMessage->dwID;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData[i].Data[eFr_LTargetID]));
		msg.dwObjectID = atoi((char*)pData[i].Data[eFr_LTargetID]);
		if(pRecverInfo)
		{
			MSG_NAME_DWORD msgTemp = msg;
			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp));
		}
		else
		{
			//´?¸¥ ¿¡???ü?®
			msg.Protocol = MP_FRIEND_LOGIN_NOTIFY_TO_AGENT;
			g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
		}
	}
}

void RFriendGetLoginFriends(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	MSG_NAME msg;
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_LOGIN_FRIEND;
	msg.dwObjectID = pMessage->dwID;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
//		strcpy(msg.Name, (char*)pData[i].Data[eFr_LFFriendName]);
		SafeStrCpy( msg.Name, (char*)pData[i].Data[eFr_LFFriendName], MAX_NAME_LENGTH + 1 );
		
		MSG_NAME msgTemp = msg;
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp));
	}
}

void RFriendGetFriendList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;	

	MSG_FRIEND_LIST_DLG msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_LIST_ACK;

	msg.count = pMessage->dwResult <= MAX_FRIEND_NUM ? (BYTE)pMessage->dwResult : MAX_FRIEND_NUM;
	
	if(pMessage->dwResult > MAX_FRIEND_NUM)
	{
		ASSERT(pMessage->dwResult <= MAX_FRIEND_NUM);
		msg.count = MAX_FRIEND_NUM;
	}
	for(DWORD i=0; i< msg.count; ++i)
	{
		msg.FriendList[i].Id = atol((char*)pData[i].Data[eFr_FLFriendid]);
		msg.FriendList[i].IsLoggIn = atoi((char*)pData[i].Data[eFr_FLIsLogin]);
		SafeStrCpy( msg.FriendList[i].Name, (char*)pData[i].Data[eFr_FLFriendname], MAX_NAME_LENGTH + 1 );
	}
	msg.dwObjectID = pMessage->dwID;
	
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, msg.GetMsgLength());
}

void RNoteIsNewNote(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;
	
	if(atoi((char*)pData->Data[0]) == 1)
	{
		MSGBASE msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_NEW_NOTE;
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}

void RNoteSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{	
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2008.01.18
	// ���� ��ü �� �ý��� �޼����� �����Ƿ� ���� ���� �޼����� ��� �� ��~
	/*
	// ..�ý��� ������ ��� ID �� 0 �̰� ������ �̸��� <SYSTEM> �̴�
	if (pMessage->dwID == 0)
	{
		// �ý��� ���� ������ �����ϸ� �޴� �÷��̾�Ը� �˸���.
		if (atoi((char*)pData->Data[eFr_NErr]) == 0)
		{
			DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
			if(Toidx == 0) //·?±?¾?¿ô »ó??
				return;
			MSGBASE rmsg;
			rmsg.Category = MP_NOTE;
			rmsg.Protocol = MP_NOTE_RECEIVENOTE;
			rmsg.dwObjectID = Toidx;

			USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(Toidx);
			if(pToRecverInfo)
			{
				g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&rmsg, sizeof(rmsg));
			}
			else //´?¸¥ ¿¡???ü?®¿¡ ??´?. 
			{
				g_Network.Broadcast2AgentServerExceptSelf( (char*)&rmsg, sizeof(rmsg) );
			}
		}
		return;
	}
	*/
	// E ����ý��� �߰� added by hseos 2008.01.18

	if(!pRecverInfo)
		return;

	if(atoi((char*)pData->Data[eFr_NErr]) == 0) //success
	{
		MSG_NAME msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_ACK;
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));

	}
	else
	{
		MSG_NAME_WORD msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_NACK;
		msg.wData = (WORD)atoi((char*)pData->Data[eFr_NErr]); // 2:invalid user, 3: full space
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}

	//???ö ¹?´? »ç¶÷
	DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
	if(Toidx == 0) //·?±?¾?¿ô »ó??
		return;
	MSGBASE rmsg;
	rmsg.Category = MP_NOTE;
	rmsg.Protocol = MP_NOTE_RECEIVENOTE;
	rmsg.dwObjectID = Toidx;

	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(Toidx);
	if(pToRecverInfo)
	{
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&rmsg, sizeof(rmsg));
	}
	else //´?¸¥ ¿¡???ü?®¿¡ ??´?. 
	{
		g_Network.Broadcast2AgentServerExceptSelf( (char*)&rmsg, sizeof(rmsg) );
	}
}

void RNoteServerSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{	
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	if(atoi((char*)pData->Data[eFr_NErr]) == 0) //success
	{
/*		MSG_NAME msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_ACK;
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
*/
	}
	else
	{
		MSG_NAME_WORD msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_NACK;
		msg.wData = (WORD)atoi((char*)pData->Data[eFr_NErr]); // 2:invalid user, 3: full space
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;

	}

	//???ö ¹?´? »ç¶÷
	DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
	if(Toidx == 0) //·?±?¾?¿ô »ó??
		return;
	MSGBASE rmsg;
	rmsg.Category = MP_NOTE;
	rmsg.Protocol = MP_NOTE_RECEIVENOTE;
	rmsg.dwObjectID = Toidx;

	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(Toidx);
	if(pToRecverInfo)
	{
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&rmsg, sizeof(rmsg));
	}
	else //´?¸¥ ¿¡???ü?®¿¡ ??´?. 
	{
		g_Network.Broadcast2AgentServerExceptSelf( (char*)&rmsg, sizeof(rmsg) );
	}
}

void RNoteList(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage )
{	
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	SYSTEMTIME ti;
	GetLocalTime(&ti);

	static MSG_FRIEND_NOTE_LIST msg;
	memset(&msg,0,sizeof(MSG_FRIEND_NOTE_LIST));
	
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_NOTELIST_ACK;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		SafeStrCpy( msg.NoteList[i].SendDate, (char*)pData[i].Data[eFr_NSentDate], MAX_SENDDATE_LENGTH+1 );		
		SafeStrCpy( msg.NoteList[i].FromName, (char*)pData[i].Data[eFr_NSender], MAX_NAME_LENGTH + 1 );
		
		msg.NoteList[i].NoteID = atoi((char*)pData[i].Data[eFr_NNoteID]);
		msg.NoteList[i].bIsRead = (BYTE)atoi((char*)pData[i].Data[eFr_NbIsRead]);
		SafeStrCpy( msg.NoteList[i].SendTitle, (char*)pData[i].Data[eFr_NTitle], MAX_NOTE_TITLE + 1 );
		msg.NoteList[i].PackageItemIdx = atoi((char*)pData[i].Data[eFr_NPackageItemIdx]);
		msg.NoteList[i].PackageMoney = atoi((char*)pData[i].Data[eFr_NPackageMoney]);
		msg.dwObjectID = pMessage->dwID; 
	}
	msg.TotalPage = (BYTE)atoi((char*)pData[0].Data[eFr_NTotalpage]);
	msg.TotalMsgNum = (WORD)atoi((char*)pData[0].Data[eFr_NTotalmsg]);
	
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	
}

void RNoteRead(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	if(1 != pMessage->dwResult)
		return;


	const LARGEQUERYST&	record	= pData[0];

	

	DWORD dwPackageItemIdx = 0;
	dwPackageItemIdx = (DWORD)atoi((char*)record.Data[4]);
	if(dwPackageItemIdx)
	{
		MSG_FRIEND_READ_NOTE_WITH_PACKAGE msg;
		ZeroMemory(&msg.ItemInfo, sizeof(msg.ItemInfo));
		ZeroMemory(&msg.OptionInfo, sizeof(msg.OptionInfo));

		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_READNOTE_WITH_PACKAGE_ACK;
		SafeStrCpy( msg.FromName, (char*)record.Data[eFr_NRNSender], MAX_NAME_LENGTH + 1 );
		SafeStrCpy( msg.Note, (char*)record.Data[eFr_NRNNote], MAX_NOTE_LENGTH + 1 );
		msg.NoteID = atoi((char*)record.Data[eFr_NRNNoteID]);
		msg.ItemIdx = (WORD)atoi((char*)record.Data[eFr_NRNItemIdx]);
		msg.PackageItem = (DWORD)atoi((char*)record.Data[eFr_NRNPackageItem]);
		msg.PackageMoney = (DWORD)atoi((char*)record.Data[eFr_NRNPackageMoney]);

		ITEMBASE& item = msg.ItemInfo;

		char* pBuf;
		char* pString;
		char tokens[32] = {","};
		pBuf = (char*)record.Data[eFr_NRNPackageItemInfo];

		if(0 == pBuf[0])
			return;

		item.dwDBIdx		= atoi( strtok(pBuf, tokens) );
		item.wIconIdx		= atoi( strtok(NULL, ",") );
		item.Position		= POSTYPE( atoi( strtok(NULL, ",") ) );
		pString = strtok(NULL, ",");
		item.Durability		= atoi( strtok(NULL, ",") );
		pString = strtok(NULL, ",");
		item.nSealed		= ITEM_SEAL_TYPE(atoi( strtok(NULL, ",")));
		pString = strtok(NULL, ",");
		item.nRemainSecond	= atoi( strtok(NULL, ",") );

		if ( item.nSealed == eITEM_TYPE_GET_UNSEAL )
			return;

		
		// �ɼ� �ε�
		{
			ITEM_OPTION& option = msg.OptionInfo;

			{
				ITEM_OPTION::Reinforce& data = option.mReinforce;

				data.mStrength			= atoi( strtok(NULL, ",") );
				data.mDexterity			= atoi( strtok(NULL, ",") );
				data.mVitality			= atoi( strtok(NULL, ",") );
				data.mIntelligence		= atoi( strtok(NULL, ",") );
				data.mWisdom			= atoi( strtok(NULL, ",") );
				data.mLife				= atoi( strtok(NULL, ",") );
				data.mMana				= atoi( strtok(NULL, ",") );
				data.mLifeRecovery		= atoi( strtok(NULL, ",") );
				data.mManaRecovery		= atoi( strtok(NULL, ",") );
				data.mPhysicAttack		= atoi( strtok(NULL, ",") );
				data.mPhysicDefence		= atoi( strtok(NULL, ",") );
				data.mMagicAttack		= atoi( strtok(NULL, ",") );
				data.mMagicDefence		= atoi( strtok(NULL, ",") );
				data.mMoveSpeed			= atoi( strtok(NULL, ",") );
				data.mEvade				= atoi( strtok(NULL, ",") );
				data.mAccuracy			= atoi( strtok(NULL, ",") );
				data.mCriticalRate		= atoi( strtok(NULL, ",") );
				data.mCriticalDamage	= atoi( strtok(NULL, ",") );
				char* pName = strtok(NULL, ",");
				char name[MAX_NAME_LENGTH] = {0,};
				strncpy(name, &pName[1], strlen(pName)-2);
				SafeStrCpy( option.mReinforce.mMadeBy, name, sizeof( option.mReinforce.mMadeBy ) );				
			}

			{
				ITEM_OPTION::Mix& data = option.mMix;

				data.mStrength		= atoi( strtok(NULL, ",") );
				data.mIntelligence	= atoi( strtok(NULL, ",") );
				data.mDexterity		= atoi( strtok(NULL, ",") );
				data.mWisdom		= atoi( strtok(NULL, ",") );
				data.mVitality		= atoi( strtok(NULL, ",") );
				char* pName = strtok(NULL, ",");
				char name[MAX_NAME_LENGTH] = {0,};
				strncpy(name, &pName[1], strlen(pName)-2);
				SafeStrCpy( option.mMix.mMadeBy, name, sizeof( option.mMix.mMadeBy ) );	
			}

			{
				ITEM_OPTION::Enchant& data = option.mEnchant;

				data.mIndex	= BYTE( atoi( strtok(NULL, ",") ) );
				data.mLevel	= BYTE( atoi( strtok(NULL, ",") ) );
				char* pName = strtok(NULL, ",");
				char name[MAX_NAME_LENGTH] = {0,};
				strncpy(name, &pName[1], strlen(pName)-2);
				SafeStrCpy( option.mEnchant.mMadeBy, name, sizeof( option.mEnchant.mMadeBy ) );
			}

			{
				ITEM_OPTION::Drop& data = option.mDrop;

				data.mValue[ 0 ].mKey	= ITEM_OPTION::Drop::Key( atoi( strtok(NULL, ",") ) );
				data.mValue[ 0 ].mValue	= float( atof( strtok(NULL, ",") ) );

				data.mValue[ 1 ].mKey	= ITEM_OPTION::Drop::Key( atoi( strtok(NULL, ",") ) );
				data.mValue[ 1 ].mValue	= float( atof( strtok(NULL, ",") ) );

				data.mValue[ 2 ].mKey	= ITEM_OPTION::Drop::Key( atoi( strtok(NULL, ",") ) );
				data.mValue[ 2 ].mValue	= float( atof( strtok(NULL, ",") ) );

				data.mValue[ 3 ].mKey	= ITEM_OPTION::Drop::Key( atoi( strtok(NULL, ",") ) );
				data.mValue[ 3 ].mValue	= float( atof( strtok(NULL, ",") ) );

				data.mValue[ 4 ].mKey	= ITEM_OPTION::Drop::Key( atoi( strtok(NULL, ",") ) );
				data.mValue[ 4 ].mValue	= float( atof( strtok(NULL, ",") ) );
			}

			msg.OptionInfo.mItemDbIndex = item.dwDBIdx;

			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, msg.GetMsgLength());
		}
	}
	else
	{
		MSG_FRIEND_READ_NOTE msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_READNOTE_ACK;
		SafeStrCpy( msg.FromName, (char*)record.Data[eFr_NRNSender], MAX_NAME_LENGTH + 1 );
		SafeStrCpy( msg.Note, (char*)record.Data[eFr_NRNNote], MAX_NOTE_LENGTH + 1 );
		msg.NoteID = atoi((char*)record.Data[eFr_NRNNoteID]);
		msg.ItemIdx = (WORD)atoi((char*)record.Data[eFr_NRNItemIdx]);
		msg.dwPackageMoney = (DWORD)atoi((char*)record.Data[eFr_NRNPackageMoney]);

		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, msg.GetMsgLength());
	}
}

void RNoteDelete(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	int nNoteID = atoi((char*)pData->Data[0]);
	if(nNoteID < 0)
		return;

	MSG_FRIEND_DEL_NOTE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_DELNOTE_ACK;
	msg.bLast = atoi((char*)pData->Data[eFr_NdbLast]);
	msg.NoteID 	= atoi((char*)pData->Data[eFr_NdNoteID]);
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}

void RNoteSave(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	int nNoteID = atoi((char*)pData->Data[0]);
	if(nNoteID < 0)
		return;

	MSG_DWORD2 msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_DELNOTE_ACK;
	msg.dwData1	= atoi((char*)pData->Data[0]);
	msg.dwData2 = atoi((char*)pData->Data[1]);
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}

void RDeleteCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pInfo = g_pUserTable->FindUser( pMessage->dwID );
	if( !pInfo )			return;

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;

	if(atoi((char*)pData->Data[0]) != 0)
	{
		msg.Protocol = MP_USERCONN_CHARACTER_REMOVE_NACK;
		msg.dwData = atoi((char*)pData->Data[0]);
	}
	else
	{
		msg.Protocol = MP_USERCONN_CHARACTER_REMOVE_ACK;
		const DWORD CharacterIdx = atoi((char*)pData->Data[1]);
		
		for(int i=0; i<MAX_CHARACTER_NUM; ++i)
		{
			if( pInfo->SelectInfoArray[i].dwCharacterID == CharacterIdx )
				memset( &pInfo->SelectInfoArray[i], 0, sizeof(CHARSELECTINFO) );
		}
	}

	g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
}

//---for GM_Tool
void RGM_WhereIsCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{

	}
	else
	{
		//(DWORD)atoi((char*)pData->Data[2])	: ¸?¹ø?£
		//(DWORD)atoi((char*)pData->Data[1])	: ?£?º objectID

		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( !pSenderInfo ) return;

		int nError = atoi((char*)pData->Data[0]);
		int nLenEr	= strlen((char*)pData->Data[0]);	//050118 Error for ID including '1'
		if( nLenEr && ( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED ) )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_WHEREIS_NACK;
			msg.dwObjectID	= pMessage->dwID;
			msg.bData		= (BYTE)nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwTargetID	= (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMapNum		= (DWORD)atoi((char*)pData->Data[2]);

//			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( dwTargetID );

			if( dwMapNum > 0 )
			{
				MSG_DWORD msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_WHEREIS_SYN;
				msg.dwObjectID	= pMessage->dwID;
				msg.dwData		= dwTargetID;	//?£?º ¾???µð

				WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMapNum );
				SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );				

				if( pInfo )
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
			{
				MSG_WORD msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_WHEREIS_ACK;
				msg.dwObjectID	= pMessage->dwID;
				msg.wData		= (WORD)dwMapNum;
				g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
		}
	}
}

void RGM_BanCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{

	}
	else
	{
		//(char*)pData->Data[0]					: º¸³½»ç¶÷ ??¸§
		//(DWORD)atoi((char*)pData->Data[1])	: ?£?º objectID

		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( !pSenderInfo ) return;

		int nError = atoi((char*)pData->Data[0]);
		int nLenEr	= strlen((char*)pData->Data[0]);	//050118 Error for ID including '1'
		if( nLenEr && ( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED ) )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_BANCHARACTER_NACK;
			msg.dwObjectID	= pMessage->dwID;
			msg.bData		= (BYTE)nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwTargetID = (DWORD)atoi((char*)pData->Data[1]);
			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( dwTargetID );

			if( pTargetInfo )
			{
				DWORD dwConIdx = pTargetInfo->dwConnectionIndex;
				OnDisconnectUser( dwConIdx );
				DisconnectUser( dwConIdx );

				MSGBASE msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_BANCHARACTER_ACK;
				msg.dwObjectID	= pMessage->dwID;
				g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

				// 06.09.12 RaMa
				LogGMToolUse( pMessage->dwID, eGMLog_Disconnect_User, MP_CHEAT_BANCHARACTER_ACK, dwTargetID, 0 );
			}
			else
			{
				MSG_DWORD2 msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_BANCHARACTER_SYN;
				msg.dwData1		= dwTargetID;
				msg.dwData2		= pMessage->dwID;

				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msg, sizeof(msg) );
			}
		}
	}
}

void RGM_UpdateUserLevel(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// pMessage->dwID
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	MSGBASE msg;
	msg.Category	= MP_CHEAT;
	msg.dwObjectID	= pMessage->dwID;

	if(atoi((char*)pData->Data[0])==0)
	{
		// ±?·± ??¸§ ¾ø¾î~~
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_NACK;
	}
	else
	{
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_ACK;
		// ¾÷µ¥???® ¼º°ø~~

		DWORD useridx = atoi((char*)pData->Data[1]);
		DWORD state = atoi((char*)pData->Data[2]);

		// 06.09.12 RaMa
		LogGMToolUse( pMessage->dwID, eGMLog_Block, useridx, state, 0 );
	}
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
}


void RGM_Login(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	DWORD dwConnectionIndex = pMessage->dwID;

	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );

	if( count == 0 || pUserInfo == NULL ) // ??»ó ¿?·ù
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_GM_LOGIN_NACK;
		msg.bData		= 0;

		g_Network.Send2User( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		return;
	}
/*
enum eOperInfo
{
	eOI_ErroCode = 0, eOI_OperIdx, eOI_OperID, eOI_OperName, eOI_OperPwd, eOI_OperPower, eOI_Date, eOI_Time, 
	eOI_IPIdx, eOI_IPAdress, eOI_IPDate, eOI_IPTime, 
};
*/
	WORD check = (WORD)atoi((char*)pData[0].Data[0]);

	if( check != 0 ) // ?¢¼? º?°¡
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_GM_LOGIN_NACK;
		msg.bData		= 1;

		g_Network.Send2User( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		return;
	}

	int nPower = atoi((char*)pData[0].Data[5]);

	if( nPower < 0 || nPower >= eGM_POWER_MAX )
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_GM_LOGIN_NACK;
		msg.bData		= 2;

		g_Network.Send2User( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		return;		
	}

	DWORD dwIdx = (DWORD)atoi((char*)pData[0].Data[1]);
	char szName[MAX_NAME_LENGTH+1];

	SafeStrCpy( szName, (char*)pData[0].Data[2], MAX_NAME_LENGTH+1 );

	GMINFO->AddGMList( dwConnectionIndex, nPower, dwIdx, szName );

	MSG_DWORD Msg;
	Msg.Category	= MP_CHEAT;
	Msg.Protocol	= MP_CHEAT_GM_LOGIN_ACK;
	Msg.dwData		= nPower;

	g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
}


void RGM_GetGMPowerList(LPQUERY pData, LPDBMESSAGE pMessage)
{
/*
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count )
	{
		DWORD dwFlag = atoi((char*)pData[0].Data[0]);
		if( dwFlag == 0 )
			GMINFO->Release();

		GM_POWER pw;
		DWORD startIdx = 0;
		for( DWORD i = 0; i < count; ++i )
		{			
			startIdx = atoi((char*)pData[i].Data[1]);
			SafeStrCpy( pw.GM_ID, (char*)pData[i].Data[2], MAX_NAME_LENGTH+1 );
			pw.dwUserID = 0;
			pw.nPower = atoi((char*)pData[i].Data[5]);

			GMINFO->AddGMList( &pw );
		}
	
		if( count >= 100 )
			GM_GetGMPowerList( startIdx, count );
	}
*/
}


/* --; ??¿ä¾ø°?³?. ??¸ø?¥µû.
void RGM_MoveToCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{

	}
	else
	{
		
		//(DWORD)atoi((char*)pData->Data[1])	: ?£?º objectID
		//(char*)pData->Data[0]					: º¸³½»ç¶÷ ??¸§

		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( !pSenderInfo ) return;

		int nError = atoi((char*)pData->Data[0]);
		if( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_MOVETOCHAR_NACK;
			msg.dwObjectID	= pMessage->dwID;
			msg.bData		= nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwTargetID = (DWORD)atoi((char*)pData->Data[1]);
			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( (DWORD)atoi((char*)pData->Data[1]) );

			//??¼­¹ö¿¡ ??³ª?
			if( pTargetInfo )
			{
				//---³ª??¿¡ gm¸í·??º ´?¸¥µ¥·?....
				//obejctid¸¦ ¸?¼­¹ö·? º¸³»??!

			}
			else
			{

			}
		}


	}
}
*/

void CheckGuildMasterLogin( DWORD dwConnectionIdx, DWORD dwPlayerIdx, char* pSearchName, DWORD dwMoney, BYTE Protocol )
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(pSearchName, "'"))	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery( eCheckGuildMasterLogin, dwConnectionIdx, "EXEC MP_GUILDFIELDWAR_CHECKMASTERLOGIN %d, \'%s\', %d, %d",
		dwPlayerIdx, pSearchName, dwMoney, Protocol );
}

void RCheckGuildMasterLogin( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIndex = pMessage->dwID;
	int nFlag = atoi( (char*)pData->Data[0] );

	switch( nFlag )
	{
	case 0:		// login
		{
			DWORD dwSenderIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMasterIdx = (DWORD)atoi((char*)pData->Data[2]);
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[3]);
			DWORD dwMoney = (DWORD)atoi((char*)pData->Data[4]);
			BYTE Protocol = (BYTE)atoi((char*)pData->Data[5]);

			MSG_DWORD3 Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = Protocol;
			Msg.dwData1 = dwSenderIdx;
			Msg.dwData2 = dwMasterIdx;
			Msg.dwData3 = dwMoney;

			USERINFO* userinfo = (USERINFO*)g_pUserTable->FindUser( dwConnectionIndex );
			if( userinfo == NULL )	return;
			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( !wPort )	return;
			SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
			if( !pSInfo )	return;

			if( userinfo->dwMapServerConnectionIndex == pSInfo->dwConnectionIndex )
			{
				g_Network.Send2Server( userinfo->dwMapServerConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
			else
			{
				g_Network.Send2Server( userinfo->dwMapServerConnectionIndex, (char*)&Msg, sizeof(Msg) );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
			/*			
			g_Network.Broadcast2MapServer( (char*)&Msg, sizeof(Msg) );
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
			MSG_BYTE Wait;
			Wait.Category = MP_GUILD_WAR;
			Wait.Protocol = MP_GUILD_WAR_WAIT;
			Wait.bData = Protocol;						
			g_Network.Send2User( dwConnectionIndex, (char*)&Wait, sizeof(Wait) );
*/			
		}
		break;
	case 1:		// sender is not master
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = (BYTE)nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );			
		}
		break;
	case 2:		// is not guild
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = (BYTE)nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	case 3:		// same guild	
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = (BYTE)nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	case 4:		// not login
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = (BYTE)nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	}
}

void CheckGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwSenderIdx, DWORD dwEnemyGuildIdx, DWORD dwMoney )
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery( eCheckGuildFieldWarMoney, dwConnectionIndex, "EXEC dbo.MP_GUILDFIELDWAR_CHECKMONEY %d, %d, %d",
		dwSenderIdx, dwEnemyGuildIdx, dwMoney );
}

void RCheckGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIndex = pMessage->dwID;
	int nFlag = atoi( (char*)pData->Data[0] );

	switch( nFlag )
	{
	case 0:		// login
		{
			DWORD dwSenderIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwEnemyGuildIdx = (DWORD)atoi((char*)pData->Data[2]);
			DWORD dwMoney = (DWORD)atoi((char*)pData->Data[3]);

			MSG_DWORD2 Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_DECLARE_ACCEPT;
			Msg.dwObjectID = dwSenderIdx;
			Msg.dwData1 = dwEnemyGuildIdx;
			Msg.dwData2 = dwMoney;

			USERINFO* userinfo = (USERINFO*)g_pUserTable->FindUser( dwConnectionIndex );
			if( userinfo == NULL )	return;
			g_Network.Send2Server( userinfo->dwMapServerConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	case 1:		// receiver not login
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = 4;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );			
		}
		break;
	case 2:		// receiver money not enough
		{
			// sender
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = 5;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );

			// receiver	
			DWORD dwReceiverIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[2]);

			MSG_BYTE Msg1;
			Msg1.Category = MP_GUILD_WAR;
			Msg1.Protocol = MP_GUILD_WAR_NACK;
			Msg1.dwObjectID = dwReceiverIdx;
			Msg1.bData = 6;			

			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( wPort )
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg1, sizeof(Msg1) );
			}
		}
		break;
	case 3:		// sender money not enough	
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = 6;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );

			// receiver	
			DWORD dwReceiverIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[2]);

			MSG_BYTE Msg1;
			Msg1.Category = MP_GUILD_WAR;
			Msg1.Protocol = MP_GUILD_WAR_NACK;
			Msg1.dwObjectID = dwReceiverIdx;
			Msg1.bData = 5;			

			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( wPort )
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg1, sizeof(Msg1) );
			}
		}
		break;
	}
}

void AddGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwPlayerIdx, DWORD dwMoney )
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery( eAddGuildFieldWarMoney, dwConnectionIndex, "EXEC dbo.MP_GUILDFIELDWAR_ADDMONEY %d, %d",
		dwPlayerIdx, dwMoney );
}

void RAddGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIndex = pMessage->dwID;
	int nFlag = atoi( (char*)pData->Data[0] );

	switch( nFlag )
	{
	case 0:		// login
		{
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwPlayerIdx = (DWORD)atoi((char*)pData->Data[2]);
			DWORD dwMoney = (DWORD)atoi((char*)pData->Data[3]);

			MSG_DWORD2 Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_ADDMONEY_TOMAP;
			Msg.dwData1 = dwPlayerIdx;
			Msg.dwData2 = dwMoney;

			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( wPort )
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
		}
		break;
	case 1:		// not login
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = 4;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );			
		}
		break;
	}
}

void EventItemUse051108( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum )
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(sCharName, "'"))	return;

	char txt[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf( txt, "EXEC UP_EVENT051108_INSERT %d, \'%s\', %d", dwUserIdx, sCharName, dwServerNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse051108, 0, txt);
}

void EventItemUse2( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum )
{
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(sCharName, "'"))	return;

	char txt[128];
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf( txt, "EXEC dbo.UP_EVENTITEMUSE2 %d, \'%s\', %d", dwUserIdx, sCharName, dwServerNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse2, 0, txt);
}

void GM_UpdateUserState(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserState)
{
	// 090325 ONS ���͸�����üũ
	if(strlen(Charactername) > MAX_NAME_LENGTH)	return;

	char szBufName[MAX_NAME_LENGTH+1];
	SafeStrCpy(szBufName, Charactername, MAX_NAME_LENGTH+1);
	if(IsCharInString(szBufName, "'"))	return;

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.FreeQuery( eGM_UpdateUserState, dwID, "EXEC dbo.MP_GMTOOL_UPDATEUSERSTATE %d, \'%s\', %d", dwServerGroup, Charactername, UserState );
}

void RGM_UpdateUserState(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// pMessage->dwID
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	MSGBASE msg;
	msg.Category	= MP_CHEAT;
	msg.dwObjectID	= pMessage->dwID;

	if(atoi((char*)pData->Data[0])==0)
	{
		// ±?·± ??¸§ ¾ø¾î~~
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_NACK;
	}
	else
	{
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_ACK;
		// ¾÷µ¥???® ¼º°ø~~
	}
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

}

void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 )
{
	char txt[128] = { 0, };
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf( txt, "EXEC dbo.UP_GMTOOLUSELOG %d, %d, %d, %d, %d",
		CharacterIdx,
		GMLogtype,
		Logkind,
		Param1,
		Param2
	);
	g_DB.LogQuery( eQueryType_FreeQuery, eLogGMToolUse, 0, txt );
}

// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
#define STORED_FAMILY_LOAD_INFO				"dbo.MP_FAMILY_LOADINFO"
#define STORED_FAMILY_LOAD_INFO_EMBLEM		"dbo.MP_FAMILY_LOADINFO_EMBLEM"
#define STORED_FAMILY_SAVE_INFO				"dbo.MP_FAMILY_SAVEINFO"
#define STORED_FAMILY_SAVE_INFO_HONORPOINT	"dbo.MP_FAMILY_SAVEINFO_HONORPOINT"
#define STORED_FAMILY_SAVE_INFO_EMBLEM		"dbo.MP_FAMILY_SAVEINFO_EMBLEM"
#define STORED_FAMILY_MEMBER_LOAD_INFO		"dbo.MP_FAMILY_MEMBER_LOADINFO"
#define STORED_FAMILY_MEMBER_LOAD_EXINFO_01	"dbo.MP_FAMILY_MEMBER_LOADEXINFO01"
#define STORED_FAMILY_MEMBER_SAVE_INFO		"dbo.MP_FAMILY_MEMBER_SAVEINFO"
#define STORED_FAMILY_CHECKNAME				"dbo.MP_FAMILY_CHECKNAME"
#define STORED_FAMILY_LEAVE_LOAD_INFO		"dbo.MP_FAMILY_LEAVE_LOADINFO"
#define STORED_FAMILY_LEAVE_SAVE_INFO		"dbo.MP_FAMILY_LEAVE_SAVEINFO"
// 091111 ONS �йи� ���� ���� ���ν��� �߰�.
#define STORED_FAMILY_DELETE_EMBLEM			"dbo.MP_FAMILY_DELETE_EMBLEM"

char txt[128];
void Family_SaveInfo(DWORD nMasterID, char* pszFamilyName, int nHonorPoint, BOOL bNicknameON, int nSaveType)
{
	if (nSaveType == 0)
	{
		// �йи� �̸��� '' �� �� �����ִϱ� 1231�йи� �� ���� ������ ���� �ѱ�����(������ Ȯ�� �� �غ���)�� ���ڿ��� �ν����� �ʴ´�.
		// SQL�������� ���ڰ� ���� ������ ���ڿ��� �ν����� �ʴ� ��...
		sprintf(txt, "EXEC  %s '%s', %d, %d, %d", STORED_FAMILY_SAVE_INFO, pszFamilyName, nMasterID, nHonorPoint, bNicknameON);
		g_DB.Query(eQueryType_FreeQuery, eFamily_SaveInfo, nMasterID, txt);
	}
	else if (nSaveType == 1)
	{
		sprintf(txt, "EXEC  %s %d, %d", STORED_FAMILY_SAVE_INFO_HONORPOINT,  nMasterID, nHonorPoint);
		g_DB.Query(eQueryType_FreeQuery, eFamily_SaveInfo, nMasterID, txt);
	}
}

void RFamily_SaveInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// �йи� ���� �� �йи��� ID�� �ޱ� ����.. 
	// ..�йи��� ó�� ���� �ÿ��� DB�� ������ �� ������ �йи��� �����Ѵ�. ���� �� ����ÿ��� Update �� �ϱ� ������ ����� ���� �ʴ´�.
	// ..��� ID�� �޾Ƽ� �����ϸ� �Ǵµ�, �޴� �迡 ��� ������ �� �޾Ƽ� ���� ������ ����!!!
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		// �йи� ����
		CSHFamily::stINFO		stInfo;
		CSHFamily::stINFO_EX	stInfoEx;

		ZeroMemory(&stInfo, sizeof(stInfo));
		ZeroMemory(&stInfoEx, sizeof(stInfoEx));
		stInfo.nID				= atoi((char*)pData[0].Data[0]);
		SafeStrCpy(stInfo.szName, (char*)pData[0].Data[1], MAX_NAME_LENGTH+1);
		stInfo.nMasterID		= atoi((char*)pData[0].Data[2]);
		stInfoEx.nHonorPoint	= atoi((char*)pData[0].Data[3]);
		stInfoEx.nNicknameON	= atoi((char*)pData[0].Data[4]);

		// ���(�ڽ�) ���� (���� ����)
		CSHFamilyMember::stINFO		stMemberInfo;

		ZeroMemory(&stMemberInfo, sizeof(stMemberInfo));
		stMemberInfo.nID		= pUserInfo->dwCharacterID;
		stMemberInfo.eConState	= CSHFamilyMember::MEMBER_CONSTATE_LOGIN;

		CSHFamilyMember csFamilyMember;
		csFamilyMember.Set(&stMemberInfo);

		// ���� �йи� ����
		CSHFamily csFamily;
		csFamily.Set(&stInfo, &stInfoEx);
		g_csFamilyManager.ASRV_CreateFamily(pUserInfo, stInfo.szName, CSHFamilyManager::FCS_COMPLETE, &csFamily);

		// ������ ���� �б�
		Family_Member_LoadInfo(pUserInfo->dwCharacterID, stInfo.nID);
		// 081205 LUJ, �α�
		InsertLogFamily(
			eLog_FamilyCreate,
			stInfo.nID,
			stInfo.nMasterID,
			stInfo.szName );
	}
}

void Family_SaveEmblemInfo(DWORD nPlayerID, DWORD nFamilyID, char* pcImg)
{
	char buf[CSHFamilyManager::EMBLEM_BUFSIZE*2+256];
	sprintf(buf, "EXEC  %s %d, 0x", STORED_FAMILY_SAVE_INFO_EMBLEM,  nFamilyID);

	int curpos = strlen(buf);
	for(int n=0;n<CSHFamilyManager::EMBLEM_BUFSIZE;++n)
	{
		sprintf(&buf[curpos],"%02x",(BYTE)pcImg[n]);
		curpos += 2;
	}

	g_DB.FreeLargeQuery(NULL, 0, buf);
}

// 091111 ONS �йи� ���� ����
void Family_DeleteEmblem(DWORD nPlayerID, DWORD nFamilyID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_DELETE_EMBLEM, nFamilyID);
	g_DB.Query(eQueryType_FreeQuery, eFamily_Delete_Emblem, nPlayerID, txt);
}


void Family_LoadInfo(DWORD nPlayerID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_LOAD_INFO, nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eFamily_LoadInfo, nPlayerID, txt);
}

void RFamily_LoadInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		if(CSHFamily* family = g_csFamilyManager.GetFamily(atoi((char*)pData[0].Data[0])))
		{
			int nLoginCnt = 0;
			pUserInfo->mFamilyIndex = family->Get()->nID;

			for(UINT i = 0; i < family->Get()->nMemberNum; ++i)
			{
				if (pUserInfo->dwCharacterID == family->GetMember(i)->Get()->nID)
				{
					family->GetMember(i)->Get()->eConState = CSHFamilyMember::MEMBER_CONSTATE_LOGIN;
				}

				if(family->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
				{
					nLoginCnt++;
				}
			}

			// ���� ������ �ִ� ����� 2���� �Ǹ� �� ����Ʈ �ð��� �����Ѵ�.
			if (nLoginCnt == CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
			{
				family->GetEx()->nHonorPointTimeTick = gCurTime;
			}

			g_csFamilyManager.ASRV_SendMemberConToOtherAgent(
				family,
				pUserInfo->dwCharacterID,
				CSHFamilyMember::MEMBER_CONSTATE_LOGIN);
			g_csFamilyManager.ASRV_SendInfoToClient(
				family);
			Family_LoadEmblemInfo(
				pUserInfo->dwCharacterID,
				family->Get()->nID);
			g_csFarmManager.ASRV_RequestFarmUIInfoToMap(
				pUserInfo->dwCharacterID,
				family);
		}
		// �йи� ���̺� �߰��Ѵ�.
		else
		{
			// ���� ����
			CSHFamily::stINFO		stInfo;
			CSHFamily::stINFO_EX	stInfoEx;

			ZeroMemory(&stInfo,		sizeof(stInfo));
			ZeroMemory(&stInfoEx,	sizeof(stInfoEx));
			stInfo.nID				= atoi((char*)pData[0].Data[0]);
			SafeStrCpy(stInfo.szName, (char*)pData[0].Data[1], MAX_NAME_LENGTH+1);
			stInfo.nMasterID		= atoi((char*)pData[0].Data[2]);
			stInfoEx.nHonorPoint	= atoi((char*)pData[0].Data[3]);
			stInfoEx.nNicknameON	= atoi((char*)pData[0].Data[4]);

			CSHFamily csFamily;
			csFamily.Set(&stInfo, &stInfoEx);

			family = g_csFamilyManager.AddFamilyToTbl(
				&csFamily);

			if(0 == family)
			{
				return;
			}

			pUserInfo->mFamilyIndex = family->Get()->nID;
			Family_Member_LoadInfo(
				pMessage->dwID,
				stInfo.nID);
			g_csFarmManager.ASRV_RequestFarmUIInfoToMap(
				pUserInfo->dwCharacterID,
				family);
		}
	}
}

void Family_LoadEmblemInfo(DWORD nPlayerID, DWORD nFamilyID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_LOAD_INFO_EMBLEM, nFamilyID);
	g_DB.FreeLargeQuery(RFamily_LoadEmblemInfo, nPlayerID, txt);
}

int convertCharToInt(char c)
{
	if('0' <= c && c <= '9')
		return c - '0';
	if('A' <= c && c <= 'F')
		return c - 'A' +10;
	if('a' <= c && c <= 'f')
		return c - 'a' +10;
	return 0;
}

HexToByte(char* pStr)
{
	int n1 = convertCharToInt(pStr[0]);
	int n2 = convertCharToInt(pStr[1]);

	return n1 * 16 + n2;
}

void RFamily_LoadEmblemInfo(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		char Img[CSHFamilyManager::EMBLEM_BUFSIZE];
		char *pStr = (char*)pData->Data[1];
		char tempBuf[3] = {0,};
		int curpos = 0;
		for(int n=0;n<CSHFamilyManager::EMBLEM_BUFSIZE;++n)
		{
			strncpy(tempBuf,&pStr[curpos],2); // "FF"
			Img[n] = (char)HexToByte(tempBuf);
			curpos += 2;
		}

		g_csFamilyManager.ASRV_RegistEmblem(pUserInfo, atoi((char*)pData->Data[0]), Img, 1);
	}
}

void Family_Member_SaveInfo(DWORD nFamilyID, DWORD nMemberID, char* pszMemberNickname)
{
	if (pszMemberNickname)
	{
		sprintf(txt, "EXEC  %s %d, %d, '%s'", STORED_FAMILY_MEMBER_SAVE_INFO, nFamilyID, nMemberID, pszMemberNickname);
	}
	else
	{
		// �Ʒ��� NULL �� �׳� �ڸ����� �����ֱ� ���� �ؽ�Ʈ��. � �� �־���� ���ν�������
		// insert ���� �ݿ����� �ʵ��� �Ǿ� ����.
		sprintf(txt, "EXEC  %s %d, %d, NULL", STORED_FAMILY_MEMBER_SAVE_INFO, nFamilyID, nMemberID);
	}
	g_DB.Query(eQueryType_FreeQuery, eFamily_Member_SaveInfo, 0, txt);
}

void Family_Member_LoadInfo(DWORD nPlayerID, DWORD nFamilyID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_MEMBER_LOAD_INFO, nFamilyID);
	g_DB.Query(eQueryType_FreeQuery, eFamily_Member_LoadInfo, nPlayerID, txt);
}

void RFamily_Member_LoadInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		CSHFamily* const family = g_csFamilyManager.GetFamily(
			pUserInfo->mFamilyIndex);

		if(0 == family)
		{
			return;
		}

		// ..��� ������ ����
		USERINFO* pMember;
		CSHFamilyMember				csMember;
		CSHFamilyMember::stINFO		stInfo;

		for(DWORD i=0; i<pMessage->dwResult; i++)
		{
			if (i >= family->GetMemberNumMax())
			{
				break;
			}

			ZeroMemory(&stInfo, sizeof(stInfo));

			stInfo.nID = atoi((char*)pData[i].Data[0]);
			sprintf(stInfo.szName, "%d", stInfo.nID);
			SafeStrCpy(stInfo.szNickname, (char*)pData[i].Data[1], MAX_NAME_LENGTH+1);
			pMember = g_pUserTableForObjectID->FindUser(stInfo.nID);
			if (pMember)
			{
				stInfo.eConState = CSHFamilyMember::MEMBER_CONSTATE_LOGIN;
			}

			// �ɹ� ����
			((CSHGroupMember*)&csMember)->Set(&stInfo);
			family->SetMember(
				&csMember,
				i);

			++(family->Get()->nMemberNum);
		}
		
		g_csFamilyManager.SetFamily(
			family);

		for(UINT i = 0; i < family->Get()->nMemberNum; ++i)
		{
			Family_Member_LoadExInfo01(
				pUserInfo->dwCharacterID,
				family->GetMember(i)->Get()->nID);
		}
		
		g_csFamilyManager.ASRV_SendMemberConToOtherAgent(
			family,
			pUserInfo->dwCharacterID,
			CSHFamilyMember::MEMBER_CONSTATE_LOGIN);
		Family_LoadEmblemInfo(
			pUserInfo->dwCharacterID,
			family->Get()->nID);
	}
}

void Family_Member_LoadExInfo01(DWORD nPlayerID, DWORD nMemberID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_MEMBER_LOAD_EXINFO_01, nMemberID);
	g_DB.Query(eQueryType_FreeQuery, eFamily_Member_LoadExInfo01, nPlayerID, txt);
}


void RFamily_Member_LoadExInfo01(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		CSHFamily* const family = g_csFamilyManager.GetFamily(
			pUserInfo->mFamilyIndex);

		if(0 == family) return;

		const DWORD nMemberID = atoi((char*)pData[0].Data[0]);

		for(UINT i = 0; i < family->Get()->nMemberNum; ++i)
		{
			if (nMemberID == family->GetMember(i)->Get()->nID)
			{
				CSHFamilyMember	csMember;
				CSHFamilyMember::stINFO	stInfo = *family->GetMember(i)->Get();
				CSHFamilyMember::stINFO_EX	stInfoEx = *family->GetMember(i)->GetEx();

				stInfo.nID			= atoi((char*)pData[0].Data[0]);
				SafeStrCpy(stInfo.szName, (char*)pData[0].Data[1], MAX_NAME_LENGTH+1);
				stInfo.nRace		= atoi((char*)pData[0].Data[2]);
				stInfo.nSex			= atoi((char*)pData[0].Data[3]);
				stInfo.nJobGrade	= atoi((char*)pData[0].Data[4]);
				int nJob[6] = {0};
				nJob[0]				= atoi((char*)pData[0].Data[5]);
				nJob[1]				= atoi((char*)pData[0].Data[6]);
				nJob[2]				= atoi((char*)pData[0].Data[7]);
				nJob[3]				= atoi((char*)pData[0].Data[8]);
				nJob[4] 			= atoi((char*)pData[0].Data[9]);
				nJob[5] 			= atoi((char*)pData[0].Data[10]);
				stInfo.nJobFirst	= nJob[0];
				stInfo.nJobCur		= nJob[stInfo.nJobGrade-1];
				stInfo.nLV			= atoi((char*)pData[0].Data[11]);
				SafeStrCpy(stInfoEx.szGuild, (char*)pData[0].Data[12], MAX_NAME_LENGTH+1);
				
				csMember.Set(
					&stInfo,
					&stInfoEx);
				family->SetMember(
					&csMember,
					i);

				if (stInfo.nID == family->Get()->nMasterID)
				{
					SafeStrCpy(
						family->Get()->szMasterName,
						stInfo.szName,
						_countof(family->Get()->szMasterName));
				}

				break;
			}
		}

		for(UINT i = 0; i < family->Get()->nMemberNum; ++i)
		{
			if(family->GetMember(i)->Get()->szName[0] == '@')
			{
				Family_Leave_SaveInfo(
					family->GetMember(i)->Get()->nID,
					CSHFamilyManager::FLK_LEAVE, TRUE);
				family->DelMember(
					family->GetMember(i)->Get()->nID);
				i--;
			}
		}

		g_csFamilyManager.SetFamily(
			family);
		g_csFamilyManager.ASRV_SendInfoToClient(
			family);
	}
}

void Family_CheckName(DWORD nPlayerID, char* pszName)
{
	// �йи� �̸��� '' �� �� �����ִϱ� 1231�йи� �� ���� ������ ���� �ѱ�����(������ Ȯ�� �� �غ���)�� ���ڿ��� �ν����� �ʴ´�.
	// SQL�������� ���ڰ� ���� ������ ���ڿ��� �ν����� �ʴ� ��...
	sprintf(txt, "EXEC  %s '%s'", STORED_FAMILY_CHECKNAME, pszName);
	g_DB.Query(eQueryType_FreeQuery, eFamily_CheckName, nPlayerID, txt);
}

void RFamily_CheckName(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( stricmp((char*)pData[0].Data[0], "") == 0 )
	{
		// �̹� DB�� �����ϴ� �̸��̴�..
		g_csFamilyManager.ASRV_CreateFamily(pUserInfo, NULL, CSHFamilyManager::FCS_NAME_ERR);
	}
	else
	{
		// DB�� ���� �̸�. ����� �� �ִ�. �ٽ� �йи� ���� �õ�
		g_csFamilyManager.ASRV_CreateFamily(pUserInfo, (char*)pData[0].Data[0], CSHFamilyManager::FCS_NAME_CHECK2);
	}
}

void Family_Leave_SaveInfo(DWORD nPlayerID, int nLeaveKind, BOOL bInit, BOOL bBreakUp)
{
	SYSTEMTIME t;
	GetLocalTime(&t);

	DWORD nLeaveDate = (t.wYear-2000)*100000000 + t.wMonth*1000000 + t.wDay*10000 + t.wHour*100 + t.wMinute;
	if (bInit) nLeaveDate = 0;

	sprintf(txt, "EXEC  %s %d, %u, %d, %d", STORED_FAMILY_LEAVE_SAVE_INFO, nPlayerID, nLeaveDate, nLeaveKind, bBreakUp);
	g_DB.Query(eQueryType_FreeQuery, eFamily_Leave_SaveInfo, 0, txt);
}

void Family_Leave_LoadInfo(DWORD nPlayerID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_LEAVE_LOAD_INFO, nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eFamily_Leave_LoadInfo, nPlayerID, txt);
}

void RFamily_Leave_LoadInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		pUserInfo->nFamilyLeaveDate = atoi((char*)pData[0].Data[0]);
		pUserInfo->nFamilyLeaveKind = atoi((char*)pData[0].Data[1]);
	}
}

void RFamily_ChangeMaster(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	if(0 == pMessage->dwResult)
	{
		return;
	}

	USERINFO* const pUserInfo = g_pUserTableForObjectID->FindUser(
		pMessage->dwID);

	if(0 == pUserInfo)
	{
		return;
	}

	CSHFamily* const family = g_csFamilyManager.GetFamily(
		pUserInfo->mFamilyIndex);

	if(0 == family)
	{
		return;
	}

	const MIDDLEQUERYST& record = pData[0];
	const DWORD dwNewMasterID = atoi(LPCTSTR(record.Data[0]));
	LPCTSTR masterName = LPCTSTR(record.Data[1]);
	const int nFarmZone = atoi(LPCTSTR(pData[0].Data[2]));
	const int nFarmID = atoi(LPCTSTR(pData[0].Data[3]));

	MSG_DWORD3_NAME stPacket;
	ZeroMemory(
		&stPacket,
		sizeof(stPacket));
	stPacket.Category	= MP_FAMILY;
	stPacket.Protocol	= MP_FAMILY_TRANSFER_TO_OTHER_AGENT;
	stPacket.dwObjectID	= family->Get()->nID;
	stPacket.dwData1 = dwNewMasterID;
	stPacket.dwData2 = pUserInfo->nFamilyLeaveDate;
	stPacket.dwData3 = pUserInfo->nFamilyLeaveKind;
	SafeStrCpy(
		stPacket.Name,
		masterName,
		_countof(stPacket.Name));
	g_csFamilyManager.ASRV_TransferFromOtherAgent(
		(char*)&stPacket);
	g_Network.Broadcast2AgentServerExceptSelf(
		(char*)&stPacket,
		sizeof(stPacket));

	// ���� ���� ���� ���� �� �����Ƿ� ��ε�ĳ�����Ѵ�
	{
		MSG_DWORD3 message;
		ZeroMemory(
			&message,
			sizeof(message));
		message.Category = MP_FARM;
		message.Protocol = MP_FARM_SETFARM_CHANGE_OWNER;
		message.dwObjectID = pMessage->dwID;
		message.dwData1 = nFarmZone;
		message.dwData2 = nFarmID;
		message.dwData3 = dwNewMasterID;
		g_Network.Broadcast2MapServer(
			(char*)&message,
			sizeof(message));
	}

	InsertLogFamily(
		eLog_FamilyChangeMaster,
		family->Get()->nID,
		pUserInfo->dwCharacterID );
}

// E �йи� �߰� added by hseos 2007.07.09	2007.07.10	2007.07.14	2007.10.11

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2008.01.22
void Farm_SendNote_DelFarm(DWORD nFarmOwnerID)
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC  dbo.MP_FARM_SENDNOTE_DELFARM %d", nFarmOwnerID);
	g_DB.Query(eQueryType_FreeQuery, eFarm_SendNote_DelFarm, nFarmOwnerID, txt);
}

void RFarm_SendNote_DelFarm(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// �йи� ��� �̸��� �� ������ ������.
	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		NoteSendtoPlayer(0, "<SYSTEM>", (char*)pData[0].Data[0], "<SYSTEM NOTICE>", "61");
	}
}
// E ����ý��� �߰� added by hseos 2008.01.22

#ifdef _NPROTECT_
void NProtectBlock(DWORD UserIdx, DWORD CharIdx, char* IP, DWORD BlockType)
{
	// 090325 ONS ���͸�����üũ
	char szBufIp[MAX_IPADDRESS_SIZE];
	SafeStrCpy(szBufIp, IP, MAX_IPADDRESS_SIZE);
	if(IsCharInString(szBufIp, "'"))	return;

    // 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC dbo.UP_NPROTECTBLOCKLOG %d, %d, \'%s\', %d, %d", UserIdx, CharIdx, szBufIp, BlockType, 6 );
	g_DB.LoginQuery( eQueryType_FreeQuery, eNProtectBlock, 0, txt);
}
#endif


void InsertLogFamilyPoint( CSHFamily* family, eFamilyLog type )
{
	if( ! family )
	{
		ASSERT( 0 );
		return;
	}

	const CSHFamily::stINFO_EX*	extendedData	= family->GetEx();
	const CSHFamily::stINFO*	data			= family->Get();

	if( ! data ||
		! extendedData )
	{
		ASSERT( 0 );
		return;
	}

	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	g_DB.LogMiddleQuery( 0, 0, "EXEC dbo.TP_FAMILY_POINT_LOG_INSERT %d, %d, %d, %d, %d, \'""\'",
		data->nID,
		extendedData->nHonorPoint,
		0,
		0,
		type );
}


//---KES PUNISH
void PunishListAdd( DWORD dwUserIdx, int nPunishKind, DWORD dwPunishTime )
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC dbo.MP_PUNISHLIST_ADD %u, %d, %u", dwUserIdx, nPunishKind, dwPunishTime );
	g_DB.LoginQuery( eQueryType_FreeQuery, ePunishList_Add, dwUserIdx, txt);
}

void PunishListLoad( DWORD dwUserIdx )
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC dbo.MP_PUNISHLIST_LOAD %u", dwUserIdx );
	g_DB.LoginQuery( eQueryType_FreeQuery, ePunishList_Load, dwUserIdx, txt);
}

void RPunishListLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pMessage->dwID );
	if( pUserInfo == NULL ) return;

	int nCount = pMessage->dwResult;
	if( nCount == 0 ) return;
	
	for( int i = 0 ; i < nCount ; ++i )
	{
		int nPunishTime = atoi( (char*)pData[i].Data[1] );
		if( nPunishTime > 0 ) 
			PUNISHMGR->AddPunishUnit( pUserInfo->dwUserID, atoi((char*)pData[i].Data[0]), nPunishTime );
	}
}

void PunishCountAdd( DWORD dwUserIdx, int nPunishKind, int nPeriodDay )
{
	// 081012 LUJ, �Ϻ� ������ ���ν��� ȣ�� �� ��ҹ��ڱ��� ��ġ�ؾ� �Ѵ�. ��, dbo�� �ҹ��ڷ� �ؾ���. �̿� ��� ���ν��� �̸��� ������
	sprintf(txt, "EXEC dbo.MP_PUNISHCOUNT_ADD %u, %d, %d", dwUserIdx, nPunishKind, nPeriodDay );
	g_DB.LoginQuery( eQueryType_FreeQuery, ePunishCount_Add, dwUserIdx, txt);
}

void RPunishCountAdd( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == NULL ) return;

	int nPunishKind		= atoi((char*)pData[0].Data[0]);
	int nCount			= atoi((char*)pData[0].Data[1]);

	if( nCount == 0 ) return;

	switch( nPunishKind )
	{
	case ePunishCount_AutoUse:
		{
			PunishListAdd( pMessage->dwID, ePunish_Login, (DWORD)(POW( 2, nCount-1 ) * 1 * 60 * 60) );	//1�ð����� �����!
		}
		break;
	};
}

void GiftEvent( DWORD dwUserIdx )
{
//	sprintf(txt, "EXEC dbo.up_event20080425_attend_check %u, %d", dwUserIdx, g_nServerSetNum );
//	g_DB.LoginQuery( eQueryType_FreeQuery, eGiftEvent, dwUserIdx, txt);
}

void RGiftEvent( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == NULL ) return;

	USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	int result = atoi((char*)pData[0].Data[0]);
	DWORD eventidx = 0;

	switch( result )
	{
	case 1:
		{
			eventidx = 6;
		}
		break;
	case 2:
		{
			eventidx = 7;
		}
		break;
	case 3:
		{
			eventidx = 8;
		}
		break;
	}

	GIFTMGR->ExcuteEvent( pUserInfo->dwCharacterID, eventidx );
}

//-------------------------------------------------------------------------------------------------
  //	NAME		: RSiegeRecallUpdate
  //	DESC		: ��ȯ�� db ���� ������Ʈ ��� ó�� �߰�.
  //	PROGRAMMER	: Yongs Lee
  //	DATE		: September 2, 2008
  //-------------------------------------------------------------------------------------------------
  void RSiegeRecallUpdate( LPQUERY pData, LPDBMESSAGE pMessage )
  {
  	SIEGERECALLMGR->RSiegeRecallUpdate( pData, pMessage ) ;
  }
  
  
  //-------------------------------------------------------------------------------------------------
  //	NAME		: RSiegeRecallLoad
  //	DESC		: ��ȯ�� db ���� �ε� ó�� �߰�.
  //	PROGRAMMER	: Yongs Lee
  //	DATE		: August 13, 2008
  //-------------------------------------------------------------------------------------------------
  void RSiegeRecallLoad( LPQUERY pData, LPDBMESSAGE pMessage )
  {
  	SIEGERECALLMGR->Result_ObjInfo_Syn( pData, pMessage ) ;
  }
  
  
  
  
  
  //-------------------------------------------------------------------------------------------------
  //	NAME		: RSiegeRecallInsert
  //	DESC		: ��ȯ�� db ���� �߰� ó�� �߰�.
  //	PROGRAMMER	: Yongs Lee
  //	DATE		: August 31, 2008
  //-------------------------------------------------------------------------------------------------
  void RSiegeRecallInsert(LPQUERY pData, LPDBMESSAGE pMessage ) 
  {
  	SIEGERECALLMGR->RSiegeRecallInsert( pData, pMessage ) ;
  }
  
  
  
  
  
  ////-------------------------------------------------------------------------------------------------
  ////	NAME		: RSiegeRecallRemove
  ////	DESC		: ��ȯ�� db ���� ���� ó�� �߰�.
  ////	PROGRAMMER	: Yongs Lee
  ////	DATE		: September 7, 2008
  ////-------------------------------------------------------------------------------------------------
  //void RSiegeRecallRemove(LPQUERY pData, LPDBMESSAGE pMessage )
  //{
  //	SIEGERECALLMGR->RSiegeRecallRemove( pData, pMessage ) ;
  //}
  
  
// 081205 LUJ, �йи� ����Ʈ �α�
void InsertLogFamily( eFamilyLog logType, DWORD familyIndex, DWORD playerIndex, const char* memo )
{
	char buffer[ MAX_NAME_LENGTH + 1 ] = { 0 };
	SafeStrCpy( buffer, memo, sizeof( buffer ) / sizeof( *buffer ) );
	
	// 090325 ONS ���͸�����üũ
	if(IsCharInString(buffer, "'"))	return;

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.TP_FAMILY_LOG_INSERT %d, %d, %d, \'%s\'",
		logType,
		familyIndex,
		playerIndex,
		memo );
}

// 090525 ShinJS --- �̸� �˻����� ��Ƽ�ʴ� �߰�
void SearchCharacterForInvitePartyMember( DWORD dwRequestPlayerID, char* pInvitedName )
{
	sprintf(txt, "EXEC  dbo.MP_SEARCH_FOR_INVITEPARTY \'%s\'", pInvitedName);
	g_DB.Query(eQueryType_FreeQuery, eSearchCharacterForInvitePartyMember, dwRequestPlayerID, txt);
}

void RSearchCharacterForInvitePartyMember( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// ��Ƽ�ʴ��û Player
	const DWORD dwRequestPlayerID = pMessage->dwID;
	USERINFO* pRequestPlayer = g_pUserTableForObjectID->FindUser( dwRequestPlayerID );
	if( !pRequestPlayer )
		return;
	
	// �̸��˻� ������ ����
	pRequestPlayer->bIsSearchingByName = FALSE;

	DWORD dwCheckSearchTime = 0;
	
	const DWORD dwResult = atoi( (char*)pData->Data[0] );
	switch( dwResult )
	{
	case 1:
		{
			// ��Ƽ�ʴ����� �������� �ʴ� ���

			MSG_DWORD msg;
			msg.Category = MP_PARTY;
			msg.Protocol = MP_PARTY_INVITE_BYNAME_NACK;
			msg.dwData = eErr_Add_NoPlayer;

			// Error ����
			g_Network.Send2User( pRequestPlayer->dwConnectionIndex, (char*)&msg, sizeof(msg) );

			// �۾� �˻�ð� ����
			dwCheckSearchTime = ePartyInvite_CheckSearchTimeNoPlayer;
		}
		break;
		
	case 2:
		{
			// ��Ƽ�ʴ����� ���������� ���� ���

			MSG_DWORD msg;
			msg.Category = MP_PARTY;
			msg.Protocol = MP_PARTY_INVITE_BYNAME_NACK;
			msg.dwData = eErr_Add_NotConnectedPlayer;

			// Error ����
			g_Network.Send2User( pRequestPlayer->dwConnectionIndex, (char*)&msg, sizeof(msg) );

			// �۾� �˻�ð� ����
			dwCheckSearchTime = ePartyInvite_CheckSearchTimeNotConnectPlayer;
		}
		break;
	
	case 3:
		{
			// ��Ƽ�ʴ����� �̹� �ٸ� ��Ƽ�� ���� �ִ� ���

			MSG_DWORD msg;
			msg.Category = MP_PARTY;
			msg.Protocol = MP_PARTY_INVITE_BYNAME_NACK;
			msg.dwData = eErr_Add_AlreadyinParty;

			// Error ����
			g_Network.Send2User( pRequestPlayer->dwConnectionIndex, (char*)&msg, sizeof(msg) );

			// �۾� �˻�ð� ����
			dwCheckSearchTime = ePartyInvite_CheckSearchTimeAlreadyInParty;
		}
		break;

	case 4:
		{
			// ��Ƽ�ʴ����� �������� ���

			// �۾� �˻�ð� ����
			dwCheckSearchTime = ePartyInvite_CheckSearchTimeSuccess;

			const DWORD dwInvitedPlayerID = atoi( (char*)pData->Data[1] );
			USERINFO* pInvitedPlayer = g_pUserTableForObjectID->FindUser( dwInvitedPlayerID );

			// ��Ƽ�ʴ����� ���� Agent, ���� �ʿ� �����ϴ� ���
			if( pInvitedPlayer &&
				pRequestPlayer->dwMapServerConnectionIndex == pInvitedPlayer->dwMapServerConnectionIndex )
			{
				// ���� MSG �̿��Ͽ� �ʴ�ó��
				MSG_DWORD2 msg;
				msg.Category	= MP_PARTY;
				msg.Protocol	= MP_PARTY_ADD_SYN;
				msg.dwObjectID	= dwRequestPlayerID;
				msg.dwData1		= dwInvitedPlayerID;

				g_Network.Send2Server( pRequestPlayer->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg) );
				break;
				
			}

			// ���� �ٸ� Agent or �ٸ� �ʿ� �����ϴ� ��� ��Ƽ�� ��ȿ���� Ȯ���Ѵ�
			MSG_DWORD msg;
			msg.Category	= MP_PARTY;
			msg.Protocol	= MP_PARTY_INVITE_BYNAME_SYN;
			msg.dwObjectID	= dwRequestPlayerID;
			msg.dwData		= dwInvitedPlayerID;

            g_Network.Send2Server( pRequestPlayer->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	}


	// �۾� ���ѽð� Ȯ��
	if( gCurTime < pRequestPlayer->dwInvitePartyByNameLastTime + dwCheckSearchTime )
	{
		// ����Ƚ�� �߰�
		++pRequestPlayer->nInvitePartyByNameCnt;

		// ����Ƚ���� �ʰ��� ��� ���ѽð� ����
		if( pRequestPlayer->nInvitePartyByNameCnt >= ePartyInvite_LimitCnt )
		{
			MSG_DWORD2 limitTimeMsg;
			memset( &limitTimeMsg, 0, sizeof(limitTimeMsg) );
			limitTimeMsg.Category = MP_PARTY;
			limitTimeMsg.Protocol = MP_PARTY_INVITE_BYNAME_NACK;
			limitTimeMsg.dwData1 = eErr_SettingLimitTime;

			switch( dwResult )
			{
				// ��Ƽ�ʴ����� �������� �ʴ� ���
			case 1:
				limitTimeMsg.dwData2 = pRequestPlayer->dwInvitePartyByNameDelayTime = ePartyInvite_OverCntLimitTimeNoPlayer;
				break;

				// ��Ƽ�ʴ����� ���������� ���� ���
			case 2:
				limitTimeMsg.dwData2 = pRequestPlayer->dwInvitePartyByNameDelayTime = ePartyInvite_OverCntLimitTimeNotConnectPlayer;
				break;

				// ��Ƽ�ʴ����� �̹� �ٸ� ��Ƽ�� ���� �ִ� ���
			case 3:
				limitTimeMsg.dwData2 = pRequestPlayer->dwInvitePartyByNameDelayTime = ePartyInvite_OverCntLimitTimeAlreadyInParty;
				break;

				// ��Ƽ�ʴ����� �������� ���
			case 4:
				limitTimeMsg.dwData2 = pRequestPlayer->dwInvitePartyByNameDelayTime = ePartyInvite_OverCntLimitTimeSuccess;
				break;
			}

			// ������ ����
			g_Network.Send2User( pRequestPlayer->dwConnectionIndex, (char*)&limitTimeMsg, sizeof(limitTimeMsg) );
		}
	}

	// �۾��ð� ����
	pRequestPlayer->dwInvitePartyByNameLastTime = gCurTime;
}

// ���̺�Ʈ
void WebEvent( DWORD dwUserIdx, DWORD type )
{
	sprintf( txt, "EXEC dbo.MP_WEB_EVENT %d, '%d'", dwUserIdx, type );
	g_DB.Query( eQueryType_FreeQuery, eItemInsert, 0, txt, dwUserIdx );
}

