#pragma once

template<class T, int N>
class Array
{
private:
	T* m_array[N];
public:
	int m_size = N;
	void InitElement(T* element, int index);
	//void SetElement(T* element, int index);
	void DeleteElement(int index);
	T GetElement(int index);
};

template<class T, int N>
void Array<T, N>::InitElement(T* element, int index) {
	m_array[index] = element;
}
template<class T, int N>
T Array<T, N>::GetElement(int index) {
	return m_array[index];
}
template<class T, int N>
void Array<T, N>::DeleteElement(int index) {
	if (m_array[index] != NULL)
		delete m_array[index];
}