#include<bits/stdc++.h>
#include<conio.h>
using namespace std;

class Account
{
public:
    string cardNumber;
    string pin; // Changed pin to string to support masking
    double balance;
    vector<string> transactionHistory;

    Account() {}
    Account(string cardNumber, string pin, double balance) : cardNumber(cardNumber), pin(pin), balance(balance) {}

    void displayTransactionHistory()
    {
        cout << "\n\t\t\tTransaction History:\n";
        for (const auto& transaction : transactionHistory)
        {
            cout << "\t\t\t" << transaction << endl;
        }
    }
};

class ATM
{
private:
    unordered_map<string, Account> accounts;
    double atmBalance;

public:
    ATM()
    {
        loadATMBalance();
        loadAccounts();
    }

    void loadAccounts()
    {
        ifstream file("accounts.txt");
        if (file.is_open())
        {
            string cardNumber, pin;
            double balance;
            while (file >> cardNumber >> pin >> balance)
            {
                accounts[cardNumber] = Account(cardNumber, pin, balance);
            }
            file.close();
        }
    }

    void saveAccounts()
    {
        ofstream file("accounts.txt");
        if (file.is_open())
        {
            for (const auto& [cardNumber, account] : accounts)
            {
                file << cardNumber << " " << account.pin << " " << account.balance << endl;
            }
            file.close();
        }
    }

    void loadATMBalance()
    {
        ifstream file("atm_balance.txt");
        if (file.is_open())
        {
            file >> atmBalance;
            file.close();
        }
        else
        {
            atmBalance = 10000; // Default balance if file doesn't exist
        }
    }

    void saveATMBalance()
    {
        ofstream file("atm_balance.txt");
        if (file.is_open())
        {
            file << fixed << setprecision(2) << atmBalance;
            file.close();
        }
    }

    void adminPanel()
    {
        string adminPassword = "admin123";
        string inputPassword;
        system("cls");
        showHeader("Admin Panel");
        cout << "\n\n\t\t\tEnter admin password: ";
        inputPassword = getPasswordInput(); // Masked input for password
        if (inputPassword == adminPassword)
        {
            int choice;
            do
            {
                system("cls");
                showHeader("Admin Panel");
                cout << "\n\t\t\t<<------------- ADMIN PANEL ------------>>";
                cout << "\n\n\t\t\t1. Refill Cash\n\t\t\t2. Check ATM Balance\n\t\t\t3. Add User\n\t\t\t4. Exit\n";
                cout << "\t\t\tEnter choice: ";
                cin >> choice;
                switch (choice)
                {
                case 1:
                    refillCash();
                    break;
                case 2:
                    checkATMBalance();
                    break;
                case 3:
                    addUser();
                    break;
                case 4:
                    cout << "\t\t\tExiting Admin Panel.\n";
                    break;
                default:
                    cout << "\t\t\tInvalid choice.\n";
                }
            }
            while (choice != 4);
        }
        else
        {
            cout << "\t\t\tIncorrect password.\n";
            waitForKeyPress();
        }
    }

    void refillCash()
    {
        double amount;
        cout << "\t\t\tPrevious ATM balance: $" << fixed << setprecision(2) << atmBalance << endl;
        cout << "\t\t\tEnter amount to add: ";
        cin >> amount;
        if (cin.fail())
        {
            cout << "\t\t\tInvalid input. Please enter a valid amount.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            waitForKeyPress();
            return;
        }
        atmBalance += amount;
        cout << "\t\t\tAdding $" << fixed << setprecision(2) << amount << " to ATM balance...\n";
        cout << "\t\t\tNew ATM balance: $" << fixed << setprecision(2) << atmBalance << endl;
        cout << "\t\t\tATM balance updated successfully.\n";
        saveATMBalance(); // Save updated balance to file
        waitForKeyPress();
    }

    void checkATMBalance()
    {
        cout << "\t\t\tCurrent ATM balance: $" << fixed << setprecision(2) << atmBalance << endl;
        waitForKeyPress();
    }

    void addUser()
    {
        string cardNumber, pin;
        double balance;
        cout << "\t\t\tEnter card number: ";
        cin >> cardNumber;
        cout << "\t\t\tEnter PIN: ";
        pin = getPasswordInput(); // Masked input for PIN
        cout << "\t\t\tEnter initial balance: ";
        cin >> balance;
        if (cin.fail())
        {
            cout << "\t\t\tInvalid input. Please enter a valid balance.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            waitForKeyPress();
            return;
        }
        accounts[cardNumber] = Account(cardNumber, pin, balance);
        saveAccounts();
        cout << "\t\t\tUser added successfully.\n";
        waitForKeyPress();
    }

    void userPanel()
    {
        if (accounts.empty())
        {
            cout << "\t\t\tNo user information available. Please contact support.\n";
            waitForKeyPress();
            return;
        }

        string cardNumber, pin;
        system("cls");
        showHeader("User Panel");
        cout << "\n\n\t\t\tEnter card number: ";
        cin >> cardNumber;
        cout << "\t\t\tEnter PIN: ";
        pin = getPasswordInput(); // Masked input for PIN

        if (accounts.find(cardNumber) != accounts.end() && accounts[cardNumber].pin == pin)
        {
            int choice;
            do
            {
                system("cls");
                showHeader("User Panel");
                cout << "\n\t\t\t<<-------------- USER PANEL ------------>>";
                cout << "\n\n\t\t\t1. Check Balance\n\t\t\t2. View Transaction History\n\t\t\t3. Cash Withdrawal\n\t\t\t4. Cash Deposit\n\t\t\t5. Fund Transfer\n\t\t\t6. Exit\n";
                cout << "\t\t\tEnter choice: ";
                cin >> choice;
                switch (choice)
                {
                case 1:
                    checkBalance(cardNumber);
                    break;
                case 2:
                    viewTransactionHistory(cardNumber);
                    break;
                case 3:
                    cashWithdrawal(cardNumber);
                    break;
                case 4:
                    cashDeposit(cardNumber);
                    break;
                case 5:
                    fundTransfer(cardNumber);
                    break;
                case 6:
                    cout << "\t\t\tExiting User Panel.\n";
                    break;
                default:
                    cout << "\t\t\tInvalid choice.\n";
                }
            }
            while (choice != 6);
        }
        else
        {
            cout << "\t\t\tInvalid card number or PIN.\n";
            waitForKeyPress();
        }
    }

    void checkBalance(const string& cardNumber)
    {
        cout << "\n\t\t\t------------------------------------------";
        cout << "\n\t\t\tCurrent Balance: $" << fixed << setprecision(2) << accounts[cardNumber].balance;
        cout << "\n\t\t\t------------------------------------------\n";
        waitForKeyPress();
    }

    void viewTransactionHistory(const string& cardNumber)
    {
        cout << "\n\t\t\t------------------------------------------";
        cout << "\n\t\t\tTransaction History for Card Number: " << cardNumber;
        cout << "\n\t\t\t------------------------------------------\n";
        accounts[cardNumber].displayTransactionHistory();
        waitForKeyPress();
    }

    void cashWithdrawal(const string& cardNumber)
    {
        double amount;
        cout << "\n\t\t\tEnter amount to withdraw: ";
        cin >> amount;
        if (cin.fail())
        {
            cout << "\t\t\tInvalid input. Please enter a valid amount.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            waitForKeyPress();
            return;
        }
        if (amount <= accounts[cardNumber].balance && amount <= atmBalance)
        {
            accounts[cardNumber].balance -= amount;
            atmBalance -= amount;
            string transaction = "Withdrawal: $" + to_string(amount) + " on " + currentDateTime();
            accounts[cardNumber].transactionHistory.push_back(transaction);
            saveAccounts();
            cout << "\n\t\t\t------------------------------------------";
            cout << "\n\t\t\tWithdrawal Successful";
            cout << "\n\t\t\tAmount: $" << fixed << setprecision(2) << amount;
            cout << "\n\t\t\tNew Balance: $" << fixed << setprecision(2) << accounts[cardNumber].balance;
            cout << "\n\t\t\t------------------------------------------\n";
            printReceipt(cardNumber, amount, "Withdrawal");
        }
        else
        {
            cout << "\n\t\t\t------------------------------------------";
            cout << "\n\t\t\tWithdrawal Failed: Insufficient Funds";
            cout << "\n\t\t\t------------------------------------------\n";
        }
        waitForKeyPress();
    }

    void cashDeposit(const string& cardNumber)
    {
        double amount;
        cout << "\n\t\t\tEnter amount to deposit: ";
        cin >> amount;
        if (cin.fail())
        {
            cout << "\t\t\tInvalid input. Please enter a valid amount.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            waitForKeyPress();
            return;
        }
        accounts[cardNumber].balance += amount;
        string transaction = "Deposit: $" + to_string(amount) + " on " + currentDateTime();
        accounts[cardNumber].transactionHistory.push_back(transaction);
        saveAccounts();
        cout << "\n\t\t\t------------------------------------------";
        cout << "\n\t\t\tDeposit Successful";
        cout << "\n\t\t\tAmount: $" << fixed << setprecision(2) << amount;
        cout << "\n\t\t\tNew Balance: $" << fixed << setprecision(2) << accounts[cardNumber].balance;
        cout << "\n\t\t\t------------------------------------------\n";
        printReceipt(cardNumber, amount, "Deposit");
        waitForKeyPress();
    }

    void fundTransfer(const string& cardNumber)
    {
        string targetCardNumber;
        double amount;
        cout << "\n\t\t\tEnter recipient card number: ";
        cin >> targetCardNumber;
        cout << "\t\t\tEnter amount to transfer: ";
        cin >> amount;
        if (accounts.find(targetCardNumber) != accounts.end() && amount <= accounts[cardNumber].balance && amount <= atmBalance)
        {
            accounts[cardNumber].balance -= amount;
            accounts[targetCardNumber].balance += amount;
            string transaction = "Transfer: $" + to_string(amount) + " to " + targetCardNumber + " on " + currentDateTime();
            accounts[cardNumber].transactionHistory.push_back(transaction);
            accounts[targetCardNumber].transactionHistory.push_back("Received: $" + to_string(amount) + " from " + cardNumber + " on " + currentDateTime());
            saveAccounts();
            cout << "\n\t\t\t------------------------------------------";
            cout << "\n\t\t\tFund Transfer Successful";
            cout << "\n\t\t\tAmount: $" << fixed << setprecision(2) << amount;
            cout << "\n\t\t\tNew Balance (Sender): $" << fixed << setprecision(2) << accounts[cardNumber].balance;
            cout << "\n\t\t\t------------------------------------------\n";
        }
        else
        {
            cout << "\n\t\t\t------------------------------------------";
            cout << "\n\t\t\tFund Transfer Failed: Invalid Details or Insufficient Funds";
            cout << "\n\t\t\t------------------------------------------\n";
        }
        waitForKeyPress();
    }

    string currentDateTime()
    {
        time_t now = time(0);
        char* dt = ctime(&now);
        string dateTime(dt);
        dateTime.pop_back(); // Remove newline character
        return dateTime;
    }

    void printReceipt(const string& cardNumber, double amount, const string& transactionType)
    {
        system("cls");
        showHeader("Transaction Receipt");
        char date[9];
        char time[9];
        _strdate(date);
        _strtime(time);
        cout << "\n\t\t\t******************************************";
        cout << "\n\t\t\t*           TRANSACTION RECEIPT           *";
        cout << "\n\t\t\t******************************************";
        cout << "\n\t\t\t  Transaction Type: " << transactionType;
        cout << "\n\t\t\t  Amount: $" << fixed << setprecision(2) << amount;
        cout << "\n\t\t\t  Date: " << date;
        cout << "\n\t\t\t  Time: " << time;
        cout << "\n\t\t\t  Account Balance: $" << fixed << setprecision(2) << accounts[cardNumber].balance;
        cout << "\n\t\t\t******************************************";
        cout << "\n\t\t\t*      THANK YOU FOR USING OUR ATM       *";
        cout << "\n\t\t\t******************************************\n";
    }

    void waitForKeyPress()
    {
        cout << "\n\t\t\tPress any key to continue . . .";
        _getch(); // Waits for a single key press
    }

    void showHeader(const string& title)
    {
        system("cls");
        cout << "\n\n\t\t\t******************************************";
        cout << "\n\t\t\t*                                        *";
        cout << "\n\t\t\t*   " << setw(18 + title.length() / 2) << title << setw(18 - title.length() / 2) << "              *";
        cout << "\n\t\t\t*                                        *";
        cout << "\n\t\t\t******************************************\n";
    }

    string getPasswordInput()
    {
        const char BACKSPACE = 8;
        const char RETURN = 13;

        string password;
        char ch = _getch();

        while (ch != RETURN)
        {
            if (ch == BACKSPACE)
            {
                if (password.length() > 0)
                {
                    cout << "\b \b";
                    password.pop_back();
                }
            }
            else
            {
                password.push_back(ch);
                cout << '*';
            }
            ch = _getch();
        }
        cout << endl;
        return password;
    }
};

int main()
{
    ATM atm;
    int choice;
    do
    {
        system("cls");
        cout << "\n\n\t\t\t******************************************";
        cout << "\n\t\t\t*                                        *";
        cout << "\n\t\t\t*           WELCOME TO YOUR BANK         *";
        cout << "\n\t\t\t*                                        *";
        cout << "\n\t\t\t******************************************\n";
        cout << "\n\n\t\t\t1. Admin\n\t\t\t2. User\n\t\t\t3. Exit\n";
        cout << "\t\t\tEnter choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            atm.adminPanel();
            break;
        case 2:
            atm.userPanel();
            break;
        case 3:
            cout << "\t\t\tExiting.\n";
            break;
        default:
            cout << "\t\t\tInvalid choice.\n";
        }
    }
    while (choice != 3);
    return 0;
}

