byte PWM_PIN = 3;

const int MINPWM = 977;
const int MAXPWM = 1983;

int valor_lido_pwm;
int valor_escala255;
 
void setup() {
  pinMode(PWM_PIN, INPUT);
  Serial.begin(115200);
}
 
void loop() {
  valor_lido_pwm = pulseIn(PWM_PIN, HIGH);
  valor_escala255 = map(valor_lido_pwm, MINPWM, MAXPWM, 0, 255);
  Serial.print("Agora: ");
  Serial.println(valor_escala255);
}
