/*
  Direção Velocidade - Converte sinais pwm de rádio controle em velocidade de motores.

  Este código foi criado para controlar um robô com dois motores independentes, um em cada lado.

  Para o controle dos motores, assume-se a existência de um módulo driver L298N, uma ponte H.

  O código faz parte do projeto Robô Pega Bolas, de Otávio Carneiro.

  https://github.com/ocarneiro/robo-pega-bolas

  twitter: @ooocarneiro
  
*/

// Pinos utilizados
byte PWM_IN_ACELERADOR = 2;
byte PWM_IN_DIRECAO = 3;
byte PWM_IN_REVERSO = 4;

byte PWM_OUT_MOTOR_A_PINO1 = 5;
byte PWM_OUT_MOTOR_A_PINO2 = 6;

byte PWM_OUT_MOTOR_B_PINO3 = 9;
byte PWM_OUT_MOTOR_B_PINO4 = 10;

const int MINPWM = 977;
const int MAXPWM = 1983;

// ponto médio do sinal = 
//       média dos valores possíveis
const int MEIOPWM = (MINPWM + MAXPWM) / 2;

const int MINSAIDA = 0;
const int MAXSAIDA = 255;
const int MEIOSAIDA = (MINSAIDA + MAXSAIDA) / 2;
const int LIMIARSAIDA = 10;

const boolean MODO_DEBUG = false;

void setup() {
  
  pinMode(PWM_IN_ACELERADOR, INPUT);
  pinMode(PWM_IN_DIRECAO, INPUT);
  pinMode(PWM_IN_REVERSO, INPUT);
  
  pinMode(PWM_OUT_MOTOR_A_PINO1, OUTPUT);
  pinMode(PWM_OUT_MOTOR_A_PINO2, OUTPUT);

  pinMode(PWM_OUT_MOTOR_B_PINO3, OUTPUT);
  pinMode(PWM_OUT_MOTOR_B_PINO4, OUTPUT);

  if (MODO_DEBUG) {
      Serial.begin(115200);
  }
}

/*
 * Converte valor lido para verdadeiro ou falso
 *  - verdadeiro = maior do que a média
 */
boolean deRadioParaBinario(int pino) {
  int valor_lido_pwm = pulseIn(pino, HIGH);
  return valor_lido_pwm > MEIOPWM;
}

/*
 * Devolve valor na escala de saída.
 * Ex: lê 1500 na escala de 1000 a 2000 e retorna 128 na escala de 0 a 255.
 */
int normalizaPwm(int pwmEntrada) {
  int pwmSaida = map(pwmEntrada, MINPWM, MAXPWM, MINSAIDA, MAXSAIDA);
  return pwmSaida;
}

/*
 * Retorna valor negativo para valores à esquerda
 *    e positivo para valores à direita
 * Ex: em uma escala de 0 a 255,
 *     0 vira -128;
 *     255 vira +128;
 *     128 vira 0;
 */
int deRadioParaDirecao(int pino) {
  int valor_lido_pwm = pulseIn(pino, HIGH);
  int valor_saida = normalizaPwm(valor_lido_pwm);
  return valor_saida - MEIOSAIDA;
}

/*
 * Retorna valor a ser enviado para motores
 *  a partir de valor lido no pino informado.
 */
int deRadioParaMotor(int pino) {
  int valor_lido_pwm = pulseIn(pino, HIGH);
  return normalizaPwm(valor_lido_pwm);
}

/*
 * Indica se direção está dentro dos limiares
 * considerados neutros (em frente).
 */
boolean direcaoNeutra(int direcao) {
  int menorDirecaoNeutra = 0 - LIMIARSAIDA;
  int maiorDirecaoNeutra = LIMIARSAIDA;
  return (direcao > menorDirecaoNeutra && direcao < maiorDirecaoNeutra);
}

void loop() {

  int velocidade_motor_esquerdo = 0;
  int velocidade_motor_direito = 0;

  int aceleracao = deRadioParaMotor(PWM_IN_ACELERADOR);

  int direcao = deRadioParaDirecao(PWM_IN_DIRECAO);

  // se a direção é próxima de zero,
  // os dois motores giram na mesma direcao
  if (direcaoNeutra(direcao)) {
    velocidade_motor_esquerdo = aceleracao;
    velocidade_motor_direito = aceleracao;
  }

  boolean reverso_ativo = deRadioParaBinario(PWM_IN_REVERSO);

  // Tratamento da ré (reverso)
  // - envia para o L298N sinal ou gnd conforme o caso.
  // se a chave estiver ligada (ré):  gnd, sinal, gnd, sinal
  // se desligada (frente): sinal, gnd, sinal, gnd
  if (reverso_ativo) {
      digitalWrite(PWM_OUT_MOTOR_A_PINO1, LOW);
      analogWrite(PWM_OUT_MOTOR_A_PINO2, velocidade_motor_esquerdo);
      digitalWrite(PWM_OUT_MOTOR_B_PINO3, LOW);
      analogWrite(PWM_OUT_MOTOR_B_PINO4, velocidade_motor_direito);      
  } else {
      analogWrite(PWM_OUT_MOTOR_A_PINO1, velocidade_motor_esquerdo);
      digitalWrite(PWM_OUT_MOTOR_A_PINO2, LOW);
      analogWrite(PWM_OUT_MOTOR_B_PINO3, velocidade_motor_direito);
      digitalWrite(PWM_OUT_MOTOR_B_PINO4, LOW);
  }
  if(MODO_DEBUG) {
    debug(reverso_ativo, 0, 1);
  }
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
