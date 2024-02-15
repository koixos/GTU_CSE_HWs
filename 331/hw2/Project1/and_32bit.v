module and_32bit (
	output [31:0] out,
	input [31:0] ain,
	input [31:0] bin
);
	and_16bit and1 (
		.out(out[15:0]),
		.ain(ain[15:0]),
		.bin(bin[15:0])
	);
	
	and_16bit and2 (
		.out(out[31:16]),
		.ain(ain[31:16]),
		.bin(bin[31:16])
	);
	
endmodule