/**
*	DO NOT INPUT ANY NUMBER WITH ZEROES AT THE FRONT
*	Using Parallel Karatsuba's Algorithm To compute product of long ints.
*	Computes albl, arbr, and midterm parallely and passses it back through a channel
*	CS16BTECH11038 Supreet Singh
*/
package main;

import ("fmt")
/*
type Numb struct{ // Struct to store numbers
	val string
	neg int
} */

func main(){
	//p := make(chan string, 1)
	//go mult("9849516516499298598298298498295292985985929924894298259674564568", "499249842942984299841984691998", p)
	//fmt.Printf("%s\n", pow("5985985529829984987579248298292894298418941659", 17))
	
	var tCases int
	fmt.Scanf("%d", &tCases)
	for t:=0; t < tCases; t++ { // Eval each test case.
		var n int // degree
		var m int // Number of points to evaluate at
		fmt.Scanf("%d", &n)
		var coeffs = make([]string, 0, n+1) // The coefficients of the polynomial
		var a string // Temporary
		for i:=0; i <= n; i++{ // take the coeffs as input ==n+1
			fmt.Scanf("%s", &a)

			coeffs = append(coeffs, a)
		} // coeff scanning loop
		
		fmt.Scanf("%d", &m)
		var toEval = make([]string, 0, m)

		for i:=0; i < m; i++{ // take the coeffs as input
			fmt.Scanf("%s", &a)
			toEval = append(toEval, a)
		}
		
		// ********* Start printing output! ****************** //
		fmt.Println("#")	
		for i:=0;i<m;i++{ // eval loop
			
			ans:=coeffs[n] // a0
			//gotback:=make([]chan string, 1)
			var chans [4]chan string
			for i := range chans {
   				chans[i] = make(chan string, 1)
			}

			var x, x2, x3, x4 string
			x=toEval[i]
			// Make X positive and set negFlagX
	
			blah:=make(chan string, 1)
			go mult(x,x,blah)
			x2 = <-blah
			go mult(x2,x,blah)
			x3 = <-blah
			go mult(x3,x,blah)
			x4 = <-blah
			x4orig:=x4

			for j:=n-1;j>=0;j-=4{ // each value of x
				go mult(coeffs[j], x, chans[0])
				if(j-1>=0) {go mult(coeffs[j-1], x2, chans[1])}		
				if(j-2>=0) {go mult(coeffs[j-2], x3, chans[2])}
				if(j-3>=0) {go mult(coeffs[j-3], x4, chans[3])}
				for i := range chans {
					if(j-i>=0){
					gh:= <- chans[i]
					//fmt.Println("*********\nAdded: ", gh)
					//fmt.Println("x2 was: ", x2, "\ncoeff was: ",coeffs[j-i], "\n")
					ans=signedAdder(ans, gh)
					//fmt.Println("now: ", ans, "\n") 
					} // if
			 	}
				if(j-4>=0){
					go mult(x4orig,x,blah)
					x = <-blah
					go mult(x4orig,x2,blah)
					x2 = <-blah
					go mult(x4orig,x3,blah)
					x3 = <-blah
					go mult(x4orig,x4,blah)
					x4 = <-blah
				}
				
			} // coeff loop
			

			var rr int
			for rr=0; rr<len(ans); rr++{ // truncate zeroes?
				if(ans[rr]!='0') {break}
			}
			if(rr<len(ans)) {ans=ans[rr:]} else{ans="0"}
		

			fmt.Println(ans)
		} // eval loop

	} // test case loop 
	fmt.Println("#")
} // main

func signedAdder(n1 string, n2 string) string{
	nf1, nf2:=0, 0
	if(n1[0]=='-'){
		nf1=1
		n1=n1[1:]
	}
	if(n2[0]=='-'){
		nf2=1
		n2=n2[1:]
	}
	if(nf1==nf2 && nf2==0){
		return add(n1, n2)
	} else if(nf1==nf2 && nf1==1){ // both neg
		return "-"+add(n1, n2)
	} else if(nf1==1){ // number 1 is negative
		res, neg := sub(n2, n1)
		if(neg!=0) {return "-"+res} else {return res}
	} else{ // number 2 is negatove
		res, neg := sub(n1, n2)
		if(neg!=0) {return "-"+res} else {return res}
	}
}

func pad(s string) string{ // Here s is a positive number
	var x = len(s)
	if(x==1 || x==2) {
		return s
	} else if(x<=4){
		return fmt.Sprintf("%04s", s )
	} else if(x<=8){
		return fmt.Sprintf("%08s", s)
	} else if(x<=16){
		return fmt.Sprintf("%016s", s)
	} else if(x<=32){
		return fmt.Sprintf("%032s", s)
	} else if(x<=64){
		return fmt.Sprintf("%064s", s)
	} else if(x<=128){
		return fmt.Sprintf("%0128s", s)
	} else if(x<=256){
		return fmt.Sprintf("%0256s", s)
	} else if(x<=512){
		return fmt.Sprintf("%0512s", s)
	} else if(x<=1024){
		return fmt.Sprintf("%01024s", s)
	}
	return s
} //pad

func pad2(s string, d int) string{ // padding with argument given as total final length
	return fmt.Sprintf("%0*s", d, s)
}

func strrev(s string) string{ // To revers a string
	runes := []rune(s)
    for i, j := 0, len(runes)-1; i < j; i, j = i+1, j-1 {
        runes[i], runes[j] = runes[j], runes[i]
    }
    return string(runes)
} //rev

func bitshift(s string, n int) string{ // ON A NON REVERSED NUMBER!, equal to number x 10^n
	return strrev(fmt.Sprintf("%0*s", n+len(s), strrev(s)))
}

func sub(n1 string, n2 string) (string, int8){ // Does the neccessary assertions for subDriver
	var j int
	for j=0; j<len(n1); j++{ // truncate zeroes?
		if(n1[j]!='0') {break}
	}
	if(j<len(n1)) {n1=n1[j:]} else{n1="0"}

	for j=0; j<len(n2); j++{ // truncate zeroes?
		if(n2[j]!='0') {break}
	}
	if(j<len(n2)) {n2=n2[j:]} else{n2="0"}

	
	if(len(n1)>len(n2)){
		n2=pad2(n2, len(n1))
		n1=strrev(n1)
		n2=strrev(n2)
		return strrev(subDriver(n1, n2)), 0
	} else if(len(n1)<len(n2)){ // n1<n2
		n1=pad2(n1, len(n2))
		n1=strrev(n1)
		n2=strrev(n2)
		return strrev(subDriver(n2, n1)), 1
	} else{
		n1=strrev(n1)
		n2=strrev(n2)
		for i:=len(n1)-1; i>=0; i--{
            if(n1[i] < n2[i]) {return strrev(subDriver(n2, n1)), 1
			} else if(n1[i] > n2[i]) {return strrev(subDriver(n1, n2)), 0
			} else if(n1[i]==n2[i] && i == 0) { return "0", 0}
        }//for
	} // else
	return "0", 0// This part of code is never reached
} // sub

func subDriver(n1 string, n2 string) string{ // Subtract two numbers s.t n1>n2, lenght is same (padding)
	// Numbers are reversed once, and len is equal!
	// Numbers are positive 
	// Returns the reversed Number
	var carry int8 // bytes are unsigned, and so are 'chars' string[i] in golang
	carry = 0
	result:= make([] byte, len(n1), len(n1))
	for i:=0; i< len(n1); i++{
		if((carry==0 && n1[i]>=n2[i]) || (carry==-1 && n1[i]>=(n2[i]+1))){
			result[i]=n1[i]-n2[i]+'0'
			if(carry==-1) {result[i] = result[i]-1}
			carry=0
		} else{
			result[i]=n1[i]-n2[i]+10+'0'
			if(carry==-1) {result[i] = result[i]-1}
			carry = -1
		}
	} // for loop
	/*var j int
	for j=len(result)-1; j>=0; j--{ // truncate zeroes?
		if(result[j]!='0') {break}
	}
	if(j>=0) {result=result[:(j+1)]}*/
	return string(result)
} // subDriver

func add(n1 string, n2 string) string{
	var j int
	for j=0; j<len(n1); j++{ // truncate zeroes?
		if(n1[j]!='0') {break}
	}
	if(j<len(n1)) {n1=n1[j:]}  else{n1="0"}

	for j=0; j<len(n2); j++{ // truncate zeroes?
		if(n2[j]!='0') {break}
	}
	if(j<len(n2)) {n2=n2[j:]}  else{n2="0"}

	if(len(n1)>len(n2)){
		n2=pad2(n2, len(n1))
	} else{
		n1=pad2(n1, len(n2))
	}
	return strrev(addDriver(strrev(n1), strrev(n2)))
}

func addDriver(n1 string, n2 string) string{ // Does the actuacl addition
	//Equal Sized Numbers, positive, ARE REVERSED!!!!
	var carry uint8 // carry can only be positive in this case
	carry = 0
	result:= make([] byte, len(n1), len(n1)+1) // Since addition can increase length by atmost 1
	var i int
	for i=0; i < len(n1); i++{
		result[i] = ((n1[i]+n2[i]+carry - '0' - '0')%10)+'0';
      	carry = (n1[i]+n2[i]+carry -'0' - '0')/10;
	}
	if(carry!=0){
		//result[i]=carry+'0'
		result = append(result, carry+'0')
		carry=0
	} 
	/*var j int
	for j=len(result)-1; j>=0; j--{ // truncate zeroes?
		if(result[j]!='0') {break}
	}
	if(j>=0) {result=result[:(j+1)]} */
	return string(result)
}

func mult(n1 string, n2 string, res chan string) {
	// Make positive numbers
	nf1, nf2:=0, 0
	if(n1[0]=='-'){
		nf1=1
		n1=n1[1:]
	}
	if(n2[0]=='-'){
		nf2=1
		n2=n2[1:]
	}

	var j int
	for j=0; j<len(n1); j++{ // truncate zeroes?
		if(n1[j]!='0') {break}
	}
	if(j<len(n1)) {n1=n1[j:]}  else{n1="0"}

	for j=0; j<len(n2); j++{ // truncate zeroes?
		if(n2[j]!='0') {break}
	}
	if(j<len(n2)) {n2=n2[j:]}  else{n2="0"}

	// First make len power of 2 and equal
	if(len(n1)>=len(n2)){
		n1=pad(n1)
		n2=pad2(n2, len(n1))
	} else{
		n2=pad(n2)
		n1=pad2(n1, len(n2))
	}

	//return multDriver(n1, n2)
	if(nf1==nf2) {res <- multDriver(n1, n2) } else {res <- ("-"+multDriver(n1, n2))}
}

func multDriver(n1 string, n2 string) string{ // Lenght is an even integer and is equal
	//takes and returns unreversed number
	var carry uint8
	carry = 0
	result:= make([] byte, len(n1)+1)
	//fmt.Println(">>>>> Recd: ", n1, " and ", n2, "<<<<<<<<<<<<<<<<<<<")
	if(len(n1)==1){ // if len is equal to 1 then do this
		var a uint8
		a = (n1[0]-'0')*(n2[0]-'0')
		carry = uint8(a)
		carry /= 10
		result[1] = a%10 + '0'
		result[0]=carry+'0'
		//fmt.Println(">>>>Done driver for: n1 =", n1, " len: ", len(n1), " n2=", n2, " len ", len(n2))
		//fmt.Println("------- Retd 1 len: ", string(result), " of len ", len(result))
		var j int
		for j=0; j<len(result); j++{ // truncate zeroes?
			if(result[j]!='0') {break}
		}
		if(j<len(result)) {result=result[j:]} else {return "0"}

		return string(result)
	} else{ // in case len >1
		//fmt.Printf("%s, %s\n", n1, n2)
		l:=int(len(n1)/2)
		al:=n1[:l]
		ar:=n1[l:]
		bl:=n2[:l]
		br:=n2[l:]
		//fmt.Println("l: ", l, "> al:", al, " bl:", bl)

		// ***** PARALLEL EXEC ... Three channels to return results to ************ //
		d1 := make(chan string, 1)
		d2:= make(chan string, 1)
		d3 := make(chan string, 1)

		//albl:=mult(al, bl)
		go mult(al, bl, d1)
		//arbr:=mult(ar,br)
		go mult(ar,br, d2)

		var j int
		/*for j=0; j<len(albl); j++{ // truncate zeroes?
			if(albl[j]!='0') {break}
		}
		if(j<len(albl)) {albl=albl[j:]}

		for j=0; j<len(arbr); j++{ // truncate zeroes?
			if(arbr[j]!='0') {break}
		}
		if(j<len(arbr)) {arbr=arbr[j:]}
		*/

		//fmt.Println("albl: ", albl, "arbr", arbr, " len", len(arbr))
		a_:=add(al, ar)
		b_:= add(bl, br)
		//fmt.Println(">>>", "al: ",al," ar: ", ar, " sum:", a_, " ; bl:", bl, " br:",br," sum:", b_)
		//fmt.Println("Len of sum: ", len(a_))
		
		//midMult:=mult(a_, b_)
		go mult(a_, b_, d3)
		
		albl:= <-d1
		arbr:= <-d2
		midMult:= <- d3

		midMult,_ = sub(midMult, albl) // these subs should be positive only
		midMult,_ = sub(midMult, arbr)
		//fmt.Println("Mid term: ", midMult," for" ,n1, " and ", n2,"!!")

		bs1:=bitshift(midMult, l)
		bs2:=bitshift(albl, 2*l)
		//fmt.Println("bs1: ", bs1)
		//fmt.Println("bs2: ", bs2)

		res:=add(arbr, bs1)
		//fmt.Println(">Res: ",res)
		res=add(res, bs2)
		//mt.Println(">------------------------ Final Res: ",res," for" ,n1, " and ", n2, "<<<<<<--\n")
		
		//var j int
		for j=0; j<len(res); j++{ // truncate zeroes?
			if(res[j]!='0') {break}
		}
		if(j<len(res)) {res=res[j:]}

		return res
	} // len > 1
	
}

func pow(x string, n int) string{ //returns x^n
	if(n==0) {return "1"}
	p := make(chan string, 1)
	if(n%2==0) {
		go mult(x,x, p)
		return pow(<- p, n/2)
	} else{
		p2:=make(chan string, 1)
		go mult(x,x, p)
		mult(x, pow(<-p, n/2), p2)
		return <-p2
	}
}