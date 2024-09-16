#pragma once
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <afxdb.h>

class DatabaseHandler {
public:
    DatabaseHandler(CDatabase* dbContext);
    void CreateBuyersTable();
    void CreateDocInfoTable();
    void CreateGeneralSettingsTable();
    void CreateReceiverMailsTable();
    void CreateSenderMailsTable();
    void CreateSignaturesTable();

private:
    CDatabase* dbContext;
};