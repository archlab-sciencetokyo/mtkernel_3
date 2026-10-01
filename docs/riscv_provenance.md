# RISC-V contribution provenance checklist

This repository is a fork/port of the official TRON Forum `mtkernel_3` source.
The RISC-V work is a derivative port of that upstream μT-Kernel 3.0 source, not
an independent kernel implementation. This is an engineering review aid, not
legal advice.

Record the upstream repository, source revision, and local fork revision when
publishing a port release:

- Upstream: `https://github.com/tron-forum/mtkernel_3`
- Upstream source revision: ____________________
- Local fork/port revision: ____________________

- [ ] Modified inherited files retain TRON Forum copyright and T-License notices.
- [ ] The release identifies itself as a fork/port and preserves upstream source history.
- [ ] New RISC-V files identify their contribution scope and use the repository
      T-License 2.2 notice format.
- [ ] No code was copied from CFU-Proving Ground, RVComp, or another external
      repository without recording origin and license.
- [ ] Board-specific RTL, addresses, startup code, and interrupt-controller code
      remain outside the generic RISC-V core.
- [ ] The distributed source includes `docs/TEF000-219-200401.pdf` or an
      authoritative equivalent of T-License 2.2.
- [ ] Product documentation uses the indication required by the applicable
      T-License 2.2 notice form.
- [ ] Generated objects, maps, binaries, traces, and local build outputs are excluded.
- [ ] Third-party code and notices are listed before redistribution.
- [ ] Ambiguous ownership or licensing is recorded for human review.

Run `python3 scripts/audit_riscv_release.py` before publishing a RISC-V revision.
