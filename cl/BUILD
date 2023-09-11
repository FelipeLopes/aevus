filegroup(
    name = "cl_files",
    srcs = glob(["*.cl"]),
)

genrule(
    name = "cl_embed",
    srcs = [":cl_files"],
    outs = ["cl.o"],
    cmd = "ld -r -b binary -o $@ $(locations :cl_files)",
)

cc_library(
    name = "cl",
    srcs = [":cl_embed"],
    visibility = ["//visibility:public"],
)