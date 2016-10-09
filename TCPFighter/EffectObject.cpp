#include "stdafx.h"
#include "BaseObject.h"
#include "EffectObject.h"

CEffectObject::CEffectObject(int iObjectID, int iObjectType, int iX, int iY, 
	int iFrameDelay, int iSpriteStart, int iSpriteEnd)
	:CBaseObject(iObjectID, iObjectType)
{
	SetPosition(iX, iY);
	SetSprite(iSpriteStart, iSpriteEnd, iFrameDelay);
}

CEffectObject::~CEffectObject()
{

}

DWORD CEffectObject::Action(DWORD dwParam)
{
	m_dwAttackID = dwParam;

	NextFrame();
	if (isEndFrame())
		return TRUE;

	return FALSE;
}

void CEffectObject::Draw(CSpriteDib *pSprite, BYTE* bypDest, int iDestWidth
	, int iDestHeight, int iDestPitch)
{
	pSprite->DrawSprite(GetSprite(), GetCurX(), GetCurY(), bypDest
		, iDestWidth, iDestHeight, iDestPitch);
}