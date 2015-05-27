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

Stream *SerialReceiver::serial = 0;
char SerialReceiver::separator = ' ';
char SerialReceiver::resetChar = ' ';
boolean SerialReceiver::isReady = false;
boolean SerialReceiver::hasPayload = false;
String SerialReceiver::command = "";
String SerialReceiver::payload = "";

#endif // #ifndef __SERIAL_RECEIVER_H__
