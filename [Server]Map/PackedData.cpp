// PackedData.cpp: implementation of the CPackedData class.
//
//////////////////////////////////////////////////////////////////////

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
#include "[CC]Header/CommonGameFunc.h"
//#include "[CC]Header/GameResourceStruct.h"
//#include "[CC]Header/CommonCalcFunc.h"
//#include "[CC]ServerModule/DataBase.h"
//#include "[CC]ServerModule/Console.h"
#include "Object.h"
#include "ServerSystem.h"
#endif

#include "Network.h"
#include "ServerTable.h"
#include "Object.h"
#include "GridSystem.h"
#include "PackedData.h"

CPackedData::CPackedData()
{
	mData.Initialize( 10 );
}

CPackedData::~CPackedData()
{
	mData.SetPositionHead();
	
	while(Data* data = mData.GetData())
	{
		delete data;
	}
}

void CPackedData::Init( void* pData,WORD MsgLen )
{
	ZeroMemory( &m_SendData, sizeof( m_SendData ) );
	m_SendData.Category = MP_PACKEDDATA;
	m_SendData.Protocol = MP_PACKEDDATA_NORMAL;
	m_SendData.wRealDataSize = MsgLen;
	memcpy(m_SendData.Data,pData,MsgLen);

	mData.SetPositionHead();

	while(Data* const data = mData.GetData())
	{
		data->mIndexList.RemoveAll();
	}
}

void CPackedData::AddObject(CObject* pObject)
{
	Data* data = mData.GetData( pObject->GetAgentNum() );

	if( 0 == data )
	{
		data = new Data;
		data->mAgentNumber = pObject->GetAgentNum();
		mData.Add( data, pObject->GetAgentNum() );
	}

	data->mIndexList.AddHead( LPVOID( pObject->GetID() ) );
}

void CPackedData::Send()
{
	if(0 == m_SendData.wRealDataSize)
	{
		return;
	}

	mData.SetPositionHead();

	while(Data* data = mData.GetData())
	{
		if( data->mIndexList.IsEmpty() )
		{
			continue;
		}

		LPSTR indexPosition = &m_SendData.Data[ m_SendData.wRealDataSize ];
		PTRLISTPOS position = data->mIndexList.GetHeadPosition();

		while(const DWORD objectIndex = DWORD(data->mIndexList.GetNext(position)))
		{
			memcpy( indexPosition, &objectIndex, sizeof( objectIndex ) );
			indexPosition += sizeof( objectIndex );
		}

		const DWORD dataLength = sizeof(m_SendData) - sizeof(m_SendData.Data) + indexPosition - m_SendData.Data;
		m_SendData.wReceiverNum = WORD( data->mIndexList.GetCount() );
		g_Network.Send2Server( data->mAgentNumber, LPSTR( &m_SendData ), dataLength );
	}
}

void CPackedData::QuickSendExceptObjectSelf(CObject* pObject,MSGBASE* pMsg,int MsgLen)
{
	if(pObject->GetInited() == FALSE)
		return;

	Init( pMsg, WORD( MsgLen ) );
		
	g_pServerSystem->GetGridSystem()->AddGridToPackedDataWithoutOne(
		pObject->GetGridPosition(),	pObject->GetID(),PACKEDDATA_OBJ, pObject);
	Send();
}

void CPackedData::QuickSend(CObject* pObject,MSGBASE* pMsg,int MsgLen)
{
	if(pObject->GetInited() == FALSE)
		return;

/*
	//////////////////////////////////////////////////////////////////////////
	// test
	if(pMsg->Category == MP_SKILL && pMsg->Protocol == MP_SKILL_SKILL_RESULT )
	{
		FILE* fp = fopen("send.txt","wb");
		if(fp)
		{
			fwrite(pMsg,1,MsgLen,fp);
			char temp[64];
			sprintf(temp,"####%s",GetCurTimeToString());
			fwrite(temp,1,strlen(temp),fp);
			fclose(fp);
		}
	}
	//////////////////////////////////////////////////////////////////////////
//*/

	GridPosition* pGridPosition = pObject->GetGridPosition();
	if(pGridPosition->x == 65535 || pGridPosition->z == 65535)
	{
//		g_Console.Log(eLogDisplay,4,"Not Inited Object Send %s",pObject->GetObjectName());
		return;
	}

	Init( pMsg, WORD( MsgLen ) );

	g_pServerSystem->GetGridSystem()->AddGridToPackedDataWithoutOne(
		pGridPosition, 0,PACKEDDATA_OBJ, pObject);	
	Send();
}

void CPackedData::SendToMapServer(MAPTYPE ToMapNum, MSGBASE* pMsg, WORD MsgLen) const
{
	static SEND_PACKED_TOMAPSERVER_DATA msg;
	ZeroMemory(
		&msg,
		sizeof(msg));
	msg.Category = MP_PACKEDDATA;
	msg.Protocol = MP_PACKEDDATA_TOMAPSERVER;
	msg.ToMapNum = ToMapNum;
	msg.wRealDataSize = MsgLen;
	memcpy(msg.Data,pMsg,MsgLen);

	g_Network.Send2AgentServer(
		(char*)&msg,
		msg.GetMsgLength());
}

void CPackedData::SendToBroadCastMapServer(MSGBASE* pMsg, WORD MsgLen) const
{
	static SEND_PACKED_TOMAPSERVER_DATA msg;
	ZeroMemory(
		&msg,
		sizeof(msg));
	msg.Category = MP_PACKEDDATA;
	msg.Protocol = MP_PACKEDDATA_TOBROADMAPSERVER;
	msg.ToMapNum = 0;
	msg.wRealDataSize = MsgLen;
	memcpy(msg.Data,pMsg,MsgLen);
	
	g_Network.Send2AgentServer(
		(char*)&msg,
		msg.GetMsgLength());
}