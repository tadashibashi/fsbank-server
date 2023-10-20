"""Python commands to manage the Insound project

Example:
    Configuring CMake project in Debug mode:
    `python run.py config Debug`

    Build server CMake project in Debug mode:
    `python run.py build Debug`

    Build & Run the server in Release mode:
    `python run.py serve Release`


Pre-install requirements:
    - python3
    - cmake
    - ninja

"""
import os
import sys

# Name of the server project file
PROJECT_NAME: str = "insound-server"

# Path to the project root (this file should be located in root)
PROJECT_PATH: str = os.path.dirname(__file__)

# Path to the build folder
BUILD_PATH: str = os.path.join(PROJECT_PATH, "build")


def config(type: str = "Debug"):
    """
        Configure server CMake project and submodule libs
    """
    target_build_dir = os.path.join(BUILD_PATH, type.lower())
    os.system((
        f"cmake -E make_directory {target_build_dir} && "
        f"cmake -G Ninja -S {PROJECT_PATH} -B {target_build_dir} "
        f"-DCMAKE_BUILD_TYPE={type} "
        f"-DCMAKE_EXPORT_COMPILE_COMMANDS=1 "
        f"-DCROW_BUILD_TESTS=OFF -DCROW_BUILD_EXAMPLES=OFF "
    ))

    # move the current compile_commands.json to the build directory


def build(type: str = "Debug", target: str = PROJECT_NAME):
    """
        Build a Cmake project, default is the server
    """
    config(type)
    os.system(f"cmake --build {BUILD_PATH} --target {target}")


def kill_server():
    """
        Kill the server if it's currently running.
        Only available on macOS and Linux
    """
    if os.name != "nt":
        os.system(f"if pgrep {PROJECT_NAME}; then pkill {PROJECT_NAME}; fi")
    else:
        print("Windows kill_server() is unimplemented")


def serve(type: str = "Debug"):
    """
        Build & run the server
    """
    kill_server()
    build(type, PROJECT_NAME)
    os.system(f"{os.path.join(BUILD_PATH, PROJECT_NAME)}")


def test(type: str = "Debug"):
    """
        Build & run the tests
    """
    build(type, "insound-tests")
    print(f"{os.path.join(BUILD_PATH, 'tests', 'insound-tests')}")
    os.system(f"{os.path.join(BUILD_PATH, 'tests', 'insound-tests')}")


def help():
    """
        Display the help menu
    """
    print("Please provide a command:")
    print("  config     <build_type>")
    print("  build      <build_type> <target_name>")
    print("  run_server <build_type>")


def main(argc: int, argv: list[str]):
    this = sys.modules[__name__]

    # There should be a command
    if argc < 2:
        help()
        exit()

    # Get the command, and run it if it is available
    command = getattr(this, argv[1])
    if command is not None and callable(command):
        # Pass args if any
        if argc > 2:
            command(*argv[2:])
        else:
            command()
    else:
        help()


if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
