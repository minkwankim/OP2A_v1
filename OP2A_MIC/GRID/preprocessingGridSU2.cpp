/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * preprocessingGridSU2.cpp
 *
 *  Created on: 18 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include "grid.hpp"
#include "../COMM/readdatafromstring.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"


namespace GRID {




void precessingGridSU2(const  std::string& mesh_file_name_SU2)
{
	int temp_dummy;
	int readIntTemp;
	int numFace;
	int numNode;
	std::string line;

	std::vector< std::vector<int> >		cell;
	std::vector< std::vector<int> >		face;
	std::vector< std::vector<double> >	node;





	// 1. Open file to read
	std::ifstream mesh_file_SU2;
	mesh_file_SU2.open(mesh_file_name_SU2.c_str());


	// 2. Set variables
	int dim;
	int nnm;
	int nfm;
	int ncm;


	if (mesh_file_SU2.is_open())
	{

		// 3. FIRST READ
		while (! mesh_file_SU2.eof())
		{
			getline(mesh_file_SU2, line);

			readIntTemp = -1;
			readIntTemp = Common::read_data_from_string::read_numeric<int>(line, "NDIME=");
			if (readIntTemp != -1)	dim	=readIntTemp;

			// CELL
			readIntTemp = -1;
			readIntTemp = Common::read_data_from_string::read_numeric<int>(line, "NELEM=");
			if (readIntTemp != -1)
			{
				ncm = readIntTemp;
				cell.resize(ncm);

				int temp_cellType;
				int temp_cellNode[8];
				int temp_cellID;

				for (int c=0; c <= ncm-1; c++)
				{

					getline(mesh_file_SU2, line);
					sscanf(line.c_str(), "%d", &temp_cellType);

					switch(temp_cellType)
					{
					case TRI3:
						sscanf(line.c_str(), "%d %d %d %d %d", &temp_dummy,
															   &temp_cellNode[0],
															   &temp_cellNode[1],
															   &temp_cellNode[2],
															   &temp_cellID);
						cell[temp_cellID].resize(1+3+1);
						cell[temp_cellID][0] = temp_dummy;
						cell[temp_cellID][1] = temp_cellNode[0];
						cell[temp_cellID][2] = temp_cellNode[1];
						cell[temp_cellID][3] = temp_cellNode[2];
						cell[temp_cellID][4] = temp_cellID;
						break;

					case QUAD4:
						sscanf(line.c_str(), "%d %d %d %d %d %d", &temp_dummy,
																  &temp_cellNode[0],
																  &temp_cellNode[1],
																  &temp_cellNode[2],
																  &temp_cellNode[3],
																  &temp_cellID);
						cell[temp_cellID].resize(1+4+1);
						cell[temp_cellID][0] = temp_dummy;
						cell[temp_cellID][1] = temp_cellNode[0];
						cell[temp_cellID][2] = temp_cellNode[1];
						cell[temp_cellID][3] = temp_cellNode[2];
						cell[temp_cellID][4] = temp_cellNode[3];
						cell[temp_cellID][5] = temp_cellID;
						break;

					case TETRA4:
						sscanf(line.c_str(), "%d %d %d %d %d %d", &temp_dummy,
																  &temp_cellNode[0],
																  &temp_cellNode[1],
																  &temp_cellNode[2],
																  &temp_cellNode[3],
																  &temp_cellID);
						cell[temp_cellID].resize(1+4+1);
						cell[temp_cellID][0] = temp_dummy;
						cell[temp_cellID][1] = temp_cellNode[0];
						cell[temp_cellID][2] = temp_cellNode[1];
						cell[temp_cellID][3] = temp_cellNode[2];
						cell[temp_cellID][4] = temp_cellNode[3];
						cell[temp_cellID][5] = temp_cellID;
						break;

					case HEXA8:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d %d", &temp_dummy,
																			  &temp_cellNode[0],
																			  &temp_cellNode[1],
																			  &temp_cellNode[2],
																			  &temp_cellNode[3],
																			  &temp_cellNode[4],
																			  &temp_cellNode[5],
																			  &temp_cellNode[6],
																			  &temp_cellNode[7],
																			  &temp_cellID);
						cell[temp_cellID].resize(1+8+1);
						cell[temp_cellID][0] = temp_dummy;
						cell[temp_cellID][1] = temp_cellNode[0];
						cell[temp_cellID][2] = temp_cellNode[1];
						cell[temp_cellID][3] = temp_cellNode[2];
						cell[temp_cellID][4] = temp_cellNode[3];
						cell[temp_cellID][5] = temp_cellNode[4];
						cell[temp_cellID][6] = temp_cellNode[5];
						cell[temp_cellID][7] = temp_cellNode[6];
						cell[temp_cellID][8] = temp_cellNode[7];
						cell[temp_cellID][9] = temp_cellID;
						break;

					case PRISM6:
						sscanf(line.c_str(), "%d %d %d %d %d %d %d %d", &temp_dummy,
																		&temp_cellNode[0],
																		&temp_cellNode[1],
																		&temp_cellNode[2],
																		&temp_cellNode[3],
																		&temp_cellNode[4],
																		&temp_cellNode[5],
																		&temp_cellID);
						cell[temp_cellID].resize(1+6+1);
						cell[temp_cellID][0] = temp_dummy;
						cell[temp_cellID][1] = temp_cellNode[0];
						cell[temp_cellID][2] = temp_cellNode[1];
						cell[temp_cellID][3] = temp_cellNode[2];
						cell[temp_cellID][4] = temp_cellNode[3];
						cell[temp_cellID][5] = temp_cellNode[4];
						cell[temp_cellID][6] = temp_cellNode[5];
						cell[temp_cellID][7] = temp_cellID;
						break;
					}
				}
			}


			// NODE
			readIntTemp = -1;
			readIntTemp = Common::read_data_from_string::read_numeric<int>(line, "NPOIN=");
			if (readIntTemp != -1)
			{
				nnm = readIntTemp;
				node.resize(nnm);

				int temp_nodeID;
				double tempX[3];

				if (dim == 2)
				{
					for (int n=0; n <= nnm-1; n++)
					{
						getline(mesh_file_SU2, line);
						sscanf(line.c_str(), "%lf %lf %d", &tempX[0], &tempX[1], &temp_nodeID);

						node[temp_nodeID].resize(2);
						node[temp_nodeID][0] = tempX[0];
						node[temp_nodeID][1] = tempX[1];
					}
				}
				else if (dim == 3)
				{
					for (int n=0; n <= nnm-1; n++)
					{
						getline(mesh_file_SU2, line);
						sscanf(line.c_str(), "%lf %lf %lf %d", &tempX[0], &tempX[1], &tempX[2], &temp_nodeID);

						node[temp_nodeID].resize(3);
						node[temp_nodeID][0] = tempX[0];
						node[temp_nodeID][1] = tempX[1];
						node[temp_nodeID][2] = tempX[2];

					}
				}
			}




		}
	}
	else
	{
		Common::ExceptionError(FromHere(), "Cannot find grid file. Please check your grid file!", Common::ErrorCodes::FileSystem());
	}

	mesh_file_SU2.close();


	// Construct faces
	nfm = 0;
	for (int c = 0; c <= ncm-1; c++)
	{
		switch (cell[c][0])
		{
		case TRI3:
			numFace = 3;
			numNode = 2;
			face.resize(nfm+numFace);

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = LINE;
			face[nfm][1] = cell[c][1];
			face[nfm][2] = cell[c][2];
			face[nfm][3] = c;
			face[nfm][4] = -1;
			nfm++;

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = LINE;
			face[nfm][1] = cell[c][2];
			face[nfm][2] = cell[c][3];
			face[nfm][3] = c;
			face[nfm][4] = -1;
			nfm++;

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = LINE;
			face[nfm][1] = cell[c][3];
			face[nfm][2] = cell[c][1];
			face[nfm][3] = c;
			face[nfm][4] = -1;
			nfm++;
			break;

		case QUAD4:
			numFace = 4;
			numNode = 2;
			face.resize(nfm+numFace);

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = LINE;
			face[nfm][1] = cell[c][1];
			face[nfm][2] = cell[c][2];
			face[nfm][3] = c;
			face[nfm][4] = -1;
			nfm++;

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = LINE;
			face[nfm][1] = cell[c][2];
			face[nfm][2] = cell[c][3];
			face[nfm][3] = c;
			face[nfm][4] = -1;
			nfm++;

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = LINE;
			face[nfm][1] = cell[c][3];
			face[nfm][2] = cell[c][4];
			face[nfm][3] = c;
			face[nfm][4] = -1;
			nfm++;

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = LINE;
			face[nfm][1] = cell[c][4];
			face[nfm][2] = cell[c][1];
			face[nfm][3] = c;
			face[nfm][4] = -1;
			nfm++;
			break;

		case TETRA4:
			numFace = 4;
			numNode = 3;
			face.resize(nfm+numFace);

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = TRI3;
			face[nfm][1] = cell[c][1];
			face[nfm][2] = cell[c][2];
			face[nfm][3] = cell[c][3];
			face[nfm][4] = c;
			face[nfm][5] = -1;
			nfm++;

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = TRI3;
			face[nfm][1] = cell[c][1];
			face[nfm][2] = cell[c][4];
			face[nfm][3] = cell[c][2];
			face[nfm][4] = c;
			face[nfm][5] = -1;
			nfm++;

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = TRI3;
			face[nfm][1] = cell[c][2];
			face[nfm][2] = cell[c][4];
			face[nfm][3] = cell[c][5];
			face[nfm][4] = c;
			face[nfm][5] = -1;
			nfm++;

			face[nfm].reserve(1 + numNode +2);
			face[nfm][0] = TRI3;
			face[nfm][1] = cell[c][1];
			face[nfm][2] = cell[c][3];
			face[nfm][3] = cell[c][4];
			face[nfm][4] = c;
			face[nfm][5] = -1;
			nfm++;
			break;
		}
	}


	// Processing Face data;
	std::vector<bool> faceFlag(nfm, false);

	for (int f = 0; f <= nfm-1; f++)
	{
		for (int f1 = 0; f1 <= nfm-1; f1++)
		{
			if (f1 != f && faceFlag[f1] == false)
			{
				if (face[f][1] == face[f1][1] && face[f][2] == face[f1][2])
				{
					faceFlag[f] = true;
				}

				if (face[f][1] == face[f1][2] && face[f][2] == face[f1][1])
				{
					faceFlag[f] = true;
					face[f][4] = face[f1][3];
				}
			}
		}
	}


	int nfm_temp = 0;
	for (int f = 0; f <= nfm-1; f++)
	{
		if (faceFlag[f] == true) nfm_temp++;
	}

	int temp1 = 0;



}












}




