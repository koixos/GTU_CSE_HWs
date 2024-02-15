module comparator_2bit (
	output
		greater,
		less,
		equal,
	input [1:0] ain,
	input [1:0] bin
);
	
	wire [2:0] w;
	wire [5:0] tmp_g;
	wire [5:0] tmp_l;
	wire [5:0] tmp_e;
	
	not (tmp_g[0], bin[0]);					//t1: 'b0
	and (tmp_g[1], ain[0], tmp_g[0]);	//t2: a0 & 'b0
	xnor (tmp_g[2], ain[1], bin[1]);		//t3: a1 ~^ b1
	not (tmp_g[3], bin[1]);					//t4: 'b1
	and (tmp_g[4], ain[1], tmp_g[3]);	//t5: a1 & 'b1
	and (tmp_g[5], tmp_g[1], tmp_g[2]);	//t6: (a1 ~^ b1) & (a0 & 'b0)
	or (greater, tmp_g[4], tmp_g[5]);	//G: (a1 & 'b1) | ((a1 ~^ b1) & (a0 & 'b0))
	
	not (tmp_l[0], ain[0]);					//t1: 'a0
	and (tmp_l[1], tmp_l[0], bin[0]);	//t2: 'a0 & b0
	xnor (tmp_l[2], ain[1], bin[1]);		//t3: a1 ~^ b1
	not (tmp_l[3], ain[1]);					//t4: 'a1
	and (tmp_l[4], tmp_l[3], bin[1]);	//t5: 'a1 & b1
	and (tmp_l[5], tmp_l[1], tmp_l[2]);	//t6: (a1 ~^ b1) & ('a0 & b0)
	or (less, tmp_l[4], tmp_l[5]);		//L: ('a1 & b1) | ((a1 ~^ b1) & ('a0 & b0))
		
	xnor (tmp_e[0], ain[0], bin[0]);	//t1: a0 ~^ b0
	xnor (tmp_e[1], ain[1], bin[1]);	//t2: a1 ~^ b1
	and (equal, tmp_e[0], tmp_e[1]);	//E: (a1 ~^ b1) & (a0 ~^ b0)
	
endmodule