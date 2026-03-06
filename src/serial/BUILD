load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "serial",
    hdrs = glob(["*.hpp"]),
    srcs = glob(["*.cpp"]),
    deps = [
        "//:tinyxml2",
        "//:boost",
    ],
    visibility = ["//visibility:public"],
)
