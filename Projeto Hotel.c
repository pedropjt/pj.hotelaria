#include <stdio.h>
#include <string.h>

#define ARQUIVO   "gerenciador.txt"   // arquivo principal
#define ARQ_TEMP  "auxiliar.txt"      // arquivo auxiliar
#define ARQUIVO_CONTA "contas.txt"
#define TOTAL_COMUM 50
#define TOTAL_MEDIUM 50
#define TOTAL_VIP 50

typedef struct {
	int codigo;
	char nome[150];
	char cpf[15];
	char telefone[20];
	char categoria[20];
	char status[20];
	char statusHospedagem[20];
} Hospede;

void limpar_buffer(void);
void garantir_arquivo(void);

// Funções para hospede
void cadastrarHospede(void);
void listar(void);
void consultar(void);
void atualizar(void);
void excluir(void);

// Funções para Hospedagem
void checkin(void);
void checkout(void);
void contaHospede(void);
void disponibilidade(void);
void consultarHospedagensEmAndamento(void);

// Permite limpar o buffer do teclado
void limpar_buffer(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

// Cria o CSV com o cabeçalho
void garantir_arquivo(void) {
	FILE *arq = fopen(ARQUIVO, "r");

	if (arq == NULL) {
		arq = fopen(ARQUIVO, "w");
		if (arq != NULL) {
			fprintf(arq, "codigo;nome;cpf;telefone;categoria;status;statusHospedagem\n");
			fclose(arq);
		}
	} else {
		fclose(arq);
	}
}

// Função usada para cadastrar o hospede
void cadastrarHospede(void) {
	Hospede h, aux;
	char linha[300];
	int escolha;
	int escolha1;

	garantir_arquivo();

	printf("Digite o código do hospede: ");
	scanf("%d", &h.codigo);
	limpar_buffer();

	FILE *arq_leitura = fopen(ARQUIVO, "r");
	if (arq_leitura != NULL) {
		fgets(linha, sizeof(linha), arq_leitura); // pula cabeçalho

		while (fscanf(arq_leitura, "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
		              &aux.codigo, aux.nome, aux.cpf, aux.telefone, aux.categoria, aux.status, aux.statusHospedagem) == 7) {
			if (aux.codigo == h.codigo) {
				printf("Já existe um hospede com esse código!\n");
				fclose(arq_leitura);
				return;
			}
		}
		fclose(arq_leitura);
	}

	FILE *arq = fopen(ARQUIVO, "a");

	if (arq == NULL) {
		printf("Erro ao abrir o arquivo %s.\n", ARQUIVO);
		return;
	}

	printf("Digite o nome do Hospede: ");
	fgets(h.nome, sizeof(h.nome), stdin);
	h.nome[strcspn(h.nome, "\n")] = '\0';

	printf("Digite o cpf do hospede: ");
	scanf("%14s", h.cpf);
	limpar_buffer();

	printf("Digite o telefone do hospede: ");
	scanf("%19s", h.telefone);
	limpar_buffer();

	do {
		printf("\n===== CATEGORIA DE QUARTO =====\n");
		printf("1- Quarto Comum = R$ 120,00\n");
		printf("2- Quarto medium = R$ 200,00\n");
		printf("3- Quarto vip = R$ 350,00\n");
		printf("Escolha: ");
		scanf("%d", &escolha);
		limpar_buffer();

		switch (escolha) {
		case 1:
			strcpy(h.categoria, "Quarto Comum");
			break;
		case 2:
			strcpy(h.categoria, "Quarto medium");
			break;
		case 3:
			strcpy(h.categoria, "Quarto vip");
			break;
		default:
			printf("Opção inválida! Tente novamente.\n");
			break;
		}
	} while (escolha < 1 || escolha > 3);

	do {
		printf("\n===== CATEGORIA DE STATUS =====\n");
		printf("1- Ativo\n");
		printf("2- Inativo\n");
		printf("Digite a opção: ");
		scanf("%d", &escolha1);
		limpar_buffer();

		switch (escolha1) {
		case 1:
			strcpy(h.status, "Ativo");
			break;
		case 2:
			strcpy(h.status, "Inativo");
			break;
		default:
			printf("Opção inválida! Tente novamente.\n");
			break;
		}
	} while (escolha1 < 1 || escolha1 > 2);

	strcpy(h.statusHospedagem, "Nao Hospedado");

	fprintf(arq, "%d;%s;%s;%s;%s;%s;%s\n",
	        h.codigo, h.nome, h.cpf, h.telefone, h.categoria, h.status, h.statusHospedagem);
	fclose(arq);

	printf("Hospede cadastrado com sucesso!!\n");
}

void listar(void) {
	Hospede h;
	char linha[300];
	int total = 0;
	FILE *arq = fopen(ARQUIVO, "r");

	if (arq == NULL) {
		printf("Erro ao abrir o arquivo %s.\n", ARQUIVO);
		return;
	}

	fgets(linha, sizeof(linha), arq);

	printf("\n%-8s %-25s %-15s %-15s %-15s %-10s %-16s\n", "CODIGO", "NOME", "CPF", "TELEFONE", "CATEGORIA", "STATUS", "HOSPEDAGEM");
	printf("---------------------------------------------------------------------------------------------------------------\n");

	while (fscanf(arq, "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
	              &h.codigo, h.nome, h.cpf, h.telefone, h.categoria, h.status, h.statusHospedagem) == 7) {
		printf("%-8d %-25s %-15s %-15s %-15s %-10s %-16s\n",
		       h.codigo, h.nome, h.cpf, h.telefone, h.categoria, h.status, h.statusHospedagem);
		total++;
	}

	if (total == 0)
		printf("Nenhum hospede cadastrado.\n");

	printf("---------------------------------------------------------------------------------------------------------------\n");
	fclose(arq);
}

void consultar(void) {
	int busca = 0, encontrado = 0;
	Hospede h;
	char linha[300];

	listar();

	FILE *arq = fopen(ARQUIVO, "r");

	if (arq == NULL) {
		printf("Erro ao abrir o arquivo %s.\n", ARQUIVO);
		return;
	}

	printf("Digite o código do hospede que deseja consultar: ");
	scanf("%d", &busca);
	limpar_buffer();

	fgets(linha, sizeof(linha), arq);

	while (fscanf(arq, "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
	              &h.codigo, h.nome, h.cpf, h.telefone, h.categoria, h.status, h.statusHospedagem) == 7) {
		if (h.codigo == busca) {
			encontrado = 1;
			printf("\n--- Dados do Hóspede ---\n");
			printf("Código: %d\n", h.codigo);
			printf("Nome: %s\n", h.nome);
			printf("CPF: %s\n", h.cpf);
			printf("Telefone: %s\n", h.telefone);
			printf("Categoria: %s\n", h.categoria);
			printf("Status: %s\n", h.status);
			printf("Status Hospedagem: %s\n", h.statusHospedagem);
			printf("------------------------\n");
			break;
		}
	}

	if (!encontrado)
		printf("Hospede com o código %d não encontrado\n", busca);

	fclose(arq);
}

void atualizar(void) {
	int busca, alterar = 0;
	int escolha, escolha1;
	Hospede h;
	char linha[300];

	listar();

	FILE *arq  = fopen(ARQUIVO, "r");
	FILE *temp = fopen(ARQ_TEMP, "w");

	if (arq == NULL || temp == NULL) {
		printf("Erro ao abrir os arquivos.\n");
		if (arq) fclose(arq);
		if (temp) fclose(temp);
		return;
	}

	printf("Digite o código do hospede que deseja atualizar: ");
	scanf("%d", &busca);
	limpar_buffer();

	fgets(linha, sizeof(linha), arq);
	fputs(linha, temp);

	while (fscanf(arq, "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
	              &h.codigo, h.nome, h.cpf, h.telefone, h.categoria, h.status, h.statusHospedagem) == 7) {

		if (h.codigo == busca) {
			printf("Hospede encontrado: %s\n", h.nome);

			printf("Novo nome: ");
			fgets(h.nome, sizeof(h.nome), stdin);
			h.nome[strcspn(h.nome, "\n")] = '\0';

			printf("Novo cpf: ");
			scanf("%14s", h.cpf);
			limpar_buffer();

			printf("Novo telefone: ");
			scanf("%19s", h.telefone);
			limpar_buffer();

			do {
				printf("\n===== NOVA CATEGORIA DE QUARTO =====\n");
				printf("Categoria atual: %s\n", h.categoria);
				printf("0- Manter categoria atual\n");
				printf("1- Quarto Comum = R$ 120,00\n");
				printf("2- Quarto medium = R$ 200,00\n");
				printf("3- Quarto vip = R$ 350,00\n");
				printf("Escolha: ");
				scanf("%d", &escolha);
				limpar_buffer();

				switch (escolha) {
				case 0:
					break;
				case 1:
					strcpy(h.categoria, "Quarto Comum");
					break;
				case 2:
					strcpy(h.categoria, "Quarto medium");
					break;
				case 3:
					strcpy(h.categoria, "Quarto vip");
					break;
				default:
					printf("Opção inválida! Tente novamente.\n");
					break;
				}
			} while (escolha < 0 || escolha > 3);

			do {
				printf("\n===== NOVO STATUS =====\n");
				printf("Status atual: %s\n", h.status);
				printf("0- Manter status atual\n");
				printf("1- Ativo\n");
				printf("2- Inativo\n");
				printf("Digite a opção: ");
				scanf("%d", &escolha1);
				limpar_buffer();

				switch (escolha1) {
				case 0:
					break;
				case 1:
					strcpy(h.status, "Ativo");
					break;
				case 2:
					strcpy(h.status, "Inativo");
					break;
				default:
					printf("Opção inválida! Tente novamente.\n");
					break;
				}
			} while (escolha1 < 0 || escolha1 > 2);

			alterar = 1;
		}

		fprintf(temp, "%d;%s;%s;%s;%s;%s;%s\n",
		        h.codigo, h.nome, h.cpf, h.telefone, h.categoria, h.status, h.statusHospedagem);
	}

	fclose(arq);
	fclose(temp);

	if (alterar) {
		remove(ARQUIVO);
		rename(ARQ_TEMP, ARQUIVO);
		printf("Cadastro do hospede alterado com sucesso!!\n");
	} else {
		remove(ARQ_TEMP);
		printf("Hospede com codigo %d nao encontrado\n", busca);
	}
}

void excluir(void) {
	int busca, excluir = 0;
	Hospede h;
	char linha[300];

	listar();

	FILE *arq  = fopen(ARQUIVO, "r");
	FILE *temp = fopen(ARQ_TEMP, "w");

	if (arq == NULL || temp == NULL) {
		printf("Erro ao abrir os arquivos.\n");
		if (arq) fclose(arq);
		if (temp) fclose(temp);
		return;
	}

	printf("Digite o codigo do hospede que deseja excluir: ");
	scanf("%d", &busca);
	limpar_buffer();

	fgets(linha, sizeof(linha), arq);
	fputs(linha, temp);

	while (fscanf(arq, "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
	              &h.codigo, h.nome, h.cpf, h.telefone, h.categoria, h.status, h.statusHospedagem) == 7) {
		if (h.codigo != busca) {
			fprintf(temp, "%d;%s;%s;%s;%s;%s;%s\n",
			        h.codigo, h.nome, h.cpf, h.telefone, h.categoria, h.status, h.statusHospedagem);
		} else {
			excluir = 1;
		}
	}

	fclose(arq);
	fclose(temp);

	if (excluir) {
		remove(ARQUIVO);
		rename(ARQ_TEMP, ARQUIVO);
		printf("Hospede excluido com sucesso!\n");
	} else {
		remove(ARQ_TEMP);
		printf("Hospede com codigo %d não encontrado\n", busca);
	}
}

void contaHospede(void) {

	int codigo;
	int opcao;
	int quantidade;
	int encontrado = 0;

	float total = 0;
	float subtotal;

	float precos[5] = {
		30.00, 50.00, 90.00, 150.00, 200.00
	};

	char produtos[5][30] = {
		"Coca Cola",
		"Salgadinho Ruffles",
		"Agua Mineral",
		"Pringles",
		"Barra de Chocolate"
	};

	Hospede h;
	char linha[300];

	FILE *cadastro = fopen(ARQUIVO, "r");

	if (cadastro == NULL) {
		printf("Erro ao abrir arquivo de hospedes.\n");
		return;
	}

	printf("\nDigite o codigo do hospede: ");
	scanf("%d", &codigo);
	limpar_buffer();

	// Procura o hospede
	fgets(linha, sizeof(linha), cadastro);

	while (fscanf(cadastro,
	              "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
	              &h.codigo, h.nome, h.cpf,
	              h.telefone, h.categoria, h.status, h.statusHospedagem) == 7) {

		if (h.codigo == codigo) {
			encontrado = 1;
			break;
		}
	}

	fclose(cadastro);

	if (!encontrado) {
		printf("Hospede nao encontrado!\n");
		return;
	}

	if (strcmp(h.statusHospedagem, "Hospedado") != 0) {
		printf("Este hospede nao esta hospedado no momento!\n");
		return;
	}

	printf("\nHospede: %s\n", h.nome);
	printf("Codigo: %d\n", h.codigo);

	FILE *conta = fopen(ARQUIVO_CONTA, "a");

	if (conta == NULL) {
		printf("Erro ao abrir arquivo de contas.\n");
		return;
	}

	do {

		printf("\n===== MENU DE CONSUMO =====\n");

		printf("1 - Coca Cola ............... R$ 30,00\n");
		printf("2 - Salgadinho Ruffles ...... R$ 50,00\n");
		printf("3 - Agua Mineral ............ R$ 90,00\n");
		printf("4 - Pringles ................ R$ 150,00\n");
		printf("5 - Barra de Chocolate ...... R$ 200,00\n");

		printf("0 - Finalizar consumo\n");

		printf("\nDigite a opcao desejada: ");
		scanf("%d", &opcao);
		limpar_buffer();

		if (opcao >= 1 && opcao <= 5) {

			printf("Digite a quantidade consumida: ");
			scanf("%d", &quantidade);
			limpar_buffer();

			if (quantidade <= 0) {
				printf("Quantidade invalida!\n");
				continue;
			}

			subtotal = precos[opcao - 1] * quantidade;

			total += subtotal;

			fprintf(conta, "%d;%s;%d;%.2f\n",
			        codigo,
			        produtos[opcao - 1],
			        quantidade,
			        subtotal);

			printf("\nProduto: %s\n", produtos[opcao - 1]);
			printf("Quantidade: %d\n", quantidade);
			printf("Subtotal: R$ %.2f\n", subtotal);

		} else if (opcao != 0) {

			printf("Opcao invalida!\n");
		}

	} while (opcao != 0);

	fclose(conta);

	printf("\n===== CONTA DO HOSPEDE =====\n");
	printf("Hospede: %s\n", h.nome);
	printf("Codigo: %d\n", h.codigo);
	printf("Total desta compra: R$ %.2f\n", total);

	printf("\nConsumo registrado com sucesso!\n");
}

// CHECkIN
void checkin(void) {
	int codigo;
	int encontrado = 0;
	Hospede h;

	FILE *arquivo = fopen(ARQUIVO, "r");
	FILE *temp = fopen(ARQ_TEMP, "w");

	if (arquivo == NULL || temp == NULL) {
		printf("Erro ao abrir os arquivos!\n");

		if (arquivo != NULL) fclose(arquivo);
		if (temp != NULL) fclose(temp);

		return;
	}

	printf("Digite o codigo do hospede: ");
	scanf("%d", &codigo);
	limpar_buffer();

	char linha[500];

	fgets(linha, sizeof(linha), arquivo);
	fputs(linha, temp);

	while (fscanf(arquivo,
	              "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
	              &h.codigo, h.nome, h.cpf,
	              h.telefone, h.categoria, h.status, h.statusHospedagem) == 7) {

		if (h.codigo == codigo) {
			encontrado = 1;

			if (strcmp(h.status, "Ativo") != 0) {
				printf("Este hospede esta Inativo e nao pode realizar check-in!\n");
			} else if (strcmp(h.statusHospedagem, "Hospedado") == 0) {
				printf("Este hospede ja realizou o check-in!\n");
			} else {
				strcpy(h.statusHospedagem, "Hospedado");
				printf("Check-in realizado com sucesso!\n");
			}
		}

		fprintf(temp, "%d;%s;%s;%s;%s;%s;%s\n",
		        h.codigo, h.nome, h.cpf,
		        h.telefone, h.categoria, h.status, h.statusHospedagem);
	}

	fclose(arquivo);
	fclose(temp);

	if (encontrado) {
		remove(ARQUIVO);
		rename(ARQ_TEMP, ARQUIVO);
	} else {
		remove(ARQ_TEMP);
		printf("Hospede nao encontrado!\n");
	}
}

// CHECK-OUT
void checkout(void) {
	int codigo;
	int encontrado = 0;
	int hospedado = 0;
	Hospede h;

	FILE *arquivo = fopen(ARQUIVO, "r");
	FILE *temp = fopen(ARQ_TEMP, "w");

	if (arquivo == NULL || temp == NULL) {
		printf("Erro ao abrir os arquivos!\n");

		if (arquivo != NULL) fclose(arquivo);
		if (temp != NULL) fclose(temp);

		return;
	}

	printf("Digite o codigo do hospede: ");
	scanf("%d", &codigo);
	limpar_buffer();

	char linha[500];

	fgets(linha, sizeof(linha), arquivo);
	fputs(linha, temp);

	while (fscanf(arquivo,
	              "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
	              &h.codigo, h.nome, h.cpf,
	              h.telefone, h.categoria, h.status, h.statusHospedagem) == 7) {

		if (h.codigo == codigo) {
			encontrado = 1;

			if (strcmp(h.statusHospedagem, "Hospedado") == 0) {
				strcpy(h.statusHospedagem, "Finalizado");
				hospedado = 1;

				printf("Check-out realizado com sucesso!\n");
			} else {
				printf("Este hospede nao possui hospedagem ativa!\n");
			}
		}

		fprintf(temp, "%d;%s;%s;%s;%s;%s;%s\n",
		        h.codigo, h.nome, h.cpf,
		        h.telefone, h.categoria, h.status, h.statusHospedagem);
	}

	fclose(arquivo);
	fclose(temp);

	if (encontrado && hospedado) {
		remove(ARQUIVO);
		rename(ARQ_TEMP, ARQUIVO);
	} else {
		remove(ARQ_TEMP);
	}

	if (!encontrado) {
		printf("Hospede nao encontrado!\n");
	}
}

void disponibilidade(void) {
	Hospede h;
	char linha[300];
	int ocupadosComum = 0, ocupadosMedium = 0, ocupadosVip = 0;

	FILE *arq = fopen(ARQUIVO, "r");

	if (arq == NULL) {
		printf("Erro ao abrir o arquivo %s.\n", ARQUIVO);
		return;
	}

	fgets(linha, sizeof(linha), arq);

	while (fscanf(arq, "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
	              &h.codigo, h.nome, h.cpf, h.telefone, h.categoria, h.status, h.statusHospedagem) == 7) {

		if (strcmp(h.statusHospedagem, "Hospedado") == 0) {
			if (strcmp(h.categoria, "Quarto Comum") == 0) {
				ocupadosComum++;
			} else if (strcmp(h.categoria, "Quarto medium") == 0) {
				ocupadosMedium++;
			} else if (strcmp(h.categoria, "Quarto vip") == 0) {
				ocupadosVip++;
			}
		}
	}

	fclose(arq);

	printf("\n===== DISPONIBILIDADE DE QUARTOS =====\n");
	printf("Quarto Comum  (R$ 120,00): %d disponiveis / %d ocupados (Total: %d)\n",
	       TOTAL_COMUM - ocupadosComum, ocupadosComum, TOTAL_COMUM);
	printf("Quarto Medium (R$ 200,00): %d disponiveis / %d ocupados (Total: %d)\n",
	       TOTAL_MEDIUM - ocupadosMedium, ocupadosMedium, TOTAL_MEDIUM);
	printf("Quarto VIP    (R$ 350,00): %d disponiveis / %d ocupados (Total: %d)\n",
	       TOTAL_VIP - ocupadosVip, ocupadosVip, TOTAL_VIP);
	printf("======================================\n");
}


void consultarHospedagensEmAndamento(void) {
	Hospede h;
	char linha[500];
	int total = 0;
	int totalComum = 0;
	int totalMedium = 0;
	int totalVip = 0;

	FILE *arq = fopen(ARQUIVO, "r");

	if (arq == NULL) {
		printf("Erro ao abrir o arquivo de hospedes!\n");
		return;
	}

	// Pula o cabecalho do arquivo
	if (fgets(linha, sizeof(linha), arq) == NULL) {
		printf("Arquivo de hospedes vazio!\n");
		fclose(arq);
		return;
	}

	printf("\n========== HOSPEDAGENS EM ANDAMENTO ==========\n");
	printf("%-8s %-25s %-20s %-18s\n",
	       "CODIGO", "NOME", "CATEGORIA", "STATUS");
	printf("--------------------------------------------------------------------------\n");

	while (fscanf(arq,
	              "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
	              &h.codigo, h.nome, h.cpf, h.telefone,
	              h.categoria, h.status, h.statusHospedagem) == 7) {

		// Exibe somente hospedes que ja realizaram checkin
		if (strcmp(h.statusHospedagem, "Hospedado") == 0) {
			printf("%-8d %-25s %-20s %-18s\n",
			       h.codigo, h.nome, h.categoria, h.statusHospedagem);

			total++;

			if (strcmp(h.categoria, "Quarto Comum") == 0) {
				totalComum++;
			} else if (strcmp(h.categoria, "Quarto medium") == 0) {
				totalMedium++;
			} else if (strcmp(h.categoria, "Quarto vip") == 0) {
				totalVip++;
			}
		}
	}

	printf("--------------------------------------------------------------------------\n");

	if (total == 0) {
		printf("Nenhuma hospedagem em andamento.\n");
	} else {
		printf("Total de hospedagens em andamento: %d\n", total);
		printf("Quartos Comuns ocupados: %d\n", totalComum);
		printf("Quartos Medium ocupados: %d\n", totalMedium);
		printf("Quartos VIP ocupados: %d\n", totalVip);
	}

	printf("==============================================\n");
	fclose(arq);
}

// RELATORIO DE HOSPEDAGEM
void relatorioHospedagem(void) {

	Hospede h;
	char linha[300];

	int totalHospedados = 0;
	int totalFinalizados = 0;

	int totalComum = 0;
	int totalMedium = 0;
	int totalVip = 0;

	FILE *arq = fopen(ARQUIVO, "r");

	if (arq == NULL) {
		printf("Erro ao abrir o arquivo de hospedes!\n");
		return;
	}

	// Pula o cabecalho do arquivo
	fgets(linha, sizeof(linha), arq);

	printf("\n========== RELATORIO DE HOSPEDAGEM ==========\n");

	printf("\n%-10s %-25s %-20s %-15s\n",
	       "CODIGO", "NOME", "CATEGORIA", "STATUS");

	printf("--------------------------------------------------------------------------\n");

	while (fscanf(arq,
	              "%d;%149[^;];%14[^;];%19[^;];%19[^;];%19[^;];%19[^\n]\n",
	              &h.codigo,
	              h.nome,
	              h.cpf,
	              h.telefone,
	              h.categoria,
	              h.status,
	              h.statusHospedagem) == 7) {

		// Mostra somente hospedagens iniciadas ou finalizadas
		if (strcmp(h.statusHospedagem, "Hospedado") == 0 ||
		        strcmp(h.statusHospedagem, "Finalizado") == 0) {

			printf("%-10d %-25s %-20s %-15s\n",
			       h.codigo,
			       h.nome,
			       h.categoria,
			       h.statusHospedagem);

			// Conta os status
			if (strcmp(h.statusHospedagem, "Hospedado") == 0) {
				totalHospedados++;
			} else {
				totalFinalizados++;
			}

			// Conta as categorias
			if (strcmp(h.categoria, "Quarto Comum") == 0) {
				totalComum++;
			} else if (strcmp(h.categoria, "Quarto medium") == 0) {
				totalMedium++;
			} else if (strcmp(h.categoria, "Quarto vip") == 0) {
				totalVip++;
			}
		}
	}

	fclose(arq);

	printf("--------------------------------------------------------------------------\n");

	printf("\n========== RESUMO DAS HOSPEDAGENS ==========\n");

	printf("Total de hospedagens em andamento: %d\n", totalHospedados);
	printf("Total de hospedagens finalizadas: %d\n",
	       totalFinalizados);

	printf("\n========== HOSPEDAGENS POR CATEGORIA ==========\n");

	printf("Quarto Comum: %d\n", totalComum);
	printf("Quarto medium: %d\n", totalMedium);
	printf("Quarto VIP: %d\n", totalVip);

	printf("==============================================\n");

	if (totalHospedados == 0 && totalFinalizados == 0) {
		printf("\nNenhuma hospedagem encontrada.\n");
	}
}

int main(void) {
	int escolha1, escolha2, escolha3;

	garantir_arquivo();

	do {
		printf("\n===========================\n");
		printf("\t GESTÃO DE HÓSPEDES\n");
		printf("===========================\n");
		printf("1- Gestão de Hóspedes\n");
		printf("2- Gestão de Hospedagem\n");
		printf("3- Conta de Hospede\n");
		printf("4- Relatórios\n");
		printf("5- Sair\n");
		printf("Digite a opção desejada: ");
		scanf("%d", &escolha1);
		limpar_buffer();

		switch (escolha1) {
		case 1:
			printf("\n===========================\n");
			printf("\t GESTÃO DE HÓSPEDES\n");
			printf("===========================\n");
			printf("1- Cadastrar Hospede\n");
			printf("2- Gerenciar Hospedes\n");
			printf("Digite a opção: ");
			scanf("%d", &escolha2);
			limpar_buffer();

			switch (escolha2) {
			case 1:
				cadastrarHospede();
				break;
			case 2:
				printf("1- Listar Hospedes\n");
				printf("2- Consultar Hospede\n");
				printf("3- Atualizar cadastro\n");
				printf("4- Excluir cadastro\n");
				printf("Digite a opção: ");
				scanf("%d", &escolha3);
				limpar_buffer();

				switch (escolha3) {
				case 1:
					listar();
					break;
				case 2:
					consultar();
					break;
				case 3:
					atualizar();
					break;
				case 4:
					excluir();
					break;
				default:
					printf("Opção inválida\n");
					break;
				}
				break;
			default:
				printf("Opção inválida\n");
				break;
			}
			break;

		case 2:
			printf("\n===== GESTÃO DE HOSPEDAGEM =====\n");
			printf("1- Consultar disponibilidade dos quartos\n");
			printf("2- Realizar Check-in\n");
			printf("3- Realizar Check-out\n");
			printf("4- Consultar hospedagens em andamento\n");
			printf("Digite a opção: ");
			scanf("%d", &escolha2);
			limpar_buffer();

			switch (escolha2) {
			case 1:
				disponibilidade();
				break;
			case 2:
				checkin();
				break;
			case 3:
				checkout();
				break;
			case 4:
				consultarHospedagensEmAndamento();
				break;
			default:
				printf("Opção inválida!\n");
				break;
			}
			break;

		case 3:
			contaHospede();
			break;

		case 4:
			relatorioHospedagem();
			break;

		case 5:
			printf("Encerrando o programa...\n");
			break;

		default:
			printf("Opção inválida, tente novamente\n");
			break;
		}
	} while (escolha1 != 5);

	return 0;
}