cc_library(
    name = "core",
    srcs = glob(["src/core/*.cpp"]),
    hdrs = glob(["src/core/*.hpp"]),
    deps = [
        "@tinyxml2//:tinyxml2",
        "@boost//:boost",
    ]
)

cc_library(
    name = "render",
    srcs = glob(["src/render/*.cpp"]),
    hdrs = glob(["src/render/*.hpp"]),
    deps = [
        "@opencl-headers//:opencl-headers",
        ":core",
    ]
)

cc_binary(
    name = "aevus",
    srcs = ["src/main.cpp"],
    deps = [
        ":render",
        ":core",
        "@opencl//:opencl",
    ],
)
