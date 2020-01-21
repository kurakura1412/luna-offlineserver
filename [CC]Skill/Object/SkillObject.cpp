#include "StdAfx.h"

#ifdef _MAP00_
//#include "[lib]yhlibrary/YHLibrary.h"
#include "[lib]yhlibrary/HashTable.h"
//#include "[lib]yhlibrary/PtrList.h"
//#include "[lib]yhlibrary/cLooseLinkedList.h"
//#include "[lib]yhlibrary/cLinkedList.h"
//#include "[lib]yhlibrary/IndexGenerator.h"
//#include "[lib]yhlibrary/cConstLinkedList.h"
//#include "[CC]Header/vector.h"
#include "[CC]Header/protocol.h"
#include "[CC]Header/CommonDefine.h"
#include "[CC]Header/CommonGameDefine.h"
#include "[CC]Header/ServerGameDefine.h"
#include "[CC]Header/CommonStruct.h"
#include "[CC]Header/ServerGameStruct.h"
//#include "[CC]Header/CommonGameFunc.h"
//#include "[CC]Header/GameResourceStruct.h"
//#include "[CC]Header/CommonCalcFunc.h"
//#include "[CC]ServerModule/DataBase.h"
//#include "[CC]ServerModule/Console.h"
#include "Object.h"
//#include "ServerSystem.h"
#endif

#include "SkillObject.h"
#include "Battle.h"
#include "UserTable.h"

// 090205 LUJ, ±¤¿ª ½ºÅ³ Çã¿ë ¿ÀÂ÷ÀÇ ±âº»°ª
float cSkillObject::mAllowRangeForWideSkill = 300.0f;

cSkillObject::cSkillObject(const SKILL_INFO& info) :
mSkillIndex(info.Index),
mSkillLevel(info.Level),
mSkillKind(info.Kind)
{
	// 080602 LUJ, ¿ÀºêÁ§Æ® Å¸ÀÔÀ» ÃÊ±âÈ­ÇÑ´Ù
	mType = cSkillObject::TypeNone;
	m_ObjectKind = eObjectKind_SkillObject;
// --- skr 16012020
  RelifeAllow = FALSE;
}

cSkillObject::~cSkillObject(void)
{}

CObject* cSkillObject::GetOperator()
{
	return g_pUserTable->FindUser( mSkillObjectInfo.operatorId );
//	return mSkillObjectInfo.pOperator;
}

void cSkillObject::Init( sSKILL_CREATE_INFO* pInfo )
{
	/// ½ºÅ³ »ý¼º Á¤º¸ ¼³Á¤
	memcpy(	&mSkillObjectInfo, pInfo, sizeof( sSKILL_CREATE_INFO ) );

	/// ±âº» Á¤º¸ ¼ÂÆÃ
	m_BaseObjectInfo.dwObjectID = pInfo->skillObjectId;
	strcpy(m_BaseObjectInfo.ObjectName,"SkillObject");
	m_BaseObjectInfo.ObjectState = eObjectState_None;
	m_SkillDir = pInfo->skillDir;

	/// »ç¿ëÀÚ Á¤º¸°¡ Àß¸øµÇ¾úÀ» °æ¿ì ½ÇÆÐ
	if( g_pUserTable->FindUser( pInfo->operatorId ) == NULL )
	{
		return;
	}
}

void cSkillObject::EndState()
{
	mState = SKILL_STATE_DESTROY;
	GetOperator()->CurCastingSkillID = 0;
}

DWORD cSkillObject::SetRemoveMsg(DWORD dwReceiverID, MSGBASE*& sendMessage)
{
	static MSG_DWORD message;
	ZeroMemory(&message, sizeof(message));
	message.Category = MP_SKILL;
	message.Protocol = MP_SKILL_SKILLOBJECT_REMOVE;
	message.dwObjectID = dwReceiverID;
	message.dwData = GetID();

	sendMessage = &message;
	return sizeof(message);
}