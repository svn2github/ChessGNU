/* eval.h

   GNU Chess engine

   Copyright (C) 2001-2015 Free Software Foundation, Inc.

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
*/


// eval.h

#ifndef EVAL_H
#define EVAL_H

// includes

#ifdef HAVE_GUILE
#include <libguile.h>
#endif
#include "board.h"
#include "util.h"
#ifdef HAVE_GUILE
#include "material.h"
#include "pawn.h"
#endif

namespace engine {

// functions

extern void eval_init ();

#ifdef HAVE_GUILE
extern SCM eval_builtin( void );
extern SCM eval_builtin1( SCM board_handle );
extern SCM eval_builtin22( void );
extern SCM eval_builtin21( void );
extern void eval_draw(const board_t * board, const material_info_t * mat_info, const pawn_info_t * pawn_info, int mul[2]);
extern void eval_piece         (const board_t * board, const material_info_t * mat_info, const pawn_info_t * pawn_info, int * opening, int * endgame);
extern void eval_king          (const board_t * board, const material_info_t * mat_info, int * opening, int * endgame);
extern void eval_passer        (const board_t * board, const pawn_info_t * pawn_info, int * opening, int * endgame);
extern void eval_pattern       (const board_t * board, int * opening, int * endgame);

extern SCM func_eval_guile;
#endif
extern int  eval      (const board_t * board);

}  // namespace engine

#endif // !defined EVAL_H

// end of eval.h

