#include "stdafx.h"
#include "CMap.h"

CMap::CMap()
{
	m_iMapWidth = dfMAP_WIDTH;
	m_iMapHeight = dfMAP_HEIGHT;

	m_iDrawPosX = 0;
	m_iDrawPosY = 0;

	memset(byTileMap, 0, sizeof(byTileMap));
}

CMap::CMap(int iMapWidth = 6400, int iMapHeight = 6400)
{
	m_iMapWidth = iMapWidth;
	m_iMapHeight = iMapHeight;

	memset(byTileMap, 0, sizeof(byTileMap));
}

CMap::~CMap()
{

}

/*------------------------------------------------------------------------------------------------*/
// Ÿ�� �׸���
/*------------------------------------------------------------------------------------------------*/
void CMap::Draw(CSpriteDib *pSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	int iDrawX = m_iDrawPosX;
	int iDrawY = m_iDrawPosY;

	//----------------------------------------------------------------------------------------------
	// Ŭ����
	//----------------------------------------------------------------------------------------------
	if (m_iDrawPosX < 0)	iDrawX = 0;
	if (m_iDrawPosY < 0)	iDrawY = 0;
	if (m_iDrawPosX + dfSCREEN_WIDTH > dfMAP_WIDTH)
		iDrawX = dfMAP_WIDTH - dfSCREEN_WIDTH;
	if (m_iDrawPosY + dfSCREEN_HEIGHT > dfMAP_HEIGHT)
		iDrawY = dfMAP_HEIGHT - dfSCREEN_HEIGHT;

	//----------------------------------------------------------------------------------------------
	// �׸���
	//----------------------------------------------------------------------------------------------
	for (int iCntY = 0; iCntY < 9; iCntY++)
	{
		for (int iCntX = 0; iCntX < 11; iCntX++)
		{
			pSprite->DrawImage(eMAP, (iCntX * 64) - (iDrawX % 64), iCntY * 64 - (iDrawY % 64),
				bypDest, iDestWidth, iDestHeight, iDestPitch);
		}
	}
}

/*------------------------------------------------------------------------------------------------*/
// ScrollX, Y ����
/*------------------------------------------------------------------------------------------------*/
void CMap::SetDrawPos(int iDrawPosX, int iDrawPosY)
{
	m_iDrawPosX = iDrawPosX - dfSCREEN_WIDTH / 2;
	m_iDrawPosY = iDrawPosY - dfSCREEN_HEIGHT / 2 - 20;
}