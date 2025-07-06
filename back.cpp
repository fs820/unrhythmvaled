//---------------------------------------
//
//�w�i�\������[back.cpp]
//Author fuma sato
//
//---------------------------------------

#include"back.h"
#include"scroll.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureBack = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBack = NULL;//�o�b�t�@�̃|�C���^
float g_PosTexU;//�e�N�X�`�����W(��)
float g_PosTexV;//�e�N�X�`�����W(�c)

//----------------------
//�w�i�̏���������
//----------------------
void InitBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBack,
		NULL
	);

	//1��

	//�e�N�X�`���̓ǂݍ���
	HRESULT hr=D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_BACK,
		&g_pTextureBack
	);

	//������
	g_PosTexU = 0.0f;
	g_PosTexV = 0.0f;

	g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

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
	pVtx[0].tex = D3DXVECTOR2(g_PosTexU, g_PosTexV);
	pVtx[1].tex = D3DXVECTOR2(g_PosTexU + 1.0f, g_PosTexV);
	pVtx[2].tex = D3DXVECTOR2(g_PosTexU, g_PosTexV + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_PosTexU + 1.0f, g_PosTexV + 1.0f);

	g_pVtxBuffBack->Unlock();//�o�b�t�@�̃A�����b�N
}

//-------------------
//�w�i�I������
//-------------------
void UninitBack(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBack != NULL)
	{
		g_pTextureBack->Release();
		g_pTextureBack = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBack != NULL)
	{
		g_pVtxBuffBack->Release();
		g_pVtxBuffBack = NULL;
	}
}

//-------------------
//�w�i�X�V����
//-------------------
void UpdateBack(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	float Xmove=0.0f, Ymove=0.0f;//�����Ǘ�

	//Xmove = GetScroll().x;
	//Ymove = GetScroll().y;

	//�e�N�X�`�����W�𓮂���
	//g_PosTexU += Xmove / SCREEN_WIDTH;
	//g_PosTexV += Ymove / SCREEN_HEIGHT;

	g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

	//�e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(g_PosTexU, g_PosTexV);
	pVtx[1].tex = D3DXVECTOR2(g_PosTexU + 1.0f, g_PosTexV);
	pVtx[2].tex = D3DXVECTOR2(g_PosTexU, g_PosTexV + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_PosTexU + 1.0f, g_PosTexV + 1.0f);

	g_pVtxBuffBack->Unlock();//�o�b�t�@�̃A�����b�N
}

//-------------------
//�w�i�`�揈��
//-------------------
void DrawBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffBack, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBack);

	//�w�i�̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		0,//�n�܂�̔ԍ�
		2//�|���S���̌�
	);
}