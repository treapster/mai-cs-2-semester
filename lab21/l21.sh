#!/bin/bash 

let seek=0
let seek_entered=0
let prefix_entered=0
let test=0
let decimal=0
let multiplier=1
let help=0
unit="b"
prefix=""
let show=0

for param in $@
do
if [[ $param =~ ^[\-].* ]]
then
case $param in
	-help | -h)
		let help=1
		;;
	-test)
		let test=1
		;;
	-show)
		let show=1
		;;
	-d)
		let decimal=1
		;;
	*)
		echo "$param is unknown function"
		exit 0
		;;

esac
else
	if [[ $prefix_entered -eq 0 ]]
	then
		let prefix_entered=1
		prefix=$param
		continue
	fi

	if [[ $seek_entered -eq 1 ]]
	then
		echo "Too much arguments. Only [prefix] [size] required"
		exit 0
	fi
	if [[ $param =~ ^[0-9]+[BKMGbkmg]?$ ]]
	then
		seek_entered=1
		unit=$( grep -o -e  [BKMGbkmg] <<< $param )
		seek=$( grep -o -E  '^[0-9]+' <<< $param )
	else
		echo "$param is unknown size format"
		exit 0
	fi
fi
done
if [[ $help -eq 1 ]]
then
	echo -e "\n$0 searches for files beginning with PREFIX and deletes the biggest of them until the size of a directory reaches SIZE or there's no such files left.\
	\n\n syntax:\
	\n $0 [OPTIONS] [PREFIX] [SIZE][UNIT]\
	\n possible units:\
	\n B/b or omitted - bytes\
	\n K/k - kilobytes\
	\n M/m - megabytes\
	\n G/g - gigabytes\

	\n SIZE should always be after PREFIX and unit has to be after size without separators, while the OPTIONS positions don't really matter.\
	\n By default both PREFIX and SIZE are required, but with -test option SIZE can be omitted. Howewer, in this case all sizes of files will only be printed in bytes.\
	
	\n Avaiable options:\
	\n -h or -help - display this guide\
	\n -test - find matching files but don't delete\
	\n -show - show every matching file with -test option, or show only matching and deleted files without it.\
	\n -d - decimal - use powers of 10 instead of 2 for units. Kilobytes become 1000 instead of 1024, megabytes are considered 10^6 instead of 2^20 and etc. Without this option binary units are used."
	if [[ $# -gt 1 ]]
	then 
		echo -e "\n\nSome other parameters were passed besides -help. Would you like to execute $0 using them?(Y/n)"
		read answer
		if [[ $answer != "Y" ]]
		then
			exit 0
		fi
	else
		exit 0
	fi

fi
if [[ $prefix_entered -eq 0 ]]
then 
	echo "No prefix provided. Use -h or -help for more info."
	exit 0
fi
if [[ $seek_entered -eq 0 && $test -eq 0 ]]
then
	echo "No size provided. Size is required for non-testing runs. Use -h or -help for more info."
	exit 0
fi
if [[ $decimal -eq 1 ]]
then
	let factor=1000
else
	let factor=1024
fi

if [[ $unit == "k" || $unit == "K" ]]
then
	let multiplier=factor
fi
if [[ $unit == "m" || $unit == "M" ]]
then
	let multiplier=factor*factor
fi
if [[ $unit == "g" || $unit == "G" ]]
then
	let multiplier=factor*factor*factor
fi
let seek*=multiplier
if [[ $unit == "" ]]
then unit="b"
fi

export 	total=$(du . -s -b) 
total=$( grep -o -E  '^[0-9]+' <<< $total ) 
oldsize=$total
export files=0
export count=0

if [[ $show -eq 1 ]]
then
	if [[ $test -eq 1 ]]
	then
		shopt -s lastpipe
		find . -name "$prefix*" -type f  -exec du  -b {} + | sort -r -h  | while read -r size path ; do echo "$size $path"; let files+=$size; let count+=1; done
		let humanfiles=$files/multiplier
		let oldhuman=$oldsize/multiplier
		echo "directory size is $oldsize bytes($oldhuman $unit), $count files can be deleted totalling $files bytes ($humanfiles $unit)"
		exit 0
	fi
	if [[ $test -eq 0 ]]
	then
		shopt -s lastpipe
		find . -name "$prefix*" -type f  -exec du  -b {} + | sort -r -h  | while read -r size path ; do if [[ $total -lt $seek ]]; then break; else out=$(rm "$path"); if [[ -z $out ]]; then let total-=$size; let count+=1; let files+=$size; echo "$size $path"; fi fi; done
		let human=$total/multiplier
		let humanfiles=$files/multiplier
		let oldhuman=$oldsize/multiplier
		echo "directory size was $oldsize bytes($oldhuman $unit) and was reduced to $total bytes($human $unit) by deleting $count files totalling $files bytes($humanfiles $unit)"
		exit 0
	fi
fi
if [[ $show -eq 0 ]]
then
	if [[ $test -eq 1 ]]
	then
		shopt -s lastpipe
		find . -name "$prefix*" -type f  -exec du  -b {} + | sort -r -h  | while read -r size path ; do let files+=$size; let count+=1; done
		let humanfiles=$files/multiplier
		let oldhuman=$oldsize/multiplier
		echo "directory size is $oldsize bytes($oldhuman $unit), $count files can be deleted totalling $files bytes($humanfiles $unit)"
		exit 0
	fi
	if [[ $test -eq 0 ]]
	then
		shopt -s lastpipe
		find . -name "$prefix*" -type f  -exec du  -b {} + | sort -r -h  | while read -r size path ; do if [[ $total -lt $seek ]]; then break; else out=$(rm "$path"); if [[ -z $out ]]; then let total-=$size; let count+=1; let files+=size; fi; fi; done
		let human=$total/multiplier
		let humanfiles=$files/multiplier
		let oldhuman=$oldsize/multiplier
		echo "directory size was $oldsize bytes($oldhuman $unit) and was reduced to $total bytes($human $unit) by deleting $count files totalling $files bytes($humanfiles $unit)"
		exit 0
	fi
fi
