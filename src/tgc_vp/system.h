/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// TODO: remove legacy TGC-VP stuff

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

// TODO: remove unneeded headers

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
// #include <util/sparse_array.h>

#include "pulpissimo/interrupt.h"
#include "pulpissimo/soc_ctrl.h"
#include "pulpissimo/soc_event.h"
#include "pulpissimo/timer.h"
#include "pulpissimo/udma.h"
#include "tlm_utils/simple_target_socket.h"

namespace tgc_vp {

class fakeMem : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<fakeMem> socket{};

  fakeMem(scc::memory<512_kB, 32> *l2_mem_ptr)
      : sc_core::sc_module{sc_core::sc_module_name{"l2-aliased-mem"}}, mem_{l2_mem_ptr} {
    socket.register_b_transport(this, &fakeMem::b_transport);
  }

 private:
  scc::memory<512_kB, 32> *mem_{nullptr};

  void b_transport(tlm::tlm_generic_payload &gp, sc_core::sc_time &t) {
    // handling request in L2
    mem_->handle_operation(gp, t);
  }
};

class PulpissimoSoC : public sc_core::sc_module {
  SC_HAS_PROCESS(PulpissimoSoC);

 public:
  sc_core::sc_in<bool> erst_n{"erst_n"};

  PulpissimoSoC(sc_core::sc_module_name nm);

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
  scc::memory<8_kB, 32> boot_rom{"boot_rom"};
  scc::memory<512_kB, 32> l2_mem{"l2_mem"};
  vpvper::pulpissimo::udma udma{"udma", &l2_mem};
  vpvper::pulpissimo::soc_ctrl soc_ctrl{"soc_control"};
  vpvper::pulpissimo::interrupt eic{"eic"};
  vpvper::pulpissimo::soc_event soc_event{"soc_event"};
  vpvper::pulpissimo::timer timer{"timer"};

  // TODO: later make this FLL
  vpvper::sifive::prci prci{"prci"};

  // there is a l2-aliased section in memory starting in SW from 0x0 worth 16kB
  // i think the loader then puts this section at start of L2 memory
  // as our VP loads up the memory module using info from elf so we fake
  // a memory section in 0x0-0x3fff space that in effect just references the L2
  fakeMem fake_mem{&l2_mem};

 protected:
  // this method is used to generate inverse reset signal from input reset, to be compatible with rest of SoC
  // peripherals
  void resetCb();
};

}  // namespace tgc_vp

#endif /* _PLATFORM_H_ */
