#include "stdafx.h"

#ifdef _MAP00_
//#include "[lib]yhlibrary/YHLibrary.h"
#include "[lib]yhlibrary/HashTable.h"
#include "[lib]yhlibrary/PtrList.h"
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

#include "attackskillunit.h"

#include "Target/Target.h"
#include "Object/ActiveSkillObject.h"
#include "Info/ActiveSkillInfo.h"

#include "AttackManager.h"
#include "PackedData.h"
// 080616 LUJ, ÃÂÃÂÃÂºÃÂ¥ÃÂÃÂ® ÃÂ¹ÃÂÃÂ»ÃÂ½ ÃÂ½ÃÂÃÂ¿ÃÂ¡ ÃÂ½ÃÂºÃÂÃÂ³ÃÂÃÂ ÃÂ¾ÃÂÃÂ¼ÃÂÃÂÃÂ» ÃÂÃÂ«ÃÂÃÂÃÂ±ÃÂ¢ ÃÂÃÂ§ÃÂÃÂ ÃÂÃÂ¼ÃÂÃÂ¶
#include "Event.h"

// --- skr 20012020
#include "[Server]Map/Player.h"

cAttackSkillUnit::cAttackSkillUnit(cActiveSkillObject& skillObject) :
cSkillUnit(skillObject)
{}

cAttackSkillUnit::~cAttackSkillUnit(void)
{
}

BOOL cAttackSkillUnit::Excute()
{
	MSG_SKILL_RESULT msg;
  
  //DWORD aaMSG_SKILL_RESULT = sizeof(MSG_SKILL_RESULT);
  //DWORD aaMSGBASE = sizeof(MSGBASE);
  //(void)aaMSG_SKILL_RESULT;
  //(void)aaMSGBASE;

	msg.InitMsg( mpParentSkill->GetID() );
	msg.SkillDamageKind = false;

	CAttackManager::AttackFunction attackFunction = 0;

	switch(mUnitType)
	{
	case UNITKIND_PHYSIC_ATTCK:
		{
			attackFunction = &CAttackManager::PhysicAttack;
			break;
		}
	case UNITKIND_MAGIC_ATTCK:
		{
			attackFunction = &CAttackManager::MagicAttack;
			break;
		}
	default:
		{
			return FALSE;
		}
	}

	CObject* const operatorObject = mpParentSkill->GetOperator();

	if(0 == operatorObject)
	{
		return FALSE;
	}

	BOOL isSucceed = FALSE;
	CTargetListIterator iter(&msg.TargetList);
	mpParentSkill->GetTarget().SetPositionHead();

	while(CObject* const pTarget = mpParentSkill->GetTarget().GetData())
	{
		if( pTarget->GetState() == eObjectState_Die )
		{
			continue;
		}
		else if(operatorObject == pTarget)
		{
			continue;
		}

// --- skr 20012020
    if( pTarget->GetObjectKind() == eObjectKind_Player ){
      CPlayer * pplayer = (CPlayer *)pTarget;
      if( pplayer->IsRelifeON() ){
        continue;
      }
    }

		RESULTINFO damage = { 0 };

		damage.mSkillIndex = mpParentSkill->GetSkillIdx();

		(ATTACKMGR->*attackFunction)(
			operatorObject,
			pTarget,
			&damage,
			mAccuracy,
			mAddDamage,
			mAddType);

		operatorObject->Execute( CGiveDamageEvent( pTarget, damage ) );
		pTarget->Execute( CTakeDamageEvent( operatorObject, damage ) );

		ApplyDamageResult( operatorObject, pTarget, damage );

		iter.AddTargetWithResultInfo( pTarget->GetID(), 1, &damage);
		iter.Release();

//* test load
//		RESULTINFO damage55;
	//	iter.GetTargetData( &damage55 );
//*/
		PACKEDDATA_OBJ->QuickSend( pTarget, &msg, msg.GetMsgLength() );

		isSucceed = (0 < damage.ManaDamage || 0 < damage.RealDamage);
	}

	return isSucceed;
}

// 100219 ShinJS --- Damage (Life/Mana) ÃÂ¸ÃÂ¦ ÃÂÃÂ»ÃÂ¿ÃÂ«ÃÂÃÂÃÂ´ÃÂ.
BOOL cAttackSkillUnit::ApplyDamageResult( CObject* pAttacker, CObject* pTarget, RESULTINFO& damageInfo )
{
	// Mana Damage
	pTarget->ManaDamage( pAttacker, &damageInfo );

	DWORD newLife = pTarget->Damage( pAttacker, &damageInfo );
	if(newLife == 0)
	{
		// 080616 LUJ, ÃÂ»ÃÂ§ÃÂ¸ÃÂ ÃÂ½ÃÂ ÃÂÃÂÃÂºÃÂ¥ÃÂÃÂ®ÃÂ¸ÃÂ¦ ÃÂÃÂ³ÃÂ¸ÃÂ®ÃÂÃÂÃÂµÃÂµÃÂ·ÃÂ ÃÂÃÂÃÂ´ÃÂ
		// 080708 LUJ, ÃÂ°ÃÂ¡ÃÂÃÂÃÂÃÂ/ÃÂÃÂÃÂÃÂÃÂÃÂÃÂ¸ÃÂ¦ ÃÂ¸ÃÂ°ÃÂµÃÂ ÃÂÃÂÃÂÃÂÃÂ·ÃÂ ÃÂ³ÃÂÃÂ±ÃÂ¤ÃÂ´ÃÂ
		pTarget->Execute( CDieEvent( pAttacker, pTarget ) );
		pAttacker->Execute( CKillEvent( pAttacker, pTarget ) );

		ATTACKMGR->sendDieMsg( pAttacker, pTarget );
		pTarget->Die( pAttacker );
	}

	return TRUE;
}