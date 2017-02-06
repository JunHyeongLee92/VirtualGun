#include "stdafx.h"

#pragma once

#define MAX_BUG_COUNT    20
#define LIFE	20
#define ITEM_COUNT		5


//struct BLACK_BUG
//{
//	int m_black_number;
//
//};

struct T_POINT
{
	int x;
	int y;
};

struct T_COLOR
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};


class Bug
{
private:
	T_POINT m_s_pos;
	T_POINT m_e_pos;
	T_POINT m_c_pos;

	float m_a;   // y = ax + b
	float m_b;

	//int m_bug_kind;

	int m_turn_flag;
	int m_x;
	int m_y;
	int m_size;
	T_COLOR m_color;

public:
	Bug();
	~Bug();


	void MakeBug(int a_width, int a_height);
	void MakeBugPos(T_POINT *a_pos, int a_side, int a_width, int a_height);
	void Turn();
	void SetSize();
	//void SetSize(int a_correct);
	int Move();
	int Check(int a_x, int a_y);

	const T_POINT *GetStartPos();
	const T_POINT *GetEndPos();

	int GetTurnFlag();
	int GetPosX();
	int GetPosY();
	int GetSize();
	int GetBugScore();
	const T_COLOR *GetColor();
};


class BugsLife 
{
private:
	//T_POINT m_black_pos;
	Bug m_bug_list[MAX_BUG_COUNT];
	int m_width, m_height, m_bug_count, m_score, m_user_life = LIFE;
	int m_item = ITEM_COUNT;
	//int m_black_size;
public:
    BugsLife();
    ~BugsLife();

	void SetArea(int a_width, int a_height);
	void ResetGame();
	int Add();
	void Move();
	void Check(int a_x, int a_y);

	void Turn();
	void Boom();

	int GetLife();
	int GetScore();
	int GetCount();
	int GetItem();

	Bug *GetBug(int a_index);
};
