-- Create a Vector type, and do some operations.

class Vector {
  -- Member variables, the coordinates
  x: Int;
  y: Int;
  z: Int;
  -- Initialize with given values.
  init(a: Int, b: Int, c: Int) : Vector{{
      x <- a;
      y <- b;
      z <- c;
      self;
    }};
    -- Getters for the 3 coordinates
    getX() : Int {
      x
    };
    getY() : Int {
      y
    };
    getZ() : Int{
      z
    };
    -- Returns dotprdd
   dotProd(o: Vector): Int{
    self.getX()*o.getX() + self.getY()*o.getY() + self.getZ()*o.getZ()
   };
   -- Vector addition
   add(o : Vector) : Vector{
      (new Vector).init(self.getX()+o.getX(), self.getY()+o.getY(),self.getZ()+o.getZ())
   };
};

class Main inherits IO {
	-- Variables for storing input
	v1: Vector;
  v2: Vector;
  choice: Int;
  main() : Object {{
    -- Basic I/O
    out_string("Enter x1, y1, z1 and x2, y2, z2: 6 ints for defining two vectors.");
    v1 <- (new Vector).init(in_int(), in_int(),in_int());
    v2 <- (new Vector).init(in_int(), in_int(),in_int());
    out_string("Enter 1 to add both or 2 to compute dot product: ");
    -- Get user choice:
    choice <- in_int();
    if choice = 2 then{
      out_string("Dot product is: ");
      out_int(v1.dotProd(v2)); -- Function invocation
      out_string("\n");
    }
    else {
      out_string("Vector addition result is: ");
      out_int((v1.add(v2)).getX());
      out_string(" ,");
      out_int((v1.add(v2)).getY());
      out_string(", ");
      out_int((v1.add(v2)).getZ());
      out_string("\n");  
    }
    fi;
  }}; -- main function
}; -- Main