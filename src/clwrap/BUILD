load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "clwrap",
    hdrs = glob(["*.hpp"]),
    srcs = glob(["*.cpp"]),
    deps = [
        "//:opencl",
    ],
    visibility = ["//visibility:public"],
)