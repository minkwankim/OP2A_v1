/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 *
 * 		Copyright (c) 2015-2016 MINKWAN KIM
 *
 * @file error_check.hpp
 * @author minkwan
 * @date Mar 3, 2016
 * @brief (Type brief description of file
 *
 * Detailed description
 */

#ifndef ERROR_CHECK_HPP_
#define ERROR_CHECK_HPP_

#include "../COMM/codelocation.hpp"
#include "../COMM/common.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"
#include "../COMM/noninstantiable.hpp"


namespace Common {

class COMMON_API ErrorCheck : public Common::NonInstantiable<ErrorCheck>
{
public:
	static void NumericValue(const Common::CodeLocation& where, const std::string& valueName, double value);
	static void NonNegative(const Common::CodeLocation& where, const std::string& valueName, double value);



};

} /* namespace Common */

#endif /* ERROR_CHECK_HPP_ */
