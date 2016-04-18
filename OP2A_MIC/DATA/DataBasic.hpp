/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * DataBasic.hpp
 *
 *  Created on: 21 Mar 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef DATA_DATABASIC_HPP_
#define DATA_DATABASIC_HPP_


#include "../COMM/map1D.hpp"


template <typename TYPE>
class DataBasic {
public:
	DataBasic();
	~DataBasic();




private:
	Common::Map1D<std::string, TYPE>	m_map;
	std::vector<TYPE> m_data;
};

#endif /* DATA_DATABASIC_HPP_ */
