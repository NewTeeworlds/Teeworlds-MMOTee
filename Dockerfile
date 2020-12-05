FROM debian:10
#RUN sed -i 's/\(deb\|security\).debian.org/mirrors.tuna.tsinghua.edu.cn/g' /etc/apt/sources.list
RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN sed -i 's|security.debian.org/debian-security|mirrors.ustc.edu.cn/debian-security|g' /etc/apt/sources.list
RUN apt update -y && apt install vim curl mariadb-client libmysqlcppconn-dev libicu63 libicu-dev gcc g++ pkg-config libssl-dev python -y
