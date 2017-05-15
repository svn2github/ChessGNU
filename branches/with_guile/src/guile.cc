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

using namespace engine;

// C++ primitives for Scheme
// Suitable for the function calls in eval_builtin()

SCM c_material_get_info( SCM board_handle, SCM mat_handle )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    material_info_t * mat_info = (material_info_t *)scm_num2ulong( mat_handle, 0, NULL );
    material_get_info( mat_info, board );
    return scm_int2num( 0 );
}

SCM c_material_get_info_opening( SCM material_info_handle )
{
    material_info_t * material_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    int opening = material_info->opening;
    return scm_int2num( opening );
}

SCM c_material_get_info_endgame( SCM material_info_handle )
{
    material_info_t * material_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    int endgame = material_info->endgame;
    return scm_int2num( endgame );
}

SCM c_material_get_mul( SCM material_info_handle, SCM mul_handle )
{
    material_info_t * material_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    int * mul = (int *)scm_num2ulong( mul_handle, 0, NULL );
    mul[White] = material_info->mul[White];
    mul[Black] = material_info->mul[Black];
    return scm_int2num( 0 );
}

SCM c_material_get_mul_white( SCM material_info_handle )
{
    material_info_t * material_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    unsigned short mul_white = material_info->mul[White];
    return scm_ushort2num( mul_white );
}

SCM c_material_get_mul_black( SCM material_info_handle )
{
    material_info_t * material_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    unsigned short mul_black = material_info->mul[Black];
    return scm_ushort2num( mul_black );
}

SCM c_board_get_opening( SCM board_handle )
{
    board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    int opening = board->opening;
    return scm_int2num( opening );
}

SCM c_board_get_endgame( SCM board_handle )
{
    board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    int endgame = board->endgame;
    return scm_int2num( endgame );
}

SCM c_pawn_get_info( SCM board_handle, SCM pawn_handle )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    pawn_info_t * pawn_info = (pawn_info_t *)scm_num2ulong( pawn_handle, 0, NULL );
    pawn_get_info( pawn_info, board );
    return scm_int2num( 0 );
}

SCM c_pawn_get_opening( SCM pawn_handle )
{
    pawn_info_t * pawn_info = (pawn_info_t *)scm_num2ulong( pawn_handle, 0, NULL );
    int opening = pawn_info->opening;
    return scm_int2num( opening );
}

SCM c_pawn_get_endgame( SCM pawn_handle )
{
    pawn_info_t * pawn_info = (pawn_info_t *)scm_num2ulong( pawn_handle, 0, NULL );
    int endgame = pawn_info->endgame;
    return scm_int2num( endgame );
}

SCM c_eval_draw( SCM board_handle, SCM material_info_handle, SCM pawn_handle, SCM mul_handle )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    const material_info_t * mat_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    const pawn_info_t * pawn_info = (pawn_info_t *)scm_num2ulong( pawn_handle, 0, NULL );
    int * mul = (int *)scm_num2ulong( mul_handle, 0, NULL );
    eval_draw( board, mat_info, pawn_info, mul );
    return scm_int2num( 0 );
}

SCM c_eval_draw_get_mul_white( SCM mul_handle )
{
    int * mul = (int *)scm_num2ulong( mul_handle, 0, NULL );
    return scm_int2num( mul[White] );
}

SCM c_eval_draw_get_mul_black( SCM mul_handle )
{
    int * mul = (int *)scm_num2ulong( mul_handle, 0, NULL );
    return scm_int2num( mul[Black] );
}

SCM c_eval_piece_get_opening( SCM board_handle, SCM material_info_handle, SCM pawn_handle, SCM scm_opening )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    const material_info_t * mat_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    const pawn_info_t * pawn_info = (pawn_info_t *)scm_num2ulong( pawn_handle, 0, NULL );
    int opening = scm_num2int( scm_opening, 0, NULL );
    int endgame;
    eval_piece( board, mat_info, pawn_info, &opening, &endgame );
    return scm_int2num( opening );
}

SCM c_eval_piece_get_endgame( SCM board_handle, SCM material_info_handle, SCM pawn_handle, SCM scm_endgame )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    const material_info_t * mat_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    const pawn_info_t * pawn_info = (pawn_info_t *)scm_num2ulong( pawn_handle, 0, NULL );
    int opening;
    int endgame = scm_num2int( scm_endgame, 0, NULL );
    eval_piece( board, mat_info, pawn_info, &opening, &endgame );
    return scm_int2num( endgame );
}

SCM c_eval_king_get_opening( SCM board_handle, SCM material_info_handle, SCM scm_opening )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    const material_info_t * mat_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    int opening = scm_num2int( scm_opening, 0, NULL );
    int endgame;
    eval_king( board, mat_info, &opening, &endgame );
    return scm_int2num( opening );
}

SCM c_eval_king_get_endgame( SCM board_handle, SCM material_info_handle, SCM scm_endgame )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    const material_info_t * mat_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    int opening;
    int endgame = scm_num2int( scm_endgame, 0, NULL );
    eval_king( board, mat_info, &opening, &endgame );
    return scm_int2num( endgame );
}

SCM c_eval_passer_get_opening( SCM board_handle, SCM pawn_handle, SCM scm_opening )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    const pawn_info_t * pawn_info = (pawn_info_t *)scm_num2ulong( pawn_handle, 0, NULL );
    int opening = scm_num2int( scm_opening, 0, NULL );
    int endgame;
    eval_passer( board, pawn_info, &opening, &endgame );
    return scm_int2num( opening );
}

SCM c_eval_passer_get_endgame( SCM board_handle, SCM pawn_handle, SCM scm_endgame )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    const pawn_info_t * pawn_info = (pawn_info_t *)scm_num2ulong( pawn_handle, 0, NULL );
    int opening;
    int endgame = scm_num2int( scm_endgame, 0, NULL );
    eval_passer( board, pawn_info, &opening, &endgame );
    return scm_int2num( endgame );
}

SCM c_eval_pattern_get_opening( SCM board_handle, SCM scm_opening )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    int opening = scm_num2int( scm_opening, 0, NULL );
    int endgame;
    eval_pattern( board, &opening, &endgame );
    return scm_int2num( opening );
}

SCM c_eval_pattern_get_endgame( SCM board_handle, SCM scm_endgame )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    int opening;
    int endgame = scm_num2int( scm_endgame, 0, NULL );
    eval_pattern( board, &opening, &endgame );
    return scm_int2num( endgame );
}

SCM c_get_phase( SCM material_info_handle )
{
    material_info_t * material_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    int phase = material_info->phase;
    return scm_int2num( phase );
}

SCM c_is_set_draw_bishop_flag( SCM material_info_handle )
{
    material_info_t * material_info = (material_info_t *)scm_num2ulong( material_info_handle, 0, NULL );
    if ( ( material_info->flags & DrawBishopFlag ) != 0 ) {
        return SCM_BOOL_T;
    } else {
        return SCM_BOOL_F;
    }
}

SCM c_board_get_piece_white( SCM board_handle )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    int wb = board->piece[White][1];
    return scm_int2num( wb );
}

SCM c_board_get_piece_black( SCM board_handle )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    int bb = board->piece[Black][1];
    return scm_int2num( bb );
}

SCM c_square_colour( SCM scm_b )
{
    int b = scm_num2int( scm_b, 0, NULL );
    int c = SQUARE_COLOUR( b );
    return scm_int2num( c );
}

SCM c_is_colour_turn_black( SCM board_handle )
{
    const board_t * board = (board_t *)scm_num2ulong( board_handle, 0, NULL );
    if ( COLOUR_IS_BLACK( board->turn ) ) {
        return SCM_BOOL_T;
    } else {
        return SCM_BOOL_F;
    }
}

// C++ primitives for Scheme
// Atomic primitives, suitable for a user-defined eval-guile-scm

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
    // Each call to scm_c_Define_gsubr maps a Scheme function to a C++ function:
    //   - Scheme functions are defined in src/eval.scm
    //   - C++ functions are defined in src/engine/
    scm_c_define_gsubr( "eval-builtin", 1, 0, 0, (void*)engine::eval_builtin );
    scm_c_define_gsubr( "material-get-info", 2, 0, 0, (void*)c_material_get_info );
    scm_c_define_gsubr( "material-get-info-opening", 1, 0, 0, (void*)c_material_get_info_opening );
    scm_c_define_gsubr( "material-get-info-endgame", 1, 0, 0, (void*)c_material_get_info_endgame );
    scm_c_define_gsubr( "material-get-mul", 2, 0, 0, (void*)c_material_get_mul );
    scm_c_define_gsubr( "material-get-mul-white", 1, 0, 0, (void*)c_material_get_mul_white );
    scm_c_define_gsubr( "material-get-mul-black", 1, 0, 0, (void*)c_material_get_mul_black );
    scm_c_define_gsubr( "board-get-opening", 1, 0, 0, (void*)c_board_get_opening );
    scm_c_define_gsubr( "board-get-endgame", 1, 0, 0, (void*)c_board_get_endgame );
    scm_c_define_gsubr( "pawn-get-info", 2, 0, 0, (void*)c_pawn_get_info );
    scm_c_define_gsubr( "pawn-get-opening", 1, 0, 0, (void*)c_pawn_get_opening );
    scm_c_define_gsubr( "pawn-get-endgame", 1, 0, 0, (void*)c_pawn_get_endgame );
    scm_c_define_gsubr( "eval-draw", 4, 0, 0, (void*)c_eval_draw );
    scm_c_define_gsubr( "eval-draw-get-mul-white", 1, 0, 0, (void*)c_eval_draw_get_mul_white );
    scm_c_define_gsubr( "eval-draw-get-mul-black", 1, 0, 0, (void*)c_eval_draw_get_mul_black );
    scm_c_define_gsubr( "eval-piece-get-opening", 4, 0, 0, (void*)c_eval_piece_get_opening );
    scm_c_define_gsubr( "eval-piece-get-endgame", 4, 0, 0, (void*)c_eval_piece_get_endgame );
    scm_c_define_gsubr( "eval-king-get-opening", 3, 0, 0, (void*)c_eval_king_get_opening );
    scm_c_define_gsubr( "eval-king-get-endgame", 3, 0, 0, (void*)c_eval_king_get_endgame );
    scm_c_define_gsubr( "eval-passer-get-opening", 3, 0, 0, (void*)c_eval_passer_get_opening );
    scm_c_define_gsubr( "eval-passer-get-endgame", 3, 0, 0, (void*)c_eval_passer_get_endgame );
    scm_c_define_gsubr( "eval-pattern-get-opening", 2, 0, 0, (void*)c_eval_pattern_get_opening );
    scm_c_define_gsubr( "eval-pattern-get-endgame", 2, 0, 0, (void*)c_eval_pattern_get_endgame );
    scm_c_define_gsubr( "get-phase", 1, 0, 0, (void*)c_get_phase );
    scm_c_define_gsubr( "is-set-draw-bishop-flag", 1, 0, 0, (void*)c_is_set_draw_bishop_flag );
    scm_c_define_gsubr( "board-get-piece-white", 1, 0, 0, (void*)c_board_get_piece_white );
    scm_c_define_gsubr( "board-get-piece-black", 1, 0, 0, (void*)c_board_get_piece_black );
    scm_c_define_gsubr( "square-colour", 1, 0, 0, (void*)c_square_colour );
    scm_c_define_gsubr( "is-colour-turn-black", 1, 0, 0, (void*)c_is_colour_turn_black );

    // Load the scheme function definition
    // TODO Find src/eval.scm no matter where gnuchess is invoked from.
    scm_c_primitive_load( "eval.scm" );

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

   // func_symbol = scm_c_lookup( "eval-guile" );
   // engine::func_eval_guile = scm_variable_ref( func_symbol );

    return;
}


static scm_t_bits board_t_tag;

// TODO Add SCM name and update_func to board_t in board.h

static SCM
make_board_t (SCM name)
{
  SCM smob;
  struct board_t *board;

  /* Step 1: Allocate the memory block.
   */
  board = (struct board_t *) scm_gc_malloc (sizeof (struct board_t), "board_t");

  /* Step 2: Initialize it with straight code.
   */
  //board->name = SCM_BOOL_F;
  //board->update_func = SCM_BOOL_F;

  /* Step 3: Create the smob.
   */
  SCM_NEWSMOB (smob, board_t_tag, board);

  /* Step 4: Finish the initialization.
   */
  //board->name = name;

  return smob;
}

