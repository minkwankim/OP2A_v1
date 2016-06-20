/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * grid_definitions.hpp
 *
 *  Created on: 7 Apr 2016
 *      Author: a1633125
 *
 *	@brief It contains the defined values for cell/face/node and grid
 */

#ifndef GRID_GRID_DEFINITIONS_HPP_
#define GRID_GRID_DEFINITIONS_HPP_


// CAN BE ADJUSTABLE
#define GRID_MAX_REFINEMENT_LVL 5
#define GRID_MAX_NUM_PATH 1000
#define GRID_MAX_NUM_LINES 2000
#define GRID_MAX_NUM_CPU 2000
#define GRID_GHOST_RESERVE_FOR_MPI GRID_MAX_NUM_CPU


// DO NOT CHANGE FOLLOWING SECTION
#define GRID_FACE_MAXSTENCIL 2
#define GRID_AXISMEMTRIC_Y 1.0e-8

#ifdef DIM3D
    #define DIM 3
    #define GRID_CELL_MAXNFACES 6
    #define GRID_CELL_MAXNNODES 8

    #define GRID_FACE_MAXNNODES 4

    #define GRID_REFINEMENT     8
#else
    #define DIM 2
    #define GRID_CELL_MAXNFACES 4
    #define GRID_CELL_MAXNNODES 4

    #define GRID_FACE_MAXNNODES 2

    #define GRID_REFINEMENT     4
#endif


// VTK Format Identifier
#define	LINE	 3
#define	TRI3	 5
#define	QUAD4	 9
#define	TETRA4   10
#define	HEXA8	 12
#define PRISM6	 13
#define PYRAMID5 14

#define BC_INTERIOR 	0
#define BC_WALL			10
#define BC_INLET		20
#define BC_OUTLET		30
#define BC_FREESTREAM	40
#define BC_SYMMETRY	    50
#define BC_AXIS			60
#define BC_ANODE		70
#define BC_CATHODE		80
#define BC_DIELECTRIC	90
#define BC_MAX_DETAIL_SUBCOND 10

#define GRID_NOR	0
#define GRID_TAN1	1
#define GRID_TAN2	2
#define GRID_X 0
#define GRID_Y 1
#define GRID_Z 2


#define BC_WEST    0
#define BC_SOUTH   1
#define BC_EAST    2
#define BC_NORTH   3
#define BC_TOP     4
#define BC_BOTTOM  5

#define INDEX_ADD_CPU    10
#define INDEX_ADD_GHOST  100


#endif /* GRID_GRID_DEFINITIONS_HPP_ */
