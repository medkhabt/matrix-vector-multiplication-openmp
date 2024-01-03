#! /bin/bash 
awk '{sum[$1]+=$3; N[$1]++}
	END { for(key in sum){
	    avg = sum[key]/N[key]; 
	    printf "%s %f\n", key, avg;
    }}' $1 
