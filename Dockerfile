FROM python:3.8-slim-buster

RUN apt-get update && apt-get install -y curl python-dev python-rpi.gpio minicom strace

RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

WORKDIR /usr/deploy
COPY . ./

WORKDIR /usr/deploy/arduino
RUN arduino-cli core update-index
RUN arduino-cli core install arduino:avr
RUN arduino-cli compile --fqbn arduino:avr:pro:cpu=8MHzatmega328  . 

RUN cd /root/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin \
    && mv avrdude avrdude-original \
    && mv /usr/deploy/autoreset autoreset \
    && mv /usr/deploy/avrdude avrdude \
    && chmod +x autoreset avrdude 

CMD arduino-cli upload --port /dev/ttyAMA0 --fqbn arduino:avr:pro:cpu=8MHzatmega328 /usr/deploy/arduino/arduino.ino &&
    

