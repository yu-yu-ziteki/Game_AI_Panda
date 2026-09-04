#include "PlayScene.h"
#include <DxLib.h>
#include "../Stage.h"
#include "../Player.h"
#include "../Enemy.h"

PlayScene::PlayScene()
{
	new Stage();
	new Player();
	new Enemy();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T))
	{
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(255, 255, 255), TRUE);
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
	Enemy* enemy = FindGameObject<Enemy>();
	if (enemy->GetState() == Enemy::Chase) {
		DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(255, 100, 100), TRUE);
	}
	else if (enemy->GetState() == Enemy::Search) {
		DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(200, 100, 255), TRUE);
	}
}
