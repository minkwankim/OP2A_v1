/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * CalArea.cpp
 *
 *  Created on: 22 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include "CalArea.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"
#include "Vector.hpp"


double CalArea::line(std::vector<double>& n1, std::vector<double>& n2)
{
	int m = n1.size();
	if (n2.size() != m)
	{
		Common::ExceptionError(FromHere(), "The dimensions of two points should be same to calculate distance", Common::ErrorCodes::Mismatch());
	}


	double len	= 0.0;
	for (int i = 0; i <= m-1; i++)	len	+= pow(n1[i] - n2[i], 2.0);

	return (sqrt(len));
}


double CalArea::triangle(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3)
{
	double a, b, c, s;
	double area;

	a = line(n1, n2);
	b = line(n2, n3);
	c = line(n3, n1);

	s = 0.5 * (a + b + c);

	area	= s * (s-a) * (s-b) * (s-c);
	if (area < 0.0) Common::ExceptionError(FromHere(), "Negative triangle area", Common::ErrorCodes::Negative());

	return (sqrt(area));
}


double CalArea::quadrilateral(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3, std::vector<double>& n4)
{
	double s1, s2;

	s1 = triangle(n1, n2, n3);
	s2 = triangle(n2, n3, n4);

	return (s1 + s2);
}


double CalArea::tetrahedron(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3, std::vector<double>& n4)
{
	double vol;

	Vector A(n4, n1);
	Vector B(n4, n2);
	Vector C(n4, n3);


	Vector temp1 = crossProduct(B, C);

	vol = dotProduct(A, temp1);
	vol		= fabs(vol) / 6.0;

	return (vol);
}



double CalArea::pyramid(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3, std::vector<double>& n4,  std::vector<double>& n5)
{
	int i;
	double volume;
	std::vector<double> n6(3, 0.0);


	for (i = 0; i <= 2; i++)	n6[i] = 0.25 * (n1[i] + n2[i] + n3[i] + n4[i]);

	volume = tetrahedron(n1, n2, n6, n5);
	volume += tetrahedron(n2, n3, n6, n5);
	volume += tetrahedron(n3, n4, n6, n5);
	volume += tetrahedron(n4, n1, n6, n5);

	return (volume);
}


double CalArea::prism(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3, std::vector<double>& n4, std::vector<double>& n5, std::vector<double>& n6)
{
	int i;
	std::vector<double> n7(3, 0.0);

	double volume;

	for (i = 0; i <= 2; i++)	n7[i] = (n1[i] + n2[i] + n3[i] + n4[i] + n5[i] + n6[i]) / 6.0;

	volume = pyramid(n1, n4, n5, n2, n7);
	volume += pyramid(n2, n3, n6, n3, n7);
	volume += pyramid(n1, n3, n6, n4, n7);
	volume += tetrahedron(n1, n2, n3, n7);
	volume += tetrahedron(n4, n6, n5, n7);

	return (volume);
}


double CalArea::hexahedron(std::vector<double>& n1, std::vector<double>& n2,  std::vector<double>& n3, std::vector<double>& n4, std::vector<double>& n5, std::vector<double>& n6, std::vector<double>& n7, std::vector<double>& n8)
{
	int i;
	std::vector<double> n9(3, 0.0);
	double volume;

	for (i = 0; i <= 2; i++)	n9[i] = 0.125 * (n1[i] + n2[i] + n3[i] + n4[i] + n5[i] + n6[i] + n7[i] + n8[i]);


	volume = pyramid(n1, n2, n3, n4, n9);
	volume += pyramid(n5, n8, n7, n6, n9);
	volume += pyramid(n1, n5, n6, n2, n9);
	volume += pyramid(n3, n7, n8, n4, n9);
	volume += pyramid(n2, n6, n7, n3, n9);
	volume += pyramid(n1, n4, n8, n5, n9);

	return (volume);
}
















