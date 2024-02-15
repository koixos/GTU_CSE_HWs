module or_4bit (
	output [3:0] out,
	input [3:0] ain,
	input [3:0] bin
);

	or (out[0], ain[0], bin[0]);
	or (out[1], ain[1], bin[1]);
	or (out[2], ain[2], bin[2]);
	or (out[3], ain[3], bin[3]);

endmodule