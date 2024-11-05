# Trabalho Prático 1: Shell + TOP + Sinais

## Integrantes do Grupo
- **Marco Antônio de Alcântara Machado**  
  - Matrícula: 2022036101
- **Daniel Oliveira Barbosa**  
  - Matrícula: 2020006450

## Descrição do Trabalho

Este projeto consiste na implementação de um shell básico e de uma versão simplificada do comando `top`, explorando conceitos de Processos, Pipes, Estruturas do Kernel e Sinais no sistema operacional Linux.

### Parte 1: Desenvolvimento de um Shell Básico
Na primeira parte, desenvolvemos um shell simplificado que permite a execução de comandos básicos, redirecionamento de entrada e saída, e criação de pipes. 

- **Comandos Simples**: Execução de comandos como `ls`, `cat`, etc.
- **Redirecionamento de Entrada/Saída**: Implementação de redirecionamento para `<` e `>`.
- **Pipes**: Suporte a comandos encadeados com `|`, como `ls | sort | uniq | wc`.

O código referente à Parte 1 está localizado no diretório `./shell`.

### Parte 2: Implementação de um Comando TOP Simplificado
Na segunda parte, implementamos um programa `meutop` que exibe informações sobre os processos do sistema, de maneira semelhante ao comando `top`.

- **Listagem de Processos**: Mostra uma tabela com PID, usuário, nome do processo e estado.
- **Envio de Sinais**: Permite o envio de sinais para processos, através do comando `PID SINAL`.

O código referente à Parte 2 está localizado no diretório `./top`.

### Estrutura do Projeto
- `shell/`: Contém o código do shell básico.
- `top/`: Contém o código do comando `meutop`.
- `assignment_prompt.md`: Descrição detalhada do trabalho.

## Como Compilar, Executar e Testar

### Parte 1 - Shell

#### Como usar o shell:
Para usar o `shell`:

1) Navegue até o diretório `shell`.

2) Execute:
```bash
gcc sh.c -o myshell
./myshell
```

3) Execute os comando comandos padrão do shell unix tais como `ls`, `grep`, `cat`. Você também pode utilizar redirecionamentos (`>` e `<`) e pipes ()`|`) juntamente com esses comandos padrão.

**OBS.: para sair do shell aperte ctrl+D**

#### Como testar o shell
Para testar o shell, navegue até o diretório `shell` e execute:
```bash
./grade.sh
```

### Parte 2 - MeuTop
Para usar o `meutop`:

1) Navegue até o diretório `shell`.

2) Abra um terminal (que chamaremos de terminal 1), compile e execute o `meutop`:
```bash
gcc meutop.c -o meutop
./meutop
```

3) Em outro terminal (que chamaremos de terminal 2), compile e execute o `tester`:
```bash
gcc tester.c -o tester
./tester
```

4) Abra o último terminal (que chamaremos de terminal 3) e execute:
```bash
ps aux | grep tester
```

Isso retornará o PID do `tester` logo após o nome do usuário. Copie esse PID.

5) Vá para o terminal 1 (que está executando o `meutop`) e digite (ou cole) o PID e o sinal que você deseja enviar a esse processo. Você pode fazer o mesmo para qualquer outro processo desde que tenha o seu PID (porém tome cuidado!).

## Observações
- O código foi desenvolvido sem o uso da função `system` para manter a implementação conforme solicitado.
- A Parte 2 utiliza o diretório `/proc` para obter informações dos processos em execução.
- O código foi testado e validado com o script de teste `grade.sh` e com o `signaltester`.