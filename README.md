# MailingAutomatization_vol.4

## Instalation prerequisites:
  * Installed Python - preferably [Python 3.12.6](https://www.python.org/ftp/python/3.12.6/python-3.12.6-amd64.exe)
  * In project **Property** set:
    * Configuration Propeties > C/C++ > General > Additional Include Directories > **"..\Python3x\include"**[^1]
    * Configuration Propeties >  Linker > General > Additional Library Directories > **"..\Python3x\libs"**[^1]
    * Configuration Propeties >  Linker > Input > Additional Dependencies > **python3x_d.lib;python3x.lib**[^1]
    * Sometimes python files (python3.dll, python3_d.dll, python3x.dll[^1], python3x_d.dll[^1]) need to be moved to project folder
  * Setup ODBC Data Source: ODBC Data Source Administrator > User DSN: Add a new Data Source with SQL Server
    * Name set to **PhoenixMailingDB**[^2]
    * Server set to local sql server
    * First set default database to master and later after first run of the app change to **PhoenixMailingDB**[^3]

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


[^1]: Python version: 'x' is '9' if python version is 3.9 or 'x' would be '12' for version 3.12
[^2]: Refers to DSN name
[^3]: Refers to database name
[^4]: Receiver emails are emails that receive delivery notes (all receivers)
