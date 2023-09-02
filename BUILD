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
    srcs = glob(["src/serial/*.cpp"]),
    hdrs = glob(["src/serial/*.hpp"]),
    deps = [
        ":tinyxml2",
        ":boost",
    ]
)

cc_library(
    name = "clwrap",
    srcs = glob(["src/clwrap/*.cpp"]),
    hdrs = glob(["src/clwrap/*.hpp"]),
    deps = [
        ":opencl",
    ]
)

cc_library(
    name = "core",
    srcs = glob(["src/core/*.cpp"]),
    hdrs = glob(["src/core/*.hpp"]),
    deps = [
        ":boost",
        ":serial",
    ]
)

cc_library(
    name = "render",
    srcs = glob(["src/render/*.cpp"]),
    hdrs = glob(["src/render/*.hpp"]),
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
    name = "ui",
    srcs = glob(["src/ui/*.cpp"]) + ["src/ui/wxfb/code/wxfb_frame.cpp"],
    hdrs = glob(["src/ui/*.hpp"]) + ["src/ui/wxfb/code/wxfb_frame.h"],
    deps = [
        ":core",
        ":images",
        ":wxwidgets",
    ]
)

cc_binary(
    name = "aevus",
    srcs = ["src/main.cpp"],
    deps = [
        ":render",
        ":ui",
        ":opencl",
    ],
)
