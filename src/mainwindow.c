/***************************************************************************
 *                                                                         * 
 * mainwindow.c                                                            *
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
#include "gvpngate.h"

//////////////////////////////////////////////
//                                          //
//     *** Callbacks for MainWindow ***     //
//                                          //
//////////////////////////////////////////////

/****************************************************************************
 *                                                                          *
 * Function: on_menuitemHelp_activate                                       *
 *                                                                          *
 * Purpose :                                                                *
 *                                                                          *
 ****************************************************************************/
void on_menuitemHelp_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	ShowAboutDialog();
}

/****************************************************************************
 *                                                                          *
 * Function: on_menuitemConnect_activate                                    *
 *                                                                          *
 * Purpose :                                                                *
 *                                                                          *
 ****************************************************************************/
void on_menuitemConnect_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	Statusbar_Message("Attempting to connect to selected VPN.  Please wait...");
	g_timeout_add (100, CreateConnection, NULL);

}

/****************************************************************************
 *                                                                          *
 * Function: on_menuitemRefresh_activate                                    *
 *                                                                          *
 * Purpose :                                                                *
 *                                                                          *
 ****************************************************************************/
void on_menuitemRefresh_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	Statusbar_Message("Refreshing vpn list.  Please wait...");
	g_timeout_add (100, Get_Vpn_List_File, NULL);
}

/****************************************************************************
 *                                                                          *
 * Function: on_vpnlisttreeview_row_activated                               *
 *                                                                          *
 * Purpose : handle row double-click                                        *
 *                                                                          *
 ****************************************************************************/
void on_vpnlisttreeview_row_activated (GtkTreeView *tree_view, GtkTreePath *path,
                                       GtkTreeViewColumn *column, gpointer user_data)
{
	Statusbar_Message("Attempting to connect to selected VPN.  Please wait...");
	g_timeout_add (100, CreateConnection, NULL);
}
/****************************************************************************
 *                                                                          *
 * Function: on_treeview_selection1_changed                                 *
 *                                                                          *
 * Purpose : handle treeview selection changed signal                       *
 *                                                                          *
 ****************************************************************************/
void on_treeview_selection1_changed (GtkTreeSelection *treeselection, gpointer user_data)
{
	VpnListSelectionChanged();
}

/****************************************************************************
 *                                                                          *
 * Function: on_toolbutton_connect_clicked                                  *
 *                                                                          *
 * Purpose : handle connect button clicked signal                           *
 *                                                                          *
 ****************************************************************************/
void on_toolbutton_connect_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	Statusbar_Message("Attempting to connect to selected VPN.  Please wait...");
	g_timeout_add (100, CreateConnection, NULL);
}

/****************************************************************************
 *                                                                          *
 * Function: on_toolbutton_refresh_clicked                                  *
 *                                                                          *
 * Purpose : handle refresh button clicked signal                           *
 *                                                                          *
 ****************************************************************************/
void on_toolbutton_refresh_clicked (GtkToolButton *toolbutton, gpointer user_data)
{
	Statusbar_Message("Refreshing vpn list.  Please wait...");
	g_timeout_add (100, Get_Vpn_List_File, NULL);
}
