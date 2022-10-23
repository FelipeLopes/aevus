
load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")


cc_import(
    name = "boost_log_setup_precompiled",
    static_library = "lib/libboost_log_setup.a",
)

cc_import(
    name = "boost_log_precompiled",
    static_library = "lib/libboost_log.a",
)

cc_import(
    name = "boost_locale_precompiled",
    static_library = "lib/libboost_locale.a",
)

cc_import(
    name = "boost_iostreams_precompiled",
    static_library = "lib/libboost_iostreams.a",
)

cc_import(
    name = "boost_graph_precompiled",
    static_library = "lib/libboost_graph.a",
)

cc_import(
    name = "boost_fiber_numa_precompiled",
    static_library = "lib/libboost_fiber_numa.a",
)

cc_import(
    name = "boost_fiber_precompiled",
    static_library = "lib/libboost_fiber.a",
)

cc_import(
    name = "boost_contract_precompiled",
    static_library = "lib/libboost_contract.a",
)

cc_import(
    name = "boost_wave_precompiled",
    static_library = "lib/libboost_wave.a",
)

cc_import(
    name = "boost_unit_test_framework_precompiled",
    static_library = "lib/libboost_unit_test_framework.a",
)

cc_import(
    name = "boost_type_erasure_precompiled",
    static_library = "lib/libboost_type_erasure.a",
)

cc_import(
    name = "boost_timer_precompiled",
    static_library = "lib/libboost_timer.a",
)

cc_import(
    name = "boost_thread_precompiled",
    static_library = "lib/libboost_thread.a",
)

cc_import(
    name = "boost_random_precompiled",
    static_library = "lib/libboost_random.a",
)

cc_import(
    name = "boost_prg_exec_monitor_precompiled",
    static_library = "lib/libboost_prg_exec_monitor.a",
)

cc_import(
    name = "boost_nowide_precompiled",
    static_library = "lib/libboost_nowide.a",
)

cc_import(
    name = "boost_json_precompiled",
    static_library = "lib/libboost_json.a",
)

cc_import(
    name = "boost_filesystem_precompiled",
    static_library = "lib/libboost_filesystem.a",
)

cc_import(
    name = "boost_coroutine_precompiled",
    static_library = "lib/libboost_coroutine.a",
)

cc_import(
    name = "boost_chrono_precompiled",
    static_library = "lib/libboost_chrono.a",
)

cc_import(
    name = "boost_wserialization_precompiled",
    static_library = "lib/libboost_wserialization.a",
)

cc_import(
    name = "boost_test_exec_monitor_precompiled",
    static_library = "lib/libboost_test_exec_monitor.a",
)

cc_import(
    name = "boost_stacktrace_noop_precompiled",
    static_library = "lib/libboost_stacktrace_noop.a",
)

cc_import(
    name = "boost_stacktrace_basic_precompiled",
    static_library = "lib/libboost_stacktrace_basic.a",
)

cc_import(
    name = "boost_stacktrace_backtrace_precompiled",
    static_library = "lib/libboost_stacktrace_backtrace.a",
)

cc_import(
    name = "boost_stacktrace_addr2line_precompiled",
    static_library = "lib/libboost_stacktrace_addr2line.a",
)

cc_import(
    name = "boost_serialization_precompiled",
    static_library = "lib/libboost_serialization.a",
)

cc_import(
    name = "boost_regex_precompiled",
    static_library = "lib/libboost_regex.a",
)

cc_import(
    name = "boost_program_options_precompiled",
    static_library = "lib/libboost_program_options.a",
)

cc_import(
    name = "boost_math_tr1l_precompiled",
    static_library = "lib/libboost_math_tr1l.a",
)

cc_import(
    name = "boost_math_tr1f_precompiled",
    static_library = "lib/libboost_math_tr1f.a",
)

cc_import(
    name = "boost_math_tr1_precompiled",
    static_library = "lib/libboost_math_tr1.a",
)

cc_import(
    name = "boost_math_c99l_precompiled",
    static_library = "lib/libboost_math_c99l.a",
)

cc_import(
    name = "boost_math_c99f_precompiled",
    static_library = "lib/libboost_math_c99f.a",
)

cc_import(
    name = "boost_math_c99_precompiled",
    static_library = "lib/libboost_math_c99.a",
)

cc_import(
    name = "boost_exception_precompiled",
    static_library = "lib/libboost_exception.a",
)

cc_import(
    name = "boost_date_time_precompiled",
    static_library = "lib/libboost_date_time.a",
)

cc_import(
    name = "boost_context_precompiled",
    static_library = "lib/libboost_context.a",
)

cc_import(
    name = "boost_container_precompiled",
    static_library = "lib/libboost_container.a",
)

cc_import(
    name = "boost_atomic_precompiled",
    static_library = "lib/libboost_atomic.a",
)




cc_library(
    name = "boost",
    
    hdrs = glob(["include/**"]),
    
    
    includes = ["include"],
    
    
    defines = ["BOOST_STACKTRACE_USE_NOOP", "BOOST_STACKTRACE_USE_BACKTRACE", "BOOST_STACKTRACE_ADDR2LINE_LOCATION=\\\"/usr/bin/addr2line\\\"", "BOOST_STACKTRACE_USE_ADDR2LINE"],
    
    
    linkopts = ["-ldl", "-lrt", "-lpthread"],
    
    visibility = ["//visibility:public"],
    
    deps = [
        # do not sort
    
    ":boost_log_setup_precompiled",
    
    ":boost_log_precompiled",
    
    ":boost_locale_precompiled",
    
    ":boost_iostreams_precompiled",
    
    ":boost_graph_precompiled",
    
    ":boost_fiber_numa_precompiled",
    
    ":boost_fiber_precompiled",
    
    ":boost_contract_precompiled",
    
    ":boost_wave_precompiled",
    
    ":boost_unit_test_framework_precompiled",
    
    ":boost_type_erasure_precompiled",
    
    ":boost_timer_precompiled",
    
    ":boost_thread_precompiled",
    
    ":boost_random_precompiled",
    
    ":boost_prg_exec_monitor_precompiled",
    
    ":boost_nowide_precompiled",
    
    ":boost_json_precompiled",
    
    ":boost_filesystem_precompiled",
    
    ":boost_coroutine_precompiled",
    
    ":boost_chrono_precompiled",
    
    ":boost_wserialization_precompiled",
    
    ":boost_test_exec_monitor_precompiled",
    
    ":boost_stacktrace_noop_precompiled",
    
    ":boost_stacktrace_basic_precompiled",
    
    ":boost_stacktrace_backtrace_precompiled",
    
    ":boost_stacktrace_addr2line_precompiled",
    
    ":boost_serialization_precompiled",
    
    ":boost_regex_precompiled",
    
    ":boost_program_options_precompiled",
    
    ":boost_math_tr1l_precompiled",
    
    ":boost_math_tr1f_precompiled",
    
    ":boost_math_tr1_precompiled",
    
    ":boost_math_c99l_precompiled",
    
    ":boost_math_c99f_precompiled",
    
    ":boost_math_c99_precompiled",
    
    ":boost_exception_precompiled",
    
    ":boost_date_time_precompiled",
    
    ":boost_context_precompiled",
    
    ":boost_container_precompiled",
    
    ":boost_atomic_precompiled",
    
    
    
    "@zlib",
    
    "@bzip2",
    
    "@libbacktrace",
    
    ],
    
)
