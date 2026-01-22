//---------------------------------------------
// ゲーム課題:課題6
// 作成者：2316023 中山拓海
//---------------------------------------------

#include "DxLib.h"
#include "Application.h"

//WinMain
//--------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application::CreateInstance();

	Application::GetInstance().Run();

	Application::GetInstance().Release();

	return 0;		//システムの終了処理
}
