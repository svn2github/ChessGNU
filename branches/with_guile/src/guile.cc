/* GNU Chess 6 - guile.cc - Guile mode and primitives

   Copyright (c) 2001-2017 Free Software Foundation, Inc.

   GNU Chess is based on the two research programs
   Cobalt by Chua Kong-Sian and Gazebo by Stuart Cracraft.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Contact Info:
     bug-gnu-chess@gnu.org
     cracraft@ai.mit.edu, cracraft@stanfordalumni.org, cracraft@earthlink.net
*/

#include <stdio.h>
#include <stdlib.h>
#include <libguile.h>

#include "engine/board.h"
#include "engine/eval.h"
#include "engine/material.h"
#include "engine/pawn.h"
#include "configmake.h"

using namespace engine;

// compute_pkgdatadir()
char const *compute_pkgdatadir()
{
   char const *pkgdatadir = getenv ("GNUCHESS_PKGDATADIR");
   return pkgdatadir ? pkgdatadir : PKGDATADIR;
}

// C++ primitives for Scheme
// Suitable for the function calls in eval_builtin()

SCM c_eval_enable_userdef(SCM enable_userdef)
{
    int result = eval_enable_userdef( scm_num2int(enable_userdef, 0, NULL) );
    return scm_int2num( result );
}

SCM c_eval_get_param(SCM param_key)
{
    int result = eval_get_param( scm_num2int(param_key, 0, NULL) );
    return scm_int2num( result );
}

SCM c_eval_set_param(SCM param_key, SCM param_value)
{
    int result = eval_set_param( scm_num2int(param_key, 0, NULL),
                                 scm_num2int(param_value, 0, NULL) );
    return scm_int2num( result );
}

// Initialize Guile and define primitives
void init_chess_guile(void)
{
    printf("GUILE enabled...\n");

    SCM func_symbol;
    SCM func;

    // Initialize Guile
    // TODO Check in Guile reference if scm_init_guile can be called once,
    // or if it must be called for every thread.
    scm_init_guile();

    // Define C++ primitives for Scheme
    // Each call to scm_c_define_gsubr maps a Scheme function to a C++ function:
    //   - Scheme functions are defined in src/eval.scm
    //   - C++ functions are defined in src/engine/
    scm_c_define_gsubr( "eval-enable-userdef", 1, 0, 0, (void*)c_eval_enable_userdef );
    scm_c_define_gsubr( "eval-get-param", 1, 0, 0, (void*)c_eval_get_param );
    scm_c_define_gsubr( "eval-set-param", 2, 0, 0, (void*)c_eval_set_param );

    // Load the scheme function definition
    const unsigned int MaxFileNameSize = 255;
    char eval_scm_file_name[] = "eval.scm";
    char eval_scm_full_file_name[MaxFileNameSize+1];
    char path[MaxFileNameSize+1];
    FILE *eval_scm_file;
    if ( ( eval_scm_file = fopen(eval_scm_file_name, "r") ) != NULL ) {
        fclose(eval_scm_file);
        strcpy(eval_scm_full_file_name,"");
    } else {
        if ( strlen(path) + strlen(eval_scm_file_name) + 1 > MaxFileNameSize ) {
            printf( "Length of file name is too long; must be less than 256.\n" );
            exit( 1 );
        }
        strcpy(path, compute_pkgdatadir());
        strcpy(eval_scm_full_file_name, path);
        strcat(eval_scm_full_file_name,"/");
    }
    strcat(eval_scm_full_file_name,eval_scm_file_name);
    if ( fopen(eval_scm_full_file_name, "r") == NULL ) {
        printf( "Cannot find Scheme file with eval function '%s'\n", eval_scm_full_file_name );
        exit( 1 );
    }
    scm_c_primitive_load( eval_scm_full_file_name );

    // Look up do-hello, a pure Scheme function, i.e. not mapped to a C++ function
    func_symbol = scm_c_lookup( "do-hello" );
    func = scm_variable_ref( func_symbol );
    // Call do-hello to print a welcome message, 0 arguments
    scm_call_0( func );

    // Look up do-print, a pure Scheme function, i.e. not mapped to a C++ function
    func_symbol = scm_c_lookup( "do-print" );
    func = scm_variable_ref( func_symbol );
    // Call do-print to print a welcome message, 1 argument
    SCM text = scm_from_locale_string( "GUILE integration in GNU Chess" );
    scm_call_1( func, text );

    // Look up eval-guile-init, a pure Scheme function, i.e. not mapped to a C++ function
    func_symbol = scm_c_lookup( "eval-guile-init" );
    func = scm_variable_ref( func_symbol );
    // Call eval-guile-init to set up user-defined evaluation function, 0 arguments
    scm_call_0( func );

   // func_symbol = scm_c_lookup( "eval-guile" );
   // engine::func_eval_guile = scm_variable_ref( func_symbol );

    return;
}
