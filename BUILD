cc_library(
    name = "gasket-lib",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["src/*.h"]),
    deps = [
        "@tinyxml2//:tinyxml2",
        "@gmp//:gmp",
    ],
)

cc_binary(
    name = "gasket",
    deps = [
        ":gasket-lib",
    ],
)
