#! /bin/bash 

stats_file='stats/cache.log'
for(( i=1 ; i<=${1} ; i++ )) 
do 
    for(( j=1 ; j<=${1} ; j++ )) 
    do 
	echo "2^$i 2^$j $(valgrind --tool=cachegrind --log-file="cache_logs/.${i}_${j}.tmp" ../../sequential -N $i  -M $j -nrepeat 1)" 


    bash cache_info.sh cache_logs/.${i}_${j}.tmp ${i} ${j} > cache_logs/.filter_${i}_${j}.tmp


cat cache_logs/.filter_${i}_${j}.tmp | 
   awk '{
    for (i = 1; i <= NF; i++) {
        a[NR, i] = $i
    }
    if (NF > p) {
        p = NF
    }
}
END {
    for (j = 1; j <= p; j++) {
        str = a[1, j]
        for (i = 2; i <= NR; i++) {
            str = str " " a[i, j]
        }
        print str
    }
}' > .result.log 
    # if file exists and not empty 
    if [[ -f $stats_file && -s $stats_file ]] 
    then 
	tail -n1 .result.log >> $stats_file 
    else 
	cat .result.log  >> $stats_file
    fi

 
# cache_logs/${i}_${j}.cachegrind.log


	rm cachegrind*
	#rm cache_logs/.*tmp
	#rm .result.log
    done

done 
