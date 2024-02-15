module not_4bit (
	output [3:0] out,
	input [3:0] inp
);

	not (out[0], inp[0]);
	not (out[1], inp[1]);
	not (out[2], inp[2]);
	not (out[3], inp[3]);
	
endmodule