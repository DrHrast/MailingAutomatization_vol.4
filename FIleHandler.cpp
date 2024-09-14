#include "pch.h"
#include "FileHandler.h"
#include <string>
#include <Python.h>
#include <locale>
#include <codecvt>
#include <filesystem>
#include <iostream>
#include "Windows.h"

namespace fs = std::filesystem;


FileHandler::FileHandler(std::vector<std::wstring> files, CDatabase* dbContext)
{
	this->files = files;
	this->dbContext = dbContext;
}

FileHandler::~FileHandler()
{
}

void FileHandler::StartPoint()
{
	LoadDirectories();
	CheckAllFiles();
}

void FileHandler::LoadDirectories() {
	CRecordset recordset(dbContext);

	//GeneralSettings table
	CString sqlQuery = _T("SELECT * FROM GeneralSettings ORDER BY ID DESC");
	recordset.Open(CRecordset::forwardOnly, sqlQuery);

	// Ensure you are actually getting valid values
	recordset.GetFieldValue(_T("RootPath"), rootDir);
	recordset.GetFieldValue(_T("InvArchPath"), invDir);
	recordset.GetFieldValue(_T("DnArchPath"), dnDir);

	AfxMessageBox(rootDir);  // Display rootDir to make sure it's valid
	recordset.Close();
}

void FileHandler::SaveToDatabase(std::wstring file, bool isInv, CString vat) {

	CString cFile(file.c_str());

	SYSTEMTIME st;
	GetLocalTime(&st);

	int boolInv;
	boolInv = isInv ? 1 : 0;

	CString currentDateTime;
	currentDateTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	CString sqlQuery;
	sqlQuery.Format(_T("INSERT INTO DocInfo (DocumentName, IsSent, DateSent, Vat) VALUES ('%s', '%d', '%s', '%s')"),
		cFile, boolInv, currentDateTime, vat);

	try {
		dbContext->ExecuteSQL(sqlQuery);
		//AfxMessageBox(_T("Record saved successfully!"));
	}
	catch (CDBException* e) {
		CString errorMsg;
		errorMsg.Format(_T("Failed to save record: '%s'"), e->m_strError);
		//AfxMessageBox(errorMsg);
		e->Delete();
	}
}

void FileHandler::MoveFiles(std::wstring file, bool isInv)
{
	std::wstring fileName = file.substr(file.find_last_of(L"\\") + 1);
	std::wstring root = rootDir.GetString();
	std::wstring destinationDir = isInv ? std::wstring(invDir.GetString()) : std::wstring(dnDir.GetString());
	// Build source and destination paths
	std::wstring sourcePath = root + L"\\" + fileName;
	std::wstring destinationPath = destinationDir + L"\\" + fileName;
	// Move the file using filesystem API
	//AfxMessageBox(CString(sourcePath.c_str()));
	//AfxMessageBox(CString(destinationDir.c_str()));

	try {
		if (fs::exists(destinationPath)) {
			AfxMessageBox(_T("File already exists in the destination"));
			return;
		}
		// Attempt to rename (move)
		fs::rename(sourcePath, destinationPath);
	}
	catch (const fs::filesystem_error& e) {
		// If rename fails, try copying instead (for cross-filesystem moves)
		try {
			fs::copy(sourcePath, destinationPath);
			fs::remove(sourcePath);  // Remove the source file after copying
		}
		catch (const fs::filesystem_error& e) {
			CString errorMsg;
			errorMsg.Format(_T("Error copying file: '%s'"), e.what());
			AfxMessageBox(errorMsg);
		}
	}
}

void FileHandler::DoDataExchange(CDataExchange* pDX)
{
}

void FileHandler::CheckAllFiles() {
	for each (std::wstring file in files) {
		std::wstring fileType = file.substr(0, 2);
		if (fileType.compare(L"Ra") == 0) {
			//DID_IT: Call python script
			std::string extractedVat;
			extractedVat = CallPythonFile(file);
			CString vat(extractedVat.c_str());
			//DID_IT: Save to a database
			SaveToDatabase(file, true, vat);
			
			//TODO: Forward info for mail processing
			
			//DID_IT: Move file into a folder
			MoveFiles(file, true);
		}
		else {
			//DID_IT: Save to a database
			SaveToDatabase(file, false);

			//TODO: Forward info for mail processing
			
			//DID_IT: Move file into a folder
			MoveFiles(file, false);
		}
	}
}

std::string FileHandler::wstring_to_string(const std::wstring& wstr)
{
	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), NULL, 0, NULL, NULL);
	std::string str(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), &str[0], sizeNeeded, NULL, NULL);
	return str;
}

std::wstring string_to_wstring(const std::string& str)
{
	// Determine the required size for the converted string
	int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), NULL, 0);
	std::wstring wstr(sizeNeeded, 0);

	// Perform the conversion
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstr[0], sizeNeeded);

	return wstr;
}

std::string FileHandler::CallPythonFile(const std::wstring& fileName)
{
	PyObject* pName, * pModule, * pFunc;
	PyObject* pArgs, * pValue;
	std::string result = "";

	Py_Initialize();

	// Add the path to your script folder
	PyObject* sysPath = PySys_GetObject("path");
	PyList_Append(sysPath, PyUnicode_FromString("C:\\Faks\\NWP\\Project\\NWP_project7\\NWP_project7"));
	PyList_Append(sysPath, PyUnicode_FromString("C:\\Users\\Korisnik\\AppData\\Local\\Programs\\Python\\Python312\\Lib\\site-packages"));

	// Load your script
	pName = PyUnicode_DecodeFSDefault("PDFReader_VatIsolation");  // No .py extension
	if (!pName) {
		AfxMessageBox(_T("Failed to decode module name."));
		Py_Finalize();
		return result;
	}

	// Import the module
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	if (pModule != nullptr) {
		// Load the function
		pFunc = PyObject_GetAttrString(pModule, "oib_isolation");
		if (pFunc && PyCallable_Check(pFunc)) {
			std::string rootPath(CT2A(rootDir.GetString()));
			std::string file(wstring_to_string(fileName));

			/*CString debugMessage;
			debugMessage.Format(_T("rootPath: %s\nfile: %s"), CString(rootPath.c_str()), CString(file.c_str()));
			AfxMessageBox(debugMessage);*/

			// Prepare arguments for the Python function
			pArgs = PyTuple_Pack(2, PyUnicode_FromString(rootPath.c_str()),
				PyUnicode_FromString(file.c_str()));

			// Call the function
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);

			if (pValue != nullptr) {
				result = PyUnicode_AsUTF8(pValue);  // Get result as string
				Py_DECREF(pValue);
			}
			else {
				AfxMessageBox(_T("Error: Function call failed"));
				PyErr_Print();  // Print error message
			}
		}
		else {
			AfxMessageBox(_T("Error: Python function not found"));
			PyErr_Print();  // Print error message
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else {
		AfxMessageBox(_T("Error: Python module not loaded"));
		PyErr_Print();  // Print error message
	}

	// Return the extracted result
	/*CString cExtractedVat(result.c_str());
	AfxMessageBox(cExtractedVat);*/

	return result;
}