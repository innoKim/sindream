#include "stdafx.h"
#include "cGroup.h"

cGroup::cGroup() : m_pMtlTex(NULL)
{
}

cGroup::~cGroup()
{
	SAFE_RELEASE(m_pMtlTex);
}
