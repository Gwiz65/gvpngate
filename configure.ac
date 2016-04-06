dnl Process this file with autoconf to produce a configure script.
dnl
dnl/***************************************************************************
dnl *                                                                         * 
dnl * configure.ac                                                            *
dnl *                                                                         * 
dnl * Copyright (C) 2016 Gwiz <gwiz2009@gmail.com>                            *
dnl *                                                                         *     
dnl * gvpngate is free software: you can redistribute it and/or modify it     *
dnl * under the terms of the GNU General Public License as published by the   *
dnl * Free Software Foundation, either version 3 of the License, or           *
dnl * (at your option) any later version.                                     *
dnl *                                                                         *
dnl * gvpngate is distributed in the hope that it will be useful, but         *
dnl * WITHOUT ANY WARRANTY; without even the implied warranty of              *
dnl * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                    *
dnl * See the GNU General Public License for more details.                    *
dnl *                                                                         *
dnl * You should have received a copy of the GNU General Public License along *
dnl * with this program.  If not, see <https://www.gnu.org/licenses/>.        *
dnl *                                                                         * 
dnl ***************************************************************************/
AC_INIT(gvpngate, 0.48)
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.11])
AM_SILENT_RULES([yes])
AC_PROG_CC
LT_INIT
PKG_CHECK_MODULES(GVPNGATE, uuid [gtk+-3.0 >= 2.90.3])
AC_OUTPUT([Makefile src/Makefile])