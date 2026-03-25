#include "Core/EntryPoint.h"

int main(int argc, char* argv[]) {
    using namespace Rhygine;

    int exitCode = EntryPoint::Run(argc, argv);

    return exitCode;
}
