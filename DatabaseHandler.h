#pragma once
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <string>

class DatabaseHandler {
public:
    void CheckAndCreateDatabase() {
        SQLHENV hEnv = NULL;
        SQLHDBC hDbc = NULL;
        SQLHSTMT hStmt = NULL;
        SQLRETURN retCode;

        // Initialize ODBC environment
        if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) {
            std::cerr << "Unable to allocate ODBC environment handle." << std::endl;
            return;
        }

        // Set ODBC version to 3
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

        // Allocate ODBC connection handle
        if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) {
            std::cerr << "Unable to allocate ODBC connection handle." << std::endl;
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
            return;
        }

        // Connect to SQL Server (Replace with your connection string)
        std::string connectionString = "DRIVER={SQL Server};SERVER=YourServer;UID=YourUsername;PWD=YourPassword;";
        retCode = SQLDriverConnect(hDbc, NULL, (SQLWCHAR*)connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
        if (retCode != SQL_SUCCESS && retCode != SQL_SUCCESS_WITH_INFO) {
            std::cerr << "Unable to connect to SQL Server." << std::endl;
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
            return;
        }

        // Check if database exists
        const std::string checkDatabaseQuery = "IF NOT EXISTS (SELECT name FROM sys.databases WHERE name = 'PhoenixMailingDB') "
            "BEGIN CREATE DATABASE PhoenixMailingDB END";
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        SQLExecDirect(hStmt, (SQLWCHAR*)checkDatabaseQuery.c_str(), SQL_NTS);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        // Switch to new database
        const std::string useDatabaseQuery = "USE PhoenixMailingDB";
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        SQLExecDirect(hStmt, (SQLWCHAR*)useDatabaseQuery.c_str(), SQL_NTS);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        // Create tables if not exists
        CreateTables(hDbc);

        // Disconnect and clean up
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }

private:
    void CreateTables(SQLHDBC hDbc) {
        SQLHSTMT hStmt = NULL;

        // Buyers Table
        const std::string createBuyersTableQuery =
            "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'Buyers') "
            "BEGIN "
            "CREATE TABLE Buyers ("
            "ID INT IDENTITY(1,1) PRIMARY KEY, "
            "Vat NVARCHAR(11), "
            "ShortName VARCHAR(30), "
            "Name VARCHAR(MAX), "
            "Primary_email VARCHAR(50), "
            "Secondary_email VARCHAR(50))"
            "END";
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        SQLExecDirect(hStmt, (SQLWCHAR*)createBuyersTableQuery.c_str(), SQL_NTS);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        // DocInfo Table
        const std::string createDocInfoTableQuery =
            "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'DocInfo') "
            "BEGIN "
            "CREATE TABLE DocInfo ("
            "ID INT IDENTITY(1,1) PRIMARY KEY, "
            "DocumentName VARCHAR(MAX), "
            "IsSent BIT, "
            "DateSent DATETIME, "
            "Vat VARCHAR(11))"
            "END";
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        SQLExecDirect(hStmt, (SQLWCHAR*)createDocInfoTableQuery.c_str(), SQL_NTS);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        // GeneralSettings Table
        const std::string createGeneralSettingsTableQuery =
            "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'GeneralSettings') "
            "BEGIN "
            "CREATE TABLE GeneralSettings ("
            "ID INT IDENTITY(1,1) PRIMARY KEY, "
            "RootPath VARCHAR(MAX), "
            "InvArchPath VARCHAR(MAX), "
            "DnArchPath VARCHAR(MAX), "
            "SenderMail VARCHAR(30), "
            "SignatureId INT, "
            "EndTime TIME(7))"
            "END";
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        SQLExecDirect(hStmt, (SQLWCHAR*)createGeneralSettingsTableQuery.c_str(), SQL_NTS);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        // ReceiverMails Table
        const std::string createReceiverMailsTableQuery =
            "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'ReceiverMails') "
            "BEGIN "
            "CREATE TABLE ReceiverMails ("
            "Id INT IDENTITY(1,1) PRIMARY KEY, "
            "Email VARCHAR(50))"
            "END";
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        SQLExecDirect(hStmt, (SQLWCHAR*)createReceiverMailsTableQuery.c_str(), SQL_NTS);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        // SenderMails Table
        const std::string createSenderMailsTableQuery =
            "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'SenderMails') "
            "BEGIN "
            "CREATE TABLE SenderMails ("
            "Id INT IDENTITY(1,1) PRIMARY KEY, "
            "Email VARCHAR(50))"
            "END";
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        SQLExecDirect(hStmt, (SQLWCHAR*)createSenderMailsTableQuery.c_str(), SQL_NTS);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        // Signatures Table
        const std::string createSignaturesTableQuery =
            "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'Signatures') "
            "BEGIN "
            "CREATE TABLE Signatures ("
            "ID INT IDENTITY(1,1) PRIMARY KEY, "
            "SignatureName VARCHAR(20), "
            "Signature VARCHAR(MAX))"
            "END";
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        SQLExecDirect(hStmt, (SQLWCHAR*)createSignaturesTableQuery.c_str(), SQL_NTS);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};

