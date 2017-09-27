#include "stdafx.h"
#include "cEnemy.h"
#include "cAStar.h"

cEnemy::cEnemy()
{
}


cEnemy::~cEnemy()
{
	Destroy();
}

void cEnemy::Update()
{
	cUnit::Update();
}

void cEnemy::Render()
{
	cUnit::Render();
}
