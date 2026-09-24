`timescale 1ns/1ps

module datapath_tb;

  reg clk = 0;
  always #5 clk = ~clk;

  reg rst = 0;
  reg we, ldX, ldY, ldI, ldJ, ldMagnit, ldImgAddr, ldGxAddr, ldGyAddr, ldSumx, ldSumy;
  reg Sx, Sy, Si, Sj, Ssumxop, Ssumyop, Ssumx, Ssumy, Smult;
  reg [1:0] Ss;
  reg [2:0] Scompl, Scompr;
  reg Sedg;
  wire lt, lteq;

  datapath uut (
    .clk(clk),
    .rst(rst),
    .we(we),
    .ldX(ldX), .ldY(ldY), .ldI(ldI), .ldJ(ldJ),
    .ldMagnit(ldMagnit),
    .ldImgAddr(ldImgAddr), .ldGxAddr(ldGxAddr), .ldGyAddr(ldGyAddr),
    .ldSumx(ldSumx), .ldSumy(ldSumy),
    .Sx(Sx), .Sy(Sy), .Si(Si), .Sj(Sj),
    .Ssumxop(Ssumxop), .Ssumyop(Ssumyop),
    .Ssumx(Ssumx), .Ssumy(Ssumy), .Smult(Smult),
    .Ss(Ss),
    .Scompl(Scompl), .Scompr(Scompr),
    .Sedg(Sedg),
    .lt(lt), .lteq(lteq)
  );

  integer i, j;

  initial begin
    $display("Starting datapath test...");

    // İlk değer atamaları
    rst = 1; #10; rst = 0;

    // X ve Y = 0 ile başla
    ldX = 1; Sx = 0; #10; ldX = 0;
    ldY = 1; Sy = 0; #10; ldY = 0;

    // sumx ve sumy = 0
    ldSumx = 1; Ssumx = 0; #10; ldSumx = 0;
    ldSumy = 1; Ssumy = 0; #10; ldSumy = 0;

    // i, j döngüsü (i=-1:1, j=-1:1)
    for (i = -1; i <= 1; i = i + 1) begin
      ldI = 1; Si = (i == -1) ? 0 : 1; Ss = 2'd3; #10; ldI = 0;
      for (j = -1; j <= 1; j = j + 1) begin
        ldJ = 1; Sj = (j == -1) ? 0 : 1; Ss = 2'd2; #10; ldJ = 0;

        // image, gx, gy adreslerini ayarla
        ldImgAddr = 1; #10; ldImgAddr = 0;
        ldGxAddr  = 1; #10; ldGxAddr  = 0;
        ldGyAddr  = 1; #10; ldGyAddr  = 0;

        // sumx += img * gx
        ldSumx = 1; Ssumx = 1; Ssumxop = 1; #10; ldSumx = 0;

        // sumy += img * gy
        ldSumy = 1; Ssumy = 1; Ssumyop = 1; #10; ldSumy = 0;
      end
    end

    // magnitude hesapla
    ldMagnit = 1; #10; ldMagnit = 0;

    // eşikleme sonucu yaz (magnitude < threshold)
    Sedg = 1; we = 1; #10;
    we = 0;

    $display("Simulation finished.");
    $stop;
  end

endmodule
