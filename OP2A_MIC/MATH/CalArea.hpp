/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * CalArea.hpp
 *
 *  Created on: 22 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef MATH_CALAREA_HPP_
#define MATH_CALAREA_HPP_

#include <vector>

#include "../COMM/common.hpp"
#include "../COMM/noninstantiable.hpp"




class CalArea:public Common::NonInstantiable<CalArea> {
public:
	static double line(std::vector<double>& n1, std::vector<double>& n2);
	static double triangle(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3);
	static double quadrilateral(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3, std::vector<double>& n4);
	static double tetrahedron(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3, std::vector<double>& n4);
	static double pyramid(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3, std::vector<double>& n4,  std::vector<double>& n5);
	static double prism(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3, std::vector<double>& n4, std::vector<double>& n5, std::vector<double>& n6);
	static double hexahedron(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3, std::vector<double>& n4, std::vector<double>& n5, std::vector<double>& n6, std::vector<double>& n7, std::vector<double>& n8);






};

#endif /* MATH_CALAREA_HPP_ */
