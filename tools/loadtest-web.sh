
COUNT=8
IP=192.168.2.187
TESTFILE=otmonitor.js

date
let n=1
while [ $n -lt ${COUNT} ]
do
	curl http://${IP}/${TESTFILE}
	sleep 1
	let n=n+1
done > /dev/null

date
let n=1
while [ $n -lt ${COUNT} ]
do
	curl http://${IP}/${TESTFILE}
	let n=n+1
done > /dev/null

date
let n=1
while [ $n -lt ${COUNT} ]
do
	curl http://${IP}/${TESTFILE} &
	let n=n+1
done > /dev/null
wait

date
