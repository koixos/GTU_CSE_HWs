module cla_32bit (
	output [31:0] sum,
	output cout,
	input [31:0] ain,
	input [31:0] bin,
	input cin
);
	
	wire [31:0] prop;
	wire [31:0] gen;
	wire carry;
	
	cla_16bit cla1 (
		.sum(sum[15:0]),
		.cout(carry),
		.ain(ain[15:0]),
		.bin(bin[15:0]),
		.cin(cin)
	);
	
	cla_16bit cla2 (
		.sum(sum[31:16]),
		.cout(cout),
		.ain(ain[31:16]),
		.bin(bin[31:16]),
		.cin(carry)
	);
	
endmodule