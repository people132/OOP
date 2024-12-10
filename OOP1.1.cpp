#include <iostream>
#include <list>
#include <complex>
#include <cmath>
#include <iterator>

using namespace std;


// Функция для добавления элемента в отсортированный список
void push(list<char>& lst, const char& element) {
    auto it = lst.begin();
    while (it != lst.end()) {
        if (*it > element) {
            break;
        }
        ++it;
    }
    lst.insert(it, element);
}

// Функция для удаления элемента из списка
void pop(list<char>& lst, const char& element) {
    auto it = find(lst.begin(), lst.end(), element);
    if (it != lst.end()) {
        lst.erase(it);
    }
}

// Фильтрация элементов списка
list<char> filter(const list<char>& lst, bool (*predicate)(const char&)) {
    list<char> result;
    for (const auto& elem : lst) {
        if (predicate(elem)) {
            result.push_back(elem);
        }
    }
    return result;
}

// Предикат: возвращает true, если символ согласная буква
bool isImaginary(const char& num) {
    return num == 'B' || num == 'C' || num == 'D' || num == 'F' || num == 'G' || num == 'H' || num == 'J' || num == 'K' ||
       num == 'L' || num == 'M' || num == 'N' || num == 'P' || num == 'Q' || num == 'R' || num == 'S' || num == 'T' ||
       num == 'V' || num == 'W' || num == 'X' || num == 'Y' || num == 'Z';
}

// Функция вывода списка
void printList(const list<char>& lst) {
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}

int main() {
    list<char> lst;

    // Добавление элементов в список
    push(lst, 'A'); 
    push(lst, 'B'); 
    push(lst, 'C'); 
    push(lst, 'D'); 
    push(lst, 'E');
    push(lst, 'F');

    cout << "List after adding items: ";
    printList(lst);

    pop(lst, 'C');
    // Вывод списка
    cout << "List after pop: C:  ";
    printList(lst);

    // Фильтрация списка по предикату (оставляем только гласные буквы)
    list<char> imagList = filter(lst, isImaginary);
    cout << "List after filtering (vowel letters only): ";
    printList(imagList);

    return 0;
}