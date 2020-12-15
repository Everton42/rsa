#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define VERDADEIRO 1;
#define FALSO 0;

int verificaPrimo(long long numero)
{
    int i;
    double raizQuadrada;

    raizQuadrada = sqrt(numero);

    for (i = 2; i <= raizQuadrada; i++)
    {
        if (numero % i == 0)
            return FALSO;
    }
    return VERDADEIRO;
}

int saoNumerosIguais(int primeiroNumero, int segundoNumero)
{
    return primeiroNumero == segundoNumero;
}

int ehDiferenteDoPrimeiroESegundoNumero(long long valor, long long primeiroNumero, long long segundoNumero)
{
    return valor != primeiroNumero && valor != segundoNumero;
}

long obterNumeroPrimo(int posicao)
{
    int ehPrimo;
    long long numero;
    do
    {
        printf("\nDigite %dº número primo:\n", posicao);
        scanf("%llu", &numero);

        ehPrimo = verificaPrimo(numero);

        if (!ehPrimo)
            printf("Você não digitou um número primo...\n");

    } while (!ehPrimo);
    return numero;
}

long long escolheE(long long phi, long long primeiroNumero, long long segundoNumero, long long n)
{
    long long i, e;
    for (i = 2; i < phi; i++)
    {
        if (phi % i != 0 && verificaPrimo(i) && ehDiferenteDoPrimeiroESegundoNumero(i, primeiroNumero, segundoNumero))
        {
            e = i;
            break;
        }
    }

    return e;
}

void divisao(long long *resto, long long *quociente, long long a, long long b)
{
    if (a >= 0)
    {
        *quociente = 0;
        *resto = a;

        while (*resto >= b)
        {
            *resto -= b;
            *quociente = *quociente + 1;
        }
    }
}

long long mdcEstendido(long long a, long long b)
{
    long long resto, quociente, xB = 1, yB = 0, x = 0, y = 1, alpha, beta, phi;
    phi = a;

    resto = a;
    while (resto != 0)
    {
        divisao(&resto, &quociente, a, b);
        a = b;
        b = resto;
        if (resto > 0)
        {
            alpha = xB - quociente * x;
            beta = yB - quociente * y;

            xB = x;
            yB = y;
            x = alpha;
            y = beta;
        }
    }

    if (beta < 0)
        beta = phi + beta;

    return beta;
}

long potencia(long long a, long long chavePrivada, long long n)
{
    long long A = a, P = 1, E = chavePrivada;

    while (1)
    {
        if (E == 0)
            return P;
        else if (E % 2 != 0)
        {
            P = (A * P) % n;
            E = (E - 1) / 2;
        }

        else
        {
            E = E / 2;
        }
        A = (A * A) % n;
    }
}

int *codifica(char *mensagem, long long chavePrivada, long long n)
{
    long long i;
    int *mensagemEncriptada;

    mensagemEncriptada = malloc(100 * sizeof(long long));
    for (i = 0; i < 100; i++)
        mensagemEncriptada[i] = potencia(mensagem[i], chavePrivada, n);

    return mensagemEncriptada;
}

char *decodifica(int *mensagemCriptografada, long long chavePublica, long long n)
{
    long long i;
    char *mensagemDecriptografada;

    mensagemDecriptografada = malloc(100 * sizeof(char));

    for (i = 0; i < 100; i++)
        mensagemDecriptografada[i] = potencia(mensagemCriptografada[i], chavePublica, n);

    return mensagemDecriptografada;
}

void main(void)
{
    int i;
    long long primeiroPrimo, segundoPrimo, n, phi, chavePrivada, chavePublica;
    char mensagem[250];
    int *mensagemCriptografada;
    char *mensagemDescriptografada;
    int ehPrimo;

    primeiroPrimo = obterNumeroPrimo(1);
    segundoPrimo = obterNumeroPrimo(2);

    if (saoNumerosIguais(primeiroPrimo, segundoPrimo))
    {
        printf("\nOs números não podem ser iguais!\n");
        return;
    }

    n = primeiroPrimo * segundoPrimo;
    phi = (primeiroPrimo - 1) * (segundoPrimo - 1);

    chavePrivada = escolheE(phi, primeiroPrimo, segundoPrimo, n);

    printf("\nChave privada: (%llu, %llu)\n", chavePrivada, n);

    chavePublica = mdcEstendido(phi, chavePrivada);

    printf("\nChave publica: (%llu, %llu)\n", chavePublica, n);

    printf("\nDigite uma mensagem:\n");

    scanf("\n");
    fgets(mensagem, 250, stdin);

    mensagemCriptografada = codifica(mensagem, chavePrivada, n);

    printf("\nMensagem encriptada: ");

    for (i = 0; i < strlen(mensagem); i++)
    {
        printf("%c", mensagemCriptografada[i]);
    }
    printf("\n");

    mensagemDescriptografada = decodifica(mensagemCriptografada, chavePublica, n);

    printf("\nMensagem desencriptada: %s\n\n", mensagemDescriptografada);

    free(mensagemCriptografada);
    free(mensagemDescriptografada);
    return;
}
