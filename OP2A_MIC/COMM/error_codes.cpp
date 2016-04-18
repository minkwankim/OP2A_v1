/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 *
 * 		Copyright (c) 2015-2016 MINKWAN KIM
 *
 * @file error_codes.cpp
 * @author minkwan
 * @date Mar 3, 2016
 * @brief (Type brief description of file
 *
 * Detailed description
 */
#include "../COMM/error_codes.hpp"

#include <string>
#include <sstream>

namespace Common {


std::string ErrorCodes::Mismatch()
{
	return ("Mismatch:");
}

std::string ErrorCodes::Mismatch(std::string &details)
{
	std::ostringstream ss;
	ss << "Mismatch " << details <<":";
	return (ss.str());
}

std::string ErrorCodes::MismatchDimension()
{
	std::string ss = "Dimension";
	return(ErrorCodes::Mismatch(ss));
}

std::string ErrorCodes::MismatchData()
{
	std::string ss = "Data value";
	return(ErrorCodes::Mismatch(ss));
}

std::string ErrorCodes::MismatchDataType()
{
	std::string ss = "Data type";
	return(ErrorCodes::Mismatch(ss));
}

std::string ErrorCodes::NotSupportedType()
{
	return ("NotSupportedType:");
}

std::string ErrorCodes::NotAllocated()
{
	return ("NotSupportedType:");
}

std::string ErrorCodes::Negative()
{
	return ("Negative value:");
}

std::string ErrorCodes::NaN()
{
	return ("NaN value:");
}

std::string ErrorCodes::Inf()
{
	return ("Inf value:");
}

std::string ErrorCodes::ExceedLimit()
{
	return ("ExceedLimit:");
}

std::string ErrorCodes::FileSystem()
{
	return ("FileSystem:");
}

std::string ErrorCodes::FailAssertion()
{
	return ("FailAssertion:");
}

std::string ErrorCodes::NoSuchValue()
{
	return ("NoSuchValue:");
}

} /* namespace Common */
