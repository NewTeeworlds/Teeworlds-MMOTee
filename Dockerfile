FROM debian:10
#RUN sed -i 's/\(deb\|security\).debian.org/mirrors.tuna.tsinghua.edu.cn/g' /etc/apt/sources.list
RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN sed -i 's|security.debian.org/debian-security|mirrors.ustc.edu.cn/debian-security|g' /etc/apt/sources.list
RUN apt update -y && apt install vim curl mariadb-client libmysqlcppconn-dev libicu63 libicu-dev gcc g++ pkg-config libssl-dev python -y
#RUN apt install wget unzip -y 
#RUN wget https://github.com/matricks/bam/archive/refs/tags/v0.4.0.zip && unzip v0.4.0.zip 
#RUN cd bam-0.4.0 && ./make_unix.sh
#RUN cd /mmotee_cn && /bam-0.4.0/bam config 
#RUN /bam-0.4.0/bam server_sql_release && /bam-0.4.0/bam server_sql_debug
#RUN cp /bam-0.4.0/bam /mmotee_cn/