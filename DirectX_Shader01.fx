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

//�O���[�o���ϐ�
float4x4 WorldVertexPos;	//���f�����W
float4x4 WorldViewPos;		//�J�������W
float4x4 WorldViewProj;		//�ϊ��s��
float4   Diffuse;		//�f�B�t���[�Y�F
bool     EnableTexture;		//�e�N�X�`���t���O

// ���_�V�F�[�_�[�̊֐�
void VertexShader_Main(
	in  float4 inPos      : POSITION,	//[����] ���W(���f�����)
	in  float4 inDiff     : COLOR0,		//[����] �F
	in  float4 inTexture  : TEXCOORD0,	//[����] �e�N�X�`�����W

	out float4 outPos     : POSITION,	//[�o��] ���W(�ˉe���)
	out float4 outDiff    : COLOR0,		//[�o��] �F
	out float4 outTexture : TEXCOORD0)	//[�o��] �e�N�X�`�����W
{
	float4 VertexPos = mul(inPos, WorldVertexPos);

	outPos = mul(mul(VertexPos, WorldViewPos), WorldViewProj);	//���_�ƕϊ��s����|����
	outDiff = max(inDiff, Diffuse);					//�F�����߂�
	outTexture = inTexture;						//�e�N�X�`�����W�͂��̂܂�
}


texture Tex;				//�s�N�Z���V�F�[�_�[�Ŏg���e�N�X�`��
sampler TexSampler = sampler_state	//�e�N�X�`���E�T���v��
{
	Texture   = (Tex);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

//�s�N�Z���V�F�[�_�[
void PixelShader_Main(
	in  float4 inDiff     : COLOR0,		//[����] �F
	in  float2 inTexture  : TEXCOORD0,	//[����] �e�N�X�`�����W

	out float4 outDiff    : COLOR0)		//[�o��] �F
{
	if( EnableTexture == true ){
		outDiff = inDiff * tex2D(TexSampler, inTexture);
	}
	else{
		outDiff = inDiff;
	}
}