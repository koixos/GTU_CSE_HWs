module cla_2bit (
	output [1:0] sum,
	output cout,
	input [1:0] ain,
	input [1:0] bin,
	input cin
);
	
	wire [1:0] prop;
	wire [1:0] gen;
	wire carry;
	wire [2:0] tmp;
	
	prop_gen_generator genr1 (
		.prop(prop[0]),
		.gen(gen[0]),
		.ain(ain[0]),
		.bin(bin[0])
	);
	
	prop_gen_generator genr2 (
		.prop(prop[1]),
		.gen(gen[1]),
		.ain(ain[1]),
		.bin(bin[1])
	);
	
	and (tmp[0], prop[0], cin);				//t0: p0 & cin
	or (carry, gen[0], tmp[0]);				//c1: g0 | (p0 & cin)
	
	and (tmp[1], prop[1], prop[0], cin);	//t1: p1 & p0 & cin
	and (tmp[2], prop[1], gen[0]);			//t2: p1 & g0
	or (cout, gen[1], tmp[2], tmp[1]);		//cout: g1 | (p1 & g0) | (p1 & p0 & cin)
	
	xor (sum[0], prop[0], cin);				//sum0: p0 ^ cin
	xor (sum[1], prop[1], carry);				//sum1: p1 ^ carry
	
endmodule