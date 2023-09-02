cc_library(
    name = "tinyxml2",
    deps = [
        "@hdrs//:tinyxml2",
        "@libs//:tinyxml2",
    ],
)

cc_library(
    name = "boost",
    deps = [
        "@hdrs//:boost",
    ],
)

cc_library(
    name = "opencl",
    deps = [
        "@hdrs//:opencl",
        "@libs//:opencl",
    ],
)

cc_library(
    name = "wxwidgets",
    deps = [
        "@hdrs//:wxwidgets",
        "@libs//:wxwidgets",
    ],
)

cc_library(
    name = "serial",
    hdrs = glob(["src/serial/*.hpp"]),
    srcs = glob(["src/serial/*.cpp"]),
    deps = [
        ":tinyxml2",
        ":boost",
    ]
)

cc_library(
    name = "clwrap",
    hdrs = glob(["src/clwrap/*.hpp"]),
    srcs = glob(["src/clwrap/*.cpp"]),
    deps = [
        ":opencl",
    ]
)

cc_library(
    name = "core",
    hdrs = glob(["src/core/*.hpp"]),
    srcs = glob(["src/core/*.cpp"]),
    deps = [
        ":boost",
        ":serial",
    ]
)

cc_library(
    name = "render",
    hdrs = glob(["src/render/*.hpp"]),
    srcs = glob(["src/render/*.cpp"]),
    deps = [
        ":clwrap",
        ":core",
    ]
)

filegroup(
    name = "image_files",
    srcs = glob(["res/*.png"]),
)

genrule(
    name = "image_embed",
    srcs = ["//:image_files"],
    outs = ["images.o"],
    cmd = "ld -r -b binary -o $@ $(locations //:image_files)",
)

cc_library(
    name = "images",
    srcs = ["//:image_embed"],
)

cc_library(
    name = "wxfb",
    hdrs = ["src/wxfb/wxfb_frame.h"],
    srcs = ["src/wxfb/wxfb_frame.cpp"],
    deps = [
        ":wxwidgets",
    ],
)

cc_library(
    name = "ui",
    srcs = glob(["src/ui/*.cpp"]),
    hdrs = glob(["src/ui/*.hpp"]),
    deps = [
        ":core",
        ":images",
        ":wxfb",
    ]
)

cc_binary(
    name = "aevus",
    srcs = ["src/aevus.cpp"],
    deps = [
        ":render",
        ":ui",
        ":opencl",
    ],
)
