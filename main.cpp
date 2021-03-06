//! @file main.cpp
//! @brief WinMain()関数の定義およびテストプログラム 

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

//Doxygen設定ファイル
#include "doxygen.h"

#include "main.h"

#ifdef _DEBUG
 #include <shlwapi.h>
 #pragma comment(lib, "Shlwapi.lib")
#endif

#ifdef ENABLE_DEBUGLOG
 //! ログ出力
 DebugLog OutputLog;
#endif

//! ステートマシン
StateMachine GameState;

//! ゲーム設定データ
Config GameConfig;

//! メインウィンドウ
WindowControl MainWindow;

//! @brief WinMain()関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//未使用引数対策
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	char path[MAX_PATH];

#ifdef ENABLE_AUTOLOADMIF
	bool NoBriefingFlag = false;
	int gamemode = 0;
#endif

	//乱数初期化
	InitRand();

	//実行ファイルのある場所を、カレントディレクトリにする。
	GetFileDirectory(__argv[0], path);
	chdir(path);

#ifdef _DEBUG
	//メモリリークの検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	char str[255];
	strcpy(str, "[Information]\nThe compiler is the Debug mode.\nIf release the software, Switch compiler to Release mode.");
	MessageBox(NULL, str, GAMENAME, MB_OK);
#endif

	//引数処理
	//メモ：Windows環境なら、lpCmdLine引数とかGetCommandLine()関数でも取れますけどね・・。
	strcpy(path, "");
	if( __argc > 1 ){
		for(int param=1; param<__argc; param++){
#ifdef ENABLE_DEBUGLOG
			//"Log"が与えられていたら、ログ出力を有効化
			if( CheckCommandParameter(__argv[param], "-Log") == true ){
				//ログ出力を有効化
				OutputLog.EnableOutputLog();

				MainWindow.ErrorInfo("Enable Debug Log...");
				continue;
			}
#endif

#ifdef ENABLE_AUTOLOADMIF
			//"Dir"が与えられていたら、カレントディレクトリ移動
			if( CheckCommandParameter(__argv[param], "-Dir") == true ){
				if( (param+1) < __argc ){
					chdir(__argv[param+1]);
					param += 1;
				}
				continue;
			}

			//"NoBriefing"が与えられていたら、（.mif指定時は）ブリーフィングを省略する
			if( CheckCommandParameter(__argv[param], "-NoBriefing") == true ){
				NoBriefingFlag = true;
				continue;
			}

			//それ以外の引数ならば拡張子を判定してみる
			if( CheckFileExtension(__argv[param], ".mif") == true ){
				//.mifであればファイルパスとして記憶
				strcpy(path, __argv[param]);
			}
#endif
		}

#ifdef ENABLE_AUTOLOADMIF
		//何かしらの.mifが指定されているならば、ブリーフィングフラグ反映
		if( path[0] != '\0' ){
			if( NoBriefingFlag == false ){ gamemode = 1; }
			else{ gamemode = 2; }
		}
#endif
	}

#ifdef ENABLE_DEBUGLOG
	//ファイル作成
	OutputLog.MakeLog();

	//ユーザー環境を出力
	GetOperatingEnvironment();

	char infostr[64];

	/*
	//ログに出力
	sprintf(infostr, "Total Parameters : %d", __argc);
	OutputLog.WriteLog(LOG_CHECK, "Parameter", infostr);
	for(int i=0; i<__argc; i++){
		sprintf(infostr, "Parameter[%d]", i);
		OutputLog.WriteLog(LOG_CHECK, infostr, __argv[i]);
	}
	*/

	//ログに出力
	OutputLog.WriteLog(LOG_CHECK, "Startup", "Start program entry point");
#endif

#ifdef _DEBUG
	//ログに出力
	OutputLog.WriteLog(LOG_CHECK, "Startup", "Visual C++ debug mode");
#endif

	//設定ファイル読み込み
	if( GameConfig.LoadFile("config.dat") == 1 ){
#ifndef ENABLE_AUTOCREATECONFIG
		MainWindow.ErrorInfo("config data open failed");
		return 1;
#else
		//デフォルト値を反映し、設定ファイルに保存する
		GameConfig.SetDefaultConfig();
		if( GameConfig.SaveFile("config.dat") == 1 ){
			MainWindow.ErrorInfo("config data save failed");
			return 1;
		}
#endif
	}

	//追加設定を読み込む
#ifdef ENABLE_ADDCONFIG
	GameConfig.CreateExtConfig("config-openxops.ini");
	GameConfig.LoadExtFile("config-openxops.ini");
#else
	GameConfig.LoadExtFile(NULL);
#endif

#ifdef ENABLE_DEBUGLOG
	//ログに出力
	sprintf(infostr, "Display resolution : %d x %d", GameConfig.GetScreenWidth(), GameConfig.GetScreenHeight());
	OutputLog.WriteLog(LOG_CHECK, "Environment", infostr);
	if( GameConfig.GetFullscreenFlag() == false ){
		OutputLog.WriteLog(LOG_CHECK, "Environment", "Window mode : Window");
	}
	else{
		OutputLog.WriteLog(LOG_CHECK, "Environment", "Window mode : Fullscreen");
	}
#endif

	//ウィンドウ初期化
	MainWindow.SetParam(hPrevInstance, nCmdShow);
	MainWindow.InitWindow(GAMENAME, GameConfig.GetScreenWidth(), GameConfig.GetScreenHeight(), GameConfig.GetFullscreenFlag());

	//基本的な初期化処理
#ifdef ENABLE_AUTOLOADMIF
	if( InitGame(&MainWindow, gamemode, path) ){
#else
	if( InitGame(&MainWindow, 0, "") ){
#endif
		return 1;
	}

	opening Opening;
	mainmenu MainMenu;
	briefing Briefing;
	maingame MainGame;
	result Result;
	InitScreen(&MainWindow, &Opening, &MainMenu, &Briefing, &MainGame, &Result);

#ifdef ENABLE_AUTOLOADMIF
	//メインゲームのみ表示するモードならば、標準でデバック表示を有効にする。
	if( gamemode == 2 ){ MainGame.SetShowInfoFlag(true); }
#endif


#ifdef ENABLE_DEBUGLOG
	//ログに出力
	OutputLog.WriteLog(LOG_CHECK, "Startup", "Start main loop");
#endif

	unsigned int framecnt = 0;

	for(int flag = 0; flag != -1; flag = MainWindow.CheckMainLoop()){
		if( flag == 1 ){
			//メイン処理
			ProcessScreen(&MainWindow, &Opening, &MainMenu, &Briefing, &MainGame, &Result, framecnt);

			//FPS調整
			ControlFps();

			framecnt++;
		}
	}

#ifdef ENABLE_DEBUGLOG
	//ログに出力
	OutputLog.WriteLog(LOG_CHECK, "Exit", "Exit main loop");
#endif


	//基本的な解放処理
	CleanupGame();

#ifdef ENABLE_DEBUGLOG
	//ログに出力
	OutputLog.WriteLog(LOG_CHECK, "Exit", "Exit program entry point");
#endif
	return 0;
}

//! @brief コマンド引数判定
//! @param param 元の文字列
//! @param cmd 判定する引数
//! @return 該当：true　非該当：false
//! @warning cmd引数の最初の一文字は無視されます。
//! @attention 次の引数を判定します。＞/[引数]、-[引数]、/[引数（小文字）]、-[引数（小文字）]
bool CheckCommandParameter(const char *param, const char *cmd)
{
	if( param == NULL ){ return false; }
	if( cmd == NULL ){ return false; }
	if( strlen(param) >= 16 ){ return false; }
	if( strlen(cmd) >= 16 ){ return false; }
	if( strlen(param) != strlen(cmd) ){ return false; }

	char str[16];
	strcpy(str, cmd);

	str[0] = '/';
	if( strcmp(param, str) == 0 ){ return true; }
	str[0] = '-';
	if( strcmp(param, str) == 0 ){ return true; }

	for(int i=0; i<(int)strlen(str); i++){ str[i] = (char)tolower((int)(str[i])); }

	str[0] = '/';
	if( strcmp(param, str) == 0 ){ return true; }
	str[0] = '-';
	if( strcmp(param, str) == 0 ){ return true; }

	return false;
}

//! @brief 角度（ラジアン）正規化
//! @param r 角度
//! @return 正規化後の角度
//! @note -PI〜PI の間に正規化します。
float AngleNormalization(float r)
{
	for(; r > (float)M_PI;    r -= (float)M_PI*2){}
	for(; r < (float)M_PI*-1; r += (float)M_PI*2){}
	return r;
}