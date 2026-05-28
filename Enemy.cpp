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
	flont_ = { 0.0f, 0.0f };
	isFound = false;
	float playerVecX = 0;
	float playerVecY = 0;
	float dot        = 0;
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
			flont_ = {0.0f, -1.0f}; //上向きの単位ベクトル
			break;
		case DOWN:
			newPos.y += ENEMY_DRAW_SIZE;
			flont_ = { 0.0f, 1.0f }; //下向きの単位ベクトル
			break;
		case LEFT:
			newPos.x -= ENEMY_DRAW_SIZE;
			flont_ = { -1.0f, 0.0f }; //左向きの単位ベクトル
			break;
		case RIGHT:
			newPos.x += ENEMY_DRAW_SIZE;
			flont_ = { 1.0f, 0.0f }; //右向きの単位ベクトル
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
	

	int distX = pPos.x - pos_.x; //プレイヤーと敵のXの距離を求める
	int distY = pPos.y - pos_.y; //プレイヤーと敵のYの距離を求める

	int diff = (int)sqrt((distX * distX) + (distY * distY)); //プレイヤーと敵の距離を求める

	if (diff == 0) diff = 1;

	playerVecX = (float)(distX / diff); //プレイヤーへの単位ベクトルX
	 playerVecY = (float)(distY / diff); //プレイヤーへの単位ベクトルY

	dot = (flont_.x * playerVecX) + (flont_.y * playerVecY); //フロントベクトルとプレイヤーへのベクトルの内積

	if (dot >= 0.7 && diff <= viewArea)
	{
		isFound = true;
	}
	else
	{
		isFound = false;
	}
	if (isFound  == false)//パンダの視野より外にいたら
	{
		isFound = false;
		if (dir_timer < 0.0f){
			dir_ = (DIR)(GetRand(3));
			dir_timer = 3.0f + dir_timer;

		}
		return;
	}

	if (isFound) //プレイヤーが視野の範囲に入っていたら
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
	DrawFormatString(0, 12, 0x000000, "x = %d y = %d", pPos.x / ENEMY_SIZE, pPos.y / ENEMY_SIZE);
	DrawFormatString(0, 0, 0x000000, "x = %d y = %d", pos_.x / ENEMY_SIZE, pos_.y / ENEMY_SIZE);
	DrawFormatString(0, 24, 0x000000, "dot = %f", dot);
	DrawFormatString(0, 36, 0x000000, "playerVecX = %f playerVecY = %f", playerVecX, playerVecY);
	if(isFound) DrawCircle(pos_.x + ENEMY_SIZE / 2, pos_.y + ENEMY_SIZE / 2, viewArea, GetColor(255,0,0), FALSE);

}
