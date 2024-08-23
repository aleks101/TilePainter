#include <iostream>
#include <vector>
#include "Log.h"

template<class T>
class Box
{
public:
	Box(int rows, int columns);
	Box(const Box& othr);
	Box& operator=(const Box& obj);
	~Box();

	std::shared_ptr<T>& AccessElement(int i, int j);

	void AddRow();
	void AddColumn();

	void DeleteRow(int row);
	void DeleteColumn(int column);

	int GetRowCount() const;
	int GetColumnCount() const;
	void ResizeRows(int newRowCount);
	void ResizeColumns(int newColumnCount);

	void Clear();
	int GetSize() { return m_cont.size(); }
private:
	void copyFrom(const Box& other);
	int m_rows, m_columns;
	std::vector<std::shared_ptr<T>> m_cont;
};

template<class T>
Box<T>::Box(int rows, int columns) :
	m_rows(rows), m_columns(columns) {
	LOGln("Box class constructor");
	if (rows > 0 && columns > 0)
		m_cont.reserve(rows * columns);
	for (int i = 0; i < m_rows; i++)
		for (int j = 0; j < m_columns; j++)
			m_cont.push_back(std::shared_ptr<T>());
}
template<class T>
void Box<T>::Clear() {
	m_cont.clear();
	m_rows = 0;
	m_columns = 0;
	LOGln("box class cleared");
}
template<class T>
void Box<T>::copyFrom(const Box& other) {
	Clear();
	if (m_cont.capacity() < other.m_cont.capacity())
		m_cont.reserve(other.m_cont.capacity() - m_cont.capacity());
	m_cont = other.m_cont;
	m_rows = other.m_rows;
	m_columns = other.m_columns;
}
template<class T>
Box<T>::Box(const Box& obj) {
	LOGln("Box class copy constructor");
	copyFrom(obj);
}
template<class T>
Box<T>& Box<T>::operator=(const Box& obj) {
	LOGln("Box class = operator");
	if (this != &obj) {
		copyFrom(obj);
	}
	return *this;
}
template<class T>
Box<T>::~Box() {
	Clear();
	LOGln("Box class deconstructor");
}
template<class T>
std::shared_ptr<T>& Box<T>::AccessElement(int i, int j) {
	if (i >= 0 && i < m_rows && j >= 0 && j < m_columns) {
		if (m_cont[i * m_columns + j] == nullptr) {
			m_cont[i * m_columns + j] = std::make_shared<T>();
		}
		return m_cont[i * m_columns + j];
	}
	throw std::out_of_range("Index out of range");
}
template<class T>
void Box<T>::AddRow() {
	int cnt = 0;
	if (m_cont.size() + m_columns > m_cont.capacity())
		m_cont.reserve(m_columns);
	int size = m_cont.size();
	for (int i = size; i < (size + m_columns); i++) {
		m_cont.push_back(std::shared_ptr<T>());
		cnt++;
	}
	m_rows++;
}
template<class T>
void Box<T>::AddColumn() {
	int cnt = 0;
	if (m_cont.size() + m_rows > m_cont.capacity())
		m_cont.reserve(m_rows);
	for (int i = m_rows; i > 0; i--) {
		m_cont.insert(m_cont.begin() + i * m_columns, std::shared_ptr<T>());
		cnt++;
	}
	m_columns++;
}
template<class T>
void Box<T>::DeleteRow(int row) {
	if (row < 0 || row >= m_rows)
		return;
	for (int i = (row + 1) * m_columns; i > row * m_columns; i--) {
		m_cont.erase(m_cont.begin() + i - 1);
	}
	m_rows--;
}
template<class T>
void Box<T>::DeleteColumn(int column) {
	if (column < 0 || column >= m_columns)
		return;
	for (int i = m_rows - 1; i >= 0; i--) {
		m_cont.erase(m_cont.begin() + i * m_columns + column);
	}
	m_columns--;
}
template<class T>
int Box<T>::GetRowCount() const {
	return m_rows;
}
template<class T>
int Box<T>::GetColumnCount() const {
	return m_columns;
}
template<class T>
void Box<T>::ResizeRows(int newRows) {
	int n = m_rows;
	if (newRows > m_rows) {
		for (int i = 0; i < newRows - n; i++)
			AddRow();
	}
	else if (newRows < m_rows) {
		for (int i = 0; i < n - newRows; i++)
			DeleteRow(GetRowCount() - 1);
	}
}
template<class T>
void Box<T>::ResizeColumns(int newColumns) {
	int n = m_columns;
	if (newColumns > m_columns) {
		for (int i = 0; i < newColumns - n; i++)
			AddColumn();
	}
	else if (newColumns < m_columns) {
		for (int i = 0; i < n - newColumns; i++)
			DeleteColumn(GetColumnCount() - 1);
	}
}