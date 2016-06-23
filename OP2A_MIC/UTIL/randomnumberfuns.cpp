/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 *
 * 		Copyright (c) 2015-2016 MINKWAN KIM
 *
 * @file randomnumberfuns.cpp
 * @author minkwan
 * @date Mar 3, 2016
 * @brief Multi-thread random number generator
 *
 * Detailed description
 */
#include <stdio.h>
#include <iostream>
#include <omp.h>
#include "mkl.h"
#include "mkl_vsl_types.h"


static unsigned int seed = 0x11111111;
static VSLStreamStatePtr stream;
static bool m_rand_flag = false;

float* ranf_OMP(int N, int ncore)
{
	int nskip = N / ncore;
    int n_rand[ncore];
    int n_start[ncore];
    int sum;

    n_rand[0]	= nskip;
    n_start[0]  = 0;
    sum         = nskip;

    for (int i = 1; i <= ncore-2; i++)
    {
    	n_rand[i]  = nskip;
    	n_start[i] = sum;
    	sum += n_rand[i];
    }

    n_start[ncore-1]  = sum;
    n_rand[ncore-1] = N - sum;


    float *buff = new float[N];


	// Creating and Initializeing Random streams
    vslNewStream(&stream, VSL_BRNG_MCG59, VSL_BRNG_RDRAND);

    mkl_set_num_threads(ncore);
#pragma omp parallel for
    for (int i = 0; i <= ncore-1; i++)
    {
    	vslSkipAheadStream(stream, nskip*i);
    	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, n_rand[i], (float*)&(buff[n_start[i]]), 0.0f, 1.0f);
    }

    return (buff);
}




float ranf(void)
{
    float rf;
    unsigned int *irf = (unsigned int * ) &rf;
    seed *= 0x2C4856AD;
    *irf = 0x3F800000 | (seed >> 9);
    return (rf-1.0);
}


double ranf_mkl(void)
{
	double out;
    if (m_rand_flag == false)
	{
    	vslNewStream(&stream, VSL_BRNG_SFMT19937, VSL_BRNG_RDRAND);
    	m_rand_flag = true;
	}

    vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, stream, 1, &out, 0.5, 1.0);
    return (out);
}

double ranf_range(double a, double b)
{
	double out;
    if (m_rand_flag == false)
	{
    	vslNewStream(&stream, VSL_BRNG_SFMT19937, VSL_BRNG_RDRAND);
    	m_rand_flag = true;
	}

	vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 1, &out, a, b);
	return (out);
}


