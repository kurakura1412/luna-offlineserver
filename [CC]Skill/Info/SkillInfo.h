#ifndef __SKILLINFO_H__INCLUDED
#define __SKILLINFO_H__INCLUDED
#pragma once

class cSkillObject;

class cSkillInfo
{
public:
	enum Type
	{
		TypeNone,
		TypeActive,
		TypeBuff,
	};
private:
	Type mType;
// --- skr 16012020
protected:
  BOOL RelifeAllow;

public:
	Type GetType() const { return mType; }
// --- skr 16012020
  BOOL isAllowByRelife(){ return RelifeAllow; }
  void SetAllowByRelife(BOOL aboll){ RelifeAllow = aboll;}
public:
// --- skr 16012020
	//cSkillInfo(Type type) : mType(type) {}
  cSkillInfo(Type type) : mType(type),RelifeAllow(FALSE) {}
	virtual ~cSkillInfo(void) {}
	virtual cSkillObject* GetSkillObject() const = 0;
	virtual SKILLKIND GetKind() const = 0;
	virtual DWORD GetIndex() const = 0;
	virtual LEVELTYPE GetLevel() const = 0;
};
#endif // __SKILLINFO_H__INCLUDED
