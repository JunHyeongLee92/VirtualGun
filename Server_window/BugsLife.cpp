#include "stdafx.h"
#include "BugsLife.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define T_RGB(r,g,b)          ((unsigned int)(((unsigned char)(r)|((unsigned short int)((unsigned char)(g))<<8))|(((unsigned int)(unsigned char)(b))<<16)))

#define PLATFORM_ANDROID   0
#define PLATFORM_WIN32     1

//int black_number = 0;

Bug::Bug()
{
	m_s_pos.x = 0;
	m_s_pos.y = 0;
	m_e_pos.x = 0;
	m_e_pos.y = 0;

	m_a = (float)0;
	m_b = (float)0;

	m_x = 0;
	m_y = 0;
	m_size = 0;

	//m_bug_kind = 0;

	m_color.red = 0;
	m_color.green = 0;
	m_color.blue = 0;

	m_turn_flag = 2;
}

Bug::~Bug()
{

}

void Bug::MakeBugPos(T_POINT *a_pos, int a_side, int a_width, int a_height)
{
	if (a_side == 0) {
		a_pos->x = rand() % (a_width - 300) + 50;
		a_pos->y = 0;
	}
	else if (a_side == 2) {
		a_pos->x = rand() % (a_width - 300) + 50;
		a_pos->y = a_height;
	}
	else if (a_side == 1) {
		a_pos->x = a_width;
		a_pos->y = rand() % (a_height - 300) + 50;
	}
	else if (a_side == 3) {
		a_pos->x = 0;
		a_pos->y = rand() % (a_height - 300) + 50;
	}
}

void Bug::Turn()
{
	
	float center;
	int check = 0;

	if (m_s_pos.x > m_e_pos.x) {
		center = m_e_pos.x + (m_s_pos.x - m_e_pos.x) / 2;
		check = center > m_x;
	}	
	else {
		center = m_s_pos.x + (m_e_pos.x - m_s_pos.x) / 2;
		check = center < m_x;
	}

	if (check) { 
 		m_c_pos.x = m_s_pos.x;
		m_c_pos.y = m_s_pos.y;

		m_s_pos.x = m_e_pos.x;
		m_s_pos.y = m_e_pos.y;

		m_e_pos.x = m_c_pos.x;
		m_e_pos.y = m_c_pos.y;

		m_a = float(m_e_pos.y - m_s_pos.y) / (m_e_pos.x - m_s_pos.x);
		m_b = m_s_pos.y - m_a*m_s_pos.x;

		m_turn_flag -= 1;
	}
}


void Bug::MakeBug(int a_width, int a_height)
{
	/*if (black_number == 0) {
		m_bug_kind = rand() % 2;
	}*/

	//if (black_number == 0 && m_bug_kind == 1)   //흡수하는 벌레
	//{
	//	m_s_pos.x = rand()%2 * a_width;
	//	m_s_pos.y = rand() % (a_height - 300) + 50;

	//	m_e_pos.x = a_width - m_s_pos.x;
	//	m_s_pos.y = a_height - m_s_pos.y;

	//	m_color.red = 0;
	//	m_color.green = 0;
	//	m_color.blue = 0;

	//	m_size = 140;
	//	black_number = 1;
	//}
	//else
	//{
	//	int first_side = rand() % 4, second_side; // 0 ~ 3

	//	MakeBugPos(&m_s_pos, first_side, a_width, a_height);

	//	do {
	//		second_side = rand() % 4;
	//	} while (first_side == second_side);

	//	MakeBugPos(&m_e_pos, second_side, a_width, a_height);

	//	if (m_e_pos.x == m_s_pos.x) m_e_pos.x += 30;

	//	m_color.red = rand() % 130 + 100;
	//	m_color.green = rand() % 130 + 100;
	//	m_color.blue = rand() % 130 + 100;

	//	m_size = rand() % 80 + 40; // 40 ~ 120
	//}
	int first_side = rand() % 4, second_side; // 0 ~ 3

	MakeBugPos(&m_s_pos, first_side, a_width, a_height);

	do {
		second_side = rand() % 4;
	} while (first_side == second_side);

	MakeBugPos(&m_e_pos, second_side, a_width, a_height);

	if (m_e_pos.x == m_s_pos.x) m_e_pos.x += 30;

	m_color.red = rand() % 130 + 100;
	m_color.green = rand() % 130 + 100;
	m_color.blue = rand() % 130 + 100;

	m_size = rand() % 80 + 40; // 40 ~ 120
	m_x = m_s_pos.x;
	m_y = 0;

	m_a = float(m_e_pos.y - m_s_pos.y) / (m_e_pos.x - m_s_pos.x);
	m_b = m_s_pos.y - m_a*m_s_pos.x;
	Move();

}

int Bug::Move()
{
	int end_flag = 0;
	if (m_e_pos.x > m_s_pos.x) {
		m_x+= 3;
		if (m_x >= m_e_pos.x)  end_flag++;
	}
	else {
		m_x -= 3;
		if (m_x <= m_e_pos.x) end_flag++;
	}

	if(!end_flag) m_y = (int)(m_a*m_x + m_b);   // y = ax + b

	return end_flag;
}

int Bug::Check(int a_x, int a_y)
{
	return int(sqrt((m_x - a_x)*(m_x - a_x) + (m_y - a_y)*(m_y - a_y))) <= (m_size/2);
}

const T_POINT *Bug::GetStartPos()
{
	return &m_s_pos;
}

const T_POINT *Bug::GetEndPos()
{
	return &m_e_pos;
}

int Bug::GetTurnFlag()
{
	return m_turn_flag;
}

int Bug::GetPosX()
{
	return m_x;
}

int Bug::GetPosY()
{
	return m_y;
}

//int Bug::GetKind()
//{
//	return m_bug_kind;
//}

void Bug::SetSize()
{
	m_size -= 20;
}

//void Bug::SetSize(int a_correct)
//{
//	m_size += a_correct;
//}


int Bug::GetSize()
{
	return m_size;
}

const T_COLOR *Bug::GetColor()
{
	return &m_color;
}

int Bug::GetBugScore()
{
	int score = int(atan(m_a) * 180 / 3.141592);
	if (score < 0) score *= -1;
	score += (400 - m_size)/2 + 1;
	return score;
}


BugsLife::BugsLife()
{
#if PLATFORM == PLATFORM_WIN32
	static char info[] = "Platform for Win32";
#elif PLATFORM == PLATFORM_ANDROID
	static char info[] = "Platform for Android";
#else
	static char info[] = "Undefined platform";
#endif

	srand((unsigned int)time(NULL));

	m_width = 0;
	m_height = 0;
	m_bug_count = 0;
	m_score = 0;
	//m_black_size = 0;
}

BugsLife::~BugsLife()
{

}

void BugsLife::SetArea(int a_width, int a_height)
{
	m_width = a_width;
	m_height = a_height;
}

void BugsLife::ResetGame()
{
	m_bug_count = 0;
	m_score = 0;
}

int BugsLife::Add()
{
	if (m_bug_count < MAX_BUG_COUNT) {
		m_bug_list[m_bug_count].MakeBug(m_width, m_height);
		m_bug_count++;
		return 1;
	}
	return 0;
}

int BugsLife::GetScore()
{
	return m_score;
}

int BugsLife::GetLife()
{
	return m_user_life;
}

void BugsLife::Move()
{
	Bug *p_pos = m_bug_list;
	Bug *p_black_pos = m_bug_list;
	for (int i = 0; i < m_bug_count; i++) {
		
		/*if (p_pos->GetKind() == 1)
		{
			p_black_pos = m_bug_list + i ;
			
			m_black_pos.x = p_pos->GetPosX();
			m_black_pos.y = p_pos->GetPosY();
			m_black_size = p_pos->GetSize();
		}*/
		/*else if (black_number == 1)
		{
			if (p_pos->GetPosX() >= m_black_pos.x - m_black_size / 2 && p_pos->GetPosX() <= m_black_pos.x + m_black_size / 2)
				if (p_pos->GetPosY() >= m_black_pos.y - m_black_size / 2 && p_pos->GetPosY() <= m_black_pos.y + m_black_size / 2)
				{
					if (m_black_size < 400) {
						p_black_pos->SetSize(p_pos->GetSize());
						p_pos->MakeBug(m_width, m_height);
					}
					else
						p_pos->MakeBug(m_width, m_height);
				}

		}*/

		if (p_pos->Move() == 1) {
			// 벌래가 밖으로 나간 경우
			//if (p_pos->GetKind() == 1) black_number = 0;
			m_score -= 500;
			m_user_life -= 1;
			p_pos->MakeBug(m_width, m_height);
		}
		p_pos++;
	}
}

void BugsLife::Check(int a_x, int a_y)
{
	int find_flag = 0;
	Bug *p_pos = m_bug_list;
	for (int i = 0; i < m_bug_count; i++) {
		if (p_pos->Check(a_x, a_y) == 1) {
			// 벌래를 클릭한 경우
			if (p_pos->GetSize() > 60) {
				p_pos->SetSize();
				find_flag = 1;
				m_score += 10 + p_pos->GetBugScore();
			}
			else if(p_pos->GetSize() <= 60) {
//				if (p_pos->GetKind() == 1) black_number = 0;
				m_score += 10 + p_pos->GetBugScore();
				find_flag = 1;
				p_pos->MakeBug(m_width, m_height);
			}
		}
		p_pos++;
	}

	if(find_flag == 0) m_score -= 10;
}

void BugsLife::Turn()
{
	Bug *p_pos = m_bug_list;
	for (int i = 0; i < m_bug_count; i++) {

		if (p_pos->GetTurnFlag() != 0)	p_pos->Turn();

		p_pos++;
	}
}

void BugsLife::Boom()
{
	Bug *p_pos = m_bug_list;
	for (int i = 0; i < m_bug_count; i++) {
		while(p_pos->GetSize() > 60)	Check(p_pos->GetPosX(), p_pos->GetPosY());
		p_pos++;
	}
	m_bug_count = 0;
	m_item -= 1;
}

int BugsLife::GetCount()
{
	return m_bug_count;
}

int BugsLife::GetItem()
{
	return m_item;
}

Bug *BugsLife::GetBug(int a_index)
{
	return m_bug_list + a_index;
}

