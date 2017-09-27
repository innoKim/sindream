#include "stdafx.h"
#include "cEffectLoader.h"
#include "cParticleGroup.h"

cEffectLoader::cEffectLoader()
{
}


cEffectLoader::~cEffectLoader()
{
}

void cEffectLoader::LoadEffect(IN string filePath, OUT vector<cParticleGroup*>& out)
{
}

void cEffectLoader::SaveEffect(IN string filePath, IN vector<cParticleGroup*>& in)
{
	HANDLE file;
	DWORD write;

	file = CreateFile(filePath.c_str(), GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	int vectorSize = in.size();
	WriteFile(file,&vectorSize, sizeof(vectorSize), &write, NULL);
	
	for (int i = 0; i < vectorSize; i++)
	{
		WriteFile(file, &in[i]->GetTexturePath(), sizeof(in[i]->GetTexturePath()), &write, NULL);
		WriteFile(file, &in[i]->GetInitParticleNumber(), sizeof(in[i]->GetInitParticleNumber()), &write, NULL);
		WriteFile(file, &in[i]->GetLifeTime(), sizeof(in[i]->GetLifeTime()), &write, NULL);
		WriteFile(file, &in[i]->GetLifeTimeVariation(), sizeof(in[i]->GetLifeTimeVariation()), &write, NULL);
		WriteFile(file, &in[i]->GetStartPosition(), sizeof(in[i]->GetStartPosition()), &write, NULL);
		WriteFile(file, &in[i]->GetStartPositionVariation(), sizeof(in[i]->GetStartPositionVariation()), &write, NULL);
		WriteFile(file, &in[i]->GetVelocity(), sizeof(in[i]->GetVelocity()), &write, NULL);
		WriteFile(file, &in[i]->GetVelocityVariation(), sizeof(in[i]->GetVelocityVariation()), &write, NULL);
		WriteFile(file, &in[i]->GetDragVelocity(), sizeof(in[i]->GetDragVelocity()), &write, NULL);
		WriteFile(file, &in[i]->GetAcceleration(), sizeof(in[i]->GetAcceleration()), &write, NULL);
		WriteFile(file, &in[i]->GetAccelerationVariation(), sizeof(in[i]->GetAccelerationVariation()), &write, NULL);
		WriteFile(file, &in[i]->GetStartColor(), sizeof(in[i]->GetStartColor()), &write, NULL);
		WriteFile(file, &in[i]->GetStartColorVariation(), sizeof(in[i]->GetStartColorVariation()), &write, NULL);
		WriteFile(file, &in[i]->GetEndColor(), sizeof(in[i]->GetEndColor()), &write, NULL);
		WriteFile(file, &in[i]->GetEndColorVariation(), sizeof(in[i]->GetEndColorVariation()), &write, NULL);
	}

	CloseHandle(file);
}
