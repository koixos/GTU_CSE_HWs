module cla_4bit (
	output [3:0] sum,
	output cout,
	input [3:0] ain,
	input [3:0] bin,
	input cin
);
	
	wire [3:0] prop;
	wire [3:0] gen;
	wire carry;
	wire [4:0] tmp;
	
	cla_2bit cla1 (
		.sum(sum[1:0]),
		.cout(carry),
		.ain(ain[1:0]),
		.bin(bin[1:0]),
		.cin(cin)
	);
	
	cla_2bit cla2 (
		.sum(sum[3:2]),
		.cout(cout),
		.ain(ain[3:2]),
		.bin(bin[3:2]),
		.cin(carry)
	);
	
	/*
	and (tmp[0], prop[0], cin);				//t0: p0 & cin
	or (carry[0], gen[0], tmp[0]);			//c1: g0 | (p0 & cin)
	
	and (tmp[1], prop[1], prop[0], cin);	//t1: p1 & p0 & cin
	and (tmp[2], prop[1], gen[0]);			//t2: p1 & g0
	or (carry[1], gen[1], tmp[2], tmp[1]);	//c2: g1 | (p1 & g0) | (p1 & p0 & cin)

	and (tmp[3], prop[2], prop[1], prop[0], cin);	//t3: p2 & p1 & p0 & cin
	and (tmp[4], prop[2], prop[1], gen[0]);			//t4: p2 & p1 & g0
	and (tmp[5], prop[2], gen[1]);						//t5: p2 & g1
	or (carry[2], gen[2], tmp[5], tmp[4], tmp[3]);	//c3: g2 | (p2 & g1) | (p2 & p1 & g0) | (p2 & p1 & p0 & cin)

	and (tmp[6], prop[3], prop[2], prop[1], prop[0], cin);
	and (tmp[7], prop[3], prop[2], prop[1], gen[0]);
	and (tmp[8], prop[3], prop[2], prop[1], gen[1]);
	and (tmp[9], prop[3], gen[2]);
	or (cout, gen[3], tmp[6], tmp[7], tmp[8], tmp[9]);
	
	xor (sum[0], prop[0], cin);
	xor (sum[1], prop[1], carry[0]);
	xor (sum[2], prop[2], carry[1]);
	xor (sum[3], prop[3], carry[2]);*/
	
endmodule