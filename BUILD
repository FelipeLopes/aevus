cc_library(
    name = "gasket",
    srcs = glob(["src/gasket/*.cpp"]),
    hdrs = glob(["src/gasket/*.hpp"]),
    deps = [
        "@tinyxml2//:tinyxml2",
        "@gmp//:gmp",
        "@boost//:boost",
    ],
)

cc_binary(
    name = "aevus",
    srcs = ["src/main.cpp"],
    deps = [
        ":gasket",
    ],
)
