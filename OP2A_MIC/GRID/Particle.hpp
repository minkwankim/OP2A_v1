/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Particle.hpp
 *
 *  Created on: 22 Jun 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include <stddef.h>

#ifndef GRID_PARTICLE_HPP_
#define GRID_PARTICLE_HPP_

class ParticleBasic {

public:
	ParticleBasic();
	~ParticleBasic();


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

	ParticleBasic* previous;
	ParticleBasic* next;
};



#define Particle  ParticleBasic

#endif /* GRID_PARTICLE_HPP_ */
