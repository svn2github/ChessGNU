#include <stdio.h>
#include <stdlib.h>
#include <libguile.h>

#include "engine/board.h"
#include "engine/eval.h"

using engine::board_t;


void guile_hello(void)
{
    printf("GUILE enabled hello...\n");

    SCM func_symbol;
    SCM func;

    scm_init_guile();

//    scm_c_define_gsubr( "eval_builtin", 0, 0, 0, engine::eval_builtin );

    // Load the scheme function definition
    scm_c_primitive_load( "script.scm" );

    func_symbol = scm_c_lookup( "do-hello" );
    func = scm_variable_ref( func_symbol );

    scm_call_0( func );

    func_symbol = scm_c_lookup( "do-print" );
    func = scm_variable_ref( func_symbol );

    SCM text = scm_from_locale_string( "GUILE integration in GNU Chess" );
    scm_call_1( func, text );

//    func_symbol = scm_c_lookup( "eval-guile" );
//    engine::func_eval_guile = scm_variable_ref( func_symbol );

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

