#include "stdafx.h"
#include "cParticleScene.h"
#include "cParticleGroup.h"
#include "cUIObject.h"
#include "cUIBar.h"

#include "cUnit.h"
#include "cPlayer.h"
#include "cUIButton.h"
#include "cUIText.h"
#include "cUIImage.h"
#include "cMap.h"

cParticleScene::cParticleScene() :
	target(0, 0, 0),
	m_pSprite(NULL),
	m_pPlayer(NULL),
	m_pMap(NULL),
	m_pCurParticleGroup(NULL),
	m_pCurValue(NULL),
	m_eValueType(eNone),
	m_eNone(eType::eNone),
	m_eFloat(eType::eFloat),
	m_eInt(eType::eInt),
	m_eVector3(eType::eVector3)
{
}

cParticleScene::~cParticleScene()
{
	SAFE_DELETE(m_pCurParticleGroup);

	for each(auto p in m_vecUIObject)
	{
		SAFE_DELETE(p);
	}

	SAFE_DELETE(m_pPlayer);

	SAFE_RELEASE(m_pSprite);

	SAFE_DELETE(m_pMap);
}

void cParticleScene::Setup()
{
	g_pShaderManager->SetTarget(&target);

	m_pCurParticleGroup = g_pEffectManager->NewEffect();

	//////////////////////////////////////////////////////

	/*m_pParticleGroup->SetInitParticleNumber(20);
	m_pParticleGroup->SetIsContinueus(false);
	m_pParticleGroup->SetGenParticleNumber(5);
	m_pParticleGroup->SetLifeTime(1.0f);
	m_pParticleGroup->SetLifeTimeVariation(0.5f);
	m_pParticleGroup->SetStartPosition(D3DXVECTOR3(0, 0, 0));
	m_pParticleGroup->SetStartPositionVariation(5.0f);
	m_pParticleGroup->SetVelocity(D3DXVECTOR3(0, 0.1f, 0));
	m_pParticleGroup->SetVelocityVariation(0.1f);
	m_pParticleGroup->SetAcceleration(D3DXVECTOR3(0, 0, 0));
	m_pParticleGroup->SetAccelerationVariation(0.01f);
	m_pParticleGroup->SetStartColor(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	m_pParticleGroup->SetStartColorVariation(D3DXCOLOR(0, 0, 0, 0.3f));
	m_pParticleGroup->SetEndColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.0f));
	m_pParticleGroup->SetDragVelocity(0.05f);
	m_pCurParticleGroup->Setup();*/
		
	////////////////////////////////////////////////////////

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pPlayer = new cPlayer;
	vector<ST_UNITLOADINFO> temp;
	temp.push_back({ STATE_IDLE, "unit/AlistarIdle.x" });
	temp.push_back({ STATE_RUN, "unit/AlistarRun.x" });
	temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x",ST_CallbackInfo(0.0f,AlistarSpell1CallBack,m_pPlayer) });
	temp.push_back({ STATE_SPELL2, "unit/AlistarSpell2.x",ST_CallbackInfo(0.0f,AlistarSpell2CallBack,m_pPlayer) });
	m_pPlayer->Setup(temp,NULL);
	m_pPlayer->SetPosition(D3DXVECTOR3(-100, 0, 0));
	g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	g_pShaderManager->SetTarget(g_pCamera->GetTarget());

	////////////////////////////////////////////////////////

	cUIObject* healthBar = new cUIBar(m_pPlayer, "texture/bar_back.png", "texture/bar_front.png", 5, 5);
	m_vecUIObject.push_back(healthBar);
	
	cUIObject* buttonSet = new cUIObject;
	m_vecUIObject.push_back(buttonSet);

	string buttonTag[12];
	buttonTag[0] = "시작 갯수";
	buttonTag[1] = "추가 갯수";
	buttonTag[2] = "수명";
	buttonTag[3] = "수명 랜덤";
	buttonTag[4] = "속도";
	buttonTag[5] = "속도 랜덤";
	buttonTag[6] = "가속도";
	buttonTag[7] = "가속도 랜덤";
	buttonTag[8] = "시작 색";
	buttonTag[9] = "시작 색 랜덤";
	buttonTag[10] = "끝 색";
	buttonTag[11] = "끝 색 랜덤";
	
	for (int i = 0; i < 12; i++)
	{
		cUIButton* button = new cUIButton;
		button->SetTag(buttonTag[i]+" 버튼");
		buttonSet->AddChild(button);
		button->SetTexture("texture/smallbutton_norm.png", "texture/smallbutton_over.png", "texture/smallbutton_selected.png");
		button->SetPosition(1000+(i%2)*80, ((i/2)+1) * 80);

		switch (i)
		{
		case 0:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetInitParticleNumber());
			button->SetCallbackObject3(&m_eInt);
		break;

		case 1:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetGenParticleNumber());
			button->SetCallbackObject3(&m_eInt);
			break;

		case 2:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetLifeTime());
			button->SetCallbackObject3(&m_eFloat);
			break;
		case 3:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetLifeTimeVariation());
			button->SetCallbackObject3(&m_eFloat);
			break;
		case 4:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetInitParticleNumber());
			button->SetCallbackObject3(&m_eInt);
			break;
		case 5:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetInitParticleNumber());
			button->SetCallbackObject3(&m_eInt);
			break;
		case 6:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetInitParticleNumber());
			button->SetCallbackObject3(&m_eInt);
			break;
		case 7:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetInitParticleNumber());
			button->SetCallbackObject3(&m_eInt);
			break;
		case 8:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetInitParticleNumber());
			button->SetCallbackObject3(&m_eInt);
			break;
		case 9:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetInitParticleNumber());
			button->SetCallbackObject3(&m_eInt);
			break;
		case 10:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetInitParticleNumber());
			button->SetCallbackObject3(&m_eInt);
			break;
		case 11:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetInitParticleNumber());
			button->SetCallbackObject3(&m_eInt);
			break;
		}


		cUIText* text = new cUIText;
		text->SetTag(buttonTag[i]);
		text->SetText(buttonTag[i]);
		text->SetFontType(cFontManager::E_SMALL);
		text->SetWidth(50);
		text->SetHeight(50);
		text->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
		button->AddChild(text);
	}


	
	cUIImage* textBox = new cUIImage;
	textBox->SetTexture("texture/textbox.png");
	textBox->SetTag("텍스트 박스");
	buttonSet->AddChild(textBox);
	textBox->SetPosition(975, 600);
	
	cUIText* text = new cUIText;
	text->SetTag("텍스트");
	text->SetText("이건데...");
	text->SetFontType(cFontManager::E_NORMAL);
	text->SetWidth(180);
	text->SetHeight(40);
	text->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	textBox->AddChild(text);

	//////////////////////
	m_pMap = new cMap;
	m_pMap->LoadSur("LoL/room_surface.obj");
}

void cParticleScene::Update()
{
	m_pCurParticleGroup->Update();

	for each (auto p in m_vecUIObject)
	{
		p->Update();
		if (p->GetChild("텍스트 박스"))
		{
			char str[128];
			
			if (m_eValueType==eFloat)
			{
				if(m_pCurValue)
				sprintf(str, "%.2f",*((float*)m_pCurValue));
			}

			else if (m_eValueType == eInt)
			{
				if (m_pCurValue)
				sprintf(str, "%d", *((int*)m_pCurValue));
			}

			else if (m_eValueType == eNone)
			{
				sprintf(str, "원하는 변수를 골라주세요");
			}

			cUIText* text = (cUIText*)p->GetChild("텍스트 박스")->GetChild("텍스트");
			text->SetText(str);
		}
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		Pop();
	}

	m_pPlayer->Update();
}

void cParticleScene::Render()
{
	m_pPlayer->Render();
}

void cParticleScene::UIRender()
{
	for each (auto p in m_vecUIObject)
	{
		p->Render(m_pSprite);
	}
	m_pCurParticleGroup->Render();
	RenderInfo();
}

void cParticleScene::Pop()
{
	m_pCurParticleGroup->Setup();
	m_pCurParticleGroup->Update();
}

void cParticleScene::RenderInfo()
{
	float x, y, between;
	x = 1280 - 200;
	y = 15;
	between = 30;

	char str[256];
	sprintf(str, "LifeTime :%.2f", m_pCurParticleGroup->GetLifeTime());
	g_pDebug->Print(str, x, y + between * 0);
	sprintf(str, "LifeTimeVariation %.2f", m_pCurParticleGroup->GetLifeTimeVariation());
	g_pDebug->Print(str, x, y + between * 1);
}

void cParticleScene::AlistarSpell1CallBack(void *CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}

void cParticleScene::AlistarSpell2CallBack(void * CallBackObj)
{
	cUnit* Alistar = (cUnit*)CallBackObj;

	Alistar->SetState(STATE_IDLE);
}

void cParticleScene::SetVariableToChange(void * scene, void * variable, void *variabletype)
{
	cParticleScene* thisScene = (cParticleScene*)scene;

	thisScene->SetCurValuePtr(variable);
	thisScene->SetValueType(*(eType*)variabletype);
}

D3DXVECTOR3 cParticleScene::playerPos()
{
	return (m_pPlayer)? m_pPlayer->GetPosition():D3DXVECTOR3(0,0,0);
}
