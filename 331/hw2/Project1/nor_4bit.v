module nor_4bit (
	output [3:0] out,
	input [3:0] ain,
	input [3:0] bin
);

	nor (out[0], ain[0], bin[0]);
	nor (out[1], ain[1], bin[1]);
	nor (out[2], ain[2], bin[2]);
	nor (out[3], ain[3], bin[3]);

endmodule