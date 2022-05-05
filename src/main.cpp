#include <Arduino.h>
#include <Arduino_LSM9DS1.h>

enum Target {
  TARGET_IDLE,
  TARGET_UP_AND_DOWN,
  TARGET_LEFT_AND_RIGHT,
  TARGET_ROTATE
};

#define NUM_SAMPLES       600
#define GESTURE_TARGET    TARGET_ROTATE

typedef struct {
  float acc_x;
  float acc_y;
  float acc_z;
  float gyro_x;
  float gyro_y;
  float gyro_z;
} vector_t;

vector_t data[NUM_SAMPLES];

int i = 0;
int gesture_count = 0;

// Function prototypes
void save_acc_data(float x, float y, float z);
void save_gyro_data(float x, float y, float z);
void print_csv_header();
void print_data_csv();

void setup() {
  // put your setup code here, to run once:

  // Initialize Serial
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Started");

  // Initialize IMU
  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

  print_csv_header();
}

void loop() {
  // put your main code here, to run repeatedly:

  // IMU variables
  float x, y, z;

  // Read IMU
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    // Read accelerometer
    IMU.readAcceleration(x, y, z);

    // Save accelerometer data
    save_acc_data(x, y, z);

    // Read gyroscope
    IMU.readGyroscope(x, y, z);

    // Save gyroscope data
    save_gyro_data(x, y, z);

    i++;
    if(i == NUM_SAMPLES) {
      print_data_csv();
      i = 0;
    }
  }
}

void save_acc_data(float x, float y, float z) {
  data[i].acc_x = x;
  data[i].acc_y = y;
  data[i].acc_z = z;
}

void save_gyro_data(float x, float y, float z) {
  data[i].gyro_x = x;
  data[i].gyro_y = y;
  data[i].gyro_z = z;
}

void print_csv_header() {
  // print the CSV header (ax0,ay0,az0,...,gx49,gy49,gz49,target)
  for (int i = 0; i < NUM_SAMPLES; i++)
  {
    Serial.print("aX");
    Serial.print(i);
    Serial.print(",aY");
    Serial.print(i);
    Serial.print(",aZ");
    Serial.print(i);
    Serial.print(",gX");
    Serial.print(i);
    Serial.print(",gY");
    Serial.print(i);
    Serial.print(",gZ");
    Serial.print(i);
    Serial.print(",");
  }
  Serial.println("target");
}

void print_data_csv() {
  for (int i = 0; i < NUM_SAMPLES; i++) {
    Serial.print(data[i].acc_x, 3);
    Serial.print(',');
    Serial.print(data[i].acc_y, 3);
    Serial.print(',');
    Serial.print(data[i].acc_z, 3);
    Serial.print(',');
    Serial.print(data[i].gyro_x, 3);
    Serial.print(',');
    Serial.print(data[i].gyro_y, 3);
    Serial.print(',');
    Serial.print(data[i].gyro_z, 3);
    Serial.print(',');
  }

  // print target at the end of samples acquisition
  Serial.println(GESTURE_TARGET);
}