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
	bool isFound;
};

