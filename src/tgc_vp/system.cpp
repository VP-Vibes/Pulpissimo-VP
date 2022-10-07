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

system::system(sc_core::sc_module_name nm) : sc_core::sc_module(nm), NAMED(router, 5, 1) {
  SC_METHOD(resetCb);
  sensitive << erst_n;

  core_complex.initiator(router.target[0]);

  size_t i = 0;
  router.initiator.at(i)(plic.socket);
  router.set_target_range(i, 0xc000000, 0x200008);

  router.initiator.at(++i)(prci.socket);
  router.set_target_range(i, 0x10008000, 0x14);

  router.initiator.at(++i)(boot_rom.target);
  router.set_target_range(i, 0x1a000000, 8_kB);

  router.initiator.at(++i)(l2_mem.target);
  router.set_target_range(i, 0x1c000000, 512_kB);

  router.initiator.at(++i)(udma.socket);
  router.set_target_range(i, 0x1A102000, 8_kB);

  plic.clk_i(tlclk_s);
  prci.hfclk_o(tlclk_s);  // clock driver
  core_complex.clk_i(tlclk_s);

  udma.clk_i(tlclk_s);

  plic.rst_i(rst_s);
  prci.rst_i(rst_s);
  core_complex.rst_i(rst_s);

  udma.rst_i(rst_s);

  plic.global_interrupts_i(global_int_s);
  plic.core_interrupt_o(core_int_s);

  core_complex.sw_irq_i(msie_int_s);
  core_complex.timer_irq_i(mtime_int_s);
  core_complex.global_irq_i(core_int_s);
  core_complex.local_irq_i(local_int_s);
}

void system::resetCb() {
  if (!erst_n.read()) {
    rst_s = true;
  } else {
    rst_s = false;
  }
}

}  // namespace tgc_vp
