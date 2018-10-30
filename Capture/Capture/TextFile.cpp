#include "stdafx.h"
#include "TextFile.h"


TextFile::TextFile()
{
	m_dir_cnt = 0;
}


TextFile::~TextFile()
{
}

BOOL TextFile::OpenFile()
{
	if (m_file.Open(m_file_name, CFile::modeCreate | CFile::modeReadWrite))
		return TRUE;
	else
		return FALSE;
}

BOOL TextFile::OpenFileRead()
{
	if (m_file.Open(m_file_name, CFile::modeRead))
		return TRUE;
	else
		return FALSE;
}

void TextFile::UpdateString(CString str)
{
	if (m_file.Open(m_file_name, CFile::modeWrite)) {
		m_file.SeekToEnd();
		m_file.WriteString(str);
		CloseFile();
	}
}

void TextFile::ReadString(CString &str)
{
	m_file.ReadString(str);
}

void TextFile::WriteString(CString str)
{
	m_file.WriteString(str);
}

void TextFile::CloseFile()
{
	m_file.Close();
}

void TextFile::FindScriptDir(CString dir)
{
	CFileFind finder;
	CString DirName;
	CString Dir_file = dir + _T("\\*.*");
	TRACE("Current Dir : %s\n", Dir_file);
	BOOL bWorking = finder.FindFile(Dir_file);

	while (bWorking)
	{
		//다음 파일 / 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory()) {
			DirName = finder.GetFileName();
			TRACE("Dir : %s\n", DirName);
			if (DirName != "." && DirName != "..")
			{
				CString script_name;
				script_name.Format("%s\\%s\\script.txt", dir, DirName);
				TRACE("script filename : %s\n", script_name);
				CFileStatus status;
				if (CFile::GetStatus(script_name, status))
					m_dir_list[m_dir_cnt++] = DirName;
					
			}
				
		}
	}
	for (int i = 0; i < m_dir_cnt; i++)
		TRACE("[%d] Dir : %s\n", i, m_dir_list[i]);
}
