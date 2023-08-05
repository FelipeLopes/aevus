cc_library(
    name = "wxwidgets",
    srcs = [
        "libwx_gtk3u_xrc-3.0.so",
        "libwx_gtk3u_html-3.0.so",
        "libwx_gtk3u_qa-3.0.so",
        "libwx_gtk3u_adv-3.0.so",
        "libwx_gtk3u_core-3.0.so",
        "libwx_baseu_xml-3.0.so",
        "libwx_baseu_net-3.0.so",
        "libwx_baseu-3.0.so",
    ],
    hdrs = glob(["wx/include/gtk3-unicode-3.0/**"]),
    includes = [
        "wx/include/gtk3-unicode-3.0",
    ],
    defines = [
        "_FILE_OFFSET_BITS=64",
        "WXUSINGDLL",
        "__WXGTK__",
    ],
    linkopts = ["-pthread"],
    deps = [
        "@wxheaders//:wxheaders",
    ],
    visibility = ["//visibility:public"],
)