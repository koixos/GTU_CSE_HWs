module not_32bit (
	output [31:0] out,
	input [31:0] inp
);
	not_16bit not1 (
		.out(out[15:0]),
		.inp(inp[15:0])
	);
	
	not_16bit not2 (
		.out(out[31:16]),
		.inp(inp[31:16])
	);
	
endmodule