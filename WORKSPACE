load("@//conandeps:dependencies.bzl", "load_conan_dependencies")
load_conan_dependencies()

new_local_repository(
    name = "opencl",
    build_file = "opencl.BUILD",
    path = "/usr/lib/x86_64-linux-gnu",
)
