module control_unit(
	input clk,
	input wire go,
	input wire rst,
	input wire lt,
	input wire lteq,
	output reg we,
	output reg ldX,
	output reg ldY,
	output reg ldI,
	output reg ldJ,
	output reg ldMagnit,
	output reg ldImgAddr,
	output reg ldGxAddr,
	output reg ldGyAddr,
	output reg ldSumx,
	output reg ldSumy,
	output reg Sx,
	output reg Sy,
	output reg Si,
	output reg Sj,
	output reg Ssumxop,
	output reg Ssumyop,
	output reg Ssumx,
	output reg Ssumy,
	output reg Smult,
	output reg [1:0] Ss,
	output reg [2:0] Scompl,
	output reg [2:0] Scompr,
	output reg Sedg
);

localparam
	S00 = 5'b00000,
	S01 = 5'b00001,
	TEST = 5'b11111,
	S02 = 5'b00010,
	S03 = 5'b00011,
	S04 = 5'b00100,
	S05 = 5'b00101,
	S06 = 5'b00110,
	S07 = 5'b00111,
	S08 = 5'b01000,
	S09 = 5'b01001,
	S10 = 5'b01010,
	S11 = 5'b01011,
	S12 = 5'b01100,
	S13 = 5'b01101,
	S14 = 5'b01110,
	S15 = 5'b01111,
	S16 = 5'b10000,
	S17 = 5'b10001,
	S18 = 5'b10010;
	
reg [4:0] state;
reg [4:0] next_state;

always @(posedge clk) begin
	if (rst) state <= S00;
	else state <= next_state;
end

// next-state logics
always @(*) begin
	next_state = state;
	case (state)
		S00: if (go) next_state = TEST;
		TEST: next_state = S01;
		S01: begin
			if (lt) next_state = S02;
			else next_state = S03;
		end
		S02: if (!lt) next_state = S01;
		S03: next_state = S04;
		S04: begin
			if (lt) next_state = S05;
			else next_state = S00;
		end
		S05: begin
			if (lt) next_state = S06;
			else next_state = S04;
		end
		S06: begin
			if (lteq) next_state = S07;
			else next_state = S09;
		end
		S07: next_state = S08;
		S08: begin
			if (lteq) next_state = S07;
			else next_state = S06;
		end
		S09: begin
			if (lt) next_state = S11;
			else next_state = S10;
		end
		S10: next_state = S12;
		S11: next_state = S12;
		S12: begin
			if (lt) next_state = S14;
			else next_state = S13;
		end
		S13: next_state = S15;
		S14: next_state = S15;
		S15: next_state = S16;
		S16: begin
			if (lteq) next_state = S18;
			else next_state = S17;
		end
		S17: next_state = S05;
		S18: next_state = S05;
		default: next_state = S00;
	endcase
end

// output logic
always @(*) begin
	case (state)
		S00: begin
			ldX = 1'd1;		ldY = 1'd1;
			Sx = 1'd0;		Sy = 1'd0;
		end		
		TEST: begin
			ldX = 1'd0;		ldY = 1'd1;
			ldI = 1'd0;		ldJ = 1'd0;
			Sy = 1'd1;		Ss = 2'd1;
		end
		S01: begin
			
			Scompl = 3'd1; Scompr = 3'd5;
		end
		S02: begin
			ldX = 1'd1;		ldY = 1'd0;
			ldI = 1'd0; 	ldJ = 1'd0;
			Sx = 1'd1;		Ss = 2'd0;
			Scompl = 3'd0; Scompr = 3'd4;
		end
		S03: begin
			ldY = 1'd1;
			Sy = 1'd0;
		end
		S04: begin
			ldX = 1'd1;		ldY = 1'd1;
			ldI = 1'd0;		ldJ = 1'd0;
			Sx = 1'd0;		Sy = 1'd1;
			Ss = 2'd1;
			Scompl = 3'd1;	Scompr = 3'd3;
		end
		S05: begin
			we = 1'd0;
			ldX = 1'd1;		ldY = 1'd0;
			ldI = 1'd1;		ldJ = 1'd0;
			ldSumx = 1'd1;	ldSumy = 1'd1;
			Sx = 1'd1;		Si = 1'd0;
			Ss = 2'd0;
			Ssumx = 1'd0;	Ssumy = 1'd0;
			Scompl = 3'd0;	Scompr = 3'd2;
		end
		S06: begin
			ldX = 1'd0;		ldY = 1'd0;
			ldI = 1'd1;		ldJ = 1'd1;
			Si = 1'd1;		Sj = 1'd0;
			Ss = 2'd3;
			Scompl = 3'd2;	Scompr = 3'd1;
		end
		S07: begin
			ldImgAddr = 1'd1;
			ldGxAddr = 1'd1;
			ldGyAddr = 1'd1;
		end
		S08: begin
			ldX = 1'd0;		ldY = 1'd0;
			ldI = 1'd0;		ldJ = 1'd1;
			ldSumx = 1'd1;	ldSumy = 1'd1;
			Sj = 1'd1;		Ss = 2'd2;
			Ssumx = 1'd1;	Ssumy = 1'd1;
			Ssumxop = 1'd1;Ssumyop = 1'd1;
			Scompl = 3'd3;	Scompr = 3'd1;
		end
		S09: begin
			Scompl = 3'd4;	Scompr = 3'd0;
		end
		S10:;		
		S11: begin
			ldSumx = 1'd1;	ldSumy = 1'd0;
			Ssumx = 1'd1;
			Ssumxop = 1'd0;
			Smult = 1'd1;
		end
		S12: begin
			Scompl = 3'd5;	Scompr = 3'd0;
		end
		S13:;	
		S14: begin
			ldSumx = 1'd0;	ldSumy = 1'd1;
			Ssumy = 1'd1;
			Ssumyop = 1'd0;
			Smult = 1'd0;
		end	
		S15: begin
			ldMagnit = 1'd1;
		end
		S16: begin
			ldMagnit = 1'd0;
			Scompl = 2'd6;	Scompr = 2'd6;
		end
		S17: begin
			we = 1'd1;
			Sedg = 1'd1;
		end
		S18: begin
			we = 1'd1;
			Sedg = 1'd0;
		end
	endcase
end
	
endmodule