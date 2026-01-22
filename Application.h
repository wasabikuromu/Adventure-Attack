//#pragma once

class SceneManager;

class Application
{
public:
	//定数(全て大文字で書く)
	//--------------------------------------
	//画面サイズの設定
	static constexpr int SCREEN_SIZE_X = 1000;
	static constexpr int SCREEN_SIZE_Y = 640;

	//メンバー関数
	bool Init(void);		//初期化処理
	void Run(void);			//ゲームループ処理
	bool Release(void);		//解放処理

	//シングルトン化
	//----------------------------
	//インスタンスの生成
	//外部から静的にインスタンスを生成する
	static void CreateInstance(void);

	//外部でインスタンスを利用可能とするために
	//インスタンスを返す関数を用意する
	//参照型にしているがポインタ型でもよい。
	static Application& GetInstance(void);

private:
	//メンバー変数
	//-------------------------------
	//管理マネージャ
//	SceneManager* sceneManager_;	//シーン管理


	//シングルトン化
	//----------------------------
	Application(void);		//デフォルトコンストラクタ
	~Application(void);		//デストラクタ

	//コピーコンストラクタを利用出来なくする
	Application(const Application& ins);

	//インスタンスの破棄
	void Destory(void);

	//静的なインスタンス格納領域
	static Application* instance_;
};
