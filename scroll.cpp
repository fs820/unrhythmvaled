//---------------------------------------
//
//スクロール処理[scroll.cpp]
//Author fuma sato
//
//---------------------------------------

#include"scroll.h"

D3DXVECTOR3 g_Scroll = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//------------------
//設定
//------------------
void SetScroll(D3DXVECTOR3 Scroll)
{
	g_Scroll = Scroll;
}
//--------------------------
//取得
//--------------------------
D3DXVECTOR3 GetScroll(void)
{
	return g_Scroll;
}