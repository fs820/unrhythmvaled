//---------------------------------------
//
//�X�R�A����[time.cpp]
//Author fuma sato
//
//---------------------------------------

#include"Time.h"

LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;//�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posTime;//�X�R�A�̈ʒu
int g_nTime;//�X�R�A�̒l
//-------------------------
//�|���S���̏���������
//-------------------------
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * TIME_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NUMBER,
		&g_pTextureTime
	);

	g_posTime = D3DXVECTOR3((TIME_WIDTH / U_MAX_S) / 2, (TIME_HEIGHT / V_MAX_S) / 2, 0.0f);
	g_nTime = 0;

	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	for (int i = 0; i < TIME_MAX; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x - (TIME_WIDTH / U_MAX_S) / 2, g_posTime.y - (TIME_HEIGHT / V_MAX_S) / 2, g_posTime.z);
		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + (TIME_WIDTH / U_MAX_S) / 2, g_posTime.y - (TIME_HEIGHT / V_MAX_S) / 2, g_posTime.z);
		pVtx[2].pos = D3DXVECTOR3(g_posTime.x - (TIME_WIDTH / U_MAX_S) / 2, g_posTime.y + (TIME_HEIGHT / V_MAX_S) / 2, g_posTime.z);
		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + (TIME_WIDTH / U_MAX_S) / 2, g_posTime.y + (TIME_HEIGHT / V_MAX_S) / 2, g_posTime.z);

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

		g_posTime.x += TIME_WIDTH / U_MAX_S;
		pVtx += VT_MAX;
	}

	g_pVtxBuffTime->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//--------------------------
//�|���S���̏I������
//--------------------------
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//--------------------------
//�|���S���̍X�V����
//--------------------------
void UpdateTime(void)
{

}

//----------------------------
//�|���S���̕`�揈��
//----------------------------
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TIME_MAX; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTime);

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
void SetTime(int nTime, bool bTex)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	int aPosTexU[TIME_MAX];
	g_nTime = nTime;

	//�X�R�A�ݒ�
	for (int i = 0; i < TIME_MAX; i++)
	{
		aPosTexU[i] = DigitT(g_nTime, i);
	}

	if (bTex)
	{
		//���W
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

		for (int i = 0; i < TIME_MAX; i++)
		{
			//�e�N�X�`��
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);

			pVtx += VT_MAX;
		}

		g_pVtxBuffTime->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}
}

//--------------------
//����������
//--------------------
int DigitT(int nData, int nDigit)
{
	int aNumber[TIME_MAX] = { 0 };//�����p�z��
	int nData2 = 1;//�v�Z�p

	for (int i = TIME_MAX - 1; i >= 0; i--)
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