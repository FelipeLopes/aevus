cc_library(
    name = "serial",
    srcs = glob(["src/serial/*.cpp"]),
    hdrs = glob(["src/serial/*.hpp"]),
    deps = [
        "@tinyxml2-headers//:tinyxml2-headers",
        "@tinyxml2//:tinyxml2",
        "@boost-headers//:boost-headers",
    ]
)

cc_library(
    name = "clwrap",
    srcs = glob(["src/clwrap/*.cpp"]),
    hdrs = glob(["src/clwrap/*.hpp"]),
    deps = [
        "@opencl-headers//:opencl-headers",
    ]
)

cc_library(
    name = "core",
    srcs = glob(["src/core/*.cpp"]),
    hdrs = glob(["src/core/*.hpp"]),
    deps = [
        "@boost-headers//:boost-headers",
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
        "@wxwidgets//:wxwidgets",
    ]
)

cc_binary(
    name = "aevus",
    srcs = ["src/main.cpp"],
    deps = [
        ":render",
        ":ui",
        "@opencl//:opencl",
    ],
)
