/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 *
 * 		Copyright (c) 2015-2016 MINKWAN KIM
 *
 * @file processorinfo.cpp
 * @author minkwan
 * @date Mar 10, 2016
 * @brief (Type brief description of file
 *
 * Detailed description
 */

#include "../COMM/common.hpp"
#include "../COMM/processorinfo.hpp"

namespace Common {

processor_info_MIC::processor_info_MIC(int nCPU, int nMIC)
    :num_CPU(-1), taskID(-1), num_threads(nCPU), num_MIC(nMIC)
{
	for (int i = 0; i <= 3; i++) MIC_ID[i] = i;
	for (int i = 0; i <= 3; i++) num_threads_MIC[i] = -1;
}


void processor_info_MIC::show_info()
{
	std::cout << "[Processor Information]" << std::endl;
	std::cout << "=======================" << std::endl << std::endl;
	std::cout << "Total number of CPUs:             " << num_CPU << std::endl;
	std::cout << "Total number of threads (OpenMP): " << num_threads << std::endl;

#ifdef MIC
	std::cout << "Total number of MICs:             " << num_CPU*num_MIC << std::endl;

	int tmp_thread_mic = 0;
	for (int k = 0; k <= num_MIC-1; k++) tmp_thread_mic += num_threads_MIC[k];
	std::cout << "Total number of threads (MIC):    " << num_CPU*tmp_thread_mic << std::endl;
#else
	std::cout << "(MIC is not used)" << std::endl;
#endif

	std::cout << std::endl << std::endl;
}


} /* namespace Common */
