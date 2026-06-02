from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy
import os


class CcstringConan(ConanFile):
    name = "ccstring"
    version = "1.0.0"
    package_type = "library"
    license = "MIT"
    author = "kronus-lx"
    url = "https://github.com/kronus-lx/ccstring"
    description = "Easy string allocation and deallocation for C programmers"
    topics = ("c", "string", "memory", "library")
    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_examples": [True, False],
    }
    default_options = {
        "shared": True,
        "fPIC": True,
        "build_examples": False,
    }
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "examples/*", "LICENSE"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CCSTRING_BUILD_EXAMPLES"] = self.options.build_examples
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        copy(self, "LICENSE", self.source_folder, os.path.join(self.package_folder, "licenses"), keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["ccstring"]
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.set_property("cmake_file_name", "ccstring")
        self.cpp_info.set_property("cmake_target_name", "ccstring::ccstring")
        self.cpp_info.set_property("cmake_target_aliases", ["ccstring"])
