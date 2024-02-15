module prop_gen_generator (
	output
		prop,
		gen,
	input
		ain,
		bin
);

	xor (prop, ain, bin);	//prop: a ^ b
	and (gen, ain, bin);		//gen: a & b

endmodule