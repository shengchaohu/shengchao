#!/bin/sh
start=20200101
end=20200331

# start date and end date
start=$(date -d $start +%Y-%m-%d)
end=$(date -d $end +%Y-%m-%d)

while [[ $start -le $end ]]
do
    echo $start
    start=$(date -d"$start + 1 day" +"%Y-%m-%d")

    # wget output file
    FILE=clearing_edit_deltas_$start.csv

    # username and password
    USER=dmorley4
    PASSWORD=Akuna@33\!3?$

    # wget download url
    URL="https://www.batstrading.com/cone/account/clearing_edit_deltas/download/?executingFirmId=ZOML&dt=$start"

    wget $URL --tries=1 --http-user=$USER --http-password=$PASSWORD --auth-no-challenge -O $FILE
    sleep .15
done