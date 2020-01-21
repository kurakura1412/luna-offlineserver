#ifndef __SKILLOBJECT_H__INCLUDED
#define __SKILLOBJECT_H__INCLUDED
#pragma once
#include "object.h"

/// ½ºÅ³ÀÇ »óÅÂ
enum SKILL_STATE
{
	SKILL_STATE_CASTING,
	SKILL_STATE_PROCESS,
	SKILL_STATE_END,
	SKILL_STATE_DESTROY,
};

struct sSKILL_CREATE_INFO
{
	DWORD skillObjectId;
	LEVELTYPE level;
	DWORD operatorId;
	MAINTARGET mainTarget;
	VECTOR3	pos;
	VECTOR3	skillDir;
	DWORD remainTime;
	int	count;
	bool isNew;
	ICONBASE usedItem;
	
	sSKILL_CREATE_INFO()
	{
		ZeroMemory(
			this,
			sizeof(*this));
		isNew = true;
	}
};

class cSkillObject :
	public CObject
{
	// 080602 LUJ, ½ºÅ³ ¿ÀºêÁ§Æ® Å¸ÀÔ
public:	
	enum Type
	{
		TypeNone,
		TypeActive,
		TypeBuff,
		TypeOnOff,
	};

	inline Type GetType() const { return mType; }

protected:
	Type mType;
	const DWORD mSkillIndex;
	const LEVELTYPE mSkillLevel;
	const SKILLKIND mSkillKind;
	sSKILL_CREATE_INFO mSkillObjectInfo;
	SKILL_STATE	mState;
	VECTOR3	m_SkillDir;
	static float mAllowRangeForWideSkill;

// --- skr 16012020
  BOOL RelifeAllow;

public:
	cSkillObject(const SKILL_INFO&);
	virtual ~cSkillObject();
	DWORD GetSkillIdx() const { return mSkillIndex; }
	SKILLKIND GetSkillKind() const { return mSkillKind; }
	LEVELTYPE GetSkillLevel() const { return mSkillLevel; }
	CObject* GetOperator();
	void GetPosition(VECTOR3* pPos) { *pPos = mSkillObjectInfo.pos; }
	virtual void Init(sSKILL_CREATE_INFO*);
	virtual SKILL_STATE	Update() { return mState; }
	virtual void EndState();
	virtual DWORD SetRemoveMsg(DWORD dwReceiverID, MSGBASE*&);
	virtual void SetMultiTarget(CTargetList*) = 0;
	SKILL_STATE	GetSkillState() const { return mState; }
	virtual	void SetEndState() { mState = SKILL_STATE_END; }
	static void SetAllowRangeForWideSkill( float range ) { cSkillObject::mAllowRangeForWideSkill = range; }
	static float GetAllowRangeForWideSkill() { return cSkillObject::mAllowRangeForWideSkill; }
	const VECTOR3& GetSkillDir() const { return m_SkillDir; }
	DWORD	GetSkillObjectId() { return mSkillObjectInfo.skillObjectId; }

// --- skr 16012020
  virtual BOOL isRelifeAllow(){ return RelifeAllow; }
  virtual void setRelifeAllow( BOOL aboll ){ RelifeAllow = aboll; }

};

#endif // __SKILLOBJECT_H__INCLUDED
