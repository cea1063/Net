#include "StdAfx.h"
#include "auth.h"

//if %2 is 1 first, 0 second
int first_seed[] = { 3, 2, 6, 9, 4 , 1, 7 };
int second_seed[] = { 8, 5, 4, 3, 6, 7, 9 };
code_data code[] = { { 86, 47 },{ 96, 70 },{ 93, 27 },{ 71, 15 },{ 93, 91 },
{ 11, 67 },{ 98, 61 },{ 88, 36 },{ 59, 46 },{ 73, 74 },
{ 20, 27 },{ 69, 93 },{ 68, 55 },{ 23, 13 },{ 60, 92 },
{ 64, 40 },{ 81, 57 },{ 12, 10 },{ 57, 18 },{ 27, 48 },
{ 47, 86 },{ 87, 21 },{ 45, 29 },{ 16, 29 },{ 45, 13 },
{ 86, 44 },{ 38, 18 },{ 73, 10 },{ 57, 37 },{ 13, 74 },
};

auth::auth(void)
{
	auth_filename.Format("C:\\Windows\\winnt.dat");
}

auth::~auth(void)
{
}

void auth::print_code()
{
	return;
}

void auth::auth_init()
{
	CTime cTime = CTime::GetCurrentTime();
	m_today.year = cTime.GetYear();
	m_today.month = cTime.GetMonth();
	m_today.day = cTime.GetDay();
	m_today.day_of_week = cTime.GetDayOfWeek();
	if (m_today.day % 2 == 1) {
		m_seed = first_seed[m_today.day_of_week - 1] * 10 + second_seed[m_today.day_of_week - 1];
		m_code_month = code[m_today.month - 1].first;
		if (m_today.day == 31)
			m_code_day = code[0].first;
		else
			m_code_day = code[m_today.day - 1].first;
	}
	else {
		m_seed = second_seed[m_today.day_of_week - 1] * 10 + first_seed[m_today.day_of_week - 1];
		m_code_month = code[m_today.month - 1].second;
		if (m_today.day == 31)
			m_code_day = code[0].second;
		else
			m_code_day = code[m_today.day - 1].second;
	}

	TRACE(" year : %d month : %d day : %d day of week : %d\n", m_today.year, m_today.month, m_today.day, m_today.day_of_week);
	m_today_str.Format("%d%02d%d", m_today.year, m_today.month, m_today.day);
	m_today_int = _ttoi(m_today_str);
	TRACE("today str : %d\n", m_today_int);
	TRACE("m_seed : %d m_code_month : %d m_code_day : %d\n", m_seed, m_code_month, m_code_day);
	m_code_str.Format("%d%d%d", m_seed, m_code_month, m_code_day);
	m_code_int = _ttoi(m_code_str);
	TRACE("m_code_str : %s\n", m_code_str);
}
int auth::read_auth_data()
{
	memset(auth_buffer, 0x0, 1024);
	UINT nBytesRead = auth_file.Read(auth_buffer, 1024);
	if (nBytesRead > 0)
	{
		char temp[4] = { 0x0, };
		TRACE("Read %d bytes %s\n", nBytesRead, auth_buffer);
		memcpy(temp, &auth_buffer[6], 3);
		sscanf_s(temp, "%x", &m_remain_count);
		TRACE("m_remain_count : %d\n", m_remain_count);
	}
	else
	{
		TRACE("Failt to read auth_buffer!!!\n");
	}
	return nBytesRead;
}

void auth::write_auth_data()
{
	int dummy_array[6];
	int i;
	memset(auth_buffer, 0x0, 1024);
	for (i = 0; i<6; i++)
	{
		dummy_array[i] = rand() % 256;
		TRACE("%d %02X\n", dummy_array[i], dummy_array[i]);
	}
	sprintf_s(auth_buffer, "%02X%02X%02X%03X%02X%02X%02X\n", dummy_array[0], dummy_array[1], dummy_array[2], m_remain_count, dummy_array[3], dummy_array[4], dummy_array[5]);
	auth_file.SeekToBegin();
	auth_file.Write(auth_buffer, 1024);
}

BOOL auth::do_auth(int pw)
{
	CFileException e;
	if (pw != m_code_int)
	{
		TRACE("pw not correct!!!\n");
		return FALSE;
	}

	if (!auth_file.Open(auth_filename, CFile::modeCreate | CFile::modeWrite, &e))
	{
		TRACE("%s Open Failed\n", auth_filename);
		return FALSE;
	}
	m_remain_count = 100;
	write_auth_data();
	auth_file.Close();
	return TRUE;
}

BOOL auth::open_auth_file()
{
	CFileException e;
	if (!auth_file.Open(auth_filename, CFile::modeReadWrite, &e))
	{
		TRACE("%s is not exist\n", auth_filename);
		return FALSE;
	}
	else
	{
		TRACE("%s is exist\n", auth_filename);
		if (read_auth_data() <= 0)
		{
			auth_file.Close();
			return FALSE;
		}
		if (m_remain_count <= 0)
		{
			auth_file.Close();
			return FALSE;
		}
		m_remain_count--;
		write_auth_data();
		auth_file.Close();
		return TRUE;
	}
}
