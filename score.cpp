//---------------------------------------
//
//�X�R�A����[score.cpp]
//Author fuma sato
//
//---------------------------------------

#include"score.h"

LPDIRECT3DTEXTURE9 g_pTextureScore=NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posScore;//�X�R�A�̈ʒu
int g_nScore;//�X�R�A�̒l
//-------------------------
//�|���S���̏���������
//-------------------------
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX*SCORE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NUMBER,
		&g_pTextureScore
	);

	g_posScore = D3DXVECTOR3(SCREEN_WIDTH-SCORE_MAX*(SCORE_WIDTH/U_MAX_S)+ (SCORE_WIDTH / U_MAX_S)/2,(SCORE_HEIGHT/V_MAX_S)/2,0.0f);
	g_nScore = 0;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, g_posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, g_posScore.z);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, g_posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, g_posScore.z);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, g_posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, g_posScore.z);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, g_posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, g_posScore.z);

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
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX_S);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S, UV_DEF / V_MAX_S);

		g_posScore.x += SCORE_WIDTH/U_MAX_S;
		pVtx += VT_MAX;
	}

	g_pVtxBuffScore->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//--------------------------
//�|���S���̏I������
//--------------------------
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//--------------------------
//�|���S���̍X�V����
//--------------------------
void UpdateScore(void)
{

}

//----------------------------
//�|���S���̕`�揈��
//----------------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore);

		//�w�i�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			VT_MAX * i,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}
}

//-----------------------------
//�X�R�A�̏�������(�ۑ�)
//-----------------------------
void SetScore(int nScore, bool bTex)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	int aPosTexU[SCORE_MAX];
	g_nScore = nScore;

	//�X�R�A�ݒ�
	for (int i = 0; i < SCORE_MAX; i++)
	{
		aPosTexU[i] = Digit(g_nScore,i);
	}

	if (bTex)
	{
		//���W
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

		for (int i = 0; i < SCORE_MAX; i++)
		{
			//�e�N�X�`��
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);

			pVtx += VT_MAX;
		}

		g_pVtxBuffScore->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}
}

//------------------------------
//�X�R�A�̉��Z
//------------------------------
void AddScore(int nValue)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	int aPosTexU[SCORE_MAX];
	g_nScore += nValue;

	//�}�C�i�X�X�R�A�����O
	if (g_nScore<0)
	{
		g_nScore = 0;
	}

	//�X�R�A�ݒ�
	for (int i = 0; i < SCORE_MAX; i++)
	{
		aPosTexU[i] = Digit(g_nScore, i);
	}

    //���W
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);

		pVtx += VT_MAX;
	}

	g_pVtxBuffScore->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//------------------------
//�X�R�A�̎擾
//------------------------
int GetScore(void)
{
	return g_nScore;
}

//--------------------
//����������
//--------------------
int Digit(int nData, int nDigit)
{
	int aNumber[SCORE_MAX] = { 0 };//�����p�z��
	int nData2 = 1;//�v�Z�p

	for (int i = SCORE_MAX - 1; i >= 0; i--)
	{//������
		if (i == 0)
		{
			aNumber[i] = nData / nData2;//�ŏ�ʂ̌�
		}
		aNumber[i] = nData % (nData2 * DIGIT) / nData2;//�v�Z
		nData2 *= DIGIT;//�������グ
	}

	return aNumber[nDigit];//�w��̌���Ԃ�
}