(define do-hello
  (lambda ()
    (display "hello, world---from Scheme")
      (newline)))

(define do-print
  (lambda (text)
    (display text)
      (newline)))

(define eval-guile
  (lambda ()
    (begin (eval_builtin))
  )
)
