#!/usr/bin/env python3
"""Host-side checks for the width-independent RISC-V port contract."""

import unittest
from pathlib import Path


MCAUSE_INT = {32: 1 << 31, 64: 1 << 63}
MCAUSE_MASK = {xlen: value - 1 for xlen, value in MCAUSE_INT.items()}
CONTEXT_REGS = 33
PHYSICAL_TIMER_API_SUPPORTED = False


def decode_cause(cause, xlen):
    """Return (is_interrupt, exception_code) using the selected XLEN."""
    interrupt = bool(cause & MCAUSE_INT[xlen])
    return interrupt, cause & MCAUSE_MASK[xlen]


def timer_delta(now, then, xlen):
    """Calculate an unsigned free-running timer delta across rollover."""
    modulus = 1 << xlen
    return (now - then) % modulus


def frame_offsets(xlen):
    """Return the assembly frame offsets for pc, status, and x1..x31."""
    width = xlen // 8
    return {
        "pc": 0,
        "mstatus": width,
        **{f"x{reg}": (reg + 1) * width for reg in range(1, 32)},
    }


class RiscvPortContractTests(unittest.TestCase):
    def test_supported_widths_have_pointer_sized_contexts(self):
        for xlen in (32, 64):
            width = xlen // 8
            self.assertIn(width, (4, 8))
            self.assertEqual(CONTEXT_REGS * width, 132 if xlen == 32 else 264)

    def test_interrupt_cause_decoding_is_unsigned(self):
        for xlen in (32, 64):
            self.assertEqual(decode_cause(MCAUSE_INT[xlen] | 7, xlen), (True, 7))
            self.assertEqual(decode_cause(11, xlen), (False, 11))
            self.assertEqual(decode_cause(MCAUSE_INT[xlen] - 1, xlen), (False, MCAUSE_MASK[xlen]))

    def test_timer_rollover(self):
        self.assertEqual(timer_delta(0x00000005, 0xFFFFFFFE, 32), 7)
        self.assertEqual(timer_delta(0x0000000000000005, 0xFFFFFFFFFFFFFFFE, 64), 7)

    def test_frame_offsets_match_save_restore_order(self):
        for xlen in (32, 64):
            width = xlen // 8
            offsets = frame_offsets(xlen)
            self.assertEqual(offsets["pc"], 0)
            self.assertEqual(offsets["mstatus"], width)
            self.assertEqual(offsets["x1"], 2 * width)
            self.assertEqual(offsets["x31"], 32 * width)
            self.assertEqual(max(offsets.values()) + width, CONTEXT_REGS * width)

    def test_timer_preset_frequencies(self):
        presets = {
            "simrv": {"hz": 1_000_000, "ticks_per_us": 1, "tick_div_10ms": 10_000},
            "rvcomp": {"hz": 150_000_000, "ticks_per_us": 150, "tick_div_10ms": 1_500_000},
        }
        for name, spec in presets.items():
            ticks_per_us = spec["hz"] // 1_000_000
            self.assertEqual(ticks_per_us, spec["ticks_per_us"], f"{name} ticks/us mismatch")
            tick_div_10ms = spec["hz"] // (1000 // 10)
            self.assertEqual(tick_div_10ms, spec["tick_div_10ms"], f"{name} tick div mismatch")

    def test_plic_claim_complete_mapping(self):
        plic_base = 0x0c000000
        claim_complete_offset = 0x200004
        self.assertEqual(plic_base + claim_complete_offset, 0x0c200004)

    def test_minimal_csr_set_and_absence_of_unneeded_csrs(self):
        required_csrs = {"mstatus", "mie", "mtvec", "mepc", "mcause"}
        unneeded_csrs = {"mscratch", "mtval", "mbadaddr", "misa"}
        repo_root = Path(__file__).resolve().parents[1]
        source_paths = list((repo_root / "kernel/sysdepend/cpu/core/riscv").glob("*.[cSh]")) + \
                       list((repo_root / "kernel/sysdepend/iote_riscv").glob("*.[ch]")) + \
                       list((repo_root / "lib/libtk/sysdepend/cpu/core/riscv").glob("*.[ch]"))
        combined_text = "\n".join(p.read_text(encoding="utf-8") for p in source_paths if p.is_file())
        for csr in required_csrs:
            self.assertIn(csr, combined_text, f"Expected minimal CSR {csr} to be used in port")
        for csr in unneeded_csrs:
            self.assertNotIn(f"csrr {csr}", combined_text, f"Unexpected CSR {csr} read found in port")
            self.assertNotIn(f"csrw {csr}", combined_text, f"Unexpected CSR {csr} write found in port")


if __name__ == "__main__":
    unittest.main()
