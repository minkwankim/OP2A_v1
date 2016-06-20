/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * writeDataTecplot.cpp
 *
 *  Created on: 10 May 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */




#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <iomanip>


#include "grid.hpp"
#include "../COMM/readdatafromstring.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"
#include "../COMM/StringOps.hpp"
#include "../COMM/VectorCompare.hpp"


namespace GRID {

void writeDatatecplot(const  std::string& fileName, Grid& grid, std::vector< std::vector<double> >& data, std::vector<std::string>& variables)
{
	std::ofstream grid_tecplot;
	grid_tecplot.open(fileName.c_str());

	int numVar = variables.size();

	if (data.size()-1 != grid.NCM)	Common::ExceptionError(FromHere(), "Number of Data in the given data set does not match with GridData", Common::ErrorCodes::Mismatch());
	if (data[1].size() != numVar)   Common::ExceptionError(FromHere(), "Number of variables in the given data set does not match with Variable Name Data", Common::ErrorCodes::Mismatch());


	grid_tecplot << "VARIABLES = \"" << variables[0] << "\"";
	for (int v = 1; v < numVar; v++) grid_tecplot << ", \"" << variables[v] << "\"";
	grid_tecplot << std::endl;

	grid_tecplot << "ZONE" << std::endl;
	grid_tecplot << "T=\" DATA \"" << std::endl;
	grid_tecplot << "DATAPACKING= BLOCK" << std::endl;
	grid_tecplot << "NODES="    << grid.NNM << std::endl;
	grid_tecplot << "ELEMENTS=" << grid.NCM << std::endl;

	if (grid.NDIM == 2)	     grid_tecplot << "ZONETYPE = FEQUADRILATERAL" << std::endl;
	else if (grid.NDIM == 3) grid_tecplot << "ZONETYPE = FEBRICK" << std::endl;

	grid_tecplot << "VARLOCATION = ([1-" << numVar << "]=CELLCENTERED)" << std::endl;

	grid_tecplot << "VARSHARELIST = ([1-" << numVar << "]=1)" << std::endl;
	grid_tecplot << "CONNECTIVITYSHAREZONE = 1" << std::endl;

	for (int v = 0; v <= numVar-1; v++)
	{
		for (int cID = 1; cID <= grid.whereisCell.size()-1; cID++)
		{
			if (grid.whereisCell[cID] >= 0 && grid.whereisCell[cID] <= grid.cells.size())
			{
				grid_tecplot << data[cID][v] << std::endl;
			}
		}
	}


	grid_tecplot.close();
}
}
