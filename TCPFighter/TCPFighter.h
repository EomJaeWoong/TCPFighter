#pragma once

#include "resource.h"

struct GreaterY
{
	bool operator() (CBaseObject *left, CBaseObject *right) const
	{
		return left->GetCurY() < right->GetCurY();
	}
};

