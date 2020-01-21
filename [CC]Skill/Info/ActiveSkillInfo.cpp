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
//#include "[CC]Header/CommonCalcFunc.h"
//#include "[CC]ServerModule/DataBase.h"
//#include "[CC]ServerModule/Console.h"
#include "Object.h"
//#include "ServerSystem.h"
#endif

#include "activeskillinfo.h"
#include "CharMove.h"
#include "Battle.h"
#include "Player.h"
#include "[CC]Header/GameResourceManager.h"
#include "Object/ActiveSkillObject.h"
#include "Object/BuffSkillObject.h"
#include "ItemManager.h"

cActiveSkillInfo::cActiveSkillInfo(void) :
cSkillInfo(cSkillInfo::TypeActive)
{
	ZeroMemory(
		&mInfo,
		sizeof(mInfo));
}

cActiveSkillInfo::~cActiveSkillInfo(void)
{}

void cActiveSkillInfo::Parse(LPCTSTR text)
{
	GAMERESRCMNGR->Parse(
		text,
		mInfo);
}

BOOL cActiveSkillInfo::IsExcutableSkillState(CObject& pObject, const ICONBASE& usedItem) const
{	
	if( pObject.GetAbnormalStatus()->IsStun ||
		pObject.GetAbnormalStatus()->IsFreezing ||
		pObject.GetAbnormalStatus()->IsStone ||
		pObject.GetAbnormalStatus()->IsSlip ||
		pObject.GetAbnormalStatus()->IsParalysis )
	{
		return FALSE;
	}

	if(pObject.GetObjectKind() == eObjectKind_Player)
	{
		CPlayer& player = (CPlayer&)pObject;

		// 090709 ShinJS --- Å»°Í¿¡ Å¾½Â »óÅÂÀÎ °æ¿ì ½ºÅ³ »ç¿ëºÒ°¡
		if(0 < player.GetMountedVehicle())
		{
			return FALSE;
		}
		else if(FALSE == player.CanSkillState())
		{
			return FALSE;
		}
		else if(player.IsNoEquip(
			mInfo.mArmorType,
			mInfo.mWeaponType,
			mInfo.mWeaponAnimationType))
		{
			return FALSE;
		}
		else if(player.GetMoney() < mInfo.Money)
		{
			return FALSE;
		}
		else if(0 < mInfo.mConsumeItem.wIconIdx)
		{
			CItemSlot* const itemSlot = player.GetSlot(
				eItemTable_Inventory);

			if(NULL == itemSlot)
			{
				return FALSE;
			}

			const ITEMBASE* const itemBase = itemSlot->GetItemInfoAbs(
				usedItem.Position);

			if(NULL == itemBase)
			{
				return FALSE;
			}
			else if(itemBase->dwDBIdx != usedItem.dwDBIdx)
			{
				return FALSE;
			}
			else if(itemBase->wIconIdx != mInfo.mConsumeItem.wIconIdx)
			{
				return FALSE;
			}

			const ITEM_INFO* const itemInfo = ITEMMGR->GetItemInfo(
				itemBase->wIconIdx);

			if(NULL == itemInfo)
			{
				return FALSE;
			}
			else if(0 < itemInfo->Stack)
			{
				if(itemBase->Durability < mInfo.mConsumeItem.Durability)
				{
					return FALSE;
				}
			}
		}

		switch(GetInfo().mConsumePoint.mType)
		{
		case ACTIVE_SKILL_INFO::ConsumePoint::TypeExperience:
			{
				if(player.GetPlayerExpPoint() < GetInfo().mConsumePoint.mValue)
				{
					return FALSE;
				}

				break;
			}
		case ACTIVE_SKILL_INFO::ConsumePoint::TypePlayerKill:
			{
				if(player.GetBadFame() < FAMETYPE(GetInfo().mConsumePoint.mValue))
				{
					return FALSE;
				}

				break;
			}
		}
	}

	// 090731 pdy ÇÊ¿ä¹öÇÁ»óÅÂ Ã¤Å© ¹ö±× ¼öÁ¤ (¸Ê¿¡¼­µµ Ã¤Å©ÇÏ°Ô ¼öÁ¤) 
	if( mInfo.RequiredBuff)												//¿ä±¸ ¹öÇÁÀÎµ¦½º °ªÀÌ À¯È¿ÇÏ´Ù¸é 
	{
		cBuffSkillObject* pSkill = pObject.GetBuffList().GetData(mInfo.RequiredBuff / 100);

		if(NULL == pSkill)
		{
			return FALSE ;
		}
		else if(pSkill->GetSkillIdx() < mInfo.RequiredBuff)
		{
			return FALSE ;
		}
	}

	const WORD MogongMana = WORD( float( pObject.GetMana() ) * mInfo.mMana.mPercent + mInfo.mMana.mPlus );
	DWORD NeedMana = (DWORD)(MogongMana*gEventRate[eEvent_ManaRate]);
	NeedMana = (DWORD) (NeedMana + ( NeedMana * (pObject.GetRateBuffStatus()->DecreaseManaRate / 100 ) ) );

	if( NeedMana > pObject.GetMana() )
	{
		return FALSE;
	}

	const WORD MogongLife = WORD( float( pObject.GetLife() ) * mInfo.mLife.mPercent + mInfo.mLife.mPlus );
	DWORD NeedLife = (DWORD)(MogongLife*gEventRate[eEvent_ManaRate]);

	if( NeedLife > pObject.GetLife() )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL cActiveSkillInfo::IsInRange(CObject& object, VECTOR3& targetPosition,float targetRadius) const
{
	const float toleranceRange = 100.0f;
	const float distance = abs(CalcDistanceXZ(CCharMove::GetPosition(&object), &targetPosition)) - targetRadius - toleranceRange;
	const float range = mInfo.Range + object.GetBonusRange();

	return range >= distance;
}

cSkillObject* cActiveSkillInfo::GetSkillObject() const
{
	return new cActiveSkillObject(
		*this);
}
// --- skr 20012020
BOOL cActiveSkillInfo::isAllowByRelifeEX() const{
  return cSkillInfo::RelifeAllow;
}