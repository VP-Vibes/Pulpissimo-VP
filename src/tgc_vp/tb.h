/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SRC_TGC_VP_TB_H_
#define SRC_TGC_VP_TB_H_

#include <generic/terminal.h>

#include <systemc>

#include "tgc_vp/rst_gen.h"
#include "tgc_vp/system.h"
namespace tgc_vp {

class SPIDevice final : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<SPIDevice> socket{};

  explicit SPIDevice(sc_core::sc_module_name);

 private:
  void b_transport(tlm::tlm_generic_payload&, sc_core::sc_time&);
};

class VP : public sc_core::sc_module {
  SC_HAS_PROCESS(VP);

 protected:
  class DummySink final : public sc_core::sc_module {
   public:
    tlm_utils::simple_target_socket<DummySink> socket{};
    explicit DummySink(sc_core::sc_module_name);

   private:
    void b_transport(tlm::tlm_generic_payload&, sc_core::sc_time&);
  };

 public:
  VP(sc_core::sc_module_name const& nm);
  virtual ~VP() = default;
  // to avoid slicing problems
  VP(const VP&) = delete;
  VP& operator=(const VP&) = delete;
  VP(VP&&) = delete;
  VP& operator=(VP&&) = delete;
  // in case copying is needed then do a clone function
  // virtual std::unique_ptr<VP> clone() const;

 protected:
  sc_core::sc_signal<bool> rst_n_{"rst_n"};

  tgc_vp::rst_gen rst_gen_{"rst_gen"};
  tgc_vp::PulpissimoSoC pulpissimo_soc_{"pulpissimo-soc"};
  SPIDevice spi_device_{"spi-device"};
  std::array<DummySink, 3> spi_sinks_{{DummySink{"spi-sink-0"}, DummySink{"spi-sink-1"}, DummySink{"spi-sink-2"}}};
};

} /* namespace tgc_vp */

#endif /* SRC_TGC_VP_TB_H_ */
