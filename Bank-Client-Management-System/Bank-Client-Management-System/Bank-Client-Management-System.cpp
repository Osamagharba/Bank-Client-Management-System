#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <conio.h>
#include "MyLibrary.h"

using namespace input;
using namespace Print_String_Format;
using namespace std;

string const FileName = "Clients.txt";

enum enMainMenueOptions 
{ 
    enShowClientList = 1, enAddNewClient = 2,
    enDeleteClient = 3, enUpdateClientInfo = 4,
    enFindClient = 5, enShowTransactionsMenue = 6, enExit = 7 
};

enum enTransactionsMenueOptions
{
    eDeposit = 1, eWithdraw = 2,
    eShowTotalBalance = 3, eShowMainMenue = 4 
};

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0.0;
    bool MarkForDelete = false;
};

void ShowMainMenue();
void TransactionsMenueScreen();

vector <string> SplitString(string Line, string Delim)
{
    vector <string> vString;
    int pos = 0;
    string sWord = "";

    while ((pos = Line.find(Delim)) != std::string::npos)
    {
        sWord = Line.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        Line.erase(0, pos + Delim.length());
    }
    if (Line != "")
    {
        vString.push_back(Line);
    }
    return vString;
}

sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
    sClient Cli;
    vector <string> vClientData;
    vClientData = SplitString(Line, Seperator);
    Cli.AccountNumber = vClientData[0];
    Cli.PinCode = vClientData[1];
    Cli.Name = vClientData[2];
    Cli.Phone = vClientData[3];
    Cli.AccountBalance = stoi(vClientData[4]); //cast string to double
    return Cli;

}

string ConvertRecordToLine(sClient Cli, string Seperator = "#//#")
{
    string Line = "";
    Line += Cli.AccountNumber + Seperator;
    Line += Cli.PinCode + Seperator;
    Line += Cli.Name + Seperator;
    Line += Cli.Phone + Seperator;
    Line += to_string(Cli.AccountBalance);
    return Line;
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream ClientsFile;
    ClientsFile.open(FileName, ios::in);

    if (ClientsFile.is_open())
    {
        string Line;
        sClient Cli;
        while (getline(ClientsFile, Line))
        {
            Cli = ConvertLineToRecord(Line);
            vClients.push_back(Cli);
        }
        ClientsFile.close();
    }
    return vClients;
}

sClient ReadClientData(string AccountNumber)
{
    sClient Cli;
    Cli.AccountNumber = AccountNumber;
    cout << "\nEnter PinCode: ";
    getline(cin, Cli.PinCode);
    cout << "Enter Name: ";
    getline(cin, Cli.Name);
    cout << "Enter Phone: ";
    getline(cin, Cli.Phone);
    cout << "Enter Account Balance: ";
    cin >> Cli.AccountBalance;
    return Cli;
}

void SaveClientsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream ClientsFile;
    ClientsFile.open(FileName, ios::out);
    string DataLine;
    if (ClientsFile.is_open())
    {
        for (sClient Cli : vClients)
        {
            if (Cli.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(Cli);
                ClientsFile << DataLine << endl;
            }
        }
        ClientsFile.close();
    }
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& Cli : vClients)
    {
        if (Cli.AccountNumber == AccountNumber)
        {
            Cli.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool MarkClientForUpdateByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& Cli : vClients)
    {
        if (Cli.AccountNumber == AccountNumber)
        {
            Cli = ReadClientData(AccountNumber);
            return true;
        }
    }
    return false;
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Cli)
{
    for (sClient C : vClients)
    {
        if (AccountNumber == C.AccountNumber)
        {
            Cli = C;
            return true;
        }
    }
    return false;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

void PrintMainMenueScreen()
{
    system("cls");
    cout << "=================================================\n";
    cout << "                Main Menue Screen                \n";
    cout << "=================================================\n";
    cout << "        [1] Show Client List.\n";
    cout << "        [2] Add New Client.\n";
    cout << "        [3] Delete Client.\n";
    cout << "        [4] Update Client Info.\n";
    cout << "        [5] Find Client.\n";
    cout << "        [6] Exit.\n";
    cout << "=================================================" << endl;
}

void PrintClientDetails(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << u8"┌" << PrintLine(u8"─", Client.Name.length() + 19) << u8"┐";
    cout << u8"\n│ Account Number : " << left << setw(Client.Name.length() + 1) <<  Client.AccountNumber << u8"│";
    cout << u8"\n│ Pin Code       : " << left << setw(Client.Name.length() + 1) << Client.PinCode << u8"│";
    cout << u8"\n│ Name           : " << left << setw(Client.Name.length() + 1) << Client.Name << u8"│";
    cout << u8"\n│ Phone          : " << left << setw(Client.Name.length() + 1) << Client.Phone << u8"│";
    cout << u8"\n│ Account Balance: " << left << setw(Client.Name.length() + 1) << Client.AccountBalance << u8"│";
    cout << endl << u8"└" << PrintLine(u8"─", Client.Name.length() + 19) << u8"┘" << endl;
}

void ShowAllClientsScreen()
{
    vector <sClient> vClients = LoadClientsDataFromFile(FileName);
    system("cls");
    cout << "                                 Client List (" << vClients.size() << ") Client(s).                                 ";
    cout << u8"\n┌────────────────┬───────────┬─────────────────────────────────────┬──────────────┬──────────┐\n";
    cout << u8"│ " << left << setw(15) << "Account Number";
    cout << u8"│ " << left << setw(10) << "Pin Code";
    cout << u8"│ " << left << setw(36) << "Client Name";
    cout << u8"│ " << left << setw(13) << "Phone";
    cout << u8"│ " << left << setw(9) << "Balance" << u8"│";
    cout << u8"\n├────────────────┼───────────┼─────────────────────────────────────┼──────────────┼──────────┤\n";

    if (vClients.size() == 0)
    {
        cout << "\t\t\t\tNo Clients Avaliable In The System!";
    }
    else
    {
        for (sClient Cli : vClients)
        {
            cout << u8"│ " << left << setw(15) << Cli.AccountNumber;
            cout << u8"│ " << left << setw(10) << Cli.PinCode;
            cout << u8"│ " << left << setw(36) << Cli.Name;
            cout << u8"│ " << left << setw(13) << Cli.Phone;
            cout << u8"│ " << left << setw(9) << Cli.AccountBalance << u8"│" << endl;
        }
    }

    cout << u8"└────────────────┴───────────┴─────────────────────────────────────┴──────────────┴──────────┘\n";

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Cli;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Cli))
    {
        PrintClientDetails(Cli);
        cout << "\n\nAre you sure you want delete this client? y/n: ";
        cin >> Answer;
        
        if (toupper(Answer) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(FileName, vClients);

            cout << "\nClient Delete Sucessfully.";
        }
    }
    return false;
}

void ShowDeleteClientsScreen()
{
    cout << PrintLine("-", 50) << endl;
    cout << PrintLine(" ", 10) << "Delete Client Screen\n";
    cout << PrintLine("-", 50) << endl;

    vector <sClient> vClients = LoadClientsDataFromFile(FileName);

    cin.ignore(1, '\n');
    string AccountNumber = ReadString("\nEnter Account Number: ");

    DeleteClientByAccountNumber(AccountNumber, vClients);
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Cli;
    Cli.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode: ";
    getline(cin >> ws, Cli.PinCode);

    cout << "Enter Name: ";
    getline(cin, Cli.Name);

    cout << "Enter Phone: ";
    getline(cin, Cli.Phone);

    cout << "Enter Account Balance: ";
    cin >> Cli.AccountBalance;

    return Cli;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient> vClients)
{
    sClient Cli;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Cli))
    {
        PrintClientDetails(Cli);
        cout << "\n\nAre you sure you want update this client? y/n: ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (sClient& Cli : vClients)
            {
                if (Cli.AccountNumber == AccountNumber)
                {
                    Cli = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveClientsDataToFile(FileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
            
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";

        cout << "\n\nPress any key to go back to Main Menue...";
        return false;
    }
}

void ShowUpdateClientsScreen()
{
    cout << PrintLine("-", 50) << endl;
    cout << PrintLine(" ", 6) << "Update Client Info Screen\n";
    cout << PrintLine("-", 50) << endl;

    vector <sClient> vClients = LoadClientsDataFromFile(FileName);
    cin.ignore(1, '\n');
    string AccountNumber = ReadString("\nEnter Account Number: ");
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector <sClient> vClient;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClient.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}

bool DeopsitBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient> vClients)
{
    char choice = 'n';

    cout << "\nAre your sure you want to perform this transaction? y/n: ";
    cin >> choice;

    if (toupper(choice) == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (AccountNumber == C.AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(FileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
    }
}

void Deposit()
{
    cout << PrintLine("-", 50) << endl;
    cout << PrintLine(" ", 18) << "Deposit Screen\n";
    cout << PrintLine("-", 50) << endl;

    vector <sClient> vClients = LoadClientsDataFromFile(FileName);
    sClient Cli;
    string AccountNumber = "";
    while (!FindClientByAccountNumber(AccountNumber, vClients, Cli))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadString("Enter Account Number: ");

    }

    PrintClientDetails(Cli);

    float DepositAmount = 0;
    cout << "Please enter deposit amount: ";
    cin >> DepositAmount;

    DeopsitBalanceToClientByAccountNumber(AccountNumber, DepositAmount, vClients);
}

void Withdraw()
{
    cout << PrintLine("-", 50) << endl;
    cout << PrintLine(" ", 18) << "Withdraw Screen\n";
    cout << PrintLine("-", 50) << endl;

    vector <sClient> vClients = LoadClientsDataFromFile(FileName);
    sClient Cli;
    string AccountNumber = "";
    do
    {
        AccountNumber = ReadString("Enter Account Number: ");

    } while (!FindClientByAccountNumber(AccountNumber, vClients, Cli));

    PrintClientDetails(Cli);

    float WithdrawAmount = 0;
    cout << "Please enter withdraw amount: ";
    cin >> WithdrawAmount;
    
    while (Cli.AccountBalance < WithdrawAmount)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to: " << Cli.AccountBalance << endl;
        cout << "Please enter another amount: ";
        cin >> WithdrawAmount;
    }

    DeopsitBalanceToClientByAccountNumber(AccountNumber, WithdrawAmount * -1, vClients);
}

sClient ReadNewClient()
{
    sClient Cli;
    cout << "Enter Account Number: ";
    getline(cin >> ws, Cli.AccountNumber);
    while (ClientExistsByAccountNumber(Cli.AccountNumber, FileName))
    {
        cout << "\nClient with [" << Cli.AccountNumber << "] already exists, Enter another Account Number: ";
        getline(cin >> ws, Cli.AccountNumber);
    }
    cout << "\nEnter PinCode: ";
    getline(cin, Cli.PinCode);
    cout << "Enter Name: ";
    getline(cin, Cli.Name);
    cout << "Enter Phone: ";
    getline(cin, Cli.Phone);
    cout << "Enter Account Balance: ";
    cin >> Cli.AccountBalance;

    return Cli;
}

void AddNewClient()
{
    sClient Cli;
    Cli = ReadNewClient();
    AddDataLineToFile(FileName, ConvertRecordToLine(Cli));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "\nAdding New Clients: \n\n";

        AddNewClient();

        cout << "\nClient Added Successfully, Do You Want To Add More Clients? Y/N: ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void ShowFindClientScreen()
{
    cout << PrintLine("-", 50) << endl;
    cout << PrintLine(" ", 15) << "Find Client Screen\n";
    cout << PrintLine("-", 50) << endl;

    vector <sClient> vClients = LoadClientsDataFromFile(FileName);
    sClient Cli;
    cin.ignore(1, '\n');
    string AccountNumber = ReadString("Enter Account Number: ");

    if (FindClientByAccountNumber(AccountNumber, vClients, Cli))
    {
        PrintClientDetails(Cli);
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }
}

void ShowAddNewClientsScreen()
{
    cout << PrintLine("-", 50) << endl;
    cout << PrintLine(" ", 10) << "Add New Clients Screen" << endl;
    cout << PrintLine("-", 50) << endl;

    AddNewClients();
}

void ShowEndScreen()
{
    cout << PrintLine("-", 50) << endl;
    cout << PrintLine(" ", 17) << "Program Ends :-)\n";
    cout << PrintLine("-", 50) << endl;
}

float TotalBalances()
{
    vector <sClient> vClients = LoadClientsDataFromFile(FileName);
    float TotalBalance = 0;
    for (sClient Cli : vClients)
    {
        TotalBalance += Cli.AccountBalance;
    }
    return TotalBalance;
}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to transactoins menue...";
    system("pause>0");
    TransactionsMenueScreen();
}

void ShowTotalBalance()
{
    ShowAllClientsScreen();
    cout << "\n\n\t\t\t\tTotal Balances = " << TotalBalances() << endl;
    GoBackToTransactionsMenue();
}

void GoBackToMainMenue() 
{
    cout << "\n\nPress any key to go back to main menue...";
    system("pause>0");
    ShowMainMenue();
} 

void PerformTransactionsMenueOptions(enTransactionsMenueOptions TransactionsMenueOptions)
{
    switch (TransactionsMenueOptions)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        Deposit();
        GoBackToTransactionsMenue();
        break;
    }
        
    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        Withdraw();
        GoBackToTransactionsMenue();
        break;
    }
    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalance();
        GoBackToTransactionsMenue();
        break;
    }
    case enTransactionsMenueOptions::eShowMainMenue:
    {
        ShowMainMenue();
    }

    }
}

void PerformMainMenueOptions(enMainMenueOptions MainMenueOptions)
{
    switch (MainMenueOptions)
    {
    case enMainMenueOptions::enShowClientList:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::enAddNewClient:
    {
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::enDeleteClient:
    {
        system("cls");
        ShowDeleteClientsScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::enUpdateClientInfo:
    {
        system("cls");
        ShowUpdateClientsScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::enFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::enShowTransactionsMenue:
    {
        system("cls");
        TransactionsMenueScreen();
        break;
    }

    case enMainMenueOptions::enExit:
    {
        system("cls");
        ShowEndScreen();
        break;
    }

    }
}

void TransactionsMenueScreen()
{
    system("cls");
    cout << "=================================================\n";
    cout << "            Transactions Menue Screen            \n";
    cout << "=================================================\n";
    cout << "            [1] Deposit.\n";
    cout << "            [2] Withdraw.\n";
    cout << "            [3] Total Balances.\n";
    cout << "            [4] Main Menue.\n";
    cout << "=================================================" << endl;
    PerformTransactionsMenueOptions((enTransactionsMenueOptions)ReadNumberInRange("Choose what do you want to do? [1 to 4]: ", 1, 4));
}

void ShowMainMenue()
{
    system("cls");
    cout << "=================================================\n";
    cout << "                Main Menue Screen                \n";
    cout << "=================================================\n";
    cout << "            [1] Show Client List.\n";
    cout << "            [2] Add New Client.\n";
    cout << "            [3] Delete Client.\n";
    cout << "            [4] Update Client Info.\n";
    cout << "            [5] Find Client.\n";
    cout << "            [6] Transactions.\n";
    cout << "            [7] Exit.\n";
    cout << "=================================================" << endl;
    PerformMainMenueOptions((enMainMenueOptions)ReadNumberInRange("Choose what do you want to do? [1 to 7]: ", 1, 7));
}

int main()
{
    SetConsoleOutputCP(65001);
    ShowMainMenue();

    system("pause>0");
}