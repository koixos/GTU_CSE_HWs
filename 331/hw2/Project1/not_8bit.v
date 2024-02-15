module not_8bit (
	output [7:0] out,
	input [7:0] inp
);

	not_4bit not1 (
		.out(out[3:0]),
		.inp(inp[3:0])
	);
	
	not_4bit not2 (
		.out(out[7:4]),
		.inp(inp[7:4])
	);

endmodule
