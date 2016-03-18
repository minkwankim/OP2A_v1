/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * readdatafromstring.hpp
 *
 *  Created on: 18 Mar 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef COMM_READDATAFROMSTRING_HPP_
#define COMM_READDATAFROMSTRING_HPP_

#include <string>
#include "noninstantiable.hpp"

namespace Common {

class read_data_from_string: public NonInstantiable<read_data_from_string>{
public:
	static void remove_space_front(std::string& line);
	static void remove_space_end(std::string& line);
	static void remove_comments(std::string& line);

	static std::string read_string(std::string& line, std::string read_format);



};

} /* namespace Common */

#endif /* COMM_READDATAFROMSTRING_HPP_ */
