/*
//@HEADER
// ************************************************************************
//
//                        Kokkos v. 2.0
//              Copyright (2014) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions Contact  H. Carter Edwards (hcedwar@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <omp.h>

#include "generate_stats.h"

void checkSizes( uint32_t &N, uint32_t &M,uint32_t &S, int &nrepeat );
double matrix_x_multiplication( float &a, float &x); 
uint32_t x_find_with_modulo(uint32_t m, uint32_t sub_matrixes_per_row);
int main( int argc, char* argv[] )
{
  uint32_t N = -1;         // number of rows 2^12
  uint32_t M = -1;         // number of columns 2^10, i think we are not gonna use the last element of size_t in M as the S won't be in size_t
  uint32_t S = 0;         // total size 2^22
  int nsplits = 1;
  int nrepeat = 100;  // number of repeats of the test
  int numberCores = 1; 
  // Read command line arguments.
  for ( int i = 0; i < argc; i++ ) {
    if ( ( strcmp( argv[ i ], "-N" ) == 0 ) || ( strcmp( argv[ i ], "-Rows" ) == 0 ) ) {
      N = pow( 2, atoi( argv[ ++i ] ) );
      printf( "  User N is %d\n", N );
    }
    else if ( ( strcmp( argv[ i ], "-M" ) == 0 ) || ( strcmp( argv[ i ], "-Columns" ) == 0 ) ) {
      M = pow( 2, atof( argv[ ++i ] ) );
      printf( "  User M is %d\n", M );
    }
    else if ( ( strcmp( argv[ i ], "-S" ) == 0 ) || ( strcmp( argv[ i ], "-Size" ) == 0 ) ) {
      S = pow( 2, atof( argv[ ++i ] ) );
      printf( "  User S is %d\n", S );
    }
    else if ( strcmp( argv[ i ], "-nrepeat" ) == 0 ) {
      nrepeat = atoi( argv[ ++i ] );
    }

    else if ( strcmp( argv[ i ], "-C" ) == 0 ) {
      numberCores = atoi( argv[ ++i ] );
    }
    else if ( strcmp( argv[ i ], "-nsplits" ) == 0 ) {
      nsplits = pow(2, atoi( argv[ ++i ] ));
    }
    else if ( ( strcmp( argv[ i ], "-h" ) == 0 ) || ( strcmp( argv[ i ], "-help" ) == 0 ) ) {
      printf( "  y^T*A*x Options:\n" );
      printf( "  -Rows (-N) <int>:      exponent num, determines number of rows 2^num (default: 2^12 = 4096)\n" );
      printf( "  -Columns (-M) <int>:   exponent num, determines number of columns 2^num (default: 2^10 = 1024)\n" );
      printf( "  -Size (-S) <int>:      exponent num, determines total matrix size 2^num (default: 2^22 = 4096*1024 )\n" );
      printf( "  -nrepeat <int>:        number of repetitions (default: 100)\n" );
      printf( "  -help (-h):            print this message\n\n" );
      exit( 1 );
    }
  }

  // Check sizes.
  checkSizes( N, M, S, nrepeat );

  // Allocate x,y,A
// TODO: try with vector and with array
  auto y = new double[N];
// How about trying to split the function when its size is great that the llc. 
  const uint32_t sub_matrix_count = nsplits;
  uint32_t sub_matrixes_per_row = (uint32_t) (nsplits/(M*N)); 
sub_matrixes_per_row = (sub_matrixes_per_row > 1) ? sub_matrixes_per_row : 1; 
// TODO free up the memory of x.
 // split x to sub_matrixesPERROW parts so it will be in sync with the submatrixes.  
/// but we actually have a problem, how do specify the handle for each case. 
  auto x =  new float[M]; 
  auto A =  new float[M*N]; 
  double result_m = 0;
  double result = 0; 
  // Initialize y vector to 1
 std::fill_n(y,N, 1); 
 std::fill_n(x,M, 1); 
 std::fill_n(A,N*M, 1); 
const uint32_t size_sub_matrix = (uint32_t)ceil((M*N)/sub_matrix_count); 
// in case there multiple sub matrixes per row, we will enter to this loop
  // Initialize x vector to 1.

  // Initialize A matrix, you can use a 1D index if you want a flat structure (i.e. a 1D array) e.g. j*M+i is the same than [j][i]
  // Timer products.
  struct timeval begin, end;

  gettimeofday( &begin, NULL );
  double sum_a_x = 0; 
  int temp = 0; 
  //omp_set_num_threads(4);
  for ( int repeat = 0; repeat < nrepeat; repeat++ ) {
    // For each line i
	result = 0; 
	sum_a_x = 0; 
	#pragma omp  parallel for   num_threads(12) reduction(+:result) private(sum_a_x)
	// if we take in consideration that a l1 cache size is 32 kb and a double size is 8b, to not have cache misses for the l1 cache we should not calculate more than a slice of 4 elements per thread.   
	for(uint32_t i = 0 ; i < M * N ; i += M ) {
	    sum_a_x = 0;
	    for(uint32_t j = 0 ; j < M ; j++) {
		sum_a_x += A[i] * x[j];  
		//printf("sum_a_x value for the i=%d and j=%d is : %f \n ", i, j, sum_a_x);
		if(j < M -1 ) {
		    result += sum_a_x * y[i];   
		   // printf("getting the result of the i=%d which is :%f \n", i, result);
		}
	    }
	} 

	// Multiply the i lines with the vector x 
        // Sum the results of the previous step into a single variable
        // Multiply the result of the previous step with the i value of vector y
        // Sum the results of the previous step into a single variable (result)

    // Output result.
    if ( repeat == ( nrepeat - 1 ) ) {
      printf( "  Computed result for %d x %d is %lf\n", N, M, result );
    }

    const double solution = (double) N * (double) M;

    if ( result != solution ) {
      printf( "  Error: result( %lf ) != solution( %lf )\n", result, solution );
    }

  }
        
  gettimeofday( &end, NULL );

  // Calculate time.
  //double time = timer.seconds();
  double time = 1.0 * ( end.tv_sec - begin.tv_sec ) +
                1.0e-6 * ( end.tv_usec - begin.tv_usec );

  // Calculate bandwidth.
  // Each matrix A row (each of length M) is read once.
  // The x vector (of length M) is read N times.
  // The y vector (of length N) is read once.
  // double Gbytes = 1.0e-9 * double( sizeof(double) * ( 2 * M * N + N ) );
  double Gbytes = 1.0e-9 * double( sizeof(double) * ( M + M * N + N ) );

  // Print results (problem size, time and bandwidth in GB/s).
  printf( "  N( %d ) M( %d ) nrepeat ( %d ) problem( %g MB ) time( %g s ) bandwidth( %g GB/s )\n",
          N, M, nrepeat, Gbytes * 1000, time, Gbytes * nrepeat / time );
  generateStats("sequential_split_modulo",  numberCores, N, M, time, nsplits);
  //std::free(A);
 delete[] A; 
 delete[] y; 
 delete[] x; 
  // std::free(A); 
  //std::free(y);
  //std::free(x);

  return 0;
}

void checkSizes( uint32_t &N, uint32_t &M, uint32_t &S, int &nrepeat ) {
  // If S is undefined and N or M is undefined, set S to 2^22 or the bigger of N and M.
  if ( S == 0 && ( N == -1 || M == -1 ) ) {
    S = pow( 2, 22 );
    if ( S < N && N != -1) S = N;
    if ( S < M && M != -1) S = M;
  }

  // If S is undefined and both N and M are defined, set S = N * M.
  if ( S == 0 ) S =  N * M;

  // If both N and M are undefined, fix row length to the smaller of S and 2^10 = 1024.
  if ( N == -1 && M == -1 ) {
    if ( S > 1024 ) {
      M = 1024;
    }
    else {
      M = S;
    }
  }

  // If only M is undefined, set it.
  if ( M == -1 ) M = S / N;

  // If N is undefined, set it.
  if ( N == -1 ) N = S / M;

  printf( "  Total size S = %d N = %d M = %d\n", S, N, M );

  // Check sizes.
  if ( ( S < 0 ) || ( N < 0 ) || ( M < 0 ) || ( nrepeat < 0 ) ) {
    printf( "  Sizes must be greater than 0.\n" );
    exit( 1 );
  }

  if ( ( N * M ) != S ) {
    printf( "  N * M != S\n" );
    exit( 1 );
  }
}


double matrix_x_multiplication( float &a, float &x ) {
	return a * x; 
}
uint32_t x_find_with_modulo(uint32_t m, uint32_t sub_matrixes_per_row ) {
	return m & (sub_matrixes_per_row - 1);  
}
