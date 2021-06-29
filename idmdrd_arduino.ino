#include <Mouse.h>

/* Joystick idle coordinates */
#define COORDS_CENTER 480

/* Dead zones from COORDS_CENTER */
#define DEAD_ZONE 10

/* Maximal coordinates of axis X and Y */
#define COORDS_MAX 1000

/* Lower is higher */
#define SENSIVITY 60

/* Values of axis coordinates from which joystick is being moved */
#define TRIGGER_MAX (COORDS_CENTER + DEAD_ZONE)
#define TRIGGER_MIN (COORDS_CENTER - DEAD_ZONE)

/* X axis directions */
enum x_dir {
  X_LEFT,
  X_RIGHT,
  X_NONE,
};

/* Y axis directions */
enum y_dir {
  Y_UP,
  Y_DOWN,
  Y_NONE,
};

/* For Mouse.h */
void sendData(enum y_dir y_dir, enum x_dir x_dir, int16_t x, int16_t y) {
  /* Workarounds for Mouse.h */
  if (x_dir == X_RIGHT) {
    x = -x;
  }

  if (y_dir == Y_DOWN) {
    y = -y;
  }

  /* Report button state (if needed) */
  if (digitalRead(8)) {
    if (!Mouse.isPressed())
      Mouse.press();
  } else {
    if (Mouse.isPressed())
      Mouse.release();
  }

  /* Report joystick moves (if needed) */
  if (x_dir != X_NONE || y_dir != Y_NONE)
    Mouse.move(x, y, 0);
}

void setup() {
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(8, INPUT);
  Mouse.begin();
}

void loop() {
  int y = analogRead(A1), final_y = 0;
  int x = analogRead(A2), final_x = 0;
  enum x_dir x_dir = X_NONE;
  enum y_dir y_dir = Y_NONE;

  /* Get joystick coordinates and directions */
  if (x > TRIGGER_MAX) {
    x_dir = X_LEFT;
    final_x = x - TRIGGER_MAX;
  } else if (x < TRIGGER_MIN) {
    x_dir = X_RIGHT;
    final_x = TRIGGER_MIN - x;
  }

  if (y > TRIGGER_MAX) {
    y_dir = Y_DOWN;
    final_y = y - TRIGGER_MAX;
  } else if (y < TRIGGER_MIN) {
    y_dir = Y_UP;
    final_y = TRIGGER_MIN - y;
  }

  /* Tell everything to Mouse.h */
  sendData(y_dir, x_dir, final_x / SENSIVITY, final_y / SENSIVITY);

  /* For stability */
  delay(10);
}
