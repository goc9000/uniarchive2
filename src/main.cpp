#include "temp_test_harness.h"

using namespace uniarchive2;

int main(int argc, char* argv[]) {
    run_test_harness(argc > 1 ? argv[1] : "test_config.json");

    return 0;
}
