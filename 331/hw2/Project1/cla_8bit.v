module cla_8bit (
	output [7:0] sum,
	output cout,
	input [7:0] ain,
	input [7:0] bin,
	input cin
);
	
	wire [7:0] prop;
	wire [7:0] gen;
	wire carry;
	
	cla_4bit cla1 (
		.sum(sum[3:0]),
		.cout(carry),
		.ain(ain[3:0]),
		.bin(bin[3:0]),
		.cin(cin)
	);
	
	cla_4bit cla2 (
		.sum(sum[7:4]),
		.cout(cout),
		.ain(ain[7:4]),
		.bin(bin[7:4]),
		.cin(carry)
	);
	
endmodule