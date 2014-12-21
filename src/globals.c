/*
 * =:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=
 * =                       P L a Z a   c h a t                           =
 * =                                                                     =
 * = A chatting client with ncurses interface for system local users     =
 * =                                                                     =
 * =:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=
 *
 * Emanuele Faranda                               black.silver@hotmail.it
 *
 * =======================================================================
 * globals :: Project wide variables and utility functions
 * =======================================================================
 *
 * ------------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * ------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "utils.h"
#include "settings.h"

PLAZA_CHAR * PlazaUsernick = NULL;
int PlazaUsernick_L = 0;

void plaza_load_nickname()
{
    /*
     * Sets the global PLAZANICK variable from environment PlazaUsernick or
     * USER variables. If none of them is set, PlazaUsernick will be set to
     * '?'.
     */
    char *def = "?";
    char *p = NULL;

    p = getenv("PLAZANICK");
    if (p == NULL) {
        p = getenv("USER");
        if (p == NULL)
            p = def;
    }

    PlazaUsernick_L = min(strlen(p), PLAZA_NICK_MAXLENGTH);
    PlazaUsernick = (PLAZA_CHAR *) malloc(sizeof(PLAZA_CHAR) *
        (PlazaUsernick_L+1));
    mbstowcs((wchar_t*)PlazaUsernick, p, PlazaUsernick_L);
}

void plaza_unload_nickname()
{
    free(PlazaUsernick);
    PlazaUsernick_L = 0;
}
