#ifndef __PLAYEROBJECT__H__
#define __PLAYEROBJECT__H__

class CPlayerObject : public CBaseObject
{
public :
	CPlayerObject();
	~CPlayerObject();

	void Action();
	void ActionProc();
	void Draw();
	void InputActionProc();
	BOOL isPlayer();

	int GetDirection();

	void SetActionAttack1();
	void SetActionAttack2();
	void SetActionAttack3();
	void SetActionMove();
	void SetActionStand();
	void SetDirection();
	void SetHP();

private :
	BOOL m_bPlayerCharacter;
	char m_chHP;
	DWORD m_dwActionCur;
	DWORD m_dwActionOld;
	int m_iDirCur;
	int m_iDirOld;
};

#endif