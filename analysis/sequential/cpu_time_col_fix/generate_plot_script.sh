# first arg: start, second arg: end
# watch out for the color number it shouldn't exceed 24
echo 'set logscale y'
echo 'set key left top'
echo -n "plot "
for(( i=$1 ; i<${2} ; i++ )) 
do 
    echo -n " \"stats_rows_col_$i.dat\" title \"Row Fixed on 2^$i\" lt 8 lc $i with linespoints"
    echo -n ","
done 
    echo " \"stats_rows_col_$2.dat\" title \"Row Fixed on 2^$2\" lt 8 lc $2 with linespoints"
#echo "plot "stats_x_1.dat" title "n row 1" linetype 8 linecolor 1, "stats_x_2.dat" lt 8 lc 3, "stats_x_3.dat" lt 8 lc 4, "stats_x_15.dat" lt 8 lc 5, "stats_x_11.dat" lt 8 lc 6, "stats_x_9.dat" lt 8 lc 7"

