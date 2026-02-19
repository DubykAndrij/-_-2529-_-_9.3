#define NOMINMAX // Забороняє windows.h визначати макроси min/max
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <windows.h>

using namespace std;

// Структура для зберігання даних про студента
struct Student {
    string surname;
    string initials;
    int grades[5] = { 0, 0, 0, 0, 0 }; // Ініціалізація для усунення C26495
};

// --- Прототипи функцій ---
void showMenu();
int getValidatedInt(string message);
Student inputStudentData();
void printStudent(const Student& s, int index);

void addStudent(Student*& students, int& size);
void removeStudent(Student*& students, int& size, int index);
void editStudent(Student* students, int size, int index);

void displayAll(Student* students, int size);
void displayExcellent(Student* students, int size);
void displayWithThrees(Student* students, int size);
void filterAndDisplayFailing(Student*& students, int& size);

void saveToFile(Student* students, int size);
void loadFromFile(Student*& students, int& size);

// --- Основна функція ---
int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    Student* students = nullptr;
    int size = 0;
    int choice;

    do {
        showMenu();
        choice = getValidatedInt("Ваш вибір: ");

        switch (choice) {
        case 1: addStudent(students, size); break;
        case 2:
            displayAll(students, size);
            if (size > 0) {
                int idx = getValidatedInt("Введіть номер для вилучення: ") - 1;
                removeStudent(students, size, idx);
            }
            break;
        case 3:
            displayAll(students, size);
            if (size > 0) {
                int idx = getValidatedInt("Введіть номер для редагування: ") - 1;
                editStudent(students, size, idx);
            }
            break;
        case 4: displayAll(students, size); break;
        case 5: displayExcellent(students, size); break;
        case 6: displayWithThrees(students, size); break;
        case 7: filterAndDisplayFailing(students, size); break;
        case 8: saveToFile(students, size); break;
        case 9: loadFromFile(students, size); break;
        case 0: cout << "Вихід..." << endl; break;
        default: cout << "Невірний вибір!" << endl;
        }
    } while (choice != 0);

    delete[] students;
    return 0;
}

// --- Реалізація функцій з виправленнями помилок ---

int getValidatedInt(string message) {
    int value;
    while (true) {
        cout << message;
        if (cin >> value) {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n'); // Додано дужки навколо max
            return value;
        }
        cout << "Помилка! Введіть число." << endl;
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n'); // Додано дужки навколо max
    }
}

void showMenu() {
    cout << "\n1. Додати 2. Вилучити 3. Редагувати 4. Список 5. Відмінники 6. Трійки 7. Двійки 8. Запис 9. Читання 0. Вихід\n";
}

Student inputStudentData() {
    Student s;
    cout << "Прізвище: "; cin >> s.surname;
    cout << "Ініціали: "; cin >> s.initials;
    for (int i = 0; i < 5; i++) {
        s.grades[i] = getValidatedInt("Оцінка " + to_string(i + 1) + ": ");
    }
    return s;
}

void addStudent(Student*& students, int& size) {
    Student* newArray = new Student[size + 1];
    for (int i = 0; i < size; i++) newArray[i] = students[i];
    newArray[size] = inputStudentData();
    delete[] students;
    students = newArray;
    size++;
}

void removeStudent(Student*& students, int& size, int index) {
    if (index < 0 || index >= size) return;
    if (size == 1) {
        delete[] students;
        students = nullptr;
        size = 0;
    }
    else {
        Student* newArray = new Student[size - 1];
        for (int i = 0, j = 0; i < size; i++) {
            if (i != index) newArray[j++] = students[i];
        }
        delete[] students;
        students = newArray;
        size--;
    }
}

void editStudent(Student* students, int size, int index) {
    if (index >= 0 && index < size) students[index] = inputStudentData();
}

void printStudent(const Student& s, int index) {
    cout << index + 1 << ". " << s.surname << " " << s.initials << " [";
    for (int i = 0; i < 5; i++) cout << s.grades[i] << (i < 4 ? "," : "");
    cout << "]" << endl;
}

void displayAll(Student* students, int size) {
    if (size == 0) cout << "Порожньо." << endl;
    for (int i = 0; i < size; i++) printStudent(students[i], i);
}

void displayExcellent(Student* students, int size) {
    for (int i = 0; i < size; i++) {
        bool exc = true;
        for (int g : students[i].grades) if (g != 5) exc = false;
        if (exc) printStudent(students[i], i);
    }
}

void displayWithThrees(Student* students, int size) {
    for (int i = 0; i < size; i++) {
        bool has3 = false;
        for (int g : students[i].grades) if (g == 3) has3 = true;
        if (has3) printStudent(students[i], i);
    }
}

void filterAndDisplayFailing(Student*& students, int& size) {
    for (int i = 0; i < size; ) {
        int d2 = 0;
        for (int g : students[i].grades) if (g == 2) d2++;
        if (d2 > 1) removeStudent(students, size, i);
        else i++;
    }
    displayAll(students, size);
}

void saveToFile(Student* students, int size) {
    string path; cout << "Файл: "; cin >> path;
    ofstream f(path);
    if (!f) return;
    f << size << endl;
    for (int i = 0; i < size; i++) {
        f << students[i].surname << " " << students[i].initials << " ";
        for (int g : students[i].grades) f << g << " ";
        f << endl;
    }
}

void loadFromFile(Student*& students, int& size) {
    string path; cout << "Файл: "; cin >> path;
    ifstream f(path);
    if (!f) return;
    delete[] students;
    f >> size;
    students = new Student[size];
    for (int i = 0; i < size; i++) {
        f >> students[i].surname >> students[i].initials;
        for (int j = 0; j < 5; j++) f >> students[i].grades[j];
    }
}