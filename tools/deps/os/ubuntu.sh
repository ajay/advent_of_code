#!/usr/bin/env bash
sudo apt install libboost-all-dev libfmt-dev googletest libgtest-dev python3 python3-pip
wget --directory-prefix /tmp https://apt.llvm.org/llvm.sh
chmod +x /tmp/llvm.sh
sudo /tmp/llvm.sh 17
sudo apt install clang clang-format-17
