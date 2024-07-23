
FROM gcc:latest

RUN apt-get update && \
    apt-get install -y make nlohmann-json3-dev

WORKDIR /usr/src/app

COPY . .

RUN make server

EXPOSE 8000

CMD ["./udp_server"]
