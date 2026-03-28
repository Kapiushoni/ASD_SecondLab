#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <windows.h>

using namespace std;

const int DEFAULT_CAPACITY = 100;


// СТРУКТУРА ЗГІДНО ВАРІАНТУ 5

struct MathLesson {
    const char* teacherName; // Використовуємо const char* замість string
    int lessonNumber;
    int presentStudents;

    MathLesson() : teacherName(""), lessonNumber(0), presentStudents(0) {}  // Конструктор за замовчуванням
    MathLesson(const char* name, int number, int students)                  // Конструктор якщо будуть дані
        : teacherName(name), lessonNumber(number), presentStudents(students) {}

    void print() {
        cout << "Викладач: " << left << setw(20) << teacherName 
             << " | Пара №: " << left << setw(3) << lessonNumber
             << " | Присутніх: " << presentStudents << endl;
    }
};


// 2. ДИНАМІЧНИЙ МАСИВ

struct DynamicArray {
    private:
        MathLesson** lessons;
        int current_size; // Перейменовано, щоб не конфліктувало з методом size()
        int capacity;

        void expand_array() {
            MathLesson** newLessons = new MathLesson*[capacity * 2];
            for (int i = 0; i < current_size; ++i)
                newLessons[i] = lessons[i];
            delete[] lessons;
            lessons = newLessons;
            capacity *= 2;
        }

    public:
        DynamicArray() : current_size(0), capacity(DEFAULT_CAPACITY) { 
            lessons = new MathLesson*[capacity]; 
        }
        ~DynamicArray() { clear(); }

        void push_back(MathLesson* v) {
            if (current_size == capacity) expand_array();
            lessons[current_size++] = v;
        }

        MathLesson* pop_back() {
            if (current_size <= 0) throw out_of_range("Dynamic array empty");
            return lessons[--current_size];
        }

        void push_front(MathLesson* v) {
            if (current_size == capacity) expand_array();
            for (int i = current_size; i > 0; i--) {
                lessons[i] = lessons[i - 1];
            }
            lessons[0] = v;
            current_size++;
        }

        MathLesson* pop_front() {
            if (current_size <= 0) throw out_of_range("Dynamic array empty");
            MathLesson* v = lessons[0];
            for (int i = 0; i < current_size - 1; i++)
                lessons[i] = lessons[i + 1];
            current_size--; 
            return v;
        }

        // ЗАВДАННЯ 4: Метод insert (вставка по індексу)
        void insert(int index, MathLesson* v) {
            if (index < 0 || index > current_size) throw out_of_range("Index out of bounds");
            if (current_size == capacity) expand_array();
            for (int i = current_size; i > index; i--) {
                lessons[i] = lessons[i - 1];
            }
            lessons[index] = v;
            current_size++;
        }

        // ЗАВДАННЯ 4: Метод remove (видалення по індексу)
        MathLesson* remove(int index) {
            if (index < 0 || index >= current_size) throw out_of_range("Index out of bounds");
            MathLesson* v = lessons[index];
            for (int i = index; i < current_size - 1; i++) {
                lessons[i] = lessons[i + 1];
            }
            current_size--;
            return v;
        }

        MathLesson* get(int index) {
            if (index >= current_size || index < 0) throw out_of_range("Index out of bounds");
            return lessons[index];
        }

        MathLesson* find_by_number(int targetNumber) {
            for (int i = 0; i < current_size; i++) {
                if (lessons[i]->lessonNumber == targetNumber) return lessons[i];
            }
            return nullptr;
        }

        int size() { return current_size; } // Вимога методички: метод має зватися size()

        void print() {
            for (int i = 0; i < current_size; i++) {
                cout << "[" << i << "] ";
                lessons[i]->print();
            }
        }

        void clear() {
            for (int i = 0; i < current_size; i++) delete lessons[i];
            delete[] lessons;
            capacity = DEFAULT_CAPACITY;
            current_size = 0;
            lessons = new MathLesson*[capacity];
        }
};


// 3. ОДНОЗВ'ЯЗНИЙ СПИСОК

struct Node {
    MathLesson* element;
    Node* next;
    Node(MathLesson* v, Node* n = nullptr): element(v), next(n) {}
};

struct LinkedList {
    Node* head;
    int current_size;

    LinkedList(): head(nullptr), current_size(0) {}
    ~LinkedList() { clear(); }

    void push_back(MathLesson* v) {
        if (head == nullptr) {
            head = new Node(v);
            current_size++;
            return;
        }
        Node* current = head;
        while (current->next != nullptr) current = current->next;
        current->next = new Node(v);
        current_size++;
    }

    void push_front(MathLesson* v) {
        head = new Node(v, head);
        current_size++;
    }

    MathLesson* pop_back() {
        if (head == nullptr) throw out_of_range("Empty linked list");
        if (head->next == nullptr) {
            MathLesson* v = head->element;
            delete head;
            head = nullptr;
            current_size--;
            return v;
        }
        Node* current = head;
        while (current->next->next != nullptr) current = current->next;
        MathLesson* v = current->next->element;
        delete current->next;
        current->next = nullptr;
        current_size--;
        return v;
    }

    MathLesson* pop_front() {
        if (head == nullptr) throw out_of_range("Empty linked list");
        Node* temp = head;
        MathLesson* v = temp->element;
        head = head->next;
        delete temp;
        current_size--;
        return v;
    }

    // ЗАВДАННЯ 4: Метод insert (вставка по індексу)
    void insert(int index, MathLesson* v) {
        if (index < 0 || index > current_size) throw out_of_range("Index out of bounds");
        if (index == 0) {
            push_front(v);
            return;
        }
        Node* current = head;
        for (int i = 0; i < index - 1; i++) current = current->next;
        current->next = new Node(v, current->next);
        current_size++;
    }

    // ЗАВДАННЯ 4: Метод remove (видалення по індексу)
    MathLesson* remove(int index) {
        if (index < 0 || index >= current_size) throw out_of_range("Index out of bounds");
        if (index == 0) return pop_front();
        
        Node* current = head;
        for (int i = 0; i < index - 1; i++) current = current->next;
        
        Node* temp = current->next;
        MathLesson* v = temp->element;
        current->next = temp->next;
        delete temp;
        current_size--;
        return v;
    }

    MathLesson* get(int index) {
        if (index < 0 || index >= current_size) throw out_of_range("Index out of bounds");
        Node* current = head;
        for (int i = 0; i < index; i++) current = current->next;
        return current->element;
    }

    int size() { return current_size; } // Вимога методички: метод має зватися size()

    void print() {
        if (head == nullptr) { cout << "Список порожній." << endl; return; }
        int i = 0;
        Node* current = head;
        while (current != nullptr) {
            cout << "[" << i++ << "] ";
            current->element->print();
            current = current->next;
        }
    }

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        current_size = 0;
    }
};


// ДОДАТКОВІ ЗАВДАННЯ (Eolymp)


// ДОДАТКОВЕ 1: Стек для перевірки дужок (на основі зв'язного списку)
struct CharNode {
    char data;
    CharNode* next;
    CharNode(char d, CharNode* n = nullptr) : data(d), next(n) {}
};

struct CharStack {
    CharNode* topNode = nullptr;
    void push(char c) { topNode = new CharNode(c, topNode); }
    char pop() {
        if (!topNode) return '\0';
        char c = topNode->data;
        CharNode* temp = topNode;
        topNode = topNode->next;
        delete temp;
        return c;
    }
    bool isEmpty() { return topNode == nullptr; }
    ~CharStack() { while (!isEmpty()) pop(); }
};

bool checkBrackets(const char* str) {
    CharStack stack;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c == '(' || c == '[' || c == '{') {
            stack.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (stack.isEmpty()) return false;
            char top = stack.pop();
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) return false;
        }
    }
    return stack.isEmpty();
}

// ДОДАТКОВЕ 2: Розворот однозв'язного списку (повертає нову голову)
Node* reverseList(Node* head) {
    Node* prev = nullptr;
    Node* current = head;
    while (current != nullptr) {
        Node* nextTemp = current->next;
        current->next = prev;
        prev = current;
        current = nextTemp;
    }
    return prev;
}

// ДОДАТКОВЕ 3: Перевірка списку на циклічність (Алгоритм Флойда)
int hasCycle(Node* head) {
    if (head == nullptr) return 0;
    Node* slow = head;
    Node* fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return 1; // Зациклення знайдено
    }
    return 0; // Немає циклу
}



// 5. АНАЛІЗ ШВИДКОДІЇ

template <typename T>
void struct_time_analysis(T& arr, double avgTimes[5], double totalTimes[5], double& overallTotal) {
    MathLesson* test = new MathLesson("Тестовий В.В.", 99, 15);
    overallTotal = 0; // Обнуляємо загальний час перед початком
    
    // 5.1 Додати в кінець 50000 елементів
    clock_t start = clock();
    for (int j = 0; j < 50000; j++) arr.push_back(test);
    totalTimes[0] = (double)(clock() - start) / CLOCKS_PER_SEC;
    avgTimes[0] = totalTimes[0] / 50000;

    // 5.2 Додати в початок 10000 елементів
    start = clock();
    for (int j = 0; j < 10000; j++) arr.push_front(test);
    totalTimes[1] = (double)(clock() - start) / CLOCKS_PER_SEC;
    avgTimes[1] = totalTimes[1] / 10000;

    // 5.3 Зчитати 20000 елементів під випадковими індексами
    start = clock();
    for (int j = 0; j < 20000; j++) {
        arr.get(rand() % arr.size()); 
    }
    totalTimes[2] = (double)(clock() - start) / CLOCKS_PER_SEC;
    avgTimes[2] = totalTimes[2] / 20000;

    // 5.4 Видалити 5000 елементів з початку
    start = clock();
    for (int j = 0; j < 5000; j++) arr.pop_front();
    totalTimes[3] = (double)(clock() - start) / CLOCKS_PER_SEC;
    avgTimes[3] = totalTimes[3] / 5000;

    // 5.5 Видалити 5000 елементів з кінця
    start = clock();
    for (int j = 0; j < 5000; j++) arr.pop_back();
    totalTimes[4] = (double)(clock() - start) / CLOCKS_PER_SEC;
    avgTimes[4] = totalTimes[4] / 5000;

    // Рахуємо суму всіх кроків
    for (int i = 0; i < 5; i++) overallTotal += totalTimes[i];
}

void struct_time_info(double avgTimes[5], double totalTimes[5], double overallTotal) {
    const char* labels[] = {"5.1 Push Back", "5.2 Push Front", "5.3 Random Get", "5.4 Pop Front ", "5.5 Pop Back  "};
    
    cout << left << setw(15) << "Операція" 
         << setw(20) << "   Загальний час (с)" 
         << "       Сер. час (с)" << endl;
    cout << string(55, '-') << endl;

    for (int i = 0; i < 5; i++)
        cout << left << setw(15) << labels[i] 
             << fixed << setprecision(6) << setw(20) << totalTimes[i] 
             << scientific << setprecision(3) << avgTimes[i] << endl;
    
    cout << string(55, '-') << endl;
    cout << "ЗАГАЛЬНИЙ ЧАС УСІХ КРОКІВ: " << fixed << setprecision(6) << overallTotal << " с" << endl;
    
    if (overallTotal > 10.0) {
        cout << "(!) Увага: Час перевищує 10 секунд. Перевірте ефективність методів." << endl;
    } else {
        cout << "(+) Час у межах норми (до 10 секунд)." << endl;
    }
}



// ГОЛОВНА ФУНКЦІЯ

int main() {
    // Налаштування для коректного відображення кирилиці в консолі Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "uk_UA.UTF-8");
    srand(time(NULL));

    cout << "=== ДЕМОНСТРАЦІЯ INSERT ТА REMOVE (ЗАВДАННЯ 4) ===" << endl;
    LinkedList demoList;
    demoList.push_back(new MathLesson("Перший В.В.", 1, 10));
    demoList.push_back(new MathLesson("Другий А.А.", 2, 20));
    cout << "Початковий список:" << endl;
    demoList.print();
    
    cout << "\nВставка нового елемента на індекс 1:" << endl;
    demoList.insert(1, new MathLesson("Вставлений І.І.", 99, 15));
    demoList.print();

    cout << "\nВидалення елемента з індексу 0:" << endl;
    demoList.remove(0);
    demoList.print();
    cout << string(50, '-') << "\n\n";

    cout << "=== ДОДАТКОВІ ЗАВДАННЯ ===" << endl;
    const char* str1 = "{[()]}";
    const char* str2 = "{[(])}";
    cout << "Дужки " << str1 << " -> " << (checkBrackets(str1) ? "Коректні" : "Помилка") << endl;
    cout << "Дужки " << str2 << " -> " << (checkBrackets(str2) ? "Коректні" : "Помилка") << endl;
    
    cout << "\nРозворот списку:" << endl;
    demoList.head = reverseList(demoList.head);
    demoList.print();
    
    cout << "\nПеревірка на цикл: " << (hasCycle(demoList.head) ? "Є цикл!" : "Циклу немає.") << endl;
    cout << string(50, '-') << "\n\n";

    demoList.clear();

    // --- ТЕСТУВАННЯ ШВИДКОДІЇ ---
    cout << "=== ТЕСТУВАННЯ ШВИДКОДІЇ (ЗАВДАННЯ 5) ===" << endl;
    DynamicArray dynArr;
    double avgDynamicTimes[5], totDynamicTimes[5], overallDyn;
    cout << "-> Аналіз Динамічного Масиву:" << endl;
    struct_time_analysis(dynArr, avgDynamicTimes, totDynamicTimes, overallDyn);
    struct_time_info(avgDynamicTimes, totDynamicTimes, overallDyn);

    cout << endl;

    LinkedList linkedArr;
    double avgLinkedTimes[5], totLinkedTimes[5], overallLinked;
    cout << "-> Аналіз Зв'язного Списку:" << endl;
    struct_time_analysis(linkedArr, avgLinkedTimes, totLinkedTimes, overallLinked);
    struct_time_info(avgLinkedTimes, totLinkedTimes, overallLinked);

    return 0;
}