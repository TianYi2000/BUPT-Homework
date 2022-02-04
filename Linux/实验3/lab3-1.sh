#!/bin/bash
get_time()
{
	time=$(date +"%F %H:%M")
}
wait_until_next59s()
{
	eval $(date 'second=%S;')
	wait_t=$(expr 119 - $second)
	sleep $t
}
get_netstat()
{
	pre_recv=$recv
	pre_send=$send
	recv=$(expr 'netstat --statistic' : '^.* \([0−9]*\) segments received$')
	send=$(expr 'netstat --statistic' : '^.* \([0−9]*\) segments sent out$')
}
cal_delta()
{
	get_netstat
	d_recv=$(expr $recv - $pre_recv)
	d_send=$(expr $send - $pre_send)
	pre_total=$total
	total=$(expr $d_send + $d_recv)
}
judge_change()
{
	if [ "$pre_total" = "" ]
	then
		End=' '
	elif [ $(expr $total - $pre_total) -gt 10 ]
    then
        End='+'
    elif [ $(expr $total - $pre_total) -lt -10 ]
    then
        End='-'
    else
        End=' '
    fi
}
init()
{
	total=""
	wait_until_next59s
	get_netstat
}
print_result()
{
	printf "%-16s%-8s%-8s%-8s%-8s\n" "$time" "$receive" "$send" "$total" "$End"
}
while true
do
	wait_until_next59s
	get_time
	cal_delta
	judge_change


