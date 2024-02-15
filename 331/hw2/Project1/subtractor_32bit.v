module subtractor_32bit (
	output [31:0] diff,
	input [31:0] ain,
	input [31:0] bin
);

	wire [31:0] temp, wcout;
	
	not_32bit not1 (
		.out(temp),
		.inp(bin)
	);
	
	cla_32bit cla1 (
		.sum(diff),
		.cout(wcout),
		.ain(ain),
		.bin(temp),
		.cin(1'b1)
	);

endmodule