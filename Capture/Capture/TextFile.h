#pragma once

class TextFile
{
public:
	TextFile();
	~TextFile();

	CStdioFile m_file;
	CString m_file_name;

	BOOL OpenFile();
	BOOL OpenFileRead();
	void CloseFile();
	void UpdateString(CString str);
	void WriteString(CString str);
	void FindScriptDir(CString Dir);
	void ReadString(CString &str);
	CString m_dir_list[100];
	int m_dir_cnt;
};

