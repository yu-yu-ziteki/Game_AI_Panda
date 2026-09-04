#pragma once
#include ".\Library\GameObject.h"
#include "global.h"

struct flont
{
	float x;
	float y;
};


class Enemy :
    public GameObject
{
	int hImage_;//画像ID
	Point pos_;//位置
	DIR dir_;//移動方向
	flont flont_; 
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;

private:
	float search_timer; // 捜索用タイマー（2秒用）
	float look_timer;   // 四方向を切り替えるためのタイマー

	float playerVecX;
	float playerVecY;
	float dot;
	float dir_timer;
	float prog_timer;
	float dt;
	
	int distX;
	int distY;
	int diff; // 距離

	bool isFoundPlayer;
	bool isAttackRange;
	bool isSearchTimeOver;

	void UpdatePatrol();//->Chase
	void UpdateChase();//->Attack
	void UpdateAttack();//->Search
	void UpdateSearch();//Patrol

public:
	enum EnemyState
	{
		Patrol,
		Chase,
		Attack,
		Search
	};
	EnemyState State;

	EnemyState GetState() const { return State; }

};