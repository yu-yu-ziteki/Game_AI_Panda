#include "Enemy.h"
#include "time.h"
#include "Stage.h"
#include "Player.h"
#include <cmath>

namespace
{
	const int ENEMY_SIZE = 48; //敵のサイズ 32*32
	const Point ENEMY_START_POS = { 20 * ENEMY_SIZE, 10 * ENEMY_SIZE }; //敵の初期位置
	const DIR INIT_ENEMY_DIR = { LEFT };
	const int ENEMY_DRAW_SIZE = 32; //敵の描画サイズ
	const int animFrame[4]{ 0, 1, 2, 1 };
	const float ANIM_INTERVAL = 0.2f;
	const int viewArea = CHA_SIZE * 7; //視野の最大
}


Enemy::Enemy()
	: GameObject() 
{
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
	flont_ = { 0.0f, 0.0f };
	isFound = false;
	playerVecX = 0;
	playerVecY = 0;
	dot        = 0;
	State = Patrol;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	float dt = Time::DeltaTime();
	Point pPos = FindGameObject<Player>()->GetPlayerPos();

	switch (dir_)
	{
	case UP:    flont_ = { 0.0f, -1.0f }; break;
	case DOWN:  flont_ = { 0.0f,  1.0f }; break;
	case LEFT:  flont_ = { -1.0f,  0.0f }; break;
	case RIGHT: flont_ = { 1.0f,  0.0f }; break;
	}

	
	int distX = pPos.x - pos_.x;
	int distY = pPos.y - pos_.y;
	int diff = (int)sqrt((distX * distX) + (distY * distY)); // 距離

	if (diff == 0) diff = 1;

	playerVecX = (float)distX / diff;
	playerVecY = (float)distY / diff;
	dot = (flont_.x * playerVecX) + (flont_.y * playerVecY);


	if (dot >= 0.7 && diff <= viewArea)
	{
		isFound = true;
	}

	if (diff >= viewArea) {
		isFound = false;
	}
	
	static float dir_timer = 3.0f;
	static float prog_timer = 0.5f;
	dir_timer -= dt;
	prog_timer -= dt;

	if (prog_timer < 0.0f)
	{
		if (isFound)
		{
			if (abs(distX) >= abs(distY))
			{
				if (pPos.x <= pos_.x) dir_ = LEFT;
				else                  dir_ = RIGHT;
			}
			else
			{
				if (pPos.y <= pos_.y) dir_ = UP;
				else                  dir_ = DOWN;
			}
		}
		else
		{
			if (dir_timer < 0.0f)
			{
				dir_ = (DIR)(GetRand(3));
				dir_timer = 3.0f;
			}
		}



		Point nextPos = pos_;
		switch (dir_)
		{
		case UP:    nextPos.y -= ENEMY_DRAW_SIZE; break;
		case DOWN:  nextPos.y += ENEMY_DRAW_SIZE; break;
		case LEFT:  nextPos.x -= ENEMY_DRAW_SIZE; break;
		case RIGHT: nextPos.x += ENEMY_DRAW_SIZE; break;
		}

		
		int mapValue = FindGameObject<Stage>()->GetMap(nextPos.x / CHA_SIZE, nextPos.y / CHA_SIZE);
		bool isOut = (nextPos.x < 1 || nextPos.x >(STAGE_WIDTH - 2) * ENEMY_DRAW_SIZE ||
			nextPos.y < 1 || nextPos.y >(STAGE_HEIGHT - 2) * ENEMY_DRAW_SIZE);

		if (mapValue != 1 && !isOut)
		{
			pos_ = nextPos;
		}

		prog_timer = 0.5f + prog_timer; // タイマーリセット
	}
}
void Enemy::Draw()
{
	Point pPos = FindGameObject<Player>()->GetPlayerPos();
	static float animTimer = ANIM_INTERVAL;
	static int frame = 0;
	int nowFrame = animFrame[frame];

	Rect iRect[4] = {
		{  nowFrame * ENEMY_SIZE, 3 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 0 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 1 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 2 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE}
	};
	DrawBox(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		GetColor(255, 255, 0), FALSE,2);
	DrawRectExtendGraph(pos_.x, pos_.y,pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		               iRect[dir_].x, iRect[dir_].y, iRect[dir_].w, iRect[dir_].h, hImage_, TRUE);
	if (animTimer < 0) {
		frame = (++frame) % 4;
		animTimer = ANIM_INTERVAL + animTimer;
	}
	animTimer = animTimer - Time::DeltaTime();
	DrawFormatString(0, 12, 0x000000, "x = %d y = %d", pPos.x / ENEMY_SIZE, pPos.y / ENEMY_SIZE);
	DrawFormatString(0, 0, 0x000000, "x = %d y = %d", pos_.x / ENEMY_SIZE, pos_.y / ENEMY_SIZE);
	DrawFormatString(0, 24, 0x000000, "dot = %f", dot);
	DrawFormatString(0, 36, 0x000000, "playerVecX = %f playerVecY = %f", playerVecX, playerVecY);
	if(isFound) DrawCircle(pos_.x + ENEMY_SIZE / 2, pos_.y + ENEMY_SIZE / 2, viewArea, GetColor(255,0,0), FALSE);

}
