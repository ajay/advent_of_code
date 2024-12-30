#!/usr/bin/env bash
set -x -e
sudo apt -y install 	\
	googletest 			\
	libboost-all-dev 	\
	libfmt-dev 			\
	libgtest-dev 		\
	python3 			\
	python3-pip

wget --directory-prefix /tmp https://apt.llvm.org/llvm.sh
chmod +x /tmp/llvm.sh
sudo /tmp/llvm.sh 18
sudo apt -y install 	\
	clang 				\
	clang-format-18
