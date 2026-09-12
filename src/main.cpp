// Faces3 Keyboard (Faces Keyboard3, A003-V3) I2C read example.
// Ported from M5Stack/examples/Face/KEYBOARD/KEYBOARD.ino to the M5Faces
// library and M5Unified.
// https://github.com/m5stack/M5Stack/tree/master/examples/Face/KEYBOARD
// https://github.com/m5stack/M5Faces

#include <M5Faces.h>
#include <M5Unified.h>

M5Faces_Keyboard3 keyboard;

// Typed text is redrawn as a whole each time it changes, so Backspace/Delete
// actually erase the last character instead of leaving stray control bytes
// on screen, and a blinking block cursor always marks the insertion point.
static String textBuffer;
static int16_t textAreaX = 0;
static int16_t textAreaY = 0;
static bool cursorOn          = true;
static uint32_t lastBlinkMs   = 0;
static constexpr uint32_t kCursorBlinkIntervalMs = 500;
static constexpr size_t kMaxBufferLen            = 1024;

static void redrawTextArea() {
  M5.Display.fillRect(textAreaX, textAreaY, M5.Display.width() - textAreaX,
                       M5.Display.height() - textAreaY, TFT_BLACK);
  M5.Display.setCursor(textAreaX, textAreaY);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.print(textBuffer);
  M5.Display.setTextColor(cursorOn ? TFT_BLACK : TFT_WHITE, cursorOn ? TFT_WHITE : TFT_BLACK);
  M5.Display.print(' ');
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setTextFont(2);
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.println("Faces3 Keyboard I2C Read Example");

  if (keyboard.begin(&M5.In_I2C) != M5FACES_OK) {
    M5.Display.println("Keyboard3 not found");
    Serial.println("Keyboard3 not found");
  }

  textAreaX = 0;
  textAreaY = M5.Display.getCursorY();
  redrawTextArea();
}

void loop() {
  M5.update();

  bool needsRedraw = false;

  if (keyboard.update() && keyboard.isPressed()) {
    uint8_t raw = keyboard.getKey();

    if (raw == KEYBOARD3_KEY_BS || raw == KEYBOARD3_KEY_DEL) {
      if (!textBuffer.isEmpty()) {
        textBuffer.remove(textBuffer.length() - 1);
      }
      needsRedraw = true;
    } else if (raw == KEYBOARD3_KEY_ENTER) {
      if (textBuffer.length() < kMaxBufferLen) {
        textBuffer += '\n';
      }
      needsRedraw = true;
    } else if (keyboard.isPrintable()) {
      char c = keyboard.getChar();
      if (textBuffer.length() < kMaxBufferLen) {
        textBuffer += c;
      }
      needsRedraw = true;
    } else {
      // Non-text keys (Fn/Alt/Sym/arrows/...) are only logged to Serial for
      // debugging and do not affect the typed text.
      const char* name = M5Faces_Keyboard3::keyboard3_code_parse(raw);
      if (name != nullptr) {
        Serial.printf("[%s] ", name);
      } else {
        Serial.printf("0x%02X ", raw);
      }
    }

    // Keep the cursor solid right after any keypress so it doesn't
    // disappear mid-blink while the user is actively typing.
    cursorOn    = true;
    lastBlinkMs = millis();
  }

  uint32_t now = millis();
  if (now - lastBlinkMs >= kCursorBlinkIntervalMs) {
    lastBlinkMs = now;
    cursorOn    = !cursorOn;
    needsRedraw = true;
  }

  if (needsRedraw) {
    redrawTextArea();
  }
}
