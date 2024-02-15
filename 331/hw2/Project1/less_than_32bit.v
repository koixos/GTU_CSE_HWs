module less_than_32bit (
	output out,
	input [31:0] ain,
	input [31:0] bin
);

	wire tmp [1:0];

	comparator_32bit comp1 (
		.greater(tmp[0]),
		.less(out),
		.equal(tmp[1]),
		.ain(ain),
		.bin(bin)
	);

endmodule