#!/bin/bash

NB_FNAME="cast.5428E.txt"
NB_OUTFNAME="$NB_FNAME.xml"

TO_BE_CLOSED="-1"
TO_BE_CLOSED_WITH="<empty>"

# Let's assumed that amount of sub-ordered pages are not more than ..
SiteDepth=7

declare -a Finalize

declare -a FinalizeWith


Init()
{
	for ((i=0; i<SiteDepth; i++));
	do
		Finalize[$i]="<no>"
		FinalizeWith[$i]="<empty>"
	done
}

DbgShow()
{
	for ((i=0; i<SiteDepth; i++));
	do
		echo " $i : ${Finalize[$i]} " >>$NB_OUTFNAME
	done
}


StartNodeI()
{
TA=$1
CSTR=$2

	CSTR="${CSTR// /_}"

	for (( t=0; t < ($TA); t++ ))
	do
		printf "\t">>$NB_OUTFNAME
	done

	#OBSOLETE: echo  "<node name=\"$CSTR\">">>$NB_OUTFNAME
	echo  "<$CSTR>">>$NB_OUTFNAME

	Finalize[$t]="<yes>"
	FinalizeWith[$t]="$CSTR"
}


CloseNodeI()
{
TBS=$1

	for (( i=0; i < ($TBS); i++ ))
	do
		printf "\t">>$NB_OUTFNAME
	done

	
	echo  "</${FinalizeWith[$i]}>">>$NB_OUTFNAME
	#OBSOLETE: echo  "</node>">>$NB_OUTFNAME
	#OBSOLETE: echo  "</node> <!-- name=\"${FinalizeWith[$i]}\" -->">>$NB_OUTFNAME

	Finalize[$i]="<no>"
	FinalizeWith[$i]="<empty>"


}

CloseNodeK()
{
TBS=$1

	for (( k=0; k < ($TBS); k++ ))
	do
		printf "\t">>$NB_OUTFNAME
	done

	
	echo  "</${FinalizeWith[$k]}>">>$NB_OUTFNAME
	#OBSOLETE: echo  "</node>">>$NB_OUTFNAME
	#OBSOLETE: echo  "</node> <!-- name=\"${FinalizeWith[$k]}\" -->">>$NB_OUTFNAME	

	Finalize[$k]="<no>"
	FinalizeWith[$k]="<empty>"


}

CloseBulkNodesI()
{
TABS=$1

	for (( s=($SiteDepth)-1; s >= ($TABS); s-- ))
	do

		if [ "${Finalize[$s]}" == "<yes>" ]; then

			CloseNodeK $s


		fi

	done
}

	Init

	./compute_tabs < ./$NB_FNAME 1> $NB_FNAME.tabs

	echo  "<?xml version=\"1.0\"?>">$NB_OUTFNAME
	#echo  "<!DOCTYPE XML-shaped Planar Graph>">>$NB_OUTFNAME
	

	while read line; do

		rawENTRY=($(echo $line));

		j=${rawENTRY[0]}
		y=${rawENTRY[@]:1}

		if [[ $j -gt $TO_BE_CLOSED ]]; then

			StartNodeI $j "$y"

			TO_BE_CLOSED=$j

		elif [[ $j -eq $TO_BE_CLOSED ]]; then

			CloseNodeI $j

			StartNodeI $j "$y"

			TO_BE_CLOSED=$j

		elif [[ $j -lt $TO_BE_CLOSED ]]; then

			# close all, incl. last 
			CloseBulkNodesI $j


			# start new XML grp.
			StartNodeI $j "$y"

			TO_BE_CLOSED=$j

		fi


	done <./$NB_FNAME.tabs

	# close all recently opened
	CloseBulkNodesI "0"


	rm -f $NB_FNAME.tabs

exit 0
