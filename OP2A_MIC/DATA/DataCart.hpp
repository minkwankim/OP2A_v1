/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * DataCart.hpp
 *
 *  Created on: 15 Jun 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef DATA_DATACART_HPP_
#define DATA_DATACART_HPP_

#include <vector>
#include <cmath>
#include "../COMM/MultiVector.hpp"
#include "../COMM/assert_mk.hpp"



template <class T>
class DataCart {
public:
	DataCart();
	~DataCart();

protected:
	std::vector<T*> m_data;                                        // Main data
	std::vector<T*> m_ghost_data;								   // Ghost data
	std::vector< std::vector<std::vector<int> > > m_indexMain;     // Position in m_data

	std::vector< std::vector<std::vector<int> > > m_indexAncestor;  // Ancestor ID
	std::vector< std::vector<std::vector<int> > > m_indexDescendant; // Position in m_data [Ancestor ID][lvl][subIndex]
	std::vector<std::vector<int> > m_indexSubSample;				// Sample size of family history tree

	unsigned int m_Max_lvl;											// Maximum number of Categories
	unsigned int m_Refine_Number;									// [Default] 2D: 4 , 3D: 8

	bool m_hasGhostData;											// Ghost data index
	int  m_index_NotAssigned;
	int  m_index_GhostDataStart;

public:
	unsigned int Nx;
	unsigned int Ny;
	unsigned int Nz;

public:
	// Initialisation Functions
	void initialize(int nx, int ny);
	void initialize(int nx, int ny, int nz);
	void initialize(int nx, int ny, bool includeGhostCell);
	void initialize(int nx, int ny, int nz, bool includeGhostCell);
	void initializeDescendant(int MaxLvl, int refineNum);

	// Setting Ghost data treatment
	void settingGhostDataStart(int indexghost);
	void settingNotAssignedData(int indexNotAssigned);


	// Calling functions
	T* operator()(const unsigned int i, const unsigned int j);
	T* operator()(const unsigned int i, const unsigned int j, const unsigned int k);
	T* operator()(const unsigned int i, const unsigned int j, const unsigned int lvl, const unsigned int subindex);
	T* operator()(const unsigned int i, const unsigned int j, const unsigned int k, const unsigned int lvl, const unsigned int subindex);

	// Data Add function
    void add(const unsigned int i, const unsigned int j, const unsigned int k, T* data, bool isMain);
    void add(const unsigned int i, const unsigned int j, T* data, bool isMain);
    void add(const unsigned int i, const unsigned int j, const unsigned int k, const unsigned int lvl, const unsigned int subindex, T* data, bool isMain);
    void add(const unsigned int i, const unsigned int j, const unsigned int lvl, const unsigned int subindex, T* data, bool isMain);

	void add(const unsigned int i, const unsigned int j, T* data);
	void add(const unsigned int i, const unsigned int j, const unsigned int k, T* data);
    void add(const unsigned int i, const unsigned int j, const unsigned int k, const unsigned int lvl, const unsigned int subindex, T* data);
    void add(const unsigned int i, const unsigned int j, const unsigned int lvl, const unsigned int subindex, T* data);

    // Set Descendant Data
    void hasDescendant(const unsigned int i, const unsigned int j, const unsigned int k);
    void hasDescendant(const unsigned int i, const unsigned int j);

    // Data Access
    T* dataAccess(unsigned int i);
    int dataSize();

    T* dataAccessGhost(unsigned int i);
    int dataSizeGhost();
};



// Constructor and Destructors
template <class T>
DataCart<T>::DataCart()
    :Nx(0), Ny(0), Nz(0), m_Max_lvl(0), m_Refine_Number(0), m_hasGhostData(false), m_index_NotAssigned(-1), m_index_GhostDataStart(-100)
{

}

template <class T>
DataCart<T>::~DataCart() {
	// TODO Auto-generated destructor stub
}



// Initialisation function for Ancestor DATA
template <class T>
void DataCart<T>::initialize(int nx, int ny)
{
	Nx = nx;
	Ny = ny;
	Nz = 0;

	m_indexMain     = Common::VectorMulti<int>::create(Nx+1, Ny+1, Nz+1);
	m_indexAncestor = Common::VectorMulti<int>::create(Nx+1, Ny+1, Nz+1);

	for (int itr_i = 0; itr_i <= Nx; itr_i++)
		for (int itr_j = 0; itr_j <= Ny; itr_j++)
			for (int itr_k = 0; itr_k <= Nz; itr_k++)
					m_indexMain[itr_i][itr_j][itr_k] = m_index_NotAssigned;

	for (int itr_i = 0; itr_i <= Nx; itr_i++)
		for (int itr_j = 0; itr_j <= Ny; itr_j++)
			for (int itr_k = 0; itr_k <= Nz; itr_k++)
				    m_indexAncestor[itr_i][itr_j][itr_k] = m_index_NotAssigned;

	m_data.reserve((Nx+1)*(Ny+1)*(Nz+1));
}

template <class T>
void DataCart<T>::initialize(int nx, int ny, int nz)
{
	Nx = nx;
	Ny = ny;
	Nz = nz;

	m_indexMain     = Common::VectorMulti<int>::create(Nx+1, Ny+1, Nz+1);
	m_indexAncestor = Common::VectorMulti<int>::create(Nx+1, Ny+1, Nz+1);

	for (int itr_i = 0; itr_i <= Nx; itr_i++)
		for (int itr_j = 0; itr_j <= Ny; itr_j++)
			for (int itr_k = 0; itr_k <= Nz; itr_k++)
					m_indexMain[itr_i][itr_j][itr_k] = m_index_NotAssigned;

	for (int itr_i = 0; itr_i <= Nx; itr_i++)
		for (int itr_j = 0; itr_j <= Ny; itr_j++)
			for (int itr_k = 0; itr_k <= Nz; itr_k++)
				    m_indexAncestor[itr_i][itr_j][itr_k] = m_index_NotAssigned;

	m_data.reserve((Nx+1)*(Ny+1)*(Nz+1));
}

template <class T>
void DataCart<T>::initialize(int nx, int ny, int nz, bool includeGhostCell)
{
	Nx = nx;
	Ny = ny;
	Nz = nz;

	if (includeGhostCell == true)
	{

		m_indexMain     = Common::VectorMulti<int>::create(Nx+2, Ny+2, Nz+2);
		m_indexAncestor = Common::VectorMulti<int>::create(Nx+2, Ny+2, Nz+2);

		for (int itr_i = 0; itr_i <= Nx+1; itr_i++)
			for (int itr_j = 0; itr_j <= Ny+1; itr_j++)
				for (int itr_k = 0; itr_k <= Nz+1; itr_k++)
						m_indexMain[itr_i][itr_j][itr_k] = m_index_NotAssigned;

		for (int itr_i = 0; itr_i <= Nx+1; itr_i++)
			for (int itr_j = 0; itr_j <= Ny+1; itr_j++)
				for (int itr_k = 0; itr_k <= Nz+1; itr_k++)
						m_indexAncestor[itr_i][itr_j][itr_k] = m_index_NotAssigned;

		m_data.reserve((Nx+1)*(Ny+1)*(Nz+1));

		int aux1;
		if (Nz != 0) aux1 = 2*(Nx*Ny + Ny*Nz + Nz*Nx);
		else         aux1 = 2*(Nx + Ny);
		m_ghost_data.reserve(aux1);
	}
	else
	{
		m_indexMain     = Common::VectorMulti<int>::create(Nx+1, Ny+1, Nz+1);
		m_indexAncestor = Common::VectorMulti<int>::create(Nx+1, Ny+1, Nz+1);

		for (int itr_i = 0; itr_i <= Nx; itr_i++)
			for (int itr_j = 0; itr_j <= Ny; itr_j++)
				for (int itr_k = 0; itr_k <= Nz; itr_k++)
						m_indexMain[itr_i][itr_j][itr_k] = m_index_NotAssigned;

		for (int itr_i = 0; itr_i <= Nx; itr_i++)
			for (int itr_j = 0; itr_j <= Ny; itr_j++)
				for (int itr_k = 0; itr_k <= Nz; itr_k++)
					    m_indexAncestor[itr_i][itr_j][itr_k] = m_index_NotAssigned;

		m_data.reserve((Nx+1)*(Ny+1)*(Nz+1));
	}
}

template <class T>
void DataCart<T>::initialize(int nx, int ny, bool includeGhostCell)
{
	initialize(nx, ny, 0, includeGhostCell);
}

template <class T>
void DataCart<T>::initializeDescendant(int MaxLvl, int refineNum)
{
	m_Max_lvl       = MaxLvl;
	m_Refine_Number = refineNum;

	m_indexSubSample.resize(m_Max_lvl+1);
	for (int itr_l = 0; itr_l <= m_Max_lvl; itr_l++) m_indexSubSample[itr_l].resize(pow(refineNum, itr_l), m_index_NotAssigned);
}


// Setting Ghost data treatment
template <class T>
void DataCart<T>::settingGhostDataStart(int indexghost)
{
	op_assert(indexghost < 0 && indexghost != m_index_NotAssigned);
	m_index_GhostDataStart = indexghost;
}

template <class T>
void DataCart<T>::settingNotAssignedData(int indexNotAssigned)
{
	op_assert(indexNotAssigned < 0);
	m_index_NotAssigned = indexNotAssigned;
}


// Calling functions
template <class T>
T* DataCart<T>::operator()(const unsigned int i, const unsigned int j)
{
	operator()(i, j, 0);
}

template <class T>
T* DataCart<T>::operator()(const unsigned int i, const unsigned int j, const unsigned int k)
{
	if (m_hasGhostData == false)
	{
		op_assert(i >= 0 && i <= Nx);
		op_assert(j >= 0 && j <= Ny);
		op_assert(k >= 0 && k <= Nz);
	}
	else
	{
		op_assert(i >= 0 && i <= Nx+1);
		op_assert(j >= 0 && j <= Ny+1);
		op_assert(k >= 0 && k <= Nz+1);
	}

	int indexPtr = m_indexMain[i][j][k];

	if (indexPtr >= 0)                                             // Main Data
	{
		return (m_data[indexPtr]);
	}
	else if(indexPtr == m_index_NotAssigned)                     // Not assigned DATA
	{
		return (NULL);
	}
	else if (indexPtr <= m_index_GhostDataStart)                 // Ghost Data
	{
		indexPtr = -(indexPtr - m_index_GhostDataStart);
		return (m_ghost_data[indexPtr]);
	}
	else
	{
		return (NULL);
	}
}

template <class T>
T* DataCart<T>::operator()(const unsigned int i, const unsigned int j, const unsigned int lvl, const unsigned int subindex)
{
	operator()(i, j, 0, lvl, subindex);
}

template <class T>
T* DataCart<T>::operator()(const unsigned int i, const unsigned int j, const unsigned int k, const unsigned int lvl, const unsigned int subindex)
{
	int indexPtr = -1;

	if (lvl != 0)
	{
		int id_Ancestor = m_indexAncestor[i][j][k];
		if (id_Ancestor != -1 && id_Ancestor < m_indexDescendant.size()) indexPtr = m_indexDescendant[id_Ancestor][lvl][subindex];
	}
	else
	{
		indexPtr = m_indexMain[i][j][k];
	}


	if (indexPtr >= 0)                                             // Main Data
	{
		return (m_data[indexPtr]);
	}
	else if(indexPtr == m_index_NotAssigned)                     // Not assigned DATA
	{
		return (NULL);
	}
	else if (indexPtr <= m_index_GhostDataStart)                 // Ghost Data
	{
		indexPtr = -(indexPtr - m_index_GhostDataStart);
		return (m_ghost_data[indexPtr]);
	}
	else
	{
		return (NULL);
	}
}


// Data Add function
template <class T>
void DataCart<T>::add(const unsigned int i, const unsigned int j, const unsigned int k, T* data, bool isMain)
{
	int indexPtr = m_indexMain[i][j][k];

	if (indexPtr == m_index_NotAssigned)
	{
		if(isMain == true)
		{
			indexPtr = m_data.size();
			m_indexMain[i][j][k] = indexPtr;
			m_data.push_back(data);
		}
		else
		{
			indexPtr = m_ghost_data.size();
			m_indexMain[i][j][k] = m_index_GhostDataStart - indexPtr;
			m_ghost_data.push_back(data);
		}
	}
	else if(indexPtr >= 0)
	{
		m_data[indexPtr] = data;
	}
	else if(indexPtr <= m_index_GhostDataStart)                 // Ghost Data
	{
		indexPtr = -(indexPtr - m_index_GhostDataStart);
		m_ghost_data[indexPtr] = data;
	}
}

template <class T>
void DataCart<T>::add(const unsigned int i, const unsigned int j, T* data, bool isMain)
{
	add(i, j, 0, data, isMain);
}

template <class T>
void DataCart<T>::add(const unsigned int i, const unsigned int j, const unsigned int k, const unsigned int lvl, const unsigned int subindex, T* data, bool isMain)
{
	if(lvl == 0)
	{
		add(i, j, k, data, isMain);
	}
	else
	{
		int indexPtr    = -1;
		int id_Ancestor = m_indexAncestor[i][j][k];

		if (id_Ancestor == m_index_NotAssigned)
		{
			id_Ancestor = m_indexDescendant.size();
			m_indexAncestor[i][j][k] = id_Ancestor;
			m_indexDescendant.push_back(m_indexSubSample);

			if (isMain == true)
			{
				indexPtr = m_data.size();
				m_indexDescendant[id_Ancestor][lvl][subindex] = indexPtr;
				m_data.push_back(data);
			}
			else
			{
				indexPtr = m_ghost_data.size();
				m_indexDescendant[id_Ancestor][lvl][subindex] = m_index_GhostDataStart - indexPtr;
				m_ghost_data.push_back(data);
			}
		}
		else
		{
			indexPtr = m_indexDescendant[id_Ancestor][lvl][subindex];
			if (indexPtr == m_index_NotAssigned)
			{
				if (isMain == true)
				{
					indexPtr = m_data.size();
					m_indexDescendant[id_Ancestor][lvl][subindex] = indexPtr;
					m_data.push_back(data);
				}
				else
				{
					indexPtr = m_ghost_data.size();
					m_indexDescendant[id_Ancestor][lvl][subindex] = m_index_GhostDataStart - indexPtr;
					m_ghost_data.push_back(data);
				}
			}
			else if(indexPtr >= 0)
			{
				m_data[indexPtr] = data;
			}
			else if(indexPtr <= m_index_GhostDataStart)
			{
				indexPtr = -(indexPtr - m_index_GhostDataStart);
				m_ghost_data[indexPtr] = data;
			}
		}
	}
}

template <class T>
void DataCart<T>::add(const unsigned int i, const unsigned int j, const unsigned int lvl, const unsigned int subindex, T* data, bool isMain)
{
	add(i, j, 0, lvl, subindex, data, isMain);
}

template <class T>
void DataCart<T>::add(const unsigned int i, const unsigned int j, const unsigned int k, T* data)
{
	int indexPtr = m_indexMain[i][j][k];


	if (indexPtr == m_index_NotAssigned)
	{
		indexPtr = m_data.size();
		m_indexMain[i][j][k] = indexPtr;
		m_data.push_back(data);
	}
	else if(indexPtr >= 0)
	{
		m_data[indexPtr] = data;
	}
	else if(indexPtr <= m_index_GhostDataStart)                 // Ghost Data
	{
		indexPtr = -(indexPtr - m_index_GhostDataStart);
		m_ghost_data[indexPtr] = data;
	}
}

template <class T>
void DataCart<T>::add(const unsigned int i, const unsigned int j, T* data)
{
	add(i, j, 0, data);
}

template <class T>
void DataCart<T>::add(const unsigned int i, const unsigned int j, const unsigned int k, const unsigned int lvl, const unsigned int subindex, T* data)
{
	if(lvl == 0)
	{
		add(i, j, k, data);
	}
	else
	{
		int indexPtr    = -1;
		int id_Ancestor = m_indexAncestor[i][j][k];

		if (id_Ancestor == -1)
		{
			id_Ancestor = m_indexDescendant.size();
			m_indexAncestor[i][j][k] = id_Ancestor;
			m_indexDescendant.push_back(m_indexSubSample);

			indexPtr = m_data.size();
			m_indexDescendant[id_Ancestor][lvl][subindex] = indexPtr;
			m_data.push_back(data);
		}
		else
		{
			indexPtr = m_indexDescendant[id_Ancestor][lvl][subindex];
			if (indexPtr == -1)
			{
				indexPtr = m_data.size();
				m_indexDescendant[id_Ancestor][lvl][subindex] = indexPtr;
				m_data.push_back(data);
			}
			else
			{
				m_data[indexPtr] = data;
			}
		}
	}
}

template <class T>
void DataCart<T>::add(const unsigned int i, const unsigned int j, const unsigned int lvl, const unsigned int subindex, T* data)
{
	add(i, j, 0, lvl, subindex, data);
}



// Set Descendant data
template <class T>
void DataCart<T>::hasDescendant(const unsigned int i, const unsigned int j, const unsigned int k)
{
	int indexAncestor = -1;

	indexAncestor = m_indexAncestor[i][j][k];

	if (indexAncestor == -1)
	{
		indexAncestor = m_indexDescendant.size();
		m_indexAncestor[i][j][k] = indexAncestor;
		m_indexDescendant.push_back(m_indexSubSample);
	}
}

template <class T>
void DataCart<T>::hasDescendant(const unsigned int i, const unsigned int j)
{
	hasDescendant(i, j, 0);
}



// Data Access
template <class T>
T* DataCart<T>::dataAccess(unsigned int i)
{
	return (m_data[i]);
}

template <class T>
int DataCart<T>::dataSize()
{
	return (m_data.size());
}

template <class T>
T* DataCart<T>::dataAccessGhost(unsigned int i)
{
	return (m_ghost_data[i]);
}

template <class T>
int DataCart<T>::dataSizeGhost()
{
	return (m_ghost_data.size());
}

#endif /* DATA_DATACART_HPP_ */
