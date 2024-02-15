module or_16bit (
	output [15:0] out,
	input [15:0] ain,
	input [15:0] bin
);

	or_8bit or1 (
		.out(out[7:0]),
		.ain(ain[7:0]),
		.bin(bin[7:0])
	);
	
	or_8bit or2 (
		.out(out[15:8]),
		.ain(ain[15:8]),
		.bin(bin[15:8])
	);
	
endmodule