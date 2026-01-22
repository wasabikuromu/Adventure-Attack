#include <DxLib.h>
#include "Vector2.h"
#include "Vector2F.h"
#include "Application.h"
#include "SceneManager.h"
#include "Stage.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EnemyMob1.h"
#include "EnemyWizard.h"
#include "EnemyBoss.h"
#include "EnemyGimp.h"
#include "Shot.h"
#include "SceneGame.h"

//デフォルトコンストラクタ
//SceneGame::SceneGame(void)
//{
//
//}

////デストラクタ
//SceneGame::~SceneGame(void)
//{
//
//}

//初期化処理
bool SceneGame::Init(void)
{
	bgImage1_ = LoadGraph("Image/UI/Back.png");
	if (bgImage1_ == -1)
	{
		return -1;
	}

	bgImage2_ = LoadGraph("Image/UI/Back2.png");
	if (bgImage2_ == -1)
	{
		return -1;
	}

	//カメラの位置の初期化
	cameraPos_.x = 0;
	cameraPos_.y = 160;

	enCounter = 0;

	damagedCnt_ = 0;

	//ステージを生成
	stage_ = new Stage();
	if (stage_->Init(this) == false)
	{
		OutputDebugString("SceneGame Init Stageで エラー\n");
		return false;
	}

	//プレイヤーを生成
	player_ = new Player();
	if (player_->Init(this) == false)
	{
		OutputDebugString("SceneGame Init Player でエラー\n");
		return false;
	}

	soundHandle0_ = LoadSoundMem("Image/Sound/Bgm.mp3");		//BGMを鳴らす処理
	soundHandle1_ = LoadSoundMem("Image/Sound/Bgm.mp3");		//BGMを鳴らす処理
	soundHandle2_ = LoadSoundMem("Image/Sound/Warp.mp3");		//発射音を鳴らす処理
	soundHandle3_ = LoadSoundMem("Image/Sound/Damage.mp3");		//BGMを鳴らす処理
	soundHandle4_ = LoadSoundMem("Image/Sound/GameOver.mp3");	//落下音を鳴らす処理

	Vector2 playerPos = player_->GetPos().ToVector2();
	Vector2 mapPos = WorldPos2MapPos(playerPos);

	//現在のマップ種別を所得する
	Stage::TYPE nowMapType = stage_->GetMapType();

	//プレイヤーの現在の座標取得
	Vector2F NowPlayerPos = player_->GetPos();

	if (nowMapType == Stage::TYPE::GROUND)
	{
		CreateEnemy();

		ChangeVolumeSoundMem(255 * 40 / 100, soundHandle0_);		//音量の設定
		PlaySoundMem(soundHandle0_, DX_PLAYTYPE_LOOP, true);		//BGMを再生

		if (stage_->IsGoal(mapPos) == true)
		{
			StopSoundMem(soundHandle0_);
		}
	}

	if (nowMapType == Stage::TYPE::UN_GROUND)
	{
		ChangeVolumeSoundMem(255 * 40 / 100, soundHandle1_);		//音量の設定
		PlaySoundMem(soundHandle1_, DX_PLAYTYPE_LOOP, true);		//BGMを再生

		if (stage_->IsGoal(mapPos) == true)
		{
			StopSoundMem(soundHandle1_);
		}
	}

	//ゲームオーバー
	gameOverFlag_ = false;

	//開始状態を設定する
	//マップの選定
	stage_->ChangeMap(Stage::TYPE::GROUND);

	return true;
}

//更新処理
void SceneGame::Update(void)
{
	//カメラの操作
	CameraMove();

	stage_->Update();
	player_->Update();

	//敵の更新処理
	size_t size = enemys_.size();
	for (int i = 0; i < size; i++)
	{
		if (player_->GetState() != Player::STATE::ATK)
		{
			enemys_[i]->ChengeIsFirst();
		}
		enemys_[i]->Update();
	}

	//イベント処理
	//----------------------------------------
	//プレイヤーの座標(ワールド座標)からマップ配列の座標に
	Vector2 playerPos = player_->GetPos().ToVector2();
	Vector2 mapPos = WorldPos2MapPos(playerPos);

	//Player::ANIM_STATE nowAnimState = Player::ANIM_STATE::RUN;

	//現在のマップ種別を所得する
	Stage::TYPE nowMapType = stage_->GetMapType();

	//地上マップの時
	if (nowMapType == Stage::TYPE::GROUND)
	{
		//ワープに乗っているかの確認
		if (stage_->IsWarp(mapPos) == true)
		{
			//地下へ移動
			stage_->ChangeMap(Stage::TYPE::UN_GROUND);

			PlaySoundMem(soundHandle2_, DX_PLAYTYPE_BACK, true);		//発射音を再生

			//プレイヤーの位置を設定
			player_->SetPos(Vector2(192.0f, 614.0f).ToVector2F());

			for (EnemyBase* enemy : enemys_)
			{
				enemy->Release();
				delete enemy;
			}
			enemys_.clear();

			CreateEnemy1();

			size = enemys_.size();
		}
	}

	//ゴールしたのかの処理
	if (stage_->IsGoal(mapPos) == true)
	{
		//現在のマップ種別を所得する
		Stage::TYPE nowMapType = stage_->GetMapType();

		//ゲームクリア
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::GAMECLEAR
			, true
		);
	}

	//プレイヤーの現在の座標取得
	Vector2F NowPlayerPos = player_->GetPos();
	//落下死判定
	if (NowPlayerPos.y > PLYAER_DIE_POINT)
	{
		gameOverFlag_ = true;

		ChangeVolumeSoundMem(255 * 75 / 100, soundHandle4_);			//音量の設定
		PlaySoundMem(soundHandle4_, DX_PLAYTYPE_NORMAL, true);			//落下音を再生

		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::GAMEOVER
			, true);

		player_->Damage(15);
	}

	//当たり判定
	//---------------------------
	if (player_->GetState() != Player::STATE::DAMAGED)
	{
		//複数の敵に対して当たり判定を行う
		for (int en = 0; en < size; en++)	//sizeは
		{
			//敵が生きているときは当たり判定を行う
			if (enemys_[en]->IsAlive() == true)
			{
				//プレイヤーと敵との当たり判定
				//-----------------------------------
				//プレイヤーがやられたときは、ゲームオーバー
				//敵が生きている かつ プレイヤーが生きている

				Vector2 enemyColPos = enemys_[en]->GetPos().ToVector2();
				Vector2 enemyColLocalPos = enemys_[en]->GetHitPos();
				enemyColPos.x += enemyColLocalPos.x;
				enemyColPos.y += enemyColLocalPos.y;

				Vector2 playerColPos = player_->GetPos().ToVector2();
				Vector2 playerColLocalPos = player_->GetHitPos();
				playerColPos.x += playerColLocalPos.x;
				playerColPos.y += playerColLocalPos.y;

				if (IsCollisionRectCenter(
					playerColPos							//プレイヤー中央座標 // 修正
					, player_->GetHitBox()					//プレイヤーのサイズ 
					, enemyColPos							//敵の中央座標
					, enemys_[en]->GetHitBox()				//敵のサイズ
				) == true)
				{
					//当たりの場合
					//-------------------------------
					//プレイヤーの体力を減らしましょう
					player_->Damage(1);
					enemys_[en]->KnockBack();

					PlaySoundMem(soundHandle3_, DX_PLAYTYPE_BACK, true);		//BGMを再生

					return;
				}


				if (IsCollisionRectCenter(
					playerColPos							//プレイヤー中央座標 // 修正
					, player_->GetHitBox()					//プレイヤーのサイズ 
					, enemyColPos							//弾の中央座標
					, enemys_[en]->GetHitBox()				//弾のサイズ
				) == true)
				{
					//当たりの場合
					//-------------------------------
					//プレイヤーの体力を減らしましょう
					player_->Damage(1);

					soundHandle3_ = LoadSoundMem("Image/Sound/Damage.mp3");		//BGMを鳴らす処理
					PlaySoundMem(soundHandle3_, DX_PLAYTYPE_BACK, true);		//BGMを再生

					return;
				}

				//プレイヤー攻撃と敵との当たり判定
				//-----------------------------------
				//プレイヤー弾が存在するときは当たり判定を行う
				//敵が生きている かつ プレイヤー弾が生きる

				Vector2 pRAtkColPos = player_->GetPos().ToVector2();
				Vector2 pRAtkColLocalPos = player_->GetRAtkHitPos();
				pRAtkColPos.x += pRAtkColLocalPos.x;
				pRAtkColPos.y += pRAtkColLocalPos.y;

				Vector2 pLAtkColPos = player_->GetPos().ToVector2();
				Vector2 pLAtkColLocalPos = player_->GetLAtkHitPos();
				pLAtkColPos.x += pLAtkColLocalPos.x;
				pLAtkColPos.y += pLAtkColLocalPos.y;

				if (player_->GetState() == Player::STATE::ATK)
				{
					//プレイヤー弾と敵との当たり判定
					if (IsCollisionRectCenter(
						pRAtkColPos								//プレイヤー中央座標 // 修正
						, player_->GetAtkHitBox()				//プレイヤーのサイズ // 修正
						, enemyColPos							//敵の中央座標
						, enemys_[en]->GetHitBox()				//敵のサイズ							
					) == true)
					{
						//当たりの場合
						//-----------------------------------

						//敵を消す
						enemys_[en]->Hit(1);
					}

				//プレイヤー弾と敵との当たり判定
				if (IsCollisionRectCenter(
					pLAtkColPos								//プレイヤー中央座標 // 修正
					, player_->GetAtkHitBox()				//プレイヤーのサイズ // 修正
					, enemyColPos							//敵の中央座標
					, enemys_[en]->GetHitBox()				//敵のサイズ							
					) == true)
					{
						//当たりの場合
						//-----------------------------------
					
						//敵を消す
						enemys_[en]->Hit(1);
					}
				}
			}
		}
	}
}
//描画処理
void SceneGame::Draw(void)
{
	DrawGraph(0, 0, bgImage1_, true);

	//現在のマップ種別を所得する
	Stage::TYPE nowMapType = stage_->GetMapType();
	if (nowMapType == Stage::TYPE::UN_GROUND)
	{
		DrawGraph(0, 0, bgImage2_, true);
	}
	
	//ステージの描画
	stage_->Draw();
	player_->Draw();

	//エネミーの描画
	size_t size = enemys_.size();
	for (int i = 0; i < size; i++)
	{
		enemys_[i]->Draw();
	}
}

//解放処理
bool SceneGame::Release(void)
{
	DeleteGraph(bgImage1_);
	DeleteGraph(bgImage2_);

	DeleteSoundMem(soundHandle0_);
	DeleteSoundMem(soundHandle1_);
	DeleteSoundMem(soundHandle2_);
	DeleteSoundMem(soundHandle3_);
	DeleteSoundMem(soundHandle4_);
	
	//エネミーの解放
	size_t size = enemys_.size();
	for (int i = 0; i < size; i++)
	{
		enemys_[i]->Release();
		delete enemys_[i];
		enemys_[i] = nullptr;
	}

	//プレイヤーの解放
	player_->Release();			//ステージ内の素材解放
	delete player_;				//インスタンスの削除
	player_ = nullptr;			//ポインタ変数を初期化

	//ステージの解放
	stage_->Release();			//ステージ内の素材解放
	delete stage_;				//インスタンスの削除
	stage_ = nullptr;			//ポインタ変数を初期化

	return true;
}

//カメラの位置を返す
Vector2 SceneGame::GetCameraPos(void)
{
	return cameraPos_;
}

//カメラの移動
void SceneGame::CameraMove(void)
{
	//デバッグ用
	//CameraMoveDebug();
	
	//プレイヤーの現在の座標取得
	Vector2F playerPos = player_->GetPos();

	//カメラにとってプレイヤーはどこにいるのか？
	//プレイヤーの座標とカメラの座標差分を算出
	int diffX = playerPos.x - cameraPos_.x;
	int diffY = playerPos.y - cameraPos_.y;

	//画面内の右枠より外に出ようとした場合は、カメラを移動させる
	if (diffX > Application::SCREEN_SIZE_X - CAMERA_WIDTH)
	{
		cameraPos_.x += diffX - (Application::SCREEN_SIZE_X - CAMERA_WIDTH);
	}

	//画面内の左枠より外に出ようとした場合は、カメラを移動させる
	if (diffX < CAMERA_WIDTH)
	{
		cameraPos_.x += diffX - CAMERA_WIDTH;;
	}

	//画面内の上枠より外に出ようとした場合は、カメラを移動させる
	if (diffY < CAMERA_HEIGHT)
	{
		cameraPos_.y += diffY - CAMERA_HEIGHT;
	}

	//画面内の下枠より外に出ようとした場合は、カメラを移動させる
	if (diffY > Application::SCREEN_SIZE_Y - CAMERA_HEIGHT2)
	{
		cameraPos_.y += diffY - (Application::SCREEN_SIZE_Y - CAMERA_HEIGHT2);
	}

	//カメラの移動制限を行い世界の果てを描画しないようにする
	if (cameraPos_.x < 0)
	{
		cameraPos_.x = 0;
	}

	if (cameraPos_.y < 0)
	{
		cameraPos_.y = 0;
	}

	if (cameraPos_.x > (Stage::CHIP_SIZE_X * Stage::MAP_GROUND_SIZE_X)
		- Application::SCREEN_SIZE_X)
	{
		cameraPos_.x = (Stage::CHIP_SIZE_X * Stage::MAP_GROUND_SIZE_X)
			- Application::SCREEN_SIZE_X;
	}

	if (cameraPos_.y > (Stage::CHIP_SIZE_Y * Stage::MAP_GROUND_SIZE_Y)
		- Application::SCREEN_SIZE_Y)
	{
		cameraPos_.y = (Stage::CHIP_SIZE_Y * Stage::MAP_GROUND_SIZE_Y)
			- Application::SCREEN_SIZE_Y;
	}
}
/*//カメラ移動(デバッグ用)
void SceneGame::CameraMoveDebug(void)
{
	const int SPEED_CAMERA = 8;

	//デバッグ用マップ移動機能
	if (CheckHitKey(KEY_INPUT_W) == 1)
	{
		cameraPos_.y -= SPEED_CAMERA;
	}

	if (CheckHitKey(KEY_INPUT_S) == 1)
	{
		cameraPos_.y += SPEED_CAMERA;
	}

	if (CheckHitKey(KEY_INPUT_A) == 1)
	{
		cameraPos_.x -= SPEED_CAMERA;
	}

	if (CheckHitKey(KEY_INPUT_D) == 1)
	{
		cameraPos_.x += SPEED_CAMERA;
	}
}	*/

//ワールド座標をマップ座標に変換する
Vector2 SceneGame::WorldPos2MapPos(Vector2 worldPos)
{
	Vector2 mapPos;

	mapPos.x = worldPos.x / Stage::CHIP_SIZE_X;
	mapPos.y = worldPos.y / Stage::CHIP_SIZE_Y;

	return mapPos;
}

//マップチップを使用した衝突判定
//ture::衝突している
bool SceneGame::IsCollisionStage(Vector2 worldPos)
{
	//ワールド座標をマップ座標に変換
	Vector2 mapPos;
	mapPos = WorldPos2MapPos(worldPos);

	//マップ座標にあるマップチップNoを取得
	int chipNo;
	chipNo = stage_->GetChipNo(mapPos);

	//マップチップNoを衝突判定する
	switch (chipNo)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 18:
	case 19:
	case 20:
	case 21:
	case 37:
	case 54:
	case 55:
	case 56:
	case 57:
	case 87:
	case 88:
	case 120:
	case 138:
	case 156:
	case 174:
		
		//衝突している
		return true;

	default:
		break;
	}

	return false;				//衝突していない
}

Vector2F SceneGame::GetPlayerPos(void)
{
	Vector2F playerPos;

	playerPos = player_->GetPos();

	return playerPos;
}

void SceneGame::PlayerHit()
{
	if (player_->GetState() != Player::STATE::DAMAGED)
	{
		//プレイヤーの体力を減らしましょう
		player_->Damage(1);

		soundHandle3_ = LoadSoundMem("Image/Sound/Damage.mp3");		//BGMを鳴らす処理
		PlaySoundMem(soundHandle3_, DX_PLAYTYPE_BACK, true);		//BGMを再生
	}
}

Vector2 SceneGame::GetPlayerHitBox(void)
{
	return player_->GetHitBox();
}

bool SceneGame::CreateEnemy(void)
{
	EnemyBase* e = nullptr;		//EnemyBase型

	e = new EnemyMob1();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(300.0f, 0.0f));
	e->SetStartPos(Vector2(0, 0));
	enemys_.push_back(e);

	e = new EnemyMob1();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(640.0f, 850.0f));
	e->SetStartPos(Vector2(100, 0));
	enemys_.push_back(e);

	e = new EnemyMob1();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(1500.0f, 0.0f));
	e->SetStartPos(Vector2(1000, 0));
	enemys_.push_back(e);

	e = new EnemyMob1();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(2200.0f, 0.0f));
	e->SetStartPos(Vector2(1700, 0));
	enemys_.push_back(e);

	e = new EnemyMob1();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(3000.0f, 0.0f));
	e->SetStartPos(Vector2(2500, 0));
	enemys_.push_back(e);

	e = new EnemyMob1();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(3500.0f, 0.0f));
	e->SetStartPos(Vector2(3000, 0));
	enemys_.push_back(e);

	e = new EnemyWizard();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(1500.0f, 645.0f));
	e->SetStartPos(Vector2(1000, 0));
	enemys_.push_back(e);

	e = new EnemyWizard();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(1800.0f, 645.0f));
	e->SetStartPos(Vector2(1300, 0));
	enemys_.push_back(e);

	e = new EnemyWizard();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(2750.0f, 645.0f));
	e->SetStartPos(Vector2(2250, 0));
	enemys_.push_back(e);

	e = new EnemyWizard();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(3830.0f, 645.0f));
	e->SetStartPos(Vector2(3330, 0));
	enemys_.push_back(e);

	e = new EnemyGimp();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(2300.0f, 485.0f));
	e->SetStartPos(Vector2(1800, 0));
	enemys_.push_back(e);
}

bool SceneGame::CreateEnemy1(void)
{
	EnemyBase* e = nullptr;		//EnemyBase型

	e = new EnemyMob1();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(800.0f, 0.0f));
	e->SetStartPos(Vector2(300, 0));
	enemys_.push_back(e);

	e = new EnemyMob1();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(2500.0f, 800.0f));
	e->SetStartPos(Vector2(2000, 0));
	enemys_.push_back(e);

	e = new EnemyMob1();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(3600.0f, 0.0f));
	e->SetStartPos(Vector2(3100, 0));
	enemys_.push_back(e);

	e = new EnemyWizard();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(1050.0f, 450.0f));
	e->SetStartPos(Vector2(550, 0));
	enemys_.push_back(e);

	e = new EnemyWizard();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(1930.0f, 290.0f));
	e->SetStartPos(Vector2(1430, 0));
	enemys_.push_back(e);

	e = new EnemyWizard();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(2870.0f, 290.0f));
	e->SetStartPos(Vector2(2370, 0));
	enemys_.push_back(e);

	e = new EnemyWizard();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(4650.0f, 610.0f));
	e->SetStartPos(Vector2(4150, 0));
	enemys_.push_back(e);

	e = new EnemyGimp();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(1700.0f, 450.0f));
	e->SetStartPos(Vector2(1200, 0));
	
	enemys_.push_back(e);

	e = new EnemyBoss();
	if (e->Init(this) == false)
	{
		OutputDebugString("SceneGame Updateでエラー\n");
		return false;
	}
	e->SetPos(Vector2F(4560.0f, 360.0f));
	e->SetStartPos(Vector2(4060, 0));
	enemys_.push_back(e);
}

//当たり判定用関数
bool SceneGame::IsCollisionRectCenter(
	Vector2 centerPos1		//オブジェクト１の中央座標
	, Vector2 size1			//オブジェクト１のサイズ
	, Vector2 centerPos2	//オブジェクト２の中央座標
	, Vector2 size2			//オブジェクト２のサイズ
)
{
	//オブジェクト１の左上と右下の座標を計算する
	Vector2 stPos1 = centerPos1;		//初期値は中央座標を入れる
	Vector2 edPos1 = centerPos1;		//初期値は中央座標を入れる
	//Vector2 hSize1 = { size1.x / 2, size1.y / 2 }; //サイズの半分を計算する
	Vector2 hSize1 = { size1.x, size1.y };		  //サイズ

	//左上座標を計算する 左座標と上座標がわかる
	stPos1.x -= hSize1.x;
	stPos1.y -= hSize1.y;

	//右上座標を計算する 右座標と下座標がわかる
	edPos1.x += hSize1.x;
	edPos1.y += hSize1.y;

	//オブジェクト２の左上と右下の座標を計算する
	Vector2 stPos2 = centerPos2;		//初期値は中央座標を入れる
	Vector2 edPos2 = centerPos2;		//初期値は中央座標を入れる
	//Vector2 hSize2 = { size2.x / 2, size2.y / 2 }; //サイズの半分を計算する
	Vector2 hSize2 = { size2.x, size2.y };		  //サイズ

	//左上座標を計算する 左座標と上座標がわかる
	stPos2.x -= hSize2.x;
	stPos2.y -= hSize2.y;

	//右上座標を計算する 右座標と下座標がわかる
	edPos2.x += hSize2.x;
	edPos2.y += hSize2.y;

	//矩形と矩形の当たり判定処理
	if (stPos1.x < edPos2.x			//オブジェクト１の左　＜　オブジェクト２の右
		&& stPos2.x < edPos1.x
		&& stPos1.y < edPos2.y
		&& stPos2.y < edPos1.y)
	{
		//重なっている状態なので衝突している
		return true;
	}

	return false;
}