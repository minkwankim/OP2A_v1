/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * grid.hpp
 *
 *  Created on: 7 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef GRID_GRID_HPP_
#define GRID_GRID_HPP_

#include <vector>

#include "Node.hpp"
#include "Face.hpp"
#include "Cell.hpp"
#include "Cell.hpp"
#include "../UTIL/ArrayNegativeIndex.hpp"



namespace GRID {


class GridSetup{
public:
	GridSetup();
	~GridSetup();

	double gridFactor;
	bool   isAxisymmetric;
};

class GridMPI{
public:
	GridMPI();
	~GridMPI();

public:
	int numLines;	// Number of lines
	std::vector< std::vector<int> > lines;

	std::vector< std::vector<int> > sendGhostIndex;
	std::vector< std::vector<int> > receiveGhostIndex;

	int *sendGhost;
	int *receiveGhost;

	ArrayNegativeIndex<int> cellProcID;	// Process IDs of each Cell ID
};


class Grid{

public:
	Grid();
	~Grid();

public:
	int NDIM;
	int NNM;
	int NFM;
	int NCM;
	int NGM;
	GridSetup configutation;
	GridMPI   mpiConfig;

	std::vector<Node> nodes;
	std::vector<Face> faces;
	std::vector<Cell> cells;
	std::vector<Cell> cells_ghost;


	std::vector<int> whereisCell;	// Cell ID --> Location in data set
	std::vector<int> whereisFace;	// Face ID --> Location in data set
	std::vector<int> whereisNode;   // Node ID --> Location in data set
	std::vector<int> whereisGhost; // Ghost ID --> Location in data set

	/*
	Common::Map1D<int, Node*> nodeIDMap;
	Common::Map1D<int, Face*> faceIDMap;
	Common::Map1D<int, Cell*> cellIDMap;
    */

	Node& NODE(int nID);
	Face& FACE(int fID);
	Cell& CELL(int cID);

	bool isInCell(std::vector<double> x, int cellID);


protected:


};

int faceComp(std::vector<int>& F1_list, std::vector<int>& F2_list, int type);
int faceTypeNumNode(int facetype);
int cellTypeNumNode(int celltype);
int cellTypeNumFace(int celltype);
bool isWallTypeBC(int bc);
void faceNodeNumberingReverse(std::vector<int>& nodeList, int type);





void precessingGridSU2(const  std::string& mesh_file_name_SU2);
void precessingGridFLUENT(const  std::string& mesh_file_name_Fluent);

void readGrid(const  std::string& mesh_file_name, Grid& grid);
void writeGridtecplot(const  std::string& mesh_file_name);
void writeGridtecplot(const  std::string& title, Grid& grid);
void writeDatatecplot(const  std::string& file_name, Grid& grid, std::vector< std::vector<double> >& data, std::vector<std::string>& variables);
void writeFulltecplotCellCenter(const  std::string& title, Grid& grid, std::vector< std::vector<double> >& data, std::vector<std::string>& variables);



void processingGrid(Grid& grid);
void errorCheckGrid(Grid& grid);

void partGridBasic(Grid& grid, int p, int NP);
void partGrid(Grid& grid, int p, Grid& grid_p);


}



#endif /* GRID_GRID_HPP_ */
