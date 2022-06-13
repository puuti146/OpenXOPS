//! @file gui-object.cpp
//! @brief gui-object�֘A�N���X�̒�`

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

//! @brief �R���X�g���N�^
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

//! @brief �f�B�X�g���N�^
ScrollbarObject::~ScrollbarObject()
{
	//
}

//! @brief �X�N���[���o�[������
//! @param x x���W
//! @param y y���W
//! @param w ��
//! @param h ����
//! @param in_totalitems ���A�C�e����
//! @param in_showtems ��ʂɕ\������A�C�e����
//! @param in_scrollitem �X�N���[�������A�C�e����
//! @warning �T�C�Y�͕��E�������ꂼ�� 8�ȏ� �ɐݒ肵�Ă��������B
//! @attention SetColor()�֐��ŃX�N���[���o�[�̐F��ݒ肵�Ă��������B
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

//! @brief �X�N���[���o�[�̐F��ݒ�
//! @param body �{�́i��둤�j�̐F
//! @param default1 �ʏ펞�̂܂݂̐F�i�O���j
//! @param default2 �ʏ펞�̂܂݂̐F�i�����j
//! @param cursor1 �J�[�\���I�����̂܂݂̐F�i�O���j
//! @param cursor2 �J�[�\���I�����̂܂݂̐F�i�����j
//! @param select1 �N���b�N���̂܂݂̐F�i�O���j
//! @param select2 �N���b�N���̂܂݂̐F�i�����j
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

//! @brief �X�N���[�������A�C�e�����ݒ�
//! @param id �A�C�e����
void ScrollbarObject::SetScrollItem(int id)
{
	if( (id < 0)||(totalitems <= id) ){ return; }

	scrollitem = id;
}

//! @brief �X�N���[�������A�C�e�����擾
//! @return �A�C�e����
int ScrollbarObject::GetScrollItem()
{
	return scrollitem;
}

//! @brief ������փX�N���[��
void ScrollbarObject::ScrollUP()
{
	if( scrollitem > 0 ){
		scrollitem -= 1;
	}
}

//! @brief �������փX�N���[��
void ScrollbarObject::ScrollDOWN()
{
	if( scrollitem < (totalitems - showtems) ){
		scrollitem += 1;
	}
}

//! @brief �X�N���[����������
//! @return �X�N���[�����ł���Ftrue�@�X�N���[�����łȂ��Ffalse
bool ScrollbarObject::CheckScrolling()
{
	if( mouse_mode == 2 ){ return true; }
	return false;
}

//! @brief �X�N���[���o�[���͏���
//! @param mouse_x �}�E�Xx���W
//! @param mouse_y �}�E�Xy���W
//! @param mouse_click �}�E�X�N���b�N�t���O
//! @param scroll_y y�����̃X�N���[����
void ScrollbarObject::Input(int mouse_x, int mouse_y, bool mouse_click, int scroll_y)
{
	//mouse_mode = 0 : �}�E�X�J�[�\�����X�N���[���o�[�̃G���A�O
	//mouse_mode = 1 : �}�E�X�J�[�\�����X�N���[���o�[�ɏ���Ă���
	//mouse_mode = 2 : �}�E�X�J�[�\�����X�N���[���o�[���N���b�N���Ă���

	//�X�N���[���o�[�̑��씻��
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

	//�X�N���[���o�[�̈ړ�
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

//! @brief �X�N���[���o�[�`�揈��
//! @param d3dg D3DGraphics�̃|�C���^
//! @attention �u���v�A�C�e�������\������A�C�e�����v�̏ꍇ�A�m�u�͕\������܂���B
void ScrollbarObject::Draw(class D3DGraphics *d3dg)
{
	if( d3dg == NULL ){ return; }

	int color, color2;

	//�G���A�`��
	d3dg->Draw2DBox(pos_x, pos_y, pos_x+width, pos_y+height, color_body);

	//�m�u�`��
	if( totalitems > showtems ){
		//�F��ݒ�
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

		//�`��
		d3dg->Draw2DBox(pos_x, pos_y+knob_y, pos_x+width, pos_y+knob_y + (int)knob_height, color);
		d3dg->Draw2DBox(pos_x+3, pos_y+knob_y+3, pos_x+width-3, pos_y+knob_y + (int)knob_height -3, color2);
	}
}
