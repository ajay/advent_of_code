#!/usr/bin/env bash
set -x -e
sudo dnf -y update
sudo dnf -y install 	\
	boost 				\
	boost-devel 		\
	boost-regex 		\
	clang 				\
	clang-tools-extra 	\
	fmt 				\
	fmt-devel 			\
	gtest 				\
	gtest-devel 		\
	iwyu				\
	python3 			\
	python3-pip
