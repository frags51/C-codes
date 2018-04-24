-- Perform basic arithmetic on user input, and print it (Without any fance formatting, spaces etc..
-- Input : Two integers
class Main inherits IO {
  main() : Object {
  	-- Bind x, y to user input
  	let x: Int <- in_int(), 
  	y: Int <- in_int(),
  	-- Bind z, w, y to the result of arithmentic operations
  	z: Int <- x + y,
  	w: Int <- x - y,
  	u: Int <- x * y in {
  		-- Output the results
  		-- No fancy formatting to make mips code simpler.
    	--out_string("Hello, world!\n");
    	--out_string("Here are the results: ");
    	out_int(z);
    	--out_string("\n");
    	out_int(w);
    	--out_string("\n");
    	out_int(u);
    	--out_string("\n");
    }
  };
}; -- Main