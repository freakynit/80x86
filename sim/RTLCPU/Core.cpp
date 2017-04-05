#include <stdexcept>
#include <fstream>
#include <VerilogDriver.h>
#include <VCore.h>
#include <boost/algorithm/string/replace.hpp>

#include "svdpi.h"

#include "Memory.h"
#include "RegisterFile.h"
#include "CPU.h"
#include "RTLCPU.h"

union reg_converter {
    uint16_t v16;
    uint8_t v8[2];
};

static const int max_cycles_per_step = 1000000;

double cur_time_stamp = 0;
double sc_time_stamp()
{
    return cur_time_stamp;
}

template <bool debug_enabled>
RTLCPU<debug_enabled>::RTLCPU(const std::string &test_name)
    : VerilogDriver<VCore, debug_enabled>(test_name),
    i_in_progress(false),
    d_in_progress(false),
    mem_latency(0),
    test_name(test_name),
    is_stopped(true)
{
    this->dut.debug_seize = 1;
    this->reset();

    this->periodic(ClockSetup, [&]{ this->data_access(); });
    this->periodic(ClockSetup, [&]{ this->instruction_access(); });
    this->periodic(ClockCapture, [&]{
        auto dev = !this->dut.d_io ? &this->mem : &this->io;
        if (this->dut.data_m_access && this->dut.data_m_wr_en) {
            if (this->dut.data_m_bytesel & (1 << 0))
                dev->template write<uint8_t>(this->dut.data_m_addr << 1,
                                             this->dut.data_m_data_out & 0xff);
            if (this->dut.data_m_bytesel & (1 << 1))
                dev->template write<uint8_t>((this->dut.data_m_addr << 1) + 1,
                                             (this->dut.data_m_data_out >> 8) & 0xff);
        }
    });
    this->periodic(ClockCapture, [&]{
        this->is_stopped = this->dut.debug_stopped;
    });
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::debug_run_proc(unsigned addr)
{
    this->after_n_cycles(0, [&]{
        this->dut.debug_addr = addr;
        this->dut.debug_run = 1;
        this->after_n_cycles(1, [&] {
            this->dut.debug_run = 0;
        });
    });

    int cycle_count = 0;

    // Start the procedure running.
    while (debug_is_stopped()) {
        this->cycle();
        ++cycle_count;
    }

    // wait for completion
    while (!debug_is_stopped()) {
        this->cycle();
        ++cycle_count;
    }

    if (cycle_count >= max_cycles_per_step)
        throw std::runtime_error("execution timeout");
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::debug_step()
{
    assert(is_stopped);

    debug_run_proc(0x00);
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_coverage()
{
#ifdef COVERAGE
    auto filename = (boost::format("microcode_%s.mcov") % test_name).str();
    boost::replace_all(filename, "/", "_");
    std::ofstream cov;
    cov.open("coverage/" + filename);

    svSetScope(svGetScopeFromName("TOP.Core.Microcode"));
    auto num_bins = this->dut.get_microcode_num_instructions();
    for (auto i = 0; i < num_bins; ++i) {
        auto count = this->dut.get_microcode_coverage_bin(i);
        cov << std::hex << i << ":" << count << std::endl;
    }
    cov.close();
#endif
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::reset()
{
    VerilogDriver<VCore, debug_enabled>::reset();
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_reg(GPR regnum, uint16_t val)
{
    if (regnum == IP)
        write_ip(val);
    else if (regnum >= ES && regnum <= DS)
        write_sr(regnum, val);
    else
        write_gpr(regnum, val);
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_sr(GPR regnum, uint16_t val)
{
    debug_write_data(val);
    debug_run_proc(0x13 + static_cast<int>(regnum));
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::debug_write_data(uint16_t val)
{
    this->after_n_cycles(0, [&] {
        this->dut.debug_wr_en = 1;
        this->dut.debug_wr_val = val;
        this->after_n_cycles(1, [&] {
            this->dut.debug_wr_en = 0;
        });
    });
    this->cycle();
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_ip(uint16_t val)
{
    assert(is_stopped);

    debug_write_data(val);
    debug_run_proc(0x11);
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_gpr(GPR regnum, uint16_t val)
{
    if (regnum < NUM_16BIT_REGS) {
        debug_write_data(val);
        debug_run_proc(0x13 + static_cast<int>(regnum));
    } else {
        auto regsel = (regnum - AL) & 0x3;
        reg_converter conv;
        conv.v16 = this->read_reg(static_cast<GPR>(regsel));

        conv.v8[regnum >= AH] = val;

        debug_write_data(conv.v16);
        debug_run_proc(0x13 + static_cast<int>(regsel));
    }
}

template <bool debug_enabled>
uint16_t RTLCPU<debug_enabled>::read_reg(GPR regnum)
{
    if (regnum == IP)
        return read_ip();
    else if (regnum >= ES && regnum <= DS)
        return read_sr(regnum);
    else
        return read_gpr(regnum);
}

template <bool debug_enabled>
uint16_t RTLCPU<debug_enabled>::read_ip()
{
    assert(this->dut.debug_stopped);
    debug_run_proc(0x0f);

    return this->dut.debug_val;
}

template <bool debug_enabled>
uint16_t RTLCPU<debug_enabled>::read_gpr(GPR regnum)
{
    if (regnum < NUM_16BIT_REGS) {
        debug_run_proc(0x03 + static_cast<int>(regnum));

        return this->dut.debug_val;
    }

    auto regsel = static_cast<int>(regnum - AL) & 0x3;
    debug_run_proc(0x03 + static_cast<int>(regsel));

    reg_converter conv;
    conv.v16 = this->dut.debug_val;

    return conv.v8[regnum >= AH];
}

template <bool debug_enabled>
uint16_t RTLCPU<debug_enabled>::read_sr(GPR regnum)
{
    debug_run_proc(0x03 + static_cast<int>(regnum));

    return this->dut.debug_val;
}

template <bool debug_enabled>
size_t RTLCPU<debug_enabled>::get_and_clear_instr_length()
{
    svSetScope(svGetScopeFromName("TOP.Core"));

    return static_cast<size_t>(this->dut.get_and_clear_instr_length());
}

template <bool debug_enabled>
size_t RTLCPU<debug_enabled>::step()
{
    this->get_and_clear_instr_length();

    this->debug_step();

    return this->get_and_clear_instr_length();
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_flags(uint16_t val)
{
    debug_write_data(val);
    debug_run_proc(0x12);
}

template <bool debug_enabled>
uint16_t RTLCPU<debug_enabled>::read_flags()
{
    this->debug_run_proc(0x10);

    return this->dut.debug_val;
}

template <bool debug_enabled>
bool RTLCPU<debug_enabled>::has_trapped()
{
    auto int_cs = this->mem.template read<uint16_t>(VEC_INT + 2);
    auto int_ip = this->mem.template read<uint16_t>(VEC_INT + 0);

    return read_sr(CS) == int_cs && read_ip() == int_ip;
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::instruction_access()
{
    if (this->dut.reset || !this->dut.instr_m_access || i_in_progress)
        return;

    i_in_progress = true;
    this->after_n_cycles(mem_latency, [&]{
        this->dut.instr_m_data_in =
            this->mem.template read<uint16_t>(this->dut.instr_m_addr << 1);
        this->dut.instr_m_ack = 1;
        this->after_n_cycles(1, [&]{
            this->dut.instr_m_ack = 0;
            i_in_progress = false;
        });
    });
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::data_access()
{
    if (this->dut.reset || !this->dut.data_m_access || d_in_progress)
        return;

    d_in_progress = true;
    this->after_n_cycles(mem_latency, [&]{
        auto v = this->dut.d_io ?
            this->io.template read<uint16_t>(this->dut.data_m_addr << 1) :
            this->mem.template read<uint16_t>(this->dut.data_m_addr << 1);
        this->dut.data_m_data_in = v;
        this->dut.data_m_ack = 1;
        this->after_n_cycles(1, [&]{
            this->dut.data_m_ack = 0;
            d_in_progress = false;
        });
    });
}

template <bool debug_enabled>
uint16_t RTLCPU<debug_enabled>::get_microcode_address()
{
    svSetScope(svGetScopeFromName("TOP.Core.Microcode"));

    return this->dut.get_microcode_address();
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_mem8(uint16_t segment, uint16_t addr,
                                       uint8_t val)
{
    auto prev_ds = read_reg(DS);

    write_reg(DS, segment);
    write_mar(addr);
    write_mdr(val);
    debug_run_proc(0x23); // Write mem 8
    write_reg(DS, prev_ds);
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_mem16(uint16_t segment, uint16_t addr,
                                        uint16_t val)
{
    auto prev_ds = read_reg(DS);

    write_reg(DS, segment);
    write_mar(addr);
    write_mdr(val);
    debug_run_proc(0x24); // Write mem 16
    write_reg(DS, prev_ds);
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_mem32(uint16_t segment, uint16_t addr,
                                        uint32_t val)
{
    write_mem16(segment, addr, val & 0xffff);
    write_mem16(segment, addr + 2, val >> 16);
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_mar(uint16_t v)
{
    debug_write_data(v);
    debug_run_proc(0x1f);
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_mdr(uint16_t v)
{
    debug_write_data(v);
    debug_run_proc(0x20);
}

template <bool debug_enabled>
uint8_t RTLCPU<debug_enabled>::read_mem8(uint16_t segment, uint16_t addr)
{
    auto prev_ds = read_reg(DS);

    write_reg(DS, segment);
    write_mar(addr);
    debug_run_proc(0x21); // Read mem 8

    uint8_t val = this->dut.debug_val;

    write_reg(DS, prev_ds);

    return val;
}

template <bool debug_enabled>
uint16_t RTLCPU<debug_enabled>::read_mem16(uint16_t segment, uint16_t addr)
{
    auto prev_ds = read_reg(DS);

    write_reg(DS, segment);
    write_mar(addr);
    debug_run_proc(0x22); // Read mem 16

    uint16_t val = this->dut.debug_val;

    write_reg(DS, prev_ds);

    return val;
}

template <bool debug_enabled>
uint32_t RTLCPU<debug_enabled>::read_mem32(uint16_t segment, uint16_t addr)
{
    return read_mem16(segment, addr) |
        (static_cast<uint32_t>(read_mem16(segment, addr + 2)) << 16);
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_io8(uint32_t addr, uint8_t val)
{
    write_mar(addr);
    write_mdr(val);
    debug_run_proc(0x27); // Write io 8
}

template <bool debug_enabled>
void RTLCPU<debug_enabled>::write_io16(uint32_t addr, uint16_t val)
{
    write_mar(addr);
    write_mdr(val);
    debug_run_proc(0x28); // Write io 16
}

template <bool debug_enabled>
uint8_t RTLCPU<debug_enabled>::read_io8(uint32_t addr)
{
    write_mar(addr);
    debug_run_proc(0x25); // Read io 8

    return this->dut.debug_val;
}

template <bool debug_enabled>
uint16_t RTLCPU<debug_enabled>::read_io16(uint32_t addr)
{
    write_mar(addr);
    debug_run_proc(0x26); // Read io 16

    return this->dut.debug_val;
}

template RTLCPU<verilator_debug_enabled>::RTLCPU(const std::string &);
