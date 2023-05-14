FROM ubuntu:22.10
RUN apt update -y && apt upgrade -y
RUN apt install gcc valgrind make time -y
COPY . /

CMD make local