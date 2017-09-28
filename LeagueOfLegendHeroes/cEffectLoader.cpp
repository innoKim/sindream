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
	HANDLE file;
	DWORD read;

	file = CreateFile(filePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (file == NULL)
	{
		CloseHandle(file);
		return;
	}

	int vectorSize;
	ReadFile(file, &vectorSize, sizeof(int), &read, NULL);

	out.resize(vectorSize);
	for (int i = 0; i < vectorSize; i++)
	{
		out[i] = new cParticleGroup;
		string path;

		int stringSize;
		ReadFile(file, &stringSize, sizeof(int), &read, NULL);
		for (int i = 0; i < stringSize; i++)
		{
			char c;
			ReadFile(file, &c, sizeof(char), &read, NULL);
			path += c;
		}
		out[i]->SetTexturePath(path);

		int initParticleNumber;
		ReadFile(file, &initParticleNumber, sizeof(int), &read, NULL);
		out[i]->SetInitParticleNumber(initParticleNumber);

		float lifeTime;
		ReadFile(file, &lifeTime, sizeof(float), &read, NULL);
		out[i]->SetLifeTime(lifeTime);

		float lifeTimeVariation;
		ReadFile(file, &lifeTimeVariation, sizeof(float), &read, NULL);
		out[i]->SetLifeTimeVariation(lifeTimeVariation);
		
		D3DXVECTOR3 startPos;
		ReadFile(file, &startPos, sizeof(D3DXVECTOR3), &read, NULL);
		out[i]->SetStartPosition(startPos);

		float startPosVariation;
		ReadFile(file, &startPosVariation, sizeof(float), &read, NULL);
		out[i]->SetStartPositionVariation(startPosVariation);

		D3DXVECTOR3 velocity;
		ReadFile(file, &velocity, sizeof(D3DXVECTOR3), &read, NULL);
		out[i]->SetVelocity(velocity);

		float velocityVaritaion;
		ReadFile(file, &velocityVaritaion, sizeof(float), &read, NULL);
		out[i]->SetVelocityVariation(velocityVaritaion);

		float dragVelocity;
		ReadFile(file, &dragVelocity, sizeof(float), &read, NULL);
		out[i]->SetDragVelocity(dragVelocity);

		D3DXVECTOR3 acceleration;
		ReadFile(file, &acceleration, sizeof(D3DXVECTOR3), &read, NULL);
		out[i]->SetAcceleration(acceleration);

		float accelerationVaritaion;
		ReadFile(file, &accelerationVaritaion, sizeof(float), &read, NULL);
		out[i]->SetAccelerationVariation(accelerationVaritaion);

		D3DXCOLOR startColor;
		ReadFile(file, &startColor, sizeof(D3DXCOLOR), &read, NULL);
		out[i]->SetStartColor(startColor);

		D3DXCOLOR startColorVariation;
		ReadFile(file, &startColorVariation, sizeof(D3DXCOLOR), &read, NULL);
		out[i]->SetStartColorVariation(startColorVariation);
		
		D3DXCOLOR endColor;
		ReadFile(file, &endColor, sizeof(D3DXCOLOR), &read, NULL);
		out[i]->SetEndColor(endColor);
		
		D3DXCOLOR endColorVariation;
		ReadFile(file, &endColorVariation, sizeof(D3DXCOLOR), &read, NULL);
		out[i]->SetEndColorVariation(endColorVariation);
	}

	CloseHandle(file);
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
		string path = in[i]->GetTexturePath();
		int stringSize = path.size();
		WriteFile(file, &stringSize, sizeof(stringSize), &write, NULL);
		for (int i = 0; i < stringSize; i++)
		{
			WriteFile(file, &path[i], sizeof(char), &write, NULL);
		}
		WriteFile(file, &in[i]->GetInitParticleNumber(), sizeof(int), &write, NULL);
		WriteFile(file, &in[i]->GetLifeTime(), sizeof(float), &write, NULL);
		WriteFile(file, &in[i]->GetLifeTimeVariation(), sizeof(float), &write, NULL);
		WriteFile(file, &in[i]->GetStartPosition(), sizeof(D3DXVECTOR3), &write, NULL);
		WriteFile(file, &in[i]->GetStartPositionVariation(), sizeof(float), &write, NULL);
		WriteFile(file, &in[i]->GetVelocity(), sizeof(D3DXVECTOR3), &write, NULL);
		WriteFile(file, &in[i]->GetVelocityVariation(), sizeof(float), &write, NULL);
		WriteFile(file, &in[i]->GetDragVelocity(), sizeof(float), &write, NULL);
		WriteFile(file, &in[i]->GetAcceleration(), sizeof(D3DXVECTOR3), &write, NULL);
		WriteFile(file, &in[i]->GetAccelerationVariation(), sizeof(float), &write, NULL);
		WriteFile(file, &in[i]->GetStartColor(), sizeof(D3DXCOLOR), &write, NULL);
		WriteFile(file, &in[i]->GetStartColorVariation(), sizeof(D3DXCOLOR), &write, NULL);
		WriteFile(file, &in[i]->GetEndColor(), sizeof(in[i]->GetEndColor()), &write, NULL);
		WriteFile(file, &in[i]->GetEndColorVariation(), sizeof(D3DXCOLOR), &write, NULL);
	}

	CloseHandle(file);
}
