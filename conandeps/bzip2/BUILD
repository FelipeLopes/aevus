
load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")


cc_import(
    name = "bz2_precompiled",
    static_library = "lib/libbz2.a",
)




cc_library(
    name = "bzip2",
    
    hdrs = glob(["include/**"]),
    
    
    includes = ["include"],
    
    
    
    visibility = ["//visibility:public"],
    
    deps = [
        # do not sort
    
    ":bz2_precompiled",
    
    
    
    ],
    
)
