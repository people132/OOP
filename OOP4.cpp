#include <iostream>
#include <random>
#include <memory>
#include <climits>
using namespace std;    


// Структура узла Декартова дерева
struct Node {
    int key, priority, size;                 // Ключ, приоритет и размер поддерева
    unique_ptr<Node> left, right;            // Левые и правые дочерние узлы
    Node* parent;                            // Указатель на родительский узел

    Node(int k, int p) : key(k), priority(p), size(1), parent(nullptr), left(nullptr), right(nullptr) {}
};

// Получение размера поддерева
int getSize(const unique_ptr<Node>& node) {
    return node ? node->size : 0;            // Возвращаем размер поддерева, если узел существует, иначе 0
}

// Обновление размера поддерева
void updateSize(Node* node) {
    if (node) {
        node->size = 1 + getSize(node->left) + getSize(node->right); // Размер = 1 (текущий узел) + размеры левого и правого поддеревьев
    }
}

// Объединение двух деревьев L и R
unique_ptr<Node> merge(unique_ptr<Node> L, unique_ptr<Node> R) {
    if (!L) return R;                        // Если L пустое, возвращаем R
    if (!R) return L;                        // Если R пустое, возвращаем L
    
    // Выбор корня в зависимости от приоритета
    if (L->priority > R->priority) {         // Если приоритет L больше, то он становится корнем
        L->right = merge(std::move(L->right), std::move(R)); // Рекурсивно объединяем R с правым поддеревом L
        if (L->right) L->right->parent = L.get(); // Устанавливаем родителя для правого дочернего узла
        updateSize(L.get());                 // Обновляем размер узла L
        return L;
    } else {                                 // Иначе R становится корнем
        R->left = merge(std::move(L), std::move(R->left)); // Рекурсивно объединяем L с левым поддеревом R
        if (R->left) R->left->parent = R.get(); // Устанавливаем родителя для левого дочернего узла
        updateSize(R.get());                 // Обновляем размер узла R
        return R;
    }
}

// Разделение дерева по ключу key на два поддерева L и R
void split(unique_ptr<Node> node, int key, unique_ptr<Node>& L, unique_ptr<Node>& R) {
    if (!node) {                             // Если узел пустой, L и R также пустые
        L = nullptr;
        R = nullptr;
        return;
    }
    if (node->key < key) {                   // Если ключ узла меньше ключа разделения
        L = std::move(node);                      // Узел уходит в левое поддерево
        split(std::move(L->right), key, L->right, R); // Рекурсивно разделяем правое поддерево L
        if (L->right) L->right->parent = L.get(); // Устанавливаем родителя для правого дочернего узла
        updateSize(L.get());                 // Обновляем размер узла L
    } else {                                 // Если ключ узла больше или равен ключу разделения
        R = std::move(node);                      // Узел уходит в правое поддерево
        split(std::move(R->left), key, L, R->left); // Рекурсивно разделяем левое поддерево R
        if (R->left) R->left->parent = R.get(); // Устанавливаем родителя для левого дочернего узла
        updateSize(R.get());                 // Обновляем размер узла R
    }
}

// Функции поиска минимального и максимального узлов в дереве
Node* findMin(Node* node) {
    while (node && node->left) {             // Переход к самому левому узлу
        node = node->left.get();
    }
    return node;                             // Возвращаем минимальный узел
}

Node* findMax(Node* node) {
    while (node && node->right) {            // Переход к самому правому узлу
        node = node->right.get();
    }
    return node;                             // Возвращаем максимальный узел
}

// Функция нахождения следующего элемента (successor)
Node* successor(Node* node) {
    if (node->right) {                       // Если есть правое поддерево
        return findMin(node->right.get());   // Минимальный узел в правом поддереве — следующий элемент
    }
    while (node->parent && node == node->parent->right.get()) { // Поднимаемся вверх, пока не найдём узел, являющийся левым ребёнком
        node = node->parent;
    }
    return node->parent;                     // Возвращаем родительский узел
}

// Итератор для обхода дерева в порядке возрастания
class TreapIterator {
    Node* node;                              // Текущий узел итератора

public:
    TreapIterator(Node* n = nullptr) : node(n) {}

    int operator*() const { return node->key; } // Оператор разыменования возвращает ключ текущего узла

    TreapIterator& operator++() {            // Оператор инкремента
        node = successor(node);              // Переход к следующему узлу
        return *this;
    }

    bool operator!=(const TreapIterator& other) const { // Оператор сравнения
        return node != other.node;
    }
};

// Класс для Декартова дерева по неявному ключу
class ImplicitTreap : protected Node {
public:
    using Node::Node;

    // Объединение деревьев по неявному ключу (без учёта ключей)
    static unique_ptr<Node> mergeImplicit(unique_ptr<Node> L, unique_ptr<Node> R) {
        if (!L) return R;
        if (!R) return L;

        if (L->priority > R->priority) {
            L->right = mergeImplicit(std::move(L->right), std::move(R));
            if (L->right) L->right->parent = L.get();
            updateSize(L.get());
            return L;
        } else {
            R->left = mergeImplicit(std::move(L), std::move(R->left));
            if (R->left) R->left->parent = R.get();
            updateSize(R.get());
            return R;
        }
    }

    // Разделение по размеру
    static void splitBySize(unique_ptr<Node> node, int size, unique_ptr<Node>& L, unique_ptr<Node>& R) {
        if (!node) {
            L = nullptr;
            R = nullptr;
            return;
        }
        if (getSize(node->left) < size) {    // Если размер левого поддерева меньше размера разделения
            L = std::move(node);
            splitBySize(std::move(L->right), size - getSize(L->left) - 1, L->right, R);
            if (L->right) L->right->parent = L.get();
            updateSize(L.get());
        } else {
            R = std::move(node);
            splitBySize(std::move(R->left), size, L, R->left);
            if (R->left) R->left->parent = R.get();
            updateSize(R.get());
        }
    }

    // Групповая функция: поиск максимального значения на отрезке
    static int rangeMax(Node* node) {
        if (!node) return INT_MIN;
        int maxLeft = rangeMax(node->left.get());
        int maxRight = rangeMax(node->right.get());
        return max({node->key, maxLeft, maxRight});
    }
};

int main() {
    // 1. Тестирование вставки и объединения
    cout << "Test 1: Insertion and Merge\n";
    unique_ptr<Node> root = make_unique<Node>(10, rand());
    root = merge(std::move(root), make_unique<Node>(20, rand()));
    root = merge(std::move(root), make_unique<Node>(5, rand()));
    root = merge(std::move(root), make_unique<Node>(15, rand()));
    cout << "Tree after insertion and merging nodes with keys 10, 20, 5, 15:\n";
    cout << "Root: " << root->key << ", Size: " << root->size << "\n";

    // 2. Тестирование разделения по ключу
    cout << "\nTest 2: Split by Key\n";
    unique_ptr<Node> L, R;
    split(std::move(root), 15, L, R);  // Разделение по ключу 15
    cout << "After split by key 15:\n";
    cout << "Left subtree max: " << (L ? findMax(L.get())->key : -1) << "\n";
    cout << "Right subtree min: " << (R ? findMin(R.get())->key : -1) << "\n";

    // 3. Тестирование функции объединения с неявным ключом
    cout << "\nTest 3: Merge with Implicit Key\n";
    unique_ptr<Node> leftTree = make_unique<Node>(1, rand());
    leftTree = merge(std::move(leftTree), make_unique<Node>(2, rand()));
    unique_ptr<Node> rightTree = make_unique<Node>(3, rand());
    unique_ptr<Node> mergedTree = ImplicitTreap::mergeImplicit(std::move(leftTree), std::move(rightTree));
    cout << "After merging two implicit trees:\n";
    cout << "Root of merged tree: " << mergedTree->key << "\n";

    // 4. Тестирование разделения по размеру
    cout << "\nTest 4: Split by Size\n";
    unique_ptr<Node> smallLeft, smallRight;
    ImplicitTreap::splitBySize(std::move(mergedTree), 1, smallLeft, smallRight);
    cout << "After split by size 1:\n";
    cout << "Left subtree size: " << getSize(smallLeft) << ", Right subtree size: " << getSize(smallRight) << "\n";

    // 5. Тестирование поиска максимального значения
    cout << "\nTest 5: Range Max\n";
    unique_ptr<Node> testTree = make_unique<Node>(50, rand());
    testTree = merge(std::move(testTree), make_unique<Node>(30, rand()));
    testTree = merge(std::move(testTree), make_unique<Node>(70, rand()));
    cout << "Max value in the tree: " << ImplicitTreap::rangeMax(testTree.get()) << "\n";

    // 6. Тестирование итератора
    cout << "\nTest 6: Tree Iterator\n";
    unique_ptr<Node> iterTree = make_unique<Node>(10, rand());
    iterTree = merge(std::move(iterTree), make_unique<Node>(5, rand()));
    iterTree = merge(std::move(iterTree), make_unique<Node>(15, rand()));
    cout << "Iterating over the tree:\n";
    for (TreapIterator it(findMin(iterTree.get())); it != TreapIterator(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    return 0;
}