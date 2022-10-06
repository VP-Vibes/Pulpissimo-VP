/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// TODO: remove legacy TGC-VP stuff

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <scc/memory.h>
#include <scc/router.h>
#include <scc/utilities.h>
#include <sifive/aon.h>
#include <sifive/clint.h>
#include <sifive/gpio.h>
#include <sifive/plic.h>
#include <sifive/prci.h>
#include <sifive/pwm.h>
#include <sifive/spi.h>
#include <sifive/uart.h>
#include <sifive/uart_terminal.h>
#include <sysc/core_complex.h>
#include <sysc/kernel/sc_module.h>
#include <tlm/scc/tlm_signal_sockets.h>

#include <array>
#include <cci_configuration>
#include <memory>

#include "pulpissimo/udma.h"

namespace tgc_vp {

class system : public sc_core::sc_module {
  SC_HAS_PROCESS(system);

 public:
  sc_core::sc_in<bool> erst_n{"erst_n"};
  // TODO: uart0 is kept for debugging in dev for now. Later remove this
  vpvper::sifive::uart_terminal uart0{"dbg-uart"};

  system(sc_core::sc_module_name nm);

 private:
  sc_core::sc_signal<sc_core::sc_time, sc_core::SC_MANY_WRITERS> tlclk_s{"tlclk_s"};
  sc_core::sc_signal<sc_core::sc_time, sc_core::SC_MANY_WRITERS> lfclk_s{"lfclk_s"};
  sc_core::sc_signal<bool, sc_core::SC_MANY_WRITERS> rst_s{"rst_s"};
  sc_core::sc_signal<bool, sc_core::SC_MANY_WRITERS> mtime_int_s{"mtime_int_s"};
  sc_core::sc_signal<bool, sc_core::SC_MANY_WRITERS> msie_int_s{"msie_int_s"};
  sc_core::sc_vector<sc_core::sc_signal<bool, sc_core::SC_MANY_WRITERS>> global_int_s{"global_int_s", 256};
  sc_core::sc_vector<sc_core::sc_signal<bool, sc_core::SC_MANY_WRITERS>> local_int_s{"local_int_s", 16};
  sc_core::sc_signal<bool, sc_core::SC_MANY_WRITERS> core_int_s{"core_int_s"};

  sysc::tgfs::core_complex core_complex{"core_complex"};
  scc::router<> router;
  scc::memory<512_MB, 32> boot_rom{"boot_rom"};
  scc::memory<512_kB, 32> l2_mem{"l2_mem"};
  vpvper::pulpissimo::udma udma{"udma"};

  // TODO: later make this event&interrupt controller
  vpvper::sifive::plic plic{"plic"};
  // TODO: later make this FLL
  vpvper::sifive::prci prci{"prci"};

  // TODO: Timer..for reference see vpvper::sifive::clint
  // TODO: other peripherals

 protected:
  // this method is used to generate inverse reset signal from input reset, to be compatible with rest of SoC
  // peripherals
  void resetCb();
};

}  // namespace tgc_vp

#endif /* _PLATFORM_H_ */
