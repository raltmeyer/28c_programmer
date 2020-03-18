# 28c_Programmer

2020 - Rogerio Altmeyer

[English Version](https://github.com/raltmeyer/28c_programmer/README.md)

## Descrição Básica

Esse gravador de EEPROMs nasceu para dar suporte ao desenvolvimento de outro projeto de retro-computing em que estou trabalhando, onde o foco é recriar computadores da década de 80 do zero de forma modular e que seja possível rodar binários originais da época.

O gravador foi criado com base na EEPROM AT28C256 de 32Kb da ATMel, e até o momento o firmware é compativel apenas com esse chip, podendo ser facilmente convertido para outras memórias paralelas.


## O que o projeto contém

O projeto é dividido em três partes:
- Projeto esquemático do circuito e desenho da placa PCB (Hardware).
- Firmware que controla a placa (C++)
- Cliente que roda na máquina host responsável pela comunicação com a placa (Python)

## Placa de circuito

A placa usa o Arduino Nano como componente principal, responsavel pela comunicação com o computador host e a programação do chip de memória.

As linhas de controle e o bus de dados estão conectados diretamente ao Nano, e as 15 linhas de endereçamento do chip são alimentadas através de dois shift registers de 8 bits ligados em sequência.

Esquemático do Circuito da Placa V1.3
![Esquemático do Circuito da Placa V1.3](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_board/Schematic_eeprom28_programmer.png)

O projeto da placa está disponível em [ZIP com Gerber da Placa](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_board/Gerber_eeprom28_programmer.zip). Utilizei os serviços de confecção da [JLCPCB.com](https://jlcpcb.com) e recomendo.

## Firmware da placa

O firmware da placa foi desenvolvido em C++ no Eclipse IDE, e traz como característica principal a implementação de interpretador de mensagens vindas do computador host. Com o interpretador de mensagens, foi possível organizar e facilitar a codificação de novas funcionalidades ao firmware.

### Núcledo o Firmware
- [28c_programmer.h](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/28c_programmer.h) Header principal do firmware
- [pin_defs.h](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/pin_defs.h) Header com a definição de pinos
- [main.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/main.cpp) Fonte principal contendo as funções setup() e loop()
- [interface.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/interface.cpp) Contém todas as funções responsáveis pela interface entre Arduino e EEPROM
- [util.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/util.cpp) Funções diversas de apoio ao código
- [cmd_processor.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_processor.cpp) Implementa o interpretador de mensagens com o host

### Funções para uso pelo host
- [cmd_clear_block.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_clear_block.cpp) Função para limpar EEPROM
- [cmd_get_version.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_get_version.cpp) Função que retorna a versão do firmware da placa
- [cmd_read_block.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_read_block.cpp) Função que lê um bloco de 16 bytes e retorna para o host
- [cmd_read_byte.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_read_byte.cpp) Função que lê 1 byte
- [cmd_write_block.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_write_block.cpp) Função que grava de 1 a 16 bytes na EEPROM

## Cliente de linha de comando para o computador host

Escrito em Python3, essa camada do projeto é responsável por ler/escrever arquivos na máquina host e enviar/receber para o firmware da placa.

### Instalação das dependências (macOS / Linux)

Para rodar o cliente, é preciso ter `python3` e instalar as seguintes dependências:
```
pip3 install argparse
pip3 install pyserial
```

Também será necessário ajustar as permissões:
```
chmod +x programmer.py
```

### Parâmetros suportados
O cliente irá mostrar o seguinte help abaixo indicando os parâmetros suportados:
```
#./programmer.py
usage: programmer.py [-h] [--file FILE] [--address ADDRESS_BEGIN]
                     [--address-end ADDRESS_END]
                     serial {write,read,clear}
programmer.py: error: the following arguments are required: serial, mode
```

Parâmetros obrigatórios:
- porta serial
- modo de operação
	- **write**
	- **read**
	- **clear**

Parâmetros opcionais:
- **--address** Especifica o endereço inicial da operação
- **--address-end** Especifica o endereço final da operação
- **--file** Obrigatório para os modos **write** e **read**, especifica o arquivo a ser lido/gravado


### Exemplos de uso

Gravar o arquivo boot.hex na posição 0x0000
```
./programmer.py /dev/porta_serial write --file boot.hex 
```

Gravar o arquivo dado.hex na posição 0x1E00
```
./programmer.py /dev/porta_serial write --file dado.hex --address 0x1e00
```

Ler toda a EEPROM para o arquivo dump.bin
```
./programmer.py /dev/porta_serial read --file dump.bin 
```

Ler da EEPROM do endereço 0x1E00 até 0x2A00 para o arquivo dump.bin
```
./programmer.py /dev/porta_serial read --file dump.bin --address 0x1e00 --address-end 0x2a00
```

Limpar toda a EEPROM (irá gravar 0xFF em todas as posições de memória)
```
./programmer.py /dev/porta_serial clear
```

Limpar um bloco da EEPROM
```
./programmer.py /dev/porta_serial clear --address 0x1e00 --address-end 0x2a00
```


## Detalhes da Placa

Placa Versão 1.3 em 3D
![3D da Placa V1.3](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_board/PCB_3D_eeprom28_programmer.png)

Placa Versão 1.3
![Placa V1.3](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_board/PCB_eeprom28_programmer.png)

Placa Versão 1.1 montada
![Placa V1.1 Montada](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_board/img_board_rev1.png)

