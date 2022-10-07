/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright (C) 2018 GreenWaves Technologies
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Authors: Eric Flamand, GreenWaves Technologies (eric.flamand@greenwaves-technologies.com)
 *  	    Germain Haugou, ETH (germain.haugou@iis.ee.ethz.ch)
 */


#include "rt/rt_api.h"

int __rt_fll_freq[ARCHI_NB_FLL];
static char __rt_fll_is_on[ARCHI_NB_FLL];

static unsigned int __rt_fll_get_mult_div_from_freq(unsigned int freq, unsigned int *mult, unsigned int *div)
{
  unsigned int fref = ARCHI_REF_CLOCK;
  unsigned int Log2M = __FL1(freq) - __FL1(fref);
  unsigned int D = __MAX(1, (FLL_LOG2_MAXM - Log2M)>>1);
  unsigned int M = (freq<<D)/fref;
  unsigned int fres;

#if 0 //PULP_CHIP == CHIP_QUENTIN

  // TODO on quentin, the fll never converges (e.g. a mult factor of 15000 is
  // not working), check what is the maximum value we can use
  while (M >= 10000)
  {
    M >>= 1;
    D--;
  };

  if (D == 0) fres = fref*M;
  else fres = (fref*M + (1<<(D-1)))>>D;   /* Rounding */

#else

  fres = (fref*M + (1<<(D-1)))>>D;   /* Rounding */

#endif

  *mult = M; *div = D+1;
  return fres;
}

static unsigned int __rt_fll_get_freq_from_mult_div(unsigned int mult, unsigned int div)
{
  // FreqOut = Fref * Mult/2^(Div-1)	With Mult on 16 bits and Div on 4 bits
  unsigned int fref = ARCHI_REF_CLOCK;
  unsigned int fres = (fref*mult)>>(div-1);
  return fres;
}


unsigned int __rt_fll_set_freq(int fll, unsigned int frequency)
{
  // Synchronize with bridge so that it does not try to access the chip
  // while we are changing the frequency
  if (fll == __RT_FLL_FC)
    __rt_bridge_req_shutdown();

  unsigned int real_freq, mult, div;
  real_freq = __rt_fll_get_mult_div_from_freq(frequency, &mult, &div);
 
  __rt_fll_freq[fll] = real_freq;
  if (__rt_fll_is_on[fll]) {
    hal_fll_update_mult_div(fll, mult, div);
  }

  return real_freq;
}

void rt_freq_wait_convergence(int fll)
{
  int irq = rt_irq_disable();
  int mult_factor_diff;
  fll_reg_conf1_t fll_conf1;
  fll_reg_conf2_t fll_conf2;
  fll_conf1.raw = hal_fll_conf_reg1_get(fll);
  fll_conf2.raw = hal_fll_conf_reg2_get(fll);
  int mult = fll_conf1.mult_factor;
  int tolerance = fll_conf2.lock_tolerance;

  do {
    mult_factor_diff = hal_fll_status_reg_get(fll) - mult;
    if (mult_factor_diff < 0)
      mult_factor_diff = -mult_factor_diff;

    if ( mult_factor_diff <= tolerance)
      break;

    rt_irq_mask_set(1<<ARCHI_FC_EVT_CLK_REF);
    rt_wait_for_interrupt();
    rt_irq_mask_clr(1<<ARCHI_FC_EVT_CLK_REF);
  } while (1);

  rt_irq_restore(irq);
}

unsigned int __rt_fll_init(int fll)
{
  rt_trace(RT_TRACE_INIT, "Initializing FLL (fll: %d)\n", fll);
  

  // In case the FLL frequency was set while it was off, update it immediately
  unsigned int freq = 200000000;

  __rt_fll_is_on[fll] = 1;

  rt_trace(RT_TRACE_INIT, "FLL is locked (fll: %d, freq: %d)\n", fll, freq);

  return freq;
}

void __rt_fll_deinit(int fll)
{
  // Just remember the FLL is off. Keep the same frequency so that we can restore the FLL
  // with the same frequency later on
  __rt_fll_is_on[fll] = 0;

  // TODO this should shutdown FLL
}

void __rt_flls_constructor()
{
  for (int i=0; i<ARCHI_NB_FLL; i++)
  {
    __rt_fll_freq[i] = 0;
    __rt_fll_is_on[i] = 0;
  }
}
