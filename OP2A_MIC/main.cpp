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


#include "COMM/error_check.hpp"
#include "COMM/timer.hpp"
#include "COMM/version.hpp"
#include "COMM/processorinfo.hpp"
#include "COMM/readdatafromstring.hpp"

#include "UTIL/vectorstasticsfn.hpp"
#include "UTIL/init_parallel_communication.hpp"

/*
 * Global variables
 */




int main(int argc, char *argv[])
{

	// Setting Version information
	time_t	m_t = time(0);
	struct	tm* m_now;
	m_now = localtime(& m_t);
	Common::version ver(1, 1,  m_now->tm_year + 1900, m_now->tm_mon + 1, m_now->tm_mday, "TEST");


	// Initialize MPI and Offloading Process
	double t0;
	Common::processor_info_MIC	proc_info(1, 2);
	init_parallel_communication(argc, argv, ver, proc_info, t0);
	if (proc_info.taskID == 0) proc_info.show_info();



	// Problem Setting Section
	std::string	simulation_title	= "Test";
	std::string	mesh_file_name		= "FILE: PlasmaGun_ver1.cas  // FILE NAME    ";
	simulation_title = Common::read_data_from_string::read_string(mesh_file_name, "FILE:");


	int a = 0;

//	std::string	species_file_name	= "species_info.dat";
}
