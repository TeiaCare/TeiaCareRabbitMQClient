# TeiaCareRabbitMQClient

Welcome to TeiaCareRabbitMQClient!

TeiaCareRabbitMQClient is a cross-platform C++ client for RabbitMQ protocol (based on AMQP).

## Getting Started

This project uses git submodules so it is required to clone it using the *--recursive* flag in order to retrive the required submodules.

```bash
git clone https://github.com/TeiaCare/TeiaCareRabbitMQClient.git --recursive
```

### Create Development Environment
In order to setup a development environment it is sufficient to run the script *scripts/env/setup.<bat|sh>* depending on your operating system.

```bash
# Linux/MacOS
chmod +x scripts/env/setup.sh
scripts/env/setup.sh

# Windows
scripts\env\setup.bat
```

### Start Development Environment
In order to start the development environment it is sufficient to activate the Python Virtual Environment just created the step above.

```bash
# Linux/MacOS
source .venv/bin/activate

# Windows
.venv\Scripts\activate.bat
```

### Setup Build Environment (Windows Only)

When building from command line on Windows it is necessary to activate the Visual Studio Developer Command Prompt.

Depending on the version of Visual Studio compiler and on its install location it is required to run *vcvars64.bat* script the set the development environment properly.

*Note*: using Visual Studio IDE or the CMake extension for VSCode this step is already managed in the background, so no action is required.

Examples:

```bash
# Visual Studio 2022 - Build Tools
"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

# Visual Studio 2019 - Enterprise
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
```

### Setup Dependencies
This script must be executed in order to install the 3rd party dependencies specified in the conanfile (note that 3rd party libs are only required for unit tests, examples and benchmarks).

See the section [Scripts Parameters](#scripts-parameters) for more details on the allowed scripts parameters.

```bash
python scripts/conan/setup.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION>

# examples:
python scripts/conan/setup.py Debug visual_studio 17
python scripts/conan/setup.py Release clang 15
```

### Configure, Build and Install

This script configures, builds and installs the library.
```bash
python scripts/cmake.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION>
```

### Integration Test
```bash
docker run --network host -p 15672:15672 -p 5672:5672 rabbitmq:3.13-management
```

## License

This project is licensed under the [Apache License, Version 2.0](./LICENSE).

[![License](https://img.shields.io/badge/License-Apache_v2-blue)](./LICENSE)

Copyright © 2025 [TeiaCare](https://teiacare.com/)
