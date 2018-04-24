(*
	Stack Interpretor : Compilers 1 Assignment 1.
	cs16btech11038
*)

-- This class A2I has been taken from external examples, as was mentioned in the assignment.
-- This is for conversion of int to string and back.
class A2I {

     c2i(char : String) : Int {
	if char = "0" then 0 else
	if char = "1" then 1 else
	if char = "2" then 2 else
        if char = "3" then 3 else
        if char = "4" then 4 else
        if char = "5" then 5 else
        if char = "6" then 6 else
        if char = "7" then 7 else
        if char = "8" then 8 else
        if char = "9" then 9 else
        { abort(); 0; }  -- the 0 is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
     };

(*
   i2c is the inverse of c2i.
*)
    i2c(i : Int) : String {
	if i = 0 then "0" else
	if i = 1 then "1" else
	if i = 2 then "2" else
	if i = 3 then "3" else
	if i = 4 then "4" else
	if i = 5 then "5" else
	if i = 6 then "6" else
	if i = 7 then "7" else
	if i = 8 then "8" else
	if i = 9 then "9" else
	{ abort(); ""; }  -- the "" is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
     };

(*
   a2i converts an ASCII string into an integer.  The empty string
is converted to 0.  Signed and unsigned strings are handled.  The
method aborts if the string does not represent an integer.  Very
long strings of digits produce strange answers because of arithmetic 
overflow.
*)
     a2i(s : String) : Int {
        if s.length() = 0 then 0 else
	if s.substr(0,1) = "-" then ~a2i_aux(s.substr(1,s.length()-1)) else
        if s.substr(0,1) = "+" then a2i_aux(s.substr(1,s.length()-1)) else
           a2i_aux(s)
        fi fi fi
     };

(*
  a2i_aux converts the usigned portion of the string.  As a programming
example, this method is written iteratively.
*)
     a2i_aux(s : String) : Int {
	(let int : Int <- 0 in	
           {	
               (let j : Int <- s.length() in
	          (let i : Int <- 0 in
		    while i < j loop
			{
			    int <- int * 10 + c2i(s.substr(i,1));
			    i <- i + 1;
			}
		    pool
		  )
	       );
              int;
	    }
        )
     };

(*
    i2a converts an integer to a string.  Positive and negative 
numbers are handled correctly.  
*)
    i2a(i : Int) : String {
	if i = 0 then "0" else 
        if 0 < i then i2a_aux(i) else
          "-".concat(i2a_aux(i * ~1)) 
        fi fi
    };
	
(*
    i2a_aux is an example using recursion.
*)		
    i2a_aux(i : Int) : String {
        if i = 0 then "" else 
	    (let next : Int <- i / 10 in
		i2a_aux(next).concat(i2c(i - next * 10))
	    )
        fi
    };

};

(* Linked list implementation of stack!
	This stack class manages things for an empty stack.
 *)
class Stack{ -- Sort of recursive definitions
	isEmpty() : Bool {true};
	peek() : String {""};
	pop() : Stack {self };
	add(d: String) : Stack { -- Push onto the stack
		(new Node).init(d, self)
	};
};

(* This class manages thongs for a non empty Stack. pop() : stack <- stack.tail();
*)
class Node inherits Stack{
	data: String; -- The head of the stack, assuming starting from this node. :: I,e, data in this node
	rem: Stack; -- the tail of this list

	isEmpty() : Bool {false};
	peek() : String {data};
	pop() : Stack {rem};
	init(d: String, r: Stack) : Stack{{
		data <- d;
		rem <- r;
		self;
	}};
};
-- The main function
class Main inherits IO{
	inp: String <- "";
	s : Stack <- new Stack;

	main() : Object{{ -- main fxn
		
		while not inp="x" loop -- Just keep looping until x instruction
		{	
			-- Prompt for input
			out_string(">");
			inp<-in_string();
			if inp="x" then "nothing"
			else -- input is not x
				if inp="d" then 
					new PrintCmd.print(s)
				else 
					if not inp="e" then -- Assumed to be valid symbol, except "d"
						s <- s.add(inp)
					else -- inp = "e" 
						let todo: String in{
							todo <- s.peek();
							s <- s.pop();
							if todo="+" then
								s <- new PlusCmd.plus(s).getStack()
							else 
								if todo="s" then
									s <- new SwapCmd.swap(s).getStack()
								else
									s <- s.add(todo) -- Need to add todo back to the stack - its an integer
								fi
							fi;
						}
					fi
				fi
				
			fi;
		}
		pool;
	}}; -- main fxn
};

-- Classes can inherit only from one class.
class StackCmd inherits IO{ -- A class that evaluates commands
	cmdsStack: Stack; -- Used to return this value, cant modify local copy
	isEmpty() : Bool {true}; -- A chain of commands?
	plus(s: Stack) : StackCmd {self}; -- default implementation
	swap(s: Stack) : StackCmd {self};
	print(s: Stack) : StackCmd {self};
	getStack() : Stack{ cmdsStack };   
}; 

class PlusCmd inherits StackCmd{ -- What a plus command should do. (stack has 2 numbers on top)
	isEmpty(): Bool {false};
	plus(s: Stack) : StackCmd{{
		let b: Stack, c: Stack, b1: Int, c1: Int, r: Int in {
			b <- s;
			s <- s.pop();
			c <- s;
			s <- s.pop();
			b1 <- new A2I.a2i(b.peek()); -- string to num
			c1 <- new A2I.a2i(c.peek()); -- string to num
			r <- b1 + c1;
			s <- s.add(new A2I.i2a(r)); -- num to string
		};
		cmdsStack <- s;
		self;
	}};
}; 

class PrintCmd inherits StackCmd{ -- What print should do
	isEmpty(): Bool {false};
	print(s: Stack) : StackCmd{{
		let s1 : Stack <- s in 
			while(not s1.isEmpty()) loop{
				out_string(s1.peek()).out_string("\n");
				s1 <- s1.pop();
			} pool; -- while loop ends
		cmdsStack <- s;
		self;
	}};
};

class SwapCmd inherits StackCmd{ -- What swap should do
	isEmpty(): Bool {false};
	swap(s: Stack) : StackCmd{{
		let temp: String, temp2: String in { -- Follow standard procedure to swap to objects
			temp <- s.peek();
			s <- s.pop();
			temp2 <- s.peek();
			s <-s.pop();
			s <- s.add(temp);
			s <- s.add(temp2);
		};
		cmdsStack <- s;
		self;
	}};
};