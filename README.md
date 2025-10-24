# Bank-Client-Management-System 
A **fully functional console-based banking system** written in **C++**, allowing users to manage clients, perform transactions, and handle persistent account data via file storage (`Clients.txt`).  
This project demonstrates **file handling**, **structured programming**, and **menu-based system design** in C++.

---

## 📋 Features

### 🧾 Main Menu
1. **Show Client List** – Displays all clients in a formatted table.  
2. **Add New Client** – Add a new client (with duplicate checking).  
3. **Delete Client** – Delete an existing client after confirmation.  
4. **Update Client Info** – Modify client details.  
5. **Find Client** – Search for a client by account number.  
6. **Transactions** – Open the transaction menu.  
7. **Exit** – Exit the program.

---

### 💰 Transactions Menu
1. **Deposit** – Add money to a client’s account.  
2. **Withdraw** – Withdraw funds (with validation).  
3. **Total Balances** – Show total balance of all accounts.  
4. **Main Menu** – Return to the main menu.

---

## 🗃️ Data Storage

All client data is stored in a file named **`Clients.txt`** in this format:
AccountNumber#//#PinCode#//#Name#//#Phone#//#Balance


Example:
A101#//#1234#//#John Doe#//#5551234#//#2500.50


The program automatically updates this file whenever clients are added, deleted, or updated.

---

## 🧱 Data Structure

```cpp
struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};
