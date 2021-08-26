FROM ubuntu:latest
RUN sed -i 's/\(archive\|security\).ubuntu.com/mirrors.tuna.tsinghua.edu.cn/g' /etc/apt/sources.list
#RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
#RUN sed -i 's|security.debian.org/debian-security|mirrors.ustc.edu.cn/debian-security|g' /etc/apt/sources.list
RUN apt update -y && apt install vim curl mariadb-client libmysqlcppconn-dev libicu-dev gcc g++ pkg-config libssl-dev python build-essential -y
#RUN apt install wget unzip -y 
RUN git clone --depth=1 https://hub.fastgit.org/phone-burner/mmotee_cn
RUN cd mmotee_cn
RUN mkdir build && cd build
RUN cmake .. -DCMAKE_BUILD_TYPE=Debug
RUN make -j8
#RUN cd bam-0.4.0 && ./make_unix.sh
#RUN cd /mmotee_cn && /bam-0.4.0/bam config 
#RUN /bam-0.4.0/bam server_sql_release && /bam-0.4.0/bam server_sql_debug
#RUN cp /bam-0.4.0/bam /mmotee_cn/