#ifndef __SHPROGRESSBAR_H__INCLUDED
#define __SHPROGRESSBAR_H__INCLUDED
/*********************************************************************

	 파일		: SHProgressBar.h
	 작성자		: hseos
	 작성일		: 2007/09/20

	 파일설명	: 게이지 클래스의 헤더

 *********************************************************************/

#pragma once

#include "cImage.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHProgressBar
{
public:
	//----------------------------------------------------------------------------------------------------------------
	enum BARTYPE
	{
		BARTYPE_01 = 0,
		BARTYPE_FARM_REPLANT,
		BARTYPE_FARM_REMANURE,
		BARTYPE_FARM_ANIMAL_REINSTALL,
		BARTYPE_FARM_ANIMAL_REFEED,
		BARTYPE_FARM_ANIMAL_RECLEANING,
	};

private:
	//----------------------------------------------------------------------------------------------------------------
	cImage		m_csImage;
	cImage		m_csBgImage;

	int			m_nImgNum;
	int			m_nWidth;

	DWORD		m_nShowTime;
	DWORD		m_nShowTimeTick;
	DWORD		m_nDeltaTime;

	int			m_nBarType;
	
public:
	//----------------------------------------------------------------------------------------------------------------
	CSHProgressBar();
	~CSHProgressBar();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 공용
	//
	//----------------------------------------------------------------------------------------------------------------


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 서버					
	//
	//----------------------------------------------------------------------------------------------------------------
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 클라이언트
	//
	BOOL		CLI_IsStarted()		{ return m_nShowTime; }
	VOID		CLI_End()			{ m_nShowTime = NULL; }

	VOID		CLI_Start(int nBarType, int nWidth, DWORD nShowTime, DWORD nDeltaTime = 0);
	VOID		CLI_Render(VECTOR2* pPos);
};
#endif // __SHPROGRESSBAR_H__INCLUDED
