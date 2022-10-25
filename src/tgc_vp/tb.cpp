/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "tgc_vp/tb.h"
namespace tgc_vp {

SPIDevice::SPIDevice(sc_core::sc_module_name name) : sc_core::sc_module{name} {
  socket.register_b_transport(this, &SPIDevice::b_transport);
}

void SPIDevice::b_transport(tlm::tlm_generic_payload &gp, sc_core::sc_time &d) {
  //
  std::cout << "SPIDevice accessed\n";
}

VP::DummySink::DummySink(sc_core::sc_module_name name) : sc_core::sc_module{name} {
  socket.register_b_transport(this, &DummySink::b_transport);
}

void VP::DummySink::b_transport(tlm::tlm_generic_payload &, sc_core::sc_time &) {
  std::cout << "ERROR: why are we accessing dummy sink in simulation??\n";
  exit(1);
}

VP::VP(const sc_core::sc_module_name &nm) : sc_core::sc_module(nm) {
  rst_gen_.rst_n(rst_n_);
  pulpissimo_soc_.erst_n(rst_n_);

  pulpissimo_soc_.connectSPIMSocket(1, spi_device_.socket);

  pulpissimo_soc_.connectSPIMSocket(0, spi_sinks_[0].socket);
  pulpissimo_soc_.connectSPIMSocket(2, spi_sinks_[1].socket);
  pulpissimo_soc_.connectSPIMSocket(3, spi_sinks_[2].socket);
}
}  // namespace tgc_vp
