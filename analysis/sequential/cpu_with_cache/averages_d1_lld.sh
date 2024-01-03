#! /bin/bash

awk '{sum[$4]+= $1 ; N[$4]++} 
	END { for(key in sum) {
	    avg = sum[key]/N[key]; 
	    printf "%s %f\n", key, avg;
    }}' $1 
