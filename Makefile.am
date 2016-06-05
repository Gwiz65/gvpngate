# Process this file with automake to produce Makefile.in
#
#/***************************************************************************
# *                                                                         * 
# * Makefile.am                                                             *
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

SUBDIRS = src 

gvpngatedocdir = ${prefix}/doc/gvpngate
gvpngatedoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS\
	LICENSE

gvpngatedesktopdir = $(prefix)/share/applications
gvpngatedesktop_DATA = gvpngate.desktop

gvpngateicondir = $(prefix)/share/icons
gvpngateicon_DATA = gvpngate.png

gvpngateuiicondir = $(pkgdatadir)/ui
gvpngateuiicon_DATA = gvpngate.png

EXTRA_DIST = \
	$(gvpngatedoc_DATA)\
	$(gvpngatedesktop_DATA)\
	$(gvpngateicon_DATA)

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(gvpngatedocdir)
