FROM gcc
RUN mkdir -p /app/
COPY serverM.c /app/
COPY serverM.h /app/
COPY socketHelper.c /app/
COPY socketHelper.h /app/
WORKDIR /app/
RUN gcc socketHelper.c serverM.c -o serverM
CMD ./serverM