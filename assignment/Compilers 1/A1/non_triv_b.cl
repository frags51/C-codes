--Reverse a string, using the built in functions
(* Input: s (A String) *)
class Main inherits IO {
	-- Variables for storing input
	a: String;
  b: String <- ""; -- Initialize
  main() : Object {{
  	a <- in_string();
    out_string(rev(a));
    out_string("\n");
  }}; -- main function

  rev(s: String) : String {{
    
    let bb:Int <- s.length()-1 in { -- Binds variables for a group of expressions
      while 0 <= bb loop
        {
          b <- b.concat(s.substr(bb, 1)); -- Concatenate elemets from behind 
          bb <- bb - 1; -- Set
        } 
      pool;
    };
    b; -- The return expression
  }};
}; -- Main