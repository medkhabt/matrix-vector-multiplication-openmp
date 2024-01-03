sed "s/,/ /g" $1 | awk '{print($3,$4,$5)}'
