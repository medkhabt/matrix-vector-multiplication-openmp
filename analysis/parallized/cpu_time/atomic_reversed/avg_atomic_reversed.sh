#! /bin/bash 
awk '{sum[$2]+=$3; N[$2]++}
	END { for(key in sum){
	    avg = sum[key]/N[key]; 
	    printf "%s %f\n", key, avg;
    }}' $1 
