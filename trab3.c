/*    * @file: 180023691.c
      * @author: Marcos Eduardo Monteiro Junqueira - 18/0023691 (memj401@hotmail.com)
      * @disciplina: Algoritmos e Programação de Computadores
      *
      *Programa que lê o database de um supermercado em binário e,com as informaçẽs fornecidas
      * pelo operador, gera uma nota fiscal*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define VERDADEIRO 1
#define FALSO 0
#define PARADA 2

typedef struct 
{
	char codigo_barra[14];
	char nome[16];
	double preco;
	char unidade;
	
} __attribute__((aligned(8))) produto;  /*Utilização de __attribute__() a fim de alinhar os endereços em blocos de 8 bytes, possibilitando a leitura correta dos dados 
										em  máquinas de arquitetura 32 bits*/

void le_nome_operador(char operador[])
{
	char lixo;
	
	do
	{
		printf("\nFavor inserir o nome do Operador:\n");
		scanf(" %19[^\n]", operador);
		
		while(lixo != '\n') /*Limpeza do Buffer caso a string passe do limite de caracteres*/
		{
			lixo = getchar();
		}
		
		lixo = '0';/*Associa-se um caractere '0' para a variável para poder reutilizá-la, caso necessário*/
		
		if (strlen(operador) > 15)
		{
			printf("\nO nome excedeu 15 caracteres!Por favor, digite outro.\n");
		}
	
	} while (strlen(operador) > 15);
}

int le_produto(produto mercadoria[], int quantidade, produto* produtos_comprados)
{
	char codigo_inserido[20];
	char lixo;
	char criterio_de_parada[] = "0000000000000";
	int i;
	
	do
	{
		printf("\nFavor inserir o código de barras do produto:\n");
		scanf(" %19[^\n]", codigo_inserido);
		
		while(lixo != '\n') 
		{
			lixo = getchar();
		}
		
		lixo = '0';
		
		if (strlen(codigo_inserido) != 13)
		{
			printf("\nCódigo de Barras Inválido\n");
		}
	
	} while (strlen(codigo_inserido) != 13);

	if (strcmp(codigo_inserido, criterio_de_parada) == 0)
	{
		return PARADA;
	}
	
	for (i = 0; i < quantidade; ++i)
	{
		if (strcmp(codigo_inserido, mercadoria[i].codigo_barra)  == 0)
		{
			*(produtos_comprados) = mercadoria[i];
			return VERDADEIRO;
		}
	}
	printf("Código de Barras inexistente\n");
	return FALSO;
}

void le_quantidade_inteira(produto produto_comprado, float* quantidade_comprada)
{
	char valor[100];
	char lixo;
	int valor_valido;
	int i;

	do
	{
		printf("Digite a quantidade:\n");
		scanf(" %99[^\n]", valor);

		while(lixo != '\n') 
		{
			lixo = getchar();
		}
		
		lixo = '0'; 
		
		valor_valido = VERDADEIRO;
		
		for (i = 0; i < strlen(valor); ++i)
		{
			if ((valor[i] > '9' || valor[i] < '0')  && valor_valido != FALSO)
			{
				printf("Quantidade inválida\n");
				valor_valido = FALSO;

			}
		}

		if (valor_valido != FALSO)
		{
			*(quantidade_comprada) = atoi(valor);
			valor_valido = VERDADEIRO;
		}
		
		else
		{
			for (i = 0; i < 100; ++i)
			{
				valor[i] = '0'; /*Iguala o vetor valor a '0' em todas as posições,de maneira a possibilitar a leitura após uma entrada inválida*/
			}
		}	
	
	}while(valor_valido != VERDADEIRO);
		
}
void le_quantidade_real(produto produto_comprado, float* quantidade_comprada)
{
	char valor[100];
	char lixo;
	int valor_valido;
	int i;

	do
	{
		printf("Digite a quantidade (para casas decimais use '.' somente):\n");
		scanf(" %99[^\n]", valor);

		while(lixo != '\n') 
		{
			lixo = getchar();
		}
		
		lixo = '0';
		
		valor_valido = VERDADEIRO;
		
		for (i = 0; i < strlen(valor); ++i)
		{
			if ((valor[i] > '9' || valor[i] < '0') && (valor[i] != '.') && (valor_valido != FALSO))
			{
				printf("Quantidade invalida\n");
				valor_valido = FALSO;

			}
		}

		if (valor_valido != FALSO)
		{
			*(quantidade_comprada) = atof(valor);
			valor_valido = VERDADEIRO;
		}
		
		else
		{
			for (i = 0; i < 100; ++i)
			{
				valor[i] = '0';
			}
		}	
	
	}while(valor_valido != VERDADEIRO);
}	

int main()
{
	char operador[20];
	int i;
	int qnt_produtos_comprados;
	int id = 1;
	float total;
	int quantidade_mercadoria;
	float quantidade_comprada[100]; 
	produto mercadoria[100];
	produto produtos_comprados[100];
	
	
	
	FILE *database = fopen("market_database.bin", "rb");
	FILE *nota= fopen("nota_fiscal.txt", "w");

	if (database == 0)
	{
		printf("Não foi possível abrir o arquivo\n");
		return 0;
	}

	else if (nota == 0)
	{
		printf("Não foi possível criar a nota fiscal\n");
		return 0;
	}
	
	for (i = 0;; ++i)
	{
		fread(&mercadoria[i],sizeof(produto),1,database);
		
		if (feof(database))
		{
			break;
		}
		
	}
	
	quantidade_mercadoria = i;
	fclose(database);
	le_nome_operador(operador);
	
	for (i = 0; i < quantidade_mercadoria; ++i)
	{
		
		
		if(le_produto(mercadoria, quantidade_mercadoria, &produtos_comprados[i]) == PARADA)
		{
			break;
		}
		
		if (produtos_comprados[i].unidade == 'u')
		{
			le_quantidade_inteira(mercadoria[i], &quantidade_comprada[i]);
		}
		else
		{
			le_quantidade_real(mercadoria[i], &quantidade_comprada[i]);
		}
	}
		
	qnt_produtos_comprados = i;
	
	fprintf(nota,"\tSupermercados APC\n");
	fprintf(nota, "\tOperador: %s\n", operador);
	fprintf(nota, "\nId\tCódigo do Produto  Nome do Produto\n");
	fprintf(nota, "\tQtd   Tipo\t   Valor Unitário\tValor Total\n");
	fprintf(nota, "\n");
	
	for(i = 0; i < qnt_produtos_comprados; ++i)
	{
		fprintf(nota, "%d\t%s      %s\n", id, produtos_comprados[i].codigo_barra, produtos_comprados[i].nome);
		fprintf(nota, "\t%.3f %c\t\t   R$ %.2f\t\tR$ %.2f\n", quantidade_comprada[i], produtos_comprados[i].unidade, 
				produtos_comprados[i].preco, (produtos_comprados[i].preco) * (quantidade_comprada[i]));

		total += (produtos_comprados[i].preco) * (quantidade_comprada[i]);
		id += 1;

	}
	
	fprintf(nota, "\nTOTAL: R$ %.2f", total);


	fclose(nota);

	return 0;
}


