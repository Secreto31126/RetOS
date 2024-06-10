FROM agodio/itba-so:2.0
RUN security=insecure
WORKDIR /root

COPY . .

RUN make all -CToolchain
ENTRYPOINT [ "make" ]
