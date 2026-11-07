LANVIBE milestone 1 - roster prototype

Adds src/pc/net_multi_roster.{c,h} and tools/test_net_multi_roster.c.
No existing files are changed. Does NOT enable 3-player LAN.
The module is intentionally not connected to the build or to sockets yet.

Contains: stable controller slots, duplicate ID rejection, full-lobby rejection,
revision-based acknowledgement barrier, reset on join/leave, 3/4 player tests.

To run on an environment with a C compiler:
  cc -std=c11 -Wall -Wextra -Werror -Isrc/pc tools/test_net_multi_roster.c src/pc/net_multi_roster.c -o roster_test
  ./roster_test

Next milestone: integrate host roster into a separate versioned multiplayer
LAN handshake, without changing the original 2-player networking path.
