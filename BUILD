cc_library(
    name = "gasket-lib",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["src/*.h"]),
)

cc_binary(
    name = "gasket",
    deps = [
        ":gasket-lib",
    ],
)
