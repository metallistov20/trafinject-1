#!/bin/bash
#
# (C) Copyright 2015, TP-Link Inc, konstantin.mauch@tp-link.com
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA


#Input file
NB_FNAME="cast.5428E.txt"

# Output file
NB_OUTFNAME="$NB_FNAME.xml"

TO_BE_CLOSED="-1"
TO_BE_CLOSED_WITH="<empty>"

# Let's assumed that amount of sub-ordered pages are not more than ..
SiteDepth=7

# Array to keep a key word (<yes>, <no>) to tell if the tag to be finalized
declare -a Finalize

# Array to keep a tag name to finalize the tag
declare -a FinalizeWith


# Initialize arr. elements
Init()
{
	for ((i=0; i<SiteDepth; i++));
	do
		Finalize[$i]="<no>"
		FinalizeWith[$i]="<empty>"
	done
}

# Debug preview of initialized arr. elements
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

	# Processing special chars: replace spaces characters with '_'
	CSTR="${CSTR// /_}"

	# Processing special chars: put prefix before starting digits
	if [[ "$CSTR" = [0-9]* ]]; then
	        CSTR="n$CSTR"
	fi

	# Processing special chars: replace '/' characters with '_'
	CSTR="${CSTR////_}"

	# Put as much <tabs> as required on this level
	for (( t=0; t < ($TA); t++ ))
	do
		printf "\t">>$NB_OUTFNAME
	done

	# Put Tag name
	echo  "<$CSTR>">>$NB_OUTFNAME
	#OBSOLETE: echo  "<node name=\"$CSTR\">">>$NB_OUTFNAME

	# Define Tag must be closed
	Finalize[$t]="<yes>"

	# Define word close tag with
	FinalizeWith[$t]="$CSTR"
}


CloseNodeI()
{
TBS=$1

	# Put as much <tabs> as required on this level
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

	# Put Tag finalization keyword 
	echo  "</${FinalizeWith[$k]}>">>$NB_OUTFNAME

	#OBSOLETE: echo  "</node>">>$NB_OUTFNAME
	#OBSOLETE: echo  "</node> <!-- name=\"${FinalizeWith[$k]}\" -->">>$NB_OUTFNAME	

	# define tag on this level should not be finalized anymore 
	Finalize[$k]="<no>"

	# define phony finalization keyword
	FinalizeWith[$k]="<empty>"


}

CloseBulkNodesI()
{
TABS=$1

	# Throughout all possible instances of tags
	for (( s=($SiteDepth)-1; s >= ($TABS); s-- ))
	do
		# If the tag to be finalized
		if [ "${Finalize[$s]}" == "<yes>" ]; then

			# then finalaze it
			CloseNodeK $s

		fi

	done
}

	# Initialized arrays 'Finalize', 'FinalizeWith'
	Init

	# Put on first position an amounty of <tabs> present in each string
	./compute_tabs < ./$NB_FNAME 1> $NB_FNAME.tabs

	# Put XML-caption (useless a by now)
	echo  "<?xml version=\"1.0\"?>">$NB_OUTFNAME

	#echo  "<!DOCTYPE XML-shaped Planar Graph>">>$NB_OUTFNAME

	# for each string in file '$NB_OUTFNAME'
	while read line; do

		# get string into array
		rawENTRY=($(echo $line));

		# get amount of tabs (so we know hierarchy level of current node)
		j=${rawENTRY[0]}

		# get rest of the array into string
		y=${rawENTRY[@]:1}

		# if we're starting new node currently 
		if [[ $j -gt $TO_BE_CLOSED ]]; then

			# start new XML node
			StartNodeI $j "$y"

			# memorize this node is subject for closing
			TO_BE_CLOSED=$j

		# if current node is of the same hierarchy as previous one
		elif [[ $j -eq $TO_BE_CLOSED ]]; then

			# then close previous XML node
			CloseNodeI $j

			# start new XML node
			StartNodeI $j "$y"

			# memorize current node is subject for closing
			TO_BE_CLOSED=$j

		# if current node has uppder hierarchy degree than previous one
		elif [[ $j -lt $TO_BE_CLOSED ]]; then

			# close all previously started nodes
			CloseBulkNodesI $j

			# start new XML node
			StartNodeI $j "$y"

			# memorize this node is subject for closing
			TO_BE_CLOSED=$j

		fi
	
	done <./$NB_FNAME.tabs

	# close all XML opened during processing last string of file '$NB_FNAME.tabs'
	CloseBulkNodesI "0"

	# remove temporary file '$NB_FNAME.tabs'
	rm -f $NB_FNAME.tabs

exit 0
