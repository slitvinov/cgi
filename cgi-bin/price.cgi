#!/bin/sh

list='btc
ltc
nmc
ppc
doge
grc
xpm
xrp
nxt
aur
dash
neo
mzc
xmr
tit
xvg
xlm
vtc
eth
etc
xno
usdt
firo
zec
bch
eos
ada
trx
ckb
algo
avax
shib
dot
deso
safemoon'

set -f
case $PATH_INFO in
    '' ) cat <<!
Content-Type: text/html

<!DOCTYPE html>
<html lang="en-US">
  <head>
    <meta charset="utf-8">
    <title>Price</title>
  </head>
  <body>
`for i in $list
do printf '    <a href="price.cgi/'$i'">'$i'</a><br>\n'
done`
  </body>
</html>
!
	 ;;
    *) t=/tmp/btc.$$
       trap 'rm $t; exit 1' 1 2 3 14 15
       if ! curl -m 5 -s 'https://min-api.cryptocompare.com/data/price?fsym='"${PATH_INFO#\/}"'&tsyms=USD' > $t
       then
	   cat <<'!'
Status: 400 Bad Request

curl failed
!
	   rm -f $t
	   exit 1
       fi
       cat <<!
Content-Type: text/plain

`awk '{sub(/^.*:/, ""); sub(/}$/, ""); print}' $t`
!
       rm $t
esac
