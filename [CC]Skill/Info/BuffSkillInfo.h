#ifndef __BUFFSKILLINFO_H__INCLUDED
#define __BUFFSKILLINFO_H__INCLUDED
#pragma once
#include "SkillInfo.h"

class cBuffSkillInfo : public cSkillInfo
{
	BUFF_SKILL_INFO mInfo;
	// 081203 LUJ, ��ũ��Ʈ�� ��ġ ���ʽ��� �����Ѵ�
	void SetStatus( CObject&, Status& status, const SkillScript::Buff&, SetSkillStatusType ) const;
	// 081203 LUJ, ��ũ��Ʈ�� ���� ���¸� �����Ѵ�
	void SetBuff( CObject&, const SkillScript&, SetSkillStatusType ) const;

public:
	cBuffSkillInfo(void);
	virtual ~cBuffSkillInfo(void);
	void Parse(LPCTSTR);
	virtual cSkillObject* GetSkillObject() const;
	const BUFF_SKILL_INFO& GetInfo() const { return mInfo; }
	void AddPassiveStatus( CPlayer* pPlayer ) const;
	void RemovePassiveStatus( CPlayer* pPlayer ) const;
	void AddBuffStatus( CObject* pTarget ) const;
	void RemoveBuffStatus( CObject* pTarget ) const;
	virtual SKILLKIND GetKind() const { return mInfo.Kind; }
	virtual DWORD GetIndex() const { return mInfo.Index; }
	virtual LEVELTYPE GetLevel() const { return mInfo.Level; }
// --- skr 12/01/2020
	void SetDelay( DWORD anum);
};
#endif // __BUFFSKILLINFO_H__INCLUDED
