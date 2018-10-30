#pragma once
#include <stdio.h>
#include "stdafx.h"

#define MAIN_TIMER 1
#define SECOND 1000
#define MAX_CMD_NUM 100

typedef struct {
	CRect capture_info;
	CRect click_info;
	int delay;
	CString filename;
	CString comment;
	BOOL priority;
}Script_List;

typedef struct my_date {
	int year;
	int month;
	int day;
	int day_of_week;
}my_date;

typedef struct line_data {
	my_date date;
	int count;
}line_data;

typedef struct code_data {
	int first;
	int second;
}code_data;

#define BMP_FRONT_GARBAGE_SIZE 0