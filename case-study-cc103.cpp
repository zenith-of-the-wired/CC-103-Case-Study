#include <iostream>
#include <cstdlib>
using namespace std;

/* =====================================================
   PROGRAM REQUIREMENTS

   ✔ Real System:
     RS Pixels - Payroll and Ordering System

   ✔ Uses struct with 8+ fields

   ✔ Uses at least two enum types

   ✔ Uses array of struct to store multiple records

   ✔ Menu-driven program

   ✔ Add Records (Payroll & Ordering)

   ✔ Search Records using Linear Search

   ✔ Uses computations:
     - Salary = days * rate + OT
     - quantity * price = subtotal
     - subtotal accumulation = total bill

   ✔ Uses Linked List to maintain record history

   ✔ Uses Stack to store recent search history

   ✔ Uses Binary Search to find records by ID

   ✔ Uses Bubble Sort to arrange records by total amount

   ✔ Uses Merge Sort to arrange records by ID

	  ✔ Add Records: Adds employee payroll and customer order records.
   ✔ Display Records: Displays salary, order, and linked list histories.
   ✔ Search Information: Finds names using Linear Search and IDs using Binary Search.
   ✔ Sort Information: Sorts totals using Bubble Sort and IDs using Merge Sort.
   ✔ Process Transactions: Computes salaries, item subtotals, and order totals.
   ✔ Generate Reports: Prints payroll receipts, order receipts, and record histories.
   ✔ Validate User Inputs: Checks numeric entries, item choices, and quantities.
   ✔ Produce Understandable Results: Displays record details and transaction totals.
   ✔ Manage Sequences: Uses a Linked List for record history and a Stack for recent searches.
 
===================================================== */


// ==================== TEXT HANDLING ====================

void copyText(char destination[], const char source[]) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

// TEXT COMPARE: Checks whether two character arrays contain the same text.
bool sameText(const char first[], const char second[]) {
    int i = 0;
    while (first[i] != '\0' && second[i] != '\0') {
        if (first[i] != second[i]) return false;
        i++;
    }
    return first[i] == second[i];
}

// ==================== DATA STRUCTURES ====================
// Stores the payment status of each record.
enum PaymentStatus { PAID, UNPAID };

// Identifies whether a record belongs to payroll or ordering.
enum RecordType { SALARY, ORDER };

// Stores all information for one payroll or ordering record.
struct Record {
    int id;
    char name[100];
    char lastName[100];
    char product[100];
    int quantity;
    int price;
    int total;
    char date[30];
    PaymentStatus status;
    RecordType type;
};

// ARRAY: Stores the system's main payroll and ordering records.
Record records[100];
int recordCount = 0;

// LINKED LIST: Stores saved records in the order they were added.
struct HistoryNode {
    Record data;
    HistoryNode* next;
};

HistoryNode* historyHead = NULL;
HistoryNode* historyTail = NULL;

// STACK: Stores IDs of recently searched records, newest first.
int searchStack[100];
int stackTop = -1;

// ==================== INPUT VALIDATION ====================
// INPUT VALIDATION: Accepts numeric input and rejects invalid entries.
int getIntInput() {
    int x;
    while (true) {
        cin >> x;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid! Enter number: ";
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
}

// LINKED LIST INSERTION: Adds a copy of each saved record to the history list.
void addToLinkedList(const Record& record) {
    HistoryNode* newNode = new HistoryNode;
    newNode->data = record;
    newNode->next = NULL;

    if (historyHead == NULL) {
        historyHead = newNode;
        historyTail = newNode;
    } else {
        historyTail->next = newNode;
        historyTail = newNode;
    }
}

// RECORD INSERTION: Saves a record in the array and linked list.
void saveRecord(const Record& record) {
    if (recordCount >= 100) {
        cout << "Record limit reached. Cannot add more records.\n";
        return;
    }

    records[recordCount] = record;
    addToLinkedList(records[recordCount]);
    recordCount++;
}

// STACK PUSH: Places a searched record ID at the top of the search history.
void pushSearchHistory(int id) {
    if (stackTop < 99) {
        searchStack[++stackTop] = id;
    } else {
        for (int i = 0; i < 99; i++) {
            searchStack[i] = searchStack[i + 1];
        }
        searchStack[99] = id;
    }
}

// ==================== PAYROLL AND ORDERING ====================
// PAYROLL SYSTEM: Computes and saves an employee's salary record.
void payrollSystem() {
    char name[100], date[30];
    int days, otHours;
    int dailyRate = 500, otRate = 60;

    cout << "\n💼 ==== PAYROLL SYSTEM ==== 💼\n";
    cout << "👤 Employee Name: ";
    cin.getline(name, 100);

    cout << "📅 Days Worked: ";
    days = getIntInput();

    cout << "⏱ OT Hours: ";
    otHours = getIntInput();

    cout << "📆 Enter Date (MM/DD/YY ex. 09/25/07): ";
    cin.getline(date, 30);

    int total = (days * dailyRate) + (otHours * otRate);
    int baseSalary = days * dailyRate;
    int overtimePay = otHours * otRate;

    cout << "\n🧾========== SALARY RECEIPT ==========\n";
    cout << "👤 Employee:\t" << name << endl;
    cout << "💵 Daily Rate:\t" << dailyRate << endl;
    cout << "📅 Days Worked:\t" << days << endl;
    cout << "💰 Base Salary:\t" << baseSalary << endl;
    cout << "⏱ OT Hours:\t" << otHours << endl;
    cout << "💵 OT Rate:\t" << otRate << endl;
    cout << "💰 OT Pay:\t\t" << overtimePay << endl;
    cout << "------------------------------------\n";
    cout << "💸 TOTAL SALARY:\t" << total << endl;
    cout << "📆 Date:\t\t" << date << endl;
    cout << "====================================\n";

    Record newRecord;
    newRecord.id = recordCount + 1;
    copyText(newRecord.name, name);
    copyText(newRecord.lastName, "");
    copyText(newRecord.product, "Salary");
    newRecord.quantity = days;
    newRecord.price = dailyRate;
    newRecord.total = total;
    copyText(newRecord.date, date);
    newRecord.status = PAID;
    newRecord.type = SALARY;

    saveRecord(newRecord);
}

// ORDERING SYSTEM: Computes item subtotals and saves each selected item.
void orderingSystem() {
    int choice, qty, total = 0;
    char name[100], date[30], product[100];

    int qty1 = 0, qty2 = 0, qty3 = 0, qty4 = 0;
    int qty5 = 0, qty6 = 0, qty7 = 0, qty8 = 0;

    const int price1 = 400, price2 = 400, price3 = 400, price4 = 500;
    const int price5 = 500, price6 = 800, price7 = 500, price8 = 500;

    cout << "\n🛒 ==== ORDERING SYSTEM ==== 🛒\n";
    cout << "👤 Enter Customer Name: ";
    cin.getline(name, 100);

    do {
        cout << "\n📋 ITEM LIST\n";
        cout << "1. Jersey Sando(400)\n2. Jersey Short(400)\n3. T-Shirt(400)\n";
        cout << "4. Long Sleeve(500)\n5. Warmer(500)\n6. Jacket(800)\n";
        cout << "7. Jogging Pants(500)\n8. Polo Shirt(500)\n9. Done\n";
        cout << "👉 Choice: ";
        choice = getIntInput();

        if (choice == 9) break;

        if (choice < 1 || choice > 8) {
            cout << "Invalid item choice.\n";
            continue;
        }

        cout << "🔢 Qty: ";
        qty = getIntInput();

        if (qty <= 0) {
            cout << "Quantity must be greater than zero.\n";
            continue;
        }

        int price = 0;
        if (choice == 1) { copyText(product, "Jersey Sando"); price = 400; qty1 += qty; }
        else if (choice == 2) { copyText(product, "Jersey Short"); price = 400; qty2 += qty; }
        else if (choice == 3) { copyText(product, "T-Shirt"); price = 400; qty3 += qty; }
        else if (choice == 4) { copyText(product, "Long Sleeve"); price = 500; qty4 += qty; }
        else if (choice == 5) { copyText(product, "Warmer"); price = 500; qty5 += qty; }
        else if (choice == 6) { copyText(product, "Jacket"); price = 800; qty6 += qty; }
        else if (choice == 7) { copyText(product, "Jogging Pants"); price = 500; qty7 += qty; }
        else if (choice == 8) { copyText(product, "Polo Shirt"); price = 500; qty8 += qty; }

        int sub = qty * price;

        cout << "📆 Enter Date (MM/DD/YY ex. 09/25/07): ";
        cin.getline(date, 30);

        Record newRecord;
        newRecord.id = recordCount + 1;
        copyText(newRecord.name, name);
        copyText(newRecord.lastName, "");
        copyText(newRecord.product, product);
        newRecord.quantity = qty;
        newRecord.price = price;
        newRecord.total = sub;
        copyText(newRecord.date, date);
        newRecord.status = PAID;
        newRecord.type = ORDER;

        saveRecord(newRecord);

    } while (true);

    cout << "\n🧾=========== ORDER RECEIPT ===========🧾\n";
    cout << "📦 ITEM\t\tQTY x PRICE\t=\tTOTAL\n";

    if (qty1 > 0) { int line = qty1 * price1; total += line; cout << "Jersey Sando\t" << qty1 << " x " << price1 << "\t=\t" << line << endl; }
    if (qty2 > 0) { int line = qty2 * price2; total += line; cout << "Jersey Short\t" << qty2 << " x " << price2 << "\t=\t" << line << endl; }
    if (qty3 > 0) { int line = qty3 * price3; total += line; cout << "T-Shirt\t\t" << qty3 << " x " << price3 << "\t=\t" << line << endl; }
    if (qty4 > 0) { int line = qty4 * price4; total += line; cout << "Long Sleeve\t" << qty4 << " x " << price4 << "\t=\t" << line << endl; }
    if (qty5 > 0) { int line = qty5 * price5; total += line; cout << "Warmer\t\t" << qty5 << " x " << price5 << "\t=\t" << line << endl; }
    if (qty6 > 0) { int line = qty6 * price6; total += line; cout << "Jacket\t\t" << qty6 << " x " << price6 << "\t=\t" << line << endl; }
    if (qty7 > 0) { int line = qty7 * price7; total += line; cout << "Jogging Pants\t" << qty7 << " x " << price7 << "\t=\t" << line << endl; }
    if (qty8 > 0) { int line = qty8 * price8; total += line; cout << "Polo Shirt\t" << qty8 << " x " << price8 << "\t=\t" << line << endl; }

    cout << "--------------------------------------\n";
    cout << "💵 TOTAL BILL:\t" << total << " pesos\n";
    cout << "📆 DATE:\t\t" << date << endl;
    cout << "======================================\n";
}

// ==================== DISPLAY AND SEARCH ====================
// DISPLAY: Shows all salary records stored in the array.
void displaySalary() {
    bool found = false;
    cout << "\n📊 === SALARY HISTORY === 📊\n";

    for (int i = 0; i < recordCount; i++) {
        if (records[i].type == SALARY) {
            cout << "👤 " << records[i].name << " | 💰 "
                 << records[i].total << " | 📆 " << records[i].date << endl;
            found = true;
        }
    }

    if (!found) cout << "⚠️ No salary records yet.\n";
}

// DISPLAY: Shows all order records stored in the array.
void displayOrders() {
    bool found = false;
    cout << "\n📦 === ORDER HISTORY === 📦\n";

    for (int i = 0; i < recordCount; i++) {
        if (records[i].type == ORDER) {
            cout << "👤 " << records[i].name << " | 🛍 "
                 << records[i].product << " | 💰 "
                 << records[i].total << " | 📆 " << records[i].date << endl;
            found = true;
        }
    }

    if (!found) cout << "⚠️ No order records yet.\n";
}

// LINEAR SEARCH: Checks every record until matching employee or customer names are found.
void searchRecords() {
    char key[100];
    cout << "🔍 Search name: ";
    cin.getline(key, 100);

    bool found = false;

    for (int i = 0; i < recordCount; i++) {
        if (sameText(records[i].name, key)) {
            cout << "👤 Name: " << records[i].name;

            if (records[i].type == SALARY)
                cout << " | 💼 Payroll";
            else
                cout << " | 🛒 Order";

            cout << " | 💰 Total: " << records[i].total
                 << " | 📆 Date: " << records[i].date << endl;

            pushSearchHistory(records[i].id);
            found = true;
        }
    }

    if (!found) cout << "❌ No records yet for this person.\n";
}

// ==================== LINKED LIST AND STACK ====================
// LINKED LIST TRAVERSAL: Displays saved records in their original insertion order.
void displayLinkedListHistory() {
    HistoryNode* current = historyHead;

    cout << "\n🔗 === LINKED LIST RECORD HISTORY === 🔗\n";

    if (current == NULL) {
        cout << "⚠️ No records yet.\n";
        return;
    }

    while (current != NULL) {
        cout << "🆔 ID: " << current->data.id
             << " | Name: " << current->data.name
             << " | Type: " << (current->data.type == SALARY ? "Payroll" : "Order")
             << " | Total: " << current->data.total
             << " | Date: " << current->data.date << endl;
        current = current->next;
    }
}

// STACK DISPLAY: Shows recent successful searches from newest to oldest.
void displaySearchStack() {
    cout << "\n📚 === RECENT SEARCH HISTORY (STACK) === 📚\n";

    if (stackTop == -1) {
        cout << "⚠️ No searched records yet.\n";
        return;
    }

    for (int i = stackTop; i >= 0; i--) {
        cout << "🔍 Record ID: " << searchStack[i] << endl;
    }
}

// ==================== SORTING AND SEARCHING ALGORITHMS ====================
// MERGE SORT: Combines sorted sections to arrange records by ascending record ID.
void mergeRecords(Record arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Record* L = new Record[n1];
    Record* R = new Record[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].id <= R[j].id) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// MERGE SORT: Divides the record list into smaller parts before merging them in ID order.
void mergeSort(Record arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        mergeRecords(arr, left, mid, right);
    }
}

// BINARY SEARCH: Finds a record ID in the sorted record copy by repeatedly halving the search range.
void binarySearchRecord() {
    if (recordCount == 0) {
        cout << "⚠️ No records available to search.\n";
        return;
    }

    Record sortedRecords[100];

    for (int i = 0; i < recordCount; i++) sortedRecords[i] = records[i];

    mergeSort(sortedRecords, 0, recordCount - 1);

    int key;
    cout << "🔎 Enter Record ID to search: ";
    key = getIntInput();

    int low = 0, high = recordCount - 1;
    bool found = false;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (sortedRecords[mid].id == key) {
            cout << "\n✅ Record Found!\n";
            cout << "ID: " << sortedRecords[mid].id
                 << " | Name: " << sortedRecords[mid].name
                 << " | Type: " << (sortedRecords[mid].type == SALARY ? "Payroll" : "Order")
                 << " | Product: " << sortedRecords[mid].product
                 << " | Total: " << sortedRecords[mid].total
                 << " | Date: " << sortedRecords[mid].date << endl;
            found = true;
            break;
        } else if (sortedRecords[mid].id < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (!found) cout << "❌ Record ID not found.\n";
}

// BUBBLE SORT: Swaps adjacent records to arrange them by ascending total amount.
void bubbleSortByTotal() {
    if (recordCount == 0) {
        cout << "⚠️ No records available to sort.\n";
        return;
    }

    Record sortedRecords[100];

    for (int i = 0; i < recordCount; i++) sortedRecords[i] = records[i];

    for (int i = 0; i < recordCount - 1; i++) {
        for (int j = 0; j < recordCount - i - 1; j++) {
            if (sortedRecords[j].total > sortedRecords[j + 1].total) {
                Record temp = sortedRecords[j];
                sortedRecords[j] = sortedRecords[j + 1];
                sortedRecords[j + 1] = temp;
            }
        }
    }

    cout << "\n🔃 === BUBBLE SORT: RECORDS BY TOTAL AMOUNT === 🔃\n";
    for (int i = 0; i < recordCount; i++) {
        cout << "🆔 ID: " << sortedRecords[i].id
             << " | Name: " << sortedRecords[i].name
             << " | Total: " << sortedRecords[i].total << endl;
    }
}

// MERGE SORT DISPLAY: Sorts a copy of the records by ID without changing the original array.
void mergeSortByID() {
    if (recordCount == 0) {
        cout << "⚠️ No records available to sort.\n";
        return;
    }

    Record sortedRecords[100];

    for (int i = 0; i < recordCount; i++) sortedRecords[i] = records[i];

    mergeSort(sortedRecords, 0, recordCount - 1);

    cout << "\n📊 === MERGE SORT: RECORDS BY ID === 📊\n";
    for (int i = 0; i < recordCount; i++) {
        cout << "🆔 ID: " << sortedRecords[i].id
             << " | Name: " << sortedRecords[i].name
             << " | Total: " << sortedRecords[i].total << endl;
    }
}

// ==================== MEMORY CLEANUP ====================
// LINKED LIST CLEANUP: Releases the memory used by the record history nodes.
void deleteLinkedList() {
    while (historyHead != NULL) {
        HistoryNode* temp = historyHead;
        historyHead = historyHead->next;
        delete temp;
    }
    historyTail = NULL;
}

// ==================== MAIN PROGRAM ====================
// MAIN MENU: Runs the original system and the added data structure demonstrations.
int main() {
    int choice;

    do {
        cout << "\n🌟 ==== RS PIXELS SYSTEM ==== 🌟\n";
        cout << "1. 💼 Payroll\n";
        cout << "2. 🛒 Ordering\n";
        cout << "3. 📊 Salary History\n";
        cout << "4. 📦 Order History\n";
        cout << "5. 🔍 Linear Search by Name\n";
        cout << "6. 🔗 Linked List Record History\n";
        cout << "7. 📚 Stack - Recent Search History\n";
        cout << "8. 🔎 Binary Search by Record ID\n";
        cout << "9. 🔃 Bubble Sort by Total Amount\n";
        cout << "10. 📊 Merge Sort by Record ID\n";
        cout << "11. 🚪 Exit\n";
        cout << "👉 Choice: ";

        choice = getIntInput();

        switch (choice) {
            case 1: payrollSystem(); break;
            case 2: orderingSystem(); break;
            case 3: displaySalary(); break;
            case 4: displayOrders(); break;
            case 5: searchRecords(); break;
            case 6: displayLinkedListHistory(); break;
            case 7: displaySearchStack(); break;
            case 8: binarySearchRecord(); break;
            case 9: bubbleSortByTotal(); break;
            case 10: mergeSortByID(); break;
            case 11: cout << "\n👋 Thank you for using RS Pixels System!\n"; break;
            default: cout << "❌ Invalid choice. Please try again.\n";
        }

    } while (choice != 11);

    deleteLinkedList();
    return EXIT_SUCCESS;
}
