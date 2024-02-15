module my_test_bench ();
	reg [31:0] ain;
	reg [31:0] bin;
	reg [2:0] opcode;
	reg
		cin,
		CLK,
		reset;
	
	wire g, l, e, cout;
	wire [31:0] res;

	alu alu (
		.result(res),
		.cout(cout),
		.ain(ain),
		.bin(bin),
		.alu_op(opcode),
		.cin(cin),
		.CLK(CLK),
		.reset(reset)
	);
	
	initial
		begin
			cin = 0;
			
			// AND
			#5 ain = 32'b00000000000000000000000000000101; bin = 32'b00000000000000000000000000000111;
			#5 opcode = 3'b000;
			#5 $display ("AND\n%b and %b = %b\n", ain, bin, res);
			
			//OR
			#5 ain = 32'b00000000000000000000000000000101; bin = 32'b00000000000000000000000000000111;
			#5 opcode = 3'b001;
			#5 $display ("OR\n%b or %b = %b\n", ain, bin, res);
			
			//XOR
			#5 ain = 32'b00000000000000000000000000000101; bin = 32'b00000000000000000000000000000111;
			#5 opcode = 3'b010;
			#5 $display ("XOR\n%b xor %b = %b\n", ain, bin, res);
			
			//NOR
			#5 ain = 32'b00000000000000000000000000000101; bin = 32'b00000000000000000000000000000111;
			#5 opcode = 3'b011;
			#5 $display ("NOR\n%b nor %b = %b\n", ain, bin, res);
			
			//LESS THAN
			#5 ain = 2'b10; bin = 2'b01;	
			#5 opcode = 3'b100;
			#5 $display ("LESS THAN\n%0d < %0d = %b\n", ain, bin, res[0]);
			
			// ADD
			#5 ain = 10; bin = 6;
			#5 opcode = 3'b101;
			#5	$display ("ADD\n%0d + %0d (cin = %0d) = %0d, cout= %0d\n", ain, bin, cin, res, cout);
			
			//SUB
			#5 ain = 4'b1001; bin = 4'b0100;
			#5 opcode = 3'b110;
			#5 $display ("SUB\n%0d - %0d = %b\n", ain, bin, res);
			
			//MOD
			#100 ain = 47; bin = 3; reset = 1; CLK = 0; opcode = 3'b111;
			#2 reset = 0;
			#100 $display("%0d", res);
		end
		
	always
		begin
			#1
			CLK = ~CLK;
		end
		
endmodule