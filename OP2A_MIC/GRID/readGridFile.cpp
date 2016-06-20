/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * readGridFile.cpp
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
#include "../COMM/readdatafromstring.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"
#include "../COMM/StringOps.hpp"
#include "../COMM/VectorCompare.hpp"


namespace GRID {

void readGrid(const  std::string& mesh_file_name, Grid& grid)
{
	// 1. Open file to read
	std::ifstream mesh_file;
	mesh_file.open(mesh_file_name.c_str());
	if (!mesh_file.is_open()) Common::ExceptionError(FromHere(), "Cannot find OP2A grid file. Please check your grid file!", Common::ErrorCodes::FileSystem());

	grid.NDIM = -1;
	grid.NNM  = -1;
	grid.NFM  = -1;
	grid.NCM  = -1;
	grid.NGM  = -1;


	int tempInt;
	std::string line;


	// Over information read
	while (!mesh_file.eof())
	{
		getline(mesh_file, line);

		tempInt = -1;
		tempInt = Common::read_data_from_string::read_numeric<int>(line, "DIM= ");
		if (tempInt != -1)	grid.NDIM = tempInt;

		tempInt = -1;
		tempInt = Common::read_data_from_string::read_numeric<int>(line, "NNM= ");
		if (tempInt != -1)	grid.NNM = tempInt;

		tempInt = -1;
		tempInt = Common::read_data_from_string::read_numeric<int>(line, "NFM= ");
		if (tempInt != -1)	grid.NFM = tempInt;

		tempInt = -1;
		tempInt = Common::read_data_from_string::read_numeric<int>(line, "NCM= ");
		if (tempInt != -1)	grid.NCM = tempInt;
	}

	if (grid.NDIM == -1) Common::ExceptionError(FromHere(), "Cannot find dimension information in the grid file!", Common::ErrorCodes::NoSuchValue());
	if (grid.NNM  == -1) Common::ExceptionError(FromHere(), "Cannot find NNM information in the grid file!", Common::ErrorCodes::NoSuchValue());
	if (grid.NFM  == -1) Common::ExceptionError(FromHere(), "Cannot find NFM information in the grid file!", Common::ErrorCodes::NoSuchValue());
	if (grid.NCM  == -1) Common::ExceptionError(FromHere(), "Cannot find NCM information in the grid file!", Common::ErrorCodes::NoSuchValue());


	// Set the size of data and initialize mapping data
	grid.nodes.resize(grid.NNM);	//grid.nodeIDMap.clear();
	grid.faces.resize(grid.NFM);	//grid.faceIDMap.clear();
	grid.cells.resize(grid.NCM);	//grid.cellIDMap.clear();

	grid.whereisNode.resize(grid.NNM+1);
	grid.whereisFace.resize(grid.NFM+1);
	grid.whereisCell.resize(grid.NCM+1);



	// Read Node Data
	mesh_file.clear();
	mesh_file.seekg(0, std::ios::beg);
	while (!mesh_file.eof())
	{
		getline(mesh_file, line);

		tempInt = -1;
		tempInt = Common::read_data_from_string::read_numeric<int>(line, "NODEDATA= ");
		if (tempInt != -1)
		{
			if (tempInt != grid.NNM) Common::ExceptionError(FromHere(), "Mismatch the number of node data!", Common::ErrorCodes::MismatchData());

			for (int n = 0; n <= grid.NNM-1; n++)
			{
				int nodeID = -1;
				double x = 0.0;
				double y = 0.0;
				double z = 0.0;

				getline(mesh_file, line);
				sscanf(line.c_str(), "%d %lf %lf %lf", &nodeID, &x, &y, &z);

				if (nodeID != -1)
				{
					grid.nodes[n].id = nodeID;
					grid.nodes[n].x[0] = x;
					grid.nodes[n].x[1] = y;
					grid.nodes[n].x[2] = z;
					//grid.nodeIDMap.insert(nodeID, &grid.nodes[n]);

					if (nodeID +1 > grid.whereisNode.size()) grid.whereisNode.resize(nodeID+1);
					grid.whereisNode[nodeID] = n;
				}
			}

		}
	}



	// Read Face Data
	mesh_file.clear();
	mesh_file.seekg(0, std::ios::beg);
	while (!mesh_file.eof())
	{
		getline(mesh_file, line);

		tempInt = -1;
		tempInt = Common::read_data_from_string::read_numeric<int>(line, "FACEDATA= ");
		if (tempInt != -1)
		{
			if (tempInt != grid.NFM) Common::ExceptionError(FromHere(), "Mismatch the number of face data!", Common::ErrorCodes::MismatchData());

			for (int f = 0; f <= grid.NFM-1; f++)
			{
				int faceID = -1;
				int faceBC = -1;
				int faceType = -1;
				int cl = -1;
				int cr = -1;
				std::vector<int> faceNodeList(GRID_FACE_MAXNNODES, -1);

				getline(mesh_file, line);
				sscanf(line.c_str(), "%d %d %d", &faceID, &faceBC, &faceType);

				if (faceID != -1)
				{
					grid.faces[f].id   = faceID;
					grid.faces[f].bc   = faceBC;
					grid.faces[f].bcindex = faceBC%BC_MAX_DETAIL_SUBCOND;
					grid.faces[f].bc = grid.faces[f].bc - grid.faces[f].bcindex;
					grid.faces[f].type = faceType;


					switch(faceType)
					{
					case LINE:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d", &faceID, &faceBC, &faceType,
						                                             &faceNodeList[0], &faceNodeList[1],
																	 &cl, &cr);

						grid.faces[f].nnodes   = 2;
						grid.faces[f].nodes[0] = faceNodeList[0];
						grid.faces[f].nodes[1] = faceNodeList[1];
						break;

					case TRI3:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d %d", &faceID, &faceBC, &faceType,
																	    &faceNodeList[0], &faceNodeList[1], &faceNodeList[2],
																	    &cl, &cr);

						grid.faces[f].nnodes   = 3;
						grid.faces[f].nodes[0] = faceNodeList[0];
						grid.faces[f].nodes[1] = faceNodeList[1];
						grid.faces[f].nodes[2] = faceNodeList[2];
						break;

					case QUAD4:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d", &faceID, &faceBC, &faceType,
																	       &faceNodeList[0], &faceNodeList[1], &faceNodeList[2], &faceNodeList[3],
																	       &cl, &cr);

						grid.faces[f].nnodes   = 3;
						grid.faces[f].nodes[0] = faceNodeList[0];
						grid.faces[f].nodes[1] = faceNodeList[1];
						grid.faces[f].nodes[2] = faceNodeList[2];
						grid.faces[f].nodes[3] = faceNodeList[3];
						break;

					default:
						Common::ExceptionError(FromHere(), "It is not supported face-type", Common::ErrorCodes::NotSupportedType());
						break;
					}

#ifdef CFD
					grid.faces[f].cl[0] = cl;
					grid.faces[f].cr[0] = cr;
#endif

					//grid.faceIDMap.insert(faceID, &grid.faces[f]);
					if (faceID + 1 > grid.whereisFace.size()) grid.whereisFace.resize(faceID+1);
					grid.whereisFace[faceID] = f;
				}
			}

		}
	}



	// Read Cell Data
	mesh_file.clear();
	mesh_file.seekg(0, std::ios::beg);
	while (!mesh_file.eof())
	{
		getline(mesh_file, line);

		tempInt = -1;
		tempInt = Common::read_data_from_string::read_numeric<int>(line, "CELLDATA= ");
		if (tempInt != -1)
		{
			if (tempInt != grid.NCM) Common::ExceptionError(FromHere(), "Mismatch the number of cell data!", Common::ErrorCodes::MismatchData());

			for (int c = 0; c <= grid.NCM-1; c++)
			{
				int cellID   = -1;
				int cellBC   = -1;
				int cellType = -1;
				std::vector<int> cellNodeList(GRID_CELL_MAXNNODES, -1);
				std::vector<int> cellFaceList(GRID_CELL_MAXNFACES, -1);

				getline(mesh_file, line);
				sscanf(line.c_str(), "%d %d %d", &cellID, &cellBC, &cellType);

				if (cellID != -1)
				{
					grid.cells[c].id   = cellID;
					grid.cells[c].bc   = cellBC;
					grid.cells[c].type = cellType;


					switch(cellType)
					{
					case TRI3:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d", &cellID, &cellBC, &cellType,
																	       &cellNodeList[0], &cellNodeList[1], &cellNodeList[2],
																	       &cellFaceList[0], &cellFaceList[1], &cellFaceList[2]);
						grid.cells[c].nnodes   = 3;
						grid.cells[c].nodes[0] = cellNodeList[0];
						grid.cells[c].nodes[1] = cellNodeList[1];
						grid.cells[c].nodes[2] = cellNodeList[2];

						grid.cells[c].nfaces   = 3;
						grid.cells[c].faces[0] = cellFaceList[0];
						grid.cells[c].faces[1] = cellFaceList[1];
						grid.cells[c].faces[2] = cellFaceList[2];
						break;

					case QUAD4:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d %d %d", &cellID, &cellBC, &cellType,
																		         &cellNodeList[0], &cellNodeList[1], &cellNodeList[2],  &cellNodeList[3],
																		         &cellFaceList[0], &cellFaceList[1], &cellFaceList[2],  &cellFaceList[3]);
						grid.cells[c].nnodes   = 4;
						grid.cells[c].nodes[0] = cellNodeList[0];
						grid.cells[c].nodes[1] = cellNodeList[1];
						grid.cells[c].nodes[2] = cellNodeList[2];
						grid.cells[c].nodes[3] = cellNodeList[3];

						grid.cells[c].nfaces   = 4;
						grid.cells[c].faces[0] = cellFaceList[0];
						grid.cells[c].faces[1] = cellFaceList[1];
						grid.cells[c].faces[2] = cellFaceList[2];
						grid.cells[c].faces[3] = cellFaceList[3];
						break;

					case TETRA4:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d %d %d", &cellID, &cellBC, &cellType,
																		         &cellNodeList[0], &cellNodeList[1], &cellNodeList[2],  &cellNodeList[3],
																		         &cellFaceList[0], &cellFaceList[1], &cellFaceList[2],  &cellFaceList[3]);
						grid.cells[c].nnodes   = 4;
						grid.cells[c].nodes[0] = cellNodeList[0];
						grid.cells[c].nodes[1] = cellNodeList[1];
						grid.cells[c].nodes[2] = cellNodeList[2];
						grid.cells[c].nodes[3] = cellNodeList[3];

						grid.cells[c].nfaces   = 4;
						grid.cells[c].faces[0] = cellFaceList[0];
						grid.cells[c].faces[1] = cellFaceList[1];
						grid.cells[c].faces[2] = cellFaceList[2];
						grid.cells[c].faces[3] = cellFaceList[3];
						break;

					case HEXA8:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &cellID, &cellBC, &cellType,
											&cellNodeList[0], &cellNodeList[1], &cellNodeList[2], &cellNodeList[3], &cellNodeList[4], &cellNodeList[5], &cellNodeList[6], &cellNodeList[7],
											&cellFaceList[0], &cellFaceList[1], &cellFaceList[2], &cellFaceList[3], &cellFaceList[4], &cellFaceList[5]);
						grid.cells[c].nnodes   = 8;
						grid.cells[c].nodes[0] = cellNodeList[0];
						grid.cells[c].nodes[1] = cellNodeList[1];
						grid.cells[c].nodes[2] = cellNodeList[2];
						grid.cells[c].nodes[3] = cellNodeList[3];
						grid.cells[c].nodes[4] = cellNodeList[4];
						grid.cells[c].nodes[5] = cellNodeList[5];
						grid.cells[c].nodes[6] = cellNodeList[6];
						grid.cells[c].nodes[7] = cellNodeList[7];

						grid.cells[c].nfaces   = 6;
						grid.cells[c].faces[0] = cellFaceList[0];
						grid.cells[c].faces[1] = cellFaceList[1];
						grid.cells[c].faces[2] = cellFaceList[2];
						grid.cells[c].faces[3] = cellFaceList[3];
						grid.cells[c].faces[4] = cellFaceList[4];
						grid.cells[c].faces[5] = cellFaceList[5];
						break;

					case PRISM6:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d %d %d %d %d %d", &cellID, &cellBC, &cellType,
											&cellNodeList[0], &cellNodeList[1], &cellNodeList[2], &cellNodeList[3], &cellNodeList[4], &cellNodeList[5],
											&cellFaceList[0], &cellFaceList[1], &cellFaceList[2], &cellFaceList[3], &cellFaceList[4]);
						grid.cells[c].nnodes   = 6;
						grid.cells[c].nodes[0] = cellNodeList[0];
						grid.cells[c].nodes[1] = cellNodeList[1];
						grid.cells[c].nodes[2] = cellNodeList[2];
						grid.cells[c].nodes[3] = cellNodeList[3];
						grid.cells[c].nodes[4] = cellNodeList[4];
						grid.cells[c].nodes[5] = cellNodeList[5];

						grid.cells[c].nfaces   = 5;
						grid.cells[c].faces[0] = cellFaceList[0];
						grid.cells[c].faces[1] = cellFaceList[1];
						grid.cells[c].faces[2] = cellFaceList[2];
						grid.cells[c].faces[3] = cellFaceList[3];
						grid.cells[c].faces[4] = cellFaceList[4];
						break;

					case PYRAMID5:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d %d %d %d %d", &cellID, &cellBC, &cellType,
											&cellNodeList[0], &cellNodeList[1], &cellNodeList[2], &cellNodeList[3], &cellNodeList[4],
											&cellFaceList[0], &cellFaceList[1], &cellFaceList[2], &cellFaceList[3], &cellFaceList[4]);
						grid.cells[c].nnodes   = 5;
						grid.cells[c].nodes[0] = cellNodeList[0];
						grid.cells[c].nodes[1] = cellNodeList[1];
						grid.cells[c].nodes[2] = cellNodeList[2];
						grid.cells[c].nodes[3] = cellNodeList[3];
						grid.cells[c].nodes[4] = cellNodeList[4];

						grid.cells[c].nfaces   = 5;
						grid.cells[c].faces[0] = cellFaceList[0];
						grid.cells[c].faces[1] = cellFaceList[1];
						grid.cells[c].faces[2] = cellFaceList[2];
						grid.cells[c].faces[3] = cellFaceList[3];
						grid.cells[c].faces[4] = cellFaceList[4];
						break;

					default:
						Common::ExceptionError(FromHere(), "It is not supported cell-type", Common::ErrorCodes::NotSupportedType());
						break;
					}

					//grid.cellIDMap.insert(cellID, &grid.cells[c]);
					if (cellID + 1 > grid.whereisCell.size()) grid.whereisCell.resize(cellID+1);
					grid.whereisCell[cellID] = c;

				}
			}
		}
	}



	mesh_file.close();
}



}

