#include "stdafx.h"
#include "BaseObject.h"
#include "PlayerObject.h"

CPlayerObject::CPlayerObject(BOOL PlayerCharacter)
	: m_bPlayerCharacter(PlayerCharacter), m_chHP(100), m_dwActionCur(dfACTION_STAND),
	m_dwActionOld(dfACTION_STAND), m_iDirCur(RIGHT), m_iDirOld (RIGHT)
{
	SetSprite(ePLAYER_STAND_R01, ePLAYER_STAND_R_MAX, 5);
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
	switch (m_dwActionInput){
	case dfACTION_ATTACK1 :
	case dfACTION_ATTACK2 :
	case dfACTION_ATTACK3 :
		if (isEndFrame())
		{
			SetActionStand();
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

/*-----------------------------------------------------------------------------------------------------------
 입력 액션 처리
 액션 셋팅과 방향, 스프라이트를 설정한다.
-----------------------------------------------------------------------------------------------------------*/
void CPlayerObject::InputActionProc()
{
	switch (m_dwActionInput)
	{
	case dfACTION_STAND:
		SetActionStand();

		if (m_dwActionOld != m_dwActionCur){
			if (m_iDirCur == LEFT){
				SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L_MAX, 5);
				SetDirection(LEFT);
			}
			else if (m_iDirCur == RIGHT){
				SetSprite(ePLAYER_STAND_R01, ePLAYER_STAND_R_MAX, 5);
				SetDirection(RIGHT);
			}
		}
		break;

	case dfACTION_MOVE_DD :
		SetActionMove(dfACTION_MOVE_DD);
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX(), GetCurY() + 3);

		if (m_dwActionOld != m_dwActionCur){
			if (m_iDirOld == LEFT){
				SetSprite(ePLAYER_MOVE_L01, ePLAYER_MOVE_L_MAX, 4);
				SetDirection(LEFT);
			}
			else if (m_iDirOld == RIGHT){
				SetSprite(ePLAYER_MOVE_R01, ePLAYER_MOVE_R_MAX, 4);
				SetDirection(RIGHT);
			}
		}
		break;

	case dfACTION_MOVE_LL :
		SetActionMove(dfACTION_MOVE_LL);
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() - 3, GetCurY());

		if (m_dwActionOld != m_dwActionCur){
			SetSprite(ePLAYER_MOVE_L01, ePLAYER_MOVE_L_MAX, 4);
			SetDirection(LEFT);
		}
		break;

	case dfACTION_MOVE_RR :
		SetActionMove(dfACTION_MOVE_RR);
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() + 3, GetCurY());

		if (m_dwActionOld != m_dwActionCur){
			SetSprite(ePLAYER_MOVE_R01, ePLAYER_MOVE_R_MAX, 4);
			SetDirection(RIGHT);
		}
		break;

	case dfACTION_MOVE_UU :
		SetActionMove(dfACTION_MOVE_UU);
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX(), GetCurY() - 3);

		if (m_dwActionOld != m_dwActionCur){
			if (m_iDirOld == LEFT){
				SetSprite(ePLAYER_MOVE_L01, ePLAYER_MOVE_L_MAX, 4);
				SetDirection(LEFT);
			}
			else if (m_iDirOld == RIGHT){
				SetSprite(ePLAYER_MOVE_R01, ePLAYER_MOVE_R_MAX, 4);
				SetDirection(RIGHT);
			}
		}
		break;

	case dfACTION_MOVE_LD :
		SetActionMove(dfACTION_MOVE_LD);
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() - 3, GetCurY() + 3);

		if (m_dwActionOld != m_dwActionCur && m_dwActionOld == dfACTION_STAND){
			SetSprite(ePLAYER_MOVE_L01, ePLAYER_MOVE_L_MAX, 4);
			SetDirection(LEFT);
		}
		break;

	case dfACTION_MOVE_LU :
		SetActionMove(dfACTION_MOVE_LU);
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() - 3, GetCurY() - 3);

		if (m_dwActionOld != m_dwActionCur && m_dwActionOld == dfACTION_STAND){
			SetSprite(ePLAYER_MOVE_L01, ePLAYER_MOVE_L_MAX, 4);
			SetDirection(LEFT);
		}
		break;

	case dfACTION_MOVE_RD :
		SetActionMove(dfACTION_MOVE_RD);
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() + 3, GetCurY() + 3);

		if (m_dwActionOld != m_dwActionCur && m_dwActionOld == dfACTION_STAND){
			SetSprite(ePLAYER_MOVE_R01, ePLAYER_MOVE_R_MAX, 4);
			SetDirection(RIGHT);
		}
		break;

	case dfACTION_MOVE_RU :
		SetActionMove(dfACTION_MOVE_RU);
		SetOldPosition(GetCurX(), GetCurY());
		SetCurPosition(GetCurX() + 3, GetCurY() - 3);

		if (m_dwActionOld != m_dwActionCur && m_dwActionOld == dfACTION_STAND){
			SetSprite(ePLAYER_MOVE_R01, ePLAYER_MOVE_R_MAX, 4);
			SetDirection(RIGHT);
		}
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
	return m_iDirCur;
}

void CPlayerObject::SetActionAttack1()	
{ 
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK1;
}

void CPlayerObject::SetActionAttack2()	
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK2;
}

void CPlayerObject::SetActionAttack3()	
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK3;
}

void CPlayerObject::SetActionMove(DWORD actionMove)		
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = actionMove;
}

void CPlayerObject::SetActionStand()	
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_STAND;
}

void CPlayerObject::SetDirection(int dir)		
{
	m_iDirOld = m_iDirCur;
	m_iDirCur = dir;
}

void CPlayerObject::SetHP(char hp)				
{
	m_chHP = hp;
}