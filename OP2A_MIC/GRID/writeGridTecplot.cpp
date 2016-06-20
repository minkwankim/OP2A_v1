/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * writeGridTecplot.cpp
 *
 *  Created on: 21 Apr 2016
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
#include "GridCart.hpp"

#include "../COMM/readdatafromstring.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"
#include "../COMM/StringOps.hpp"
#include "../COMM/VectorCompare.hpp"
#include "../COMM/fileHandle.hpp"


namespace GRID {



void writeGridtecplot(const  std::string& mesh_file_name)
{
	Grid grid;
	readGrid(mesh_file_name, grid);


	std::string fileName;
	std::vector<std::string> fileName_temp;

	Common::StringOps::split(mesh_file_name, '.', fileName_temp);
	fileName = fileName_temp[0] + ".plt";

	std::ofstream grid_tecplot;
	grid_tecplot.open(fileName.c_str());



	grid_tecplot << "TITLE = \"" << fileName_temp[0] << "\"" << std::endl;
	grid_tecplot << "FILETYPE = GRID" << std::endl;


	if (grid.NDIM == 2)	     grid_tecplot << std::scientific << std::setprecision(16) << "VARIABLES = \"X\" \"Y\" " << std::endl;
	else if (grid.NDIM == 3) grid_tecplot << std::scientific << std::setprecision(16) << "VARIABLES = \"X\" \"Y\" \"Z\" " << std::endl;

	grid_tecplot << "ZONE" << std::endl;
	grid_tecplot << "T=\"" << fileName_temp[0] <<"\"" << std::endl;
	grid_tecplot << "DATAPACKING= POINT" << std::endl;
	grid_tecplot << "NODES="    << grid.NNM << std::endl;
	grid_tecplot << "ELEMENTS=" << grid.NCM << std::endl;

	if (grid.NDIM == 2)	     grid_tecplot << "ZONETYPE = FEQUADRILATERAL" << std::endl;
	else if (grid.NDIM == 3) grid_tecplot << "ZONETYPE = FEBRICK" << std::endl;

	for (int n = 1; n <= grid.NNM; n++)
	{
		for (int d = 0; d <= grid.NDIM-1; d++) grid_tecplot << grid.NODE(n).x[d] << " ";
		grid_tecplot << std::endl;
	}

	grid_tecplot << std::endl;
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


void writeGridtecplot(const  std::string& title, Grid& grid)
{
	std::string fileName;
	fileName = title + ".plt";

	std::ofstream grid_tecplot;
	grid_tecplot.open(fileName.c_str());



	grid_tecplot << "TITLE = \"" << title << "\"" << std::endl;
	grid_tecplot << "FILETYPE = GRID" << std::endl;


	if (grid.NDIM == 2)	     grid_tecplot << std::scientific << std::setprecision(16) << "VARIABLES = \"X\" \"Y\" " << std::endl;
	else if (grid.NDIM == 3) grid_tecplot << std::scientific << std::setprecision(16) << "VARIABLES = \"X\" \"Y\" \"Z\" " << std::endl;

	grid_tecplot << "ZONE" << std::endl;
	grid_tecplot << "T=\"" << title <<"\"" << std::endl;
	grid_tecplot << "DATAPACKING= POINT" << std::endl;
	grid_tecplot << "NODES="    << grid.NNM << std::endl;
	grid_tecplot << "ELEMENTS=" << grid.NCM << std::endl;

	if (grid.NDIM == 2)	     grid_tecplot << "ZONETYPE = FEQUADRILATERAL" << std::endl;
	else if (grid.NDIM == 3) grid_tecplot << "ZONETYPE = FEBRICK" << std::endl;

	for (int n = 1; n <= grid.NNM; n++)
	{
		for (int d = 0; d <= grid.NDIM-1; d++) grid_tecplot << grid.NODE(n).x[d] << " ";
		grid_tecplot << std::endl;
	}

	grid_tecplot << std::endl;
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




void writeGridCartTecplot(const  std::string& title, CartCell& cells)
{
	std::string fileName;
	fileName = title + ".plt";


	std::ofstream tecplot_header;      tecplot_header.open("_header.tmp");
	std::ofstream tecplot_node_data;   tecplot_node_data.open("_node_data.tmp");
	std::ofstream tecplot_cell_conn;   tecplot_cell_conn.open("_cell_conn.tmp");


	int ncm = 0;
	int nnm = 0;


	for (int c = 0; c < cells.dataSize(); c++)
	{
		if (cells.dataAccess(c)->type >= 0)
		{
			ncm++;
			vector2D nodes = cells.dataAccess(c)->nodePos();

			for (int n = 0; n < nodes.size(); n++)
			{
				tecplot_node_data << std::scientific << std::setprecision(16) << nodes[n][0];
				for (int k = 1; k < DIM; k++) tecplot_node_data << " " << std::scientific << std::setprecision(16) << nodes[n][k];
				tecplot_node_data << std::endl;
				nnm++;

				tecplot_cell_conn << nnm << " ";
			}
			tecplot_cell_conn << std::endl;
		}

	}
	tecplot_node_data << std::endl;


	tecplot_header << "TITLE = \"" << title << "\"" << std::endl;
	tecplot_header << "FILETYPE = GRID" << std::endl;

	if (DIM == 2)   tecplot_header << std::scientific << std::setprecision(16) << "VARIABLES = \"X\" \"Y\" " << std::endl;
	else            tecplot_header << std::scientific << std::setprecision(16) << "VARIABLES = \"X\" \"Y\" \"Z\" " << std::endl;

	tecplot_header << "ZONE" << std::endl;
	tecplot_header << "T=\"" << title <<"\"" << std::endl;
	tecplot_header << "DATAPACKING= POINT" << std::endl;
	tecplot_header << "NODES="    << nnm << std::endl;
	tecplot_header << "ELEMENTS=" << ncm << std::endl;

	if (DIM == 2)  tecplot_header << "ZONETYPE = FEQUADRILATERAL" << std::endl;
	else           tecplot_header << "ZONETYPE = FEBRICK" << std::endl;


	tecplot_header.close();
	tecplot_node_data.close();
	tecplot_cell_conn.close();

	Common::fileHandle::Xmerge("_header.tmp", "_node_data.tmp", "_cell_conn.tmp", fileName.c_str());
}


}


