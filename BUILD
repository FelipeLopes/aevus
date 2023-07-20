cc_library(
    name = "gasket",
    srcs = glob(["src/gasket/*.cpp"]),
    hdrs = glob(["src/gasket/*.hpp", "src/render/*.hpp"]),
    deps = [
        "@tinyxml2//:tinyxml2",
        "@gmp//:gmp",
        "@boost//:boost",
    ],
)

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
        "@tinyxml2//:tinyxml2",
        "@boost//:boost",
    ]
)

cc_binary(
    name = "aevus",
    srcs = ["src/main.cpp"],
    deps = [
        ":gasket",
        ":render",
        ":core",
        "@opencl//:opencl"
    ],
)
