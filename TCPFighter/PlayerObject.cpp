#include "stdafx.h"
#include "BaseObject.h"
#include "PlayerObject.h"

CPlayerObject::CPlayerObject(BOOL PlayerCharacter)
	: m_bPlayerCharacter(PlayerCharacter), m_chHP(100), m_dwActionCur(dfACTION_STAND),
	m_dwActionOld(dfACTION_STAND), m_iDirCur(0), m_iDirOld (0)
{

}

CPlayerObject::~CPlayerObject()
{

}

DWORD CPlayerObject::Action(DWORD dwParam)
{
	NextFrame();
	ActionProc();
	return FALSE;
}

void CPlayerObject::ActionProc()
{
	switch (m_dwActionCur){
	case dfACTION_ATTACK1 :
		break;

	case dfACTION_ATTACK2 :
		break;

	case dfACTION_ATTACK3 :
		if (isEndFrame())
		{
			SetActionStand();

			m_dwActionCur = dfACTION_STAND;
		}
		break;
	default :
		InputActionProc();
		break;
	}
}

void CPlayerObject::Draw(CSpriteDib *pSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	pSprite->DrawSprite(GetSprite(), GetCurX(), GetCurY(), bypDest, iDestWidth,
		iDestHeight, iDestPitch);
}

void CPlayerObject::InputActionProc()
{
	switch (m_dwActionCur)
	{
	case dfACTION_MOVE_DD :
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX(), GetCurY() + 1);
		break;

	case dfACTION_MOVE_LL :
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() - 1, GetCurY());
		break;

	case dfACTION_MOVE_RR :
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() + 1, GetCurY());
		break;

	case dfACTION_MOVE_UU :
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX(), GetCurY() - 1);
		break;

	case dfACTION_MOVE_LD :
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() - 1, GetCurY() + 1);
		break;

	case dfACTION_MOVE_LU :
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() - 1, GetCurY() - 1);
		break;

	case dfACTION_MOVE_RD :
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() + 1, GetCurY() + 1);
		break;

	case dfACTION_MOVE_RU :
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() + 1, GetCurY() - 1);
		break;
	}
}

BOOL CPlayerObject::isPlayer()
{
	if (m_bPlayerCharacter == TRUE)	return true;
	else							return false;
}

int CPlayerObject::GetDirection()
{
	return false;
}

void CPlayerObject::SetActionAttack1()	{}
void CPlayerObject::SetActionAttack2()	{}
void CPlayerObject::SetActionAttack3()	{}
void CPlayerObject::SetActionMove()		{}
void CPlayerObject::SetActionStand()	{}
void CPlayerObject::SetDirection()		{}
void CPlayerObject::SetHP()				{}