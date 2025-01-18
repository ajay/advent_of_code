#!/usr/bin/env bash
set -x -e
sudo apt -y install		\
	googletest			\
	libboost-all-dev	\
	libgtest-dev		\
	python3				\
	python3-pip

wget --directory-prefix /tmp https://apt.llvm.org/llvm.sh
chmod +x /tmp/llvm.sh
sudo /tmp/llvm.sh 19
sudo apt -y install		\
	clang				\
	clang-format-19		\
	libclang-19-dev		\
	llvm-19-dev

wget --directory-prefix /tmp https://launchpad.net/ubuntu/+archive/primary/+files/iwyu_8.23-1_amd64.deb
sudo dpkg -i /tmp/iwyu_8.23-1_amd64.deb

wget --directory-prefix /tmp https://launchpad.net/ubuntu/+source/fmtlib/10.1.1+ds1-4/+build/29236594/+files/libfmt-dev_10.1.1+ds1-4_amd64.deb
sudo dpkg -i /tmp/libfmt-dev_10.1.1+ds1-4_amd64.deb
