#include "pch.h"
#include "DatabaseHandler.h"
#include "NWP_project7.h"
#include "afxdialogex.h"
#include "CSettingsTab.h"
#include <iostream>
#include <stdio.h>


DatabaseHandler::DatabaseHandler(CDatabase* dbContext) {
    this->dbContext = dbContext;
}

void DatabaseHandler::CreateBuyersTable()
{
    CString sqlQuery(_T("CREATE TABLE Buyers(")
        _T("ID INT IDENTITY(1,1) PRIMARY KEY, ")
        _T("Vat NVARCHAR(11), ")
        _T("ShortName VARCHAR(30), ")
        _T("Name VARCHAR(MAX), ")
        _T("Primary_email VARCHAR(50), ")
        _T("Secondary_email VARCHAR(50))"));

    try
    {
        dbContext->ExecuteSQL(sqlQuery);
    }
    catch (CDBException* e)
    {
        e->Delete();
    }
}

void DatabaseHandler::CreateDocInfoTable()
{
    CString sqlQuery(_T("CREATE TABLE DocInfo (")
        _T("ID INT IDENTITY(1,1) PRIMARY KEY, ")
        _T("DocumentName VARCHAR(MAX), ")
        _T("IsSent BIT, ")
        _T("DateSent DATETIME, ")
        _T("Vat VARCHAR(11))"));
    try
    {
        dbContext->ExecuteSQL(sqlQuery);
    }
    catch (CDBException* e)
    {
        e->Delete();
    }
}

void DatabaseHandler::CreateGeneralSettingsTable()
{
    CString sqlQuery(_T("CREATE TABLE GeneralSettings (")
        _T("ID INT IDENTITY(1,1) PRIMARY KEY, ")
        _T("RootPath VARCHAR(MAX), ")
        _T("InvArchPath VARCHAR(MAX), ")
        _T("DnArchPath VARCHAR(MAX), ")
        _T("SenderMail VARCHAR(30), ")
        _T("SignatureId INT, ")
        _T("EndTime TIME(7))"));
    try
    {
        dbContext->ExecuteSQL(sqlQuery);
    }
    catch (CDBException* e)
    {
        e->Delete();
    }
}

void DatabaseHandler::CreateReceiverMailsTable()
{
    CString sqlQuery(_T("CREATE TABLE ReceiverMails (")
        _T("Id INT IDENTITY(1,1) PRIMARY KEY, ")
        _T("Email VARCHAR(50))"));
    try
    {
        dbContext->ExecuteSQL(sqlQuery);
    }
    catch (CDBException* e)
    {
        e->Delete();
    }
}

void DatabaseHandler::CreateSenderMailsTable()
{
    CString sqlQuery(_T("CREATE TABLE SenderMails (")
        _T("Id INT IDENTITY(1,1) PRIMARY KEY, ")
        _T("Email VARCHAR(50))"));
    try
    {
        dbContext->ExecuteSQL(sqlQuery);
    }
    catch (CDBException* e)
    {
        e->Delete();
    }
}

void DatabaseHandler::CreateSignaturesTable()
{
    CString sqlQuery(_T("CREATE TABLE Signatures (")
        _T("ID INT IDENTITY(1,1) PRIMARY KEY, ")
        _T("SignatureName VARCHAR(20), ")
        _T("Signature VARCHAR(MAX))"));
    try
    {
        dbContext->ExecuteSQL(sqlQuery);
    }
    catch (CDBException* e)
    {
        e->Delete();
    }
}