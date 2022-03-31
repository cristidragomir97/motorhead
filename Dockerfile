FROM python:3.9-slim-bullseye 
RUN pip install smbus2
WORKDIR /usr/motorhead
COPY constants.py /usr/motorhead/constants.py
COPY lib.py /usr/motorhead/lib.py
COPY motorhead.py /usr/motorhead/motorhead.py
COPY motorhead.json /usr/motorhead/motorhead.json

CMD sleep infinity