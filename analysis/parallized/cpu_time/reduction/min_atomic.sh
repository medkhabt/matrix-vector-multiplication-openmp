#! /bin/bash 
awk '{if(!($1 + $2) in sum){
        sum[$1+$2]=$3;
	x[$1+$2]=$1;
	y[$1+$2]=$2;
      }else{
        if(sum[$1+$2] == 0 || sum[$1+$2] >= $3){
          sum[$1+$2]=$3
	  x[$1+$2]=$1;
	  y[$1+$2]=$2;
        }
      }
     }
     END { for(key in sum){
	    printf "%s %s %s %f\n", key, x[key], y[key], sum[key];
    }}' $1 
