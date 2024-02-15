module alu (
	output [31:0] result,
	output cout,
	input [31:0] ain,
	input [31:0] bin,
	input
		[2:0] alu_op,
		cin,
		CLK,
		reset
);

	wire [31:0] res [7:0];
	
	and_32bit and1 (
		.out(res[0]),
		.ain(ain),
		.bin(bin)
	);
	
	or_32bit or1 (
		.out(res[1]),
		.ain(ain),
		.bin(bin)
	);
	
	xor_32bit xor1 (
		.out(res[2]),
		.ain(ain),
		.bin(bin)
	);
	
	nor_32bit nor1 (
		.out(res[3]),
		.ain(ain),
		.bin(bin)
	);
	
	less_than_32bit lt1 (
		.out(res[4]),
		.ain(ain),
		.bin(bin)
	);
	
	cla_32bit cla1 (
		.sum(res[5]),
		.cout(cout),
		.ain(ain),
		.bin(bin),
		.cin(cin)
	);
	
	subtractor_32bit sub1 (
		.diff(res[6]),
		.ain(ain),
		.bin(bin)
	);
	
	mod mod1 (
		.res(res[7]),
		.ain(ain),
		.bin(bin),
		.CLK(CLK),
		.reset(reset)
	);
	
	mux81_32bit mux1 (
		.out(result),
		.ain(res[0]),
		.bin(res[1]),
		.cin(res[2]),
		.din(res[3]),
		.ein(res[4]),
		.fin(res[5]),
		.gin(res[6]),
		.hin(res[7]),
		.control(alu_op)
	);
	
endmodule