/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Standalone: cc -std=c11 -Wall -Wextra -Werror -Isrc/pc
 * tools/test_net_multi_roster.c src/pc/net_multi_roster.c -o roster_test */
#include <assert.h>
#include <stdio.h>

#include "../src/pc/net_multi_roster.h"

int main(void) {
    PcNetMultiRoster r;
    uint8_t port = 99;
    assert(!pc_net_multi_roster_init(&r, 2, 11));
    assert(!pc_net_multi_roster_init(&r, 3, 0));
    assert(pc_net_multi_roster_init(&r, 3, 11));
    assert(!pc_net_multi_roster_ready(&r));
    assert(!pc_net_multi_roster_join(&r, 11, &port));
    assert(pc_net_multi_roster_join(&r, 22, &port) && port == 1);
    assert(pc_net_multi_roster_join(&r, 33, &port) && port == 2);
    assert(!pc_net_multi_roster_join(&r, 44, &port));
    assert(pc_net_multi_roster_port(&r, 33) == 2);
    assert(!pc_net_multi_roster_ack(&r, 11, r.revision - 1));
    assert(pc_net_multi_roster_ack(&r, 11, r.revision));
    assert(pc_net_multi_roster_ack(&r, 22, r.revision));
    assert(!pc_net_multi_roster_ready(&r));
    assert(pc_net_multi_roster_ack(&r, 33, r.revision));
    assert(pc_net_multi_roster_ready(&r));
    assert(!pc_net_multi_roster_leave(&r, 11));
    assert(pc_net_multi_roster_leave(&r, 22));
    assert(!pc_net_multi_roster_ready(&r));
    assert(pc_net_multi_roster_join(&r, 44, &port) && port == 1);
    assert(!pc_net_multi_roster_ready(&r));
    assert(pc_net_multi_roster_ack(&r, 11, r.revision));
    assert(pc_net_multi_roster_ack(&r, 33, r.revision));
    assert(pc_net_multi_roster_ack(&r, 44, r.revision));
    assert(pc_net_multi_roster_ready(&r));

    assert(pc_net_multi_roster_init(&r, 4, 10));
    assert(pc_net_multi_roster_join(&r, 20, &port) && port == 1);
    assert(pc_net_multi_roster_join(&r, 30, &port) && port == 2);
    assert(pc_net_multi_roster_join(&r, 40, &port) && port == 3);
    for (uint64_t id = 10; id <= 40; id += 10)
        assert(pc_net_multi_roster_ack(&r, id, r.revision));
    assert(pc_net_multi_roster_ready(&r));
    puts("multi roster tests: PASS");
    return 0;
}
