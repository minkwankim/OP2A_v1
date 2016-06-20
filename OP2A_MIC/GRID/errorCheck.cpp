/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * errorCheck.cpp
 *
 *  Created on: 5 May 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */




#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <limits>


#include "grid.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"


namespace GRID {

void errorCheckGrid(Grid& grid)
{
	std::cout << "Start Error Checking in loaded grid data...." << std::endl;
	std::ostringstream error_message;

	// Basic information check
	std::cout << " -Basic data info check: ";
	if (grid.NNM != grid.nodes.size())
	{
		error_message << "[Node data size error]:  [INFO Section]=" << grid.NNM << "    [Size of Date set]=" <<grid.nodes.size();
		Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Mismatch());
	}

	if (grid.NFM != grid.faces.size())
	{
		error_message << "[Face data size error]:  [INFO Section]=" << grid.NFM << "    [Size of Date set]=" <<grid.faces.size();
		Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Mismatch());
	}

	if (grid.NCM != grid.cells.size())
	{
		error_message << "[Cell data size error]:  [INFO Section]=" << grid.NCM << "    [Size of Date set]=" <<grid.cells.size();
		Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Mismatch());
	}

	if (grid.NGM != grid.cells_ghost.size())
	{
		error_message << "[Ghost-Cell data size error]:  [INFO Section]=" << grid.NGM << "    [Size of Date set]=" <<grid.cells_ghost.size();
		Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Mismatch());
	}
	std::cout << "OK" << std::endl;



	// Node Data
	std::cout << " -Node data check: ";
	for (int n = 0; n < grid.NNM; n++)
	{
		if (n != grid.whereisNode[grid.nodes[n].id])
		{
			error_message << "[Node data mapping error at POS:" << n << "]:  [Actual ID]=" << grid.nodes[n].id << "    [Mapped POS]=" << n;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Mismatch());
		}

		for (int k = 0; k < grid.NDIM; k++)
		{
			if (grid.nodes[n].x[k] != grid.nodes[n].x[k])
			{
				error_message << "[Node position data error at POS[" << n << "  ID[" << grid.nodes[n].id <<"]]:  It has NaN value at X["<< k <<"]";
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
			}

			if (grid.nodes[n].x[k] == std::numeric_limits<double>::infinity())
			{
				error_message << "[Node position data error at POS[" << n << "  ID[" << grid.nodes[n].id <<"]]:  It has Inf value at X["<< k <<"]";
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
			}
		}

		if (grid.NODE(grid.nodes[n].id).id != grid.nodes[n].id)
		{
			error_message << "[Node data mapping error, NODE fn]:  [Actual ID]=" << grid.nodes[n].id << "    [Mapped ID]=" << grid.NODE(grid.nodes[n].id).id;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Mismatch());
		}
	}
	std::cout << "OK" << std::endl;


	// Face Data
	std::cout << " -Face data check: ";
	for (int f = 0; f < grid.NFM; f++)
	{
		if (f != grid.whereisFace[grid.faces[f].id])
		{
			error_message << "[Face data mapping error at POS:" << f <<"]:  [Actual ID]=" << grid.faces[f].id << "    [Mapped Pos]=" << f;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Mismatch());
		}

		for (int k = 0; k < grid.NDIM; k++)
		{
			if (grid.faces[f].x[k] != grid.faces[f].x[k])
			{
				error_message << "[Face position data error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has NaN value at X["<< k <<"]";
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
			}

			if (grid.faces[f].x[k] == std::numeric_limits<double>::infinity())
			{
				error_message << "[Face position data error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has Inf value at X["<< k <<"]";
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
			}
		}


		if (grid.faces[f].area != grid.faces[f].area)
		{
			error_message << "[Face area value error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has NaN value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
		}

		if (grid.faces[f].area == std::numeric_limits<double>::infinity())
		{
			error_message << "[Face aera value error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has Inf value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
		}


		if (grid.faces[f].area_inv != grid.faces[f].area_inv)
		{
			error_message << "[Face area_inv value error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has NaN value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
		}

		if (grid.faces[f].area_inv == std::numeric_limits<double>::infinity())
		{
			error_message << "[Face aera_inv value error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has Inf value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
		}



		if (grid.faces[f].bcindex < 0 || grid.faces[f].bcindex >= BC_MAX_DETAIL_SUBCOND)
		{
			error_message << "[Face BC_index error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  BC_index =" << grid.faces[f].bcindex;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
		}

		if (grid.faces[f].bc != BC_INTERIOR &&
			grid.faces[f].bc != BC_WALL &&
			grid.faces[f].bc != BC_INLET&&
			grid.faces[f].bc != BC_OUTLET&&
			grid.faces[f].bc != BC_FREESTREAM &&
			grid.faces[f].bc != BC_SYMMETRY &&
			grid.faces[f].bc != BC_AXIS &&
			grid.faces[f].bc != BC_ANODE &&
			grid.faces[f].bc != BC_CATHODE &&
			grid.faces[f].bc != BC_DIELECTRIC)
		{
			error_message << "[Face BC error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  BC =" << grid.faces[f].bc;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
		}


		if (grid.faces[f].nnodes > GRID_FACE_MAXNNODES || grid.faces[f].nnodes < 2)
		{
			error_message << "[Face nnodes(number of nodes) error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  nnodes =" << grid.faces[f].nnodes;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
		}


		for (int n = 0; n < grid.faces[f].nnodes; n++)
		{
			if (grid.faces[f].nodes[n] <= 0 ||  grid.faces[f].nodes[n] > grid.NNM)
			{
				error_message << "[Face node_index error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  node_index["<< n  << "]=" << grid.faces[f].nodes[n];
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NoSuchValue());
			}
		}

		if (grid.faces[f].type != LINE &&
			grid.faces[f].type != TRI3 &&
			grid.faces[f].type != QUAD4)
		{
			error_message << "[Face type error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  Face type=" << grid.faces[f].type;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NotAllocated());
		}

		for (int k1 = 0; k1 < grid.NDIM; k1++)
		{
			double test_double = 0.0;
			for (int k2 = 0; k2 < grid.NDIM; k2++)
			{
				if (grid.faces[f].n[k1][k2] != grid.faces[f].n[k1][k2])
				{
					error_message << "[Face directional vector error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has NaN value:n[" << k1 << "][" << k2 <<"]";
					Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
				}

				if (grid.faces[f].n[k1][k2] == std::numeric_limits<double>::infinity())
				{
					error_message << "[Face directional vector error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has Inf value:n[" << k1 << "][" << k2 <<"]";
					Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
				}

				test_double += grid.faces[f].n[k1][k2]*grid.faces[f].n[k1][k2];
			}

			test_double = sqrt(test_double);

			if (fabs(1.0 - test_double) > 1.0e-6)
			{
				error_message << "[Face directional vector error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It is not unit vector:n[" << k1 << "]";
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
			}
		}


#ifdef CFD
		if (grid.faces[f].distWall != grid.faces[f].distWall)
		{
			error_message << "[Face distWall value error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has NaN value";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
		}

		if (grid.faces[f].distWall == std::numeric_limits<double>::infinity())
		{
			error_message << "[Face distWall value error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has Inf value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
		}

		if (grid.faces[f].distWall < 0.0)
		{
			error_message << "[Face distWall value error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has a negative value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Negative());
		}

		if (grid.faces[f].nDotWall != grid.faces[f].nDotWall)
		{
			error_message << "[Face nDotWall value error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has NaN value";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
		}

		if (grid.faces[f].nDotWall == std::numeric_limits<double>::infinity())
		{
			error_message << "[Face nDotWall value error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  It has Inf value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
		}


		if (grid.faces[f].cl[0] > grid.NCM || grid.faces[f].cl[0] < -grid.NGM || grid.faces[f].cl[0] == 0)
		{
			error_message << "[Face stencil:CL error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  CL=" << grid.faces[f].cl[0];
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
		}

		if (grid.faces[f].cl[1] > grid.NCM || grid.faces[f].cl[1] < -grid.NGM || grid.faces[f].cl[1] == 0)
		{
			error_message << "[Face stencil:CLL error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  CLL=" << grid.faces[f].cl[1];
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
		}

		if (grid.faces[f].cr[0] > grid.NCM || grid.faces[f].cr[0] < -grid.NGM || grid.faces[f].cr[0] == 0)
		{
			error_message << "[Face stencil:CR error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  CR=" << grid.faces[f].cr[0];
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
		}

		if (grid.faces[f].cr[1] > grid.NCM || grid.faces[f].cr[1] < -grid.NGM || grid.faces[f].cr[1] == 0)
		{
			error_message << "[Face stencil:CRR error at POS[" << f << "  ID[" << grid.faces[f].id <<"]]:  CRR=" << grid.faces[f].cr[1];
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
		}
#endif
	}
	std::cout << "OK" << std::endl;


	// Cell Data
	std::cout << " -Cell data check: ";
	for (int c = 0; c < grid.NCM; c++)
	{
		if (c != grid.whereisCell[grid.cells[c].id])
		{
			error_message << "[Cell data mapping error at POS:" << c <<"]:  [Actual ID]=" << grid.cells[c].id << "    [Pos]=" << c;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Mismatch());
		}

		for (int k = 0; k < grid.NDIM; k++)
		{
			if (grid.cells[c].x[k] != grid.cells[c].x[k])
			{
				error_message << "[Cell position data error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  It has NaN value at X["<< k <<"]";
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
			}

			if (grid.cells[c].x[k] == std::numeric_limits<double>::infinity())
			{
				error_message << "[Cell position data error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  It has Inf value at X["<< k <<"]";
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
			}
		}


		if (grid.cells[c].vol != grid.cells[c].vol)
		{
			error_message << "[Cell vol value error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  It has NaN value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
		}

		if (grid.cells[c].vol == std::numeric_limits<double>::infinity())
		{
			error_message << "[Cell vol value error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  It has Inf value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
		}


		if (grid.cells[c].vol_inv != grid.cells[c].vol_inv)
		{
			error_message << "[Cell vol_inv value error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  It has NaN value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
		}

		if (grid.cells[c].vol_inv == std::numeric_limits<double>::infinity())
		{
			error_message << "[Cell vol_inv value error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  It has Inf value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
		}


		if (grid.cells[c].charLength != grid.cells[c].charLength)
		{
			error_message << "[Cell charLength value error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  It has NaN value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
		}

		if (grid.cells[c].charLength == std::numeric_limits<double>::infinity())
		{
			error_message << "[Cell charLength value error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  It has Inf value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
		}


		if (grid.cells[c].bc != BC_INTERIOR &&
			grid.cells[c].bc != BC_WALL &&
			grid.cells[c].bc != BC_INLET&&
			grid.cells[c].bc != BC_OUTLET&&
			grid.cells[c].bc != BC_FREESTREAM &&
			grid.cells[c].bc != BC_SYMMETRY &&
			grid.cells[c].bc != BC_AXIS &&
			grid.cells[c].bc != BC_ANODE &&
			grid.cells[c].bc != BC_CATHODE &&
			grid.cells[c].bc != BC_DIELECTRIC)
		{
			error_message << "[Cell BC error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  BC =" << grid.cells[c].bc;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
		}


		if (grid.cells[c].nnodes > GRID_CELL_MAXNNODES || grid.cells[c].nnodes < 3)
		{
			error_message << "[Cell nnodes(number of nodes) error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  nnodes =" << grid.cells[c].nnodes;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
		}


		for (int n = 0; n < grid.cells[c].nnodes; n++)
		{
			if (grid.cells[c].nodes[n] <= 0 ||  grid.cells[c].nodes[n] > grid.NNM)
			{
				error_message << "[Cell node_index error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  node_index["<< n  << "]=" << grid.cells[c].nodes[n];
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NoSuchValue());
			}
		}


		if (grid.cells[c].nfaces > GRID_CELL_MAXNFACES || grid.cells[c].nfaces < 3)
		{
			error_message << "[Cell nfaces(number of faces) error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  nfaces =" << grid.cells[c].nfaces;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::ExceedLimit());
		}

		for (int f = 0; f < grid.cells[c].nfaces; f++)
		{
			if (grid.cells[c].faces[f] <= 0 ||  grid.cells[c].faces[f] > grid.NFM)
			{
				error_message << "[Cell face_index error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  face_index["<< f  << "]=" << grid.cells[c].faces[f];
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NoSuchValue());
			}
		}

		if (grid.cells[c].type != TRI3 &&
			grid.cells[c].type != QUAD4 &&
			grid.cells[c].type != TETRA4 &&
			grid.cells[c].type != HEXA8 &&
			grid.cells[c].type != PRISM6 &&
			grid.cells[c].type != PYRAMID5)
		{
			error_message << "[Cell type error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  Cell type=" << grid.cells[c].type;
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NotAllocated());
		}


#ifdef CFD
		if (grid.cells[c].distWall != grid.cells[c].distWall)
		{
			error_message << "[Cell distWall value error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  It has NaN value";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NaN());
		}

		if (grid.cells[c].distWall == std::numeric_limits<double>::infinity())
		{
			error_message << "[Cell distWall value error at POS[" << c << "  ID[" << grid.cells[c].id <<"]]:  It has Inf value.";
			Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::Inf());
		}
#endif


	}
	std::cout << "OK" << std::endl;
}




}
