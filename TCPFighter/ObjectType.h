#ifndef __OBJECTTYPE__H__
#define __OBJECTTYPE__H__

#define dfACTION_MOVE_LL 0
#define dfACTION_MOVE_LU 1
#define dfACTION_MOVE_UU 2
#define dfACTION_MOVE_RU 3
#define dfACTION_MOVE_RR 4
#define dfACTION_MOVE_RD 5
#define dfACTION_MOVE_DD 6
#define dfACTION_MOVE_LD 7

#define dfACTION_ATTACK1
#define dfACTION_ATTACK2
#define dfACTION_ATTACK3

#define dfACTION_STAND

enum e_OBJECT_TYPE
{
	eTYPE_PLAYER,
	eTYPE_EFFECT
};

#endif