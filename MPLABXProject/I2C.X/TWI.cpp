/* 
 * File:   TWI.cpp
 * Author: marcone
 * 
 * Created on 27 de Outubro de 2019, 15:57
 */

#include "TWI.h"

TWI::TWI() {                                                /* TWI initialize function */
    TWBR = BITRATE(TWSR = 0x00);							/* Get bit rate register value by formula */
}

TWI::~TWI() {}

/*IN�CIO(S)
    - Condi��o START emitida pelo mestre.
    - � gerado pela transi��o Alto para Baixo no SDA enquanto o SCL � Alto.
    - Enquanto o barramento estiver livre, o dispositivo mestre emite uma condi��o START com o endere�o do dispositivo escravo (SLA) para iniciar a transmiss�o.
    Fun��o Start - inicia a condi��o START
    Argumento de entrada: - possui argumento de entrada do endere�o de grava��o do dispositivo escravo (SLA + W).
    Retorno: - retorna o status do evento.
 */
uint8_t TWI::Start(char write_address){						/* TWI start function */
	uint8_t status;											/* Declarar vari�vel */
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);					/* Ative o TWI, gere condi��o de in�cio e limpe o sinalizador de interrup��o */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde at� que o TWI termine seu trabalho atual (condi��o inicial) */
	status = TWSR & 0xF8;									/* Leia o registro de status TWI com mascaramento de tr�s bits inferiores */
	if (status != 0x08){									/* Verificar a condi��o de in�cio foi transmitida com sucesso ou n�o? */
        return 0;											/* Caso contr�rio, retorne 0 para indicar falha na condi��o de in�cio */
	}
    TWDR = write_address;									/* Se sim, escreva SLA + W no registro de dados TWI */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Ative o TWI e limpe o sinalizador de interrup��o */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde at� o TWI concluir seu trabalho atual (opera��o de grava��o) */
	status = TWSR & 0xF8;									/* Leia o registro de status TWI com mascaramento de tr�s bits inferiores */	
	if (status == 0x18){									/* Verifique se o SLA + W foi transmitido e ack recebido ou n�o?  */
        return 1;											/* Se sim, retorne 1 para indicar a confirma��o recebida, ou seja, pronto para aceitar o byte de dados */
	}
    if (status == 0x20){									/* Verifique o SLA + W foi transmitido e o nack recebido ou n�o? */
        return 2;											/* Se sim, retorne 2 para indicar o nack recebido, ou seja, o dispositivo est� ocupado */
	}else
        return 3;											/* Caso contr�rio, retorne 3 para indicar falha no SLA + W */
}

/*IN�CIO REPETIDO(Sr)
    - Condi��o START REPETIDA emitida pelo mestre.
    - � gerado emitindo outra condi��o START para iniciar uma nova transmiss�o.
    - A condi��o START REPEATED com endere�o de dispositivo escravo (SLA) � emitida entre a condi��o START e STOP
    Fun��o Repeated_Start - gera a condi��o REPEATED START para opera��o de leitura.
    Argumento de entrada: - possui argumento de entrada do endere�o de leitura do dispositivo escravo (SLA + R).
    Retorno: - retorna o status do evento.
 */
uint8_t TWI::Repeated_Start(char read_address){				/* TWI repeated start function */
	uint8_t status;											/* Declarar vari�vel */
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);					/* Ative o TWI, gere condi��o de in�cio e limpe o sinalizador de interrup��o */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde at� que o TWI termine seu trabalho atual (condi��o inicial) */
	status = TWSR & 0xF8;									/* Leia o registro de status TWI com mascaramento de tr�s bits inferiores */
	if (status != 0x10){									/* Verifique a condi��o de in�cio repetido foi transmitida com sucesso ou n�o? */
        return 0;											/* Se n�o, retorne 0 para indicar falha de condi��o de in�cio repetida */
	}
    TWDR = read_address;									/* Se sim, escreva SLA + R no registro de dados TWI */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Ative o TWI e limpe o sinalizador de interrup��o */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde at� o TWI concluir seu trabalho atual (opera��o de grava��o) */
	status = TWSR & 0xF8;									/* Leia o registro de status TWI com mascaramento de tr�s bits inferiores */
	if (status == 0x40){									/* Verifique se o SLA + R foi transmitido e ack aceito ou n�o? */
        return 1;											/* Se sim, retorne 1 para indicar a confirma��o recebida */ 
	}
    if (status == 0x20){									/* Verificar SLA + R foi transmitido e nack recebido ou n�o? */
        return 2;											/* Se sim, retorne 2 para indicar o nack recebido, ou seja, o dispositivo est� ocupado */
	}else
        return 3;											/* Outro retorno 3 para indicar falha no SLA + R */
}

/*PARAR(P)
    - Evento STOP emitido pelo mestre para indicar que ele deve parar.
    - � gerado pela transi��o Baixa para Alta no SDA enquanto o SCL � Alto.
    Fun��o Stop - inicia a condi��o STOP
    Argumento de entrada: - n�o possui argumento de entrada.
    Retorno: - n�o retorna nenhum tipo de dado.
 */
void TWI::Stop(){											/* TWI stop function */
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);					/* Ative o TWI, gere a condi��o de parada e limpe o sinalizador de interrup��o */
	while(TWCR & (1<<TWSTO));								/* Aguarde at� a execu��o da condi��o de parada */
}

void TWI::Start_Wait(char write_address){					/* I2C start wait function */
	uint8_t status;											/* Declarar vari�vel */
	while (1)
	{
		TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);				/* Ative o TWI, gere condi��o de in�cio e limpe o sinalizador de interrup��o */
		while (!(TWCR & (1<<TWINT)));						/* Aguarde at� que o TWI termine seu trabalho atual (condi��o inicial) */
		status = TWSR & 0xF8;								/* Leia o registro de status TWI com mascaramento de tr�s bits inferiores */
		if (status != 0x08){								/* Verifique se a condi��o de in�cio foi transmitida com sucesso ou n�o? */
            continue;										/* Se n�o, continue com o loop inicial novamente */
		}
        TWDR = write_address;								/* Se sim, escreva SLA + W no registro de dados TWI */
		TWCR = (1<<TWEN)|(1<<TWINT);						/* Ative o TWI e limpe o sinalizador de interrup��o */
		while (!(TWCR & (1<<TWINT)));						/* Aguarde at� o TWI concluir seu trabalho atual (opera��o de grava��o) */
		status = TWSR & 0xF8;								/* Leia o registro de status TWI com mascaramento de tr�s bits inferiores */
		if (status != 0x18 ){								/* Verifique se o SLA + W foi transmitido e o ack recebido ou n�o? */
			TWI::Stop();									/* Caso contr�rio, gere a condi��o de parada */
			continue;										/* Continue com o loop inicial novamente */
		}
		break;												/* Se sim, interrompa o loop */
	}
}

/*ESCREVA(W)
    - O evento WRITE data / address � emitido pelo mestre ap�s o reconhecimento bem-sucedido do START recebido do dispositivo escravo.
    Fun��o Write - grava dados/endere�o no barramento
    Argumento de entrada: - possui argumento de entrada de dados / endere�o.
    Retorno: - retorna o status do evento.
 */
uint8_t TWI::Write(char data){								/* TWI write function */
	uint8_t status;											/* Declarar vari�vel */
	TWDR = data;											/* Copiar dados no registro de dados TWI */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Ative o TWI e limpe o sinalizador de interrup��o */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde at� o TWI concluir seu trabalho atual (opera��o de grava��o) */
	status = TWSR & 0xF8;									/* Leia o registro de status TWI com mascaramento de tr�s bits inferiores */
	if (status == 0x28){									/* Verifique se os dados foram transmitidos e ack recebido ou n�o? */
        return 0;											/* Se sim, retorne 0 para indicar que a confirma��o foi recebida */
	}
    if (status == 0x30){									/* Verificar se os dados foram transmitidos e nack recebido ou n�o? */
        return 1;											/* Se sim, retorne 1 para indicar nack recebido */
	}else
        return 2;											/* Outro retorno 2 para indicar falha na transmiss�o de dados */
}

/*LER(R)
    - O evento de leitura de dados � emitido pelo mestre ap�s a condi��o REPEATED START bem-sucedida.
    Fun��o Read_Ack - l� dados dispon�veis na linha SDA e retorna ao dispositivo escravo a confirma��o sobre a leitura bem-sucedida dos dados,
    e tamb�m informa ao escravo para transmitir outros dados.
    Argumento de entrada: - n�o possui argumento de entrada.
    Retorno: - retorna dados recebidos.
 */
char TWI::Read_Ack(){										/* TWI read ack function */
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);					/* Ativar TWI, gerar o ack e limpar o sinalizador de interrup��o */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde at� o TWI concluir seu trabalho atual (opera��o de leitura) */
	return TWDR;											/* Retornar dados recebidos */
}	

/*Fun��o Read_Nack - l� o �ltimo byte de dados necess�rio dispon�vel na linha SDA,mas n�o retorna o reconhecimento. 
    - Indica ao escravo que o mestre n�o deseja os pr�ximos dados e deseja interromper a comunica��o.
    Argumento de entrada: - n�o possui argumento de entrada.
    Retorno: - retorna os dados recebidos.
 */
char TWI::Read_Nack(){										/* TWI read nack function */
	TWCR=(1<<TWEN)|(1<<TWINT);								/* Ative o TWI e limpe o sinalizador de interrup��o */
	while (!(TWCR & (1<<TWINT)));							/* Aguarde at� o TWI concluir seu trabalho atual (opera��o de leitura) */
	return TWDR;											/* Retornar dados recebidos */
}	

/*Fun��o de inicializa��o I2C Slave
    - Atribuir um endere�o de dispositivo de 7 bits no registro TWAR. 
    - Ap�s atribuir o endere�o, precisamos ativar o TWI e o bit de reconhecimento no TWCR.
    - E limpe o sinalizador de interrup��o do TWI escrevendo a l�gica 1 nele.
    Argumento de entrada: - possui argumento de entrada do endere�o escravo.
    Retorno: - n�o retorna nenhum tipo de dado.
 */
void TWI::Slave_Init(uint8_t slave_address){
	TWAR = slave_address;                                   /* Atribuir endere�o no registro de endere�o TWI */
	TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT);              /* Ativar TWI, Ativar gera��o de ack, limpar a interrup��o TWI */
}

/*Ouvir o Barramento
    - O dispositivo escravo sempre precisa ouvir o barramento TWI para verificar se ele � endere�ado por qualquer mestre.
    - Quando endere�ado, o bit TWINT � configurado. Ent�o, precisa monitorar bit TWINT.
    Fun��o Slave_Listen
    Argumento de entrada: - n�o possui nenhum argumento de entrada.
    Retorno: - retorna o status do evento.
 */
uint8_t TWI::Slave_Listen(){
	while(1)
	{
		uint8_t status;                                     /* Declarar vari�vel */
        TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);    /* Obter reconhecimento, Ativar TWI, Limpar sinalizador de interrup��o TWI*/
		while (!(TWCR & (1<<TWINT)));                       /* Aguarde para ser enderessado */
		status = TWSR & 0xF8;                               /* Leia o registro de status TWI com mascaramento de tr�s bits inferiores */
		if (status == 0x60 || status == 0x68){              /* Verifique se o SLA + W foi recebido e retornado o ack (TWEA = 1) */
            return 0;                                       /* Se sim, retorne 0 para indicar que a confirma��o retornou */
		}
        if (status == 0xA8 || status == 0xB0){              /* Verificar se o SLA + R foi recebido e retornado o ack (TWEA = 1) */
            return 1;                                       /* Se sim, retorne 1 para indicar que a confirma��o retornou */
        }
        if (status == 0x70 || status == 0x78){              /* Verifique se uma chamada geral foi recebida e retornado o ack (TWEA = 1) */
            return 2;                                       /* Se sim, retorne 2 para indicar que a confirma��o retornou */
		}else
            continue;                                       /* Sen�o continue */
	}
}

/*Transmitir dados
    - Depois de ser endere�ado pelo mestre com endere�o SLA + R, o escravo deve enviar os dados solicitados.
    - Os dados a serem enviados precisam ser gravados no registro TWDR.
    - Ap�s a grava��o dos dados, ative o TWI com confirma��o e limpe o sinalizador de interrup��o.
    - Se o reconhecimento n�o for recebido do mestre, o dispositivo escravo apaga a bandeira TWINT e novamente escuta o barramento.
    - Al�m disso, se RECIPITED START/STOP recebido, o dispositivo escravo apaga a bandeira TWINT e novamente escuta o barramento.
    Fun��o Slave_Transmit
    Argumento de entrada: - possui argumento de entrada de dados de caracteres a serem enviados.
    Retorno: - retorna o status do evento.
 */
uint8_t TWI::Slave_Transmit(char data){
	uint8_t status;
	TWDR = data;                                            /* Grave dados no TWDR para serem transmitidos */
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWEA);                  /* Ative o TWI e limpe o sinalizador de interrup��o */
	while (!(TWCR & (1<<TWINT)));                           /* Aguarde at� o TWI concluir seu trabalho atual (opera��o de grava��o) */
	status = TWSR & 0xF8;                                   /* Leia o registro de status TWI com mascaramento de tr�s bits inferiores */
	if (status == 0xA0){                                    /* Verifique se STOP/REPEATED START foi recebido */
		TWCR |= (1<<TWINT);                                 /* Se sim, limpe o sinalizador de interrup��o e retorne -1 */
		return -1;
	}
	if (status == 0xB8){                                    /* Verifique se os dados foram transmitidos e a confirma��o ack recebida */
		return 0;                                           /* Se sim, retorne 0 */
	}
    if (status == 0xC0){                                    /* Verifique se os dados foram transmitidos e a confirma��o nack recebida */
		TWCR |= (1<<TWINT);                                 /*  Se sim, limpe o sinalizador de interrup��o e retorne -2 */
		return -2;
	}
	if (status == 0xC8){                                    /* Se o �ltimo byte de dados transmitido com confirma��o recebeu TWEA = 0 */
        return -3;                                          /* Se sim, retorne -3 */
	}else                                                   /* else retorna -4 */
        return -4;
}

/*Receber dados
    - Depois de ser endere�ado pelo mestre com o endere�o SLA + W, o escravo precisa receber dados enviados pelo mestre.
    - Ap�s cada byte recebido, o escravo precisa retornar o reconhecimento sobre o dom�nio.
    - Se START / STOP REPETIDO for recebido, o dispositivo escravo apagar� a bandeira TWINT e ouvir� novamente o barramento.
    - Fun��o Slave_Recieve
    Argumento de entrada: - n�o possui argumento de entrada.
    Retorno: - retorna dados recebidos ou status do evento.
 */
char TWI::Slave_Receive(){
	uint8_t status;                                         /* Declarar vari�vel */
	TWCR=(1<<TWEN)|(1<<TWEA)|(1<<TWINT);                    /* Ativar TWI, gerador de ack e limpe o sinalizador de interrup��o */
	while (!(TWCR & (1<<TWINT)));                           /* Aguarde at� o TWI concluir seu trabalho atual (opera��o de leitura) */
	status = TWSR & 0xF8;                                   /* Leia o registro de status TWI com mascaramento de tr�s bits inferiores */
	if (status == 0x80 || status == 0x90)                   /* Verifique se os dados foram recebidos e ack retornado (TWEA = 1) */
	return TWDR;                                            /* Se sim, retorne os dados recebidos */
	if (status == 0x88 || status == 0x98)                   /* Verifique se os dados foram recebidos, nack retornou e passou para o modo escravo n�o endere�ado */
	return TWDR;                                            /* Se sim, retorne os dados recebidos */
	if (status == 0xA0){                                    /* Verifique se foi recebido STOP/REPEATED START */
		TWCR |= (1<<TWINT);                                 /* Se sim, limpe o sinalizador de interrup��o e retorne 0 */
		return -1;
	}else
        return -2;                                          /* Outro retorno 1 */
}
