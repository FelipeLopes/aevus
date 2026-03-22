load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "grad",
    hdrs = glob(["*.hpp"]),
    srcs = glob(["*.cpp"]),
    deps = [
        "//:boost",
    ],
    visibility = ["//visibility:public"],
)