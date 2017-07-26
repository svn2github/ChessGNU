#! GNU Chess 6 - eval.scm: Scheme evaluation

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
!#

#|
Chain of calls:

    C++ main()
      Scheme eval-guile-init
        C++ engine::eval_enable_userdef()
        C++ engine::eval_set_param()

    C++ eval()
      Scheme eval-guile
        ;Scheme eval-guile-c
        ;  C++ eval_builtin()
        Scheme eval-guile-scm
          C++ primitives
; eval-guile-c: Invokes primitive eval-builtin, the original eval function.
; eval-guile-scm: Invoked Scheme eval-guile, in turned invoked from C++ eval().  It is a Scheme version of the original C++ eval(), now (eval_builtin(), based on the C++ primitives.
; eval-guile: Invoked from C++ eval(). Invokes Scheme eval-guile-scm.
|#

#|
Prints a friendly greeting.
|#
(define do-hello
  (lambda ()
    (display "hello, world---from Scheme")
      (newline)))

#|
Prints a message passed as a string argument.
|#
(define do-print
  (lambda (text)
    (display text)
      (newline)))

#|
Enable/disable user-defined evaluation function.
Set parameters of the evaluation function.
|#
(define eval-guile-init
  (lambda ()
    (begin
      (eval-enable-userdef 1)
    )
  )
)

#|
Invokes primitive eval-builtin, the original eval function.
|#
(define eval-guile-c
  (lambda (board_handle)
    (begin (eval-builtin board_handle))
  )
)

#|
Invoked Scheme eval-guile, in turned invoked from C++ eval().
It is a Scheme version of the original C++ eval(), now (eval_builtin(),
based on the C++ primitives.
|#
(define eval-guile-scm
;int eval(const board_t * board) {
  (lambda (board-handle mat-handle pawn-handle mul-handle)
    (begin 

;   int opening, endgame;
;   material_info_t mat_info[1];
;   pawn_info_t pawn_info[1];
;   int mul[ColourNb];
;   int phase;
;   int eval;
;   int wb, bb;
;
;   ASSERT(board!=NULL);
;
;   ASSERT(board_is_legal(board));
;   ASSERT(!board_is_check(board)); // exceptions are extremely rare
;
;   // init
;
;   opening = 0;
;   endgame = 0;
      (define eval-result 0)
      (define mul-white 0)
      (define mul-black 0)

      (define phase (get-phase mat-handle))
      (define wb (board-get-piece-white board-handle))
      (define bb (board-get-piece-black board-handle))
      (define ValueDraw 0)
      (define ValueMate 30000)
      (define ValueEvalInf (- ValueMate 256))
;
;   // material
;
;   material_get_info(mat_info,board);
;
;   opening += mat_info->opening;
;   endgame += mat_info->endgame;
      (define dummy-result-1 (material-get-info board-handle mat-handle))
      (define mo (material-get-info-opening mat-handle))
      (define me (material-get-info-endgame mat-handle))
;
;   mul[White] = mat_info->mul[White];
;   mul[Black] = mat_info->mul[Black];
;
;   // PST
;
;   opening += board->opening;
;   endgame += board->endgame;
      (define dummy-result-2 (material-get-mul mat-handle mul-handle)) 
      (define bo (board-get-opening board-handle))
      (define be (board-get-endgame board-handle))
;
;   // pawns
;
;   pawn_get_info(pawn_info,board);
;
;   opening += pawn_info->opening;
;   endgame += pawn_info->endgame;
      (define dummy-result-3 (pawn-get-info board-handle pawn-handle))
      (define po (pawn-get-opening pawn-handle))
      (define pe (pawn-get-endgame pawn-handle))

      (define o (+ mo bo po))
      (define e (+ me be pe))

      ; where this come from?
      (define mat-mul-white (material-get-mul-white mat-handle))
      (define mat-mul-black (material-get-mul-black mat-handle))
;
;   // draw
;
;   eval_draw(board,mat_info,pawn_info,mul);
      (define dummy-result-4 (eval-draw board-handle mat-handle pawn-handle mul-handle)) 
      (set! mul-white (eval-draw-get-mul-white mul-handle))
      (set! mul-black (eval-draw-get-mul-black mul-handle))
;
;   if (mat_info->mul[White] < mul[White]) mul[White] = mat_info->mul[White];
      (if (< mat-mul-white mul-white) (begin (set! mul-white mat-mul-white)))
;   if (mat_info->mul[Black] < mul[Black]) mul[Black] = mat_info->mul[Black];
      (if (< mat-mul-black mul-black) (begin (set! mul-black mat-mul-black)))
;
;   if (mul[White] == 0 && mul[Black] == 0) return ValueDraw;
      (if (and (= mul-white 0) (= mul-black 0))
        (set! eval-result 0)
;
;   // eval
        (begin
;
;   eval_piece(board,mat_info,pawn_info,&opening,&endgame);
          (set! o (eval-piece-get-opening board-handle mat-handle pawn-handle o))
          (set! e (eval-piece-get-endgame board-handle mat-handle pawn-handle e))
;   eval_king(board,mat_info,&opening,&endgame);
          (set! o (eval-king-get-opening board-handle mat-handle o))
          (set! e (eval-king-get-endgame board-handle mat-handle e))
;   eval_passer(board,pawn_info,&opening,&endgame);
          (set! o (eval-passer-get-opening board-handle pawn-handle o))
          (set! e (eval-passer-get-endgame board-handle pawn-handle e))
;   eval_pattern(board,&opening,&endgame);
          (set! o (eval-pattern-get-opening board-handle o))
          (set! e (eval-pattern-get-endgame board-handle e))
;
;   // phase mix
;
;   phase = mat_info->phase;
;   eval = ((opening * (256 - phase)) + (endgame * phase)) / 256;
          (set! eval-result (quotient 
                              (+ 
                                (* 
                                  o 
                                  (- 256 phase)
                                ) 
                                (*
                                  e 
                                  phase
                                )
                              ) 
                              256
                            )
          )
;
;   // drawish bishop endgames
;
;   if ((mat_info->flags & DrawBishopFlag) != 0) {
          (if (is-set-draw-bishop-flag mat-handle)
            (begin
;
;      wb = board->piece[White][1];
;      ASSERT(PIECE_IS_BISHOP(board->square[wb]));
;
;      bb = board->piece[Black][1];
;      ASSERT(PIECE_IS_BISHOP(board->square[bb]));
;
;      if (SQUARE_COLOUR(wb) != SQUARE_COLOUR(bb)) {
              (if (not (= (square-colour wb) (square-colour bb)))
                (begin
;         if (mul[White] == 16) mul[White] = 8; // 1/2
                  (if (= mul-white 16) (set! mul-white 8))
;         if (mul[Black] == 16) mul[Black] = 8; // 1/2
                  (if (= mul-black 16) (set! mul-black 8))
;      }
                )
;   }
              )
            )
          )
;
;   // draw bound
;
;   if (eval > ValueDraw) {
          (if (> eval-result ValueDraw)
;      eval = (eval * mul[White]) / 16;
            (set! eval-result (quotient (* eval-result mul-white) 16))
;   } else if (eval < ValueDraw) {
            (if (< eval-result ValueDraw)
;      eval = (eval * mul[Black]) / 16;
              (set! eval-result (quotient (* eval-result mul-black) 16))
            )
;   }
          )
;
;   // value range
;
;   if (eval < -ValueEvalInf) eval = -ValueEvalInf;
          (if (< eval-result (- ValueEvalInf)) (set! eval-result (- ValueEvalInf)))         
;   if (eval > +ValueEvalInf) eval = +ValueEvalInf;
          (if (> eval-result ValueEvalInf) (set! eval-result ValueEvalInf))         
;
;   ASSERT(eval>=-ValueEvalInf&&eval<=+ValueEvalInf);
;
;   // turn
;
;   if (COLOUR_IS_BLACK(board->turn)) eval = -eval;
          (if (is-colour-turn-black board-handle) (set! eval-result (- eval-result)))
;
;   ASSERT(!value_is_mate(eval));
;
        )
      )
;   return eval;
      eval-result
    )
  )
;}

)

#|
Invoked from C++ eval().
Invokes Scheme eval-guile-scm.
; Invokes Scheme eval-guile-c a hook for C++ eval_builtin().
|#
(define eval-guile
  (lambda (board-handle mat-handle pawn-handle mul-handle)
    ;(begin (eval-guile-c board-handle))
    (begin (eval-guile-scm board-handle mat-handle pawn-handle mul-handle))
  )
)

