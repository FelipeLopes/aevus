
load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")


cc_import(
    name = "backtrace_precompiled",
    static_library = "lib/libbacktrace.a",
)




cc_library(
    name = "libbacktrace",
    
    hdrs = glob(["include/**"]),
    
    
    includes = ["include"],
    
    
    
    visibility = ["//visibility:public"],
    
    deps = [
        # do not sort
    
    ":backtrace_precompiled",
    
    
    
    ],
    
)
