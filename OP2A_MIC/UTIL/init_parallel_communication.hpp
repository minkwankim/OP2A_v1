/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 *
 * 		Copyright (c) 2015-2016 MINKWAN KIM
 *
 * @file init_parallel_communication.hpp
 * @author minkwan
 * @date Mar 10, 2016
 * @brief (Type brief description of file
 *
 * Detailed description
 */

#ifndef INIT_PARALLEL_COMMUNICATION_HPP_
#define INIT_PARALLEL_COMMUNICATION_HPP_

#include "../COMM/processorinfo.hpp"
#include "../COMM/version.hpp"






void init_parallel_communication(int argc, char *argv[], Common::version ver, Common::processor_info_MIC& proc_info, double& t0);




#endif /* INIT_PARALLEL_COMMUNICATION_HPP_ */
