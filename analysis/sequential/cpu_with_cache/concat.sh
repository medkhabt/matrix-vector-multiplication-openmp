awk 'NR==FNR{a[$2_$3]=$4;next}{if($9_$10 in a) {print a[$9_$10], $0}}' stats.dat cache.dat
