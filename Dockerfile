#待完成
FROM ubuntu:focal
LABEL authors="xuan"

# 设置工作目录
WORKDIR /app

# 避免安装过程中出现交互式提示
ARG DEBIAN_FRONTEND=noninteractive
RUN sed -i 's@//.*archive.ubuntu.com@//mirrors.ustc.edu.cn@g' /etc/apt/sources.list && \
    apt update && apt install software-properties-common vim  -y && add-apt-repository ppa:ubuntu-toolchain-r/test &&  \
    apt install gcc-11 g++-11 libleveldb-dev libsnappy-dev  -y


ENTRYPOINT ["top", "-b"]