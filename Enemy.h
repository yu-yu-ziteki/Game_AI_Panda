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
	int hImage_;//‰æ‘œID
	Point pos_;//ˆÊ’u
	DIR dir_;//ˆÚ“®•ûŒü
	flont flont_; 
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;

private:
	float playerVecX;
	float playerVecY;
	float dot;
	float dir_timer;
	float prog_timer;
	float dt;
	
	int distX;
	int distY;
	int diff; // ‹——£

	bool isFoundPlayer;
	bool isAttackRange;
	bool isSearchTimeOver;

	void UpdatePatrol();//“K“–‚ÉˆÚ“®
	void UpdateChase();//’Ç‚¢‚©‚¯‚é
	/*void UpdateAttack();
	void UpdateSearch();*/

	enum EnemyState
	{
		Patrol,
		Chase,
	/*	Attack,
		Search*/
	};
	EnemyState State;
};

