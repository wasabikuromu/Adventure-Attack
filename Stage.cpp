#include <DxLib.h>
#include <fstream>
#include <string>
#include <sstream>
#include "Application.h"
#include "AsoUtility.h"
#include "SceneGame.h"
#include "Stage.h"

//デフォルトコンストラクタ
Stage::Stage(void)
{
	sceneGame_ = nullptr;

	for (int i = 0; i < NUM_MAP_CHIPS; i++)
	{
		imgMapChips_[i] = 0;
	}


	/*for (int y = 0; y < 40; y++)
	{
		for (int x = 0; x < 50; x++)
		{
			groundMap_[y][x] = 0;
		}
	}*/
}

Stage::~Stage(void)
{

}

//初期化処理(初回の１度のみ実行される)
bool Stage::Init(SceneGame* parent)
{

	sceneGame_ = parent;

	//マップチップの画像読み込み 
	int ret;

	ret = LoadDivGraph("Image/Stage/Map2.png"
		, NUM_MAP_CHIPS
		, NUM_MAP_CHIPS_X
		, NUM_MAP_CHIPS_Y
		, CHIP_SIZE_X
		, CHIP_SIZE_Y
		, &(imgMapChips_[0])
	);

	if (ret == -1)
	{
		OutputDebugString("マップチップ2の読み込み失敗\n");
		return false;
	}

	//外部ファイルより地上マップデータを読み込む	
	LoadGroundCsvData();

	//外部ファイルより地下マップデータを読み込む
	LoadUnGroundCsvData();

	//初期で使うマップを指定
	ChangeMap(TYPE::GROUND);

	return true;
}

//更新処理(毎フレーム実行)
void Stage::Update(void)
{
	
}

//描画処理(毎フレーム実行)
void Stage::Draw(void)
{
	Vector2 cameraPos = sceneGame_->GetCameraPos();

	//マップの描画
	for (int y = 0; y < usedMapSize_.y; y++)
	{
		for (int x = 0; x < usedMapSize_.x; x++)
		{
			//利用マップのマップチップを取得
			int mapChipNO = usedMap_[y][x];

			//チップNOを使ってマップチップを描画
			DrawGraph(CHIP_SIZE_X * x - cameraPos.x, CHIP_SIZE_Y * y - cameraPos.y
				, imgMapChips_[mapChipNO], true);

			//マップ枠の描画
			int sx = CHIP_SIZE_X * x - cameraPos.x;
			int sy = CHIP_SIZE_Y * y - cameraPos.y;
			int ex = sx + CHIP_SIZE_X;
			int ey = sy + CHIP_SIZE_Y;
			
			//DrawBox(sx, sy, ex, ey, 0xFFFFFF, false);
			//SetFontSize(8);
			//DrawFormatString(sx, sy,0xFF0000, "%d,%d", x, y);
			//DrawFormatString(sx, sy + 5,0xFF0000, "%d", mapChipNO);
		}
	}
}

//解放処理(終了時の一度のみ実行)
bool Stage::Release(void)
{
	//マップチップの解放
	for (int i = 0; i < NUM_MAP_CHIPS; i++)
	{
		DeleteGraph(imgMapChips_[i]);
	}

	return true;
}

//外部ファイルから地上ステージCSVデータを読み込む
void Stage::LoadGroundCsvData(void)
{
	//ファイルの準備
	//std::ifstream ifs = std::ifstream("Data/Map2.csv");
	//if (!ifs)
	//{
	//	//ファイル操作のifsがエラー
	//	OutputDebugString("地上ステージのifstream準備失敗\n");
	//	return;
	//}

	std::ifstream ifs = std::ifstream("Data/Map3.csv");
	if (!ifs)
	{
		//ファイル操作のifsがエラー
		OutputDebugString("地上ステージのifstream準備失敗\n");
		return;
	}

	//二次元配列の場所を指定用
	int x = 0;
	int y = 0;

	//CSVファイルの1行毎に読み込む処理
	std::string line;		//一行のデータを格納する領域
	while (getline(ifs, line))
	{	
		//Split関数の戻り値受け取り用変数
		std::vector <std::string> strSplit;
		strSplit = AsoUtility::Split(line, ',');

		//マップチップのNoを格納する領域
		int chipNo;

		//分割したデータをマップ配列に格納する
		for (int x = 0; x < strSplit.size(); x++)
		{
			//文字列を数値に変換する
			chipNo = stoi(strSplit[x]);

			//マップチップのNoをマップ配列に格納する
			groundMap_[y][x] = chipNo;
		}

		y++;		//行の処理が終わったので次の行に移動
	}
}

void Stage::LoadUnGroundCsvData(void)
{
	std::ifstream ifs = std::ifstream("Data/Map4.csv");
	if (!ifs)
	{
		//ファイル操作のifsがエラー
		OutputDebugString("地上ステージのifstream準備失敗\n");
		return;
	}

	//二次元配列の場所を指定用
	int x = 0;
	int y = 0;

	//CSVファイルの1行毎に読み込む処理
	std::string line;		//一行のデータを格納する領域
	while (getline(ifs, line))
	{
		//Split関数の戻り値受け取り用変数
		std::vector <std::string> strSplit;
		strSplit = AsoUtility::Split(line, ',');

		//マップチップのNoを格納する領域
		int chipNo;

		//分割したデータをマップ配列に格納する
		for (int x = 0; x < strSplit.size(); x++)
		{
			//文字列を数値に変換する
			chipNo = stoi(strSplit[x]);

			//マップチップのNoをマップ配列に格納する
			unGroundMap_[y][x] = chipNo;
		}

		y++;		//行の処理が終わったので次の行に移動
	}
}



//マップチップ番号を返す
int Stage::GetChipNo(Vector2 mapPos)
{
	return usedMap_[mapPos.y][mapPos.x];
}

//マップサイズを返す
Vector2 Stage:: GetUsedMapSize(void)
{
	return usedMapSize_;
}

bool Stage::IsWarp(Vector2 mapPos)
{
	int chipNo = GetChipNo(mapPos);

	switch (chipNo)
	{
	case 6:
	case 7:
	case 8:
	case 24:
	case 25:
	case 26:

		return true;

	default:
		break;
	}

	return false;
}

bool Stage::IsGoal(Vector2 mapPos)
{
	int chipNo = GetChipNo(mapPos);

	switch (chipNo)
	{
	case 84:
	case 85:
	case 86:
	case 102:
	case 103:
	case 104:

		return true;

	default:
		break;
	}

	return false;
}


//マップ種別を返す
Stage::TYPE Stage::GetMapType(void)
{
	return type_;
}

//処理で利用するマップ配列を初期化する
void Stage::ClearUsedMap(void)
{
	//二重ループを利用してusedMap_[][]配列の中をキレイ(-1)にする。
	for (int y = 0; y < MAP_MAX_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_MAX_SIZE_Y; x++)
		{
			usedMap_[y][x] = -1;
		}
	}
}

//マップを変更する
void Stage::ChangeMap(TYPE value)
{
	//処理で利用するマップ配列を初期化する
	ClearUsedMap();

	if (value == TYPE::GROUND)
	{
		type_ = TYPE::GROUND;	//処理で利用するマップを地上マップ

		//地上マップのサイズを
		//処理で利用する格納したマップのサイズに設定する
		usedMapSize_ = Vector2(MAP_GROUND_SIZE_X, MAP_GROUND_SIZE_Y);

		//二重ループを利用して
		//usedMap_[][]の中にgroundMap_[][]の中身をコピーする

		for (int y = 0; y < usedMapSize_.y; y++)
		{
			for (int x = 0; x < usedMapSize_.x; x++)
			{
				usedMap_[y][x] = groundMap_[y][x];
			}
		}
	}
	else if (value == TYPE::UN_GROUND)
	{
		type_ = TYPE::UN_GROUND;	//処理で利用するマップを地上マップ

		//地下マップのサイズを
		//処理で利用する格納したマップのサイズに設定する
		usedMapSize_ = Vector2(UN_MAP_GROUND_SIZE_X, UN_MAP_GROUND_SIZE_Y);

		//二重ループを利用して
		//usedMap_[][]の中にgroundMap_[][]の中身をコピーする

		for (int y = 0; y < usedMapSize_.y; y++)
		{
			for (int x = 0; x < usedMapSize_.x; x++)
			{
				usedMap_[y][x] = unGroundMap_[y][x];
			}
		}

		
	}
	
}