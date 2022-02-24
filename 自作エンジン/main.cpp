#include "OnoGame.h"
#include "safeDelete.h"

//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ゲームポインタ置き場
	OnoGame* game = new OnoGame;

	game->Run();

	safe_delete(game);

	return 0;
}