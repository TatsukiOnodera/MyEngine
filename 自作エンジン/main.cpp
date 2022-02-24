#include "MyGame.h"

//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ゲームポインタ置き場
	MyGame* game = new MyGame;

	//ゲーム初期化
	game->Initialize();

	while (1)  // ゲームループ
	{
		//ゲーム毎フレーム更新
		game->Update();

		//ゲーム終了リクエスト
		if (game->IsEndRequest())
		{
			break;
		}

		//ゲーム描画
		game->Draw();
	}

	//ゲーム終了
	game->Finalize();

	return 0;
}