FROM python:3.8-slim-buster 
RUN apt-get update && apt-get install -y python3-pip
RUN pip3 install smbus2

CMD python3 motorhead.py