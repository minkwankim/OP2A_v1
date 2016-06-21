/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 *
 * 		Copyright (c) 2015-2016 MINKWAN KIM
 *
 * @file main.cpp
 * @author minkwan
 * @date Mar 1, 2016
 * @brief (Type brief description of file
 *
 * Detailed description
 */

#include <time.h>
#include <omp.h>
#include <mpi.h>
#include <iostream>
#include <iomanip>
#include "mkl.h"


#include "COMM/error_check.hpp"
#include "COMM/timer.hpp"
#include "COMM/version.hpp"
#include "COMM/processorinfo.hpp"
#include "COMM/readdatafromstring.hpp"
#include "COMM/MultiVector.hpp"

#include "UTIL/vectorstasticsfn.hpp"
#include "UTIL/init_parallel_communication.hpp"
#include "UTIL/ArrayNegativeIndex.hpp"

#include "PROB/ProblemSetup.hpp"

#include "GRID/grid.hpp"
#include "GRID/GridCart.hpp"
#include "GRID/Particles.hpp"
#include "GRID/Element.hpp"


#include "MATH/Matrix.hpp"
#include "MATH/Vector.hpp"

#include "DATA/DataCart.hpp"

/*
 * Global variables
 */

#define CURRENT_MAIN_VERSION 1
#define CURRENT_SUB_VERSION  1
#define CURRENT_VERSION_TYPE "TEST"

#define NUM_THREADS_PER_CPU       12
#define NUM_MIC_INSTALLED         1




int main(int argc, char *argv[])
{
	/*
	 * ==========================================
	 * Pre-Processing Part ver 1.0
	 * ==========================================
	 * It is initially developed by Min Kwan Kim
	 *
	 *                Last Updated
	 *                             on 21/June/2016
	 *                             by Min Kwan Kim
	 */

	// STEP 1. Setting Version information
	// [Please DO NOT CHANGE
	time_t	m_t = time(0);
	struct	tm* m_now;
	m_now = localtime(& m_t);
	Common::version ver(CURRENT_MAIN_VERSION, CURRENT_SUB_VERSION,  m_now->tm_year + 1900, m_now->tm_mon + 1, m_now->tm_mday, CURRENT_VERSION_TYPE);

	// STEP 2. Initialize MPI and Offloading Process
	double t0;
	Common::processor_info_MIC	proc_info(NUM_THREADS_PER_CPU, NUM_MIC_INSTALLED);
	init_parallel_communication(argc, argv, ver, proc_info, t0);
	if (proc_info.taskID == 0) proc_info.show_info();









	/*
	 * =====================================================================
	 * [CODE TESTING SECTION]
	 * =====================================================================
	 * Variables for test should be started with test[XXXX]
	 */
	std::vector<double> testXs(DIM, 0.0);
	std::vector<double> testXe(DIM, 0.0);	testXe[0] = 10.0; testXe[1] = 20.0;
	std::vector<int>    testNx(DIM, 0);     testNx[0] = 100;  testNx[1] = 200;
	std::vector<int>    testBC(4, 0);       testBC[0] = 10;   testBC[1] = 10; testBC[2] = 10; testBC[3] = 10;

	GRID::CartCell testCell;

	testCell.createBackgroundMesh(testXs, testXe, testNx, testBC);
	testCell.refineCell(testCell(1,2));
	testCell.refineCell(testCell(2,2));

	testCell.refineCell(testCell(1, 2, 1, 2));
	testCell.refineCell(testCell(1, 2, 1, 1));
	testCell.refineCell(testCell(1, 2, 2, 8));

	GRID::writeGridCartTecplot("testCart", testCell);



	GRID::precessingGridSU2("test1.su2");











	std::cout <<"Done!!" << std::endl;
	std::cout <<"Done!!" << std::endl;


	/*
	GRID::readGrid("test.op2", testGrid);
	GRID::processingGrid(testGrid);
	GRID::errorCheckGrid(testGrid);

	std::vector<int> cellProcID;
	GRID::partGridBasic(testGrid, proc_info.taskID, 4);
	std::cout << "Part is done";
	//GRID::Grid testGrid1;
	//GRID::partGrid(testGrid, 1, testGrid1);



	// TESTing Part Mesh

	vector2D dataTemp = Common::MultiVector::create2D(testGrid.NCM+1, 1);
	for (int i = 1; i <= testGrid.NCM; i++)	dataTemp[i][0] = testGrid.mpiConfig.cellProcID[i];
	std::vector<std::string> variableTemp(1);
	variableTemp[0] = "CPU numbers";

	std::ostringstream testTitle;
	testTitle << "testData" << proc_info.taskID;
	GRID::writeFulltecplotCellCenter(testTitle.str(), testGrid, dataTemp, variableTemp);







	std::string filename = "test.dat";
	//ProblemCommon test_problemsetup;
	//test_problemsetup.read(filename);



	// Problem Setting Section
	std::string	simulation_title	= "Test";
	std::string	mesh_file_name		= "FILE:// FILE NAME    ";
	double tmp = Common::read_data_from_string::read_numeric<double>(mesh_file_name, "FILE:", 1000);



//	std::string	species_file_name	= "species_info.dat";

 */
}





