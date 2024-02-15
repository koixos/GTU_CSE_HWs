module or81_32bit(
	output [31:0] out,
	input [31:0] ain,
	input [31:0] bin,
	input [31:0] cin,
	input [31:0] din,
	input [31:0] ein,
	input [31:0] fin,
	input [31:0] gin,
	input [31:0] hin
);

	wire [31:0] w [5:0];

	or_32bit or1 (
		.out(w[0]),
		.ain(ain),
		.bin(bin)
	);
	
	or_32bit or2 (
		.out(w[1]),
		.ain(w[0]),
		.bin(cin)
	);
	
	or_32bit or3 (
		.out(w[2]),
		.ain(w[1]),
		.bin(din)
	);
	
	or_32bit or4 (
		.out(w[3]),
		.ain(w[2]),
		.bin(ein)
	);
	
	or_32bit or5 (
		.out(w[4]),
		.ain(w[3]),
		.bin(fin)
	);
	
	or_32bit or6 (
		.out(w[5]),
		.ain(w[4]),
		.bin(gin)
	);
	
	or_32bit or7 (
		.out(out),
		.ain(w[5]),
		.bin(hin)
	);

endmodule