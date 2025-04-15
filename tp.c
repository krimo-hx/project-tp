#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100

typedef struct {
    char name[100];
    int accountNumber;
    char password[20];
    float balance;
} Account;

Account accounts[MAX_USERS];
int totalUsers = 0;

void saveToFile() {
    FILE *fp = fopen("accounts.txt", "w");
    for (int i = 0; i < totalUsers; i++) {
        fprintf(fp, "%s %d %s %.2f\n", accounts[i].name, accounts[i].accountNumber, accounts[i].password, accounts[i].balance);
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("accounts.txt", "r");
    if (fp == NULL) return;

    while (fscanf(fp, "%s %d %s %f", accounts[totalUsers].name, &accounts[totalUsers].accountNumber,
                  accounts[totalUsers].password, &accounts[totalUsers].balance) == 4) {
        totalUsers++;
    }
    fclose(fp);
}

int findAccountIndex(int accountNumber) {
    for (int i = 0; i < totalUsers; i++) {
        if (accounts[i].accountNumber == accountNumber)
            return i;
    }
    return -1;
}

void createAccount() {
    Account acc;
    printf("Enter your name: ");
    scanf("%s", acc.name);
    printf("Enter account number: ");
    scanf("%d", &acc.accountNumber);

    if (findAccountIndex(acc.accountNumber) != -1) {
        printf("❌ Account already exists.\n");
        return;
    }

    printf("Enter password: ");
    scanf("%s", acc.password);
    acc.balance = 0.0;

    accounts[totalUsers++] = acc;
    saveToFile();
    printf("✅ Account created successfully.\n");
}

int login() {
    int accNumber;
    char pass[20];
    printf("Enter account number: ");
    scanf("%d", &accNumber);
    printf("Enter password: ");
    scanf("%s", pass);

    int idx = findAccountIndex(accNumber);
    if (idx == -1 || strcmp(accounts[idx].password, pass) != 0) {
        printf("❌ Login failed.\n");
        return -1;
    }
    printf("✅ Login successful.\n");
    return idx;
}

void depositMoney(int index) {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    accounts[index].balance += amount;
    saveToFile();
    printf("✅ Deposit successful. New balance: %.2f\n", accounts[index].balance);
}

void transferMoney(int index) {
    int toAcc;
    float amount;
    printf("Enter account number to transfer to: ");
    scanf("%d", &toAcc);
    int toIndex = findAccountIndex(toAcc);
    if (toIndex == -1) {
        printf("❌ Account not found.\n");
        return;
    }
    printf("Enter amount to transfer: ");
    scanf("%f", &amount);
    if (accounts[index].balance < amount) {
        printf("❌ Not enough balance.\n");
        return;
    }
    accounts[index].balance -= amount;
    accounts[toIndex].balance += amount;
    saveToFile();
    printf("✅ Transfer successful.\n");
}

void checkBalance(int index) {
    printf("Your balance is: %.2f\n", accounts[index].balance);
}

void deleteAccount(int index) {
    if (accounts[index].balance != 0) {
        printf("❌ Account is not empty.\n");
        return;
    }
    for (int i = index; i < totalUsers - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    totalUsers--;
    saveToFile();
    printf("✅ Account deleted.\n");
}

void userMenu(int index) {
    int choice;
    do {
        printf("\n--- User Menu ---\n");
        printf("1. Deposit\n");
        printf("2. Transfer\n");
        printf("3. Check Balance\n");
        printf("4. Delete Account\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: depositMoney(index); break;
            case 2: transferMoney(index); break;
            case 3: checkBalance(index); break;
            case 4: deleteAccount(index); return;
            case 5: return;
            default: printf("❌ Invalid choice.\n");
        }
    } while (1);
}

int main() {
    loadFromFile();
    int choice;
    do {
        printf("\n--- Welcome to Our Bank ---\n");
        printf("1. Create Account\n");
        printf("2. Login to Account\n");
        printf("3. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: createAccount(); break;
            case 2: {
                int index = login();
                if (index != -1)
                    userMenu(index);
                break;
            }
            case 3: printf("👋 Goodbye!\n"); exit(0);
            default: printf("❌ Invalid choice.\n");
        }
    } while (1);

    return 0;
}
