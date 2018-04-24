-- Computes max of 3 natural numbers, outputs it
(* Input: n (integer) *)
class Main inherits IO {
	-- Variables for storing input
	a: Int <- 0;
	b: Int <- 0;
  c: Int <- 0;

  main() : Object {{
     
  	a <- in_int(); -- get input
    b <- in_int();
    c <- in_int();
  	if a < b then
      if b < c then
        out_int(c)
      else 
        out_int(b)
      fi
    else
      if a<c then
        out_int(c)
      else 
        out_int(a)
      fi
    fi;
  }}; -- main function
}; -- Main