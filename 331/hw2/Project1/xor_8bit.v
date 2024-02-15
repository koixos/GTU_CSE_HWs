module xor_8bit (
	output [7:0] out,
	input [7:0] ain,
	input [7:0] bin
);

	xor_4bit xor1 (
		.out(out[3:0]),
		.ain(ain[3:0]),
		.bin(bin[3:0])
	);
	
	xor_4bit xor2 (
		.out(out[7:4]),
		.ain(ain[7:4]),
		.bin(bin[7:4])
	);

endmodule
