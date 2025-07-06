//------------------------------------------
//
//�Q�[�����C���̏���[game.cpp]
//Author fuma sato
//
//------------------------------------------

#include"game.h"
#include"player.h"
#include"block.h"
#include"fade.h"
#include"input.h"
#include"time.h"
#include"score.h"
#include"sound.h"
#include"back.h"
#include"pause.h"
#include"enemy.h"
#include"attack.h"
#include"effect.h"
#include"particle.h"
#include"scroll.h"
#include"data.h"
#include"cursor.h"
#include"item.h"

//�O���[�o���ϐ�
GAMESTATE g_gameState = GAMESTATE_NONE;
int g_nCounterGameState = 0;
int g_GameTime = 0;
bool g_bClear = false;
bool g_bEdit = false;

//--------------------
//����������
//--------------------
void InitGame(void)
{
	InitBack();
	InitPlayer();//�v���C���[
	InitBlock();
	InitEnemy();//�G
	InitItem();
	InitScore();//�X�R�A
	InitTime();
	InitAttack();
	InitEffect();
	InitParticle();

	g_gameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	g_GameTime = 0;
	g_bClear = false;
	g_bEdit = false;

	SetScroll(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	FILE* pFile;
	D3DXVECTOR3 BlockPos[MAX_BLOCK] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f) };
	D3DXVECTOR3 EnemyPos[MAX_ENEMY] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f) };
	int EnemyType[MAX_ENEMY] = { 0 };
	D3DXVECTOR3 ItemPos[MAX_ITEM] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f) };
	TYPE ItemType[MAX_ITEM] = { TYPE_NORMAL };
	pFile = fopen(EDIT_FILE, "rb");
	if (pFile != NULL)
	{//�J������
		for (int i = 0; i < MAX_BLOCK; i++)
		{
			fread(&BlockPos[i], sizeof(D3DXVECTOR3), 1, pFile);//���l����������
			if (BlockPos[i] != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			{
				SetBlock(BlockPos[i], BLOCK_WIDTH_DEF, BLOCK_HEIGHT_DEF);
			}
		}
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			fread(&EnemyPos[i], sizeof(D3DXVECTOR3), 1, pFile);//���l����������
			fread(&EnemyType[i], sizeof(int), 1, pFile);//���l����������
			if (EnemyPos[i] != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			{
				SetEnemy(EnemyPos[i], EnemyType[i]);//�G����
			}
		}
		for (int i = 0; i < MAX_ITEM; i++)
		{
			fread(&ItemPos[i], sizeof(D3DXVECTOR3), 1, pFile);//���l����������
			fread(&ItemType[i], sizeof(TYPE), 1, pFile);//���l����������
			if (ItemPos[i] != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			{
				SetItem(ItemPos[i], D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemType[i]);//�G����
			}
		}
		fclose(pFile);//�t�@�C�������
	}

	SetScore(0,true);//�X�R�A��������
	SetTime(TIME_LIMIT - g_GameTime / FRAME, true);

	PlaySound(SOUND_LABEL_BGM);
}

//------------------
//�I������
//------------------
void UninitGame(void)
{
	g_gameState = GAMESTATE_NONE;

	//�T�E���h
	StopSound(SOUND_LABEL_BGM);

	UninitTime();
	UninitScore();//�X�R�A
	UninitItem();
	UninitEnemy();//�G
	UninitBlock();
	UninitPlayer();//�v���C���[
	UninitParticle();
	UninitEffect();
	UninitBack();//�w�i
}

//--------------
//�X�V����
//--------------
void UpdateGame(void)
{
	FADE fade;
	fade = GetFade();
	if (fade == FADE_NONE)
	{
#ifdef _DEBUG

		if (GetKeyboradTrigger(DIK_YEN))
		{
			g_bEdit = !g_bEdit;
		}

#endif

		if (!g_bEdit)
		{
			static int SpoItemCount = 0;

			if (g_gameState != GAMESTATE_PAUSE)
			{
				Player* pPlayer;

				pPlayer = GetPlayer();

				switch (g_gameState)
				{
				case GAMESTATE_NORMAL:
					g_GameTime++;
					SetTime(TIME_LIMIT - g_GameTime / FRAME, true);
					if (g_GameTime / FRAME >= TIME_LIMIT)
					{
						g_gameState = GAMESTATE_END;
					}
					else if (Goal())
					{
						g_gameState = GAMESTATE_END;
						g_bClear = true;
					}
					else if (pPlayer->state == PLAYERSTATE_DIE)
					{
						g_gameState = GAMESTATE_END;
					}
					//else if (g_GameTime / FRAME == TIME_LIMIT / 2)
					//{
					//	SetScroll(D3DXVECTOR3(-SCROLL_X, -SCROLL_Y, 0.0f));
					//}
					break;
				case GAMESTATE_END:
					g_nCounterGameState++;
					if (g_nCounterGameState >= INTER)
					{
						FADE fade;
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM4);
							//�ؑ�
							SetFade(MODE_RESULT);
						}
					}
					break;
				}

				if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_SENTER) == true)
				{
					g_gameState = GAMESTATE_PAUSE;
					StopSound();
				}

#ifdef _DEBUG

				if (GetKeyboradTrigger(DIK_M) == true)
				{
					g_gameState = GAMESTATE_END;
				}

#endif

				for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
				{
					if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
					{
						if (GetdJoykeyTrigger(ELEKEY_START, (CONTROLLER)i) == true)
						{
							g_gameState = GAMESTATE_PAUSE;
							StopSound();
						}
					}
					else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
					{
						if (GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i) == true)
						{
							g_gameState = GAMESTATE_PAUSE;
							StopSound();
						}
					}
					else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
					{
						if (GetdJoykeyTrigger(NINKEY_�{, (CONTROLLER)i) == true)
						{
							g_gameState = GAMESTATE_PAUSE;
							StopSound();
						}
					}
					else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
					{
						if (GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i) == true)
						{
							g_gameState = GAMESTATE_PAUSE;
							StopSound();
						}
					}
				}

				UpdateBack();//�w�i
				UpdateAttack();//
				UpdateBlock();
				UpdatePlayer();//�v���C���[
				UpdateEnemy();//�G
				UpdateEffect();
				UpdateParticle();
				UpdateItem();
				UpdateScore();//�X�R�A
				UpdateTime();
			}
			else
			{
				UpdatePause();
			}

		}
		else
		{
			EditMode();
		}
	}
}

//-------------------
//�`�揈��
//-------------------
void DrawGame(void)
{
	DrawBack();//�w�i
	DrawEffect();
	DrawPlayer();//�v���C���[
	DrawBlock();
	DrawEnemy();//�G
	DrawItem();
	DrawScore();//�X�R�A
	DrawTime();
	if(g_gameState==GAMESTATE_PAUSE)
	{
		DrawPause();
	}
}

//----------------------
//�Q�[����Ԑؑ�
//----------------------
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//-----------------------
//�Q�[����Ԏ擾
//-----------------------
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//---------------------
//�N���A�擾
//---------------------
bool GetClear(void)
{
	return g_bClear;
}

//----------------
//�G�f�B�b�g����
//----------------
bool Edit(void)
{
	return g_bEdit;
}

//---------------------
//�G�f�B�b�g
//---------------------
void EditMode(void)
{
	static D3DXVECTOR3 EditScroll = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static int Select = false;
	static int nEnemy = 0;
	static int nItem = 0;
	D3DXVECTOR3 NullVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nNullType = -1;
	TYPE NullType = TYPE_NORMAL;
	if (GetMouseTrigger(MOUSE_LEFT))
	{
		CURSOR* pCursor;
		pCursor = GetCursorIn();
		switch (Select % 3)
		{
		case 0:
			Block* pBlock;
			pBlock = GetBlock();
			for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
			{
				if (pBlock->bUse)
				{
					D3DXVECTOR3 Space = pCursor->pos - pBlock->pos;
					float Long = sqrtf(Space.x * Space.x + Space.y * Space.y);
					if (Long <= EDIT_BLOCK_SPACE)
					{
						if (pCursor->pos.x <= pBlock->pos.x - pBlock->fWidth * 0.5f)
						{
							SetBlock(D3DXVECTOR3(pBlock->pos.x - BLOCK_WIDTH_DEF, pBlock->pos.y, 0.0f), BLOCK_WIDTH_DEF, BLOCK_HEIGHT_DEF);
							break;
						}
						else if (pCursor->pos.x >= pBlock->pos.x + pBlock->fWidth * 0.5f)
						{
							SetBlock(D3DXVECTOR3(pBlock->pos.x + BLOCK_WIDTH_DEF, pBlock->pos.y, 0.0f), BLOCK_WIDTH_DEF, BLOCK_HEIGHT_DEF);
							break;
						}
						else if (pCursor->pos.y <= pBlock->pos.y - pBlock->fHeight * 0.5f)
						{
							SetBlock(D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y - BLOCK_HEIGHT_DEF, 0.0f), BLOCK_WIDTH_DEF, BLOCK_HEIGHT_DEF);
							break;
						}
						else if (pCursor->pos.y >= pBlock->pos.y + pBlock->fHeight * 0.5f)
						{
							SetBlock(D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y + BLOCK_HEIGHT_DEF, 0.0f), BLOCK_WIDTH_DEF, BLOCK_HEIGHT_DEF);
							break;
						}
						else
						{
							SetBlock(D3DXVECTOR3(pBlock->pos.x - BLOCK_WIDTH_DEF, pBlock->pos.y, 0.0f), BLOCK_WIDTH_DEF, BLOCK_HEIGHT_DEF);
							SetBlock(D3DXVECTOR3(pBlock->pos.x + BLOCK_WIDTH_DEF, pBlock->pos.y, 0.0f), BLOCK_WIDTH_DEF, BLOCK_HEIGHT_DEF);
							SetBlock(D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y - BLOCK_HEIGHT_DEF, 0.0f), BLOCK_WIDTH_DEF, BLOCK_HEIGHT_DEF);
							SetBlock(D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y + BLOCK_HEIGHT_DEF, 0.0f), BLOCK_WIDTH_DEF, BLOCK_HEIGHT_DEF);
							break;
						}
					}
				}
				else if (i == MAX_BLOCK - 1)
				{
					SetBlock(pCursor->pos, BLOCK_WIDTH_DEF, BLOCK_HEIGHT_DEF);
				}
			}
			break;
		case 1:
			SetEnemy(pCursor->pos, nEnemy % NUM_ENEMY);
			break;
		case 2:
			SetItem(pCursor->pos,D3DXVECTOR3(0.0f,0.0f,0.0f), (TYPE)(nItem % TYPE_MAX));
			break;
		}

		UpdateBlock();
		UpdateEditEnemy();
		UpdateItem();
	}
	else if (GetMouseTrigger(MOUSE_RIGHT))
	{
		CURSOR* pCursor;
		pCursor = GetCursorIn();
		Block* pBlock;
		pBlock = GetBlock();
		Enemy* pEnemy;
		pEnemy = GetEnemy();
		Item* pItem;
		pItem = GetItem();
		float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
		switch (nEnemy % NUM_ENEMY)
		{
		case 0:
			EnemyWidth = ENEMY_WIDTH;
			EnemyHeight = ENEMY_HEIGHT;
			break;
		case 1:
			EnemyWidth = ENEMY_WIDTH2;
			EnemyHeight = ENEMY_HEIGHT2;
			break;
		case 2:
			EnemyWidth = ENEMY_WIDTH3;
			EnemyHeight = ENEMY_HEIGHT3;
			break;
		case 3:
			EnemyWidth = ENEMY_WIDTH4;
			EnemyHeight = ENEMY_HEIGHT4;
			break;
		}
		for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
		{
			if (pCursor->pos.x >= pBlock->pos.x - BLOCK_WIDTH_DEF * 0.5f && pCursor->pos.x <= pBlock->pos.x + BLOCK_WIDTH_DEF * 0.5f && pCursor->pos.y >= pBlock->pos.y - BLOCK_HEIGHT_DEF * 0.5f && pCursor->pos.y <= pBlock->pos.y + BLOCK_HEIGHT_DEF * 0.5f)
			{
				pBlock->bUse = false;
			}
		}
		for (int i = 0; i < MAX_ENEMY; i++,pEnemy++)
		{
			if (pCursor->pos.x >= pEnemy->pos.x - EnemyWidth * 0.5f && pCursor->pos.x <= pEnemy->pos.x + EnemyWidth * 0.5f && pCursor->pos.y >= pEnemy->pos.y - EnemyHeight && pCursor->pos.y <= pEnemy->pos.y)
			{
				pEnemy->bUse = false;
			}
		}
		for (int i = 0; i < MAX_ITEM; i++, pItem++)
		{
			if (pCursor->pos.x >= pItem->pos.x - ITEM_SIZE * 0.5f && pCursor->pos.x <= pItem->pos.x + ITEM_SIZE * 0.5f && pCursor->pos.y >= pItem->pos.y - ITEM_SIZE && pCursor->pos.y <= pItem->pos.y + ITEM_SIZE)
			{
				pItem->bUse = false;
			}
		}
	}
	else if (GetMousePress(MOUSE_SENTER))
	{
		Block* pBlock;
		pBlock = GetBlock();
		Enemy* pEnemy;
		pEnemy = GetEnemy();
		Item* pItem;
		pItem = GetItem();
		float Xmove = 0.0f, Ymove = 0.0f;
		Xmove = *GetMouseMove();
		Ymove = *(GetMouseMove() + 1);
		for (int i = 0; i < MAX_BLOCK; i++,pBlock++)
		{
			pBlock->pos.x += Xmove * CURSOR_SPEED_X;
			pBlock->pos.y += Ymove * CURSOR_SPEED_Y;
		}
		for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
		{
			pEnemy->pos.x += Xmove * CURSOR_SPEED_X;
			pEnemy->pos.y += Ymove * CURSOR_SPEED_Y;
		}
		for (int i = 0; i < MAX_ITEM; i++, pItem++)
		{
			pItem->pos.x += Xmove * CURSOR_SPEED_X;
			pItem->pos.y += Ymove * CURSOR_SPEED_Y;
		}
		EditScroll += D3DXVECTOR3(-Xmove * CURSOR_SPEED_X, -Ymove * CURSOR_SPEED_Y, 0.0f);

		UpdateBlock();
		UpdateEditEnemy();
		UpdateItem();
	}
	else if (GetMouseWheel() != 0.0f)
	{
	    Select++;
	}
	else if (GetMouseTrigger(MOUSE_B1))
	{
		switch (Select % 3)
		{
		case 1:
			nEnemy++;
			break;
		case 2:
			nItem++;
			break;
		}
	}
	else if (GetMouseTrigger(MOUSE_B2))
	{
	switch (Select % 3)
	{
	case 1:
		nEnemy--;
		break;
	case 2:
		nItem--;
		break;
	}
	}

	if (GetKeyboradTrigger(DIK_S))
	{
		FILE* pFile;
		Block* pBlock;
		Enemy* pEnemy;
		Item* pItem;
		pBlock = GetBlock();
		pEnemy = GetEnemy();
		pItem = GetItem();
		pFile = fopen(EDIT_FILE, "wb");
		if (pFile != NULL)
		{//�J������
			for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
			{
				if (pBlock->bUse)
				{
					fwrite(&pBlock->pos, sizeof(D3DXVECTOR3), 1, pFile);//���l����������
				}
				else
				{
					fwrite(&NullVector, sizeof(D3DXVECTOR3), 1, pFile);//���l����������
				}
			}
			for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
			{
				if (pEnemy->bUse)
				{
					fwrite(&pEnemy->pos, sizeof(D3DXVECTOR3), 1, pFile);//���l����������
					fwrite(&pEnemy->nType, sizeof(int), 1, pFile);//���l����������
				}
				else
				{
					fwrite(&NullVector, sizeof(D3DXVECTOR3), 1, pFile);//���l����������
					fwrite(&nNullType, sizeof(int), 1, pFile);//���l����������
				}
			}
			for (int i = 0; i < MAX_ITEM; i++, pItem++)
			{
				if (pItem->bUse)
				{
					fwrite(&pItem->pos, sizeof(D3DXVECTOR3), 1, pFile);//���l����������
					fwrite(&pItem->Type, sizeof(int), 1, pFile);//���l����������
				}
				else
				{
					fwrite(&NullVector, sizeof(D3DXVECTOR3), 1, pFile);//���l����������
					fwrite(&NullType, sizeof(TYPE), 1, pFile);//���l����������
				}
			}
			fclose(pFile);//�t�@�C�������
			EditScroll = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	if (GetKeyboradTrigger(DIK_L))
	{
		FILE* pFile;
		Block* pBlock;
		Enemy* pEnemy;
		Item* pItem;
		pBlock = GetBlock();
		pEnemy = GetEnemy();
		pItem = GetItem();
		for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
		{
			pBlock->bUse = false;
		}
		for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
		{
			pEnemy->bUse = false;
		}
		for (int i = 0; i < MAX_ITEM; i++, pItem++)
		{
			pItem->bUse = false;
		}
		pBlock = GetBlock();
		pEnemy = GetEnemy();
		pItem = GetItem();
		pFile = fopen(EDIT_FILE, "rb");
		if (pFile != NULL)
		{//�J������
			for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
			{
				fread(&pBlock->pos, sizeof(D3DXVECTOR3), 1, pFile);//���l����������
				if (pBlock->pos!=NullVector)
				{
					pBlock->bUse = true;
				}
			}
			for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
			{
				fread(&pEnemy->pos, sizeof(D3DXVECTOR3), 1, pFile);//���l����������
				fread(&pEnemy->nType, sizeof(int), 1, pFile);//���l����������
				if (pEnemy->pos != NullVector)
				{
					pEnemy->bUse = true;
				}
			}
			for (int i = 0; i < MAX_ITEM; i++, pItem++)
			{
				fread(&pItem->pos, sizeof(D3DXVECTOR3), 1, pFile);//���l����������
				fread(&pItem->Type, sizeof(TYPE), 1, pFile);//���l����������
				if (pItem->pos != NullVector)
				{
					pItem->bUse = true;
				}
			}
			fclose(pFile);//�t�@�C�������
		}

		EditScroll = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		UpdateBlock();
		UpdateEditEnemy();
		UpdateItem();
	}

	if (GetKeyboradTrigger(DIK_D))
	{
		Block* pBlock;
		Enemy* pEnemy;
		Item* pItem;
		pBlock = GetBlock();
		pEnemy = GetEnemy();
		pItem = GetItem();
		for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
		{
			pBlock->pos += EditScroll;
		}
		for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
		{
			pEnemy->pos += EditScroll;
		}
		for (int i = 0; i < MAX_ITEM; i++, pItem++)
		{
			pItem->pos += EditScroll;
		}
		EditScroll = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		UpdateBlock();
		UpdateEditEnemy();
		UpdateItem();
	}

	if (GetKeyboradTrigger(DIK_LSHIFT))
	{
		Block* pBlock;
		Enemy* pEnemy;
		Item* pItem;
		pBlock = GetBlock();
		pEnemy = GetEnemy();
		pItem = GetItem();
		for (int i = 0; i < MAX_BLOCK; i++,pBlock++)
		{
			pBlock->bUse = false;
		}
		for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
		{
			pEnemy->bUse = false;
		}
		for (int i = 0; i < MAX_ITEM; i++, pItem++)
		{
			pItem->bUse = false;
		}
	}
	if (GetKeyboradTrigger(DIK_BACK))
	{
		Block* pBlock;
		pBlock = GetBlock();
		for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
		{
			pBlock->bUse = false;
		}
	}
	if (GetKeyboradTrigger(DIK_END))
	{
		Enemy* pEnemy;
		pEnemy = GetEnemy();
		for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
		{
			pEnemy->bUse = false;
		}
	}
	if (GetKeyboradTrigger(DIK_RSHIFT))
	{
		Item* pItem;
		pItem = GetItem();
		for (int i = 0; i < MAX_ENEMY; i++, pItem++)
		{
			pItem->bUse = false;
		}
	}
}