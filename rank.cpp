//------------------------------------------
//
//�����L���O�̏���[rank.cpp]
//Author fuma sato
//
//------------------------------------------

#include"rank.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"score.h"
#include"data.h"
#include"cursor.h"
#include<assert.h>

#define SELECT_WIDTH (512)
#define SELECT_HEIGHT (128)
#define NUMBER_WIDTH (64)
#define NUMBER_HEIGHT (750)
#define RANK_MAX (6)
#define RANK_TIME (600)
#define NUMBER_SEPAESE (20)

typedef enum
{
	RANKSTATE_NORMAL = 0,
	RANKSTATE_SELECT,
	RANKSTATE_MAX
}RANKSTATE;

typedef enum
{
	SELECT_BACK = 0,
	SELECT_RETRY,
	SELECT_EXIT,
	SELECT_MAX
}SELECT;

LPDIRECT3DTEXTURE9 g_apTextureRank[RANK_MAX] = { NULL };//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;//�o�b�t�@�̃|�C���^
RANKSTATE g_RankState = RANKSTATE_NORMAL;
int g_nNewDataRank;
bool g_bRankin = false;

//--------------------
//����������
//--------------------
void InitRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^
	D3DXVECTOR3 posScore;//�X�R�A�̈ʒu
	D3DXVECTOR3 posScoreDef;//�X�R�A�̈ʒu
	int nScore[MAX_DATA+1] = { 0 };
	int aPosTexUr[MAX_DATA+1][SCORE_MAX];
	int* pData;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * (SCORE_MAX*MAX_DATA+SELECT_MAX+2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_BACK,
		&g_apTextureRank[0]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NUMBER,
		&g_apTextureRank[1]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_BACKTITLE,
		&g_apTextureRank[2]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_RETRY,
		&g_apTextureRank[3]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EXIT,
		&g_apTextureRank[4]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_RANK,
		&g_apTextureRank[5]
	);

	//�ߋ��̃X�R�A�̎擾
	pData = LoadData();
	for (int i = 0; i < MAX_DATA; i++,pData++)
	{
		nScore[i] = *pData;
	}

	//����̃X�R�A�̎擾
	nScore[MAX_DATA] = GetScore();
	pData = Soat(&nScore[0]);
	for (int i = 0; i < MAX_DATA; i++, pData++)
	{
		nScore[i] = *pData;
	}
	//�Z�[�u����
	SaveData(&nScore[0]);

	//�\������
	for (int i = 0; i < MAX_DATA; i++)
	{
		if (nScore[i] == GetScore()&&nScore[i]!=0)
		{
			g_nNewDataRank = i;
			g_bRankin=true;
			break;
		}
		else if (i==MAX_DATA-1)
		{
			g_nNewDataRank = 99;
			g_bRankin = false;
		}
	}
	for (int i = 0; i < MAX_DATA; i++)
	{
		for (int i2 = 0; i2 < SCORE_MAX; i2++)
		{
			aPosTexUr[i][i2] = Digit(nScore[i], i2);
		}
	}

	SetScore(0, false);
	posScore = D3DXVECTOR3(SCREEN_WIDTH / 2 - (SCORE_WIDTH / U_MAX_S) * (SCORE_MAX / 2), SCORE_HEIGHT/2, 0.0f);
	posScoreDef = posScore;
	D3DXVECTOR3 posSelect = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SELECT_HEIGHT, 0.0f);
	g_RankState = RANKSTATE_NORMAL;

	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

		//���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += VT_MAX;
	for (int i = 0; i < MAX_DATA; i++)
	{
		for (int i2 = 0; i2 < SCORE_MAX; i2++)
		{
			//���W�ݒ�
			pVtx[0].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
			pVtx[1].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
			pVtx[2].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
			pVtx[3].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);

			//rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			//�e�N�X�`��
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2], UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2], UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S) + UV_DEF / V_MAX_S);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S) + UV_DEF / V_MAX_S);

			posScore.x += SCORE_WIDTH / U_MAX_S;
			pVtx += VT_MAX;
		}
		posScore = posScoreDef;
		posScore.y += SCORE_HEIGHT * (i+1);
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{

		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[1].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[2].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[3].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�J���[
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;
	}

	//���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - SCORE_WIDTH / 2 - NUMBER_WIDTH- NUMBER_SEPAESE, SCREEN_HEIGHT / 2 - NUMBER_HEIGHT / 2- NUMBER_SEPAESE/2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - SCORE_WIDTH / 2- NUMBER_SEPAESE, SCREEN_HEIGHT / 2 - NUMBER_HEIGHT / 2- NUMBER_SEPAESE/2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - SCORE_WIDTH / 2 - NUMBER_WIDTH- NUMBER_SEPAESE, SCREEN_HEIGHT / 2 + NUMBER_HEIGHT / 2- NUMBER_SEPAESE/2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - SCORE_WIDTH / 2- NUMBER_SEPAESE, SCREEN_HEIGHT / 2 + NUMBER_HEIGHT / 2- NUMBER_SEPAESE/2, 0.0f);

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += VT_MAX;

	g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	PlaySound(SOUND_LABEL_BGM6);
	if (g_bRankin)
	{
		if (g_nNewDataRank==0)
		{
			PlaySound(SOUND_LABEL_BEST);
		}
		else
		{
			PlaySound(SOUND_LABEL_RANKIN);
		}
	}
	else
	{
		PlaySound(SOUND_LABEL_NORANKIN);
	}
}

//--------------------
//�I������
//--------------------
void UninitRank(void)
{
	//�T�E���h
	StopSound(SOUND_LABEL_BGM6);
	//�e�N�X�`���̔j��
	for (int i = 0; i < RANK_MAX; i++)
	{
		if (g_apTextureRank[i] != NULL)
		{
			g_apTextureRank[i]->Release();
			g_apTextureRank[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
}

//--------------------
//�X�V����
//--------------------
void UpdateRank(void)
{
	static SELECT SelectNew = SELECT_BACK;
	if (g_RankState==RANKSTATE_NORMAL)
	{
		static int nRankTime = 0;

		if (nRankTime % RANK_TIME == 0)
		{
			nRankTime = 0;
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				//�T�E���h
				StopSound(SOUND_LABEL_BGM6);
				//�ؑ�
				SetFade(MODE_TITLE);
			}
		}

		if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A,CONTROLLER_MAX) == true || GetJoykeyTrigger(JOYKEY_START, CONTROLLER_MAX) == true ||GetMouseTrigger(MOUSE_LEFT)==true)
		{
			nRankTime = 0;
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				g_RankState = RANKSTATE_SELECT;
				VERTEX_2D* pVtx;//���_���|�C���^

				g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

			}
		}
		for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
		{
			if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
			{
				if (GetdJoykeyTrigger(ELEKEY_A, (CONTROLLER)i)|| GetdJoykeyTrigger(ELEKEY_START, (CONTROLLER)i))
				{
					nRankTime = 0;
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_RankState = RANKSTATE_SELECT;
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					}
				}
			}
			else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
			{
				if (GetdJoykeyTrigger(PSKEY_CI, (CONTROLLER)i)|| GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i))
				{
					nRankTime = 0;
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_RankState = RANKSTATE_SELECT;
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					}
				}
			}
			else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
			{
				if (GetdJoykeyTrigger(NINKEY_A, (CONTROLLER)i)||GetdJoykeyTrigger(NINKEY_�{, (CONTROLLER)i))
				{
					nRankTime = 0;
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_RankState = RANKSTATE_SELECT;
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					}
				}
			}
			else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
			{
				if (GetdJoykeyTrigger(DKEY_A, (CONTROLLER)i)|| GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i))
				{
					nRankTime = 0;
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_RankState = RANKSTATE_SELECT;
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					}
				}
			}
		}
		nRankTime++;
	}
	else
	{
		if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_B, CONTROLLER_MAX) == true || GetJoykeyTrigger(JOYKEY_BACK, CONTROLLER_MAX) == true ||GetMouseTrigger(MOUSE_RIGHT) == true)
		{
			g_RankState = RANKSTATE_NORMAL;
		}
		else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_SENTER) == true)
		{
			FADE fade;
			switch (SelectNew)
			{
			case SELECT_BACK:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�T�E���h
					StopSound(SOUND_LABEL_BGM6);
					//�ؑ�
					SetFade(MODE_TITLE);
				}
				break;
			case SELECT_RETRY:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�T�E���h
					StopSound(SOUND_LABEL_BGM6);
					//�ؑ�
					SetFade(MODE_GAME);
				}
				break;
			case SELECT_EXIT:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�T�E���h
					StopSound(SOUND_LABEL_BGM6);
					//�ؑ�
					HWND hWnd;
					hWnd = GethWnd();
					PostMessage(hWnd, WM_QUIT, 0, 0);
				}
				break;
			}
		}
		else if (GetMouseTrigger(MOUSE_LEFT) == true)
		{
			VERTEX_2D* pVtx;//���_���|�C���^
			CURSOR* pCursor;
			pCursor = GetCursorIn();

			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1);

			for (int i = 0; i < SELECT_MAX; i++)
			{
				if (pCursor->pos.x >= pVtx[0].pos.x && pCursor->pos.x <= pVtx[3].pos.x && pCursor->pos.y >= pVtx[0].pos.y && pCursor->pos.y <= pVtx[3].pos.y)
				{
					FADE fade;
					switch (i)
					{
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							SetFade(MODE_GAME);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
					break;
				}
				pVtx += VT_MAX;
			}

			g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		}
		if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_UP,STICK_LEFT,CONTROLLER_MAX) || GetMouseWheel() > 0.0f)
		{
			VERTEX_2D* pVtx;//���_���|�C���^

			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

			//�J���[
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

			switch (SelectNew)
			{
			case SELECT_BACK:
				SelectNew = SELECT_EXIT;
				break;
			case SELECT_RETRY:
				SelectNew = SELECT_BACK;
				break;
			case SELECT_EXIT:
				SelectNew = SELECT_RETRY;
				break;
			}

			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

			//�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		}
		else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() < 0.0f)
		{
			VERTEX_2D* pVtx;//���_���|�C���^

			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

			//�J���[
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

			switch (SelectNew)
			{
			case SELECT_BACK:
				SelectNew = SELECT_RETRY;
				break;
			case SELECT_RETRY:
				SelectNew = SELECT_EXIT;
				break;
			case SELECT_EXIT:
				SelectNew = SELECT_BACK;
				break;
			}
			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

			//�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		}
	}

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (g_RankState == RANKSTATE_NORMAL)
			{
				if (GetdJoykeyTrigger(ELEKEY_START,(CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_RankState = RANKSTATE_SELECT;
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					}
				}
			}
			else
			{
				if (GetdJoykeyTrigger(ELEKEY_BACK, (CONTROLLER)i) == true)
				{
					g_RankState = RANKSTATE_NORMAL;
				}
				else if (GetdJoykeyTrigger(ELEKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							SetFade(MODE_GAME);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_BACK:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_BACK;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_RETRY;
						break;
					}

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_BACK:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_BACK;
						break;
					}
					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (g_RankState == RANKSTATE_NORMAL)
			{
				if (GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_RankState = RANKSTATE_SELECT;
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					}
				}
			}
			else
			{
				if (GetdJoykeyTrigger(PSKEY_BACK, (CONTROLLER)i) == true)
				{
					g_RankState = RANKSTATE_NORMAL;
				}
				else if (GetdJoykeyTrigger(PSKEY_CI, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							SetFade(MODE_GAME);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_BACK:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_BACK;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_RETRY;
						break;
					}

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_BACK:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_BACK;
						break;
					}
					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (g_RankState == RANKSTATE_NORMAL)
			{
				if (GetdJoykeyTrigger(NINKEY_�{, (CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_RankState = RANKSTATE_SELECT;
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					}
				}
			}
			else
			{
				if (GetdJoykeyTrigger(NINKEY_�|, (CONTROLLER)i) == true)
				{
					g_RankState = RANKSTATE_NORMAL;
				}
				else if (GetdJoykeyTrigger(NINKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							SetFade(MODE_GAME);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_BACK:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_BACK;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_RETRY;
						break;
					}

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_BACK:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_BACK;
						break;
					}
					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (g_RankState == RANKSTATE_NORMAL)
			{
				if (GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_RankState = RANKSTATE_SELECT;
						VERTEX_2D* pVtx;//���_���|�C���^

						g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

						pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

						//�J���[
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

						g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					}
				}
			}
			else
			{
				if (GetdJoykeyTrigger(DKEY_BACK, (CONTROLLER)i) == true)
				{
					g_RankState = RANKSTATE_NORMAL;
				}
				else if (GetdJoykeyTrigger(DKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							SetFade(MODE_GAME);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM6);
							//�ؑ�
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_BACK:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_BACK;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_RETRY;
						break;
					}

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_BACK:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_BACK;
						break;
					}
					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
			}
		}
	}

	if (g_RankState == RANKSTATE_NORMAL&&g_bRankin)
	{
		static int nCount=0;
		nCount++;
		if (nCount%15==0)
		{
			static bool bP=false;
			VERTEX_2D* pVtx;//���_���|�C���^
			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

			pVtx += VT_MAX * (SCORE_MAX * g_nNewDataRank + 1);

			for (int i = 0; i < SCORE_MAX; i++)
			{
				if (bP)
				{
					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
				pVtx += VT_MAX;
			}

			g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			bP = !bP;
		}
	}
}

//--------------------
//�`�揈��
//--------------------
void DrawRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureRank[0]);

	//�w�i�̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		0,//�n�܂�̔ԍ�
		2//�|���S���̌�
	);

	int nCount=0;
	for (int i = 0; i < MAX_DATA; i++)
	{
		for (int i2 = 0; i2 < SCORE_MAX; i2++)
		{
			nCount++;
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureRank[1]);

			//�w�i�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				nCount * VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}

	if (g_RankState==RANKSTATE_SELECT)
	{
		for (int i = 0; i < SELECT_MAX; i++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureRank[i + 2]);

			//�w�i�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				i * VT_MAX + (VT_MAX * (SCORE_MAX * MAX_DATA + 1)),//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureRank[5]);

	//�w�i�̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		VT_MAX * (SCORE_MAX * MAX_DATA + SELECT_MAX+1),//�n�܂�̔ԍ�
		2//�|���S���̌�
	);
}