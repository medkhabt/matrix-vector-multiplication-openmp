#! /bin/bah

## the first arg represnts the last instance test 
## to represent power of 2 we can just bitshift to the left  (1 << n)

for(( i=1 ; i<=${1} ; i++ )) 
do 
    for(( j=1 ; j<=${1} ; j++ )) 
    do 
	echo "2^$i 2^$j $(../sequential -N $i  -M $j -nrepeat 10)" 	
    done

done 
