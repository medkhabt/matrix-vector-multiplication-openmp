awk 'NR==FNR{a[$3_$4]=$5;next}{if($9_$10 in a) {print a[$9_$10], $0}}' stats.dat cache.dat
