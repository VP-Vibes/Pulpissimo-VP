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
  static unsigned char data{0};

  auto cmd{gp.get_command()};
  if (cmd == tlm::TLM_READ_COMMAND) {
    for (int i = 0; i < gp.get_data_length(); ++i) {
      gp.get_data_ptr()[i] = data;
      data++;
    }
  } else if (cmd == tlm::TLM_WRITE_COMMAND) {
    for (int i = 0; i < gp.get_data_length(); ++i) {
      std::cout << "[external spi device] data = " << static_cast<uint32_t>(gp.get_data_ptr()[i]) << "\n";
    }
  }

  gp.set_response_status(tlm::TLM_OK_RESPONSE);
}

AudioDevice::AudioDevice(sc_core::sc_module_name name) : sc_core::sc_module{name} {
  socket.register_b_transport(this, &AudioDevice::b_transport);
}

void AudioDevice::b_transport(tlm::tlm_generic_payload &gp, sc_core::sc_time &d) {
  static unsigned char data{0};

  auto cmd{gp.get_command()};
  if (cmd == tlm::TLM_READ_COMMAND) {
    for (int i = 0; i < gp.get_data_length(); ++i) {
      gp.get_data_ptr()[i] = data;
      data += 2;
    }
  }

  gp.set_response_status(tlm::TLM_OK_RESPONSE);
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
  pulpissimo_soc_.connectSPIMSocket(0, spi_sink_.socket);

  pulpissimo_soc_.connectI2SSocket(0, audio_device_.socket);
  pulpissimo_soc_.connectI2SSocket(1, i2s_sink_.socket);
}
}  // namespace tgc_vp
