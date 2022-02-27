#include "OnoGame.h"
#include "safeDelete.h"

//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ポインタ置き場
	OnoGame* game = new OnoGame;

	//ゲーム開始
	game->Run();

	safe_delete(game);

	return 0;
}