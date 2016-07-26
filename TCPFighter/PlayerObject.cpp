#include "stdafx.h"
#include "BaseObject.h"
#include "PlayerObject.h"

CPlayerObject::CPlayerObject(BOOL PlayerCharacter, int iObjectID, int iObjectType, char chHP, int iDirection)
	: CBaseObject(iObjectID, iObjectType), m_bPlayerCharacter(PlayerCharacter), m_chHP(chHP), m_dwActionCur(dfACTION_STAND),
	m_dwActionOld(dfACTION_STAND), m_iDirCur(iDirection), m_iDirOld(iDirection)
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
	// 공격이 안끝났을 때
	if ((m_dwActionCur == dfACTION_ATTACK1 || m_dwActionCur == dfACTION_ATTACK2 || m_dwActionCur == dfACTION_ATTACK3)
		&& (!isEndFrame()))
	{
		m_dwActionInput = m_dwActionCur;
		return;
	}

	// 공격이 끝났을 때
	else if ((m_dwActionCur == dfACTION_ATTACK1 || m_dwActionCur == dfACTION_ATTACK2 || m_dwActionCur == dfACTION_ATTACK3)
		&& isEndFrame())
	{
		SetActionStand();
		m_dwActionInput = dfACTION_STAND;
	}

	switch (m_dwActionInput)
	{
	case dfACTION_ATTACK1 :
		SetActionAttack1();
		break;

	case dfACTION_ATTACK2 :
		SetActionAttack2();
		break;

	case dfACTION_ATTACK3 :
		SetActionAttack3();
		break;

	default :
		InputActionProc();
		break;
	}
}

void CPlayerObject::Draw(CSpriteDib *pSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	pSprite->DrawSprite50(eSHADOW, GetCurX(), GetCurY(), bypDest, iDestWidth,
		iDestHeight, iDestPitch);
	
	if (m_bPlayerCharacter)
		pSprite->DrawSpriteRed(GetSprite(), GetCurX(), GetCurY(), bypDest, iDestWidth,
			iDestHeight, iDestPitch);
	else
		pSprite->DrawSprite(GetSprite(), GetCurX(), GetCurY(), bypDest, iDestWidth,
		iDestHeight, iDestPitch);

	pSprite->DrawSprite(eGUAGE_HP, GetCurX() - 35, GetCurY() + 9, bypDest, iDestWidth,
		iDestHeight, iDestPitch, m_chHP);
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
		break;

	case dfACTION_MOVE_LL :
		SetPosition(GetCurX() - dfSPEED_PLAYER_X, GetCurY());
		SetActionMove(dfACTION_MOVE_LL);
		break;

	case dfACTION_MOVE_RR :
		SetPosition(GetCurX() + dfSPEED_PLAYER_X, GetCurY());
		SetActionMove(dfACTION_MOVE_RR);
		break;

	case dfACTION_MOVE_DD:
		SetPosition(GetCurX(), GetCurY() + dfSPEED_PLAYER_Y);
		SetActionMove(dfACTION_MOVE_DD);
		break;

	case dfACTION_MOVE_UU :
		SetPosition(GetCurX(), GetCurY() - dfSPEED_PLAYER_Y);
		SetActionMove(dfACTION_MOVE_UU);
		break;

	case dfACTION_MOVE_LD :
		SetPosition(GetCurX() - dfSPEED_PLAYER_X, GetCurY() + dfSPEED_PLAYER_Y);
		SetActionMove(dfACTION_MOVE_LD);
		break;

	case dfACTION_MOVE_LU :
		SetPosition(GetCurX() - dfSPEED_PLAYER_X, GetCurY() - dfSPEED_PLAYER_Y);
		SetActionMove(dfACTION_MOVE_LU);
		break;

	case dfACTION_MOVE_RD :
		SetPosition(GetCurX() + dfSPEED_PLAYER_X, GetCurY() + dfSPEED_PLAYER_Y);
		SetActionMove(dfACTION_MOVE_RD);
		break;

	case dfACTION_MOVE_RU :
		SetPosition(GetCurX() + dfSPEED_PLAYER_X, GetCurY() - dfSPEED_PLAYER_Y);
		SetActionMove(dfACTION_MOVE_RU);
		break;
	}

	if (GetCurX() < dfRANGE_MOVE_LEFT)		SetPosition(dfRANGE_MOVE_LEFT, GetCurY());
	if (GetCurX() > dfRANGE_MOVE_RIGHT)		SetPosition(dfRANGE_MOVE_RIGHT, GetCurY());
	if (GetCurY() < dfRANGE_MOVE_TOP)		SetPosition(GetCurX(), dfRANGE_MOVE_TOP);
	if (GetCurY() > dfRANGE_MOVE_BOTTOM)	SetPosition(GetCurX(), dfRANGE_MOVE_BOTTOM);
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

	if (m_dwActionOld != m_dwActionCur){
		if (GetDirection() == LEFT)
			SetSprite(ePLAYER_ATTACK1_L01, ePLAYER_ATTACK1_L_MAX, 3);
		else if (GetDirection() == RIGHT)
			SetSprite(ePLAYER_ATTACK1_R01, ePLAYER_ATTACK1_R_MAX, 3);
	}
}

void CPlayerObject::SetActionAttack2()	
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK2;

	if (m_dwActionOld != m_dwActionCur){
		if (GetDirection() == LEFT)
			SetSprite(ePLAYER_ATTACK2_L01, ePLAYER_ATTACK2_L_MAX, 4);
		else if (GetDirection() == RIGHT)
			SetSprite(ePLAYER_ATTACK2_R01, ePLAYER_ATTACK2_R_MAX, 4);
	}
}

void CPlayerObject::SetActionAttack3()	
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK3;
	if (m_dwActionOld != m_dwActionCur){
		if (GetDirection() == LEFT)
			SetSprite(ePLAYER_ATTACK3_L01, ePLAYER_ATTACK3_L_MAX, 4);
		else if (GetDirection() == RIGHT)
			SetSprite(ePLAYER_ATTACK3_R01, ePLAYER_ATTACK3_R_MAX, 4);
	}

}

/*---------------------------------------------------------------------------------------------*/
// Player Move Setting
/*---------------------------------------------------------------------------------------------*/
void CPlayerObject::SetActionMove(DWORD actionMove)		
{
	st_NETWORK_PACKET_HEADER Header;
	char Packet[100];

	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = actionMove;

	if (GetCurX() > GetOldX())			SetDirection(RIGHT);
	else if (GetCurX() < GetOldX())		SetDirection(LEFT);

	if (m_dwActionOld == dfACTION_STAND || GetDirection() != m_iDirOld)
	{
		if (m_iDirCur == RIGHT)			SetSprite(ePLAYER_MOVE_R01, ePLAYER_MOVE_R_MAX, 4);
		else if (m_iDirCur == LEFT) 	SetSprite(ePLAYER_MOVE_L01, ePLAYER_MOVE_L_MAX, 4);
	}

	if (m_dwActionCur != m_dwActionOld)
	{
		MakePacket_MoveStart(&Header, Packet, m_dwActionCur, GetCurX(), GetCurY());
		SendQ.Put((char *)&Header, sizeof(Header));
		SendQ.Put(Packet, Header.bySize);
	}
}

/*---------------------------------------------------------------------------------------------*/
// Player Stand Setting
/*---------------------------------------------------------------------------------------------*/
void CPlayerObject::SetActionStand()	
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_STAND;

	if (m_dwActionOld != dfACTION_STAND){
		if (m_iDirCur == LEFT)			SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L_MAX, 5);
		else if (m_iDirCur == RIGHT)	SetSprite(ePLAYER_STAND_R01, ePLAYER_STAND_R_MAX, 5);
	}
}

/*---------------------------------------------------------------------------------------------*/
// Player Direction Setting
/*---------------------------------------------------------------------------------------------*/
void CPlayerObject::SetDirection(int dir)		
{
	m_iDirOld = m_iDirCur;
	m_iDirCur = dir;
}

/*---------------------------------------------------------------------------------------------*/
// Player HP Setting
/*---------------------------------------------------------------------------------------------*/
void CPlayerObject::SetHP(char hp)				
{
	m_chHP = hp;
}