FROM cristidragomir97/arduino_core AS arduino-builder
ARG ARDUINO_FQBN

WORKDIR /usr/sketch
COPY sketch.ino /usr/sketch/sketch.ino
RUN arduino-cli compile --fqbn ${ARDUINO_FQBN} sketch.ino --export-binaries

FROM python:3.9-slim-bullseye AS runtime
RUN apt-get update && apt-get install -y avrdude 
RUN pip install pyserial
COPY --from=arduino-builder  /usr/sketch/build/arduino*/sketch.ino.with_bootloader.hex /flashme.hex

CMD avrdude -v -p ${AVRDUDE_CPU} -c arduino -P ${SERIAL_PORT} -b 57600 -D -U flash:w:/flashme.hex \
    && python3 -m serial.tools.miniterm ${SERIAL_PORT} ${MONITOR_BAUD}
