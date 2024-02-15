module xor_32bit (
	output [31:0] out,
	input [31:0] ain,
	input [31:0] bin
);
	xor_16bit xor1 (
		.out(out[15:0]),
		.ain(ain[15:0]),
		.bin(bin[15:0])
	);
	
	xor_16bit xor2 (
		.out(out[31:16]),
		.ain(ain[31:16]),
		.bin(bin[31:16])
	);
	
endmodule