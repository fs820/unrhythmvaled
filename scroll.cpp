//---------------------------------------
//
//�X�N���[������[scroll.cpp]
//Author fuma sato
//
//---------------------------------------

#include"scroll.h"

D3DXVECTOR3 g_Scroll = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//------------------
//�ݒ�
//------------------
void SetScroll(D3DXVECTOR3 Scroll)
{
	g_Scroll = Scroll;
}
//--------------------------
//�擾
//--------------------------
D3DXVECTOR3 GetScroll(void)
{
	return g_Scroll;
}