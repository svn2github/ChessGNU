#! GNU Chess 6 - eval.scm: Scheme evaluation

   Copyright (c) 2001-2015 Free Software Foundation, Inc.

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

(define do-hello
  (lambda ()
    (display "hello, world---from Scheme")
      (newline)))

(define do-print
  (lambda (text)
    (display text)
      (newline)))

(define eval-guile-c
  (lambda (board_handle)
    (begin (eval-builtin board_handle))
  )
)

(define eval-guile-scm
  (lambda (board-handle)
    (begin 
      (define material-handle (material-get-info board-handle))
      (define mo (material-get-info-opening material-handle))
      (define me (material-get-info-endgame material-handle))
      (define mul-handle (material-get-mul material-handle)) 
      (define bo (board-get-opening board-handle))
      (define be (board-get-endgame board-handle))
      (define pawn-handle (pawn-get-info board-handle))
      (define po (pawn-get-opening pawn-handle))
      (define pe (pawn-get-endgame pawn-handle))
      (define o (+ mo bo po))
      (define e (+ me be pe))
      (define mul-white 0)
      (define mul-black 0)
      (define mat-mul-white (material-get-mul-white material-handle))
      (define mat-mul-black (material-get-mul-black material-handle))
      (define phase (get-phase material-handle))
      (define wb (board-get-piece-white board-handle))
      (define bb (board-get-piece-black board-handle))
      (define ValueDraw 0)
      (define ValueMate 30000)
      (define ValueEvalInf (- ValueMate 256))
      (define eval-result 0)
      (set! mul-handle (eval-draw board-handle material-handle pawn-handle mul-handle)) 
      (set! mul-white (eval-draw-get-mul-white mul-handle))
      (set! mul-black (eval-draw-get-mul-black mul-handle))
      (if (< mat-mul-white mul-white) (begin (set! mul-white mat-mul-white)))
      (if (< mat-mul-black mul-black) (begin (set! mul-black mat-mul-black)))
      (if (and (= mul-white 0) (= mul-black 0))
        (set! eval-result 0)
        (begin
          (set! o (eval-piece-get-opening board-handle material-handle pawn-handle o))
          (set! e (eval-piece-get-endgame board-handle material-handle pawn-handle e))
          (set! o (eval-king-get-opening board-handle material-handle o))
          (set! e (eval-king-get-endgame board-handle material-handle e))
          (set! o (eval-passer-get-opening board-handle pawn-handle o))
          (set! e (eval-passer-get-endgame board-handle pawn-handle e))
          (set! o (eval-pattern-get-opening board-handle o))
          (set! e (eval-pattern-get-endgame board-handle e))
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
          (if (is-set-draw-bishop-flag material-handle)
            (begin
              (if (not (= (square-colour wb) (square-colour bb)))
                (begin
                  (if (= mul-white 16) (set! mul-white 8))
                  (if (= mul-black 16) (set! mul-black 8))
                )
              )
            )
          )
          (if (> eval-result ValueDraw)
            (set! eval-result (quotient (* eval-result mul-white) 16))
            (if (< eval-result ValueDraw)
              (set! eval-result (quotient (* eval-result mul-black) 16))
            )
          )
          (if (< eval-result (- ValueEvalInf)) (set! eval-result (- ValueEvalInf)))         
          (if (> eval-result ValueEvalInf) (set! eval-result ValueEvalInf))         
          (if (is-colour-turn-black board-handle) (set! eval-result (- eval-result)))
        )
      )
      eval-result
    )
  )
)

(define eval-guile
  (lambda (board-handle)
    (begin (eval-guile-scm board-handle))
  )
)
