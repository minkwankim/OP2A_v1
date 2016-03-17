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


/*
 * TEST
 */
/*
#include "COMMON/error_check.hpp"
#include "COMMON/timer.hpp"
#include "COMMON/version.hpp"
#include "COMMON/processorinfo.hpp"


#include "UTIL/vectorstasticsfn.hpp"
#include "UTIL/init_parallel_communication.hpp"
*/

/*
 * Global variables */




int main(int argc, char *argv[])
{
	double t0;
	time_t	m_t = time(0);
	struct	tm* m_now;
	m_now = localtime(& m_t);

	std::cout << "TEST";

	//Common::processor_info_MIC	proc_info(2, 2);
	//Common::version ver(1, 1,  m_now->tm_year + 1900, m_now->tm_mon + 1, m_now->tm_mday, "TEST");

	//init_parallel_communication(argc, argv, ver, proc_info, t0);

	int totalProcs;
#pragma offload target(mic:0)
	totalProcs = omp_get_num_procs();


	int N = 2000000000;
	int nthread = 12;
	float* buff;
//
//	double dt;
//	dt = Common::dtime();
//	for (int n = 0; n <= 5; n++)
//		buff = ranf_OMP(N, nthread);
//	dt = Common::dtime() - dt;
//
//	double dt1 = Common::dtime();
//	for (int n = 0; n <= 5; n++)
//	{
//		for (int i = 0; i < N; i++)	buff[i] = ranf();
//	}
//	dt1 = Common::dtime() - dt1;
//
//
//	// Problem Setting Section
//	std::string	simulation_title	= "Test";
//	std::string	mesh_file_name		= "PlasmaGun_ver1.cas";
//	std::string	species_file_name	= "species_info.dat";
}
