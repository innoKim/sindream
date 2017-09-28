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
#include "cUITextInput.h"
#include "cMap.h"
#include "cGridPlane.h"
#include "cEffectLoader.h"

cParticleScene::cParticleScene() :
	target(0, 0, 0),
	m_pSprite(NULL),
	m_pPlayer(NULL),
	m_pCurParticleGroup(NULL),
	m_pCurValue(NULL),
	m_eValueType(eNone),
	m_eNone(eType::eNone),
	m_eFloat(eType::eFloat),
	m_eInt(eType::eInt),
	m_eVector3(eType::eVector3),
	m_eColor(eType::eColor),
	m_pGrid(NULL),
	m_pTextInput(NULL)
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

	SAFE_DELETE(m_pGrid)
}

void cParticleScene::Setup()
{
	g_pShaderManager->SetSkyBoxOn(false);

	g_pShaderManager->SetTarget(&target);

	m_pCurParticleGroup = g_pEffectManager->NewEffect();

	//////////////////////////////////////////////////////

	/*m_pParticleGroup->SetInitParticleNumber(20);
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

	/*m_pPlayer = new cPlayer;
	vector<ST_UNITLOADINFO> temp;
	temp.push_back({ STATE_IDLE, "unit/AlistarIdle.x" });
	temp.push_back({ STATE_RUN, "unit/AlistarRun.x" });
	temp.push_back({ STATE_SPELL1, "unit/AlistarSpell1.x",ST_CallbackInfo(0.0f,AlistarSpell1CallBack,m_pPlayer) });
	temp.push_back({ STATE_SPELL2, "unit/AlistarSpell2.x",ST_CallbackInfo(0.0f,AlistarSpell2CallBack,m_pPlayer) });
	m_pPlayer->Setup(temp,NULL);
	m_pPlayer->SetPosition(D3DXVECTOR3(-100, 0, 0));
	g_pCamera->SetTarget(m_pPlayer->GetPosPtr());
	g_pShaderManager->SetTarget(g_pCamera->GetTarget());*/

	////////////////////////////////////////////////////////

	UISetup();

	//////////////////////
	m_pGrid = new cGridPlane;
	m_pGrid->Setup(100, 50);
}

void cParticleScene::Update()
{
	m_pCurParticleGroup->Update();

	for each (auto p in m_vecCurParticleGroup)
	{
		p->Update();
	}

	TextBoxPrint();
	ValueControl();


	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		Pop();
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_CONTROL))
	{
		PopVector();
	}

	if (g_pKeyManager->IsStayKeyDown(VK_LSHIFT)&&g_pKeyManager->IsOnceKeyDown(VK_RBUTTON))
	{
		SetParticlePos();
	}

	if(m_pPlayer)m_pPlayer->Update();
}

void cParticleScene::Render()
{
	if (m_pPlayer)m_pPlayer->Render();
}

void cParticleScene::UIRender()
{
	for each (auto p in m_vecUIObject)
	{
		p->Render(m_pSprite);
	}

	m_pCurParticleGroup->Render();

	for each (auto p in m_vecCurParticleGroup)
	{
		p->Render();
	}
	
	RenderInfo();

	if(m_pGrid) m_pGrid->Render();
}

void cParticleScene::Pop()
{
	m_pCurParticleGroup->Setup();
	m_pCurParticleGroup->Update();
}

void cParticleScene::PopVector()
{
	for each(auto p in m_vecCurParticleGroup)
	{
		p->Setup();
		p->Update();
	}
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

void cParticleScene::ClearParticleVector()
{
	for each(auto p in m_vecCurParticleGroup)
	{
		SAFE_DELETE(p);
	}

	m_vecCurParticleGroup.clear();
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

void cParticleScene::SaveButtonCallBack(void * CallBackObj)
{
	cParticleScene* thisScene = (cParticleScene*)CallBackObj;

	thisScene->Save();

}

void cParticleScene::LoadButtonCallBack(void * CallBackObj)
{
	cParticleScene* thisScene = (cParticleScene*)CallBackObj;

	thisScene->Load();
}

void cParticleScene::SetTextureCallBack(void * CallBackObj)
{
	cParticleScene* thisScene = (cParticleScene*)CallBackObj;

	cUITextInput* ti = thisScene->GetTextInputPtr();

	thisScene->GetCurParticleGroupPtr()->SetTexturePath(ti->GetText());

	for each (auto p in thisScene->GetCurParticleGroupVectorRef())
	{
		p->SetTexturePath(ti->GetText());
	}
}

void cParticleScene::NewParticleCallBack(void * CallBackObj)
{
	cParticleScene* thisScene = (cParticleScene*)CallBackObj;
	
	cParticleGroup* newParticleGroup = new cParticleGroup;
	newParticleGroup->Clone(thisScene->GetCurParticleGroupPtr());
	thisScene->GetCurParticleGroupVectorRef().push_back(newParticleGroup);
}

void cParticleScene::InitCallBack(void * CallBackObj)
{
	cParticleScene* thisScene = (cParticleScene*)CallBackObj;

	thisScene->ClearParticleVector();
	thisScene->GetCurParticleGroupPtr()->Init();
}

D3DXVECTOR3 cParticleScene::playerPos()
{
	return (m_pPlayer)? m_pPlayer->GetPosition():D3DXVECTOR3(0,0,0);
}

void cParticleScene::UISetup()
{

	cUIObject* healthBar = new cUIBar(m_pPlayer, "texture/bar_back.png", "texture/bar_front.png", 5, 5);
	m_vecUIObject.push_back(healthBar);

	cUIObject* buttonSet = new cUIObject;
	m_vecUIObject.push_back(buttonSet);

	string buttonTag[19];
	buttonTag[0] = "���� ����";
	buttonTag[1] = "����";
	buttonTag[2] = "���� ����";
	buttonTag[3] = "��ġ ����";
	buttonTag[4] = "�ӵ� ����";
	buttonTag[5] = "�ӵ�";
	buttonTag[6] = "�ӵ� ����";
	buttonTag[7] = "���ӵ�";
	buttonTag[8] = "���ӵ� ����";
	buttonTag[9] = "���� ��";
	buttonTag[10] = "���� �� ����";
	buttonTag[11] = "�� ��";
	buttonTag[12] = "�� �� ����";
	buttonTag[13] = "���� �ϱ�";
	buttonTag[14] = "�ҷ� ����";
	buttonTag[15] = "�ؽ��� ��������";
	buttonTag[16] = "�߰� ��ƼŬ";
	buttonTag[17] = "�ʱ�ȭ";

	for (int i = 0; i < 18; i++)
	{
		cUIButton* button = new cUIButton;
		button->SetTag(buttonTag[i] + " ��ư");
		buttonSet->AddChild(button);
		button->SetTexture("texture/smallbutton_norm.png", "texture/smallbutton_over.png", "texture/smallbutton_selected.png");
		
		if (i < 13)
		{
			button->SetPosition(920 + (i % 4) * 80, ((i / 4) + 1) * 80);
		}
		else
		{
			button->SetPosition(((i+1) % 2 + 1) * 80, (((i-13) / 2) + 1) * 80);
		}
		
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
			button->SetCallbackObject2(&m_pCurParticleGroup->GetLifeTime());
			button->SetCallbackObject3(&m_eFloat);
			break;

		case 2:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetLifeTimeVariation());
			button->SetCallbackObject3(&m_eFloat);
			break;

		case 3:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetStartPositionVariation());
			button->SetCallbackObject3(&m_eFloat);
			break;

		case 4:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetDragVelocity());
			button->SetCallbackObject3(&m_eFloat);
			break;

		case 5:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetVelocity());
			button->SetCallbackObject3(&m_eVector3);
			break;

		case 6:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetVelocityVariation());
			button->SetCallbackObject3(&m_eFloat);
			break;

		case 7:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetAcceleration());
			button->SetCallbackObject3(&m_eVector3);
			break;

		case 8:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetAccelerationVariation());
			button->SetCallbackObject3(&m_eFloat);
			break;

		case 9:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetStartColor());
			button->SetCallbackObject3(&m_eColor);
			break;
		case 10:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetStartColorVariation());
			button->SetCallbackObject3(&m_eColor);
			break;
		case 11:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetEndColor());
			button->SetCallbackObject3(&m_eColor);
			break;

		case 12:
			button->SetCallback3(SetVariableToChange);
			button->SetCallbackObject(this);
			button->SetCallbackObject2(&m_pCurParticleGroup->GetEndColorVariation());
			button->SetCallbackObject3(&m_eColor);
			break;

		case 13:
			button->SetCallback(SaveButtonCallBack);
			button->SetCallbackObject(this);
			break;

		case 14:
			button->SetCallback(LoadButtonCallBack);
			button->SetCallbackObject(this);
			break;

		case 15:
			button->SetCallback(SetTextureCallBack);
			button->SetCallbackObject(this);
			break;

		case 16:
			button->SetCallback(NewParticleCallBack);
			button->SetCallbackObject(this);
			break;

		case 17:
			button->SetCallback(InitCallBack);
			button->SetCallbackObject(this);
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
	textBox->SetTag("�ؽ�Ʈ �ڽ�");
	buttonSet->AddChild(textBox);
	textBox->SetPosition(975, 640);

	cUIText* text = new cUIText;
	text->SetTag("�ؽ�Ʈ");
	text->SetText("�̰ǵ�...");
	text->SetFontType(cFontManager::E_SMALL);
	text->SetWidth(180);
	text->SetHeight(40);
	text->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	textBox->AddChild(text);

	cUITextInput* textInput = new cUITextInput("texture/textbox.png");
	textInput->SetPosition(60, 320);
	buttonSet->AddChild(textInput);

	m_pTextInput = textInput;
}

void cParticleScene::TextBoxPrint()
{
	for each (auto p in m_vecUIObject)
	{
		p->Update();
		if (p->GetChild("�ؽ�Ʈ �ڽ�"))
		{
			char str[128];

			if (m_eValueType == eFloat)
			{
				if (m_pCurValue)
					sprintf(str, "%.2f", *((float*)m_pCurValue));
			}

			else if (m_eValueType == eInt)
			{
				if (m_pCurValue)
					sprintf(str, "%d", *((int*)m_pCurValue));
			}

			else if (m_eValueType == eNone)
			{
				sprintf(str, "���ϴ� ������ ����ּ���");
			}

			else if (m_eValueType == eVector3)
			{
				D3DXVECTOR3 tempVector = *((D3DXVECTOR3*)m_pCurValue);
				sprintf(str, "x : %.2f, y : %.2f, z : %.2f", tempVector.x, tempVector.y, tempVector.z);
			}

			else if (m_eValueType == eColor)
			{
				D3DXCOLOR tempColor = *((D3DXCOLOR*)m_pCurValue);
				sprintf(str, "r : %.2f, g : %.2f, b : %.2f, a : %.2f", tempColor.r, tempColor.g, tempColor.b, tempColor.a);
			}

			cUIText* text = (cUIText*)p->GetChild("�ؽ�Ʈ �ڽ�")->GetChild("�ؽ�Ʈ");
			text->SetText(str);
		}
	}
}

void cParticleScene::ValueControl()
{
	if (m_eValueType == eFloat)
	{
		if (g_pKeyManager->IsStayKeyDown('T'))
		{
			*((float*)m_pCurValue) += 0.1f;
		}
		if (g_pKeyManager->IsStayKeyDown('G'))
		{
			*((float*)m_pCurValue) -= 0.1f;
		}
	}

	else if (m_eValueType == eInt)
	{
		if (g_pKeyManager->IsStayKeyDown('T'))
		{
			*((int*)m_pCurValue) += 1;
		}
		if (g_pKeyManager->IsStayKeyDown('G'))
		{
			*((int*)m_pCurValue) -= 1;
		}
	}

	else if (m_eValueType == eNone)
	{
	}

	else if (m_eValueType == eVector3)
	{
		if (g_pKeyManager->IsStayKeyDown('T'))
		{
			*((D3DXVECTOR3*)m_pCurValue) += D3DXVECTOR3(0.1f,0,0);
		}
		if (g_pKeyManager->IsStayKeyDown('G'))
		{
			*((D3DXVECTOR3*)m_pCurValue) -= D3DXVECTOR3(0.1f, 0, 0);
		}

		if (g_pKeyManager->IsStayKeyDown('Y'))
		{
			*((D3DXVECTOR3*)m_pCurValue) += D3DXVECTOR3(0, 0.1f, 0);
		}
		if (g_pKeyManager->IsStayKeyDown('H'))
		{
			*((D3DXVECTOR3*)m_pCurValue) -= D3DXVECTOR3(0, 0.1f, 0);
		}

		if (g_pKeyManager->IsStayKeyDown('U'))
		{
			*((D3DXVECTOR3*)m_pCurValue) += D3DXVECTOR3(0, 0, 0.1f);
		}
		if (g_pKeyManager->IsStayKeyDown('J'))
		{
			*((D3DXVECTOR3*)m_pCurValue) -= D3DXVECTOR3(0, 0, 0.1f);
		}
	}

	else if (m_eValueType == eColor)
	{
		if (g_pKeyManager->IsStayKeyDown('T'))
		{
			(*((D3DXCOLOR*)m_pCurValue)).r += 0.1f;
		}
		if (g_pKeyManager->IsStayKeyDown('G'))
		{
			(*((D3DXCOLOR*)m_pCurValue)).r -= 0.1f;
		}

		if (g_pKeyManager->IsStayKeyDown('Y'))
		{
			(*((D3DXCOLOR*)m_pCurValue)).g += 0.1f;
		}
		if (g_pKeyManager->IsStayKeyDown('H'))
		{
			(*((D3DXCOLOR*)m_pCurValue)).g -= 0.1f;
		}

		if (g_pKeyManager->IsStayKeyDown('U'))
		{
			(*((D3DXCOLOR*)m_pCurValue)).b += 0.1f;
		}
		if (g_pKeyManager->IsStayKeyDown('J'))
		{
			(*((D3DXCOLOR*)m_pCurValue)).b -= 0.1f;
		}

		if (g_pKeyManager->IsStayKeyDown('I'))
		{
			(*((D3DXCOLOR*)m_pCurValue)).a += 0.1f;
		}
		if (g_pKeyManager->IsStayKeyDown('K'))
		{
			(*((D3DXCOLOR*)m_pCurValue)).a -= 0.1f;
		}
	}
}

void cParticleScene::Save()
{
	cEffectLoader el;
	el.SaveEffect(m_pTextInput->GetText(),m_vecCurParticleGroup);
}

void cParticleScene::Load()
{
	cEffectLoader el;
	ClearParticleVector();
	el.LoadEffect(m_pTextInput->GetText(), m_vecCurParticleGroup);
}

void cParticleScene::SetParticlePos()
{
	RayInfo ray = RayToScreenPoint(g_ptMouse.x, g_ptMouse.y);

	while (fabs(ray.pos.y)>0.1f&& fabs(ray.pos.y)<10000.0f)
	{
		ray.pos += ray.dir*0.1f;
	}

	D3DXVECTOR3 hitpos(ray.pos.x, 0, ray.pos.z);

	m_pCurParticleGroup->SetStartPosition(hitpos);
}
