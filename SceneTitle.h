//#pragma once

#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	//SceneTitle(void);		//デフォルトコンストラクタ
	//~SceneTitle(void);	//デストラクタ

	bool Init(void) override;		//初期化処理
	void Update(void) override;		//更新処理
	void Draw(void) override;		//描画処理
	bool Release(void) override;	//解放処理

private:
	//メンバー変数
	int titleImage_;		//タイトル画像

	int titleCounter_;		//タイトル画像用のカウンタ

	int titleSpace_;

	int soundClick_;		//クリック音
};
