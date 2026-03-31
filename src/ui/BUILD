load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "ui",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.hpp"]),
    deps = [
        "//src/core:core",
        "//src/render:render",
        "//res:images",
        "//wxfb:wxfb",
    ],
    visibility = ["//visibility:public"],
)