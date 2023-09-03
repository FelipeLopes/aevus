filegroup(
    name = "image_files",
    srcs = glob(["*.png"]),
)

genrule(
    name = "image_embed",
    srcs = [":image_files"],
    outs = ["images.o"],
    cmd = "ld -r -b binary -o $@ $(locations :image_files)",
)

cc_library(
    name = "images",
    srcs = [":image_embed"],
    visibility = ["//visibility:public"],
)