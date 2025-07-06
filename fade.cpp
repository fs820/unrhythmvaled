//------------------------------------------
//
//�t�F�[�h����[fade.cpp]
//Author fuma sato
//
//------------------------------------------
#include"fade.h"

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;//�o�b�t�@�̃|�C���^
FADE g_fade;
MODE g_modeNext;
D3DXCOLOR g_coloFade;

//-------------
//����������
//-------------
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL
	);

	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_coloFade = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);

	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

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
	pVtx[0].col = g_coloFade;
	pVtx[1].col = g_coloFade;
	pVtx[2].col = g_coloFade;
	pVtx[3].col = g_coloFade;

	g_pVtxBuffFade->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

	SetMode(g_modeNext);
}

//-----------------
//�I������
//-----------------
void UninitFade(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//-----------------
//�X�V����
//-----------------
void UpdateFade(void)
{
	if (g_fade!=FADE_NONE)
	{
		if (g_fade==FADE_IN)
		{
			g_coloFade.a -= 0.01f;
			if (g_coloFade.a<=0.0f)
			{
				g_coloFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if (g_fade==FADE_OUT)
		{
			g_coloFade.a += 0.01f;
			if (g_coloFade.a>=1.0f)
			{
				g_coloFade.a = 1.0f;
				g_fade = FADE_IN;

				//���[�h�ؑ�
				SetMode(g_modeNext);
			}
		}
		VERTEX_2D* pVtx;//���_���|�C���^

		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N
		//�J���[
		pVtx[0].col = g_coloFade;
		pVtx[1].col = g_coloFade;
		pVtx[2].col = g_coloFade;
		pVtx[3].col = g_coloFade;

		g_pVtxBuffFade->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}
}

//----------------
//�`�揈��
//----------------
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�w�i�̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		0,//�n�܂�̔ԍ�
		2//�|���S���̌�
	);
}

//-----------------
//�t�F�[�h�ݒ�
//-----------------
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;
	g_modeNext = modeNext;
	g_coloFade = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
}

//-----------------
//�t�F�[�h�ݒ�
//-----------------
void SetFade2(MODE modeNext)
{
	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_coloFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
}

//------------------
//�t�F�[�h�擾
//------------------
FADE GetFade(void)
{
	return g_fade;
}