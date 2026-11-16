#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdint>

using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";


// تعريف دالة القائمة الرئيسية مسبقاً لاستخدامها داخل التوابع الأخرى
void ShowMainMenue();
void ShowTransacionMenu();
void ShowManageUsersMenu();
void Login();
short ReadPermisionsToSet();
void GoBackToMainMenu();

enum enTransactionsOption
{
    eDeposit = 1,
    eWithdrow = 2,
    eTotalBalances = 3,
    eMainMenu = 4
};

// خيارات القائمة الرئيسية
enum enMainMenueOptions
{
    eListClients = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eTransactions = 6,
    eManageUsers = 7,
    eLogout = 8
};
enum enManageUsersOptions
{
    eListUsers = 1,
    eAddNewUser = 2,
    eDeleteUser = 3,
    eUpdateUser = 4,
    eFindUser = 5,
    eMainMenue = 6
};

enum enPermissions
{
    eAll = -1,
    pListClients = 1,
    pAddNewClient = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient = 16,
    pTransactions = 32,
    pManageUsers = 64
};

bool CheckAccessPermission(enPermissions Permissions);

// هيكل بيانات العميل
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

struct sUser
{
    string Username;
    string Password;
    short Permisions = 0;
    bool MarkForDelete = false;

};

sUser CurrentUser;


// تفكيك السلسلة النصية إلى عناصر Vector بناءً على المحدد (Delim)
vector<string> SplitString(string S1, string Delim = "#//#")
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}

// تحويل السطر من الملف إلى هيكل بيانات client
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData = SplitString(Line, Seperator);

    if (vClientData.size() >= 5)
    {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stod(vClientData[4]);
    }

    return Client;
}

sUser ConvertLinetoRecordInUserFile(string Line, string Seperator = "#//#")
{
    sUser User;
    vector<string> vUserData = SplitString(Line, Seperator);

    if (vUserData.size() >= 3)
    {
        User.Username = vUserData[0];
        User.Password = vUserData[1];
        User.Permisions = stoi(vUserData[2]);
    }

    return User;
}
// تحويل بيانات العميل إلى سطر نصي لحفظه في الملف
string ConvertRecordToLineInUserFile(sUser User, string Seperator = "#//#")
{
    string stUserRecord = "";
    
    stUserRecord += User.Username + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permisions);

    return stUserRecord;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}


// تحميل بيانات العملاء من الملف
vector<sClient> LoadCleintsDataFromFile(string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // فتح الملف للقراءة فقط

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }

        MyFile.close();
    }

    return vClients;
}

vector <sUser> LoadUsersDataFromFile(string FileName)
{
    vector <sUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // فتح الملف للقراءة فقط

    if (MyFile.is_open())
    {
        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertLinetoRecordInUserFile(Line);
            vUsers.push_back(User);
        }

        MyFile.close();
    }

    return vUsers;
}

// طباعة سطر عميل محدد داخل الجدول
void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientRecordBalanceLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void PrintManageUsersList(sUser User)
{
    cout << "| " << setw(15) << left << User.Username;
    cout << "| " << setw(40) << left << User.Password;
    cout << "| " << setw(12) << left << User.Permisions;

}


void ShowAccessDeniesMessage()
{
    cout << "Access Denied! \nYou not have a Permission - To Do This";
}

    // عرض جميع عملاء
void ShowAllClientsScreen()
{

    if (!CheckAccessPermission(enPermissions::pListClients))
    {
        ShowAccessDeniesMessage();
        GoBackToMainMenu();
        return;
    }



    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n____________________________________________________________________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n____________________________________________________________________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (sClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
        }
    }

    cout << "\n____________________________________________________________________________________________________\n" << endl;
}

// طباعة بطاقة تفاصيل العميل
void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccount Number: " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.Name;
    cout << "\nPhone          : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

void PrintUserCard(sUser User)
{
    cout << "\nThe following are the User details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername: " << User.Username;
    cout << "\nPassword: " << User.Password;
    cout << "\nPermissions: " << User.Permisions;
    cout << "\n-----------------------------------\n";
}


// البحث عن عميل بواسطة رقم الحساب
bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool FinduserByUsername(string Username, vector<sUser> vUsers, sUser & User)
{
    for (sUser C : vUsers)
    {
        if (C.Username == Username)
        {
            User = C;
            return true;
        }
    }
    return false;
}

bool FindClientByUsernameAndPassword(string Username,string Password, vector<sUser> vUsers, sUser &User)
{
    for (sUser & C : vUsers)
    {
        if (C.Username == Username && C.Password == Password)
        {
            User = C;
            return true;
        }
    }
    return false;
}



// التحقق من وجود رقم الحساب مسبقاً
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

bool UserExistsByUsername(string Username, string FileName)
{
    vector<sUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertLinetoRecordInUserFile(Line);
            if (User.Username == Username)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

// قراءة بيانات عميل جديد من المستخدم
sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance? ";
    cin >> Client.AccountBalance;

    return Client;
}

// تغيير بيانات العميل أثناء التحديث
sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance? ";
    cin >> Client.AccountBalance;

    return Client;
}

sUser ChangeUserRecord(string Username)
{
    sUser User;
    User.Username = Username;

    cout << "\n\nEnter Password? ";
    getline(cin >> ws, User.Password);

    User.Permisions = ReadPermisionsToSet();

    return User;
}

// تعليم العميل للحذف
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool MarkUserForDeleteUsername(string Username, vector<sUser>& vUsers)
{
    for (sUser& C : vUsers)
    {
        if (C.Username == Username)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

// حفظ بيانات العملاء المتبقية إلى الملف بعد الحذف أو التحديث
vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); // إعادة كتابة الملف بالكامل

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

    return vClients;
}
vector<sUser> SaveUsersDataToFile(string FileName, vector<sUser> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); // إعادة كتابة الملف بالكامل

    string DataLine;

    if (MyFile.is_open())
    {
        for (sUser C : vUsers)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLineInUserFile(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

    return vUsers;
}

// إضافة سطر جديد للملف
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

// إضافة عميل واحد
void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

// إضافة عدة عملاء
void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

// حذف عميل برقم الحساب
bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want delete this client? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            // تحديث قائمة العملاء في الذاكرة
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

    return false;
}

bool DeleteClientByUsername(string Username, vector<sUser>& vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FinduserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want delete this User? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteUsername(Username, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);

            // تحديث قائمة العملاء في الذاكرة
            vUsers = LoadUsersDataFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << Username << ") is Not Found!";
        return false;
    }

    return false;
}


// تحديث بيانات عميل برقم الحساب
bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

    return false;
}

bool UpdateUserByUsername(string Username, vector<sUser>& vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FinduserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this User? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sUser& C : vUsers)
            {
                if (C.Username == Username)
                {
                    C = ChangeUserRecord(Username);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Username (" << Username << ") is Not Found!";
        return false;
    }

    return false;
}


// قراءة رقم الحساب من المستخدم
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter Account Number? ";
    cin >> AccountNumber;
    return AccountNumber;
}
string ReadUserUsername()
{
    string Username = "";
    cout << "\nPlease enter Username? ";
    cin >> Username;
    return Username;
}

// شاشة حذف عميل
void ShowDeleteClientScreen()
{

    if (!CheckAccessPermission(enPermissions::pDeleteClient))
    {
        ShowAccessDeniesMessage();
        GoBackToMainMenu();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}
void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUser = LoadUsersDataFromFile(UsersFileName);
    string Username = ReadUserUsername();
    DeleteClientByUsername(Username, vUser);
}

// شاشة تحديث بيانات عميل
void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enPermissions::pUpdateClient))
    {
        ShowAccessDeniesMessage();
        GoBackToMainMenu();
        return;
    }


    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string Username = ReadUserUsername();
    UpdateUserByUsername(Username, vUsers);
}


// شاشة إضافة عملاء جُدد
void ShowAddNewClientsScreen()
{

    if (!CheckAccessPermission(enPermissions::pAddNewClient))
    {
        ShowAccessDeniesMessage();
        GoBackToMainMenu();
        return;
    }


    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

// شاشة البحث عن عميل
void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enPermissions::pFindClient))
    {
        ShowAccessDeniesMessage();
        GoBackToMainMenu();
        return;
    }


    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    sUser User;
    string Username = ReadUserUsername();

    if (FinduserByUsername(Username, vUsers, User))
        PrintUserCard(User);
    else
        cout << "\nUser with Username [" << Username << "] is not found!";
}



short ReadTransactionOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}


short ReadManageUserOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}



void GoBackToManageUsersMenu()
{
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    system("pause>0");
    ShowManageUsersMenu();
}

void GoBackToTransactionMenu()
{
    cout << "\n\nPress any key to go back to Transaction Menu...";
    system("pause>0");
    ShowTransacionMenu();
}
// العودة للقائمة الرئيسية
void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}






int ReadDepositeAmount()
{
    int NumberDeposit;
    cout << "\nPlease enter Deposite amount? ";
    cin >> NumberDeposit;
    return NumberDeposit;
}
int ReadWithdrowAmount()
{
    int NumberWithdrow;
    cout << "\nPlease enter Withdrow amount? ";
    cin >> NumberWithdrow;
    return NumberWithdrow;
}

int CalculateDeposit(sClient& Client, int NumberDeposit)
{
    return Client.AccountBalance += NumberDeposit;
}

sClient ExcuteDeposit(sClient& Client, int NumberDeposit)
{

    while (NumberDeposit < 50)
    {
        cout << "Number Deposit (" << NumberDeposit <<
            ") Is a few , Please enter Number bigger than 50 L.E? ";
        cin >> NumberDeposit;
    }
    CalculateDeposit(Client, NumberDeposit);
    return Client;
}

sClient ExcuteWithdraw(sClient& Client, int NumberWithdraw)
{
    while (NumberWithdraw < 50)
    {
        cout << "Number Deposit (" << NumberWithdraw <<
            ") Is a few , Please enter Number bigger than 50 L.E? ";
        cin >> NumberWithdraw;
    }
    CalculateDeposit(Client, NumberWithdraw * -1);
    return Client;

}

void ShowDepositScreen()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    char MakeSure = 'n';
    cout << "\n-------------------------------------\n";
    cout << "\t\tDeposit Screen\n";
    cout << "\n-------------------------------------\n";
    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "Client With [" << AccountNumber << "] Does Not Exist.\n\n";

        cout << "Please enter AccountNumber? ";
        cin >> AccountNumber;
    }
    PrintClientCard(Client);
    double NumberDeposit = ReadDepositeAmount();

    cout << "Are You Sure You want Perform thie Transaction? y/n ? ";
    cin >> MakeSure;
    if (MakeSure == 'Y' || MakeSure == 'y')
    {
        ExcuteDeposit(Client, NumberDeposit);
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C = Client;
                break;
            }
        }
        SaveCleintsDataToFile(ClientsFileName, vClients);
    }


}
void ShowWithdrowScreen()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    char MakeSure = 'n';
    cout << "\n-------------------------------------\n";
    cout << "\t\tWithdrow Screen\n";
    cout << "\n-------------------------------------\n";
    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "Client With [" << AccountNumber << "] Does Not Exist.\n\n";

        cout << "Please enter AccountNumber? ";
        cin >> AccountNumber;
    }
    PrintClientCard(Client);
    double NumberWithdrow = ReadWithdrowAmount();
    while (NumberWithdrow > Client.AccountBalance)
    {
        cout << "Amount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> NumberWithdrow;
    }
    cout << "\n\nAre You Sure You want Perform thie Transaction? y/n ? ";
    cin >> MakeSure;
    if (MakeSure == 'Y' || MakeSure == 'y')
    {
        ExcuteWithdraw(Client, NumberWithdrow);
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C = Client;
                break;
            }
        }
        SaveCleintsDataToFile(ClientsFileName, vClients);
    }

}
void ShowTotalBalancesScreen()
{
    double Balance = 0;
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\tBalances List (" << vClients.size() << ") Client(s).\n";
    cout << "\n____________________________________________________________________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n____________________________________________________________________________________________________\n" << endl;
    for (sClient& Client : vClients)
    {
        PrintClientRecordBalanceLine(Client);
        Balance += Client.AccountBalance;
        cout << endl;
    }
    cout << "\n____________________________________________________________________________________________________\n" << endl;

    cout << "\n\tTotal Balance = " << Balance << endl;
}

void ShowUsersList()
{
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    cout << "\n\t\t\t\tBalances List (" << vUsers.size() << ") Client(s).\n";
    cout << "\n____________________________________________________________________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Username";
    cout << "| " << left << setw(40) << "Password";
    cout << "| " << left << setw(12) << "Permisions";
    cout << "\n____________________________________________________________________________________________________\n" << endl;
    for (sUser& User : vUsers)
    {
        PrintManageUsersList(User);
        cout << endl;
    }
    cout << "\n____________________________________________________________________________________________________\n" << endl;
}

short ReadPermisionsToSet()
{
    sUser User;
    char Agreement = 'D';
    cout << "Do You Want to give full access? ";
    cin >> Agreement;
    if (Agreement == 'y' || Agreement == 'Y')
    {
        User.Permisions += enPermissions::eAll;
    }
    else
    {
        cout << "\nDo You Want to give Access to: \n";

        cout << "Show Client List y/n? ";
        cin >> Agreement;
        if (Agreement == 'y' || Agreement == 'Y')
        {
            User.Permisions += enPermissions::pListClients;
        }
        cout << "Add New Client y/n? ";
        cin >> Agreement;
        if (Agreement == 'y' || Agreement == 'Y')
        {
            User.Permisions += enPermissions::pAddNewClient;
        }
        cout << "Delete Client y/n? ";
        cin >> Agreement;
        if (Agreement == 'y' || Agreement == 'Y')
        {
            User.Permisions += enPermissions::pDeleteClient;
        }
        cout << "Update Client y/n? ";
        cin >> Agreement;
        if (Agreement == 'y' || Agreement == 'Y')
        {
            User.Permisions += enPermissions::pUpdateClient;
        }
        cout << "Find Client y/n? ";
        cin >> Agreement;
        if (Agreement == 'y' || Agreement == 'Y')
        {
            User.Permisions += enPermissions::pFindClient;
        }
        cout << "Transactions y/n? ";
        cin >> Agreement;
        if (Agreement == 'y' || Agreement == 'Y')
        {
            User.Permisions += enPermissions::pTransactions;
        }
        cout << "Manage Users y/n? ";
        cin >> Agreement;
        if (Agreement == 'y' || Agreement == 'Y')
        {
            User.Permisions += enPermissions::pManageUsers;
        }
    }
    return User.Permisions;
}

sUser ReadNewUser()
{
    sUser User;

    cout << "Enter Username? ";
    getline(cin >> ws, User.Username);

    while (UserExistsByUsername(User.Username, UsersFileName))
    {
        cout << "\User with [" << User.Username << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.Username);
    }
    cout << "Enter Password? ";
    getline(cin, User.Password);

    User.Permisions = ReadPermisionsToSet();

    return User;
}

void AddNewUser()
{
    sUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLineInUserFile(User));
}

void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New Users:\n\n";
        AddNewUser();
        cout << "\User Added Successfully, do you want to add more Users? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewUserScreen()
{
    
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";

    AddNewUsers();
}

void PerformTransactionOption(enTransactionsOption TransactionsOption)
{
    switch (TransactionsOption)
    {
    case enTransactionsOption::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionMenu();
        break;
    }
    case enTransactionsOption::eWithdrow:
        system("cls");
        ShowWithdrowScreen();
        GoBackToTransactionMenu();
        break;
    case enTransactionsOption::eTotalBalances:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionMenu();
        break;
    case enTransactionsOption::eMainMenu:
        system("cls");
        ShowMainMenue();
        break;

    }
}

void PerformManageUsersOption(enManageUsersOptions ManageUsersOption)
{
    switch (ManageUsersOption)
    {
    case enManageUsersOptions::eListUsers:
    {
        system("cls");
        ShowUsersList();
        GoBackToManageUsersMenu();
        break;
    }
    case enManageUsersOptions::eAddNewUser:
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersOptions::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersOptions::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersOptions::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersOptions::eMainMenue:
        system("cls");
        ShowMainMenue();
        break;
    }
}


// شاشة الخروج
void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}




// قراءة خيار المستخدم من القائمة الرئيسية
short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

bool CheckAccessPermission(enPermissions Permission)
{
    if (CurrentUser.Permisions == enPermissions::eAll)
        return true;

    if ((Permission & CurrentUser.Permisions) == Permission)
        return true;
    else
        return false;

}

// تنفيذ خيار القائمة الرئيسية Selected
void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenu();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenueOptions::eTransactions:
        system("cls");
        ShowTransacionMenu();
        break;
    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManageUsersMenu();
        break;
    case enMainMenueOptions::eLogout:
        system("cls");
        Login();
        break;
    }
}

void ShowManageUsersMenu()
{

    if (!CheckAccessPermission(enPermissions::pManageUsers))
    {
        ShowAccessDeniesMessage();
        GoBackToMainMenu();
        return;
    }


    system("cls");
    cout << "==================================================\n";
    cout << "              Manage Users Menu Screen             \n";
    cout << "==================================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "==================================================\n";

    PerformManageUsersOption((enManageUsersOptions)ReadManageUserOption());
}

void ShowTransacionMenu()
{
    if (!CheckAccessPermission(enPermissions::pTransactions))
    {
        ShowAccessDeniesMessage();
        GoBackToMainMenu();
        return;
    }


    system("cls");
    cout << "==================================================\n";
    cout << "              Transaction Menu Screen             \n";
    cout << "==================================================\n";
    cout << "\t\t[1] Deposit.\n";
    cout << "\t\t[2] Withdrow.\n";
    cout << "\t\t[3] Total Balances.\n";
    cout << "\t\t[4] MainMenu.\n";
    cout << "==================================================\n";

    PerformTransactionOption((enTransactionsOption)ReadTransactionOption());
}


// عرض القائمة الرئيسية
void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";

    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

void ShowLoginScreen()
{
    system("cls");
    cout << "_________________________________\n";
    cout << "          Login Screen            \n";
    cout << "_________________________________\n";
}

void Login()
{
    ShowLoginScreen();
    string UserName , PassWord;
    vector <sUser> vUsers;
    do
    {
        cout << "Enter UserName? ";
        cin >> UserName;
        cout << "Enter Password? ";
        cin >> PassWord;

        vUsers = LoadUsersDataFromFile(UsersFileName);
        if (FindClientByUsernameAndPassword(UserName, PassWord, vUsers, CurrentUser))
        {
            ShowMainMenue();
        }
        else
        {
            ShowLoginScreen();
            cout << "Invalid Username/Password!\n";

        }
    } while (!FindClientByUsernameAndPassword(UserName, PassWord, vUsers, CurrentUser));

    
}

int main()
{
    Login();
    system("pause>0");
    return 0;
}