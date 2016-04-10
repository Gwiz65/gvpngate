/***************************************************************************
 *                                                                         * 
 * gvpngate.c                                                              *
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

/****************************
 *        Structures        *
 ****************************/
struct stat st = {0};

/****************************
 *       Variables          *
 ****************************/
guint  MainWin_StatusBarID;
GtkWidget *MainWindow;
GtkWidget *MainWin_StatusBar;
GtkListStore *VPN_List;

/****************************
 *    Function Declares     *
 ****************************/
void Destroy_Main_Window (GtkWidget*, gpointer);
GtkWidget* Create_Main_Window (void);
int main (int argc, char *argv[]);

/****************************************************************************
 *                                                                          *
 * Function: ShowAboutDialog                                                *
 *                                                                          *
 * Purpose :                                                                *
 *                                                                          *
 ****************************************************************************/
void ShowAboutDialog (void)
{
	gint result = gtk_dialog_run (GTK_DIALOG (AboutBox));

	switch (result)
	{
	case GTK_RESPONSE_ACCEPT:
		break;
	default:
		gtk_widget_hide(AboutBox);
		break;
	}
}

/****************************************************************************
 *                                                                          *
 * Function: VpnListSelectionChanged                                        *
 *                                                                          *
 * Purpose : handle treeview selection changed signal                       *
 *                                                                          *
 ****************************************************************************/
void VpnListSelectionChanged (void)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *ipaddress = NULL;
	gchar *port = NULL;
	gchar *cipher = NULL;
	gchar *auth = NULL;
	gchar *device = NULL;
	gchar *protocol = NULL;

	if (gtk_tree_selection_get_selected
	    (gtk_tree_view_get_selection
	     (GTK_TREE_VIEW(VPN_List_Treeview)), &model, &iter))
	{
		// a row has been selected
		// light up connect button
		gtk_widget_set_sensitive (MainWin_Menu_Connect, TRUE);
		gtk_widget_set_sensitive (MainWin_TB_Connect, TRUE);
		{
			gchar *decoded = NULL;
			gchar *config_data = NULL;
			gsize out_len;
			gboolean getline_loop = TRUE;
			gint linepos = 0;  //first char of current line

			// get info from config data
			gtk_tree_model_get (model, &iter, 14, &config_data, -1);
			decoded = (gchar*)g_base64_decode(config_data, &out_len);
			while (getline_loop)
			{
				gint ctr = 0;
				gchar *line = NULL;

				while ((decoded[linepos+ctr] != '\0') &&
				       (decoded[linepos+ctr] != '\n')) ctr++;
				if (decoded[linepos+ctr] == '\0') 
				{
					if (ctr > 0) 
						line = g_strndup (decoded+linepos, ctr-1);
					getline_loop = FALSE; 
				}
				else
					line = g_strndup (decoded+linepos, ctr);
				if (ctr > 0)
				{
					// get intreasting info 
					// ip & port
					if (!strncmp(line, "remote ", 7))
					{
						gint ctr = 0;
						gint ctr2 = 0;

						while ((line[7+ctr] != '\0') &&
						       (line[7+ctr] != ' ')) ctr++;
						if (line[7+ctr] == '\0') ctr--;
						ipaddress = g_strndup (line+7, ctr);
						ctr++;
						while ((line[7+ctr+ctr2] != '\0') &&
						       (line[7+ctr+ctr2] != ' ')) ctr2++;
						if (line[7+ctr+ctr2] == '\0') ctr2--;
						port = g_strndup(line+7+ctr, ctr2);
					}
					// cipher
					if (!strncmp(line, "cipher ", 7))
					{
						gint ctr = 0;

						while ((line[7+ctr] != '\0') &&
						       (line[7+ctr] != ' ')) ctr++;
						if (line[7+ctr] == '\0') ctr--;
						cipher = g_strndup (line+7, ctr);
					}
					// auth
					if (!strncmp(line, "auth ", 5))
					{
						gint ctr = 0;

						while ((line[5+ctr] != '\0') &&
						       (line[5+ctr] != ' ')) ctr++;
						if (line[5+ctr] == '\0') ctr--;
						auth = g_strndup (line+5, ctr);
					}
					// device
					if (!strncmp(line, "dev ", 4))
					{
						gint ctr = 0;

						while ((line[4+ctr] != '\0') &&
						       (line[4+ctr] != ' ')) ctr++;
						if (line[4+ctr] == '\0') ctr--;
						device = g_strndup (line+4, ctr);
					}
					// protocol
					if (!strncmp(line, "proto ", 6))
					{
						gint ctr = 0;

						while ((line[6+ctr] != '\0') &&
						       (line[6+ctr] != ' ')) ctr++;
						if (line[6+ctr] == '\0') ctr--;
						protocol = g_strndup (line+6, ctr);
					}
					// testing purposes - displays config data
					//g_print("\n");
					//g_print(line);
				}
				linepos = linepos + ctr + 1;
				g_free(line);
			}
			g_free(decoded);
			g_free(config_data);
		}
		// set info label1
		{
			gchar *name = NULL;
			gchar *country = NULL;
			gchar *labelstr = NULL;

			gtk_tree_model_get (model, &iter, 0, &name, -1);
			gtk_tree_model_get (model, &iter, 5, &country, -1);
			labelstr = g_strconcat (name, "\n", country, NULL);
			gtk_label_set_markup (GTK_LABEL(MainWin_Info_1), labelstr);
			g_free(name);
			g_free(country);
			g_free(labelstr);
		}	
		// set info label2
		{
			gchar *labelstr = NULL;

			labelstr = g_strconcat (ipaddress, "\n", port, NULL);
			gtk_label_set_markup (GTK_LABEL(MainWin_Info_2), labelstr);
			g_free(labelstr);
		}	
		// set info label3
		{
			gchar *labelstr = NULL;

			labelstr = g_strconcat (cipher, "\n", auth, NULL);
			gtk_label_set_markup (GTK_LABEL(MainWin_Info_3), labelstr);
			g_free(labelstr);
		}	
		// set info label4
		{
			gchar *labelstr = NULL;

			labelstr = g_strconcat (device, "\n", protocol, NULL);
			gtk_label_set_markup (GTK_LABEL(MainWin_Info_4), labelstr);
			g_free(labelstr);
		}	
	}
	else
	{
		// no row selected
		// grey out connect menu item
		gtk_widget_set_sensitive (MainWin_Menu_Connect, FALSE);
		// grey out connect button & menu
		gtk_widget_set_sensitive (MainWin_TB_Connect, FALSE);
		// clear info labels
		gtk_label_set_markup (GTK_LABEL(MainWin_Info_1), NULL); 
		gtk_label_set_markup (GTK_LABEL(MainWin_Info_2), NULL);
		gtk_label_set_markup (GTK_LABEL(MainWin_Info_3), NULL);
		gtk_label_set_markup (GTK_LABEL(MainWin_Info_4), NULL);
	}
	g_free(ipaddress);
	g_free(port);
	g_free(cipher);
	g_free(auth);
	g_free(device);
	g_free(protocol);
}

/****************************************************************************
 *                                                                          *
 * Function: CreateConnection                                               *
 *				                                                            *
 * Purpose : handle connect button clicked signal                           *
 *                                                                          *
 ****************************************************************************/
gboolean CreateConnection (gpointer data)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *cmdstr = NULL;
	gchar *vpnname = NULL;
	gchar *ipaddress = NULL;
	gchar *port = NULL;
	gchar *cipher = NULL;
	gchar *auth = NULL;
	gchar *device = NULL;
	gchar *protocol = NULL;
	gchar *nm_id = NULL;
	gchar *filestr = NULL;
	gboolean bWritingCrt = FALSE;
	FILE *crtfile;
	FILE *connectfile;
	gint ret = 0;
	gint status;
	pid_t pid;

	Statusbar_Message("Attempting to connect to selected VPN. Please wait...");
	// collect needed info from selected row
	if (gtk_tree_selection_get_selected
	    (gtk_tree_view_get_selection
	     (GTK_TREE_VIEW(VPN_List_Treeview)), &model, &iter))
	{
		// a row has been selected
		// get info from config data
		gchar *decoded = NULL;
		gchar *config_data = NULL;
		gsize out_len;
		gboolean getline_loop = TRUE;
		gint linepos = 0;  //first char of current line

		gtk_tree_model_get (model, &iter, 14, &config_data, -1);
		decoded = (gchar*)g_base64_decode(config_data, &out_len);
		// get vpnname
		gtk_tree_model_get (model, &iter, 0, &vpnname, -1);
		// open crt file
		{
			gchar *tempstr = NULL;

			tempstr = g_strconcat(WorkDir, "/", vpnname, ".crt", NULL);
			crtfile = fopen(tempstr, "w");
			if (crtfile == NULL)
			{
				Statusbar_Message("Unable to write CRT file. This sucks.");
				return(FALSE);
			}
			g_free(tempstr);
		}
		// getline loop
		while (getline_loop)
		{
			gint ctr = 0;
			gchar *line = NULL;

			while ((decoded[linepos+ctr] != '\0') &&
			       (decoded[linepos+ctr] != '\n')) ctr++;
			if (decoded[linepos+ctr] == '\0') 
			{
				if (ctr > 0) 
					line = g_strndup (decoded+linepos, ctr-1);
				getline_loop = FALSE; 
			}
			else
				line = g_strndup (decoded+linepos, ctr);
			if (ctr > 0)
			{
				// get intreasting info 
				// ip & port
				if (!strncmp(line, "remote ", 7))
				{
					gint ctr = 0;
					gint ctr2 = 0;

					while ((line[7+ctr] != '\0') &&
					       (line[7+ctr] != ' ')) ctr++;
					if (line[7+ctr] == '\0') ctr--;
					ipaddress = g_strndup (line+7, ctr);
					ctr++;
					while ((line[7+ctr+ctr2] != '\0') &&
					       (line[7+ctr+ctr2] != ' ')) ctr2++;
					if (line[7+ctr+ctr2] == '\0') ctr2--;
					port = g_strndup(line+7+ctr, ctr2);
				}
				// cipher
				if (!strncmp(line, "cipher ", 7))
				{
					gint ctr = 0;

					while ((line[7+ctr] != '\0') &&
					       (line[7+ctr] != ' ')) ctr++;
					if (line[7+ctr] == '\0') ctr--;
					cipher = g_strndup (line+7, ctr);
				}
				// auth
				if (!strncmp(line, "auth ", 5))
				{
					gint ctr = 0;

					while ((line[5+ctr] != '\0') &&
					       (line[5+ctr] != ' ')) ctr++;
					if (line[5+ctr] == '\0') ctr--;
					auth = g_strndup (line+5, ctr);
				}
				// device
				if (!strncmp(line, "dev ", 4))
				{
					gint ctr = 0;

					while ((line[4+ctr] != '\0') &&
					       (line[4+ctr] != ' ')) ctr++;
					if (line[4+ctr] == '\0') ctr--;
					device = g_strndup (line+4, ctr);
				}
				// protocol
				if (!strncmp(line, "proto ", 6))
				{
					gint ctr = 0;

					while ((line[6+ctr] != '\0') &&
					       (line[6+ctr] != ' ')) ctr++;
					if (line[6+ctr] == '\0') ctr--;
					protocol = g_strndup (line+6, ctr);
				}
				// create crt file here by copying lines between <ca></ca>
				if (!strncmp(line, "</ca>", 5)) bWritingCrt = FALSE;
				if (bWritingCrt && (crtfile != NULL))
					fprintf (crtfile, "%s\n", line);
				if (!strncmp(line, "<ca>", 4)) bWritingCrt = TRUE;
				// testing purposes - displays config data
				//g_print("\n");
				//g_print(line);
			}
			linepos = linepos + ctr + 1;
			g_free(line);
		}
		// close crt file
		if (crtfile != NULL) fclose(crtfile);			
		g_free(decoded);
		g_free(config_data);
	}
	else
	{
		// no row selected
		// this shouldn't happen since connect button is only lit when
		// a row is selected
	}
	// create a connection file in the work directory
	{
		gchar *tempstr = NULL;

		tempstr = g_strconcat(WorkDir, "/", vpnname, NULL);
		connectfile = fopen(tempstr, "w");
		if (connectfile == NULL)
		{
			Statusbar_Message("Unable to write temp connection file. This sucks.");
			return(FALSE);
		}
		else
		{
			uuid_t uuid;
			char uuid_str[37];
			gchar *country = NULL;
			gint matchctr = 1;
			char buffer[10];
			gboolean bValidName = FALSE;
			
			// get info from selected row
			gtk_tree_model_get (model, &iter, 5, &country, -1);
			// see if this connection exists
			filestr = g_strconcat("/etc/NetworkManager/system-connections/", 
			                      vpnname, NULL);
			if (stat(filestr, &st) == 0) 
			{
				// use gvpngate_suid to delete it
				ret = 0;
				pid = fork();
				if (pid == 0)
				{
					execlp("gvpngate_suid", "gvpngate_suid", filestr, NULL);
					_exit(EXIT_FAILURE);
				}
				else if (pid < 0) ret = -1;
				else if (waitpid (pid, &status, 0) != pid)  ret = -1;
				// check for fail or exit status other than 0
				if (!((!ret) && WIFEXITED(status) && !WEXITSTATUS(status)))
				{
					// close connection file
					if (connectfile != NULL) fclose(connectfile);
					// remove connection file from workdir
					if (stat(connectfile, &st) == 0) remove(connectfile);
					Statusbar_Message("Gvpngate_suid failed to delete system file. This sucks.");
					return(FALSE);
				}
				sleep(1);
			}
			// get a list of connections
			cmdstr = g_strconcat(WorkDir, "/status", NULL);
			ret = 0;
			pid = fork();
			if (pid == 0)
			{
				int fd = open(cmdstr, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
				dup2(fd, STDOUT_FILENO);   // make stdout go to status file
				close(fd);
				execlp("nmcli", "nmcli", "-t", "-f", "NAME,TYPE", "con", NULL);
				_exit(EXIT_FAILURE);
			}
			else if (pid < 0) ret = -1;
			else if (waitpid (pid, &status, 0) != pid)  ret = -1;
			// check for fail or exit status other than 0
			if (!((!ret) && WIFEXITED(status) && !WEXITSTATUS(status)))
			{
				// close connection file
				if (connectfile != NULL) fclose(connectfile);
				// remove connection file from workdir
				if (stat(connectfile, &st) == 0) remove(connectfile);
				Statusbar_Message("Failed to get list of current connections. This sucks.");
				return(FALSE);
			}
			// get next available nm id name
			while(!bValidName)
			{
				FILE *statusfile;
				gchar *line = NULL;
				gchar *tempstr = NULL;
				size_t len = 0;
				size_t read;

				bValidName = TRUE;
				snprintf(buffer, 10, " %d", matchctr);
				nm_id = g_strconcat(country, buffer, NULL);
				tempstr = g_strconcat(WorkDir, "/status", NULL);
				statusfile = fopen(tempstr, "r");
				if (statusfile == NULL)
				{
					// close connection file
					if (connectfile != NULL) fclose(connectfile);
					// remove connection file from workdir
					if (stat(connectfile, &st) == 0) remove(connectfile);
					Statusbar_Message("Unable to read status file. This sucks.");
					return(FALSE);
				}
				else
				{
					// getline loop
					while ((read = getline(&line, &len, statusfile)) != -1) 
					{
						gint ctr = 0;
						gint pos = 0;
						gchar *token1 = NULL;
						gchar *token2 = NULL;
						gint tok1_len = 0;
						
						//get token1
						while ((line[ctr] != ':') && 
						       (line[ctr] != '\0') &&
						       (line[ctr] != '\n')) ctr++;
						if (line[ctr] == '\0') ctr--;
						if (line[ctr] == '\n') ctr--;
						if (ctr > 0) token1 = g_strndup (line, ctr);
						tok1_len =ctr;
						pos = ctr + 1;
						//get token2
						ctr = 0;
						while ((line[ctr+pos] != ':') && 
						       (line[ctr+pos] != '\0') &&
						       (line[ctr+pos] != '\n')) ctr++;
						if (line[ctr+pos] == '\0') ctr--;
						if (line[ctr+pos] == '\n') ctr--;
						token2 = g_strndup (line+pos, ctr+1);
						// see if our name exists
						if ((!strncmp(token2, "vpn", 3)) &&
							(!strncmp(token1, nm_id, tok1_len)))
							bValidName = FALSE;
						g_free(token1);
						g_free(token2);
					}
				}
				matchctr++;
				if (statusfile != NULL) fclose(statusfile);
				g_free(line);
				g_free(tempstr);
			}
			// write connection file
			fprintf(connectfile, "[connection]\n");
			fprintf(connectfile, "id=%s\n", nm_id);
			// generate uuid
			uuid_generate_time_safe(uuid);
			uuid_unparse_lower(uuid, uuid_str);
			fprintf(connectfile, "uuid=%s\n", uuid_str);
			fprintf(connectfile, "type=vpn\n\n[vpn]\n");
			fprintf(connectfile, 
			        "service-type=org.freedesktop.NetworkManager.openvpn\n");
			fprintf(connectfile, "connection-type=password\n");
			fprintf(connectfile, "auth=%s\n", auth);
			fprintf(connectfile, "password-flags=0\n");
			fprintf(connectfile, "remote=%s\n", ipaddress);
			fprintf(connectfile, "cipher=%s\n", cipher);
			if (!strncmp(protocol, "tcp", 3))
				fprintf(connectfile, "proto-tcp=yes\n");
			if (!strncmp(device, "tap", 3))
				fprintf(connectfile, "tap-dev=yes\n");
			fprintf(connectfile, "port=%s\n", port);
			fprintf(connectfile, "username=vpn\n");
			fprintf(connectfile, "ca=%s/%s.crt\n", WorkDir, vpnname);
			fprintf(connectfile, "\n[vpn-secrets]\npassword=vpn\n");
			fprintf(connectfile, "\n[ipv4]\nmethod=auto\n");
			// close connection file
			if (connectfile != NULL) fclose(connectfile);
			g_free(country);
		}
		g_free(tempstr);
	}
	// run suid program to copy connection file
	cmdstr = g_strconcat(WorkDir, "/", vpnname, NULL);
	filestr = g_strconcat("/etc/NetworkManager/system-connections/", 
	                      vpnname, NULL);
	ret = 0;
	pid = fork();
	if (pid == 0)
	{
		execlp("gvpngate_suid", "gvpngate_suid", cmdstr, filestr, NULL);
		_exit(EXIT_FAILURE);
	}
	else if (pid < 0) ret = -1;
	else if (waitpid (pid, &status, 0) != pid)  ret = -1;
	// check for fail or exit status other than 0
	if (!((!ret) && WIFEXITED(status) && !WEXITSTATUS(status)))
	{
		// remove connection file from workdir
		cmdstr = g_strconcat(WorkDir, "/", vpnname, NULL);
		if (stat(cmdstr, &st) == 0) remove(cmdstr);
		Statusbar_Message("Gvpngate_suid failed to copy connection file. This sucks.");
		return(FALSE);
	}
	// remove connection file from workdir
	cmdstr = g_strconcat(WorkDir, "/", vpnname, NULL);
	if (stat(cmdstr, &st) == 0) remove(cmdstr);
	// check for running connection and disconnect
	cmdstr = g_strconcat(WorkDir, "/status", NULL);
	ret = 0;
	pid = fork();
	if (pid == 0)
	{
		int fd = open(cmdstr, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
		dup2(fd, STDOUT_FILENO);   // make stdout go to status file
		close(fd);
		execlp("nmcli", "nmcli", "-t", "-f", "NAME,VPN", "con", "status", NULL);
		_exit(EXIT_FAILURE);
	}
	else if (pid < 0) ret = -1;
	else if (waitpid (pid, &status, 0) != pid)  ret = -1;
	// check for fail or exit status other than 0
	if (!((!ret) && WIFEXITED(status) && !WEXITSTATUS(status)))
	{
		Statusbar_Message("Failed to get running connection list. This sucks.");
		return(FALSE);
	}
	else
	{
		FILE *statusfile;
		gchar *line = NULL;
		gchar *tempstr = NULL;
		size_t len = 0;
		size_t read;

		tempstr = g_strconcat(WorkDir, "/status", NULL);
		statusfile = fopen(tempstr, "r");
		if (statusfile == NULL)
		{
			Statusbar_Message("Unable to read status file. This sucks.");
		}
		else
		{
			// getline loop
			while ((read = getline(&line, &len, statusfile)) != -1) 
			{
				gint ctr = 0;
				gint pos = 0;
				gchar *token1 = NULL;
				gchar *token2 = NULL;

				//get token1
				while ((line[ctr] != ':') && 
				       (line[ctr] != '\0') &&
				       (line[ctr] != '\n')) ctr++;
				if (line[ctr] == '\0') ctr--;
				if (line[ctr] == '\n') ctr--;
				if (ctr > 0) token1 = g_strndup (line, ctr);
				pos = ctr + 1;
				//get token2
				ctr = 0;
				while ((line[ctr+pos] != ':') && 
				       (line[ctr+pos] != '\0') &&
				       (line[ctr+pos] != '\n')) ctr++;
				if (line[ctr+pos] == '\0') ctr--;
				if (line[ctr+pos] == '\n') ctr--;
				token2 = g_strndup (line+pos, ctr+1);
				if (!strncmp(token2, "yes", 3))
				{
					// kill the vpn connection
					ret = 0;
					pid = fork();
					if (pid == 0)
					{
						execlp("nmcli", "nmcli", "con", "down", "id", token1, NULL);
						_exit(EXIT_FAILURE);
					}
					else if (pid < 0) ret = -1;
					else if (waitpid (pid, &status, 0) != pid)  ret = -1;
					// check for fail or exit status other than 0
					if (!((!ret) && WIFEXITED(status) && !WEXITSTATUS(status)))
					{
						Statusbar_Message("Failed to kill running connection. This sucks.");
						return(FALSE);
					}
					sleep(1);
				}
				g_free(token1);
				g_free(token2);
			}
		}
		// close file
		if (statusfile != NULL) fclose(statusfile);
		g_free(line);
		g_free(tempstr);
	}
	sleep(1);
	// fire up the new connection
	ret = 0;
	pid = fork();
	if (pid == 0)
	{
		// suppress output from nmcli
		int fd =  open("/dev/null", O_RDWR); 
		dup2(fd, STDOUT_FILENO);   // make stdout go to null file
		dup2(fd, STDERR_FILENO);   // make stderr go to null file
		close(fd);
		execlp("nmcli", "nmcli", "con", "up", "id", nm_id, NULL);
		_exit(EXIT_FAILURE);
	}
	else if (pid < 0) ret = -1;
	else if (waitpid (pid, &status, 0) != pid)  ret = -1;
	// check to see if connection worked
	if ((!ret) && WIFEXITED(status) && !WEXITSTATUS(status))
	{
		gchar *msg = NULL;

		msg = g_strconcat("Successfully connected to: ", vpnname, 
		                  " (", nm_id, ")", NULL); 
		Statusbar_Message(msg); 
		g_free(msg);
	}
	else
	{
		ret = 0;
		pid = fork();
		if (pid == 0)
		{
			execlp("nmcli", "nmcli", "con", "delete", "id", nm_id, NULL);
			_exit(EXIT_FAILURE);
		}
		else if (pid < 0) ret = -1;
		else if (waitpid (pid, &status, 0) != pid)  ret = -1;
		// check for fail or exit status other than 0
		if ((!ret) && WIFEXITED(status) && !WEXITSTATUS(status))
		{
			sleep(1);
			Statusbar_Message("Connection failed.  VPN has been removed from Network Manager.");
		}
		else
		{
			Statusbar_Message("Failed to remove vpn connection. This sucks.");
			return(FALSE);
		}
	}
	g_free(cmdstr);
	g_free(vpnname);
	g_free(ipaddress);
	g_free(port);
	g_free(cipher);
	g_free(auth);
	g_free(device);
	g_free(protocol);
	g_free(nm_id);   
	g_free(filestr);
	return(FALSE);
}

/****************************************************************************
 *                                                                          *
 * Function: Statusbar_Message                                              *
 *                                                                          *
 * Purpose : display a message in the main window status bar                *
 *                                                                          *
 ****************************************************************************/
void Statusbar_Message(gchar *msg)
{
	gtk_statusbar_push(GTK_STATUSBAR(MainWin_StatusBar),
	                   MainWin_StatusBarID, msg);
}

/****************************************************************************
 *                                                                          *
 * Function: Get_Vpn_List_File                                              *
 *                                                                          *
 * Purpose : download & parse the vpn list and fill the list store          *
 *                                                                          *
 ****************************************************************************/
gboolean Get_Vpn_List_File(gpointer data)
{
	gint ret;
	gint status;
	pid_t pid;
	FILE *vpnlistfile;
	gchar *line = NULL;
	size_t len = 0;
	size_t read;
	gchar *tempstr = NULL;

	// unselect rows
	gtk_tree_selection_unselect_all
		(gtk_tree_view_get_selection(GTK_TREE_VIEW(VPN_List_Treeview)));
	// call wget to retrieve data file
	tempstr = g_strconcat("--output-document=", WorkDir, "/vpn.tmp", NULL);
	ret = 0;
	pid = fork();
	if (pid == 0)
	{
		execlp("wget", "wget", tempstr, "--quiet", "--timeout=20", 
		       "http://www.vpngate.net/api/iphone/", NULL);
		_exit(EXIT_FAILURE);
	}
	else if (pid < 0) ret = -1;
	else if (waitpid (pid, &status, 0) != pid)  ret = -1;
	// check to see if we got the file
	if ((!ret) && WIFEXITED(status) && !WEXITSTATUS(status))
	{
		gchar *tempstr2 = NULL;

		// rename vpn.tmp to vpn.lst
		tempstr = g_strconcat(WorkDir,"/vpn.tmp", NULL);
		tempstr2 = g_strconcat(WorkDir,"/vpn.lst", NULL);
		rename(tempstr, tempstr2);
		Statusbar_Message("Vpn list successfully downloaded.");
		g_free(tempstr2);
	}
	else
	{
		// clean up tmp file on download fail
		tempstr = g_strconcat(WorkDir, "/vpn.tmp", NULL);
		if (stat(tempstr, &st) == 0) remove(tempstr);
		// see if we have an old file
		tempstr = g_strconcat(WorkDir, "/vpn.lst", NULL);
		if (stat(tempstr, &st) == 0)
		{
			Statusbar_Message("Vpn list download failed. Using old vpn list.");
		}
		else 
		{
			Statusbar_Message("Vpn list download has completely failed.");
			return(FALSE);
		}
	}
	// open vpn list file
	tempstr = g_strconcat(WorkDir, "/vpn.lst", NULL);
	vpnlistfile = fopen(tempstr, "r");
	if (vpnlistfile == NULL)
	{
		Statusbar_Message("Unable to read VPN list. This sucks.");
		return(FALSE);
	}
	else
	{
		// clear list store
		gtk_list_store_clear (VPN_List);
	}
	// getline loop
    while ((read = getline(&line, &len, vpnlistfile)) != -1) 
	{
		if (!((line[0] == '*') || (line[0] == '#')))
		{
			// we have a valid line
			gint ctr = 0;
			gint pos = 0;
			gchar *token = NULL;
			GtkTreeIter iter;
			
			//append line to list store
			gtk_list_store_append (VPN_List, &iter);

			// get vpn name
			while (line[pos+ctr] != ',') ctr++;
			// limit to 20 chars
			if (ctr >= 20)
				token = g_strndup (line+pos, 20); 
			else
				token = g_strndup (line+pos, ctr); 
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 0, token, -1);
			pos = pos + ctr + 1;

			//get IP
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			token = g_strndup (line+pos, ctr);
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 1, token, -1);
			pos = pos + ctr + 1;

			//get Score
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			token = g_strndup (line+pos, ctr);
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 2, atol(token), -1);
			pos = pos + ctr + 1;

			//get Ping
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			token = g_strndup (line+pos, ctr);
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 3, atol(token), -1);
			pos = pos + ctr + 1;

			//get Speed
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			token = g_strndup (line+pos, ctr);
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 4, atol(token), -1);
			pos = pos + ctr + 1;

			// get CountryLong
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			// limit to 20 chars
			if (ctr >= 20)
				token = g_strndup (line+pos, 20); 
			else
				token = g_strndup (line+pos, ctr); 
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 5, token, -1);
			pos = pos + ctr + 1;

			// get CountryShort
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			token = g_strndup (line+pos, ctr);
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 6, token, -1);
			pos = pos + ctr + 1;

			// get NumVpnSessions
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			token = g_strndup (line+pos, ctr);
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 7, atol(token), -1);
			pos = pos + ctr + 1;

			// get Uptime
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			token = g_strndup (line+pos, ctr);
  		    // add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 8, atol(token), -1);
			pos = pos + ctr + 1;

			// get TotalUsers
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			token = g_strndup (line+pos, ctr);
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 9, atol(token), -1);
			pos = pos + ctr + 1;

			// get TotalTraffic
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			token = g_strndup (line+pos, ctr);
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 10, atol(token), -1);
			pos = pos + ctr + 1;

			// get LogType
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			token = g_strndup (line+pos, ctr);
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 11, token, -1);
			pos = pos + ctr + 1;

			// get Operator
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			// limit to 30 chars
			if (ctr >= 30)
				token = g_strndup (line+pos, 30); 
			else
				token = g_strndup (line+pos, ctr); 
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 12, token, -1);
			pos = pos + ctr + 1;

			// get Message
			ctr = 0;
			while (line[pos+ctr] != ',') ctr++;
			// limit to 30 chars
			if (ctr >= 30)
				token = g_strndup (line+pos, 30); 
			else
				token = g_strndup (line+pos, ctr); 
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
			    gtk_list_store_set (VPN_List, &iter, 13, token, -1);
			pos = pos + ctr + 1;

			// get ConfigData
			ctr = 0;
			while (line[pos+ctr] != '\n') ctr++;
			token = g_strndup (line+pos, ctr);
			// add to list store
			if (g_utf8_validate (token, -1, NULL))
				gtk_list_store_set (VPN_List, &iter, 14, token, -1);
			
			g_free(token);
		}
	}
	// close file
	if (vpnlistfile != NULL) fclose(vpnlistfile);
	g_free(line);
	g_free(tempstr);
	return(FALSE);
}

/****************************************************************************
 *                                                                          *
 * Function: Destroy_Main_Window                                            *
 *                                                                          *
 * Purpose : called when the main window is closed                          *
 *                                                                          *
 ****************************************************************************/
void Destroy_Main_Window (GtkWidget *widget, gpointer data)
{
	gchar *cmdstr = NULL;
	gint ret = -1;
	gint status;
	pid_t pid;

	// get a list of .crt files
	cmdstr = g_strconcat(WorkDir, "/status", NULL);
	ret = 0;
	pid = fork();
	if (pid == 0)
	{
		int fd = open(cmdstr, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
		dup2(fd, STDOUT_FILENO);   // make stdout go to status file
		close(fd);
		execlp("find", "find", WorkDir, "-name", "*.crt", "-printf", "%P\n", NULL);
		_exit(EXIT_FAILURE);
	}
	else if (pid < 0) ret = -1;
	else if (waitpid (pid, &status, 0) != pid)  ret = -1;
	// check for fail or exit status other than 0
	if (!((!ret) && WIFEXITED(status) && !WEXITSTATUS(status)))
	{
		Statusbar_Message("Failed to get directory listing. This sucks.");
	}
	else
	{
		FILE *statusfile;
		gchar *line = NULL;
		gchar *tempstr = NULL;
		size_t len = 0;
		size_t read;

		tempstr = g_strconcat(WorkDir, "/status", NULL);
		statusfile = fopen(tempstr, "r");
		if (statusfile == NULL)
		{
			Statusbar_Message("Unable to read status file. This sucks.");
		}
		else
		{
			// step through status file
			while ((read = getline(&line, &len, statusfile)) != -1) 
			{
				gchar *tmpstr2 = NULL;
				gchar *tmpstr3 = NULL;
				gchar *tmpstr4 = NULL;

				tmpstr2 = g_strndup (line, read - 5);
				tmpstr3 = g_strconcat("/etc/NetworkManager/system-connections/", 
			                      tmpstr2, NULL);
				tmpstr4 = g_strconcat(WorkDir, "/", tmpstr2, ".crt", NULL);
				// if it dosen't exist in NM dir, delete from work dir
				if (stat(tmpstr3, &st) != 0)
					remove(tmpstr4);
				g_free(tmpstr2);
				g_free(tmpstr3);
				g_free(tmpstr4);
			}
		}
		fclose(statusfile);
		g_free(line);
		g_free(tempstr);
	}
	// delete status file
	cmdstr = g_strconcat(WorkDir, "/status", NULL);
	if (stat(cmdstr, &st) == 0) remove(cmdstr);
	 // remove all our status messages
	gtk_statusbar_remove_all(GTK_STATUSBAR(MainWin_StatusBar),
	                         MainWin_StatusBarID);
	// release our widgets
	gtk_widget_destroy (MainWindow);
	gtk_widget_destroy (MainWin_StatusBar);
	gtk_widget_destroy (MainWin_TB_Connect);
	gtk_widget_destroy (MainWin_Info_1);
	gtk_widget_destroy (MainWin_Info_2);
	gtk_widget_destroy (MainWin_Info_3);
	gtk_widget_destroy (MainWin_Info_4);
	gtk_widget_destroy (VPN_List_Treeview);
	gtk_widget_destroy (MainWin_Menu_Connect);
	gtk_widget_destroy (AboutBox);
	// release global strings
	g_free(WorkDir);
	// release local string
	g_free(cmdstr);
	// kill the main loop
	gtk_main_quit ();
}

/****************************************************************************
 *                                                                          *
 * Function: Create_Main_Window                                             *
 *                                                                          *
 * Purpose : creates the main window                                        *
 *                                                                          *
 ****************************************************************************/
GtkWidget* Create_Main_Window (void)
{
	GtkWidget *window; //function ret - mainwindow handle
	GtkBuilder *builder;
	GError* error = NULL;

	// Load UI from file 
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}
	// Auto-connect signal handlers
	gtk_builder_connect_signals (builder, NULL);
	// widgets we care about - don't forget to release them later
	window = GTK_WIDGET (gtk_builder_get_object(builder, "mainwindow"));
	MainWin_StatusBar = GTK_WIDGET (gtk_builder_get_object
	                                (builder,"mainwindowstatbar"));
	MainWin_TB_Connect = GTK_WIDGET (gtk_builder_get_object
	                                (builder, "toolbutton_connect"));
	MainWin_Info_1 = GTK_WIDGET (gtk_builder_get_object
	                                (builder, "infolabel1"));
	MainWin_Info_2 = GTK_WIDGET (gtk_builder_get_object
	                                (builder, "infolabel2"));
	MainWin_Info_3 = GTK_WIDGET (gtk_builder_get_object
	                                (builder, "infolabel3"));
	MainWin_Info_4 = GTK_WIDGET (gtk_builder_get_object
	                                (builder, "infolabel4"));
	VPN_List_Treeview = GTK_WIDGET (gtk_builder_get_object
	                                (builder, "vpnlisttreeview"));
	MainWin_Menu_Connect = GTK_WIDGET (gtk_builder_get_object
	                                (builder, "menuitemConnect"));
	AboutBox = GTK_WIDGET (gtk_builder_get_object
	                                (builder, "aboutbox"));
	// list store object
	VPN_List = GTK_LIST_STORE (gtk_builder_get_object
	                                (builder,"vpn_liststore"));
	// release builder
	g_object_unref (builder);
	// set version in about dialog
	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG(AboutBox), 
	                              g_strconcat("Version ", GVPNGATE_VERSION, 
	                                          NULL));
	// initalize statusbar and set the statusbar conxtext id
	MainWin_StatusBarID = gtk_statusbar_get_context_id
		(GTK_STATUSBAR(MainWin_StatusBar), "Main Window Messages");
	// create a dummy.crt file so ls command doesn't complain on cleanup
	{
		FILE *dummyfile;
		gchar *tempstr = NULL;

		// use a name that is pretty sure not to be someone's NM vpn name
		tempstr = g_strconcat(WorkDir, "/_gvpngate_temporary_dummy_junk_file_.crt", NULL);
		dummyfile = fopen(tempstr, "w");
		if (dummyfile != NULL) fclose(dummyfile);
		g_free(tempstr);
	}
	Statusbar_Message("Downloading vpn list.  Please wait...");
	return window;
}

/****************************************************************************
 *                                                                          *
 * Function: main                                                           *
 *                                                                          *
 * Purpose : program main entry function                                    *
 *                                                                          *
 ****************************************************************************/
int main (int argc, char *argv[])
{
	// initalize gtk
	gtk_init (&argc, &argv);
	// set our work directory
	WorkDir = g_strconcat (g_get_home_dir (), "/.gvpngate", NULL);
	// make sure work directory exists
	if (stat(WorkDir, &st) == -1) mkdir(WorkDir, 0700);
	// create main window
	MainWindow = Create_Main_Window ();
	// show main window
	gtk_widget_show (MainWindow);
	// get vpn list after main loop has started
	g_timeout_add (500, Get_Vpn_List_File, NULL);
	// start main loop
	gtk_main ();
	return 0;
}
