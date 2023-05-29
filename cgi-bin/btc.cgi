#!/bin/sh

t=/tmp/btc.$$
trap 'rm $t; exit 1' 1 2 3 14 15

if ! curl -m 5 -s http://api.coindesk.com/v1/bpi/currentprice.json  > $t
then
    cat <<!
Status: 400 Bad Request

curl failed
!
   rm -f $t
   exit 1
fi

printf 'Content-Type: text/plain\n\n'
awk -v RS=, -v FS=: '$1 == "\"rate_float\"" {sub(/}$/, "", $2); print $2; exit}' $t
rm $t
