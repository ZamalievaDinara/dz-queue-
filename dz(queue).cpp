using namespace std;

#include <iostream>
#include <string>

enum Sex  { male, female };
enum Goal { credit, deposit };

// Фамилия И. О. | пол | возраст | место работы | размер дохода (при наличии) | цель обращения (кредит/вклад)
struct Customer
{
    string name;
    Sex    sex;
    int    age;
    string workPlace;
    double income = NAN;
    Goal   goal;
};

struct queue
{

    elem* head = nullptr;
    elem* tail;
    size_t size = 0;

};

man create_elem(string name, int age)
{
    man m;
    m.age = age;
    m.name = name;
    return m;
}

// добавить элемент в начало очереди
void enqueue(queue*& memberOfQueue, string name, int age) // memberOfqueue это просто перемннная из структуры queue*&
{
    man m = create_elem(name, age);
    // сначало создаем структуру elem
    elem* newelem = new elem;
    newelem->m = m;

    if (memberOfQueue->size == 0)  // if queue -> head // проверяем случай если в  очереди никого не было до
        memberOfQueue->head = memberOfQueue->tail = newelem;
    else
    {
        memberOfQueue->tail->next = newelem;
        memberOfQueue->tail = newelem;
    }
}

//вытащитб элемнент. показываем ег. удаляем
bool dequeue(queue*& memberOfQueue, man& man)
{
    if (memberOfQueue->size == 0)
        return false;
    man = memberOfQueue->head->m;
    elem* rem = memberOfQueue->head;
    memberOfQueue->head = memberOfQueue->head->next;
    memberOfQueue->size--;
    delete rem;
    return true;
}

// вывод на экран
void Show(queue*& memberOfQueue)
{
    man m;
    while (memberOfQueue->size > 0) //покав размерность  очереди больше 0
    {
        dequeue(memberOfQueue, m);
        cout << m.name << " " << m.age << endl;
    }
}

void clear(queue*& memberOfQueue)
{
    while (memberOfQueue->size > 0) //покав размерность  очереди больше 0
    {
        elem* rem = memberOfQueue->head;
        memberOfQueue->head = memberOfQueue->head->next;
        memberOfQueue->size--;
        delete rem;
    }
}

int main()
{
    queue* q = new queue;
    enqueue(q, "Vasya", 19);
    enqueue(q, "Petya", 16);
    Show(q);
}

