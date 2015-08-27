# Input file to generate <./configure>
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

AC_INIT(trafinj, version-1.0)

AC_MSG_NOTICE([configuring tr.inj. lib])

echo "Testing a compiler"
AC_PROG_CC
AC_LANG(C)

cu_in_h=1
AC_CHECK_HEADERS(curl/curl.h, [], [cu_in_h=0])
if test $cu_in_h == 0
then
	AC_MSG_NOTICE([curl/curl.h is missing.])
	echo 1>&2 "** check libcurl headers"
	exit -1
else
	AC_MSG_NOTICE([curl/curl.h is present])
fi


cu_in_l=1
AC_CHECK_LIB(curl, curl_easy_init, [], [cu_in_l=0])
if test $cu_in_l == 0
then
	AC_MSG_NOTICE([libcurl.a items fail.])
	echo 1>&2 "** install libcurl"
	exit -1

else
	AC_MSG_NOTICE([libcurl.a items found working.])
fi


go_in_h=1
AC_CHECK_HEADERS(getopt.h, [], [go_in_h=0])
if test $go_in_h == 0
then
	AC_MSG_NOTICE([getopt.h is missing.])
	echo 1 >&2 "** check libc headers"
	exit -1
else
	AC_MSG_NOTICE([getopt.h is present])
fi

go_in_l=1
AC_CHECK_LIB(c, getopt_long, [], [go_in_l=0])
if test $go_in_l == 0
then
	AC_MSG_NOTICE([getopt items fail.])
	echo 1 >&2 "** check libc library"
	exit -1
else
	AC_MSG_NOTICE([getopt items found working])
fi


xm_in_h=1
AC_CHECK_HEADERS(libxml/parser.h, [], [xm_in_h=0])
if test $xm_in_h == 0
then
	AC_MSG_NOTICE([libxml/parser.h is missing.])
	echo 1>&2 "** check libxml2 headers"
	exit -1
else
	AC_MSG_NOTICE([libxml/parser.h is present])
fi



xm_in_l=1
AC_SEARCH_LIBS(xmlFreeDoc, xml2, [], [xm_in_l=0], [])
if test $xm_in_l == 0
then
	AC_MSG_NOTICE([libxml2.a items fail.])
	echo 1>&2 "** install libxml2"
	exit -1
else
	AC_MSG_NOTICE([libxml2.a items found working.])
fi



AC_SUBST(BUILD_CURL, $BUILD_CURL)
AC_SUBST(BUILD_XML2, $BUILD_XML2)

AC_SUBST(PLATFORM, $(uname -s))

AC_SUBST(INSTALL_OPTIONS, $INSTALL_OPTIONS)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

