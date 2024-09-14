#pragma once
#include "CMainTab.h"
#include <string>

class FileHandler
{
private:
	std::wstring fileName;
	std::wstring wRootDir;
	CDatabase* dbContext;
	CString rootDir;
	CString invDir;
	CString dnDir;

private:
	void LoadDirectories();
	std::string CallPythonFile(const CString folderPath, const std::wstring& fileName);
	std::string wstring_to_string(const std::wstring& wstr);

public:
	FileHandler(CWnd* pParent = nullptr, std::wstring fileName = _T(""));
	virtual ~FileHandler();
};