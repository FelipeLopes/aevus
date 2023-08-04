#include "core/flame.hpp"
#include "render/iterator.hpp"
#include "clwrap/opencl.hpp"

using std::string;

int main(int argc, char* argv[]) {
    try {
        auto context = clwrap::OpenCL::getInstance().createQueuedContext(0,1);

        auto flame = std::make_shared<core::Flame>();

        FILE* fp = fopen("../in.xml", "r");
        flame->deserialize(fp);
        fclose(fp);

        render::Iterator iterator(context, flame);

        flame->serialize(stdout);

    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}