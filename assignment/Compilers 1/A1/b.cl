-- Outputs a substring of given length of the given string.
-- Input : length (Int) the_string (String) * 
class Main inherits IO {
  main() : Object {
  	-- Bind x, y to user input
  	let x: Int <- in_int(), 
  	z: String <- in_string() in {
  		if z.length() < x then
        out_string("Error! Enter number less than length!")
      else 
        out_string(z.substr(0, x))
      fi;
    } -- in
  };
}; -- Main