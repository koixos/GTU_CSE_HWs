module xor_4bit (
	output [3:0] out,
	input [3:0] ain,
	input [3:0] bin
);

	xor (out[0], ain[0], bin[0]);
	xor (out[1], ain[1], bin[1]);
	xor (out[2], ain[2], bin[2]);
	xor (out[3], ain[3], bin[3]);

endmodule