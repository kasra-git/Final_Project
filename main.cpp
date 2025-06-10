#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <vector>

using namespace std;

// Structs //
struct Book 
{
    int id;
    char name[100];
    char author[100];
    bool available;
};

struct Student 
{
    char username[50];
    char password[50];
    float balance;
    int borrowed_books[10];
    int borrowed_count;
};

// Helper Functions // 
void clearScreen() 
{
    system("cls || clear");
}

void pauseScreen() 
{
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

// File's Functions //
void save_books_to_file(const vector<Book>& books) 
{
    ofstream file("books.txt");
    if (file.is_open()) 
    {
        for (const auto& book : books) 
        {
            file << book.id << "," << book.name << "," << book.author << "," << book.available << "\n";
        }
        file.close();
    }
}

void load_books_from_file(vector<Book>& books) 
{
    ifstream file("books.txt");
    if (file.is_open()) 
    {
        books.clear();
        Book book;
        char comma;
        while (file >> book.id >> comma) 
        {
            file.getline(book.name, 100, ',');
            file.getline(book.author, 100, ',');
            file >> book.available;
            books.push_back(book);
        }
        file.close();
    }
}

void saveStudentsToFile(const vector<Student>& students) 
{
    ofstream file("students.txt");
    if (file.is_open()) 
    {
        for (const auto& student : students) 
        {
            file << student.username << "," << student.password << "," << student.balance << ",";
            for (int i = 0; i < student.borrowed_count; i++) 
            {
                file << student.borrowed_books[i];
                if (i < student.borrowed_count - 1) file << ";";
            }
            file << "\n";
        }
        file.close();
    }
}

void loadStudentsFromFile(vector<Student>& students) 
{
    ifstream file("students.txt");
    if (file.is_open()) 
    {
        students.clear();
        string line;
        while (getline(file, line)) 
        {
            Student student;
            size_t pos = line.find(',');
            strncpy(student.username, line.substr(0, pos).c_str(), 50);
            line.erase(0, pos + 1);

            pos = line.find(',');
            strncpy(student.password, line.substr(0, pos).c_str(), 50);
            line.erase(0, pos + 1);

            pos = line.find(',');
            student.balance = stof(line.substr(0, pos));
            line.erase(0, pos + 1);


            student.borrowed_count = 0;
            if (!line.empty()) 
            {
                size_t semicolon_pos;
                while ((semicolon_pos = line.find(';')) != string::npos) 
                {
                    student.borrowed_books[student.borrowed_count++] = stoi(line.substr(0, semicolon_pos));
                    line.erase(0, semicolon_pos + 1);
                }
                if (!line.empty()) {
                    student.borrowed_books[student.borrowed_count++] = stoi(line);
                }
            }

            students.push_back(student);
        }
        file.close();
    }
}


bool adminLogin() 
{
    string password;
    cout << "Enter admin password: ";
    cin >> password;
    return password == "367cc77E";
}

void addBook(vector<Book>& books) 
{
    Book newBook;
    cout << "Enter book ID: ";
    cin >> newBook.id;
    cout << "Enter book name: ";
    cin.ignore();
    cin.getline(newBook.name, 100);
    cout << "Enter book author: ";
    cin.getline(newBook.author, 100);
    newBook.available = true;
    books.push_back(newBook);
    save_books_to_file(books);
    cout << "Book added successfully!\n";
}

void editBook(vector<Book>& books) 
{
    int id;
    cout << "Enter book ID to edit: ";
    cin >> id;

    for (auto& book : books) {
        if (book.id == id) {
            cout << "Enter new name: ";
            cin.ignore();
            cin.getline(book.name, 100);
            cout << "Enter new author: ";
            cin.getline(book.author, 100);
            save_books_to_file(books);
            cout << "Book updated successfully!\n";
            return;
        }
    }
    cout << "Book not found!\n";
}

void registerStudent(vector<Student>& students) 
{
    Student newStudent;
    cout << "Enter username: ";
    cin >> newStudent.username;
    cout << "Enter password: ";
    cin >> newStudent.password;
    newStudent.balance = 50.0f; 
    newStudent.borrowed_count = 0;
    students.push_back(newStudent);
    saveStudentsToFile(students);
    cout << "Student registered successfully with initial balance of $50!\n";
}

void viewStudents(const vector<Student>& students) 
{
    cout << "\nRegistered Students:\n";
    cout << left << setw(20) << "Username" << setw(10) << "Balance" << "\n";
    for (const auto& student : students) {
        cout << left << setw(20) << student.username << setw(10) << student.balance << "\n";
    }
}

void checkStudentBalance(const vector<Student>& students) 
{
    char username[50];
    cout << "Enter student username: ";
    cin >> username;

    for (const auto& student : students) 
    {
        if (strcmp(student.username, username) == 0) {
            cout << "Balance for " << username << " is: $" << student.balance << "\n";
            return;
        }
    }
    cout << "Student not found!\n";
}

void adminMenu(vector<Book>& books, vector<Student>& students) 
{
    int choice;
    do {
        clearScreen();
        cout << "\nAdmin Menu\n";
        cout << "1. Add Book\n";
        cout << "2. Edit Book\n";
        cout << "3. Register Student\n";
        cout << "4. View Students\n";
        cout << "5. Check Student Balance\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(books); break;
            case 2: editBook(books); break;
            case 3: registerStudent(students); break;
            case 4: viewStudents(students); break;
            case 5: checkStudentBalance(students); break;
            case 6: break;
            default: cout << "Invalid choice!\n";
        }
        pauseScreen();
    } while (choice != 6);
}


int studentLogin(const vector<Student>& students, char* username) 
{
    char password[50];
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    for (int i = 0; i < students.size(); i++) {
        if (strcmp(students[i].username, username) == 0 && 
            strcmp(students[i].password, password) == 0) 
            {
            return i;
        }
    }
    return -1; // ناموفق
}

void viewAvailableBooks(const vector<Book>& books) 
{
    cout << "\nAvailable Books:\n";
    cout << left << setw(10) << "ID" << setw(30) << "name" << setw(25) << "Author" << "\n";
    for (const auto& book : books) 
    {
        if (book.available) 
        {
            cout << left << setw(10) << book.id << setw(30) << book.name << setw(25) << book.author << "\n";
        }
    }
}

void borrowBook(vector<Book>& books, Student& student, vector<Student>& students) 
{
    if (student.balance < 2.0f) {
        cout << "Insufficient balance ($2 required). Current balance: $" << student.balance << "\n";
        return;
    }

    viewAvailableBooks(books);
    int bookId;
    cout << "Enter book ID to borrow: ";
    cin >> bookId;

    for (auto& book : books) {
        if (book.id == bookId && book.available) 
        {
            if (student.borrowed_count < 10) 
            {
                book.available = false;
                student.borrowed_books[student.borrowed_count++] = bookId;
                student.balance -= 2.0f;
                save_books_to_file(books);
                saveStudentsToFile(students);
                cout << "Book borrowed successfully! $2 deducted from your account.\n";
            } 
            else 
            {
                cout << "You have reached the maximum number of borrowed books (10).\n";
            }
            return;
        }
    }
    cout << "Book not available or not found!\n";
}

void returnBook(vector<Book>& books, Student& student, vector<Student>& students) 
{
    if (student.borrowed_count == 0) {
        cout << "You haven't borrowed any books!\n";
        return;
    }

    cout << "Your borrowed books:\n";
    for (int i = 0; i < student.borrowed_count; i++) 
    {
        for (const auto& book : books) {
            if (book.id == student.borrowed_books[i]) 
            {
                cout << "ID: " << book.id << " - " << book.name << " by " << book.author << "\n";
                break;
            }
        }
    }

    int bookId;
    cout << "Enter book ID to return: ";
    cin >> bookId;

    for (int i = 0; i < student.borrowed_count; i++) {
        if (student.borrowed_books[i] == bookId) {

            for (int j = i; j < student.borrowed_count - 1; j++) {
                student.borrowed_books[j] = student.borrowed_books[j + 1];
            }
            student.borrowed_count--;


            for (auto& book : books) {
                if (book.id == bookId) {
                    book.available = true;
                    break;
                }
            }

            save_books_to_file(books);
            saveStudentsToFile(students);
            cout << "Book returned successfully!\n";
            return;
        }
    }
    cout << "Book ID not found in your borrowed list!\n";
}

void rechargeAccount(Student& student, vector<Student>& students) {
    float amount;
    cout << "Current balance: $" << student.balance << "\n";
    cout << "Enter amount to recharge: $";
    cin >> amount;

    if (amount > 0) {
        student.balance += amount;
        saveStudentsToFile(students);
        cout << "Account recharged successfully! New balance: $" << student.balance << "\n";
    } else {
        cout << "Invalid amount!\n";
    }
}

void studentMenu(vector<Book>& books, vector<Student>& students, int studentIndex) {
    int choice;
    do {
        clearScreen();
        cout << "\nStudent Menu - Welcome " << students[studentIndex].username << "\n";
        cout << "Current balance: $" << students[studentIndex].balance << "\n";
        cout << "1. View Available Books\n";
        cout << "2. Borrow a Book\n";
        cout << "3. Return a Book\n";
        cout << "4. Recharge Account\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewAvailableBooks(books); break;
            case 2: borrowBook(books, students[studentIndex], students); break;
            case 3: returnBook(books, students[studentIndex], students); break;
            case 4: rechargeAccount(students[studentIndex], students); break;
            case 5: break;
            default: cout << "Invalid choice!\n";
        }
        pauseScreen();
    } while (choice != 5);
}


int main() {
    vector<Book> books;
    vector<Student> students;


    load_books_from_file(books);
    loadStudentsFromFile(students);

    int choice;
    do {
        clearScreen();
        cout << "\nLibrary Management System\n";
        cout << "1. Login as Admin\n";
        cout << "2. Login as Student\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: 
            {
                if (adminLogin()) 
                {
                    adminMenu(books, students);
                } else 
                {
                    cout << "Invalid admin password!\n";
                    pauseScreen();
                }
                break;
            }
            case 2: {
                char username[50];
                int studentIndex = studentLogin(students, username);
                if (studentIndex != -1) 
                {
                    studentMenu(books, students, studentIndex);
                } else 
                {
                    cout << "Invalid username or password!\n";
                    pauseScreen();
                }
                break;
            }
            case 0:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice!\n";
                pauseScreen();
        }
    } while (choice != 0);

    return 0;
}