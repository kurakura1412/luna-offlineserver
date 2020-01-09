/*********************************************************************

	 ÆÄÀÏ		: SHMain.cpp
	 ÀÛ¼ºÀÚ		: hseos
	 ÀÛ¼ºÀÏ		: 2007/04/12

	 ÆÄÀÏ¼³¸í	: ¸ÞÀÎ Å¬·¡½ºÀÇ ¼Ò½º

 *********************************************************************/

#include "stdafx.h"

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
//#include "[CC]Header/ServerGameStruct.h"
//#include "[CC]Header/CommonGameFunc.h"
//#include "[CC]Header/GameResourceStruct.h"
//#include "[CC]Header/CommonCalcFunc.h"
//#include "[CC]ServerModule/DataBase.h"
//#include "[CC]ServerModule/Console.h"
//#include "Object.h"
//#include "ServerSystem.h"
#endif

#include "SHMain.h"
#include "Common/SHMath.h"
#include "Debug/SHDebug.h"
#include "Farm/SHFarmManager.h"
#include "Date/SHDateManager.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSHMain
//

// -------------------------------------------------------------------------------------------------------------------------------------
// CSHMain Method																												  »ý¼ºÀÚ
//
CSHMain::CSHMain()
{
}

// -------------------------------------------------------------------------------------------------------------------------------------
// ~CSHMain Method																												  ÆÄ±«ÀÚ
//
CSHMain::~CSHMain()
{
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Init Method																													  ÃÊ±âÈ­
//
VOID CSHMain::Init()
{
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Main Method																													    ¸ÞÀÎ
//
VOID CSHMain::MainLoop()
{
#if defined(_DEBUG)
	CSHDebug::MainLoop();
#endif
	g_csFarmManager.MainLoop();
	g_csDateManager.MainLoop();
}

// -------------------------------------------------------------------------------------------------------------------------------------
// Render Method																												  ±×¸®±â
//
VOID CSHMain::Render()
{
#if defined(_DEBUG)
	CSHDebug::Render();
	CSHDebug::RenderErrorCode();
#endif
}
