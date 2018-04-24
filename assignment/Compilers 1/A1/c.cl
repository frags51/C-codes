-- Swaps two numbers
(* Input: Two ingers, swap their values. *)
class Main inherits IO {
	-- Variables for storing input
	a: Int <- 0;
	b: Int <- 0;
	-- Temporary variable for swapping.
	c: Int <- 0;
  main() : Object {{
	a <- in_int(); -- Get Input
	b <- in_int(); 	-- Get Input
	-- Swapppp
	c <- a;
	a <- b;
	b <- c;
	out_string("a: ");
	out_int(a);
	out_string(" b: ");
	out_int(b);

  }};
}; -- Main