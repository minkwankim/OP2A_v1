/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * ProblemCommon.hpp
 *
 *  Created on: 21 Mar 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef PROB_PROBLEMCOMMON_HPP_
#define PROB_PROBLEMCOMMON_HPP_



#define TITLE	        "Problem name:"
#define GRID_FILENAME	"Grid file name:"
#define OUT_FILENAME	"Result file name:"



#include "../COMM/common.hpp"
#include "../COMM/processorinfo.hpp"

class ProblemCommon {
public:
	ProblemCommon();
	~ProblemCommon();


public:
	std::string title;            // Problem name
	std::string filename_grid;    // Grid/Mesh file name
	std::string filename_output;  // Output data file name

	Common::processor_info_MIC    proc_info;        // Processor information


	void read(std::string filename);


};

#endif /* PROB_PROBLEMCOMMON_HPP_ */
