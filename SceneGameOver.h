//#pragma once

#include "SceneBase.h"

class SceneGameOver : public SceneBase
{
public:
	//SceneGameOver(void);		//デフォルトコンストラクタ
	//~SceneGameOver(void);		//デストラクタ

	bool Init(void) override;		//初期化処理
	void Update(void) override;		//更新処理
	void Draw(void) override;		//描画処理
	bool Release(void) override;	//解放処理

private:
	int gameOverImage_;				//ゲームオーバー画像格納領域
	int gameOverImage1_;				//ゲームオーバー画像格納領域

	int gameOverCounter;

	int soundGameOver_;

	int soundClick_;
};