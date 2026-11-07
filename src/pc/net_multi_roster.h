/* SPDX-License-Identifier: GPL-3.0-or-later */
#ifndef PC_NET_MULTI_ROSTER_H
#define PC_NET_MULTI_ROSTER_H

#include <stdbool.h>
#include <stdint.h>

/* Experimental multiplayer roster. This module is not yet connected to
 * the existing two-player session, transport or game simulation. */
#define PC_NET_MULTI_MAX_PLAYERS 4
#define PC_NET_MULTI_MIN_PLAYERS 3

typedef struct PcNetMultiRoster {
    uint64_t peer_ids[PC_NET_MULTI_MAX_PLAYERS];
    uint8_t occupied; /* bit i: controller port i is assigned */
    uint8_t ready;    /* bit i: assigned peer has acknowledged this roster */
    uint8_t capacity; /* 3 or 4, fixed when lobby created */
    uint32_t revision; /* increased on every membership change */
} PcNetMultiRoster;

/* IDs must be unique and nonzero; zero means an unused slot. */
bool pc_net_multi_roster_init(PcNetMultiRoster* roster, uint8_t capacity,
                              uint64_t host_id);
bool pc_net_multi_roster_join(PcNetMultiRoster* roster, uint64_t peer_id,
                              uint8_t* assigned_port);
bool pc_net_multi_roster_leave(PcNetMultiRoster* roster, uint64_t peer_id);
bool pc_net_multi_roster_ack(PcNetMultiRoster* roster, uint64_t peer_id,
                             uint32_t revision);
bool pc_net_multi_roster_ready(const PcNetMultiRoster* roster);
int pc_net_multi_roster_port(const PcNetMultiRoster* roster, uint64_t peer_id);

#endif
