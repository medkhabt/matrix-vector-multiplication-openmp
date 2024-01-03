#! /bin/bash
if [[ $# -eq 0 ]]
then
    read cache_log
else 
    cache_log=$1    
fi
tail -n9 $cache_log | sed 's/==[0-9]*==//g' | sed 's/([^)]*)//g' | sed '/^[[:space:]]*$/d' | awk -F: '{gsub(/ /,"_",$1); print $1,$2}'

printf "\n Row $2 \n Col $3" 
