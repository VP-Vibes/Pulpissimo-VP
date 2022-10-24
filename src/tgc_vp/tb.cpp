/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "tgc_vp/tb.h"
namespace tgc_vp {

SC_HAS_PROCESS(VP);
VP::VP(const sc_core::sc_module_name &nm) : sc_core::sc_module(nm) {
  rst_gen.rst_n(rst_n);
  pulpissimo_soc.erst_n(rst_n);
}
}  // namespace tgc_vp
