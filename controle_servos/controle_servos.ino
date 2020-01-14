byte PWM_IN_1 = 3;
byte PWM_IN_2 = 4;
byte PWM_IN_3 = 5;

byte PWM_OUT_1 = 9;
byte PWM_OUT_2 = 10;

byte LASER = 11;

const int MINPWM = 977;
const int MAXPWM = 1983;

const int MINSERVO = 10;
const int MAXSERVO = 170;

int valor_lido_pwm;
int valor_escalaServo;
 
void setup() {
  pinMode(PWM_IN_1, INPUT);
  pinMode(PWM_IN_2, INPUT);
  pinMode(PWM_OUT_1, OUTPUT);
  pinMode(PWM_OUT_2, OUTPUT);
  pinMode(LASER, OUTPUT);
  Serial.begin(115200);
}

int lePwm(int pino) {
  valor_lido_pwm = pulseIn(pino, HIGH);
  valor_escalaServo = map(valor_lido_pwm, MINPWM, MAXPWM, MINSERVO, MAXSERVO);
  return valor_escalaServo;
}

boolean lePwmBinario(int pino) {
  valor_lido_pwm = pulseIn(pino, HIGH);
  int media = (MINPWM + MAXPWM);
  media = media / 2;
  return valor_lido_pwm > media;
}

void loop() {
  int valor1 = lePwm(PWM_IN_1);
  analogWrite(PWM_OUT_1, valor1);
  int valor2 = lePwm(PWM_IN_2);
  analogWrite(PWM_OUT_2, valor2);
  boolean valor3 = lePwmBinario(PWM_IN_3);
  digitalWrite(LASER, valor3);
  debug(valor1, valor2, valor3);
  delay(100); 
}

void debug(int valor1, int valor2, int valor3) {
  Serial.print("Valor 1: ");
  Serial.print(valor1);
  Serial.print("; Valor 2: ");
  Serial.print(valor2);
  Serial.print("; Valor 3: ");
  Serial.println(valor3);  
}
