#include "pch.h"
#include "FileHandler.h"
#include <string>
#include <Python.h>
#include <locale>
#include <codecvt>


FileHandler::FileHandler(CWnd* pParent, std::wstring fileName)
{
	this->fileName = fileName;
}

FileHandler::~FileHandler()
{
}

void FileHandler::LoadDirectories() {
	CRecordset recordset(dbContext);

	//GeneralSettings table
	CString sqlQuery = _T("SELECT * FROM GeneralSettings ORDER BY DESC");
	recordset.Open(CRecordset::forwardOnly, sqlQuery);

	recordset.GetFieldValue(_T("RootPath"), rootDir);
	recordset.GetFieldValue(_T("InvArchPath"), invDir);
	recordset.GetFieldValue(_T("DnArchPath"), dnDir);
	recordset.Close();

	std::wstring wRootDir(rootDir.GetString());
}

std::string FileHandler::wstring_to_string(const std::wstring& wstr)
{
	// Use the conversion facet to convert wide string to narrow string
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstr);
}

std::string FileHandler::CallPythonFile(const CString folderPath, const std::wstring& fileName)
{
	PyObject* pName, * pModule, * pFunc;
	PyObject* pArgs, * pValue;
	std::string result = "";

	Py_Initialize();

	pName = PyUnicode_DecodeFSDefault("PDFReader_VatIsolation");

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != nullptr) {
		pFunc = PyObject_GetAttrString(pModule, "oib_isolation");

		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(2);
			PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(wstring_to_string(wRootDir).c_str()));
			PyTuple_SetItem(pArgs, 1, PyUnicode_FromString(wstring_to_string(fileName).c_str()));

			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);

			if (pValue != nullptr) {
				result = PyUnicode_AsUTF8(pValue);
				Py_DECREF(pValue);
			}
			else {
				//Error handle - call faild
			}
		}
		else {
			//Error handle - python function not found
		}

		Py_XDECREF(pFunc);
		Py_DECREF(pModule);

	}
	else {
		//Error handle - module not loaded
	}

	Py_Finalize();

	AfxMessageBox(_T("Vat: '%s", result));

	return result;
}
