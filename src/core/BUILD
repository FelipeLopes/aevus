load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "core",
    hdrs = glob(["*.hpp"]),
    srcs = glob(["*.cpp"]),
    deps = [
        "//:boost",
        "//src/serial:serial",
    ],
    visibility = ["//visibility:public"],
)