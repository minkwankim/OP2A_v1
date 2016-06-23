/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Particle.cpp
 *
 *  Created on: 22 Jun 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include "Particle.hpp"

ParticleBasic::ParticleBasic()
: species(-1), Erot(0.0), Evib(0.0), Eele(0.0), Wratio(1.0)
{
	X[0] = 0.0;
	X[1] = 0.0;
	X[2] = 0.0;

	V[0] = 0.0;
	V[1] = 0.0;
	V[2] = 0.0;

	previous = NULL;
	next = NULL;
}

ParticleBasic::~ParticleBasic()
{

}
