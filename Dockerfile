FROM agodio/itba-so:1.0
RUN security=insecure
WORKDIR /root

COPY . .

RUN make all -CToolchain
CMD [ "make", "all" ]
