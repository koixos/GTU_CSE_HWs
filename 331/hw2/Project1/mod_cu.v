module mod_cu (
	output reg
		isSub,
		isAssgn,
		isComp,
		assgn,
		isDone,
	input
		less_than,
		CLK,
		reset
);
	
	reg [2:0] curr_state, next_state;
	
	localparam 
		START = 3'b000,
		SUB 	= 3'b001,
		ASSGN = 3'b010,
		COMP	= 3'b011,
		END = 3'b100;
					
	
	always @(posedge CLK)
		begin
			if(reset)
				curr_state <= START;
			else
				curr_state <= next_state;
		end
		
	always @(*)
		begin
			case(curr_state)
				START:
					begin
						next_state = SUB;
					end		
				SUB: 
					begin
						next_state = ASSGN;
					end
				ASSGN:
					begin
						next_state = COMP;
					end
				COMP:
					begin
						if(less_than)
							next_state = END;
						else
							next_state = SUB;
					end
				END:
					begin
						next_state = END; 
					end
			endcase
		end
		
	always @(*)
		begin
			isSub = 1'b0;
			isAssgn = 1'b0;
			isComp = 1'b0;
			assgn = 1'b0;
			isDone = 1'b0;
			
			case(curr_state)
				START:
					begin
						isAssgn = 1'b1;
					end
				SUB: 
					begin
						isAssgn = 1'b0;
						isSub = 1'b1;
						isComp = 1'b0;
					end		
				ASSGN:
					begin
						isSub = 1'b0;
						assgn = 1'b1;
					end
				COMP:
					begin
						assgn = 1'b0;
						isComp = 1'b1;
					end	
				END:
					begin
						isAssgn = 1'b0;
						isSub = 1'b0;
						assgn = 1'b0;
						isComp = 1'b0;
						isDone = 1'b1;
					end				
			endcase
		end
		
endmodule