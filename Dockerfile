FROM python:3.8-slim-buster

RUN apt-get update && apt-get install -y curl
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

WORKDIR /usr/deploy
COPY . ./

WORKDIR /usr/deploy/arduino
RUN arduino-cli core update-index
RUN arduino-cli core install arduino:avr
RUN arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328  . 

CMD sleep infinity
