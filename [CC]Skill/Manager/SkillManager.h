#ifndef __SKILLMANAGER_H__INCLUDED
#define __SKILLMANAGER_H__INCLUDED
#pragma once


#include "Object/SkillObject.h"


#define SKILLMGR	cSkillManager::GetInstance()


class cSkillInfo;
class cActiveSkillInfo;
class cBuffSkillInfo;

class cSkillManager
{
	CYHHashTable<cSkillInfo>	m_SkillInfoTable;
	CYHHashTable<cSkillObject>	m_SkillObjectTable;
	CYHHashTable<SKILL_TREE_INFO>	m_SkillTreeTable;
	std::list< DWORD > mProcessList;

	cSkillManager(void);
public:
	virtual ~cSkillManager(void);
	static cSkillManager* GetInstance();

	/// �ʱ�ȭ/����
	void Init();
	void Release();

	/// ��ų����Ʈ �ε�
	void LoadSkillInfoList();
	cSkillInfo* GetSkillInfo(DWORD skillIndex);
	const cActiveSkillInfo* GetActiveInfo(DWORD skillIndex);
	const cBuffSkillInfo* GetBuffInfo(DWORD skillIndex);
// --- skr 20012020
  cBuffSkillInfo* GetBuffInfoEX(DWORD skillIndex);
  
	cSkillObject* GetSkillObject(DWORD SkillObjID);

	/// ��ų ����
	void MonsterAttack(DWORD SkillIdx,CObject* pMonster,CObject* pTarget);
	BOOL PetAttack(DWORD SkillIdx,CObject* pPet,CObject* pTarget);
	SKILL_RESULT OnSkillStartSyn(MSG_SKILL_START_SYN* pmsg, ITEMBASE* pItemBase = NULL);
	SKILL_RESULT OnPetSkillStartSyn(MSG_SKILL_START_SYN* pmsg, ITEMBASE* pItemBase = NULL);
	cSkillObject* OnBuffSkillStart( cSkillInfo* pSkillInfo, sSKILL_CREATE_INFO* pCreateInfo );
	// 090204 LUJ, count�� ��ġ ������ Ȯ����
	void BuffSkillStart( DWORD characteridx, DWORD skillidx, DWORD remaintime, int count, VECTOR3* pDir=NULL );
	void AddPassive( CPlayer* pTarget, DWORD SkillIdx );
	void RemovePassive( CPlayer* pTarget, DWORD SkillIdx );

	void OnSkillTargetUpdate( MSG_SKILL_UPDATE_TARGET* pmsg );
	void OnSkillCancel( MSG_DWORD* pmsg );
	void ReleaseSkillObject(cSkillObject*);
	void Process();
	
	/// ��Ʈ��ũ �޼��� ����
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	DWORD GetNewSkillObjectID();
	DWORD GetSkillSize(DWORD skillIndex) const;
	void AddBuffSkill(CObject&, const ACTIVE_SKILL_INFO&);
	void AddActiveSkill(CObject&, DWORD skillIndex);
	BOOL IsLearnableSkill(const CHARACTER_TOTALINFO&, const SKILL_BASE&);
	SKILL_RESULT CreateActiveSkill( DWORD dwSkillidx, DWORD dwOperatorID, DWORD dwTargetID, const VECTOR3& dir );

private:
	typedef std::map< DWORD, DWORD >	SkillSize;
	SkillSize							mSkillSize;

	// 090701 ONS skillscript.bin���� switch_button�׸��� TRUE�� ������ ������ų�� ������Ų��
	void RemoveGroupBuffSkill(CObject*, const ACTIVE_SKILL_INFO&);
	void Recall(DWORD recallPlayerIndex, DWORD targetPlayerIndex, LPCTSTR targetPlayerName, MAPTYPE targetMap, DWORD skillIndex);
	BOOL IsInvalidRecallTarget(CPlayer&, DWORD targetPlayerIndex, const ACTIVE_SKILL_INFO&);
#ifdef _MYLUNA_
public:
	void ReleaseME();
	void InitME();

#endif
};

#endif // __SKILLMANAGER_H__INCLUDED
