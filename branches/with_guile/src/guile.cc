#include <stdio.h>
#include <stdlib.h>
#include <libguile.h>

void guile_hello(void)
{
    printf("GUILE enabled hello...\n");

    SCM func_symbol;
    SCM func;

    scm_init_guile();

    // Load the scheme function definition
    scm_c_primitive_load( "script.scm" );

    func_symbol = scm_c_lookup( "do-hello" );
    func = scm_variable_ref( func_symbol );

    scm_call_0( func );

    func_symbol = scm_c_lookup( "do-print" );
    func = scm_variable_ref( func_symbol );

    SCM text = scm_from_locale_string( "GUILE integration in GNU Chess" );
    scm_call_1( func, text );

    return;
}

