cc_library(
    name = "tinyxml2",
    deps = [
        "@hdrs//:tinyxml2",
        "@libs//:tinyxml2",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "boost",
    deps = [
        "@hdrs//:boost",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "opencl",
    deps = [
        "@hdrs//:opencl",
        "@libs//:opencl",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "wxwidgets",
    deps = [
        "@hdrs//:wxwidgets",
        "@libs//:wxwidgets",
    ],
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
    visibility = ["//visibility:public"],
)

genrule(
    name = "wxfb_gen",
    srcs = ["src/wxfb/aevus.fbp"],
    outs = [
        "src/wxfb/wxfb_frame.h",
        "src/wxfb/wxfb_frame.cpp",
    ],
    cmd = """sh $(location src/wxfb/gen.sh) $(location src/wxfb/aevus.fbp) \
        $(location src/wxfb/wxfb_frame.h) $(location src/wxfb/wxfb_frame.cpp)""",
    tools = ["src/wxfb/gen.sh"],
)

cc_library(
    name = "wxfb",
    srcs = ["//:wxfb_gen"],
    includes = ["src/wxfb"],
    deps = [":wxwidgets"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "aevus",
    srcs = ["src/aevus.cpp"],
    deps = [
        "//src/render:render",
        "//src/ui:ui",
    ],
)
