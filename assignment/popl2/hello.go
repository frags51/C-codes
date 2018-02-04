/**
*	DO NOT INPUT ANY NUMBER WITH ZEROES AT THE FRONT
*	Using Karatsuba's Algorithm To compute product of long ints.
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
	fmt.Printf("%s\n", mult("2598", "69198"))
	/*
	var tCases int
	fmt.Scanf("%d", &tCases)
	for t:=0; t < tCases; t++ { // Eval each test case.
		var n int // degree
		var m int // Number of points to evaluate at
		fmt.Scanf("%d", &n)
		var coeffs = make([]string, 0, n+1) // The coefficients of the polynomial
		var a string // Temporary
		for i:=0; i <= n; i++{ // take the coeffs as input
			fmt.Scanf("%s ", &a)
			coeffs = append(coeffs, pad(a))
		} // coeff scanning loop
		
		fmt.Scanf("%d", &m)
		var toEval = make([]string, 0, m)

		for i:=0; i < n; i++{ // take the coeffs as input
			fmt.Scanf("%s ", &a)
			toEval = append(toEval, pad(a))
		} 

	} // test case loop */
} // main

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

func mult(n1 string, n2 string) string{
	// First make len power of 2 and equal
	if(len(n1)>=len(n2)){
		n1=pad(n1)
		n2=pad2(n2, len(n1))
	} else{
		n2=pad(n2)
		n1=pad2(n1, len(n2))
	}

	return multDriver(n1, n2)
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
		//fmt.Println("Done driver for: n1 =", n1, " len: ", len(n1), " n2=", n2, " len ", len(n2))
		//fmt.Println("------- Retd 1 len: ", string(result), " of len ", len(result))
		return string(result)
	} else{ // in case len >1
		//fmt.Printf("%s, %s\n", n1, n2)
		l:=int(len(n1)/2)
		al:=n1[:l]
		ar:=n1[l:]
		bl:=n2[:l]
		br:=n2[l:]
		//fmt.Println("l: ", l, "> al:", al, " bl:", bl)
		albl:=mult(al, bl)
		
		arbr:=mult(ar,br)
		//fmt.Println("albl: ", albl, "arbr", arbr, " len", len(arbr))
		a_:=add(al, ar)
		b_:= add(bl, br)
		//fmt.Println(">>>", "al: ",al," ar: ", ar, " sum:", a_, " ; bl:", bl, " br:",br," sum:", b_)
		//fmt.Println("Len of sum: ", len(a_))
		midMult:=mult(a_, b_)
		
		midMult,_ = sub(midMult, albl) // these subs should be positive only
		midMult,_ = sub(midMult, arbr)
		//fmt.Println("Mid term: ", midMult)

		bs1:=bitshift(midMult, l)
		bs2:=bitshift(albl, 2*l)
		//fmt.Println("bs1: ", bs1)
		//fmt.Println("bs2: ", bs2)

		res:=add(arbr, bs1)
		//fmt.Println(">Res: ",res)
		res=add(res, bs2)
		//fmt.Println(">------------------------ Final Res: ",res," for" ,n1, " and ", n2, "<<<<<<--")
		
		var j int
		for j=0; j<len(res); j++{ // truncate zeroes?
			if(res[j]!='0') {break}
		}
		if(j<len(res)) {res=res[j:]}

		return res
	} // len > 1
	
}