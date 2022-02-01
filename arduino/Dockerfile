FROM python:3.8-slim-buster AS arduino-builder

ARG ARDUINO_CORE
ARG ARDUINO_FQBN

RUN apt-get update && apt-get install -y curl 

RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

WORKDIR /usr/deploy
COPY . ./

WORKDIR /usr/deploy/arduino
RUN arduino-cli core update-index
RUN arduino-cli core install ${ARDUINO_CORE}
RUN arduino-cli compile --fqbn ${ARDUINO_FQBN} . --export-binaries

FROM python:3.8-slim-buster AS runtime 
RUN apt-get update && apt-get install -y python3 python3-pip avrdude
RUN pip3 install pyserial
COPY --from=arduino-builder  /usr/deploy/arduino/build/arduino.*/arduino.ino.with_bootloader.hex /flashme.hex

CMD modprobe i2c-dev && avrdude -v -p ${AVRDUDE_CPU} -c arduino -P ${SERIAL_PORT} -b 57600 -D -U flash:w:/flashme.hex \
    && python3 -m serial.tools.miniterm ${SERIAL_PORT} ${MONITOR_BAUD}
