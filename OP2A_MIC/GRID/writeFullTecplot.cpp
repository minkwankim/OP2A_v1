/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * writeFullTecplot.cpp
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

void writeFulltecplotCellCenter(const  std::string& title, Grid& grid, std::vector< std::vector<double> >& data, std::vector<std::string>& variables)
{
	std::string fileName;
	fileName = title + ".plt";

	std::ofstream grid_tecplot;
	grid_tecplot.open(fileName.c_str());


	int numVar = variables.size();
	if (data.size()-1 != grid.NCM) Common::ExceptionError(FromHere(), "Number of Data in the given data set does not match with GridData", Common::ErrorCodes::Mismatch());
	if (data[1].size() != numVar)  Common::ExceptionError(FromHere(), "Number of variables in the given data set does not match with Variable Name Data", Common::ErrorCodes::Mismatch());


	// Header
	grid_tecplot << "TITLE = Simulation Result of \"" << title << "\"" << std::endl;
	if (grid.NDIM == 2)	     grid_tecplot << std::scientific << std::setprecision(16) << "VARIABLES = \"X\", \"Y\" ";
	else if (grid.NDIM == 3) grid_tecplot << std::scientific << std::setprecision(16) << "VARIABLES = \"X\", \"Y\", \"Z\" ";
	for (int v = 0; v < numVar; v++) grid_tecplot << ", \"" << variables[v] << "\"";
	grid_tecplot << std::endl;

	// Zone Info
	grid_tecplot << "ZONE" << std::endl;
	grid_tecplot << "T=\"" << title <<"\"" << std::endl;
	grid_tecplot << "DATAPACKING= BLOCK" << std::endl;
	grid_tecplot << "NODES="    << grid.NNM << std::endl;
	grid_tecplot << "ELEMENTS=" << grid.NCM << std::endl;

	if (grid.NDIM == 2)	     grid_tecplot << "ZONETYPE = FEQUADRILATERAL" << std::endl;
	else if (grid.NDIM == 3) grid_tecplot << "ZONETYPE = FEBRICK" << std::endl;

	if (numVar == 1) grid_tecplot << "VARLOCATION = ([3]=CELLCENTERED)" << std::endl;
	else             grid_tecplot << "VARLOCATION = ([3-" << numVar+2 <<"]=CELLCENTERED)" << std::endl;

	for (int d = 0; d <= grid.NDIM-1; d++)
	{
		for (int n = 1; n <= grid.NNM; n++)
		{
			grid_tecplot << grid.NODE(n).x[d] << std::endl;
		}

		grid_tecplot << std::endl;
	}

	for (int v = 0; v <= numVar-1; v++)
	{
		for (int cID = 1; cID <= grid.NCM; cID++)
		{
			grid_tecplot << data[cID][v] << std::endl;
		}

		grid_tecplot << std::endl;
	}


	for (int c = 1; c <= grid.NCM; c++)
	{
		switch(grid.CELL(c).type)
		{
		case TRI3:
			grid_tecplot << grid.CELL(c).nodes[0] << " ";
			grid_tecplot << grid.CELL(c).nodes[1] << " ";
			grid_tecplot << grid.CELL(c).nodes[2] << " ";
			grid_tecplot << grid.CELL(c).nodes[2] << std::endl;
			break;

		case QUAD4:
			grid_tecplot << grid.CELL(c).nodes[0] << " ";
			grid_tecplot << grid.CELL(c).nodes[1] << " ";
			grid_tecplot << grid.CELL(c).nodes[2] << " ";
			grid_tecplot << grid.CELL(c).nodes[3] << std::endl;
			break;

		case TETRA4:
			grid_tecplot << grid.CELL(c).nodes[0] << " ";
			grid_tecplot << grid.CELL(c).nodes[1] << " ";
			grid_tecplot << grid.CELL(c).nodes[2] << " ";
			grid_tecplot << grid.CELL(c).nodes[2] << " ";
			grid_tecplot << grid.CELL(c).nodes[3] << " ";
			grid_tecplot << grid.CELL(c).nodes[3] << " ";
			grid_tecplot << grid.CELL(c).nodes[3] << " ";
			grid_tecplot << grid.CELL(c).nodes[3] << std::endl;
			break;

		case HEXA8:
			grid_tecplot << grid.CELL(c).nodes[0] << " ";
			grid_tecplot << grid.CELL(c).nodes[1] << " ";
			grid_tecplot << grid.CELL(c).nodes[2] << " ";
			grid_tecplot << grid.CELL(c).nodes[3] << " ";
			grid_tecplot << grid.CELL(c).nodes[4] << " ";
			grid_tecplot << grid.CELL(c).nodes[5] << " ";
			grid_tecplot << grid.CELL(c).nodes[6] << " ";
			grid_tecplot << grid.CELL(c).nodes[7] << std::endl;
			break;

		case PRISM6:
			grid_tecplot << grid.CELL(c).nodes[0] << " ";
			grid_tecplot << grid.CELL(c).nodes[1] << " ";
			grid_tecplot << grid.CELL(c).nodes[2] << " ";
			grid_tecplot << grid.CELL(c).nodes[2] << " ";
			grid_tecplot << grid.CELL(c).nodes[3] << " ";
			grid_tecplot << grid.CELL(c).nodes[4] << " ";
			grid_tecplot << grid.CELL(c).nodes[5] << " ";
			grid_tecplot << grid.CELL(c).nodes[5] << std::endl;
			break;

		case PYRAMID5:
			grid_tecplot << grid.CELL(c).nodes[0] << " ";
			grid_tecplot << grid.CELL(c).nodes[1] << " ";
			grid_tecplot << grid.CELL(c).nodes[2] << " ";
			grid_tecplot << grid.CELL(c).nodes[3] << " ";
			grid_tecplot << grid.CELL(c).nodes[4] << " ";
			grid_tecplot << grid.CELL(c).nodes[4] << " ";
			grid_tecplot << grid.CELL(c).nodes[4] << " ";
			grid_tecplot << grid.CELL(c).nodes[4] << std::endl;
			break;
		}
	}

	grid_tecplot.close();
}



}

