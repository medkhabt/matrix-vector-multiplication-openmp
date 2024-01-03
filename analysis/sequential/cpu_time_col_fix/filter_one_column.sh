for(( i=1 ; i<=${1} ; i++ )) 
do 
    echo "we are here with i = $i" 
    # awk -v var="$i" '$3 ~ /^var$/ {print($2,$4)}' stats.csv  
    awk -v var="^$i\$" '$3 ~ var {print($2,$4)}' stats.csv > stats_rows_col_$i.dat 
done
