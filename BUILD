cc_library(
    name = "gasket-lib",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["src/*.h"]),
    copts = ["-Iconandeps/tinyxml2/include"],
    deps = [
        "@tinyxml2//:tinyxml2",
    ],
)

cc_binary(
    name = "gasket",
    deps = [
        ":gasket-lib",
    ],
)
