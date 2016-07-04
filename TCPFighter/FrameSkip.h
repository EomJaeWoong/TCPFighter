#ifndef __FRAMESKIP__H__
#define __FRAMESKIP__H__

class CFrameSkip
{
public :
	CFrameSkip();
	~CFrameSkip();
	void FrameSkip();

private :
	int m_dwSystemTick;
	int m_iMaxFPS;
	int m_iOneFrameTick;
	int i_iTick;
};

#endif