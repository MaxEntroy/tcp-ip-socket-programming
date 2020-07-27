cc_library(
  name = "err",
  srcs = [
    "err.cc"
  ],
  deps = []
)

cc_library(
  name = "udp_server",
  srcs = [
    "udp_server.cc"
  ],
  deps = [
    ":err"
  ]
)

cc_library(
  name = "udp_client",
  srcs = [
    "udp_client.cc"
  ],
  deps = [
    ":err"
  ]
)
