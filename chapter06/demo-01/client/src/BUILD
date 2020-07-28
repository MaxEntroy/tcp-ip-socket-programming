cc_binary(
  name = "EchoClient",
  srcs = [
    "main.cc"
  ],
  deps = [
    ":echo_client",
    "#gflags",
    "#pthread"
  ]
)

cc_library(
  name = "echo_client",
  srcs = [
    "echo_client.cc"
  ],
  deps = [
    "//common:udp_client"
  ]
)
