#!/bin/bash
Root_url='bing.ioliu.cn'

touch_dir()
{
	if [ ! -d  $1 ]
	then
		mkdir $1
	fi
}

get_img()
{
	Url=$Root_url$1
	Discribe=$2
	Date=$3
	File_name="$Date $Discribe.jpg"
	wget "$Url" -O "detail_html/$File_name.html"
	Url_img=$(cat "detail_html/$File_name.html" | awk '/data-progressive/ {print $0}' | \
		sed 's/.*data-progressive="\(.*\)?imageslim.*/\1/g')
	if [ ! -f "bing/$File_name" ]
	then
		wget "$Url_img" -O "tmp/$File_name"
		if [ $? = 0 -a ! -f "bing/$File_name" ]
		then
			mv "tmp/$File_name" "bing/$File_name"
		else
			rm "tmp/$File_name"
		fi
	fi
}

get_img_rand()
{
	Date=$1
	Url=$2
	Discribe=""
	for i in $(seq 3 $#)
	do
		Discribe="$Discribe ${!i}"
	done
	if [ $(expr length "$Discribe") -gt 100 ]
	then
		Discribe=${Discribe:0:100}
	fi
	File_name="$Date $Discribe.jpg"
	Exist=0
	wget "$Url" -O "tmp/$File_name"
	if [ $? = 0 ]
	then
		for file in bing/*
		do
			diff "tmp/$File_name" "$file" > /dev/null
			if [ $0 == 0 ]
			then
				Exist=1
				break
			fi	
		done
		if [ $Exist = 0 ]
		then
			cp "tmp/$File_name" "bing/$File_name"
		fi
	fi
	rm "tmp/$File_name"
}
rand_loop()
{
	for i in $(seq $1)
	do
		wget "https://bing.ioliu.cn/v1/rand?type=json" -O "rand.html"
		args=$(cat "rand.html" | sed "s/,\"/\n/g" | sed "s/\":\"/ /g" | sed "s/\"//g" | \
			sed "s/{/\n/g" | sed "s/(.*//g" | awk -f fetch_rand.awk)
		get_img_rand $args
		sleep 0.5
	done
}

if [ $# = 2 ]
then
	if [ $1 = 'rand' ]
	then
		rand_loop $2
	fi
	START=$1
	END=$2
elif [ $# = 0 ]
then
	START=1
	END=126
else
	echo "Usage: $0 <begin> <end>"
	exit 1
fi

touch_dir index_html
touch_dir detail_html
touch_dir bing
touch_dir tmp

for index in $(seq $START $END)
do
	index_url=$Root_url/?p=$index
	wget "$index_url" -O "index_html/index$index.html"
	cat index_html/index$index.html | sed -e 's/></\n/g' | awk -f fetch.awk > index_html/fetched_index$index.html

	while read line
	do
		Clean_line=$(echo "${line}" | sed 's/^.*href=\"\([^"]*\)\"|/\1 /g' | \
			sed 's/h3>\([^ ]*\).*|/\1 /g' | sed 's/em class="t">\(.*\)<.*/\1/g')
		get_img $Clean_line
	done < index_html/fetched_index$index.html	
	
done

