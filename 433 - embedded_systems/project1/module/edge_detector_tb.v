`timescale 1ns / 1ps

module edge_detector_tb;
    reg clk;
    reg go;
    reg rst;
	 reg file_closed = 0;
	 
	  // Saat üretimi
    initial clk = 0;
    always #100 clk = ~clk;

	 my_module detector(
		.clk(clk),
		.go(go),
		.rst(rst)
	);
	 
	 integer i = 0, f;
	 initial begin
		f = $fopen("edge_ram_output.txt", "w");
		 if (f == 0) begin
			  $display("Dosya açılamadı!");
			  $finish;
		 end
		 
		// Başlangıçta reset aktif
		rst = 1;
		go = 0;
		#250;
		  
		// Reset bırakılıyor
		rst = 0;
		#200;

		// Sistemi başlat
		go = 1;
		#200;
		
		while (i < 20) begin
			@(posedge clk);
			$display("Current state: %b", detector.cu.state);
			case(detector.cu.state)
				5'b11111: $display("TEST => lt: %d", detector.dp.lt);
				5'b00000: $display("S0 => x: %d & y: %d", detector.dp.x, detector.dp.y);
				5'b00001: $display("S1 => y: %d & lt: %d", detector.dp.y, detector.dp.lt);
				5'b00010: $display("S2 => x: %d & y: %d & lt: %d", detector.dp.x, detector.dp.y, detector.dp.lt);
				5'b00011: $display("S3 => y: %d & lt: %d", detector.dp.y, detector.dp.lt);
			endcase
			i = i + 1;
		end	
		  
		go = 0;	
	end
	
	always @(posedge clk) begin
		if (!file_closed && detector.dp.we) begin
			$fdisplay(f, "Addr: %0d Data: %02x", detector.dp.edgWrAddr, detector.dp.edgData);
		end
		if (i >= 20) begin
			$fclose(f);
			file_closed = 1;
			$display("Yazılan RAM verileri 'edge_ram_output.txt' dosyasına aktarıldı.");
			$finish;
		end
	end

endmodule
