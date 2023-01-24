#! /bin/sh

if [ $# -ne 1 ]
then
	echo "Usage : $0 [m | s | q]" >& 2
	exit 1
fi

clesIpc=`ipcs -"$1" | grep 0x | cut -f2 -d' '`

for cle in $clesIpc
do
 ipcrm -$1 $cle
done

exit 0