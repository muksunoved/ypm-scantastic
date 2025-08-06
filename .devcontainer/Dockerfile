# Base image
# Date: 2025-01-27
FROM ubuntu@sha256:3afff29dffbc200d202546dc6c4f614edc3b109691e7ab4aa23d02b42ba86790

# Update the package database and install required tools
RUN apt-get update --fix-missing
RUN apt-get install -y python3.12 python3-pip pipx git less vim sudo cmake make g++-14 clangd-19 libgtest-dev

# Update clangd and g++ versions to the latest ones
RUN ln -s  /usr/bin/clangd-19 /usr/bin/clangd
RUN ln -fs /usr/bin/g++-14 /usr/bin/g++

RUN git config --global core.editor code


RUN  echo "ubuntu ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers.d/ubuntu \
    && chmod 0440 /etc/sudoers.d/ubuntu

RUN cd /usr/src/googletest \
    && sudo cmake -B build \
    && sudo cmake --build build --target install

# Set the default command
CMD ["/bin/bash"]
