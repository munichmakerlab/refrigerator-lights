#ifndef __SERIAL_RECEIVER_H__
#define __SERIAL_RECEIVER_H__

class SerialReceiver {
  private:
    static boolean hasPayload;
    static Stream *serial;
  public:
    static char separator;
    static char resetChar;
    static String command;
    static String payload;
    static boolean isReady;
    static void reset() {
      isReady = false;
      hasPayload = false;
      command = "";
      payload = "";
      // reserve 200 bytes for the inputString:
      command.reserve(200);
      payload.reserve(200);
    }
    static void initial(Stream &serialPort, char sep=' ', char res=' ') {
      reset();
      serial = &serialPort;
      separator = sep;
      resetChar = res;
    }
    static boolean append(char c) {
      if (c == resetChar && resetChar != ' ') {
        reset();
        return false;
      }
      if (c == '\n') {
        isReady = true;
        return true;
      }
      if (c == separator && !hasPayload) {
        hasPayload = true;
        return false;
      }
      if (hasPayload)
        payload += c;
      else
        command += c;
      return false;
    }
    static void processSerialEvent() {
      while (serial->available()) {
        char inChar = (char)serial->read();
        if (append(inChar))
          return;
      }
    }
};

#endif // #ifndef __SERIAL_RECEIVER_H__
