#include "stdafx.h"
#include "BaseObject.h"

/////////////////////////////////////////////////////////////////////////
// 생성자, 파괴자
/////////////////////////////////////////////////////////////////////////
CBaseObject::CBaseObject()
{

}

CBaseObject::~CBaseObject()
{

}

void CBaseObject::ActionInput()
{

}

void CBaseObject::NextFrame()
{

}

BOOL CBaseObject::isEndFrame()
{

}

/////////////////////////////////////////////////////////////////////////
// Getter
/////////////////////////////////////////////////////////////////////////
int CBaseObject::GetCurX()			{ return m_iCurX; }
int CBaseObject::GetCurY()			{ return m_iCurY; }
int CBaseObject::GetObjectID()		{ return m_iObjectID; }
int CBaseObject::GetObjectType()	{ return m_iObjectType; }
int CBaseObject::GetOldX()			{ return m_iOldX; }
int CBaseObject::GetOldY()			{ return m_iOldY; }
int CBaseObject::GetSprite()		{ return m_iSpriteNow; }

/////////////////////////////////////////////////////////////////////////
// Setter
/////////////////////////////////////////////////////////////////////////
void CBaseObject::SetCurPosition(int curX, int curY)
{
	m_iCurX = curX;
	m_iCurY = curY;
}

void CBaseObject::SetObjectID(int objectID)
{
	m_iObjectID = objectID;
}

void CBaseObject::SetObjectType(e_OBJECT_TYPE ObjectType)
{
	m_iObjectType = ObjectType;
}

void CBaseObject::SetOldPosition(int oldX, int oldY)
{
	m_iOldX = oldX;
	m_iOldY = oldY;
}

void CBaseObject::SetPosition(int x, int y)
{
	SetOldPosition(m_iCurX, m_iCurY);
	SetCurPosition(x, y);
}

void CBaseObject::SetSprite(e_SPRITE sStart, e_SPRITE sEnd)
{
	m_iSpriteStart = sStart;
	m_iSpriteNow = m_iSpriteStart;
	m_iSpriteEnd = sEnd;
}