#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::string command = "/work/johan/SVF/SVF/llvm-16.0.0.obj/bin/llvm-link";

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg != "-lm" && arg != "-DSPEC_LINUX_AARCH64" && arg != "-fgnu89-inline" && arg != "-DSPEC_LINUX") {
            command += " " + arg + " ";
        }
    }

    // std::cout << command ;
    int ret = system(command.c_str());
    if (ret != 0) {
        std::cerr << "llvm-link failed with return code: " << ret << std::endl;
        return ret;
    }

    return 0;
}
