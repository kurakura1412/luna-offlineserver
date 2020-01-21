// ReLife feature that allow player invincible for period of time after revival.
// enable them to counter enemies attacks.

#include "stdafx.h"
#include <vector>
#include "[lib]yhlibrary/HashTable.h"
#include "protocol.h"
#include "CommonDefine.h"
#include "CommonGameDefine.h"
#include "ServerGameDefine.h"
#include "CommonStruct.h"
#include "ServerGameStruct.h"
#include "CommonCalcFunc.h"
//#include "CommonGameFunc.h"
#include "../[CC]Header/GameResourceManager.h"
#include "../[CC]Skill/Server/Info/BuffSkillInfo.h"
#include "../[CC]Skill/Server/Manager/SkillManager.h"

#include "MHFile.h"
#include "Relife.h"

CRelife::CRelife()
{
	dwmapnum = 0;
  relife_time_count = 0;
  relifebuffidx = 0;
  relife_timeup = 0;
  relife_buff_count = 0;
	relife_mode = TRUE; // default yes
  relife_icon =  FALSE; //
}
CRelife::~CRelife()
{}

void CRelife::LoadSetup()
{
	char tempBuf[256] = {0};
	DWORD mapnumme = 0;
	DWORD alloskillA = 0;
	DWORD alloskillF = 0;
	DWORD alloskillE = 0;
  DWORD allobuffA = 0;
	DWORD allobuffF = 0;
	DWORD allobuffE = 0;
	CMHFile file;
	file.Init("System/Resource/Relife.txt", "r");
	if(file.IsInited() == FALSE)
	{
		//MessageBox(0, "System/Resource/Relife.txt is not found!", 0, MB_OK);
		return;
	}
	Allowed_skill.reserve(200);
	Allowed_skill_range.reserve(200);
  Allowed_buff.reserve(200);
  Allowed_buff_range.reserve(200);
	while(1)
	{
		file.GetString(tempBuf);
		if(file.IsEOF())
			break;
		if(tempBuf[0] == '@')
		{
			file.GetLineX(tempBuf, 256);
			continue;
		}
		else if(0==strcmp(tempBuf, "#RELIFE_TIME"))
		{
			relife_timeup = file.GetDword();
		}
		else if(0==strcmp(tempBuf, "#EXCLUDE_MAP"))
		{
			mapnumme = file.GetDword();
			if( mapnumme == dwmapnum ){
				relife_mode = FALSE;
				break;
			}
		}
		else if(0==strcmp(tempBuf, "#ALLOWED_SKILL_A"))
		{
			alloskillA = file.GetDword();
			Allowed_skill.push_back( alloskillA );
		}
		else if(0==strcmp(tempBuf, "#ALLOWED_SKILL_RANGE"))
		{
			alloskillF = file.GetDword();
			alloskillE = file.GetDword();
			AllSkillRange stuff;
			stuff.dwFirst = alloskillF;
			stuff.dwEnd = alloskillE;
			Allowed_skill_range.push_back(stuff);
		}
    else if(0==strcmp(tempBuf, "#RELIFE_BUFF_ID"))
		{
      DWORD sklidx = file.GetDword();
      cBuffSkillInfo* sklbuff = SKILLMGR->GetBuffInfoEX(sklidx);
      if( sklbuff ){
        sklbuff->SetDelay( relife_timeup );
        relife_icon = TRUE;
        relifebuffidx = sklidx;
      }
    }
    else if(0==strcmp(tempBuf, "#RELIFE_BUFF_COUNT"))
		{
      relife_buff_count = file.GetDword();
    }
    else if(0==strcmp(tempBuf, "#ALLOWED_BUFF_A"))
		{
			allobuffA = file.GetDword();
			Allowed_buff.push_back( alloskillA );
		}
		else if(0==strcmp(tempBuf, "#ALLOWED_BUFF_RANGE"))
		{
			allobuffF = file.GetDword();
			allobuffE = file.GetDword();
			AllSkillRange stuff;
			stuff.dwFirst = alloskillF;
			stuff.dwEnd = alloskillE;
			Allowed_buff_range.push_back(stuff);
    }
	}
	file.Release();

  Finalize();
}

BOOL CRelife::isRelifeMod()
{
	return relife_mode;
}

void CRelife::reset()
{
	relife_mode = TRUE;
	Allowed_skill.clear();
	Allowed_skill_range.clear();
  Allowed_buff.clear();
	Allowed_buff_range.clear();
}

BOOL CRelife::isAllowSkill(DWORD skillnum)
{
  BOOL ret = FALSE;
  DWORD ii, iii;
  AllSkillRange morestuff;
  DWORD ui = Allowed_skill.size();
  DWORD ai = Allowed_skill_range.size();
  for( ii = 0; ii < ui; ii++ )  {
    if( Allowed_skill[ii] == skillnum ){
      ret = TRUE;
      break;
    }
  }
  if( ret == FALSE ){
    for( iii = 0; iii < ai; iii++){
      if( skillnum >= Allowed_skill_range[iii].dwFirst &&
          skillnum <= Allowed_skill_range[iii].dwEnd ){
        ret = TRUE;
        break;
      }
    }
  }
  return ret;
}

void CRelife::setmapnum( WORD anum )
{
	dwmapnum = (DWORD)anum;
}
void CRelife::Init()
{
}
CRelife* CRelife::GetInstance()
{
  static CRelife instance;
	return &instance;
}
BOOL CRelife::isRelifeIcon()
{
  return relife_icon;
}
BOOL CRelife::isAllowBuff(DWORD buffidx)
{
  BOOL ret = FALSE;
  DWORD ii, iii;
  AllSkillRange morestuff;
  DWORD ui = Allowed_buff.size();
  DWORD ai = Allowed_buff_range.size();
  if( relife_icon && (relifebuffidx != 0) && (buffidx == relifebuffidx) ){
    ret = TRUE;
  }
  else {
    for( ii = 0; ii < ui; ii++ )  {
      if( Allowed_buff[ii] == buffidx ){
        ret = TRUE;
        break;
      }
    }
    if( ret == FALSE ){
      for( iii = 0; iii < ai; iii++){
        if( buffidx >= Allowed_buff_range[iii].dwFirst &&
            buffidx <= Allowed_buff_range[iii].dwEnd ){
          ret = TRUE;
          break;
        }
      }
    }
  }

  return ret;
}
void CRelife::SetSkillAllow(DWORD sklidx)
{
  if( sklidx != 0 ){
    cSkillInfo* skInfo = SKILLMGR->GetSkillInfo( sklidx );
    if( skInfo ){
      skInfo->SetAllowByRelife( TRUE );
    }
  }
}
void CRelife::Finalize()
{
  DWORD i,ii,iii,iv, ifirst, iend,xa,xb;
  DWORD sklsize = Allowed_skill.size();
  DWORD sklallsize = Allowed_skill_range.size();
  for( i = 0 ; i < sklsize; i++){
    SetSkillAllow( Allowed_skill.at(i));
  }
  for(ii=0; ii < sklallsize; ii++){
    xa = Allowed_skill_range.at(ii).dwEnd;
    xb = Allowed_skill_range.at(ii).dwFirst;
    iend = (xa > xb) ? xa : xb;
    ifirst = (xa < xb) ? xa : xb;
    iii = iend - ifirst;
    for(iv = 0; iv < iii; iv++ ){
      SetSkillAllow( ifirst + iv );
    }
  }
}
