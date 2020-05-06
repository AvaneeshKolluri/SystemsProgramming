###############################################################################
# Name: Avaneesh Kolluri
# Pledge: I pledge my honor that I have abided by the Stevens Honor System.
# Date: February 6, 2020
# Simple bash script that unzips a file containing zip files of student
# submissions and a single grading script.
# It parses the first and last names from the submission zip files,
# creates folders for each student with the name "last_first", and grades the
# submissions. 
###############################################################################
#!/bin/bash

size_flag=0

readonly jvar=~/.junk

displayUsage(){
cat << displayUsage
Usage: $(basename "$0") [-hlp] [list of files]
	-h: Display help.
	-l: List junked files.
	-p: Purge all files.
	[list of files] with no other arguments to junk those files.
displayUsage
}
listJunkedFiles=0
purgeAllFiles=0

declare -a filenames

shift "$((OPTIND-1))"
index=0
for f in $@; do
	filenames[$index]="$f"
	((++index))
done

while getopts ":hlp" option; do
	case "$option" in
		h) size_flag=$(($size_flag+1))
			displayUsage;;
		l) size_flag=$(($size_flag+1))
			listJunkedFiles=1;;
		p) size_flag=$(($size_flag+1))
			purgeAllFiles=1;;
		?) size_flag=$(($size_flag+1))
			printf "Error: Unknown option '-%s' .\n" $OPTARG >&2
			displayUsage
			exit 1;;
	esac
done

if [ $size_flag -eq 0 ]; then
	if [ $index -eq 0 ]; then
		displayUsage
		exit 0
	fi
fi


if [ $size_flag -gt 1 ]; then
	printf "Error: Too many options enabled. \n"
	displayUsage
	exit 1
fi


if [ $size_flag -eq 1 ]; then
	if [ $index -gt 1 ]; then
		printf "Error: Too many options enabled. \n"
		displayUsage
		exit 1
	fi
fi


if [ $listJunkedFiles -eq 1 ]; then
	ls -lAF "$jvar"
	exit 0
fi

if [ $purgeAllFiles -eq 1 ]; then
	rm -rf $jvar/* $jvar/.* 2>/dev/null
	exit 0
fi



#Part b
if [ ! -d "$jvar" ]; then  
	mkdir "$jvar"
	exit 0
fi

#part c

#{$filenames[@]}
for each in ${filenames[@]}; do
	if [ ! -e "$each" ] ; then
		printf "Warning: $each not found. \n"
	else
		mv $each $jvar
	fi
done



#part e
exit 0
