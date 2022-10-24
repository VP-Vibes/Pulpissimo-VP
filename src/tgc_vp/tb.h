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

class VP : public sc_core::sc_module {
 public:
  tgc_vp::PulpissimoSoC pulpissimo_soc{"pulpissimo-soc"};
  tgc_vp::rst_gen rst_gen{"rst_gen"};
  sc_core::sc_signal<bool> rst_n{"rst_n"};

  VP(sc_core::sc_module_name const& nm);
};

} /* namespace tgc_vp */

#endif /* SRC_TGC_VP_TB_H_ */
