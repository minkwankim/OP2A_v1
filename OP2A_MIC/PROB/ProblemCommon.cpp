/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * ProblemCommon.cpp
 *
 *  Created on: 21 Mar 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include "ProblemCommon.hpp"
#include "../COMM/error_exception.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/readdatafromstring.hpp"


ProblemCommon::ProblemCommon() {
	// TODO Auto-generated constructor stub

}

ProblemCommon::~ProblemCommon() {
	// TODO Auto-generated destructor stub
}


void ProblemCommon::read(std::string filename)
{
	std::string line;
	std::string temp_string;

	std::ifstream prob_file;
	prob_file.open(filename.c_str());

	if (prob_file.is_open())
	{
		while (! prob_file.eof())
		{
			getline(prob_file, line);

			temp_string = Common::read_data_from_string::read_string(line, std::string(TITLE));
			if (temp_string.size() > 0) title = temp_string;

			temp_string = Common::read_data_from_string::read_string(line, std::string(GRID_FILENAME));
			if (temp_string.size() > 0) filename_grid = temp_string;

			temp_string = Common::read_data_from_string::read_string(line, std::string(OUT_FILENAME));
			if (temp_string.size() > 0) filename_output = temp_string;

		}
	}
	else
	{
		Common::ExceptionError(FromHere(), "cannot find a problem setup data file.", Common::ErrorCodes::FileSystem());
	}

	prob_file.close();
}
