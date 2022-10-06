/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "tgc_vp/system.h"

namespace tgc_vp {
using namespace sc_core;
using namespace vpvper::sifive;
using namespace sysc::tgfs;

system::system(sc_core::sc_module_name nm) : sc_core::sc_module(nm), NAMED(router, platfrom_mmap.size() + 2 + 1, 1) {
  core_complex.initiator(router.target[0]);
  size_t i = 0;
  for (const auto& e : platfrom_mmap) {
    router.initiator.at(i)(e.target);
    router.set_target_range(i, e.start, e.size);
    i++;
  }
  router.initiator.at(i)(mem_qspi.target);
  router.set_target_range(i, 0x20000000, 512_MB);
  router.initiator.at(++i)(mem_ram.target);
  router.set_target_range(i, 0x80000000, 128_kB);

  router.initiator.at(++i)(udma.socket);
  router.set_target_range(i, 0x1A102000, 8_kB);

  plic.clk_i(tlclk_s);
  aon.clk_i(tlclk_s);
  aon.lfclkc_o(lfclk_s);
  prci.hfclk_o(tlclk_s);  // clock driver
  clint.tlclk_i(tlclk_s);
  clint.lfclk_i(lfclk_s);
  core_complex.clk_i(tlclk_s);

  udma.clk_i(tlclk_s);

  uart0.rst_i(rst_s);
  plic.rst_i(rst_s);
  aon.rst_o(rst_s);
  prci.rst_i(rst_s);
  clint.rst_i(rst_s);
  core_complex.rst_i(rst_s);

  udma.rst_i(rst_s);

  aon.erst_n_i(erst_n);

  clint.mtime_int_o(mtime_int_s);
  clint.msip_int_o(msie_int_s);

  plic.global_interrupts_i(global_int_s);
  plic.core_interrupt_o(core_int_s);

  core_complex.sw_irq_i(msie_int_s);
  core_complex.timer_irq_i(mtime_int_s);
  core_complex.global_irq_i(core_int_s);
  core_complex.local_irq_i(local_int_s);

  uart0.irq_o(global_int_s[3]);
}

}  // namespace tgc_vp
