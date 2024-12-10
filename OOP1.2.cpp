#include <iostream>
#include <list>
#include <string>
#include <tuple>

using namespace std;

// Класс Фильм
class Film {
public:
    string director_lastname;
    string director_firstname;
    string title;
    string country;
    int year;
    double cost;
    double revenue;

    // Конструктор
    Film(string lname, string fname, string t, string c, int y, double cost, double rev)
        : director_lastname(lname), director_firstname(fname), title(t), country(c), year(y), cost(cost), revenue(rev) {}

    // Перегрузка оператора "<" для сортировки по приоритету
    bool operator<(const Film& other) const {
        // Основной критерий - доход (по убыванию)
        if (revenue != other.revenue)
            return revenue > other.revenue;

        // Второй критерий - стоимость (по убыванию)
        if (cost != other.cost)
            return cost > other.cost;

        // Третий критерий - год выпуска (по возрастанию)
        if (year != other.year)
            return year < other.year;

        // Четвертый критерий - фамилия режиссера (по возрастанию)
        if (director_lastname != other.director_lastname)
            return director_lastname < other.director_lastname;

        // Пятый критерий - имя режиссера (по возрастанию)
        if (director_firstname != other.director_firstname)
            return director_firstname < other.director_firstname;

        // Шестой критерий - название фильма (по возрастанию)
        return title < other.title;
    }

    // Функция для вывода информации о фильме
    void print() const {
        cout << director_lastname << " " << director_firstname << " - " << title
             << " (" << year << "), " << country << ". Price: " << cost
             << ", Income: " << revenue << endl;
    }
};

// Функция для вставки фильма в отсортированный список
void insert(list<Film>& lst, const Film& film) {
    auto it = lst.begin();
    while (it != lst.end() && !(*it < film)) {
        ++it;
    }
    lst.insert(it, film);
}

// Функция для удаления и возврата фильма с наибольшим приоритетом
Film pop(list<Film>& lst) {
    if (lst.empty()) {
        throw runtime_error("Список пуст!");
    }
    Film top = lst.front();
    lst.pop_front();
    return top;
}

// Функция для вывода всех фильмов в списке
void printList(const list<Film>& lst) {
    for (const auto& film : lst) {
        film.print();
    }
}

int main() {
    list<Film> films;

    // Добавляем несколько фильмов
    insert(films, Film("Smith", "John", "The Adventure", "USA", 2010, 1500000, 3000000));
    insert(films, Film("Doe", "Jane", "Romantic Story", "UK", 2012, 2000000, 5000000));
    insert(films, Film("Anderson", "Paul", "Sci-Fi Saga", "Canada", 2015, 2500000, 7500000));
    insert(films, Film("Lee", "Bruce", "Martial Arts", "China", 2005, 1000000, 8000000));
    insert(films, Film("Smith", "Anna", "Drama", "USA", 2020, 1800000, 4000000));

    cout << "List of film:" << endl;
    printList(films);

    // Удаляем фильм с наибольшим приоритетом
    cout << "\nDeleting a movie with the highest priority..." << endl;
    Film top_film = pop(films);
    cout << "Movie removed:" << endl;
    top_film.print();

    cout << "\nRemaining list of films:" << endl;
    printList(films);

    return 0;
}