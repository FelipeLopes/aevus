load("@//conandeps:dependencies.bzl", "load_conan_dependencies")
load_conan_dependencies()

new_local_repository(
    name = "opencl",
    build_file = "opencl.BUILD",
    path = "/usr/lib/x86_64-linux-gnu",
)

new_local_repository(
    name = "wxheaders",
    build_file = "wxheaders.BUILD",
    path = "/usr/include/wx-3.0",
)

new_local_repository(
    name = "wxwidgets",
    build_file = "wxwidgets.BUILD",
    path = "/usr/lib/x86_64-linux-gnu",
)