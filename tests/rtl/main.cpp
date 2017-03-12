#include <memory>
#include <string>

#include <gtest/gtest.h>

#include "RTLCPU.h"

double cur_time_stamp = 0;
double sc_time_stamp()
{
    return cur_time_stamp;
}

std::unique_ptr<CPU> get_cpu(const std::string &test_name)
{
    return std::make_unique<RTLCPU<>>(test_name);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "\
-Adc/ArithmeticAlImmedTest.*:\
Adc/ArithmeticAxImmedTest.*:\
Adc/ArithmeticMemImmed16Test.*:\
Adc/ArithmeticMemImmed16TestExtend.*:\
Adc/ArithmeticMemImmed8Test.*:\
Adc/ArithmeticMemReg16TestReversed.*:\
Adc/ArithmeticMemReg8TestReversed.*:\
Adc/ArithmeticRegImmed16Test.*:\
Adc/ArithmeticRegImmed16TestExtend.*:\
Adc/ArithmeticRegImmed8Test.*:\
Adc/ArithmeticRegMem16Test.*:\
Adc/ArithmeticRegReg16Test.*:\
Adc/ArithmeticRegReg16TestReversed.*:\
Adc/ArithmeticRegReg8TestReversed.*:\
Add/ArithmeticAlImmedTest.*:\
Add/ArithmeticAxImmedTest.*:\
Add/ArithmeticMemImmed16Test.*:\
Add/ArithmeticMemImmed16TestExtend.*:\
Add/ArithmeticMemImmed8Test.*:\
Add/ArithmeticMemReg16TestReversed.*:\
Add/ArithmeticMemReg8TestReversed.*:\
Add/ArithmeticRegImmed16Test.*:\
Add/ArithmeticRegImmed16TestExtend.*:\
Add/ArithmeticRegImmed8Test.*:\
Add/ArithmeticRegReg16TestReversed.*:\
Add/ArithmeticRegReg8TestReversed.*:\
And/ArithmeticAlImmedTest.*:\
And/ArithmeticAxImmedTest.*:\
And/ArithmeticMemImmed16Test.*:\
And/ArithmeticMemImmed16TestExtend.*:\
And/ArithmeticMemImmed8Test.*:\
And/ArithmeticMemReg16TestReversed.*:\
And/ArithmeticMemReg8Test.*:\
And/ArithmeticMemReg8TestReversed.*:\
And/ArithmeticRegImmed16Test.*:\
And/ArithmeticRegImmed16TestExtend.*:\
And/ArithmeticRegImmed8Test.*:\
And/ArithmeticRegMem16Test.*:\
And/ArithmeticRegReg16Test.*:\
And/ArithmeticRegReg16TestReversed.*:\
And/ArithmeticRegReg8Test.*:\
And/ArithmeticRegReg8TestReversed.*:\
Cmp/ArithmeticAlImmedTest.*:\
Cmp/ArithmeticAxImmedTest.*:\
Cmp/ArithmeticMemImmed16Test.*:\
Cmp/ArithmeticMemImmed16TestExtend.*:\
Cmp/ArithmeticMemImmed8Test.*:\
Cmp/ArithmeticMemReg16TestReversedNoResult.*:\
Cmp/ArithmeticMemReg8Test.*:\
Cmp/ArithmeticMemReg8TestReversedNoResult.*:\
Cmp/ArithmeticRegImmed16Test.*:\
Cmp/ArithmeticRegImmed16TestExtend.*:\
Cmp/ArithmeticRegImmed8Test.*:\
Cmp/ArithmeticRegMem16Test.*:\
Cmp/ArithmeticRegReg16Test.*:\
Cmp/ArithmeticRegReg16TestReversedNoResult.*:\
Cmp/ArithmeticRegReg8Test.*:\
Cmp/ArithmeticRegReg8TestReversedNoResult.*:\
CmpsRepe/Cmps16Fixture.*:\
CmpsRepe/Cmps8Fixture.*:\
CmpsRepne/Cmps16Fixture.*:\
CmpsRepne/Cmps8Fixture.*:\
Dec/IncMem16Test.*:\
Dec/IncMem8Test.*:\
Dec/IncReg8Test.*:\
Div/DivMem16Test.*:\
Div/DivMem8Test.*:\
Div/DivReg16Test.*:\
Div/DivReg8Test.*:\
EmulateFixture.Aaa:\
EmulateFixture.Aad:\
EmulateFixture.AadSigned:\
EmulateFixture.Aam:\
EmulateFixture.Aam129:\
EmulateFixture.AamDivByZero:\
EmulateFixture.AamSigned:\
EmulateFixture.Aas:\
EmulateFixture.CallDirectInter:\
EmulateFixture.CallDirectIntra:\
EmulateFixture.CallIndirectInter:\
EmulateFixture.CallIndirectIntraMem:\
EmulateFixture.CallIndirectIntraReg:\
EmulateFixture.CbwNegative:\
EmulateFixture.CbwPositive:\
EmulateFixture.ClcClearsCarry:\
EmulateFixture.ClcDoesntSetCarry:\
EmulateFixture.CldClearsDirection:\
EmulateFixture.CldDoesntSetDirection:\
EmulateFixture.CliClearsInterrupt:\
EmulateFixture.CliDoesntSetInterrupt:\
EmulateFixture.CmcInverts0:\
EmulateFixture.CmcInverts1:\
EmulateFixture.CmpsbDec:\
EmulateFixture.CmpswDec:\
EmulateFixture.CwdNegative:\
EmulateFixture.CwdPositive:\
EmulateFixture.DSSegmentOverrideIsNop:\
EmulateFixture.Daa:\
EmulateFixture.Das:\
EmulateFixture.DecReg:\
EmulateFixture.DirectInter:\
EmulateFixture.EscMem:\
EmulateFixture.EscReg:\
EmulateFixture.Hlt:\
EmulateFixture.InFixed16:\
EmulateFixture.InFixed8:\
EmulateFixture.InVariable16:\
EmulateFixture.InVariable8:\
EmulateFixture.IncFEInvalidReg:\
EmulateFixture.IncReg:\
EmulateFixture.Int3:\
EmulateFixture.IntN:\
EmulateFixture.IntoNotTaken:\
EmulateFixture.IntoTaken:\
EmulateFixture.InvalidRegD0:\
EmulateFixture.InvalidRegD1:\
EmulateFixture.InvalidRegD2:\
EmulateFixture.InvalidRegD3:\
EmulateFixture.InvalidRegF6:\
EmulateFixture.InvalidRegF7:\
EmulateFixture.InvalidRegFE:\
EmulateFixture.Iret:\
EmulateFixture.JcxzNotTaken:\
EmulateFixture.JcxzTaken:\
EmulateFixture.JmpDirectIntra:\
EmulateFixture.JmpDirectIntraShort:\
EmulateFixture.JmpIndirectInterMem:\
EmulateFixture.JmpIndirectInterReg:\
EmulateFixture.JmpIndirectIntraMem:\
EmulateFixture.JmpIndirectIntraReg:\
EmulateFixture.Lahf:\
EmulateFixture.Lds:\
EmulateFixture.LdsReg:\
EmulateFixture.LeaAxBx:\
EmulateFixture.LeaAxBxSiDisplacement:\
EmulateFixture.Les:\
EmulateFixture.LesReg:\
EmulateFixture.Lock:\
EmulateFixture.Lodsb:\
EmulateFixture.LodsbDec:\
EmulateFixture.Lodsw:\
EmulateFixture.LodswDec:\
EmulateFixture.LoopNotTaken:\
EmulateFixture.LoopTaken:\
EmulateFixture.LoopeNotTaken:\
EmulateFixture.LoopeNotTakenNonZero:\
EmulateFixture.LoopeTaken:\
EmulateFixture.LoopnzNotTaken:\
EmulateFixture.LoopnzNotTakenNonZero:\
EmulateFixture.LoopnzTaken:\
EmulateFixture.MovsbDec:\
EmulateFixture.MovsbInc:\
EmulateFixture.MovswDec:\
EmulateFixture.MovswInc:\
EmulateFixture.MultipleAdd:\
EmulateFixture.Neg16Mem:\
EmulateFixture.Neg16Reg:\
EmulateFixture.Neg8Mem:\
EmulateFixture.Neg8Reg:\
EmulateFixture.Not16Mem:\
EmulateFixture.Not16Reg:\
EmulateFixture.Not8Mem:\
EmulateFixture.Not8Reg:\
EmulateFixture.OutFixed16:\
EmulateFixture.OutFixed8:\
EmulateFixture.OutVariable16:\
EmulateFixture.OutVariable8:\
EmulateFixture.PopMemFF:\
EmulateFixture.PopMemSegmentOverride:\
EmulateFixture.PopReg5X:\
EmulateFixture.PopRegFF:\
EmulateFixture.PopRegFFInvalidReg:\
EmulateFixture.PopSR:\
EmulateFixture.Popf:\
EmulateFixture.PushMemFF:\
EmulateFixture.PushMemSegmentOverride:\
EmulateFixture.PushReg5X:\
EmulateFixture.PushRegFF:\
EmulateFixture.PushRegFFInvalidReg:\
EmulateFixture.PushSR:\
EmulateFixture.Pushf:\
EmulateFixture.RetInter:\
EmulateFixture.RetInterAddSp:\
EmulateFixture.RetIntra:\
EmulateFixture.RetIntraAddSp:\
EmulateFixture.Sahf:\
EmulateFixture.ScasbDec:\
EmulateFixture.ScasbDecRepe:\
EmulateFixture.ScasbInc:\
EmulateFixture.ScasbIncRepe:\
EmulateFixture.ScaswDec:\
EmulateFixture.ScaswDecRepe:\
EmulateFixture.ScaswInc:\
EmulateFixture.ScaswIncRepe:\
EmulateFixture.StcDoesntClearCarry:\
EmulateFixture.StcSetsCarry:\
EmulateFixture.StdDoesntClearDirection:\
EmulateFixture.StdSetsDirection:\
EmulateFixture.StiDoesntClearInterrupt:\
EmulateFixture.StiSetsInterrupt:\
EmulateFixture.Stosb:\
EmulateFixture.StosbDec:\
EmulateFixture.Stosw:\
EmulateFixture.StoswDec:\
EmulateFixture.TrapOnInt3:\
EmulateFixture.Wait:\
EmulateFixture.XchgALALNop:\
EmulateFixture.XchgRegAccumulator:\
EmulateFixture.XchgRegMem16:\
EmulateFixture.XchgRegMem8:\
EmulateFixture.XchgRegReg16:\
EmulateFixture.XchgRegReg8:\
EmulateFixture.Xlat:\
IMul/MulMem16Test.*:\
IMul/MulMem8Test.*:\
IMul/MulReg16Test.*:\
IMul/MulReg8Test.*:\
Inc/IncMem16Test.*:\
Inc/IncMem8Test.*:\
Inc/IncReg16Test.*:\
Inc/IncReg8Test.*:\
JmpConditional/JmpFixture.*:\
JmpConditionalAlias/JmpFixture.*:\
Mul/MulMem16Test.*:\
Mul/MulMem8Test.*:\
Mul/MulReg16Test.*:\
Mul/MulReg8Test.*:\
Or/ArithmeticAlImmedTest.*:\
Or/ArithmeticAxImmedTest.*:\
Or/ArithmeticMemImmed16Test.*:\
Or/ArithmeticMemImmed16TestExtend.*:\
Or/ArithmeticMemImmed8Test.*:\
Or/ArithmeticMemReg16TestReversed.*:\
Or/ArithmeticMemReg8Test.*:\
Or/ArithmeticMemReg8TestReversed.*:\
Or/ArithmeticRegImmed16Test.*:\
Or/ArithmeticRegImmed16TestExtend.*:\
Or/ArithmeticRegImmed8Test.*:\
Or/ArithmeticRegMem16Test.*:\
Or/ArithmeticRegReg16Test.*:\
Or/ArithmeticRegReg16TestReversed.*:\
Or/ArithmeticRegReg8Test.*:\
Or/ArithmeticRegReg8TestReversed.*:\
Rclg1/ShiftMem16Test.*:\
Rclg1/ShiftMem8Test.*:\
Rclg1/ShiftReg16Test.*:\
Rclg1/ShiftReg8Test.*:\
RclgCL/ShiftCLTest.*:\
RclgN/ShiftMem16Test.*:\
RclgN/ShiftMem8Test.*:\
RclgN/ShiftReg16Test.*:\
RclgN/ShiftReg8Test.*:\
Rcrg1/ShiftMem16Test.*:\
Rcrg1/ShiftMem8Test.*:\
Rcrg1/ShiftReg16Test.*:\
Rcrg1/ShiftReg8Test.*:\
RcrgCL/ShiftCLTest.*:\
RcrgN/ShiftMem16Test.*:\
RcrgN/ShiftMem8Test.*:\
RcrgN/ShiftReg16Test.*:\
RcrgN/ShiftReg8Test.*:\
Rol1/ShiftMem16Test.*:\
Rol1/ShiftMem8Test.*:\
Rol1/ShiftReg16Test.*:\
Rol1/ShiftReg8Test.*:\
RolCL/ShiftCLTest.*:\
RolN/ShiftMem16Test.*:\
RolN/ShiftMem8Test.*:\
RolN/ShiftReg16Test.*:\
RolN/ShiftReg8Test.*:\
Ror1/ShiftMem16Test.*:\
Ror1/ShiftMem8Test.*:\
Ror1/ShiftReg16Test.*:\
Ror1/ShiftReg8Test.*:\
RorCL/ShiftCLTest.*:\
RorN/ShiftMem16Test.*:\
RorN/ShiftMem8Test.*:\
RorN/ShiftReg16Test.*:\
RorN/ShiftReg8Test.*:\
Sar1/ShiftMem16Test.*:\
Sar1/ShiftMem8Test.*:\
Sar1/ShiftReg16Test.*:\
Sar1/ShiftReg8Test.*:\
SarCL/ShiftCLTest.*:\
SarN/ShiftMem16Test.*:\
SarN/ShiftMem8Test.*:\
SarN/ShiftReg16Test.*:\
SarN/ShiftReg8Test.*:\
Sbb/ArithmeticAlImmedTest.*:\
Sbb/ArithmeticAxImmedTest.*:\
Sbb/ArithmeticMemImmed16Test.*:\
Sbb/ArithmeticMemImmed16TestExtend.*:\
Sbb/ArithmeticMemImmed8Test.*:\
Sbb/ArithmeticMemReg16TestReversed.*:\
Sbb/ArithmeticMemReg8Test.*:\
Sbb/ArithmeticMemReg8TestReversed.*:\
Sbb/ArithmeticRegImmed16Test.*:\
Sbb/ArithmeticRegImmed16TestExtend.*:\
Sbb/ArithmeticRegImmed8Test.*:\
Sbb/ArithmeticRegMem16Test.*:\
Sbb/ArithmeticRegReg16Test.*:\
Sbb/ArithmeticRegReg16TestReversed.*:\
Sbb/ArithmeticRegReg8Test.*:\
Sbb/ArithmeticRegReg8TestReversed.*:\
SegmentOverride/MovOverride.*:\
Shl1/ShiftMem16Test.*:\
Shl1/ShiftMem8Test.*:\
Shl1/ShiftReg16Test.*:\
Shl1/ShiftReg8Test.*:\
ShlCL/ShiftCLTest.*:\
ShlN/ShiftMem16Test.*:\
ShlN/ShiftMem8Test.*:\
ShlN/ShiftReg16Test.*:\
ShlN/ShiftReg8Test.*:\
Shr1/ShiftMem16Test.*:\
Shr1/ShiftMem8Test.*:\
Shr1/ShiftReg16Test.*:\
Shr1/ShiftReg8Test.*:\
ShrCL/ShiftCLTest.*:\
ShrN/ShiftMem16Test.*:\
ShrN/ShiftMem8Test.*:\
ShrN/ShiftReg16Test.*:\
ShrN/ShiftReg8Test.*:\
Sub/ArithmeticAlImmedTest.*:\
Sub/ArithmeticAxImmedTest.*:\
Sub/ArithmeticMemImmed16Test.*:\
Sub/ArithmeticMemImmed16TestExtend.*:\
Sub/ArithmeticMemImmed8Test.*:\
Sub/ArithmeticMemReg16TestReversed.*:\
Sub/ArithmeticMemReg8Test.*:\
Sub/ArithmeticMemReg8TestReversed.*:\
Sub/ArithmeticRegImmed16Test.*:\
Sub/ArithmeticRegImmed16TestExtend.*:\
Sub/ArithmeticRegImmed8Test.*:\
Sub/ArithmeticRegMem16Test.*:\
Sub/ArithmeticRegReg16Test.*:\
Sub/ArithmeticRegReg16TestReversed.*:\
Sub/ArithmeticRegReg8Test.*:\
Sub/ArithmeticRegReg8TestReversed.*:\
Test/ArithmeticAlImmedTest.*:\
Test/ArithmeticAxImmedTest.*:\
Test/ArithmeticMemImmed16Test.*:\
Test/ArithmeticMemImmed8Test.*:\
Test/ArithmeticMemReg8Test.*:\
Test/ArithmeticRegImmed16Test.*:\
Test/ArithmeticRegImmed8Test.*:\
Test/ArithmeticRegMem16Test.*:\
Test/ArithmeticRegReg16Test.*:\
Test/ArithmeticRegReg8Test.*:\
Xor/ArithmeticAlImmedTest.*:\
Xor/ArithmeticAxImmedTest.*:\
Xor/ArithmeticMemImmed16Test.*:\
Xor/ArithmeticMemImmed8Test.*:\
Xor/ArithmeticMemReg16TestReversed.*:\
Xor/ArithmeticMemReg8Test.*:\
Xor/ArithmeticMemReg8TestReversed.*:\
Xor/ArithmeticRegImmed16Test.*:\
Xor/ArithmeticRegImmed8Test.*:\
Xor/ArithmeticRegMem16Test.*:\
Xor/ArithmeticRegReg16Test.*:\
Xor/ArithmeticRegReg16TestReversed.*:\
Xor/ArithmeticRegReg8Test.*:\
Xor/ArithmeticRegReg8TestReversed.*";

    return RUN_ALL_TESTS();
}
