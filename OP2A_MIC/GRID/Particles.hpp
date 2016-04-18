/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Particles.hpp
 *
 *  Created on: 31 Mar 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef GRID_PARTICLES_HPP_
#define GRID_PARTICLES_HPP_

#include "grid.hpp"

namespace GRID {

class Particles {
public:
	Particles();
	~Particles();


public:
	int species;

	/* Position */
	double X[3];

	/* Velocities */
	double V[3];


	/* Internal Energies */
	double Erot;
	double Evib;
	double Eele;

	double Wratio;


	// Internal functions
	//void assign_location(Grid& grid, int cell_ID);
	void assign_location(Grid& grid, int cell_ID);
	void assign_location_TRI3(Grid& grid, int cell_ID);
	void assign_location_QUAD4(Grid& grid, int cell_ID);
	void assign_location_TETRA4(Grid& grid, int cell_ID);
	void assign_location_HEXA8(Grid& grid, int cell_ID);
	void assign_location_PRISM6(Grid& grid, int cell_ID);

};

} /* namespace GRID */

#endif /* GRID_PARTICLES_HPP_ */
