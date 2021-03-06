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

//グローバル変数
float4x4 WorldVertexPos;	//モデル座標
float4x4 WorldViewPos;		//カメラ座標
float4x4 WorldViewProj;		//変換行列
float4   Diffuse;		//ディフューズ色
bool     EnableTexture;		//テクスチャフラグ

// 頂点シェーダーの関数
void VertexShader_Main(
	in  float4 inPos      : POSITION,	//[入力] 座標(モデル空間)
	in  float4 inDiff     : COLOR0,		//[入力] 色
	in  float4 inTexture  : TEXCOORD0,	//[入力] テクスチャ座標

	out float4 outPos     : POSITION,	//[出力] 座標(射影空間)
	out float4 outDiff    : COLOR0,		//[出力] 色
	out float4 outTexture : TEXCOORD0)	//[出力] テクスチャ座標
{
	float4 VertexPos = mul(inPos, WorldVertexPos);

	outPos = mul(mul(VertexPos, WorldViewPos), WorldViewProj);	//頂点と変換行列を掛ける
	outDiff = max(inDiff, Diffuse);					//色を求める
	outTexture = inTexture;						//テクスチャ座標はそのまま
}


texture Tex;				//ピクセルシェーダーで使うテクスチャ
sampler TexSampler = sampler_state	//テクスチャ・サンプラ
{
	Texture   = (Tex);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

//ピクセルシェーダー
void PixelShader_Main(
	in  float4 inDiff     : COLOR0,		//[入力] 色
	in  float2 inTexture  : TEXCOORD0,	//[入力] テクスチャ座標

	out float4 outDiff    : COLOR0)		//[出力] 色
{
	if( EnableTexture == true ){
		outDiff = inDiff * tex2D(TexSampler, inTexture);
	}
	else{
		outDiff = inDiff;
	}
}
