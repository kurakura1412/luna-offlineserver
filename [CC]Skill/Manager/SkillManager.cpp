#include "stdafx.h"

#ifdef _MAP00_
//#include "[lib]yhlibrary/YHLibrary.h"
#include "[lib]yhlibrary/HashTable.h"
#include "[lib]yhlibrary/PtrList.h"
//#include "[lib]yhlibrary/cLooseLinkedList.h"
#include "[lib]yhlibrary/cLinkedList.h"
//#include "[lib]yhlibrary/IndexGenerator.h"
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

#include "skillmanager.h"
#include "Info/ActiveSkillInfo.h"
#include "Info/BuffSkillInfo.h"
#include "Delay/Delay.h"
#include "Tree/SkillTree.h"
#include "[CC]Header/GameResourceManager.h"
#include "UserTable.h"
#include "MHFile.h"
#include "Player.h"
// 080708 LUJ, ±¤¿ª ½ºÅ³À» »ç¿ëÇÏ´Â ÇÔÁ¤ Ã³¸®¸¦ À§ÇØ ÂüÁ¶
#include "Trap.h"
#include "[CC]Skill/Server/Object/BuffSkillObject.h"
#include "CharMove.h"
#include "PackedData.h"

#include "BattleSystem_Server.h"
#include "[CC]ServerModule/DataBase.h"
#include "MapDBMsgParser.h"
#include "SkillTreeManager.h"

#include "ObjectStateManager.h"

#include "ItemManager.h"
// 080602 LUJ, Àü¼­¹ö ´ë»ó ½ºÅ³ ±â´ÉÀ§ÇØ ÂüÁ¶
#include "Guild.h"
#include "GuildManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "hseos/Family/SHFamilyManager.h"
#include "pet.h"
#include "MoveManager.h"
#include "SiegeWarfareMgr.h"
// 080821 KTH -- SIegeDungeonManager Include
#include "[CC]SiegeDungeon/SiegeDungeonMgr.h"
#include "VehicleManager.h"
// 081006 LYW --- SkillManager : °ø¼ºÀü Ä³ÅÍÆÞÆ® °ø°Ý Ã³¸®¿¡ ÇÊ¿äÇÑ Çì´õ Æ÷ÇÔ.
#include "SiegeRecallMgr.h"
#include "SiegeWarfareMgr.h"
// 081209 LUJ, ½ºÅ³ Å¸°Ù Á¤»óÀ» Ã¼Å©ÇÒ ¶§ ±æµåÀü ¿©ºÎµµ °Ë»çÇÏ±â À§ÇØ ÂüÁ¶
#include "GuildFieldWarMgr.h"

cSkillManager::cSkillManager(void)
{
}

cSkillManager::~cSkillManager(void)
{
}

void cSkillManager::Init()
{
	/// Hash Å×ÀÌºí ÃÊ±âÈ­
	m_SkillInfoTable.Initialize(1000);
	m_SkillObjectTable.Initialize(10000);
	m_SkillTreeTable.Initialize(64);

	/// ½ºÅ³ ¸®½ºÆ® ·Îµù
	LoadSkillInfoList();
}

void cSkillManager::Release()
{
	/// ½ºÅ³ ¿ÀºêÁ§Æ® »èÁ¦
	cSkillObject* pObject;
	m_SkillObjectTable.SetPositionHead();
	while((pObject = m_SkillObjectTable.GetData())!= NULL)
	{
		ReleaseSkillObject(pObject);
	}
	m_SkillObjectTable.RemoveAll();

	/// ½ºÅ³ Á¤º¸ »èÁ¦
	cSkillInfo* pSInfo = NULL;
	m_SkillInfoTable.SetPositionHead();
	while((pSInfo = m_SkillInfoTable.GetData())!= NULL)
	{
		delete pSInfo;
	}
	m_SkillInfoTable.RemoveAll();

	SKILL_TREE_INFO* pInfo = NULL;

	m_SkillTreeTable.SetPositionHead();
	while((pInfo = m_SkillTreeTable.GetData())!= NULL)
	{
		delete pInfo;
	}
	m_SkillTreeTable.RemoveAll();

}


void cSkillManager::LoadSkillInfoList()
{
	CMHFile file;
	file.Init("System/Resource/SkillList.bin","rb");

	while(FALSE == file.IsEOF())
	{
		TCHAR text[MAX_PATH] = {0};
		file.GetLine(
			text,
			sizeof(text) / sizeof(*text));

		cActiveSkillInfo* const pInfo = new cActiveSkillInfo;
		pInfo->Parse(text);

		if( pInfo->GetIndex() == 0 )
		{
			delete pInfo;
			continue;
		}

		m_SkillInfoTable.Add(pInfo,pInfo->GetIndex());

		// 091211 LUJ, ½ºÅ³ °³¼ö¸¦ ¼¾´Ù
		const DWORD index = pInfo->GetIndex() / 100 * 100 + 1;
		++mSkillSize[index];
	}

	file.Release();
	file.Init("System/Resource/Skill_Buff_List.bin","rb");	

	while(FALSE == file.IsEOF())
	{
		TCHAR text[MAX_PATH] = {0};
		file.GetLine(
			text,
			sizeof(text) / sizeof(*text));

		cBuffSkillInfo* const pInfo = new cBuffSkillInfo;
		pInfo->Parse(text);

		if( pInfo->GetIndex() == 0 )
		{
			delete pInfo;
			continue;
		}

		m_SkillInfoTable.Add(pInfo,pInfo->GetIndex());

		// 091211 LUJ, ½ºÅ³ °³¼ö¸¦ ¼¾´Ù
		const DWORD index = pInfo->GetIndex() / 100 * 100 + 1;
		++mSkillSize[index];
	}

	file.Release();
	file.Init("System/Resource/skill_get_list.bin","rb");

	while(FALSE == file.IsEOF())
	{
		SKILL_TREE_INFO* pInfo = new SKILL_TREE_INFO;

		pInfo->ClassIndex = file.GetWord();
		pInfo->Count = file.GetWord();

		for( WORD cnt = 0; cnt < pInfo->Count; cnt++ )
		{
			pInfo->SkillIndex[ cnt ] = file.GetDword();
		}

		m_SkillTreeTable.Add( pInfo, pInfo->ClassIndex );
	}

	file.Release();
}

cSkillObject* cSkillManager::GetSkillObject(DWORD SkillObjID)
{
	return (cSkillObject*)m_SkillObjectTable.GetData(SkillObjID);
}

cSkillInfo* cSkillManager::GetSkillInfo(DWORD SkillInfoIdx)
{
	return (cSkillInfo*)m_SkillInfoTable.GetData(SkillInfoIdx);
}

void cSkillManager::Process()
{
	for( std::list< DWORD >::iterator processIter = mProcessList.begin(), 
		itend = mProcessList.end()
	; processIter != itend ; ++processIter )
	{
		const DWORD objectIndex = *processIter;
		cSkillObject* const skillObject = GetSkillObject(objectIndex);
		if( 0 == skillObject ||
			SKILL_STATE_DESTROY == skillObject->Update() )
		{
			ReleaseSkillObject(skillObject);
			processIter = mProcessList.erase( processIter );
#ifdef _MYLUNA_
			if( processIter == itend )
				break;
#else
			if( processIter == mProcessList.end() )
				break;
#endif
			continue;
		}
	}
}

void cSkillManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_SKILL_START_SYN:
		{
			MSG_SKILL_START_SYN* pmsg = ( MSG_SKILL_START_SYN* )pMsg;
				
			CObject* object = g_pUserTable->FindUser( pmsg->Operator );
		
			if( ! object )
			{
				break;
			}

			// 080904 LYW --- SkillManager : °ø¼ºÀü Áß »ç¿ëÇÒ ¼ö ¾ø´Â ½ºÅ³ Ã¼Å©.
			if( SIEGEWARFAREMGR->Is_CastleMap() == FALSE &&
				SIEGEWARFAREMGR->IsSiegeWarfareZone() &&
				SIEGEWARFAREMGR->GetState() > eSiegeWarfare_State_Before )
			{
				if( SIEGEWARFAREMGR->IsFobiddenSkill(pmsg->SkillIdx) ) return ;
			}

			SKILL_RESULT result = SKILL_FAIL_ETC;

			if( object->GetObjectKind() == eObjectKind_Player )
				result = OnSkillStartSyn( pmsg );
			else if( object->GetObjectKind() == eObjectKind_Pet )
				result = OnPetSkillStartSyn( pmsg );

			// 080610 LUJ, ½ºÅ³ ½ÇÆÐ ½Ã ¸Þ½ÃÁö¸¦ ¹ÝÈ¯ÇÏµµ·Ï ÇÔ
			if( result != SKILL_SUCCESS )
			{
				CObject* object = g_pUserTable->FindUser( pmsg->Operator );

				if( ! object )
				{
					break;
				}

				MSG_DWORD2 message;
				ZeroMemory( &message, sizeof( message ) );
				message.Category	= MP_SKILL;
				message.Protocol	= MP_SKILL_START_NACK;
				message.dwData1		= pmsg->SkillIdx;
				message.dwData2		= result;

				object->SendMsg( &message, sizeof( message ) );
			}
		}
		break;
	// 080602 LUJ, Àü¼­¹ö ´ë»ó ½ºÅ³ÀÌ ÀüÆÄµÊ
	case MP_SKILL_START_TO_MAP:
		{
			const MSG_ITEM_SKILL_START_TO_MAP* const receivedMessage = (MSG_ITEM_SKILL_START_TO_MAP*)pMsg;
			const cActiveSkillInfo* const skillInfo = GetActiveInfo( receivedMessage->mSkill.wSkillIdx );

			if(0 == skillInfo ||
				TARGET_KIND_WORLD != skillInfo->GetInfo().Area)
			{
				break;
			}

			typedef std::set< DWORD >	ObjectIndexSet;
			ObjectIndexSet				objectIndexSet;

			switch(skillInfo->GetInfo().AreaTarget)
			{
			case eSkillAreaTargetGuild:
				{
					GUILDMEMBERINFO memberInfo[ MAX_GUILD_MEMBER ] = { 0 };
					CGuild*	guild = GUILDMGR->GetGuild( receivedMessage->dwObjectID );

					if( ! guild )
					{
						break;
					}

					guild->GetTotalMember( memberInfo );

					for(
						DWORD size = sizeof( memberInfo ) / sizeof( *memberInfo );
						size--; )
					{
						objectIndexSet.insert( memberInfo[ size ].MemberIdx );
					}

					break;
				}
			case eSkillAreaTargetFamily:
				{
					const DWORD familyIndex	 = receivedMessage->dwObjectID;
					// 080602 LUJ, ÆÐ¹Ð¸®´Â ¸Ê ¼­¹ö¿¡ µ¥ÀÌÅ¸°¡ ¾ø°í, ¿¡ÀÌÀüÆ®¿¡ ÀÖ´Ù. ¹ø°Å·ÓÁö¸¸
					//				DB¿¡ È¸¿ø ¸ñ·ÏÀ» Äõ¸®ÇØ¼­ ¹öÇÁ¸¦ Àû¿ëÇÏµµ·Ï ÇÏÀÚ.
					// 081012 LUJ, ÀÏºÎ ·ÎÄÃÀº ÇÁ·Î½ÃÀú È£Ãâ ½Ã ´ë¼Ò¹®ÀÚ±îÁö ÀÏÄ¡ÇØ¾ß ÇÑ´Ù. ´Ü, dbo´Â ¼Ò¹®ÀÚ·Î ÇØ¾ßÇÔ. ÀÌ¿¡ ¸ðµç ÇÁ·Î½ÃÀú ÀÌ¸§À» ¼öÁ¤ÇÔ
					g_DB.FreeMiddleQuery(
						RSkillAddToFamily,
						skillInfo->GetIndex(),
						"EXEC dbo.MP_FAMILY_MEMBER_LOADINFO %d",
						familyIndex );
					break;
				}
			case eSkillAreaTargetParty:
				{
					const DWORD	partyIndex	= receivedMessage->dwObjectID;
					CParty*		party		= PARTYMGR->GetParty( partyIndex );

					if( ! party )
					{
						break;
					}

					for( DWORD i = 0; i < MAX_PARTY_LISTNUM; ++i )
					{
						objectIndexSet.insert(party->GetMemberID(i));
					}

					break;
				}
			}

			// 080602 LUJ, ¼±ÅÃµÈ Áý´ÜÀ» ´ë»óÀ¸·Î ¹öÇÁ ½ºÅ³À» Àû¿ëÇÑ´Ù
			for(
				ObjectIndexSet::const_iterator it = objectIndexSet.begin();
				objectIndexSet.end() != it;
				++it )
			{
				CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( *it );

				if( !	player ||
						player->GetObjectKind() != eObjectKind_Player )
				{
					continue;
				}

				AddBuffSkill(
					*player,
					skillInfo->GetInfo());
			}

			break;
		}
	case MP_SKILL_UPDATE_TARGET_SYN:
		{
			MSG_SKILL_UPDATE_TARGET* pmsg = ( MSG_SKILL_UPDATE_TARGET* )pMsg;

			OnSkillTargetUpdate( pmsg );
		}
		break;
	case MP_SKILL_CANCEL_SYN:
		{
			MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;
			OnSkillCancel( pmsg );
		}
		break;
		// 091127 LUJ, ÇÃ·¹ÀÌ¾î¸¦ ¼ÒÈ¯ÇÑ´Ù
	case MP_SKILL_RECALL_SYN:
		{
			const MSG_NAME_DWORD3* const receivedMessage = (MSG_NAME_DWORD3*)pMsg;
			const DWORD	recallPlayerIndex = receivedMessage->dwObjectID;
			const DWORD	targetPlayerinex = receivedMessage->dwData1;
			const DWORD	skillIndex = receivedMessage->dwData2;
			const MAPTYPE targetMap = MAPTYPE(receivedMessage->dwData3);
			LPCTSTR targetPlayerName = receivedMessage->Name;

			Recall(
				recallPlayerIndex,
				targetPlayerinex,
				targetPlayerName,
				targetMap,
				skillIndex);
		}
		break;
	// 100211 ONS ºÎÈ°°è¿­ ½ºÅ³ »ç¿ë½Ã ´ë»óÀÚ¿¡°Ô ºÎÈ°ÀÇ»ç¸¦ ¹¯´Â Ã³¸® Ãß°¡
	case MP_SKILL_RESURRECT_ACK:
		{
			MSGBASE* pmsg = ( MSGBASE* )pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );

			if( ! pPlayer || eObjectKind_Player != pPlayer->GetObjectKind() )
			{
				break;
			}

			const cActiveSkillInfo* const pSkillInfo = GetActiveInfo(
				pPlayer->GetCurResurrectIndex());

			if(0 == pSkillInfo)
			{
				break;
			}
			
			// ºÎÈ°´ë»óÀÌ ¼ö¶ôÇßÀ» °æ¿ì, ºÎÈ° Ã³¸®¸¦ ½ÇÇàÇÑ´Ù.
			EXPTYPE exp = pPlayer->OnResurrect();
			exp = (EXPTYPE)(exp * ( pSkillInfo->GetInfo().UnitData / 100.f )) ;
			if( exp )
			{
				pPlayer->AddPlayerExpPoint( exp );
			}
			pPlayer->SetCurResurrectIndex( 0 );	
		}
		break;
	// ºÎÈ° ½ºÅ³¿¡ ÀÇÇÑ ºÎÈ°À» °ÅºÎÇßÀ» °æ¿ì
	case MP_SKILL_RESURRECT_NACK:
		{
			MSGBASE* pmsg = ( MSGBASE* )pMsg;
			CObject* object = g_pUserTable->FindUser( pmsg->dwObjectID );
			if( ! object || eObjectKind_Player != object->GetObjectKind() )
			{
				break;
			}

			CPlayer* pPlayer = (CPlayer*)object;
			pPlayer->SetCurResurrectIndex( 0 );	
		}
		break;
	}
}

// 080602 LUJ, ¹öÇÁ ½ºÅ³À» Àû¿ëÇÑ´Ù
// 080616 LUJ, CPlayer -> CObject·Î º¯°æ
void cSkillManager::AddBuffSkill( CObject& object, const ACTIVE_SKILL_INFO& skillInfo )
{
	for(
		DWORD size = sizeof( skillInfo.Buff ) / sizeof( *( skillInfo.Buff ) );
		size--; )
	{
		const DWORD	buffSkillIndex = skillInfo.Buff[ size ];
		const cBuffSkillInfo* const info = GetBuffInfo( buffSkillIndex );

		if( !	info ||
				info->GetKind() != SKILLKIND_BUFF )
		{
			continue;
		}

		BuffSkillStart(
			object.GetID(),
			info->GetInfo().Index,
			info->GetInfo().DelayTime ? info->GetInfo().DelayTime : 1,
			info->GetInfo().Count );

		if( info->GetInfo().NoUpdate )
			return;

		// 080602 LUJ, DelayTimeÀ» ÃÖ¼Ò 1ÀÌ»ó ÁØ ÀÌÀ¯: CBuffSkillObject::ProcessStart()ÀÇ ¹öÇÁ ½ºÅ³ »ý¼º ºÎºÐ¿¡¼­ ÂüÁ¶ÇÔ
		CharacterBuffAdd(
			object.GetID(),
			info->GetInfo().Index,
			info->GetInfo().DelayTime ? info->GetInfo().DelayTime : 1,
			info->GetInfo().Count,
			info->GetInfo().IsEndTime );
	}
}

SKILL_RESULT cSkillManager::OnSkillStartSyn(MSG_SKILL_START_SYN* pmsg, ITEMBASE* pItemBase)
{
	CPlayer* const pOperator = (CPlayer*)g_pUserTable->FindUser(pmsg->Operator);

	if(0 == pOperator)
	{
		return SKILL_OPERATOR_INVALUED;
	}
	else if(eObjectKind_Player != pOperator->GetObjectKind())
	{
		return SKILL_OPERATOR_INVALUED;
	}
	else if(0 < pOperator->CurCastingSkillID)
	{
		return SKILL_OPERATOR_INVALUED;
	}
	else if(eBATTLE_KIND_GTOURNAMENT == pOperator->GetBattle()->GetBattleKind())
	{
		if(eBattleTeam_Max == pOperator->GetBattleTeam())
		{
			return SKILL_STATE_INVALUED;
		}
	}

	const cActiveSkillInfo* const pSkillInfo = SKILLMGR->GetActiveInfo( pmsg->SkillIdx );

	if(0 == pSkillInfo)
	{
		return SKILL_INFO_INVALUED;
	}
	else if(FALSE == pSkillInfo->IsExcutableSkillState(*pOperator, pmsg->mConsumeItem))
	{
		return SKILL_STATE_INVALUED;
	}
	else if(pOperator->IsCoolTime(pSkillInfo->GetInfo()))
	{
		return SKILL_COOLING;
	}
	else if(0 ==  pItemBase)
	{
		const SkillScript& skillScript = GAMERESRCMNGR->GetSkillScript( pmsg->SkillIdx );

		if( skillScript.mIsNeedCheck )
		{
			const SKILL_BASE* const skillBase = pOperator->GetSkillTree().GetData(
				pmsg->SkillIdx / 100 * 100 + 1 );

			if( ! skillBase )
			{
				LogScriptHack(
					*pOperator,
					"SkillList.bin",
					"*player has no skill" );
				return SKILL_OPERATOR_INVALUED;
			}

			const BYTE skillLevelFromClient = BYTE( pmsg->SkillIdx % 100 );
			const BOOL isWrongSkillLevel = ( skillLevelFromClient > skillBase->Level );

			if( isWrongSkillLevel )
			{
				LogScriptHack(
					*pOperator,
					"SkillList.bin",
					"*wrong level" );
				return SKILL_OPERATOR_INVALUED;
			}
		}
	}
	// 080410 KTH -- ¾ÆÀÌÅÛ »ç¿ëÀ¸·Î ½ºÅ³À» »ç¿ëÇÒ °æ¿ì ¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ È®ÀÎÇÑ´Ù.
	else
	{
		const ITEM_INFO* const itemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );

		if(0 == itemInfo)
		{
			return SKILL_OPERATOR_INVALUED;
		}
		else if(pSkillInfo->GetIndex() != itemInfo->SupplyValue)
		{
			return SKILL_OPERATOR_INVALUED;
		}
	}

	switch(pSkillInfo->GetInfo().Unit)
	{
	case UNITKIND_PHYSIC_ATTCK:
		{
			if(pOperator->GetAbnormalStatus()->IsBlockAttack)
			{
				return SKILL_STATE_INVALUED;
			}

			break;
		}
	default:
		{
			if(pOperator->GetAbnormalStatus()->IsSilence)
			{
				return SKILL_STATE_INVALUED;
			}

			break;
		}
	}

	OBJECTSTATEMGR_OBJ->EndObjectState(
		pOperator,
		eObjectState_Immortal);

	VECTOR3 TargetPos = {0};
	CObject* pTargetObject = 0;

	if(FALSE == GetMainTargetPos(&pmsg->MainTarget, &TargetPos, &pTargetObject))
	{
		return SKILL_TARGET_INVALUED;
	}

	switch(pTargetObject ? pTargetObject->GetObjectKind() : eObjectKind_None)
	{
	case eObjectKind_Player:
		{
			CPlayer* const targetPlayer = (CPlayer*)pTargetObject;

			if(targetPlayer->GetImmortalTime() &&
				targetPlayer->GetBattleTeam() != pOperator->GetBattleTeam())
			{
				return SKILL_TARGET_IMMORTALED;
			}
			else if(pOperator->IsPKMode())
			{
				break;
			}
			else if(targetPlayer->IsPKMode())
			{
				break;
			}
			else if(SIEGEWARFAREMGR->IsSiegeWarfareZone(g_pServerSystem->GetMapNum()))
			{
				break;
			}
			else if(SIEGEDUNGEONMGR->IsSiegeDungeon(g_pServerSystem->GetMapNum()))
			{
				break;
			}
			else if(GUILDWARMGR->IsGuildWar(targetPlayer->GetGuildIdx()) &&
				GUILDWARMGR->IsGuildWar(pOperator->GetGuildIdx()))
			{
				break;
			}
			else if(UNITKIND_PHYSIC_ATTCK != pSkillInfo->GetInfo().Unit &&
				UNITKIND_MAGIC_ATTCK != pSkillInfo->GetInfo().Unit)
			{
				if(UNITKIND_RESURRECTION == pSkillInfo->GetInfo().Unit &&
					eObjectState_Die != OBJECTSTATEMGR_OBJ->GetObjectState(pTargetObject))
				{
					return SKILL_TARGET_INVALUED;
				}

				break;
			}
			else if(eBATTLE_KIND_NONE != pOperator->GetBattle()->GetBattleKind() &&
				eBATTLE_KIND_NONE != targetPlayer->GetBattle()->GetBattleKind())
			{
				break;
			}
			else if(pTargetObject != pOperator)
			{
				return SKILL_TARGET_INVALUED;
			}

			for(CTargetListIterator iter(&pmsg->TargetList);
				0 < iter.GetNextTarget();)
			{
				CObject* const pObject = g_pUserTable->FindUser(
					iter.GetTargetID());

				if(0 == pObject)
				{
					continue;
				}
				else if(eObjectKind_Player != pObject->GetObjectKind())
				{
					continue;
				}

				return SKILL_TARGET_INVALUED;
			}

			break;
		}
	case eObjectKind_Monster:
	case eObjectKind_BossMonster:
	case eObjectKind_SpecialMonster:
	case eObjectKind_FieldBossMonster:
	case eObjectKind_FieldSubMonster:
	case eObjectKind_ToghterPlayMonster:
	case eObjectKind_ChallengeZoneMonster:
		{
			pOperator->SetLastActionTime(gCurTime);
			break;
		}
	}

	switch(pSkillInfo->GetInfo().Target)
	{
	case eSkillTargetKind_None:
		{
			return SKILL_TARGET_INVALUED;
		}
	case eSkillTargetKind_OnlyOthers:
		{
			if( pTargetObject == pOperator )
			{
				return SKILL_TARGET_INVALUED;
			}

			break;
		}
	case eSkillTargetKind_OnlySelf:
		{
			if( pTargetObject != pOperator )
			{
				return SKILL_TARGET_INVALUED;
			}

			break;
		}
	}

	const float TargetRadius = (pTargetObject ? pTargetObject->GetRadius() : 0);

	if(pSkillInfo->IsInRange(*pOperator, TargetPos, TargetRadius) == FALSE )
	{
		return SKILL_RANGE_NOTENOUGH;
	}
	else if(SKILLKIND_ONOFF == pSkillInfo->GetKind())
	{
		BOOL destroy = FALSE;

		for(WORD i = 0; i < MAX_BUFF_COUNT; ++i)
		{
			const DWORD idx = pSkillInfo->GetInfo().Buff[ i ];
			cSkillObject* const pSkill = pTargetObject->GetBuffList().GetData( idx / 100 );

			if(0 == pSkill)
			{
				continue;
			}
			else if(pSkill->GetSkillIdx() > pSkillInfo->GetInfo().Buff[ i ])
			{
				continue;
			}

			pSkill->SetEndState();
			pSkill->EndState();
			destroy = TRUE;
		}

		if( destroy )
		{
			// 100615 ShinJS --- ½ºÅ³Á¦°Å Á¤º¸¸¦ Àü¼ÛÇÑ´Ù.
			MSG_DWORD msg;
			ZeroMemory( &msg, sizeof(msg) );
			msg.Category = MP_SKILL;
			msg.Protocol = MP_SKILL_START_DESTROY_ACK;
			msg.dwObjectID = pOperator->GetID();
			msg.dwData = pmsg->mConsumeItem.dwDBIdx;

			pOperator->SendMsg( &msg, sizeof(msg) );

			return SKILL_SUCCESS;
		}
	}

	// 090629 ONS ÇöÀç ½ÇÇàÇÑ ½ºÅ³°ú ±âÁ¸¿¡ ½ÇÇàÁßÀÎ ¹öÇÁ½ºÅ³ÀÌ °°Àº ±×·ìÀÌ Á¸ÀçÇÒ °æ¿ì ÇØ´ç ¹öÇÁ½ºÅ³À» Á¦°ÅÇÑ´Ù.
	// ¸¶Á·ÀÇ °¡È£°è¿­(º£¸£Á¦ÀÇ °¡È£, ·çÇÇ¾ÆÀÇ °¡È£, ÆÄ¸£¼¼ÀÌÄ«ÀÇ °¡È£)ÀÇ ¹öÇÁ½ºÅ³ÀÇ °æ¿ì, 
	// ½ºÅ³ ¹ßµ¿½Ã ±âÁ¸¿¡ ½ÇÇàÁßÀÌ´ø °¡È£ ¹öÇÁ½ºÅ³ÀÌ ÀÖ´Ù¸é ÇØÁ¦ÇÏ°í ½ÇÇàÇØ¾ßÇÑ´Ù. 
	// È®ÀåÀ» À§ÇØ °¡È£°è¿­À» ÇÏ³ªÀÇ ±×·ìÀ¸·Î ÁöÁ¤ÇÏ°í ´Ù¸¥ ±×·ì¿¡ ¿µÇâÀ» ¹ÞÁö ¾Ê°í Á¦°ÅÇÒ ¼ö ÀÖµµ·Ï Ã³¸®.
	RemoveGroupBuffSkill(
		pTargetObject,
		pSkillInfo->GetInfo());

	// 080602 LUJ, Àü¿ª ½ºÅ³Àº DB¿¡¼­ Á÷Á¢ Ã³¸®ÇÑ ÈÄ Àü¼ÛµÈ´Ù. Ãë¼Ò µîÀ» À§ÇØ ¿ÀºêÁ§Æ® ID¸¦ Á÷Á¢ ¹Þ¾Æ¿Â´Ù
	const DWORD skillObjectIndex = GetNewSkillObjectID();

	// ±æµå/ÆÐ¹Ð¸® Æ÷ÀÎÆ® µî µ¿±âÈ­°¡ ÇÊ¿äÇÑ °æ¿ì¿¡¸¸ Ã³¸®ÇÑ´Ù. ÀÌ·¯ÇÑ °ÍµéÀº ÀÏ´Ü ½ºÅ³À» ¹ßµ¿½ÃÅ°°í
	// Á¶°ÇÀÌ ÀûÇÕÁö ¾ÊÀ» ¶§ Ãë¼ÒÇÑ´Ù. µû¶ó¼­ ½ºÅ³À» Ãë¼ÒÇØµµ ¼Ò¸ð°¡ µÈ´Ù.
	switch(pSkillInfo->GetInfo().mConsumePoint.mType)
	{
	case ACTIVE_SKILL_INFO::ConsumePoint::TypeFamily:
	case ACTIVE_SKILL_INFO::ConsumePoint::TypeGuild:
		{
			const ACTIVE_SKILL_INFO::ConsumePoint& consumePoint = pSkillInfo->GetInfo().mConsumePoint;

			g_DB.FreeMiddleQuery(
				RSkillCheckUsable,
				skillObjectIndex,
				"EXEC dbo.MP_SKILL_CHECK_USABLE %d, %d, %d, %d",
				pOperator->GetID(),
				pSkillInfo->GetIndex(),
				ACTIVE_SKILL_INFO::ConsumePoint::TypeFamily == consumePoint.mType ? consumePoint.mValue : 0,
				ACTIVE_SKILL_INFO::ConsumePoint::TypeGuild == consumePoint.mType ? consumePoint.mValue : 0);
			break;
		}
	}

	cSkillObject* const pSObj = pSkillInfo->GetSkillObject();

	if(0 == pSObj)
	{
		return SKILL_INFO_INVALUED;
	}

	sSKILL_CREATE_INFO info;
	info.level = pSkillInfo->GetLevel();
	info.operatorId = pOperator->GetID();
	info.mainTarget = pmsg->MainTarget;
	info.skillDir = pmsg->SkillDir;	
	// 080602 LUJ, DB ¾÷µ¥ÀÌÆ® ½ÇÆÐ ½Ã ½ºÅ³ ¿ÀºêÁ§Æ®¸¦ Ãë¼ÒÇÒ ¼ö ÀÖµµ·Ï À§¿¡¼­ ÀÎµ¦½º¸¦ ¹Ì¸® ¹Þ¾Æ³õµµ·Ï Çß´Ù
	info.skillObjectId = skillObjectIndex;
	info.pos = (pSkillInfo->GetInfo().Target ? TargetPos : *CCharMove::GetPosition(pOperator));
	info.usedItem = pmsg->mConsumeItem;

	// 090108 LUJ, ¸ÖÆ¼ Å¸°ÙÀ» ¼³Á¤ÇÒ ¶§ À¯È¿ ¿©ºÎ¸¦ °Ë»çÇÏ¹Ç·Î, ¸ÕÀú ÃÊ±âÈ­¸¦ ÇØ¾ßÇÑ´Ù.
	pSObj->Init( &info );
	pSObj->SetMultiTarget( &( pmsg->TargetList ) );
	pSObj->SetBattle( pOperator->GetBattleID(), pOperator->GetBattleTeam() );
	pSObj->SetGridID( pOperator->GetGridID() );
  
// --- skr 16012020
  //pSkillInfo->isAllowByRelifeEX();
  pSObj->setRelifeAllow( pSkillInfo->isAllowByRelifeEX() );

	/// Å×ÀÌºí¿¡ µî·Ï
	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	mProcessList.push_back(pSObj->GetID());
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	pOperator->SetCurrentSkill(pSObj->GetID());
	pOperator->SetCoolTime(pSkillInfo->GetInfo());

	/// ½ºÅ³ »ý¼º ¼º°ø
	return SKILL_SUCCESS;
}

void cSkillManager::MonsterAttack(DWORD SkillIdx,CObject* pMonster,CObject* pTarget)
{
	// 081006 LYW --- AttackSkillUnit : °ø¼ºÀüÀÇ Ä³ÅÍÆÞÆ®°¡ ¼º ¸¶½ºÅÍ ±æµå´Â °ø°ÝÇÏÁö ¾Êµµ·Ï Ã³¸®.
	if( !pMonster || !pTarget ) return ;

	// °ø¼º Á¸ÀÎÁö È®ÀÎÇÑ´Ù.
	if( SIEGEWARFAREMGR->IsSiegeWarfareZone() )
	{
		const EObjectKind byOperatorKind = pMonster->GetObjectKind();
		const EObjectKind byTargetKind	= pTarget->GetObjectKind();

		if(byOperatorKind & eObjectKind_Monster)
		{
			// ½ÃÀü ¸ó½ºÅÍ°¡ Ä³ÅÍÆÞÆ®ÀÎÁö È®ÀÎÇÑ´Ù.
			if( SIEGERECALLMGR->IsRecallMonster( pMonster->GetID() ) )
			{
				DWORD dwTargetGuildIdx = 0 ;
				DWORD dwMasterGuildIdx = 0 ;

				// Ä³ÅÍÆÞÆ®°¡ ·ç¼¾/Á¦ºáÀÇ ¸¶½ºÅÍ ±æµå¸¦ ±¸ºÐÇÏ±â À§ÇØ Á¾Á· ´ëÇ¥ÄÚµå¸¦ »ç¿ëÇÏ¿©
				// Ä³ÅÍ ÆÞÆ® ±¸ºÐÀ» ÇÏ±â·Î ÇÑ´Ù --- October 6, 2008 by ¼Û°¡¶÷
				// MonsterRace == 100 ==> ·ç½¨(81) / MonsterRace == 101 ==> Á¦ºá(83)
				const BASE_MONSTER_LIST& pInfo = ((CMonster*)pMonster)->GetSMonsterList() ;

				if( pInfo.MonsterRace == 100 )
				{
					dwMasterGuildIdx = SIEGEWARFAREMGR->GetCastleGuildIdx(eNeraCastle_Lusen) ;
				}
				else if( pInfo.MonsterRace == 101 )
				{
					dwMasterGuildIdx = SIEGEWARFAREMGR->GetCastleGuildIdx(eNeraCastle_Zebin) ;
				}

				// Å¸°ÙÀÌ Player ¶ó¸é,
				if( byTargetKind == eObjectKind_Player )
				{
					dwTargetGuildIdx = ((CPlayer*)pTarget)->GetGuildIdx() ;
				}
				// Å¸°ÙÀÌ Pet ÀÌ¶ó¸é ¸¶½ºÅÍ¸¦ ¹Þ¾Æ ±æµå ÀÎµ¦½º¸¦ ¹Þ°í, 
				// ¼ºÀÇ ¸¶½ºÅÍ ±æµåÀÎÁö È®ÀÎÇÑ´Ù.
				else if( byTargetKind == eObjectKind_Pet )
				{
					CPlayer* const player = (CPlayer*)g_pUserTable->FindUser(
						pTarget->GetOwnerIndex());

					if(0 == player)
					{
						dwTargetGuildIdx = 0;
					}
					else if(eObjectKind_Player == player->GetObjectKind())
					{
						dwTargetGuildIdx = player->GetGuildIdx();
					}
				}

				// ¼ºÀÇ ¸¶½ºÅÍ ±æµå¿Í °°À¸¸é °ø°ÝÀ» ÇÏÁö ¾Ê´Â´Ù.
				if( dwMasterGuildIdx != 0 )
				{
					if( dwMasterGuildIdx == dwTargetGuildIdx ) return ;
				}
			}
		}
	}

	const cActiveSkillInfo* const pSkillInfo = GetActiveInfo(SkillIdx);
	
	if(pSkillInfo == NULL)
		return;

	if( pMonster->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, Ä§¹¬ »óÅÂÀÎ °æ¿ì, ¹°¸® °ø°ÝÀ» Á¦¿ÜÇÑ ¸ðµç ½ºÅ³À» Â÷´ÜÇÑ´Ù
		if( pSkillInfo->GetInfo().Unit != UNITKIND_PHYSIC_ATTCK )
			return;
	}
	if( pMonster->GetAbnormalStatus()->IsBlockAttack )
	{
		if( pSkillInfo->GetInfo().Unit == UNITKIND_PHYSIC_ATTCK )
			return;
	}
	
	VECTOR3* pMonPos = CCharMove::GetPosition(pMonster);
	VECTOR3* pTargetPos = CCharMove::GetPosition(pTarget);
	VECTOR3 dir = *pTargetPos - *pMonPos;

	// ½ºÅ³ ¿ÀºêÁ§Æ® »ý¼º
	sSKILL_CREATE_INFO info;
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// ½ºÅ³ °´Ã¼ ID
	info.skillObjectId = GetNewSkillObjectID();

	info.level = 1;
//	info.pOperator = pMonster;
	info.operatorId = pMonster->GetID();
	info.mainTarget.SetMainTarget( pTarget->GetID() );
	info.skillDir = dir;	
	info.remainTime = 0;
	/// ½ºÅ³ »ý¼º À§Ä¡
	if( pSkillInfo->GetInfo().Target )
	{
		info.pos = *CCharMove::GetPosition(pTarget);
	}
	else
	{
		info.pos = *CCharMove::GetPosition(pMonster);
	}
	
	// battle
	//info.pBattle = pMonster->GetBattle();

	// Å×ÀÌºí¿¡ µî·Ï
	pSObj->Init( &info );
	pSObj->SetBattle( pMonster->GetBattleID(), pMonster->GetBattleTeam() );
	pSObj->SetGridID( pMonster->GetGridID() );

	// 080708 LUJ, Å¸°Ù ¸ñ·ÏÀ» ´ã±â À§ÇØ ¼±¾ð
	CTargetList targetList;
	targetList.Clear();

	switch( pSkillInfo->GetInfo().Area )
	{
	case TARGET_KIND_MULTI:
	case TARGET_KIND_MULTI_MONSTER:
	case TARGET_KIND_MULTI_ALL:
		{
			CUserTable::ObjectKindSet objectKindeSet;
			objectKindeSet.insert( eObjectKind_Monster );
			objectKindeSet.insert( eObjectKind_BossMonster );
			objectKindeSet.insert( eObjectKind_FieldBossMonster );
			objectKindeSet.insert( eObjectKind_FieldSubMonster );
			objectKindeSet.insert( eObjectKind_Trap );
			objectKindeSet.insert( eObjectKind_Player );

			g_pUserTable->GetTargetInRange(
				CCharMove::GetPosition( pTarget ),
				pSkillInfo->GetInfo().Range,
				&targetList,
				pMonster->GetGridID(),
				0,
				objectKindeSet );
			break;
		}
	}

	CTargetList fileteredTargetList;
	fileteredTargetList.Clear();

	// ¼ÒÀ¯ÁÖ À¯¹«¿¡ µû¶ó Æ¯Á¤ ´ë»óÀ» Á¦¿ÜÇÑ´Ù
	{
		std::set< EObjectKind > filterContainer;

		if(CObject* const owner = g_pUserTable->FindUser(pMonster->GetOwnerIndex()))
		{
			if(eObjectKind_Player == owner->GetObjectKind())
			{
				filterContainer.insert(
					eObjectKind_Monster);
				filterContainer.insert(
					eObjectKind_BossMonster);
				filterContainer.insert(
					eObjectKind_SpecialMonster);
				filterContainer.insert(
					eObjectKind_FieldBossMonster);
				filterContainer.insert(
					eObjectKind_FieldSubMonster);
				filterContainer.insert(
					eObjectKind_ToghterPlayMonster);
				filterContainer.insert(
					eObjectKind_ChallengeZoneMonster);
				filterContainer.insert(
					eObjectKind_Trap);
			}
			else if(eObjectKind_Monster & owner->GetObjectKind())
			{
				filterContainer.insert(
					eObjectKind_Player);
				filterContainer.insert(
					eObjectKind_Pet);
			}
		}
		else
		{
			filterContainer.insert(
				eObjectKind_Player);
			filterContainer.insert(
				eObjectKind_Pet);
		}

		CTargetListIterator iterator( &targetList );
		CTargetListIterator iteratorFiltered( &fileteredTargetList );

		while( iterator.GetNextTarget() )
		{
			if(CObject* const object = g_pUserTable->FindUser( iterator.GetTargetID() ))
			{
				if(filterContainer.end() == filterContainer.find(object->GetObjectKind()))
				{
					continue;
				}

				iteratorFiltered.AddTargetWithNoData( 
					object->GetID(),
					SKILLRESULTKIND_NONE);
			}
		}

		iteratorFiltered.Release();
	}
	
	pSObj->SetMultiTarget( &fileteredTargetList );

	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	mProcessList.push_back(pSObj->GetID());
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	pMonster->SetCurrentSkill( pSObj->GetID() );
}

BOOL cSkillManager::PetAttack(DWORD SkillIdx,CObject* pPet,CObject* pTarget)
{
	const cActiveSkillInfo* const pSkillInfo = GetActiveInfo(SkillIdx);
	
	if(pSkillInfo == NULL)
		return SKILL_INFO_INVALUED;

	if( pPet->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, Ä§¹¬ »óÅÂÀÎ °æ¿ì, ¹°¸® °ø°ÝÀ» Á¦¿ÜÇÑ ¸ðµç ½ºÅ³À» Â÷´ÜÇÑ´Ù
		if( pSkillInfo->GetInfo().Unit != UNITKIND_PHYSIC_ATTCK )
			return SKILL_STATE_INVALUED;
	}
	if( pPet->GetAbnormalStatus()->IsBlockAttack )
	{
		if( pSkillInfo->GetInfo().Unit == UNITKIND_PHYSIC_ATTCK )
			return SKILL_STATE_INVALUED;
	}
	
	VECTOR3* pPetPos = CCharMove::GetPosition(pPet);
	VECTOR3* pTargetPos = CCharMove::GetPosition(pTarget);
	VECTOR3 dir = *pTargetPos - *pPetPos;

	float dist = CalcDistanceXZ( pPetPos, pTargetPos ) - pTarget->GetRadius();

	if( dist > pSkillInfo->GetInfo().Range )
		return SKILL_RANGE_NOTENOUGH;

	// ½ºÅ³ ¿ÀºêÁ§Æ® »ý¼º
	sSKILL_CREATE_INFO info;
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// ½ºÅ³ °´Ã¼ ID
	info.skillObjectId = GetNewSkillObjectID();

	info.level = 1;
//	info.pOperator = pPet;
	info.operatorId = pPet->GetID();
	info.mainTarget.SetMainTarget( pTarget->GetID() );
	info.skillDir = dir;	
	info.remainTime = 0;
	/// ½ºÅ³ »ý¼º À§Ä¡
	if( pSkillInfo->GetInfo().Target )
	{
		info.pos = *CCharMove::GetPosition(pTarget);
	}
	else
	{
		info.pos = *CCharMove::GetPosition(pPet);
	}
	
	// battle
	//info.pBattle = pMonster->GetBattle();

	// Å×ÀÌºí¿¡ µî·Ï
	pSObj->Init( &info );
	pSObj->SetBattle( pPet->GetBattleID(), pPet->GetBattleTeam() );
	pSObj->SetGridID( pPet->GetGridID() );

	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	mProcessList.push_back(pSObj->GetID());
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	pPet->SetCurrentSkill( pSObj->GetID() );

	return SKILL_SUCCESS;
}

cSkillObject* cSkillManager::OnBuffSkillStart( cSkillInfo* pSkillInfo, sSKILL_CREATE_INFO* pCreateInfo )
{
	CObject* pOperator = g_pUserTable->FindUser( pCreateInfo->operatorId );

	if( !pOperator )
		return NULL;

	CObject* pTargetObject = g_pUserTable->FindUser( pCreateInfo->mainTarget.dwMainTargetID );
	if( !pTargetObject )
	{
		return NULL;
	}

	// 100614 ShinJS --- Á¦°ÅÁ¶°Ç¿¡ ¸¸Á·ÇÑ °æ¿ì ¹öÇÁ¸¦ »ý¼ºÇÏÁö ¾Ê´Â´Ù.
	if( pSkillInfo->GetType() != cSkillInfo::TypeBuff )
		return NULL;

	const BUFF_SKILL_INFO& buffInfo = ((cBuffSkillInfo*)pSkillInfo)->GetInfo();
	BYTE buffRuleResult = eBUFF_START_RESULT_SUCCESS;

	if( pTargetObject->GetObjectBattleState() == eObjectBattleState_Peace && 
		buffInfo.mRulePeace == eBuffSkillActiveRule_Delete )
	{
		buffRuleResult = eBUFF_START_RESULT_PEACE_RULE_DELETE;
	}
	else if( pTargetObject->GetObjectBattleState() == eObjectBattleState_Battle &&
			 buffInfo.mRuleBattle == eBuffSkillActiveRule_Delete )
	{
		 buffRuleResult = eBUFF_START_RESULT_BATTLE_RULE_DELETE;
	}
	else if( CCharMove::IsMoving( pTargetObject ) == TRUE &&
			 buffInfo.mRuleMove == eBuffSkillActiveRule_Delete )
	{
		buffRuleResult = eBUFF_START_RESULT_MOVE_RULE_DELETE;
	}
	else if( CCharMove::IsMoving( pTargetObject ) == FALSE &&
			 buffInfo.mRuleStop == eBuffSkillActiveRule_Delete )
	{
		buffRuleResult = eBUFF_START_RESULT_STOP_RULE_DELETE;
	}
	else if( pTargetObject->GetState() == eObjectState_Rest &&
		buffInfo.mRuleRest == eBuffSkillActiveRule_Delete )
	{
		buffRuleResult = eBUFF_START_RESULT_REST_RULE_DELETE;
	}

	// ¹öÇÁ ÇØÁ¦Á¶°Ç¸¸Á·½Ã ´ë»óÀÚ¿¡°Ô ¾Ë·ÁÁÜ.
	if( buffRuleResult != eBUFF_START_RESULT_SUCCESS )
	{
		MSG_DWORD2 msg;
		ZeroMemory( &msg, sizeof(msg) );
		msg.Category = MP_SKILL;
		msg.Protocol = MP_SKILL_BUFF_START_FAIL_NOTIFY;
		msg.dwObjectID = pTargetObject->GetID();
		msg.dwData1 = buffRuleResult;
		msg.dwData2 = buffInfo.Index;

		pTargetObject->SendMsg( &msg, sizeof(msg) );
		return NULL;
	}

	cSkillObject* const pSObj = pSkillInfo->GetSkillObject();
	if(0 == pSObj)
	{
		return 0;
	}

	/// ½ºÅ³ °´Ã¼ ID
	pCreateInfo->skillObjectId = GetNewSkillObjectID();

	/// ½ºÅ³ °´Ã¼ ÃÊ±âÈ­
	pSObj->Init( pCreateInfo );
	pSObj->SetBattle( pOperator->GetBattleID(), pOperator->GetBattleTeam() );
	pSObj->SetGridID( pOperator->GetGridID() );

	/// Å×ÀÌºí¿¡ µî·Ï
	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	mProcessList.push_back(pSObj->GetID());
	g_pServerSystem->AddSkillObject( pSObj, &( pCreateInfo->pos ) );

	return pSObj;
}

// 090204 LUJ, countÀÇ ¼öÄ¡ ¹üÀ§¸¦ È®ÀåÇÔ
void cSkillManager::BuffSkillStart( DWORD characteridx, DWORD skillidx, DWORD remaintime, int count, VECTOR3* pDir )
{
	CObject* pTarget = g_pUserTable->FindUser( characteridx );

	if( !pTarget )
	{
		return;
	}

	cSkillInfo* pSkillInfo = GetSkillInfo( skillidx );
	sSKILL_CREATE_INFO createinfo;

	createinfo.level = pSkillInfo->GetLevel();
	createinfo.operatorId = characteridx;
	createinfo.mainTarget.SetMainTarget(characteridx);
	createinfo.pos = *CCharMove::GetPosition(pTarget);
	createinfo.remainTime = remaintime;
	createinfo.count = count;
	createinfo.isNew = false;
	if( pDir != NULL )
	{
		createinfo.skillDir = *pDir;
	}

	if(cBuffSkillObject* const pBuff = (cBuffSkillObject*)OnBuffSkillStart( pSkillInfo, &createinfo ))
	{
		pTarget->AddEventSkill(*pBuff);
	}
}

void cSkillManager::AddPassive( CPlayer* pTarget, DWORD SkillIdx )
{
	const cActiveSkillInfo* const pActive = GetActiveInfo(
		SkillIdx);

	if(0 == pActive)
	{
		return;
	}
	
	for( DWORD cnt = 0; cnt < MAX_BUFF_COUNT; ++cnt)
	{
		const DWORD index = pActive->GetInfo().Buff[ cnt ];
		const cBuffSkillInfo* const pSkillInfo = GetBuffInfo( index );

		if( !pSkillInfo )
		{
			continue;
		}
		else if( BUFF_SKILL_INFO::Condition::TypeNone != pSkillInfo->GetInfo().mCondition.mType )
		{
			pTarget->AddSpecialSkill( pSkillInfo );
		}
		else if(pTarget->IsEnable(pSkillInfo->GetInfo()))
		{
			pSkillInfo->AddPassiveStatus( pTarget );
		}
	}
}

void cSkillManager::RemovePassive( CPlayer* pTarget, DWORD SkillIdx )
{
	const cActiveSkillInfo* const pActive = GetActiveInfo(
		SkillIdx);

	if(0 == pActive)
	{
		return;
	}

	for( DWORD cnt = 0; cnt < MAX_BUFF_COUNT; ++cnt)
	{
		const DWORD index = pActive->GetInfo().Buff[ cnt ];
		const cBuffSkillInfo* const pSkillInfo = GetBuffInfo( index );

		if( !pSkillInfo )
		{
			continue;
		}
		else if( BUFF_SKILL_INFO::Condition::TypeNone != pSkillInfo->GetInfo().mCondition.mType )
		{
			pTarget->RemoveSpecialSkill( pSkillInfo );
		}
		else if(FALSE == pTarget->IsEnable(pSkillInfo->GetInfo()))
		{
			pSkillInfo->RemovePassiveStatus( pTarget );
		}
	}
}

void cSkillManager::ReleaseSkillObject(cSkillObject* pSkillObject)
{
	/// ½ºÅ³À» »èÁ¦ÇÑ´Ù
	m_SkillObjectTable.Remove( pSkillObject->GetID() );
	g_pServerSystem->RemoveSkillObject( pSkillObject->GetID() );
	
	// 091211 LUJ, ½ºÅ³ ¿ÀºêÁ§Æ® Àç»ç¿ë °úÁ¤¿¡¼­ Ä¡¸íÀûÀÎ ¸Þ¸ð¸® ´©¼ö°¡ ÀÖ´Ù
	//			Á¤Àû Ä³½ºÆÃÀ¸·Î ¸Þ¸ð¸® »ç¿ë Å©±â¿¡ °ü°è¾øÀÌ »ç¿ëÇÏ¿© ºñÇã¿ë
	//			¸Þ¸ð¸®¿¡ ´ëÇÑ ¾²±â°¡ Á¡ÁøÀûÀ¸·Î ¹ß»ýÇÑ´Ù. ÀÌ¸¦ ÇÇÇÏ±â À§ÇØ
	//			ÀÏ´Ü Àç»ç¿ëÀ» ÇÏÁö ¾Êµµ·Ï ÇÑ´Ù. ÈÄ¿¡ Á»´õ ¾ÈÀüÇÏµµ·Ï ½ºÅ³
	//			¿ÀºêÁ§Æ® º°·Î Á¤Àû ¸Þ¸ð¸® Ç®À» ¸¸µé ¿¹Á¤ÀÌ´Ù
	SAFE_DELETE(pSkillObject);
}

DWORD cSkillManager::GetNewSkillObjectID()
{
	//static tempID = SKILLOBJECT_ID_START;
	//if(++tempID >= SKILLOBJECT_ID_MAX)
	//	tempID = SKILLOBJECT_ID_START;
	static DWORD tempID = SKILLOBJECT_ID_MAX;
	if(++tempID >= SKILLOBJECT_ID_START)
		tempID = SKILLOBJECT_ID_MAX;
	return tempID;
}


DWORD cSkillManager::GetSkillSize( DWORD index ) const
{
	SkillSize::const_iterator it = mSkillSize.find( index );

	return mSkillSize.end() == it ? 0 : it->second;
}


cSkillManager* cSkillManager::GetInstance()
{
	static cSkillManager instance;

	return &instance;
}


void cSkillManager::OnSkillTargetUpdate( MSG_SKILL_UPDATE_TARGET* pmsg )
{
	cSkillObject* pSkillObject = m_SkillObjectTable.GetData( pmsg->SkillObjectID );

	if(0 == pSkillObject)
	{
		return;
	}

	CObject* const object = pSkillObject->GetOperator();

	// 100115 LUJ, ´Ù¸¥ ÇÃ·¹ÀÌ¾î¿¡ ÀÇÇØ Å¸°ÙÀÌ ¹Ù²îÁö ¾Êµµ·Ï °Ë»ç¸¦ Ã¶ÀúÈ÷ ÇÑ´Ù
	if(0 == object)
	{
		return;
	}
	else if(eObjectKind_Player != object->GetObjectKind())
	{
		return;
	}
	else if(pmsg->dwObjectID != object->GetID())
	{
		return;
	}

    pSkillObject->SetMultiTarget(&(pmsg->TargetList));
}

void cSkillManager::OnSkillCancel( MSG_DWORD* pmsg )
{
	CObject* pObject = g_pUserTable->FindUser( pmsg->dwObjectID );

	if( !pObject || pObject->GetObjectKind() != eObjectKind_Player )
	{
		return;
	}

	CPlayer* pOperator = (CPlayer*)pObject;
	cSkillObject* pSkillObject = m_SkillObjectTable.GetData( pmsg->dwData );
	if( !pSkillObject )
	{
		return;
	}

	const cActiveSkillInfo* pSkillInfo = GetActiveInfo( pSkillObject->GetSkillIdx() );
	if( !pSkillInfo )
		return;
	else if(SKILL_STATE_END == pSkillObject->GetSkillState() ||
		FALSE == pOperator->IsCanCancelSkill())
	{
		MSG_DWORD msg;
		msg.Category = MP_SKILL;
		msg.Protocol = MP_SKILL_CANCEL_NACK;
		msg.dwObjectID = pmsg->dwObjectID;
		msg.dwData = pOperator->GetSkillCancelDelay();

		pOperator->SendMsg( &msg, sizeof(msg) );
		return;
	}

	MSG_DWORD msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_CANCEL_NOTIFY;
	msg.dwObjectID = pmsg->dwObjectID;
	msg.dwData = pmsg->dwData;

	PACKEDDATA_OBJ->QuickSend( pOperator, &msg, sizeof( MSG_DWORD ) );

	pSkillObject->SetEndState();
	pSkillObject->EndState();
	pOperator->ResetCoolTime( pSkillInfo->GetInfo() );
}

// 080616 LUJ, ¾×Æ¼ºê ½ºÅ³À» Ãß°¡ÇÑ´Ù
void cSkillManager::AddActiveSkill( CObject& object, DWORD skillIndex )
{
	const cActiveSkillInfo* const skillInfo = GetActiveInfo( skillIndex );

	if( ! skillInfo )
	{
		return;
	}

	cSkillObject* skillObject = skillInfo->GetSkillObject();

	if( ! skillObject )
	{
		return;
	}

	/// ½ºÅ³ »ý¼º Á¤º¸
	sSKILL_CREATE_INFO createInfo;
	ZeroMemory( &createInfo, sizeof( createInfo ) );
	createInfo.skillObjectId	= skillObject->GetID();
	createInfo.level			= skillInfo->GetLevel();
//	createInfo.pOperator		= &object;
	createInfo.operatorId		= object.GetID();
	createInfo.mainTarget.SetMainTarget( object.GetID() );
	createInfo.skillDir			= skillObject->GetSkillDir();

	/// ½ºÅ³ »ý¼º À§Ä¡
	if( skillInfo->GetInfo().Target )
	{
		VECTOR3 position = { 0 };
		object.GetPosition( &position );

		createInfo.pos.x = position.x;
		createInfo.pos.z = position.z;
	}
	else
	{
		createInfo.pos = *CCharMove::GetPosition( &object );
	}

	CTargetList targetList;
	// 080708 LUJ, ¹Ýµå½Ã ÃÊ±âÈ­°¡ ÇÊ¿äÇÏ´Ù
	targetList.Clear();
	
	switch(skillInfo->GetInfo().Area )
	{
	case TARGET_KIND_SINGLE:
	case TARGET_KIND_SINGLE_CIRCLE:
		{
			CTargetListIterator iter( &targetList );

			iter.AddTargetWithNoData( object.GetID(), SKILLRESULTKIND_NONE );
			// 080708 LUJ, Release()°¡ È£ÃâµÇ¾î¾ß Å¸°Ù ³»ÀÇ ¹öÆÛ ±æÀÌ°¡ ¼³Á¤µÈ´Ù
			iter.Release();
			break;
		}
	case TARGET_KIND_MULTI:
	case TARGET_KIND_MULTI_ARC:
	case TARGET_KIND_MULTI_CIRCLE:
	case TARGET_KIND_MULTI_RECT:
		{
			VECTOR3 position = { 0 };
			object.GetPosition( &position );

			g_pUserTable->GetTargetInRange(
				&position,
				skillInfo->GetInfo().AreaData,
				&targetList,
				object.GetGridID() ); 
			break;
		}
	case TARGET_KIND_MULTI_MONSTER:
		{
			CUserTable::ObjectKindSet objectKindeSet;
			objectKindeSet.insert( eObjectKind_Monster );
			objectKindeSet.insert( eObjectKind_BossMonster );
			objectKindeSet.insert( eObjectKind_FieldBossMonster );
			objectKindeSet.insert( eObjectKind_FieldSubMonster );
			objectKindeSet.insert( eObjectKind_ChallengeZoneMonster );
			objectKindeSet.insert( eObjectKind_Trap );

			VECTOR3 position = { 0 };
			object.GetPosition( &position );

			g_pUserTable->GetTargetInRange(
				&position,
				skillInfo->GetInfo().AreaData,
				&targetList,
				object.GetGridID(),
				0,
				objectKindeSet );
			break;
		}
		// 090204 LUJ, ¸ó½ºÅÍ¿Í ÇÃ·¹ÀÌ¾î ¸ðµÎ¸¦ Å¸°ÙÀ¸·Î Àâ´Â´Ù
	case TARGET_KIND_MULTI_ALL:
		{
			CUserTable::ObjectKindSet objectKindSet;
			objectKindSet.insert( eObjectKind_Monster );
			objectKindSet.insert( eObjectKind_BossMonster );
			objectKindSet.insert( eObjectKind_FieldBossMonster );
			objectKindSet.insert( eObjectKind_FieldSubMonster );
			objectKindSet.insert( eObjectKind_ChallengeZoneMonster );
			objectKindSet.insert( eObjectKind_Trap );

			// 100601 ShinJS --- PK °¡´É Áö¿ª¿¡¼­¸¸ PVP Çã¿ë
			const BOOL isValidPlayerKill(TRUE == g_pServerSystem->GetMap()->IsPKAllow());
			if( object.GetObjectKind() == eObjectKind_Player && isValidPlayerKill )
				objectKindSet.insert( eObjectKind_Player );

			VECTOR3 position = { 0 };
			object.GetPosition( &position );

			g_pUserTable->GetTargetInRange(
				&position,
				skillInfo->GetInfo().AreaData,
				&targetList,
				object.GetGridID(),
				0,
				objectKindSet );
			break;
		}
	}

	// 090204 LUJ, ¸ÕÀú ÃÊ±âÈ­µÇ¾î¾ß ÇÑ´Ù
	skillObject->Init( &createInfo );
	skillObject->SetMultiTarget( &targetList );
	skillObject->SetBattle( object.GetBattleID(), object.GetBattleTeam() );
	skillObject->SetGridID( object.GetGridID() );

	/// Å×ÀÌºí¿¡ µî·Ï
	m_SkillObjectTable.Add( skillObject, skillObject->GetID() );
	mProcessList.push_back(skillObject->GetID());
	g_pServerSystem->AddSkillObject( skillObject, &createInfo.pos );
}

SKILL_RESULT cSkillManager::OnPetSkillStartSyn(MSG_SKILL_START_SYN* pmsg, ITEMBASE* pItemBase)
{
	/// ½ÃÀüÀÚ Á¤º¸°¡ ¾ø´Ù¸é ½ÇÆÐ
	// 080602 LUJ, ½ºÅ³ ¹ßµ¿Àº ÇÃ·¹ÀÌ¾î¸¸ ÇÒ ¼ö ÀÖÀ¸¹Ç·Î Ä³½ºÆÃÇÑ´Ù
	CPet* pOperator = ( CPet* )g_pUserTable->FindUser( pmsg->Operator );

	// 080602 LUJ, °Ë»ç ±¸¹® Ãß°¡
	if( !	pOperator											||
			pOperator->GetObjectKind()	!= eObjectKind_Pet	||
			pOperator->CurCastingSkillID )
	{
		return SKILL_OPERATOR_INVALUED;
	}

	// 080410 KTH -- ÀÏ¹Ý ½ºÅ³ÀÏ °æ¿ì ½ºÅ³ Æ®¸®¿¡ Æ÷ÇÔ µÇ¾îÀÖ´Â ½ºÅ³ÀÎÁö È®ÀÎÇÑ´Ù.
	if( pItemBase )
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );
		const cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( pmsg->SkillIdx );

		if( pSkillInfo == NULL || pItemInfo == NULL )
			return SKILL_INFO_INVALUED;

		if( pmsg->SkillIdx / 100 != pItemInfo->SupplyValue / 100 )
			return SKILL_INFO_INVALUED;
	}
	// 090226 LUJ, ½ºÅ³ °Ë»ç
	else
	{
		const SkillScript&	skillScript	= GAMERESRCMNGR->GetSkillScript( pmsg->SkillIdx );
		const BOOL			checkFailed	= ( skillScript.mIsNeedCheck && ! pOperator->IsHaveSkill( pmsg->SkillIdx ) );

		if( checkFailed )
		{
			LogScriptHack(
				*pOperator,
				"SkillList.bin",
				"*pet has no skill" );
			return SKILL_OPERATOR_INVALUED;
		}
	}

	const cSkillInfo* ptemp = GetSkillInfo( pmsg->SkillIdx );

	if( ptemp == NULL )
	{
		return SKILL_INFO_INVALUED;
	}

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )ptemp;

	if( pOperator->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, Ä§¹¬ Áß¿¡´Â ¹°¸® °ø°Ý¸¸ °¡´ÉÇÏ´Ù
		if( pSkillInfo->GetKind() != SKILLKIND_PHYSIC )
		{
			return SKILL_STATE_INVALUED;
		}
	}
	if( pOperator->GetAbnormalStatus()->IsBlockAttack )
	{
		if( pSkillInfo->GetInfo().Unit == UNITKIND_PHYSIC_ATTCK )
			return SKILL_STATE_INVALUED;
	}

	/// ÃÖÃÊ Á¢¼Ó½Ã ¹«Àû »óÅÂ ÇØÁ¦
	if( pOperator->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pOperator, eObjectState_Immortal, 0 );

	/// ±æµå Åä³Ê¸ÕÆ®³ª °ø¼ºÀü¿¡¼­ °ü¶÷ÀÚ´Â ½ºÅ³À» »ç¿ëÇÒ ¼ö ¾ø´Ù
	{
		CObject* const ownerObject = g_pUserTable->FindUser(
			pOperator->GetOwnerIndex());

		if(0 == ownerObject)
		{
			return SKILL_STATE_INVALUED;
		}
		else if(2 == ownerObject->GetBattleTeam())
		{
			return SKILL_STATE_INVALUED;
		}
	}

	if(FALSE == pSkillInfo->IsExcutableSkillState(*pOperator, pmsg->mConsumeItem))
	{
		return SKILL_STATE_INVALUED;
	}

	VECTOR3 TargetPos;
	CObject * pTargetObject = NULL;
	float TargetRadius;

	/// Å¸°Ù ¼³Á¤ È®ÀÎ
	if(GetMainTargetPos(&pmsg->MainTarget,&TargetPos, &pTargetObject) == FALSE)
	{
		/// Å¸°ÙÀÌ ¾ø´Ù
		return SKILL_TARGET_INVALUED;
	}

	switch( pSkillInfo->GetInfo().Target )
	{
	case eSkillTargetKind_None:
		{
			return SKILL_TARGET_INVALUED;
		}
		break;
	case eSkillTargetKind_OnlyOthers:
		{
			if( pTargetObject == pOperator )
			{
				return SKILL_TARGET_INVALUED;
			}
		}
		break;
	case eSkillTargetKind_Ground:
		{
		}
		break;
	case eSkillTargetKind_OnlySelf:
		{
			if( pTargetObject != pOperator )
			{
				return SKILL_TARGET_INVALUED;
			}
		}
		break;
	case eSkillTargetKind_AllObject:
		{
		}
		break;
	}

	/// Å¸°Ù ¿ÀºêÁ§Æ®ÀÇ Å©±â¸¦ ±¸ÇÑ´Ù
	if(pmsg->MainTarget.MainTargetKind == MAINTARGET::MAINTARGETKIND_POS)
		TargetRadius = 0;
	else
	{
		pTargetObject = g_pUserTable->FindUser( pmsg->MainTarget.dwMainTargetID );
		TargetRadius = pTargetObject->GetRadius();
	}	

	if( pSkillInfo->IsInRange(*pOperator, TargetPos, TargetRadius) == FALSE)
	{
		if(pTargetObject)
		{
			RESULTINFO DamageInfo = {0};
			DamageInfo.mSkillIndex = pSkillInfo->GetIndex();

			pTargetObject->Damage(pOperator, &DamageInfo);
		}
		return SKILL_RANGE_NOTENOUGH;
	}
	
	if( pSkillInfo->GetKind() == SKILLKIND_ONOFF )
	{
		BOOL destroy = FALSE;

		for( WORD i = 0; i < MAX_BUFF_COUNT; i++ )
		{
			const DWORD idx = pSkillInfo->GetInfo().Buff[ i ];
			
			if( cSkillObject* pSkill = pTargetObject->GetBuffList().GetData( idx / 100 ) )
			{
				if( pSkill->GetSkillIdx() <= pSkillInfo->GetInfo().Buff[ i ] )
				{
					pSkill->SetEndState();
					pSkill->EndState();
					destroy = TRUE;
				}
			}
		}
		
		if( destroy )
		{
			return SKILL_DESTROY;
		}
	}

	// 080602 LUJ, Àü¿ª ½ºÅ³Àº DB¿¡¼­ Á÷Á¢ Ã³¸®ÇÑ ÈÄ Àü¼ÛµÈ´Ù. Ãë¼Ò µîÀ» À§ÇØ ¿ÀºêÁ§Æ® ID¸¦ Á÷Á¢ ¹Þ¾Æ¿Â´Ù
	const DWORD skillObjectIndex = GetNewSkillObjectID();

	sSKILL_CREATE_INFO info;
	info.level = pSkillInfo->GetInfo().Level;
	info.operatorId = pOperator->GetID();
	info.mainTarget = pmsg->MainTarget;
	info.skillDir = pmsg->SkillDir;	

	/// ½ºÅ³ ¿ÀºêÁ§Æ® »ý¼º
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// ½ºÅ³ °´Ã¼ ID
	// 080602 LUJ, DB ¾÷µ¥ÀÌÆ® ½ÇÆÐ ½Ã ½ºÅ³ ¿ÀºêÁ§Æ®¸¦ Ãë¼ÒÇÒ ¼ö ÀÖµµ·Ï À§¿¡¼­ ÀÎµ¦½º¸¦ ¹Ì¸® ¹Þ¾Æ³õµµ·Ï Çß´Ù
	info.skillObjectId = skillObjectIndex;
	
	if( pSkillInfo->GetInfo().Target )
	{
		info.pos.x = TargetPos.x;
		info.pos.z = TargetPos.z;
	}
	else
	{
		info.pos = *CCharMove::GetPosition(pOperator);
	}

	// 090123 LUJ, °´Ã¼¸¦ ÃÊ±âÈ­ÇÑ ÈÄ Å¸°Ù °Ë»ç¸¦ ¼öÇàÇØ¾ßÇÑ´Ù.
	pSObj->Init( &info );
	pSObj->SetMultiTarget( &( pmsg->TargetList ) );
	pSObj->SetBattle( pOperator->GetBattleID(), pOperator->GetBattleTeam() );
	pSObj->SetGridID( pOperator->GetGridID() );

	/// Å×ÀÌºí¿¡ µî·Ï
	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	mProcessList.push_back(pSObj->GetID());
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	/// ½ºÅ³ »ý¼º ¼º°ø
	return SKILL_SUCCESS;
}

// 090105 LUJ, ÇÃ·¹ÀÌ¾î°¡ ÇÐ½À °¡´ÉÇÑ ½ºÅ³ÀÎÁö Á¶»çÇÑ´Ù
BOOL cSkillManager::IsLearnableSkill( const CHARACTER_TOTALINFO& totalInfo, const SKILL_BASE& skillBase )
{
	// 090106 LUJ, grade °ªÀ» »ç¿ëÇÑ ÈÄ °¨»êÇÏµµ·Ï ¼öÁ¤
	for(	WORD grade = totalInfo.JobGrade;
			0 < grade;
			--grade )
	{
		const DWORD jobType		= DWORD( totalInfo.Job[ grade - 1 ] );
		// 090106 LUJ, Á÷¾÷ÀÌ 1´Ü°èÀÎ °æ¿ì jobTypeÀº Ç×»ó 1ÀÌ µÇ¾î¾ßÇÑ´Ù
		const DWORD jobIndex	= ( totalInfo.Job[ 0 ] * 1000 ) + ( ( totalInfo.Race + 1 ) * 100 ) + ( grade * 10 ) + ( 1 == grade ? 1 : jobType );
		// 090105 LUJ, Á÷¾÷ ¹øÈ£¸¦ ÀÌ¿ëÇØ¼­ ½ºÅ³ ÇÐ½À Æ®¸®ÀÇ Æ÷ÀÎÅÍ¸¦ È¹µæÇÑ´Ù
		const SKILL_TREE_INFO* const skillTree = m_SkillTreeTable.GetData( jobIndex );

		if( ! skillTree )
		{
			break;
		}

		// 090105 LUJ, ½ºÅ³ Æ®¸®¿¡ ÀúÀåµÈ ½ºÅ³ Áß Å¬¶óÀÌ¾ðÆ® ¿äÃ»¿¡ ÀûÇÕÇÑ Á¶°ÇÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù
		for(	DWORD i = 0;
				i < skillTree->Count;
				++i )
		{
			const DWORD serverSkillIndex = skillTree->SkillIndex[ i ] / 100;
			const DWORD clientSkillIndex = skillBase.wSkillIdx / 100;

			if( serverSkillIndex != clientSkillIndex )
			{
				continue;
			}

			const DWORD serverSkillLevel = skillTree->SkillIndex[ i ] % 100;
			// 090106 LUJ, ºÒÇÊ¿äÇÑ ¿¬»ê Á¦°Å
			const DWORD clientSkillLevel = skillBase.Level;

			if( serverSkillLevel >= clientSkillLevel )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void cSkillManager::RemoveGroupBuffSkill( CObject* pObject, const ACTIVE_SKILL_INFO& skillInfo)
{
	cPtrList templist;

	const SkillScript& curSkillScript = GAMERESRCMNGR->GetSkillScript( skillInfo.Index );
	pObject->GetBuffList().SetPositionHead();

	while(cBuffSkillObject* const pSObj = pObject->GetBuffList().GetData())
	{
		const SkillScript& skillScript = GAMERESRCMNGR->GetSkillScript( pSObj->GetSkillIdx() );
		if( ( curSkillScript.mSkillGroupNum > 0 )	&&
			( skillScript.mSkillGroupNum	> 0	)	&&
			( skillScript.mSkillGroupNum == curSkillScript.mSkillGroupNum )	)
		{
			templist.AddTail( pSObj );
		}
	}

	PTRLISTPOS pos = templist.GetHeadPosition();
	while( pos )
	{
		cSkillObject* const pSObj = ( cSkillObject* )templist.GetNext( pos );
		pSObj->SetEndState();
		pSObj->EndState();
		pObject->GetBuffList().Remove( pSObj->GetSkillIdx() );
	}
}

void cSkillManager::Recall(DWORD recallPlayerIndex, DWORD targetPlayerIndex, LPCTSTR targetPlayerName, MAPTYPE targetMap, DWORD skillIndex)
{
	CPlayer* const recallPlayer = (CPlayer*)g_pUserTable->FindUser(recallPlayerIndex);

	if(0 == recallPlayer ||
		recallPlayer->GetObjectKind() != eObjectKind_Player)
	{
		return;
	}

	MSG_RECALL_NACK errorMessage(
		MP_MOVE,
		MP_MOVE_RECALL_NACK,
		MSG_RECALL_NACK::TypeNone,
		recallPlayerIndex,
		0);
	SafeStrCpy(
		errorMessage.mRecallPlayerName,
		recallPlayer->GetObjectName(),
		sizeof(errorMessage.mRecallPlayerName) / sizeof(*errorMessage.mRecallPlayerName));

	if(g_pServerSystem->IsNoRecallMap(*recallPlayer))
	{
		errorMessage.mType = MSG_RECALL_NACK::TypePlayerLocateOnBlockedMap;
		recallPlayer->SendMsg(
			&errorMessage,
			sizeof(errorMessage));
		return;
	}
	else if(recallPlayerIndex == targetPlayerIndex)
	{
		errorMessage.mType = MSG_RECALL_NACK::TypePlayerCannotRecallOneself;
		recallPlayer->SendMsg(
			&errorMessage,
			sizeof(errorMessage));
		return;
	}

	const BOOL isHasNoSkill = (0 == recallPlayer->GetSkillTree().GetData(skillIndex));
	const cActiveSkillInfo* const activeSkillInfo = GetActiveInfo(skillIndex);

	if(isHasNoSkill ||
		0 == activeSkillInfo)
	{
		errorMessage.mType = MSG_RECALL_NACK::TypePlayerHasNoSkill;
		recallPlayer->SendMsg(
			&errorMessage,
			sizeof(errorMessage));
		return;
	}
	else if(IsInvalidRecallTarget(
		*recallPlayer,
		targetPlayerIndex,
		activeSkillInfo->GetInfo()))
	{
		errorMessage.mType = MSG_RECALL_NACK::TypeTargetIsNotExist;
		recallPlayer->SendMsg(
			&errorMessage,
			sizeof(errorMessage));
		return;
	}

	// 091127 LUJ, Á¤º¸¸¦ ¼³Á¤ÇÏ°í ¼ÒÈ¯ÇÑ´Ù
	MSG_RECALL_SYN sendMessage(
		MP_MOVE,
		MP_MOVE_RECALL_ASK_SYN_TO_SERVER);
	sendMessage.mSkillIndex	= skillIndex;
	sendMessage.mRandomPositionRange = activeSkillInfo->GetInfo().AreaData;
	sendMessage.mLimitMiliSecond = activeSkillInfo->GetInfo().UnitData * 1000;
	sendMessage.mChannelIndex = recallPlayer->GetCurChannel();

	// 091127 LUJ, ¸®ÄÝ »ç¿ëÀÚ Á¤º¸ º¹»ç
	{
		MSG_RECALL_SYN::Player& player = sendMessage.mRecallPlayer;
		player.mIndex = recallPlayerIndex;
		player.mMapType	= g_pServerSystem->GetMapNum();
		SafeStrCpy(
			player.mName,
			recallPlayer->GetObjectName(),
			sizeof(player.mName) / sizeof(*player.mName));
	}

	// 091127 LUJ, ¸®ÄÝ ´ë»óÀÚ Á¤º¸ º¹»ç
	{
		MSG_RECALL_SYN::Player& player = sendMessage.mTargetPlayer;
		player.mIndex = targetPlayerIndex;
		player.mMapType	= targetMap;
		SafeStrCpy(
			player.mName,
			targetPlayerName,
			sizeof(player.mName) / sizeof(*player.mName));
	}

	MOVEMGR->RequestToRecall(sendMessage);
}

BOOL cSkillManager::IsInvalidRecallTarget(CPlayer& recallPlayer, DWORD targetPlayerIndex, const ACTIVE_SKILL_INFO& info)
{
	switch(info.AreaTarget)
	{
	case eSkillAreaTargetGuild:
		{
			CGuild* const guild = GUILDMGR->GetGuild(recallPlayer.GetGuildIdx());

			if(0 == guild)
			{
				break;
			}

			const GUILDMEMBERINFO* const member = guild->GetMemberInfo(targetPlayerIndex);

			if(0 == member)
			{
				break;
			}

			return FALSE;
		}
	case eSkillAreaTargetParty:
		{
			CParty* const party = PARTYMGR->GetParty(recallPlayer.GetPartyIdx());

			if(0 == party)
			{
				break;
			}
			else if(party->GetMasterID() != recallPlayer.GetID())
			{
				break;
			}
			else if(FALSE == party->IsPartyMember(targetPlayerIndex))
			{
				break;
			}

			return FALSE;
		}
	}

	return TRUE;
}

SKILL_RESULT cSkillManager::CreateActiveSkill( DWORD dwSkillidx, DWORD dwOperatorID, DWORD dwTargetID, const VECTOR3& dir )
{
	const cActiveSkillInfo* pSkillInfo = GetActiveInfo( dwSkillidx );

	if( ! pSkillInfo )
		return SKILL_INFO_INVALUED;

	CObject* pOperator = g_pUserTable->FindUser( dwOperatorID );
	if( ! pOperator )
		return SKILL_OPERATOR_INVALUED;

	CObject* pTarget = g_pUserTable->FindUser( dwTargetID );
	if( ! pTarget )
		return SKILL_TARGET_INVALUED;

	// ½ºÅ³ ¿ÀºêÁ§Æ® »ý¼º
	sSKILL_CREATE_INFO info;
	info.skillObjectId = GetNewSkillObjectID();
	info.level = 1;
	info.operatorId = dwOperatorID;
	info.mainTarget.SetMainTarget( dwTargetID );
	info.skillDir = dir;
	info.pos = *CCharMove::GetPosition( pTarget );

	cSkillObject* pSObj = pSkillInfo->GetSkillObject();
	pSObj->Init( &info );
	pSObj->SetBattle( pOperator->GetBattleID(), pOperator->GetBattleTeam() );
	pSObj->SetGridID( pOperator->GetGridID() );

	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	mProcessList.push_back( pSObj->GetID() );
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	return SKILL_SUCCESS;
}

const cActiveSkillInfo* cSkillManager::GetActiveInfo(DWORD skillIndex)
{
	if(const cSkillInfo* const skillInfo = GetSkillInfo(skillIndex))
	{
		return cSkillInfo::TypeActive == skillInfo->GetType() ? (cActiveSkillInfo*)skillInfo : 0;
	}

	return 0;
}

const cBuffSkillInfo* cSkillManager::GetBuffInfo(DWORD skillIndex)
{
	if(const cSkillInfo* const skillInfo = GetSkillInfo(skillIndex))
	{
		return cSkillInfo::TypeBuff == skillInfo->GetType() ? (cBuffSkillInfo*)skillInfo : 0;
	}

	return 0;
}

#ifdef _MYLUNA_
void cSkillManager::ReleaseME()
{
	cSkillObject* pObject;
	m_SkillObjectTable.SetPositionHead();
	while((pObject = m_SkillObjectTable.GetData())!= NULL)
	{
		ReleaseSkillObject(pObject);
	}
	m_SkillObjectTable.RemoveAll();
	mProcessList.clear();
}
void cSkillManager::InitME()
{
	
}

#endif // _MYLUNA_

// --- skr 20012020
cBuffSkillInfo* cSkillManager::GetBuffInfoEX(DWORD skillIndex){
	if(const cSkillInfo* const skillInfo = GetSkillInfo(skillIndex))
	{
		return cSkillInfo::TypeBuff == skillInfo->GetType() ? (cBuffSkillInfo*)skillInfo : 0;
	}

	return 0;
}