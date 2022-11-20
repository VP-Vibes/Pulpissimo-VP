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

PulpissimoSoC::PulpissimoSoC(sc_core::sc_module_name nm) : vpvper::pulpissimo::SoC{nm}, NAMED(router, 9, 1) {
  SC_METHOD(resetCb);
  sensitive << erst_n;
  SC_THREAD(raiseCoreInterrupt);

  core_complex.mhartid = 992;
  core_complex.initiator(router.target[0]);

  size_t i = 0;
  router.initiator.at(i)(eic.socket);
  router.set_target_range(i, 0x1A109000, 8_kB);

  router.initiator.at(++i)(prci.socket);
  router.set_target_range(i, 0x10008000, 0x14);

  router.initiator.at(++i)(boot_rom.target);
  router.set_target_range(i, 0x1A000000, 8_kB);

  router.initiator.at(++i)(l2_mem.target);
  router.set_target_range(i, 0x1C000000, 512_kB);

  router.initiator.at(++i)(udma.socket);
  router.set_target_range(i, 0x1A102000, 8_kB);

  router.initiator.at(++i)(soc_ctrl.socket);
  router.set_target_range(i, 0x1A104000, 4_kB);

  router.initiator.at(++i)(soc_event.socket);
  router.set_target_range(i, 0x1A106000, 12_kB);

  router.initiator.at(++i)(fake_mem.socket);
  router.set_target_range(i, 0x0, 16_kB);

  router.initiator.at(++i)(timer.socket);
  router.set_target_range(i, 0x1A10B000, 4_kB);

  prci.hfclk_o(tlclk_s);  // clock driver
  core_complex.clk_i(tlclk_s);
  udma.clk_i(tlclk_s);
  soc_ctrl.clk_i(tlclk_s);
  eic.clk_i(tlclk_s);
  soc_event.clk_i(tlclk_s);
  timer.clk_i(tlclk_s);

  prci.rst_i(rst_s);
  core_complex.rst_i(rst_s);
  udma.rst_i(rst_s);
  soc_ctrl.rst_i(rst_s);
  eic.rst_i(rst_s);
  soc_event.rst_i(rst_s);
  timer.rst_i(rst_s);

  core_complex.sw_irq_i(msie_int_s);
  core_complex.timer_irq_i(mtime_int_s);
  core_complex.global_irq_i(core_int_s);
  core_complex.local_irq_i(local_int_s);
}

void PulpissimoSoC::resetCb() {
  if (!erst_n.read()) {
    rst_s = true;
  } else {
    rst_s = false;
  }
}

void PulpissimoSoC::readMemory(unsigned char *data, uint32_t addr, size_t len) {
  sc_core::sc_time delay{sc_core::SC_ZERO_TIME};
  tlm::tlm_generic_payload gp{};
  gp.set_command(tlm::TLM_READ_COMMAND);
  gp.set_address(addr);
  gp.set_data_ptr(data);
  gp.set_data_length(len);
  gp.set_streaming_width(len);

  l2_mem.handle_operation(gp, delay);
  if (gp.get_response_status() != tlm::TLM_OK_RESPONSE) {
    std::cerr << "[pulpissimo] memory not read properly";
  }
}

void PulpissimoSoC::writeMemory(unsigned char *data, uint32_t addr, size_t len) {
  sc_core::sc_time delay{sc_core::SC_ZERO_TIME};
  tlm::tlm_generic_payload gp{};
  gp.set_command(tlm::TLM_WRITE_COMMAND);
  gp.set_address(addr);
  gp.set_data_ptr(data);
  gp.set_data_length(len);
  gp.set_streaming_width(len);

  l2_mem.handle_operation(gp, delay);
  if (gp.get_response_status() != tlm::TLM_OK_RESPONSE) {
    std::cerr << "[pulpissimo] memory not written properly";
  }
}

void PulpissimoSoC::connectSPIMSocket(size_t id, tlm::tlm_target_socket<> &target_socket) {
  sockets_.spim_initiators[id].bind(target_socket);
}

void PulpissimoSoC::connectI2SSocket(size_t id, tlm::tlm_target_socket<> &target_socket) {
  sockets_.i2s_initiators[id].bind(target_socket);
}

void PulpissimoSoC::transmitSPIMSocket(size_t id, tlm::tlm_generic_payload &gp, sc_core::sc_time &delay) {
  sockets_.spim_initiators[id]->b_transport(gp, delay);
}

void PulpissimoSoC::setEvent(size_t id) { soc_event.push(id); }

void PulpissimoSoC::raiseInterrupt(size_t id) {
  // eic.push(id);

  raise_core_irq_.notify();
}

void PulpissimoSoC::raiseCoreInterrupt() {
  while (1) {
    wait(raise_core_irq_);
    core_int_s.write(true);
    // we raise it high for >= cycle
    // currently the TGC core is running at 70 MHz though I am not sure if Pulpissimo demands a fixed
    // operating frequency on its CPU core
    wait(15, SC_NS);
    core_int_s.write(false);
  }
}

}  // namespace tgc_vp
