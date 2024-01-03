#include <iostream>
#include <fstream>
#include <cmath>
#include "generate_stats.h"
void generateStats(const char (&version)[], const int nb_core, const int N, const int M, const double &runtime, const int chunk) {
       //	struct stat buffer; 
	//bool file_exists = stat ( "stats.csv", &buffer ) != 0; 
	std::ofstream stats; 
	stats.open("stats.csv", std::ios_base::app); 
	//if(file_exists){
	//	stats << "version, nbcore, num_steps, runtime" << std::endl; 
	//}
	// columns names 
	// values 
	stats << version << "," << nb_core << "," << chunk << "," <<  log2(N) << "," << log2(M) << "," << runtime << std::endl;
	
	stats.close();
}
