module not_16bit (
	output [15:0] out,
	input [15:0] inp
);

	not_8bit not1 (
		.out(out[7:0]),
		.inp(inp[7:0])
	);
	
	not_8bit not2 (
		.out(out[15:8]),
		.inp(inp[15:8])
	);
	
endmodule