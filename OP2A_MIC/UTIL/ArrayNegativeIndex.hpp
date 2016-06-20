/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * ArrayNegativeIndex.hpp
 *
 *  Created on: 12 May 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef UTIL_ARRAYNEGATIVEINDEX_HPP_
#define UTIL_ARRAYNEGATIVEINDEX_HPP_


#include <stdio.h>
#include <omp.h>
#include <mpi.h>

#include "../COMM/assert_mk.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"

template <class T>
class ArrayNegativeIndex {
public:
	ArrayNegativeIndex();
	ArrayNegativeIndex(int startIndex, int endIndex);
	ArrayNegativeIndex(int startIndex, int endIndex, T value);

	~ArrayNegativeIndex();

public:
	T& operator[] (const int i);

	void resizeMaxIndex(int a);
	void resizeMinIndex(int a);
	int indexMax();
	int indexMin();

	T* data();

private:
	std::vector<T> m_data;
	int m_startIndex;
	int m_endIndex;
};

template <class T>
ArrayNegativeIndex<T>::ArrayNegativeIndex()
: m_startIndex(0) , m_endIndex(0)
{

}

template <class T>
ArrayNegativeIndex<T>::ArrayNegativeIndex(int startIndex, int endIndex)
:m_startIndex(startIndex), m_endIndex(endIndex)
{
	op_assert(endIndex >= startIndex);
	int dataSize = endIndex - startIndex;
	m_data.resize(dataSize+1, 0);
}

template <class T>
ArrayNegativeIndex<T>::ArrayNegativeIndex(int startIndex, int endIndex, T value)
:m_startIndex(startIndex), m_endIndex(endIndex)
{
	op_assert(endIndex >= startIndex);

	int dataSize = endIndex - startIndex;
	m_data.resize(dataSize+1, value);

	//for (int iIndex = 0; iIndex <= dataSize; iIndex++) m_data[iIndex] = value;
}


template <class T>
ArrayNegativeIndex<T>::~ArrayNegativeIndex()
{
	// TODO Auto-generated destructor stub
}


template <class T>
T& ArrayNegativeIndex<T>::operator[] (const int i)
{
	op_assert(i >= m_startIndex);
	op_assert(i <= m_endIndex);

	int index = i - m_startIndex;
	index = fabs(index);

	return (m_data[index]);
}


template <class T>
int ArrayNegativeIndex<T>::indexMax()
{
	return(m_endIndex);
}

template <class T>
int ArrayNegativeIndex<T>::indexMin()
{
	return(m_startIndex);
}

template <class T>
void ArrayNegativeIndex<T>::resizeMaxIndex(int a)
{
	int dataSize;

	if  (a > m_endIndex)
	{
		m_endIndex = a;
		dataSize = m_endIndex - m_startIndex;
		m_data.resize(fabs(dataSize)+1, 0);
	}
	else if (a < m_endIndex)
	{
		int temp = a - m_endIndex;

		m_endIndex = a;
		m_data.erase(m_data.end()+temp, m_data.end());
	}
}

template <class T>
void ArrayNegativeIndex<T>::resizeMinIndex(int a)
{
	int dataSize;
	int temp;

	if  (a < m_startIndex)
	{
		temp = m_startIndex - a;

		m_startIndex = a;
		m_data.insert(m_data.begin(), temp, 0);
	}
	else if (a > m_startIndex)
	{
		int temp = a - m_startIndex;

		m_startIndex = a;
		m_data.erase(m_data.begin(), m_data.begin()+temp);
	}
}


template <class T>
T* ArrayNegativeIndex<T>::data()
{
	return(m_data.data());
}


#endif /* UTIL_ARRAYNEGATIVEINDEX_HPP_ */
