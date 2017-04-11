module Top(input logic clk,
	   input logic rst_in_n,
           output logic s_ras_n,
           output logic s_cas_n,
           output logic s_wr_en,
           output logic [1:0] s_bytesel,
           output logic [12:0] s_addr,
           output logic s_cs_n,
           output logic s_clken,
           inout [15:0] s_data,
           output logic [1:0] s_banksel,
           output logic sdr_clk,
           output logic [7:0] leds);

wire sys_clk;
wire reset_n;
wire reset = ~reset_n | debug_reset;

wire [1:0] ir;
wire tdo;
wire tck;
wire tdi;
wire sdr;
wire cdr;
wire udr;
wire debug_stopped;
wire debug_seize;
wire debug_reset;
wire debug_run;
wire [7:0] debug_addr;
wire [15:0] debug_wr_val;
wire [15:0] debug_val;
wire debug_wr_en;
wire [19:1] data_m_addr;
wire [15:0] data_m_data_in;
wire [15:0] data_m_data_out;
wire data_m_access;
reg data_m_ack;
wire data_m_wr_en;
wire [1:0] data_m_bytesel;
wire d_io;
wire sdram_access = data_m_access & ~d_io;

reg instr_m_ack;
reg instr_m_access;
always_ff @(posedge clk)
    instr_m_ack <= instr_m_access;

assign leds = {8'b0};

BitSync         ResetSync(.clk(sys_clk),
                          .d(rst_in_n),
                          .q(reset_n));


VirtualJTAG VirtualJTAG(.ir_out(),
                        .tdo(tdo),
                        .ir_in(ir),
                        .tck(tck),
                        .tdi(tdi),
                        .virtual_state_sdr(sdr),
                        .virtual_state_e1dr(),
                        .virtual_state_cdr(cdr),
                        .virtual_state_udr(udr));

JTAGBridge      JTAGBridge(.cpu_clk(sys_clk),
                           .*);

SysPLL	SysPLL(.refclk(clk),
	       .outclk_0(sys_clk),
               .outclk_1(sdr_clk));

SDRAMController #(.size(32 * 1024 * 1024),
                  .clkf(25000000))
                SDRAMController(.clk(sys_clk),
                                .reset(reset),
                                .cs(sdram_access),
                                .h_addr(data_m_addr),
                                .h_wdata(data_m_data_out),
                                .h_rdata(data_m_data_in),
                                .h_wr_en(data_m_wr_en),
                                .h_bytesel(data_m_bytesel),
                                .h_compl(data_m_ack),
                                .h_config_done(),
                                .*);

Core Core(.clk(sys_clk),
	  .reset(reset),
	  .instr_m_addr(),
	  .instr_m_data_in(),
	  .instr_m_access,
	  .instr_m_ack,
	  .data_m_addr,
	  .data_m_data_in,
	  .data_m_data_out,
	  .data_m_access,
	  .data_m_ack,
	  .data_m_wr_en,
	  .data_m_bytesel,
	  .d_io,
	  .lock(),
	  .debug_stopped,
	  .debug_seize,
	  .debug_addr,
	  .debug_run,
	  .debug_val,
	  .debug_wr_val,
	  .debug_wr_en);

endmodule