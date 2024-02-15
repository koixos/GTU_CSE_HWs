module nor_16bit (
	output [15:0] out,
	input [15:0] ain,
	input [15:0] bin
);

	nor_8bit nor1 (
		.out(out[7:0]),
		.ain(ain[7:0]),
		.bin(bin[7:0])
	);
	
	nor_8bit nor2 (
		.out(out[15:8]),
		.ain(ain[15:8]),
		.bin(bin[15:8])
	);
	
endmodule