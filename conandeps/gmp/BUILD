
load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")


cc_import(
    name = "gmpxx_precompiled",
    static_library = "lib/libgmpxx.a",
)

cc_import(
    name = "gmp_precompiled",
    static_library = "lib/libgmp.a",
)




cc_library(
    name = "gmp",
    
    hdrs = glob(["include/**"]),
    
    
    includes = ["include"],
    
    
    
    visibility = ["//visibility:public"],
    
    deps = [
        # do not sort
    
    ":gmpxx_precompiled",
    
    ":gmp_precompiled",
    
    
    
    ],
    
)
