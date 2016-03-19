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
 * with this program.  If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                         * 
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main ( int argc, char *argv[] )
{
    if ( argc != 3 ) /* argc should be 3 for correct execution */
    {
        printf("Incorrect arguments passed to gvpngate_suid");
		return 1;
    }
    else 
    {
		char cmd[2048];

		if (!strncmp(argv[1], "**remove_connection**", 21))
		{
			// remove argv[2]
			sprintf(cmd, "rm -f \"%s\">NULL", argv[2]);
			system(cmd);
			
		}
		else
		{
			// We assume argv[1] is the source and argv[2] is the dest 
			sprintf(cmd, "cp \"%s\" \"%s\">NULL", argv[1], argv[2]);
			system(cmd);

			// change owner and group to root
			sprintf(cmd, "chown root:root \"%s\">NULL", argv[2]);
			system(cmd);

			// set permissions
			sprintf(cmd, "chmod 0600 -v \"%s\">NULL", argv[2]);
			system(cmd);
		}
	}
	return 0;
}
