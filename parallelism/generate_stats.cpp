#include <iostream>
#include <fstream>
#include <cmath>
#include "generate_stats.h"
void generateStats(const char (&version)[], const int nb_core, const int N, const int M, const double &runtime, const size_t nsplits) {
       //	struct stat buffer; 
	//bool file_exists = stat ( "stats.csv", &buffer ) != 0; 
	std::ofstream stats; 
	stats.open("stats.csv", std::ios_base::app); 
	//if(file_exists){
	//	stats << "version, nbcore, num_steps, runtime" << std::endl; 
	//}
	// columns names 
	// values 
	printf("the value of nsplits is : %zu \n " , nsplits);
	stats << version << "," << nb_core << "," <<  log2(N) << "," << log2(M) << "," << runtime << "," << log2(nsplits) - log2(N) << std::endl;
	
	stats.close();
}
