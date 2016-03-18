/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 *
 * 		Copyright (c) 2015-2016 MINKWAN KIM
 *
 * @file processorinfo.hpp
 * @author minkwan
 * @date Mar 10, 2016
 * @brief (Type brief description of file
 *
 * Detailed description
 */

#ifndef PROCESSORINFO_HPP_
#define PROCESSORINFO_HPP_


#define THREAD_PER_MIC_CORE	4
#define MAX_CPUs			100
#define THREAD_PER_CPU		2
#define MIC_PER_NODE		4


namespace Common {

class processor_info_MIC {
public:
	explicit processor_info_MIC(int nCPU, int nMIC);

	int num_CPU;		    // Total number of Processor (CPUs)
	int num_MIC;		    // Number of MICs

	int taskID;			    // Current CPU ID
	int num_threads;	    // Number of thread for CPUs

	int MIC_ID[4];		    // MIC ID list
	int num_threads_MIC[4]; // Number of thread for MIC

	void show_info();

};



} /* namespace Common */

#endif /* PROCESSORINFO_HPP_ */
