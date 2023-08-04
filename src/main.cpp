#include "core/flame.hpp"
#include "render/iterator.hpp"
#include "render/opencl.hpp"

using std::string;

int main(int argc, char* argv[]) {
    try {
        auto context = render::OpenCL::getInstance().createQueuedContext(0,1);

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