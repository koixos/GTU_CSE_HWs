module datapath(
	input clk,
	input wire rst,
	input wire we,
	input wire ldX,
	input wire ldY,
	input wire ldI,
	input wire ldJ,
	input wire ldMagnit,
	input wire ldImgAddr,
	input wire ldGxAddr,
	input wire ldGyAddr,
	input wire ldSumx,
	input wire ldSumy,
	input wire Sx,
	input wire Sy,
	input wire Si,
	input wire Sj,
	input wire Ssumxop,
	input wire Ssumyop,
	input wire Ssumx,
	input wire Ssumy,
	input wire Smult,
	input wire [1:0] Ss,
	input wire [2:0] Scompl,
	input wire [2:0] Scompr,
	input wire Sedg,
	output reg lt,
	output reg lteq
);

parameter SZ = 9'd256;
parameter TH = 8'd185;

reg [3:0] gxAddr;
reg [3:0] gyAddr;
reg [15:0] imgAddr;
reg [15:0] edgRdAddr;
reg [15:0] edgWrAddr;
reg [7:0] edgData;

reg signed [16:0] sumx;
reg signed [16:0] sumy;
reg signed [16:0] magnit;
reg signed [1:0] i;
reg signed [1:0] j;
reg [8:0] x;
reg [8:0] y;

wire signed [7:0] gxDout;
wire signed [7:0] gyDout;
wire [7:0] imgDout;
wire [7:0] edgDout;

Gx sobel_gx (
	.clock(clk),
	.address(gxAddr),
	.q(gxDout)
);

Gy sobel_gy (
	.clock(clk),
	.address(gyAddr),
	.q(gyDout)
);

img_rom my_img (
	.clock(clk),
	.address(imgAddr),
	.q(imgDout)
);

edg_ram_dual my_edg (
	.clock(clk),
	.data(edgData),
	.rdaddress(edgRdAddr),
	.wraddress(edgWrAddr),
	.wren(we),
	.q(edgDout)
);

always @(posedge clk) begin
	/* assigning x */
	if (ldX) begin
		if (Sx == 1'd0) x <= 9'd0;
		else if (Sx == 1'd1 && Ss == 2'd0) x <= x + 9'd1;
	end
	
	/* assigning y */
	if (ldY) begin
		if (Sy == 1'd0) y <= 9'd0;
		else if (Sy == 1'd1 && Ss == 2'd1) y <= y + 9'd1;
	end
	
	/* assigning i */
	if (ldI) begin
		if (Si == 1'd0) i <= -2'sd1;
		else if (Si == 1'd1 && Ss == 2'd3) i <= i + 2'd1;
	end
	
	/* assigning j */
	if (ldJ) begin
		if (Sj == 1'd0) j <= -2'sd1;
		else if (Sj == 1'd1 && Ss == 2'd2) j <= j + 2'd1;
	end
end

always @(posedge clk) begin
	if (ldSumx) begin
		if (Ssumx == 1'd0) sumx <= 17'd0;
		else if (Ssumx == 1'd1 && Ssumxop == 1'd1) sumx <= sumx + (imgDout * gxDout);
		else if (Ssumx == 1'd1 && Ssumxop == 1'd0) sumx <= sumx * -2'sd1;
	end
	
	if (ldSumy) begin
		if (Ssumy == 1'd0) sumy <= 17'd0;
		else if (Ssumy == 1'd1 && Ssumyop == 1'd1) sumy <= sumy + (imgDout * gyDout);
		else if (Ssumy == 1'd1 && Ssumyop == 1'd0) sumy <= sumy * -2'sd1;
	end
	
	if (ldMagnit)
		magnit <= sumx + sumy;
	
	if (ldImgAddr)
		imgAddr <= ((y + i) << 8) + x + j;
	
	if (ldGxAddr)
		gxAddr <= ((1 + i) * 3) + 1 + j;
	
	if (ldGyAddr)
		gyAddr <= ((1 + i) * 3) + 1 + j;
end

always @(posedge clk) begin
	if (we) begin
		edgWrAddr <= (y << 8) + x;
		if (Sedg == 1'd0) edgData <= 8'd0;
		else if (Sedg == 1'd1) edgData <= 8'd255;
	end
end

always @(posedge clk) begin
	lt <= 1'b0;
	lteq <= 1'b0;

	if (Scompl == 3'd1 && Scompr == 3'd5) begin
		if (y < SZ) lt <= 1;
		else lt <= 0;
	end
	if (Scompl == 3'd0 && Scompr == 3'd4) begin
		if (x < SZ) lt <= 1;
		else lt <= 0;
	end
	if (Scompl == 3'd1 && Scompr == 3'd3) begin
		if (y < SZ - 1) lt <= 1;
		else lt <= 0;
	end
	if (Scompl == 3'd0 && Scompr == 3'd2) begin
		if (x < SZ - 1) lt <= 1;
		else lt <= 0;
	end
	if (Scompl == 3'd4 && Scompr == 3'd0) begin
		if (sumx < 0) lt <= 1;
		else lt <= 0;
	end
	if (Scompl == 3'd5 && Scompr == 3'd0) begin
		if (sumy < 0) lt <= 1;
		else lt <= 0;
	end
	if (Scompl == 3'd2 && Scompr == 3'd1) begin
		if (i < 1 || i == 1) lteq <= 1;
		else lteq <= 0;
	end
	if (Scompl == 3'd3 && Scompr == 3'd1) begin
		if (j < 1 || j == 1) lteq <= 1;
		else lteq <= 0;
	end
	if (Scompl == 3'd6 && Scompr == 3'd6) begin
		if (magnit < TH || magnit == TH) lteq <= 1;
		else lteq <= 0;
	end
end

endmodule