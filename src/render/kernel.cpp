#include "kernel.hpp"
#include <boost/assign/list_of.hpp>

using clwrap::CLQueuedContext;

namespace render {

Kernel::Kernel(CLQueuedContext& context, std::string name):
    CLExecutable(context, name, lookup.find(name)->second.first,
        lookup.find(name)->second.second) { }

const std::map<std::string, std::pair<char*, char*>> Kernel::lookup =
    boost::assign::map_list_of
        ("iterate", std::make_pair(_binary_cl_iterate_cl_start, _binary_cl_iterate_cl_end))
        ("mapping", std::make_pair(_binary_cl_mapping_cl_start, _binary_cl_mapping_cl_end));

}