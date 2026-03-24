load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "core",
    hdrs = glob(["*.hpp"]),
    srcs = glob(["*.cpp"]),
    deps = [
        "//src/grad:grad",
        "//:tinyxml2",
        "//:boost",
    ],
    visibility = ["//visibility:public"],
)