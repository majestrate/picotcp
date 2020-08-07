cmake_minimum_required(VERSION 3.10)  # bionic's cmake version

# Has to be set before `project()`, and ignored on non-macos:
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "macOS deployment target (Apple clang only)")

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(PROJECT_NAME libpicotcp)
project(${PROJECT_NAME}
    VERSION 0.1.0
    DESCRIPTION "userland network stack"
    LANGUAGES C CXX)

configure_file(include/pico_defines.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/pico_defines.h @ONLY)
  
add_library(picotcp STATIC
  ${CMAKE_CURRENT_BINARY_DIR}/include/pico_defines.h
  stack/pico_device.c
  stack/pico_frame.c
  stack/pico_md5.c
  stack/pico_protocol.c
  stack/pico_socket.c
  stack/pico_socket_multicast.c
  stack/pico_stack.c
  stack/pico_tree.c
  modules/pico_6lowpan.c
  modules/pico_6lowpan_ll.c
  modules/pico_802154.c
  modules/pico_aodv.c
  modules/pico_arp.c
  modules/pico_dev_ipc.c
  modules/pico_dev_loop.c
  modules/pico_dev_mock.c
  modules/pico_dev_null.c
  modules/pico_dev_pcap.c
  modules/pico_dev_pipe.c
  modules/pico_dev_ppp.c
  modules/pico_dev_radio_mgr.c
  modules/pico_dev_radiotest.c
  modules/pico_dev_tap.c
  modules/pico_dev_tap_windows.c
  modules/pico_dev_tun.c
  modules/pico_dev_vde.c
  modules/pico_dhcp_client.c
  modules/pico_dhcp_common.c
  modules/pico_dhcp_server.c
  modules/pico_dns_client.c
  modules/pico_dns_common.c
  modules/pico_dns_sd.c
  modules/pico_ethernet.c
  modules/pico_fragments.c
  modules/pico_hotplug_detection.c
  modules/pico_icmp4.c
  modules/pico_icmp6.c
  modules/pico_igmp.c
  modules/pico_ipfilter.c
  modules/pico_ipv4.c
  modules/pico_ipv6.c
  modules/pico_ipv6_nd.c
  modules/pico_mcast.c
  modules/pico_mdns.c
  modules/pico_mld.c
  modules/pico_mm.c
  modules/pico_nat.c
  modules/pico_olsr.c
  modules/pico_posix.c
  modules/pico_slaacv4.c
  modules/pico_sntp_client.c
  modules/pico_socket_tcp.c
  modules/pico_socket_udp.c
  modules/pico_strings.c
  modules/pico_tcp.c
  modules/pico_tftp.c
  modules/pico_udp.c)

target_include_directories(picotcp PRIVATE modules)

target_include_directories(picotcp
    PUBLIC
        $<INSTALL_INTERFACE:>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

add_library(picotcp::picotcp ALIAS picotcp)

export(
    TARGETS picotcp
    NAMESPACE picotcp::
    FILE picotcpTargets.cmake
)
install(
    TARGETS picotcp
    EXPORT picotcpConfig
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
