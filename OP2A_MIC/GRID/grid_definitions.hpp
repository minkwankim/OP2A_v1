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

#ifdef DIM3D
    #define DIM 3
    #define GRID_CELL_MAXNFACES 6
    #define GRID_CELL_MAXNNODES 8

    #define GRID_FACE_MAXNNODES 4

#else
    #define DIM 2
    #define GRID_CELL_MAXNFACES 4
    #define GRID_CELL_MAXNNODES 4

    #define GRID_FACE_MAXNNODES 2
#endif



// VTK Format Identifier
#define	LINE	 3
#define	TRI3	 5
#define	QUAD4	 9
#define	TETRA4   10
#define	HEXA8	 12
#define PRISM6	 13
#define PYRAMID5 14





#define GRID_FACE_MAXSTENCIL 2





#endif /* GRID_GRID_DEFINITIONS_HPP_ */
