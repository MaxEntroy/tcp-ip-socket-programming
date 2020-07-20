cc_binary(
  name = "CalClient",
  srcs = [
    "main.cc",
  ],
  deps = [
    ":cal_client",
    "#gflags",
    "#pthread"
  ]
)

cc_library(
  name = "cal_client",
  srcs = [
    "cal_client.cc"
  ],
  deps = [
    "//common:tcp_client",
    "//common:io_aux",
    "//common:err",
    "//app_protocol:cal_proto",
  ]
)
