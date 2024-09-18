# MailingAutomatization_vol.4

## Instalation prerequisites:
  * Installed Python - preferably [Python 3.12.6](https://www.python.org/ftp/python/3.12.6/python-3.12.6-amd64.exe)
  * In project **Property** set:
    * Configuration Propeties > C/C++ > General > Additional Include Directories > **"..\Python3x\include"**[^1]
    * Configuration Propeties >  Linker > General > Additional Library Directories > **"..\Python3x\libs"**[^1]
    * Configuration Propeties >  Linker > Input > Additional Dependencies > **python3x_d.lib;python3x.lib**[^1]
    * Run python.exe, enter line **pip install PyPDF2**
    * Sometimes python files (python3.dll, python3_d.dll, python3x.dll[^1], python3x_d.dll[^1]) need to be moved to project folder
    * In FileHandler.cpp enter python script directory in this line: *PyList_Append(sysPath, PyUnicode_FromString("..\\pythonScriptFolder"));*
    * In FileHandler.cpp enter python site-pacages folder directory in this line: *PyList_Append(sysPath, PyUnicode_FromString("..\\Python\\Python312\\Lib\\site-packages"));*
  * [Install SQL Management Studio (SSMS)](https://learn.microsoft.com/en-us/sql/ssms/download-sql-server-management-studio-ssms?view=sql-server-ver16#download-ssms)
  * Create databse **PhoenixMailingDB** ![Screenshot (1)](https://github.com/user-attachments/assets/0e976be0-2679-4dc8-9416-06fda7bfc223)
  * Setup ODBC Data Source: ODBC Data Source Administrator > User DSN: Add a new Data Source with SQL Server
    * Name set to **PhoenixMailingDB**[^2]
    * Server set to local sql server
    * Set default database to **PhoenixMailingDB**[^3]

## User Instructions:
  This is tab control based, single document MFC application. Tabs are as follows: Specific, BuyerData and AdditionalSettings.
  ### Specific:
   * Directory control group:
     * Root directory - directory from where application is taking all files
     * Ivnoice archive directory - directory where invoices are moved
     * Delivery notes directory - directory where delivery notes are moved
   * Sender email: specifies what email address will email be sent from
   * Email signature: specifies what will be a body of an email
   * Start button: runs mail logic
  ### BuyerData:
   * Add new buyer group
   * Update exsisting buyer group
   * Buyer list: lists all saved buyers with their VAT number, name, primary and secondary email address
  ### AdditionalSettings:
   * Group to add new sender to the database - it will show in combobox on first tab
   * Group to add ner receiver[^4] to the database
   * Receiver list view that lists all receiver email addresses
   * Add signature group: allows user to add a email body content, signature name will show in combobox on first tab

## Application Flow:
  1. Directory Monitoring:
     * When the "Start" button is pressed, the application checks the folder set in the "Root Directory." It scans for any files present in that folder.

  2. File Processing:
     * If a file is named "Racun", the application triggers a Python script to process the file and extract the VAT (Value Added Tax) number from it.
     * If the file is named something else, it is skipped from further processing.

  3. VAT Lookup and Email Dispatch:
     * After extracting the VAT from the "Racun" file, the application searches the Buyers database for a matching VAT number.
     * If a matching VAT is found, the application retrieves all associated email addresses for that buyer. It prepares to send an email to all these addresses.
     * If the file is skipped (i.e., not named "Racun"), the application proceeds by preparing to send an email to all email addresses listed in the RecipientsMails table.

  4. Sending Emails via Outlook:
     * The email is sent using the Windows API, which opens Microsoft Outlook and sends the email from the sender email selected in the Sender Email combo box.

  5. Document Information Logging:
     * After sending the email, all relevant document information is saved to the DocInfo table for future reference or potential expansion of the application.


[^1]: Python version: 'x' is '9' if python version is 3.9 or 'x' would be '12' for version 3.12
[^2]: Refers to DSN name
[^3]: Refers to database name
[^4]: Receiver emails are emails that receive delivery notes (all receivers)
