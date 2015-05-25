#ifndef __SERIAL_RECEIVER_H__
#define __SERIAL_RECEIVER_H__

class SerialReceiver {
  private:
    static boolean hasPayload;
  public:
    static char separator;
    static String command;
    static String payload;
    static boolean isReady;
    static void reset() {
      isReady = false;
      hasPayload = false;
      command = "";
      payload = "";
    }
    static void initial(char sep=' ') {
      reset();
      separator = sep;
    }
    static boolean append(char c) {
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
    static void processSerialEvent(Stream &serial) {
      while (serial.available()) {
        char inChar = (char)serial.read();
        if (append(inChar))
          return;
      }
    }
};

char SerialReceiver::separator = ' ';
boolean SerialReceiver::isReady = false;
boolean SerialReceiver::hasPayload = false;
String SerialReceiver::command = "";
String SerialReceiver::payload = "";

#endif // #ifndef __SERIAL_RECEIVER_H__
