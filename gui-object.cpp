//! @file gui-object.cpp
//! @brief gui-object関連クラスの定義

//--------------------------------------------------------------------------------
// 
// OpenXOPS
// Copyright (c) 2014-2022, OpenXOPS Project / [-_-;](mikan) All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of the OpenXOPS Project nor the names of its contributors 
//   may be used to endorse or promote products derived from this software 
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL OpenXOPS Project BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//--------------------------------------------------------------------------------

#include "gui-object.h"

//! @brief コンストラクタ
ScrollbarObject::ScrollbarObject()
{
	pos_x = 0;
	pos_y = 0;
	width = 0;
	height = 0;
	totalitems = 0;
	showtems = 0;
	scrollitem = 0;
	knob_height = 0.0f;
	knob_scale = 0.0f;
	knob_y = 0;
	mouse_mode = 0;
	color_body = 0x00000000;
	color1_default = 0x00000000;
	color2_default = 0x00000000;
	color1_cursor = 0x00000000;
	color2_cursor = 0x00000000;
	color1_select = 0x00000000;
	color2_select = 0x00000000;
}

//! @brief ディストラクタ
ScrollbarObject::~ScrollbarObject()
{
	//
}

//! @brief スクロールバー初期化
//! @param x x座標
//! @param y y座標
//! @param w 幅
//! @param h 高さ
//! @param in_totalitems 総アイテム数
//! @param in_showtems 画面に表示するアイテム数
//! @param in_scrollitem スクロールしたアイテム数
//! @warning サイズは幅・高さそれぞれ 8以上 に設定してください。
//! @attention SetColor()関数でスクロールバーの色を設定してください。
void ScrollbarObject::Create(int x, int y, int w, int h, int in_totalitems, int in_showtems, int in_scrollitem)
{
	pos_x = x;
	pos_y = y;
	width = w;
	height = h;
	totalitems = in_totalitems;
	showtems = in_showtems;
	scrollitem = in_scrollitem;
	if( totalitems < showtems ){
		showtems = totalitems;
		knob_height = (float)h;
		knob_scale = 1.0f;
	}
	else{
		knob_height = (float)h / totalitems * showtems;
		knob_scale = ((float)h - knob_height) / (totalitems - showtems);
	}
	knob_y = (int)Round(knob_scale * scrollitem);
	mouse_mode = 0;
}

//! @brief スクロールバーの色を設定
//! @param body 本体（後ろ側）の色
//! @param default1 通常時のつまみの色（外側）
//! @param default2 通常時のつまみの色（内側）
//! @param cursor1 カーソル選択時のつまみの色（外側）
//! @param cursor2 カーソル選択時のつまみの色（内側）
//! @param select1 クリック時のつまみの色（外側）
//! @param select2 クリック時のつまみの色（内側）
void ScrollbarObject::SetColor(int body, int default1, int default2, int cursor1, int cursor2, int select1, int select2)
{
	color_body = body;
	color1_default = default1;
	color2_default = default2;
	color1_cursor = cursor1;
	color2_cursor = cursor2;
	color1_select = select1;
	color2_select = select2;
}

//! @brief スクロールしたアイテム数設定
//! @param id アイテム数
void ScrollbarObject::SetScrollItem(int id)
{
	if( (id < 0)||(totalitems <= id) ){ return; }

	scrollitem = id;
}

//! @brief スクロールしたアイテム数取得
//! @return アイテム数
int ScrollbarObject::GetScrollItem()
{
	return scrollitem;
}

//! @brief 上方向へスクロール
void ScrollbarObject::ScrollUP()
{
	if( scrollitem > 0 ){
		scrollitem -= 1;
	}
}

//! @brief 下方向へスクロール
void ScrollbarObject::ScrollDOWN()
{
	if( scrollitem < (totalitems - showtems) ){
		scrollitem += 1;
	}
}

//! @brief スクロール中か判定
//! @return スクロール中である：true　スクロール中でない：false
bool ScrollbarObject::CheckScrolling()
{
	if( mouse_mode == 2 ){ return true; }
	return false;
}

//! @brief スクロールバー入力処理
//! @param mouse_x マウスx座標
//! @param mouse_y マウスy座標
//! @param mouse_click マウスクリックフラグ
//! @param scroll_y y方向のスクロール量
void ScrollbarObject::Input(int mouse_x, int mouse_y, bool mouse_click, int scroll_y)
{
	//mouse_mode = 0 : マウスカーソルがスクロールバーのエリア外
	//mouse_mode = 1 : マウスカーソルがスクロールバーに乗っている
	//mouse_mode = 2 : マウスカーソルがスクロールバーをクリックしている

	//スクロールバーの操作判定
	if( mouse_mode == 2 ){
		if( mouse_click == false ){
			mouse_mode = 0;	//or 1
		}
	}
	if( mouse_mode != 2 ){
		if( (pos_x < mouse_x)&&(mouse_x < (pos_x+width))
			&&(pos_y + (int)(knob_scale * scrollitem) < mouse_y)&&(mouse_y < pos_y + (int)(knob_scale * scrollitem + knob_height))
		){
			if( mouse_click == true ){
				mouse_mode = 2;
			}
			else{
				mouse_mode = 1;
			}
		}
		else{
			mouse_mode = 0;
		}
	}

	//スクロールバーの移動
	if( mouse_mode == 2 ){
		knob_y += scroll_y;
		if( knob_y < 0 ){ knob_y = 0; }
		if( knob_y > (height - (int)knob_height) ){ knob_y = (height - (int)knob_height); }

		scrollitem = (int)Round((float)knob_y / knob_scale);
		if( scrollitem < 0 ){ scrollitem = 0; }
		if( scrollitem > (totalitems - showtems) ){ scrollitem = (totalitems - showtems); }
	}
	else{
		knob_y = (int)Round(knob_scale * scrollitem);
	}
}

//! @brief スクロールバー描画処理
//! @param d3dg D3DGraphicsのポインタ
//! @attention 「合計アイテム数＜表示するアイテム数」の場合、ノブは表示されません。
void ScrollbarObject::Draw(class D3DGraphics *d3dg)
{
	if( d3dg == NULL ){ return; }

	int color, color2;

	//エリア描画
	d3dg->Draw2DBox(pos_x, pos_y, pos_x+width, pos_y+height, color_body);

	//ノブ描画
	if( totalitems > showtems ){
		//色を設定
		if( mouse_mode == 2 ){
			color = color1_select;
			color2 = color2_select;
		}
		else if( mouse_mode == 1 ){
			color = color1_cursor;
			color2 = color2_cursor;
		}
		else{
			color = color1_default;
			color2 = color2_default;
		}

		//描画
		d3dg->Draw2DBox(pos_x, pos_y+knob_y, pos_x+width, pos_y+knob_y + (int)knob_height, color);
		d3dg->Draw2DBox(pos_x+3, pos_y+knob_y+3, pos_x+width-3, pos_y+knob_y + (int)knob_height -3, color2);
	}
}
