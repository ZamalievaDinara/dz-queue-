using namespace std;

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// Пробельные символы
const char* whitespaces = " \t\n\r\f\v";

// Удаляет пробельные символы в начале и конце строки
std::string& trim(std::string& s)
{
	s.erase(s.find_last_not_of(whitespaces) + 1);
	s.erase(0, s.find_first_not_of(whitespaces));
	return s;
}

bool readColumnValue(std::istream& stream, string& value, char splitter)
{
	if (!std::getline(stream, value, splitter))
		return false;
	value = trim(value);
	return true;
}

// Структура "Клиент"
struct Customer
{
	string name;         // Фамилия И.О.
	string sex;          // Пол
	int    age = -1;     // Возраст
	string workPlace;    // Место работы
	float  income = NAN; // Размер дохода (при наличии)
	string goal;         // Цель обращения (кредит/вклад)

	// Парсит строку в поля структуры
	bool readFromStream(std::istream& stream, char splitter)
	{
		std::string value;

		if (!readColumnValue(stream, name, splitter))
			return false;
		if (!readColumnValue(stream, sex, splitter))
			return false;
		if (!readColumnValue(stream, value, splitter))
			return false;
		age = value.empty() ? -1 : std::stoi(value);
		if (!readColumnValue(stream, workPlace, splitter))
			return false;
		if (!readColumnValue(stream, value, splitter))
			return false;
		income = value.empty() ? NAN : std::stof(value);
		if (!readColumnValue(stream, goal, splitter))
			return false;

		return true;
	}

	void writeToStream(std::ostream& stream, char splitter)
	{
		stream << name << splitter;
		stream << sex << splitter;
		if (age > 0)
			stream << age;
		stream << splitter;
		stream << workPlace << splitter;
		if (income != NAN)
			stream << income;
		stream << splitter;
		stream << goal << endl;
	}
};

// Элемент очереди
template<typename T>
struct QueueItem
{
	T value;              // Данные
	QueueItem* next_item; // Указатель на следующий элемент

	QueueItem(const T& itemValue)
	{
		value = itemValue;
		next_item = nullptr;
	}
};

// Очередь
template<typename T>
struct Queue
{
	size_t size = 0;                     // размер очереди
	QueueItem<T>* front_item = nullptr;  // указатель на начало очереди
	QueueItem<T>* back_item = nullptr;  // указатель на конец очереди

	// Возвращает ссылку на первый элемент
	// Обязательно предварительно проверять на наличие элементов в очереди
	T& front()
	{
		return front_item->value;
	}

	// Проверяет, является ли очередь пустой
	bool empty()
	{
		return size == 0;
	}

	// Выталкивает (удаляет) первый элемент из очереди
	void pop()
	{
		if (!front_item)
			return;

		auto* pop_item = front_item;
		front_item = front_item->next_item;
		if (back_item == pop_item)
			back_item == nullptr;
		delete pop_item;
		size--;
	}

	// Добавляет новый элемент в конец очереди
	T& push(const T& value)
	{
		auto* new_item = new QueueItem<T>(value);

		if (back_item)
			back_item->next_item = new_item;
		back_item = new_item;

		if (!front_item)
			front_item = new_item;
		size++;
		return new_item->value;
	}

	// Очищает очередь
	void clear()
	{
		if (empty())
			return;

		while (front_item->next_item)
		{
			auto* delete_item = front_item;
			front_item = front_item->next_item;
			delete delete_item;
		}
		delete front_item;

		front_item = nullptr;
		back_item = nullptr;
		size = 0;
	}

	void readFromStream(std::istream& stream)
	{
		clear();
		for (std::string line; std::getline(stream, line); )
		{
			std::istringstream line_stream(line);
			T value;
			if (value.readFromStream(line_stream, '/t'))
				push(value);
		}
	}

	void writeToStream(std::ostream& stream)
	{
		while (!empty())
		{
			front().writeToStream(stream, '\t');
			pop();
		}
	}
};

int main()
{
	setlocale(LC_ALL, "Rus");

	Queue<Customer> source;
	Queue<Customer> credit;
	Queue<Customer> deposit;

	const std::string source_filename = "source.txt";
	const std::string credit_filename = "credit.txt";
	const std::string deposit_filename = "deposit.txt";


	std::fstream source_stream(source_filename, std::fstream::in);
	if (!source_stream.is_open()) {
		std::cout << "Failed to open " << source_filename << endl;
		return -1;
	}
	source.readFromStream(source_stream);

	while (!source.empty())
	{
		Customer customer = source.front();
		source.pop();

		if (customer.goal == "кредит")
			credit.push(customer);
		else if (customer.goal == "депозит")
			deposit.push(customer);
	}

	std::fstream credit_stream(credit_filename, std::fstream::out);
	if (!credit_stream.is_open()) {
		std::cout << "Failed to open " << credit_filename << endl;
		return -1;
	}
	credit.writeToStream(credit_stream);

	std::fstream deposit_stream(deposit_filename, std::fstream::out);
	if (!deposit_stream.is_open()) {
		std::cout << "Failed to open " << deposit_filename << endl;
		return -1;
	}
	deposit.writeToStream(deposit_stream);

	return 0;
}
