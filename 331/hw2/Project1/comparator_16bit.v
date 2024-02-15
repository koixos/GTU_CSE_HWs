module comparator_16bit (
	output
		greater,
		less,
		equal,
	input [15:0] ain,
	input [15:0] bin
);

	wire [1:0] G;
	wire [1:0] L;
	wire [1:0] E;
	wire [1:0] tmp;
	
	comparator_8bit comp1 (
		.greater(G[0]),
		.less(L[0]),
		.equal(E[0]),
		.ain(ain[7:0]),
		.bin(bin[7:0])
	);
	
	comparator_8bit comp2 (
		.greater(G[1]),
		.less(L[1]),
		.equal(E[1]),
		.ain(ain[15:8]),
		.bin(bin[15:8])
	);
	
	and (tmp[0], E[1], G[0]);		//t1: e2 & g1
	or (greater, G[1], tmp[0]);	//G: g2 | (e2 & g1)
	
	and (tmp[1], E[1], L[0]);		//t2: e2 & l1
	or (less, L[1], tmp[1]);		//L: l2 | (e2 & l1)
	
	and (equal, E[0], E[1]);		//E: e1 & e2

endmodule