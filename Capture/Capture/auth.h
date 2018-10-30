#pragma once

#include "dataType.h"
class auth
{
public:
	auth(void);
	~auth(void);

	my_date m_today;
	CString m_today_str;
	int m_today_int;
	CString m_first_line;
	line_data first_line;
	CString m_second_line;
	line_data second_line;
	int *m_seed_array;
	int m_seed;
	int m_code_month;
	int m_code_day;
	CString m_code_str;
	int m_code_int;
	void print_code();
	void auth_init();

	//auth file
	CString auth_filename;
	CFile auth_file;
	BOOL open_auth_file();
	char auth_buffer[1024];
	int m_remain_count;
	int read_auth_data();
	void write_auth_data();
	BOOL do_auth(int pw);

	//account file
	CString account_filename;
	CFile account_file;

};
