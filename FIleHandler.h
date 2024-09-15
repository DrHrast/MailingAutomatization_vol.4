#pragma once
#include "CMainTab.h"
#include <string>

class FileHandler
{
private:
	std::vector<std::wstring> files;
	std::wstring fileName;
	std::wstring wRootDir;
	CDatabase* dbContext;
	CString rootDir;
	CString invDir;
	CString dnDir;
	CString recipient;
	int signatureId;
	int recipientCount;

private:
	void LoadDirectories();
	std::string CallPythonFile(const std::wstring& fileName);
	std::string wstring_to_string(const std::wstring& wstr);
	std::wstring string_to_wstring(const std::string& str);
	void CheckAllFiles();
	void SaveToDatabase(std::wstring file, bool isInv, CString vat = _T(""));
	void MoveFiles(std::wstring file, bool isInv);
	void MailingProcessing(std::wstring fileName, CString vat = _T(""));
	void GetRecipients();
	CString GetInvoiceRecipients(CString vat);
	CString GetSubject(std::wstring fileName);
	CString GetMailBody();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	void SetDatabase(CDatabase* pDatabase) { dbContext = pDatabase; }
	FileHandler(std::vector<std::wstring> files, CDatabase* dbContext);
	virtual ~FileHandler();
	void StartPoint();
};