#!/usr/bin/env python
# Copyright 2024 TeiaCare
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from conans import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain
import re

def get_project_version():
    with open('VERSION', encoding='utf8') as version_file:
        version_regex = r'^\d+\.\d+\.\d+$'
        version = version_file.read().strip()
        if re.match(version_regex, version):
            return version
        else:
            raise ValueError(f"Invalid version detected into file VERSION: {version}")

class TeiaCareRabbitMQClient(ConanFile):
    name = "teiacare_sdk"
    version = get_project_version()
    license = "Apache License"
    author = "TeiaCare"
    url = "https://github.com/TeiaCare/TeiaCareRabbitMQClient"
    description = "TeiaCareRabbitMQClient is a cross-platform C++ client for RabbitMQ protocol (based on AMQP)"
    topics = ("sdk")
    exports = "VERSION"
    exports_sources = "CMakeLists.txt", "README.md", "VERSION", "cmake/*", "sdk/CMakeLists.txt", "sdk/include/*", "sdk/src/*"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "CMakeDeps"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            del self.options.fPIC

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = "ON" if self.options.shared else "OFF"
        tc.variables["TC_ENABLE_UNIT_TESTS"] = False
        tc.variables["TC_ENABLE_UNIT_TESTS_COVERAGE"] = False
        tc.variables["TC_ENABLE_BENCHMARKS"] = False
        tc.variables["TC_ENABLE_EXAMPLES"] = False
        tc.variables["TC_ENABLE_WARNINGS_ERROR"] = True
        tc.variables["TC_ENABLE_SANITIZER_ADDRESS"] = False
        tc.variables["TC_ENABLE_SANITIZER_THREAD"] = False
        tc.variables["TC_ENABLE_CLANG_FORMAT"] = False
        tc.variables["TC_ENABLE_CLANG_TIDY"] = False
        tc.variables["TC_ENABLE_CPPCHECK"] = False
        tc.variables["TC_ENABLE_CPPLINT"] = False
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy(pattern="VERSION")
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["teiacare_sdk"]
        self.cpp_info.set_property("cmake_file_name", "teiacare_sdk")
        self.cpp_info.set_property("cmake_target_name", "teiacare::sdk")
