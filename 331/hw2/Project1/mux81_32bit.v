module mux81_32bit (
	output [31:0] out,
	input [31:0] ain,
	input [31:0] bin,
	input [31:0] cin,
	input [31:0] din,
	input [31:0] ein,
	input [31:0] fin,
	input [31:0] gin,
	input [31:0] hin,
	input [2:0] control
);

	wire [31:0] w [7:0];

	and41_32bit and1(
		.out(w[0]),
		.ain(ain),
		.bin(!control[0]),
		.cin(!control[1]),
		.din(!control[2])
	);
	
	and41_32bit and2(
		.out(w[1]),
		.ain(bin),
		.bin(control[0]),
		.cin(!control[1]),
		.din(!control[2])
	);
	
	and41_32bit and3(
		.out(w[2]),
		.ain(cin),
		.bin(!control[0]),
		.cin(control[1]),
		.din(!control[2])
	);
	
	and41_32bit and4(
		.out(w[3]),
		.ain(din),
		.bin(control[0]),
		.cin(control[1]),
		.din(!control[2])
	);
	
	and41_32bit and5(
		.out(w[4]),
		.ain(ein),
		.bin(!control[0]),
		.cin(!control[1]),
		.din(control[2])
	);
	
	and41_32bit and6(
		.out(w[5]),
		.ain(fin),
		.bin(control[0]),
		.cin(!control[1]),
		.din(control[2])
	);
	
	and41_32bit and7(
		.out(w[6]),
		.ain(gin),
		.bin(!control[0]),
		.cin(control[1]),
		.din(control[2])
	);
	
	and41_32bit and8(
		.out(w[7]),
		.ain(hin),
		.bin(control[0]),
		.cin(control[1]),
		.din(control[2])
	);
	
	or81_32bit or1(
		.out(out),
		.ain(w[0]),
		.bin(w[1]),
		.cin(w[2]),
		.din(w[3]),
		.ein(w[4]),
		.fin(w[5]),
		.gin(w[6]),
		.hin(w[7])
	);

endmodule