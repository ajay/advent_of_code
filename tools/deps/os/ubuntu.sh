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
	clang-format-18		\
	libclang-18-dev		\
	llvm-18-dev

wget --directory-prefix /tmp http://launchpadlibrarian.net/727348130/iwyu_8.22-1_amd64.deb
sudo dpkg -i /tmp/iwyu_8.22-1_amd64.deb
