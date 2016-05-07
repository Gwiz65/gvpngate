/***************************************************************************
 *                                                                         * 
 * gvpngate.h                                                              *
 *                                                                         * 
 * Copyright (C) 2016 Gwiz <gwiz2009@gmail.com>                            *
 *                                                                         *     
 * gvpngate is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation, either version 3 of the License, or           *
 * (at your option) any later version.                                     *
 *                                                                         *
 * gvpngate is distributed in the hope that it will be useful, but         *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                    *
 * See the GNU General Public License for more details.                    *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program.  If not, see <https://www.gnu.org/licenses/>.        *
 *                                                                         * 
 ***************************************************************************/
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <uuid/uuid.h>

/****************************
 *        Defines           *
 ****************************/
// local or system ui file
#define UI_FILE PACKAGE_DATA_DIR"/ui/gvpngate.ui"
//#define UI_FILE "src/gvpngate.ui"

// define version
#define GVPNGATE_VERSION "0.54"

/****************************
 * Global Function Declares *
 ****************************/
void ShowAboutDialog (void);
void VpnListSelectionChanged (void);
gboolean CreateConnection (gpointer);
void Statusbar_Message(gchar*);
gboolean Get_Vpn_List_File(gpointer);
