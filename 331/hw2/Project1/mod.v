module mod (
	output [31:0] res,
	input [31:0] ain, 
	input [31:0] bin, 
	input
		CLK,
		reset
	);
	
	mod_cu cu (
		.isSub(Wsub), 
		.isAssgn(weq), 
		.isComp(wcomp), 
		.assgn(wtemp), 
		.isDone(wdone),
		.less_than(wlt),
		.CLK(CLK), 
		.reset(reset) 
	);
						
	mod_dp dp (
		.res(res),
		.less_than(wlt),
		.ain(ain),
		.bin(bin),
		.isSub(Wsub), 
		.isAssgn(weq), 
		.isComp(wcomp), 
		.assgn(wtemp), 
		.isDone(wdone),
		.CLK(CLK) 
	);
	
endmodule