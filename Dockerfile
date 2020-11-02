ARG IMG_VERSION=20200516

FROM krenol/cpp_raspbian:${IMG_VERSION}

ARG PROJECT_DIR=rpisocket
ARG BUILD_TEST
ARG SOCKET_PORT
ENV BUILD_TEST "$BUILD_TEST"
ENV SOCKET_PORT "$SOCKET_PORT"

WORKDIR /data
RUN apt-get update && apt-get upgrade -y && apt-get install -y libboost-all-dev
# copy files
COPY ./${PROJECT_DIR} ./${PROJECT_DIR}

#prepare build
RUN mkdir build && cd build && cmake ../${PROJECT_DIR} 

#build
RUN cmake --build ./build

COPY ./docker-entrypoint.sh /
ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["start"]