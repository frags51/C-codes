--Computes sum of first 'n' natural numbers
(* Input: n (integer) *)
class Main inherits IO {
	-- Variables for storing input
	a: Int <- 0;
	b: Int <- 0;
  main() : Object {{
  	a <- in_int(); -- get input
  	while 0 < a -- no >= operator
  	loop
  	{ -- statement block
  		b <- b + a; -- Add, and decrement counter
  		a <- a-1;
  	}
  	pool; -- end loop (^-^)
  	out_int(b); -- spit it out
  }}; -- main function
}; -- Main