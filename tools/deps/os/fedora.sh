#!/usr/bin/env bash
set -x -e
sudo dnf update
sudo dnf install clang clang-tools-extra boost boost-devel boost-regex fmt fmt-devel gtest gtest-devel python3 python3-pip
