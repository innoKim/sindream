#include "stdafx.h"
#include "cMainGame.h"

cMainGame::cMainGame()
{
	g_pKeyManager->Setup();
}

cMainGame::~cMainGame()
{
	g_pKeyManager->Destroy();
}

void cMainGame::Setup()
{
}

void cMainGame::Update()
{
}

void cMainGame::Render()
{
}
