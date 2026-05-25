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
	const int viewArea = CHA_SIZE * 7;
}


Enemy::Enemy()
	: GameObject() 
{
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	//GetRand(数値)
	//3秒に1回向きをランダムに変える
	static float dir_timer = 3.0f;
	static float prog_timer = 0.5f;
	float dt = Time::DeltaTime();
	dir_timer = dir_timer - dt;
	prog_timer = prog_timer - dt;
	Point pPos = FindGameObject<Player>()->GetPlayerPos();
	

	Point newPos = pos_;
	if (prog_timer < 0.0f)
	{
		switch (dir_)
		{
		case UP:
			newPos.y -= ENEMY_DRAW_SIZE;
			break;
		case DOWN:
			newPos.y += ENEMY_DRAW_SIZE;
			break;
		case LEFT:
			newPos.x -= ENEMY_DRAW_SIZE;
			break;
		case RIGHT:
			newPos.x += ENEMY_DRAW_SIZE;
			break;
		default:
			break;
		}

		int mapValue = FindGameObject<Stage>()->GetMap(newPos.x / CHA_SIZE, newPos.y / CHA_SIZE);
		//移動先がステージの外に出ないようにする

		if (!(newPos.x < 1 || newPos.x >(STAGE_WIDTH - 2) * ENEMY_DRAW_SIZE
			|| newPos.y < 1 || newPos.y >(STAGE_HEIGHT - 2) * ENEMY_DRAW_SIZE))
		{
			pos_ = newPos;
		}
		prog_timer = 0.5f + prog_timer;
	}
	

	int diffX = pPos.x - pos_.x;
	int diffY = pPos.y - pos_.y;

	diffX = diffX * diffX;
	diffY = diffY * diffY;
	int diff = (int)sqrt(diffX + diffY);

	if (diff > viewArea)//パンダの視野より外にいたら
	{
		if (dir_timer < 0.0f)
		{
			dir_ = (DIR)(GetRand(3));
			dir_timer = 3.0f + dir_timer;
		}
		return;
	}

		if (diffX >= diffY)//Xの距離の方が大きかったら左右に動く
		{
			if (pPos.x <= pos_.x)
			{
				dir_ = LEFT;
			}
			else
			{
				dir_ = RIGHT;
			}
		}
		else                //Yの距離の方が大きかったら上下に動く
		{
			if (pPos.y <= pos_.y)
			{
				dir_ = UP;
			}
			else
			{
				dir_ = DOWN;
			}
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
	DrawFormatString(0, 12, 0x000000, "x = %d y = %d %c", pPos.x / ENEMY_SIZE, pPos.y / ENEMY_SIZE);
	DrawFormatString(0, 0, 0x000000, "x = %d y = %d %c", pos_.x / ENEMY_SIZE, pos_.y / ENEMY_SIZE);
	DrawCircle(pos_.x + ENEMY_SIZE / 2, pos_.y + ENEMY_SIZE / 2, viewArea, GetColor(255,0,0), FALSE);

}
