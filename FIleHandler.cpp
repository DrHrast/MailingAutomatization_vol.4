#include "pch.h"
#include "FileHandler.h"
#include <string>
#include <Python.h>
#include <locale>
#include <codecvt>
#include <filesystem>
#include <iostream>
#include "Windows.h"
#include <MAPI.h>

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
	GetRecipients();
	LoadDirectories();
	CheckAllFiles();
}

void FileHandler::LoadDirectories() {
	CRecordset recordset(dbContext);

	//GeneralSettings table
	CString sqlQuery = _T("SELECT * FROM GeneralSettings ORDER BY ID DESC");
	recordset.Open(CRecordset::forwardOnly, sqlQuery);

	CString idValue;

	// Ensure you are actually getting valid values
	recordset.GetFieldValue(_T("RootPath"), rootDir);
	recordset.GetFieldValue(_T("InvArchPath"), invDir);
	recordset.GetFieldValue(_T("DnArchPath"), dnDir);
	recordset.GetFieldValue(_T("SignatureId"), idValue);

	signatureId = _ttoi(idValue);
	//AfxMessageBox(rootDir);
	recordset.Close();
	return;
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
	CString escapedFileName = cFile;
	escapedFileName.Replace(_T("'"), _T("''"));  // Escape single quotes by replacing ' with ''
	sqlQuery.Format(_T("INSERT INTO DocInfo (DocumentName, IsSent, DateSent, Vat) VALUES ('%s', '%d', '%s', '%s')"),
		escapedFileName, boolInv, currentDateTime, vat);

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

	//Return the extracted result
	/*CString cExtractedVat(result.c_str());
	AfxMessageBox(cExtractedVat);*/

	return result;
}
//
//void FileHandler::MailingProcessing(std::wstring fileName, CString vat)
//{
//	HMODULE hMapi = ::LoadLibrary(_T("MAPI32.DLL"));
//	if (!hMapi) {
//		AfxMessageBox(_T("MAPI32.DLL failed to load."));
//		return;
//	}
//
//	// Get function pointers to MAPI functions
//	LPMAPISENDMAIL pMapiSendMail = (LPMAPISENDMAIL)GetProcAddress(hMapi, "MAPISendMail");
//	if (!pMapiSendMail) {
//		AfxMessageBox(_T("pMapiSendMail is NULL. Could not load MAPI function."));
//		::FreeLibrary(hMapi);
//		return;
//	}
//
//	MapiMessage message = { 0 };
//	MapiRecipDesc recipientDesc = { 0 };
//
//	//AfxMessageBox(vat);
//
//	CStringA recipientAnsi;
//	CStringA subjectAnsi(GetSubject(fileName));
//	CStringA bodyAnsi(GetMailBody());
//
//	//Delivery notes
//	if (vat.IsEmpty()) {
//
//		recipientDesc.ulRecipClass = MAPI_TO;
//		recipientAnsi = recipient;
//		recipientDesc.lpszName = recipientAnsi.GetBuffer();
//		message.nRecipCount = recipientCount;
//		message.lpRecips = &recipientDesc;
//
//	}
//	//Invoices
//	else {
//
//		recipientDesc.ulRecipClass = MAPI_TO;
//		/*CStringA buyerRecipientAnsi(GetInvoiceRecipients(vat));
//		recipientDesc.lpszName = (LPSTR)(LPCSTR)buyerRecipientAnsi;*/
//		recipientAnsi = recipient;
//		recipientDesc.lpszName = recipientAnsi.GetBuffer();
//		CString rec(GetInvoiceRecipients(vat));
//		AfxMessageBox(rec);
//		//message.nRecipCount = 2;
//		message.nRecipCount = recipientCount;
//		message.lpRecips = &recipientDesc;
//	}
//
//	message.lpszSubject = subjectAnsi.GetBuffer();
//	message.lpszNoteText = bodyAnsi.GetBuffer();
//
//	CString attachmentPath(rootDir + _T("\\") + fileName.c_str());
//	if (!attachmentPath.IsEmpty()) {
//		MapiFileDesc fileDesc = { 0 };
//		fileDesc.nPosition = (ULONG)-1;
//		CStringA attPathAnsi(attachmentPath);
//		fileDesc.lpszPathName = attPathAnsi.GetBuffer();
//		message.nFileCount = 1;
//		message.lpFiles = &fileDesc;
//	}
//
//	// Send the mail
//	ULONG mailResult = pMapiSendMail(0L, 0L, &message, MAPI_LOGON_UI | MAPI_DIALOG, 0L);
//
//	if (mailResult != SUCCESS_SUCCESS) {
//		CString errorMsg;
//		switch (mailResult) {
//		case MAPI_E_FAILURE:
//			errorMsg = _T("General MAPI failure.");
//			break;
//		case MAPI_E_INSUFFICIENT_MEMORY:
//			errorMsg = _T("Insufficient memory to proceed.");
//			break;
//		case MAPI_E_LOGIN_FAILURE:
//			errorMsg = _T("Login failure.");
//			break;
//		case MAPI_E_USER_ABORT:
//			errorMsg = _T("User aborted the mail operation.");
//			break;
//		case MAPI_E_TOO_MANY_FILES:
//			errorMsg = _T("Too many file attachments.");
//			break;
//		case MAPI_E_TOO_MANY_RECIPIENTS:
//			errorMsg = _T("Too many recipients.");
//			break;
//		case MAPI_E_ATTACHMENT_NOT_FOUND:
//			errorMsg = _T("Attachment not found.");
//			break;
//		case MAPI_E_ATTACHMENT_OPEN_FAILURE:
//			errorMsg = _T("Attachment open failure.");
//			break;
//		default:
//			errorMsg.Format(_T("Unknown MAPI error code: %lu"), mailResult);
//			break;
//		}
//		AfxMessageBox(errorMsg);
//	}
//	else {
//		AfxMessageBox(_T("Mail sent successfully!"));
//	}
//}

void FileHandler::MailingProcessing(std::wstring fileName, CString vat)
{

	HMODULE hMapi = ::LoadLibrary(_T("MAPI32.DLL"));
	if (!hMapi) {
		AfxMessageBox(_T("MAPI32.DLL failed to load."));
		return;
	}

	// Get function pointers to MAPI functions
	LPMAPISENDMAIL pMapiSendMail = (LPMAPISENDMAIL)GetProcAddress(hMapi, "MAPISendMail");
	if (!pMapiSendMail) {
		::FreeLibrary(hMapi);
		return;
	}

	MapiMessage message = { 0 };
	std::vector<MapiRecipDesc> recipients;

	CStringA subjectAnsi(GetSubject(fileName));
	CStringA bodyAnsi(GetMailBody());

	// Handling recipients for Delivery notes or Invoices based on VAT
	if (vat.IsEmpty()) {

		// Split the recipient list by ';'
		int pos = 0;
		CString token;
		CString emailList = recipient;

		while (!(token = emailList.Tokenize(_T(";"), pos)).IsEmpty()) {
			if (!token.Trim().IsEmpty()) {
				MapiRecipDesc recipientDesc = { 0 };
				recipientDesc.ulRecipClass = MAPI_TO;
				CStringA recipientAnsi(token);
				recipientDesc.lpszName = _strdup((LPSTR)(LPCSTR)recipientAnsi);
				recipients.push_back(recipientDesc);
			}
		}
	}
	else {

		//CString invoiceRecipient = GetInvoiceRecipients(vat);
		//AfxMessageBox(_T("Invoice recipient(s): ") + invoiceRecipient);

		int pos = 0;
		CString token;

		while (!(token = recipient.Tokenize(_T(";"), pos)).IsEmpty()) {
			if (!token.Trim().IsEmpty()) {
				MapiRecipDesc recipientDesc = { 0 };
				recipientDesc.ulRecipClass = MAPI_TO;
				CStringA recipientAnsi(token);
				recipientDesc.lpszName = _strdup((LPSTR)(LPCSTR)recipientAnsi);
				recipients.push_back(recipientDesc);
			}
		}
	}

	// Set the message details
	message.lpszSubject = (LPSTR)(LPCSTR)subjectAnsi;
	message.lpszNoteText = (LPSTR)(LPCSTR)bodyAnsi;

	// Attach recipients to the message
	message.nRecipCount = (ULONG)recipients.size();
	message.lpRecips = recipients.data();

	// Attachments
	CString attachmentPath = rootDir + _T("\\") + fileName.c_str();

	if (!attachmentPath.IsEmpty()) {
		if (_waccess(attachmentPath, 0) == -1) {
			AfxMessageBox(_T("Attachment file does not exist!"));
			return;
		}

		MapiFileDesc fileDesc = { 0 };
		fileDesc.nPosition = (ULONG)-1;
		CStringA attPathAnsi(attachmentPath);
		fileDesc.lpszPathName = _strdup(attPathAnsi);
		message.nFileCount = 1;
		message.lpFiles = &fileDesc;
	}

	// Send the mail (without showing the draft UI)
	ULONG mailResult = pMapiSendMail(0L, 0L, &message, MAPI_LOGON_UI, 0L);

	if (mailResult != SUCCESS_SUCCESS) {
		CString errorMsg;
		switch (mailResult) {
		case MAPI_E_FAILURE:
			errorMsg = _T("General MAPI failure.");
			break;
		case MAPI_E_INSUFFICIENT_MEMORY:
			errorMsg = _T("Insufficient memory to proceed.");
			break;
		case MAPI_E_LOGIN_FAILURE:
			errorMsg = _T("Login failure.");
			break;
		case MAPI_E_USER_ABORT:
			errorMsg = _T("User aborted the mail operation.");
			break;
		case MAPI_E_TOO_MANY_FILES:
			errorMsg = _T("Too many file attachments.");
			break;
		case MAPI_E_TOO_MANY_RECIPIENTS:
			errorMsg = _T("Too many recipients.");
			break;
		case MAPI_E_ATTACHMENT_NOT_FOUND:
			errorMsg = _T("Attachment not found.");
			break;
		case MAPI_E_ATTACHMENT_OPEN_FAILURE:
			errorMsg = _T("Attachment open failure.");
			break;
		default:
			errorMsg.Format(_T("Unknown MAPI error code: %lu"), mailResult);
			break;
		}
		AfxMessageBox(errorMsg);
	}

	::FreeLibrary(hMapi);

	// Free allocated memory for recipients
	for (auto& rec : recipients) {
		free(rec.lpszName);
	}
}

void FileHandler::GetRecipients()
{
	recipientCount = 0;
	if (!dbContext || !dbContext->IsOpen()) {
		AfxMessageBox(_T("Database context is not open."));
		return;
	}

	CRecordset recordset(dbContext);
	CString sqlQuery = _T("SELECT * FROM ReceiverMails");

	try {
		recordset.Open(CRecordset::forwardOnly, sqlQuery);
	}
	catch (CDBException* e) {
		AfxMessageBox(_T("Error opening recordset for ReceiverMails."));
		e->Delete();
		return;
	}

	while (!recordset.IsEOF()) {
		CString email;
		recordset.GetFieldValue(_T("Email"), email);
		recipient += email + _T(";");
		recordset.MoveNext();
		recipientCount++;
	}
	recordset.Close();
	//AfxMessageBox(_T("Recipients loaded successfully."));
}

CString FileHandler::GetInvoiceRecipients(CString vat)
{

	//Incorrect syntax somewere here. Needs fixing. But only for release.
	CRecordset recordset(dbContext);
	CString sqlQuery = _T("SELECT (PrimaryEmail, SecondaryEmail) FROM Buyers WHERE Vat = '%s'", vat);
	recordset.Open(CRecordset::forwardOnly, sqlQuery);
	CString prim, second;
	recordset.GetFieldValue(_T("PrimaryEmail"), prim);
	recordset.GetFieldValue(_T("SecondaryEmail"), second);

	prim = prim + _T(";") + second;
	return prim;
}


CString FileHandler::GetSubject(std::wstring fileName)
{
	CString subject;
	subject.Format(_T("Biomax %s"), fileName.c_str());

	return subject;
}

CString FileHandler::GetMailBody()
{
	CRecordset recordset(dbContext);
	CString sqlQuery;
	sqlQuery = _T("SELECT * FROM Signatures");
	recordset.Open(CRecordset::forwardOnly, sqlQuery);
	while (!recordset.IsEOF()) 
	{
		CString temp;
		recordset.GetFieldValue(_T("ID"), temp);
		if (_ttoi(temp) == signatureId) {
			CString content;
			recordset.GetFieldValue(_T("Signature"), content);
			return content;
		}
	}
	recordset.Close();
}

void FileHandler::CheckAllFiles() {
	for each (std::wstring file in files) {
		std::wstring fileType = file.substr(0, 2);
		if (fileType.compare(L"Ra") == 0) {
			//DID_IT: Call python script
			std::string extractedVat = CallPythonFile(file);
			std::wstring wstrVat = std::wstring(extractedVat.begin(), extractedVat.end());
			CString vat(wstrVat.c_str());

			//DID_IT: Save to a database
			SaveToDatabase(file, true, vat);
			
			//TODO: Forward info for mail processing
			MailingProcessing(file, vat);

			//DID_IT: Move file into a folder
			MoveFiles(file, true);
		}
		else {
			//DID_IT: Save to a database
			SaveToDatabase(file, false);

			//TODO: Forward info for mail processing
			MailingProcessing(file);
			
			//DID_IT: Move file into a folder
			MoveFiles(file, false);
		}
	}
}