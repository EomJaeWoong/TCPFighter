#ifndef __EFFECTOBJECT__H__
#define __EFFECTOBJECT__H__

class CEffectObject : public CBaseObject
{
public :
	CEffectObject();
	~CEffectObject();

	void Action();
	void Draw();

private :
	BOOL m_bEffectStart;
	DWORD m_dwAttackID;
};

#endif