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
#include <sys/stat.h>
 
int main ( int argc, char *argv[] )
{
	switch (argc) 		
	{
		case 1:
		{
			// no args - send reload
			system("nmcli con reload");
			break;
		}
		case 2:
		{
			// only one filename - delete it
			if (remove(argv[1])) 
			{
				printf("Unable to delete system connection file.");
				return 1;
			}
			break;
		}	
		case 3:
		{
			FILE *src;
			FILE *dst;

			// We assume argv[1] is the source and argv[1] is the dest 
			src = fopen(argv[1], "r");
			dst = fopen(argv[2], "w");
			if ((src != NULL) && (dst != NULL))
			{
				char *line;
				size_t len = 0;

				while (getline(&line, &len, src) != -1) 
					fprintf(dst, "%s", line);
				fclose(src);
				fclose(dst);
				// change owner and group to root
				if (chown(argv[2], 0, 0)) 
				{
					printf("Unable to set ownership of system connection file.");
					return 1;
				}
				// set permissions
				if (chmod(argv[2], strtol("0600", 0, 8))) 
				{
					printf("Unable to set permissions of system connection file.");
					return 1;
				}
			}
			else
			{
				printf("Unable to create system connection file.");
				return 1;
			}
			break; 
		}
		default:
		{
			printf("Incorrect arguments passed to gvpngate_suid");
			return 1;
		}
	}
	return 0;
}
