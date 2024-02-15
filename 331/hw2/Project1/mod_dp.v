module mod_dp (
	output reg [31:0] res,
	output reg less_than,
	input [31:0] ain,
	input [31:0] bin,
	input
		isSub,
		isAssgn,
		isComp,
		assgn,
		isDone,
		CLK
);
	
	reg[31:0] wres;
	reg[31:0] wsub;
	
	always @(posedge CLK)
		begin
			if(isAssgn)
				wres <= ain;
			else if(isComp)
				less_than = (wres < bin);
			else if(isSub)
				wsub = wres - bin;
			else if(assgn)
				wres = wsub;
			else if(isDone)
				res = wres + bin;
		end
	
endmodule 