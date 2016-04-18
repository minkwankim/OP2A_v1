/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Particles.cpp
 *
 *  Created on: 31 Mar 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include "Particles.hpp"
#include "../UTIL/vectorstasticsfn.hpp"

namespace GRID {

Particles::Particles()
: species(-1), Erot(0.0), Evib(0.0), Eele(0.0), Wratio(1.0)
{
    X[0] = 0.0;
    X[1] = 0.0;
    X[2] = 0.0;

    V[0] = 0.0;
    V[1] = 0.0;
    V[2] = 0.0;
}

Particles::~Particles()
{
	// TODO Auto-generated destructor stub
}



void Particles::assign_location(Grid& grid, int Cell_ID)
{
	switch (grid.CELL(Cell_ID)->type)
	{
	case TRI3:
		assign_location_TRI3(grid, Cell_ID);
		break;

	case QUAD4:
		assign_location_QUAD4(grid, Cell_ID);
		break;

	case TETRA4:
		assign_location_TETRA4(grid, Cell_ID);
		break;

	case HEXA8:
		assign_location_HEXA8(grid, Cell_ID);
		break;

	case PRISM6:
		assign_location_PRISM6(grid, Cell_ID);
		break;
	}
}



void Particles::assign_location_TRI3(Grid& grid, int Cell_ID)
{
	int num_try = 0;
	double Xm[DIM];
	bool is_inside = false;


	while (is_inside == false && num_try < 1000)
	{
		for (int d = 0; d <= DIM-1; d++)	Xm[d] = double(ranf());
		if (Xm[0] < 1.0 && Xm[1] < 1.0 && (Xm[0] + Xm[1]) < 1.0) is_inside = true;
		num_try++;
	}

	double psi1 = 1 - Xm[0] - Xm[1];
	double psi2 = Xm[1];
	double psi3 = Xm[0];

	X[0] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[0]*psi1
			+ grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[0]*psi2
			+ grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[0]*psi3;

	X[1] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[1]*psi1
			+ grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[1]*psi2
			+ grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[1]*psi3;
}

void Particles::assign_location_QUAD4(Grid& grid, int Cell_ID)
{
	double Xm[DIM];

	for (int d = 0; d <= DIM-1; d++)	Xm[d] = double(ranf_range(-1.0, 1.0));

	double psi1 = 0.25 * (1-Xm[0]) * (1-Xm[1]);
	double psi2 = 0.25 * (1+Xm[0]) * (1-Xm[1]);
	double psi3 = 0.25 * (1+Xm[0]) * (1+Xm[1]);
	double psi4 = 0.25 * (1-Xm[0]) * (1+Xm[1]);

	X[0] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[0]*psi1
			+ grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[0]*psi2
			+ grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[0]*psi3
			+ grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[0]*psi4;

	X[1] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[1]*psi1
			+ grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[1]*psi2
			+ grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[1]*psi3
			+ grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[1]*psi4;
}


void Particles::assign_location_TETRA4(Grid& grid, int Cell_ID)
{
	int num_try = 0;
	double Xm[3];
	bool is_inside = false;


	while (is_inside == false && num_try < 1000)
	{
		for (int d = 0; d <= DIM-1; d++)	Xm[d] = double(ranf());
		if ((Xm[0] + Xm[1] + Xm[3]) < 1.0) is_inside = true;
		num_try++;
	}


    double psi1 = 1 - Xm[0] - Xm[1] - Xm[2];
    double psi2 = Xm[0];
    double psi3 = Xm[1];
    double psi4 = Xm[2];

	X[0] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[0]*psi1
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[0]*psi2
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[0]*psi3
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[0]*psi4;

	X[1] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[1]*psi1
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[1]*psi2
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[1]*psi3
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[1]*psi4;

	X[2] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[2]*psi1
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[2]*psi2
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[2]*psi3
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[2]*psi4;
}


void Particles::assign_location_HEXA8(Grid& grid, int Cell_ID)
{
	double Xm[3];

	for (int d = 0; d <= 2; d++)	Xm[d] = double(ranf_range(-1.0, 1.0));

	double aux = 1.0 / 8.0;
	double psi1 = aux * (1-Xm[0]) * (1-Xm[1]) * (1-Xm[2]);
	double psi2 = aux * (1+Xm[0]) * (1-Xm[1]) * (1-Xm[2]);
	double psi3 = aux * (1+Xm[0]) * (1+Xm[1]) * (1-Xm[2]);
	double psi4 = aux * (1-Xm[0]) * (1+Xm[1]) * (1-Xm[2]);
	double psi5 = aux * (1-Xm[0]) * (1-Xm[1]) * (1+Xm[2]);
	double psi6 = aux * (1+Xm[0]) * (1-Xm[1]) * (1+Xm[2]);
	double psi7 = aux * (1+Xm[0]) * (1+Xm[1]) * (1+Xm[2]);
	double psi8 = aux * (1-Xm[0]) * (1+Xm[1]) * (1+Xm[2]);


	X[0] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[0]*psi1
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[0]*psi2
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[0]*psi3
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[0]*psi4
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[4])->x[0]*psi5
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[5])->x[0]*psi6
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[6])->x[0]*psi7
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[7])->x[0]*psi8;

	X[1] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[1]*psi1
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[1]*psi2
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[1]*psi3
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[1]*psi4
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[4])->x[1]*psi5
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[5])->x[1]*psi6
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[6])->x[1]*psi7
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[7])->x[1]*psi8;

	X[2] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[2]*psi1
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[2]*psi2
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[2]*psi3
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[2]*psi4
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[4])->x[2]*psi5
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[5])->x[2]*psi6
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[6])->x[2]*psi7
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[7])->x[2]*psi8;
}


void Particles::assign_location_PRISM6(Grid& grid, int Cell_ID)
{
	double Xm[3];

	for (int d = 0; d <= 2; d++)	Xm[d] = double(ranf_range(-1.0, 1.0));

	double aux = 1.0 / 8.0;
	double psi1 = aux * (1-Xm[0]) * (1-Xm[1]) * (1-Xm[2]);
	double psi2 = aux * (1+Xm[0]) * (1-Xm[1]) * (1-Xm[2]);
	double psi3 = aux * (1+Xm[0]) * (1+Xm[1]) * (1-Xm[2]);
	double psi4 = aux * (1-Xm[0]) * (1+Xm[1]) * (1-Xm[2]);
	double psi5 = aux * (1-Xm[0]) * (1-Xm[1]) * (1+Xm[2]);
	double psi6 = aux * (1+Xm[0]) * (1-Xm[1]) * (1+Xm[2]);
	double psi7 = aux * (1+Xm[0]) * (1+Xm[1]) * (1+Xm[2]);
	double psi8 = aux * (1-Xm[0]) * (1+Xm[1]) * (1+Xm[2]);


	X[0] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[0]*psi1
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[0]*psi2
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[0]*psi3
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[0]*psi4
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[4])->x[0]*psi5
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[5])->x[0]*psi6
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[6])->x[0]*psi7
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[7])->x[0]*psi8;

	X[1] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[1]*psi1
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[1]*psi2
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[1]*psi3
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[1]*psi4
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[4])->x[1]*psi5
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[5])->x[1]*psi6
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[6])->x[1]*psi7
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[7])->x[1]*psi8;

	X[2] = grid.NODE(grid.CELL(Cell_ID)->nodes[0])->x[2]*psi1
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[1])->x[2]*psi2
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[2])->x[2]*psi3
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[3])->x[2]*psi4
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[4])->x[2]*psi5
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[5])->x[2]*psi6
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[6])->x[2]*psi7
		 + grid.NODE(grid.CELL(Cell_ID)->nodes[7])->x[2]*psi8;
}





} /* namespace GRID */
