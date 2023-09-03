genrule(
    name = "wxfb_gen",
    srcs = ["aevus.fbp"],
    outs = [
        "wxfb_frame.h",
        "wxfb_frame.cpp",
    ],
    cmd = "sh $(location gen.sh) $(location aevus.fbp) $(location wxfb_frame.h) $(location wxfb_frame.cpp)",
    tools = ["gen.sh"],
)

cc_library(
    name = "wxfb",
    srcs = [":wxfb_gen"],
    includes = ["."],
    deps = ["//:wxwidgets"],
    visibility = ["//visibility:public"],
)