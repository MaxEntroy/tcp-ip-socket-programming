cc_binary(
  name = "CalServer",
  srcs = [
    "main.cc"
  ],
  deps = [
    ":cal_server",
    "#gflags",
    "#pthread"
  ]
)


cc_library(
  name = "cal_server",
  srcs = [
    "cal_server.cc"
  ],
  deps = [
    "//common:udp_server"
  ]
)
