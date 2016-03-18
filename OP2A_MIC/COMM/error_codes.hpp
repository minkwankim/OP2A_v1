/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 *
 * 		Copyright (c) 2015-2016 MINKWAN KIM
 *
 * @file error_codes.hpp
 * @author minkwan
 * @date Mar 3, 2016
 * @brief (Type brief description of file
 *
 * Detailed description
 */

#ifndef ERROR_CODES_HPP_
#define ERROR_CODES_HPP_


#include "../COMM/common.hpp"
#include "../COMM/noninstantiable.hpp"

namespace Common {

class ErrorCodes: NonInstantiable<ErrorCodes>
{
public:
	static std::string Mismatch();
	static std::string Mismatch(std::string &details);

	static std::string MismatchDimension();
	static std::string MismatchData();
	static std::string MismatchDataType();

	static std::string NotSupportedType();
	static std::string NotAllocated();

	static std::string Negative();
	static std::string NaN();
	static std::string Inf();

	static std::string ExceedLimit();
	static std::string FileSystem();
};

} /* namespace Common */

#endif /* ERROR_CODES_HPP_ */
