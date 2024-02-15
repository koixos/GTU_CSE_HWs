module and_4bit (
	output [3:0] out,
	input [3:0] ain,
	input [3:0] bin
);

	and (out[0], ain[0], bin[0]);
	and (out[1], ain[1], bin[1]);
	and (out[2], ain[2], bin[2]);
	and (out[3], ain[3], bin[3]);

endmodule