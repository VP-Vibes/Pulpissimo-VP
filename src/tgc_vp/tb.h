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
  VP(sc_core::sc_module_name const& nm);
  virtual ~VP() = default;
  // even though below are not needed but they
  // (i) make my intent clear that this should not be defined in future to avoid slicing problem
  // (ii) nice compiler message if copy/move are done
  VP(const VP&) = delete;
  VP& operator=(const VP&) = delete;
  VP(VP&&) = delete;
  VP& operator=(VP&&) = delete;
  // in case copying is needed then do a clone function
  // virtual std::unique_ptr<VP> clone() const;

 protected:
  tgc_vp::PulpissimoSoC pulpissimo_soc{"pulpissimo-soc"};
  tgc_vp::rst_gen rst_gen{"rst_gen"};
  sc_core::sc_signal<bool> rst_n{"rst_n"};
};

} /* namespace tgc_vp */

#endif /* SRC_TGC_VP_TB_H_ */
