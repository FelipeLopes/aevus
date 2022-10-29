cc_library(
    name = "gasket-lib",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["src/*.hpp"]),
    deps = [
        "@tinyxml2//:tinyxml2",
        "@gmp//:gmp",
        "@boost//:boost",
    ],
)

cc_binary(
    name = "gasket",
    deps = [
        ":gasket-lib",
    ],
)
