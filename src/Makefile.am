## Process this file with automake to produce Makefile.in
##
#/***************************************************************************
# *                                                                         * 
# * src/Makefile.am                                                         *
# *                                                                         * 
# * Copyright (C) 2016 Gwiz <gwiz2009@gmail.com>                            *
# *                                                                         *     
# * gvpngate is free software: you can redistribute it and/or modify it     *
# * under the terms of the GNU General Public License as published by the   *
# * Free Software Foundation, either version 3 of the License, or           *
# * (at your option) any later version.                                     *
# *                                                                         *
# * gvpngate is distributed in the hope that it will be useful, but         *
# * WITHOUT ANY WARRANTY; without even the implied warranty of              *
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                    *
# * See the GNU General Public License for more details.                    *
# *                                                                         *
# * You should have received a copy of the GNU General Public License along *
# * with this program.  If not, see <https://www.gnu.org/licenses/>.        *
# *                                                                         * 
# ***************************************************************************/

uidir = $(pkgdatadir)/ui
ui_DATA = gvpngate.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GVPNGATE_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gvpngate \
	gvpngate_suid

gvpngate_SOURCES = \
	gvpngate.h \
	gvpngate.c \
	mainwindow.c	

gvpngate_LDFLAGS = \
	-Wl,--export-dynamic

gvpngate_LDADD = $(GVPNGATE_LIBS)

gvpngate_suid_SOURCES =  \
	gvpngate_suid.c


EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)

# set bin suid
install-exec-hook:
	chmod 4755 $(bindir)/gvpngate_suid


