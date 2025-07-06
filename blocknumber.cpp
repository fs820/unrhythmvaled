//---------------------------------------
//
//�w�i�\������[BlockNumber.cpp]
//Author fuma sato
//
//---------------------------------------

#include"blocknumber.h"
#include"block.h"
#include"time.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureBlockNumber = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlockNumber = NULL;//�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posNumber;

//----------------------
//�w�i�̏���������
//----------------------
void InitBlockNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_BLOCK * NUM_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlockNumber,
		NULL
	);

	//1��

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NUMBER,
		&g_pTextureBlockNumber
	);

	//������
	g_posNumber = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_pVtxBuffBlockNumber->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

	for (int i = 0; i < MAX_BLOCK * NUM_MAX; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posNumber.x - (((TIME_WIDTH * SIZE_INA) / 10.0f) / U_MAX_S) / 2, g_posNumber.y - ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
		pVtx[1].pos = D3DXVECTOR3(g_posNumber.x + (((TIME_WIDTH * SIZE_INA) / 10.0f) / U_MAX_S) / 2, g_posNumber.y - ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
		pVtx[2].pos = D3DXVECTOR3(g_posNumber.x - (((TIME_WIDTH * SIZE_INA) / 10.0f) / U_MAX_S) / 2, g_posNumber.y + ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
		pVtx[3].pos = D3DXVECTOR3(g_posNumber.x + (((TIME_WIDTH * SIZE_INA) / 10.0f) / U_MAX_S) / 2, g_posNumber.y + ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX_S);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S, UV_DEF / V_MAX_S);

		pVtx += VT_MAX;
	}

	g_pVtxBuffBlockNumber->Unlock();//�o�b�t�@�̃A�����b�N
}

//-------------------
//�w�i�I������
//-------------------
void UninitBlockNumber(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBlockNumber != NULL)
	{
		g_pTextureBlockNumber->Release();
		g_pTextureBlockNumber = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBlockNumber != NULL)
	{
		g_pVtxBuffBlockNumber->Release();
		g_pVtxBuffBlockNumber = NULL;
	}
}

//-------------------
//�w�i�X�V����
//-------------------
void UpdateBlockNumber(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	Block* pBlock;
	pBlock = GetBlock();
	float TexU = 0.0f;

	g_pVtxBuffBlockNumber->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

	for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
	{
		if (pBlock->bUse)
		{
			g_posNumber = pBlock->pos;
			for (int i2 = 0; i2 < NUM_MAX; i2++)
			{
				TexU = (float)DigitBN(i, i2);
				//���W�ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_posNumber.x - ((TIME_WIDTH * SIZE_INA) / U_MAX_S) / 2 + (i2 - 1) * ((TIME_WIDTH * SIZE_INA) / U_MAX_S), g_posNumber.y - ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
				pVtx[1].pos = D3DXVECTOR3(g_posNumber.x + ((TIME_WIDTH * SIZE_INA) / U_MAX_S) / 2 + (i2 - 1) * ((TIME_WIDTH * SIZE_INA) / U_MAX_S), g_posNumber.y - ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
				pVtx[2].pos = D3DXVECTOR3(g_posNumber.x - ((TIME_WIDTH * SIZE_INA) / U_MAX_S) / 2 + (i2 - 1) * ((TIME_WIDTH * SIZE_INA) / U_MAX_S), g_posNumber.y + ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
				pVtx[3].pos = D3DXVECTOR3(g_posNumber.x + ((TIME_WIDTH * SIZE_INA) / U_MAX_S) / 2 + (i2 - 1) * ((TIME_WIDTH * SIZE_INA) / U_MAX_S), g_posNumber.y + ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);

				//�e�N�X�`��
				pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * TexU, UV_DEF / V_MAX_S);
				pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * TexU + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S);
				pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * TexU, UV_DEF / V_MAX_S + UV_DEF / V_MAX_S);
				pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * TexU + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S + UV_DEF / V_MAX_S);

				pVtx += VT_MAX;
			}
		}
		else
		{
			pVtx += VT_MAX * NUM_MAX;
		}
	}

	g_pVtxBuffBlockNumber->Unlock();//�o�b�t�@�̃A�����b�N
}

//-------------------
//�w�i�`�揈��
//-------------------
void DrawBlockNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	Block* pBlock;

	pBlock = GetBlock();
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffBlockNumber, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBlockNumber);

	for (int i = 0; i < MAX_BLOCK * NUM_MAX; i++)
	{
		if (pBlock->bUse)
		{
			//�w�i�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				i * VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}

		if (i % NUM_MAX == 0)
		{
			pBlock++;
		}
	}
}

//--------------------
//����������
//--------------------
int DigitBN(int nData, int nDigit)
{
	int aNumber[NUM_MAX] = { 0 };//�����p�z��
	int nData2 = 1;//�v�Z�p

	for (int i = NUM_MAX - 1; i >= 0; i--)
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