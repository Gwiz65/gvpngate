/***************************************************************************
 *                                                                         * 
 * gvpngate_suid.c                                                         *
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <linux/limits.h>

int main ( int argc, char *argv[] )
{
	switch (argc) 		
	{
		case 1:
		{
			int ret = 0;
			int status;
			pid_t pid;

			pid = fork();
			if (pid == 0)
			{
				int fd_null =  open("/dev/null", O_RDWR); 
				dup2(fd_null, STDOUT_FILENO);	
				dup2(fd_null, STDERR_FILENO); 	
				close(fd_null);
				execlp("nmcli", "nmcli", "con", "reload", NULL);
				_exit(EXIT_FAILURE);
			}
			else if (pid < 0) ret = -1;
			else if (waitpid (pid, &status, 0) != pid)  ret = -1;
			if (!((!ret) && WIFEXITED(status) && !WEXITSTATUS(status)))
			{
				printf("gvpngate_suid: Unable reload connections.");
				printf("\n");
				return 1;
			}
			break;
		}
		case 2:
		{
			char tmpstr[PATH_MAX];

			// only a filename - delete it
			sprintf(tmpstr, "/etc/NetworkManager/system-connections/%s", argv[1]);
			if (remove(tmpstr)) 
			{
				printf("gvpngate_suid: Unable to delete system connection file.");
				printf("\n");
				return 1;
			}
			break;
		}	
		case 3:
		{
			FILE *src;
			FILE *dst;
			char tmpstr[PATH_MAX];

			// We assume argv[1] is the source and argv[2] is the vpnname
			sprintf(tmpstr, "/etc/NetworkManager/system-connections/%s", argv[2]);
			src = fopen(argv[1], "r");
			dst = fopen(tmpstr, "w");
			if ((src != NULL) && (dst != NULL))
			{
				char *line;
				size_t len = 0;

				while (getline(&line, &len, src) != -1) fprintf(dst, "%s", line);
				fclose(src);
				fclose(dst);
				// change owner and group to root
				if (chown(tmpstr, 0, 0)) 
				{
					printf("gvpngate_suid: Unable to set ownership of system connection file.");
					printf("\n");
					return 1;
				}
				// set permissions
				if (chmod(tmpstr, strtol("0600", 0, 8))) 
				{
					printf("gvpngate_suid: Unable to set permissions of system connection file.");
				    	printf("\n");					
					return 1;
				}
			}
			else
			{
				printf("gvpngate_suid: Unable to create system connection file.");
				printf("\n");
				return 1;
			}
			break; 
		}
		default:
		{
			printf("gvpngate_suid: Incorrect number of arguments.");
			printf("\n");
			return 1;
		}
	}
	return 0;
}
