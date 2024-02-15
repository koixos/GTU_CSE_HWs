module and41_32bit (
	output [31:0] out,
	input [31:0] ain,
	input
		bin,
		cin,
		din
);

	and(w, bin, cin, din);
	and_32bit and1 (out, ain, {32{w}});

endmodule