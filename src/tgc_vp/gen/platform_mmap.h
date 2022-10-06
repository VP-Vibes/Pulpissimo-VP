/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _PLATFORM_MMAP_H_
#define _PLATFORM_MMAP_H_
// need double braces, see
// https://stackoverflow.com/questions/6893700/how-to-construct-stdarray-object-with-initializer-list#6894191
const std::array<scc::target_memory_map_entry<32>, 5> platfrom_mmap = {{{clint.socket, 0x2000000, 0xc000},
                                                                        {plic.socket, 0xc000000, 0x200008},
                                                                        {aon.socket, 0x10000000, 0x150},
                                                                        {prci.socket, 0x10008000, 0x14},
                                                                        {uart0.socket, 0x10013000, 0x1c}}};

#endif /* _PLATFORM_MMAP_H_ */
