/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "net_multi_roster.h"

#include <stddef.h>
#include <string.h>

bool pc_net_multi_roster_init(PcNetMultiRoster* roster, uint8_t capacity,
                              uint64_t host_id) {
    if (!roster || !host_id || capacity < PC_NET_MULTI_MIN_PLAYERS ||
        capacity > PC_NET_MULTI_MAX_PLAYERS)
        return false;
    memset(roster, 0, sizeof(*roster));
    roster->capacity = capacity;
    roster->peer_ids[0] = host_id;
    roster->occupied = 1u;
    roster->revision = 1;
    return true;
}

int pc_net_multi_roster_port(const PcNetMultiRoster* roster, uint64_t peer_id) {
    if (!roster || !peer_id)
        return -1;
    for (int i = 0; i < roster->capacity && i < PC_NET_MULTI_MAX_PLAYERS; i++)
        if ((roster->occupied & (1u << i)) && roster->peer_ids[i] == peer_id)
            return i;
    return -1;
}

bool pc_net_multi_roster_join(PcNetMultiRoster* roster, uint64_t peer_id,
                              uint8_t* assigned_port) {
    if (!roster || !peer_id || !assigned_port ||
        roster->capacity < PC_NET_MULTI_MIN_PLAYERS ||
        roster->capacity > PC_NET_MULTI_MAX_PLAYERS ||
        pc_net_multi_roster_port(roster, peer_id) >= 0)
        return false;
    for (uint8_t port = 1; port < roster->capacity; port++) {
        if (!(roster->occupied & (1u << port))) {
            roster->peer_ids[port] = peer_id;
            roster->occupied |= (uint8_t)(1u << port);
            roster->ready = 0; /* any roster change invalidates all ACKs */
            roster->revision++;
            *assigned_port = port;
            return true;
        }
    }
    return false;
}

bool pc_net_multi_roster_leave(PcNetMultiRoster* roster, uint64_t peer_id) {
    int port = pc_net_multi_roster_port(roster, peer_id);
    if (!roster || port <= 0) /* host cannot leave without closing lobby */
        return false;
    roster->peer_ids[port] = 0;
    roster->occupied &= (uint8_t)~(1u << port);
    roster->ready = 0;
    roster->revision++;
    return true;
}

bool pc_net_multi_roster_ack(PcNetMultiRoster* roster, uint64_t peer_id,
                             uint32_t revision) {
    int port = pc_net_multi_roster_port(roster, peer_id);
    if (!roster || port < 0 || revision != roster->revision)
        return false;
    roster->ready |= (uint8_t)(1u << port);
    return true;
}

bool pc_net_multi_roster_ready(const PcNetMultiRoster* roster) {
    if (!roster || roster->capacity < PC_NET_MULTI_MIN_PLAYERS ||
        roster->capacity > PC_NET_MULTI_MAX_PLAYERS)
        return false;
    const uint8_t required = (uint8_t)((1u << roster->capacity) - 1u);
    return roster->occupied == required && (roster->ready & required) == required;
}
