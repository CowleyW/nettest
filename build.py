import argparse
import os
from subprocess import call

def configure():
    call(["cmake", "-G", "MinGW Makefiles", "-S", ".", "-B", "out/"])
    call(["cp", "out/compile_commands.json", "compile_commands.json"])


def build_main(proj, run):
    call(["cmake", "-G", "MinGW Makefiles", "-S", ".", "-B", "out/"])

    os.chdir("out")
    call(["make", proj])

    if (run):
        call([f"{proj}.exe"])

    os.chdir("..")


# Build script main
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="build")
    parser.add_argument("--configure", dest="configure", action="store_true", 
                        help="only configure CMake")
    parser.add_argument("--run", dest="run", action="store_true",
                        help="run the executable")

    args = parser.parse_args()

    if args.configure:
        configure()
    else:
        build_main("client", args.run)
        build_main("server", args.run)

