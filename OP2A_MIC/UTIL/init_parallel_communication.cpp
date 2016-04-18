/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 *
 * 		Copyright (c) 2015-2016 MINKWAN KIM
 *
 * @file init_parallel_communication.cpp
 * @author minkwan
 * @date Mar 10, 2016
 * @brief Initialize Parallel Communication
 *
 */

#include <stdio.h>
#include <omp.h>
#include <mpi.h>

#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"
#include "../COMM/processorinfo.hpp"
#include "../COMM/timer.hpp"
#include "../COMM/version.hpp"

/*
 * @param num_proc     Maximum number of avaialble processors
 * @param num_thread   Maximum number of threads. Valid only in MIC and GPU mode
 * @param taskID       Current CPU number (0= master node)
 * @param t0           Starting wall time info
 */
void init_parallel_communication(int argc, char *argv[], Common::version ver, Common::processor_info_MIC& proc_info, double& t0)
{
	int num_threads;
	int max_threads;
	int micid;


	// Initiate MPI
	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_info.taskID);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_info.num_CPU);

	max_threads = omp_get_max_threads();
	if (proc_info.num_CPU == 1) proc_info.num_threads = max_threads;
	else						proc_info.num_threads = THREAD_PER_CPU;


	// Initiate MIC (Offloading)
#ifdef MIC
    for (int i = 0; i <= proc_info.num_MIC-1; i++)
    {
    	micid = proc_info.MIC_ID[i];

    	#pragma offload target(mic:micid)
            num_threads = omp_get_max_threads();

        proc_info.num_threads_MIC[i] = num_threads;
    }
#else
    proc_info.num_MIC = 0;
#endif


    // Check Error
    if (proc_info.num_CPU > MAX_CPUs)		    Common::ExceptionError(FromHere(), "Number of tasks exceeds MAX_CPUs", Common::ErrorCodes::ExceedLimit());
    if (proc_info.num_MIC > MAX_MIC_PER_NODE)	Common::ExceptionError(FromHere(), "Number of MIC exceeds MAX_MIC_PER_NODE", Common::ErrorCodes::ExceedLimit());


    // Print version info
    if (proc_info.taskID == 0)	ver.info();
    t0 = Common::dtime();
}









