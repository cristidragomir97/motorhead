FROM python:3.9-slim-bullseye 
RUN pip install smbus2
WORKDIR /usr/motorhead
COPY constants.py /usr/motorhead/constants.py
COPY lib.py /usr/motorhead/lib.py
COPY main.py /usr/motorhead/main.py

CMD sleep infinity