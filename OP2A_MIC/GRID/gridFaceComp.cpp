/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * gridFaceComp.cpp
 *
 *  Created on: 18 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */


#include "grid.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"


namespace GRID {


int faceComp(std::vector<int>& F1_list, std::vector<int>& F2_list, int type)
{
	int comp = 0;


	if (type == LINE)
	{
		if (F1_list[0] == F2_list[0] && F1_list[1] == F2_list[1])	comp = 1;
		if (F1_list[0] == F2_list[1] && F1_list[1] == F2_list[0])	comp = -1;
	}
	else if (type == TRI3)
	{
		if (F1_list[0] == F2_list[0])
		{
			if (F1_list[1] == F2_list[1] && F1_list[2] == F2_list[2])	comp = 1;
			if (F1_list[1] == F2_list[2] && F1_list[2] == F2_list[1])	comp = -1;
		}
		else if (F1_list[0] == F2_list[1])
		{
			if (F1_list[1] == F2_list[2] && F1_list[2] == F2_list[0])	comp = 1;
			if (F1_list[1] == F2_list[0] && F1_list[2] == F2_list[2])	comp = -1;
		}
		else if (F1_list[0] == F2_list[2])
		{
			if (F1_list[1] == F2_list[0] && F1_list[2] == F2_list[1])	comp = 1;
			if (F1_list[1] == F2_list[1] && F1_list[2] == F2_list[0])	comp = -1;
		}
	}
	else if (type == QUAD4)
	{
		if (F1_list[0] == F2_list[0])
		{
			if (F1_list[1] == F2_list[1] && F1_list[2] == F2_list[2] &&  F1_list[3] == F2_list[3])	comp = 1;
			if (F1_list[1] == F2_list[3] && F1_list[2] == F2_list[3] &&  F1_list[3] == F2_list[1])	comp = -1;
		}
		else if (F1_list[0] == F2_list[1])
		{
			if (F1_list[1] == F2_list[2] && F1_list[2] == F2_list[3] &&  F1_list[3] == F2_list[0])	comp = 1;
			if (F1_list[1] == F2_list[0] && F1_list[2] == F2_list[3] &&  F1_list[3] == F2_list[2])	comp = -1;
		}
		else if (F1_list[0] == F2_list[2])
		{
			if (F1_list[1] == F2_list[3] && F1_list[2] == F2_list[0] &&  F1_list[3] == F2_list[1])	comp = 1;
			if (F1_list[1] == F2_list[1] && F1_list[2] == F2_list[0] &&  F1_list[3] == F2_list[3])	comp = -1;
		}
		else if (F1_list[0] == F2_list[3])
		{
			if (F1_list[1] == F2_list[0] && F1_list[2] == F2_list[1] &&  F1_list[3] == F2_list[1])	comp = 1;
			if (F1_list[1] == F2_list[2] && F1_list[2] == F2_list[1] &&  F1_list[3] == F2_list[0])	comp = -1;
		}
	}


	return (comp);
}





}

