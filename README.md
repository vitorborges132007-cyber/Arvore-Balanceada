# Amazon Engineering Challenge — Árvores Balanceadas

## 1. Sobre o projeto

Este projeto implementa duas estruturas de dados de árvores binárias de busca balanceadas em C++: a Árvore AVL e a Árvore Rubro-Negra.

O objetivo é estudar o funcionamento dessas estruturas, implementar suas principais operações e comparar seus comportamentos por meio de testes e medições de desempenho.

As estruturas foram implementadas manualmente, sem utilizar bibliotecas prontas de árvores balanceadas.

## 2. Funcionalidades

O programa oferece as seguintes funcionalidades para ambas as árvores:

- Inserção de valores inteiros.
- Busca de valores.
- Remoção de valores.
- Percurso em ordem crescente.
- Impressão da estrutura da árvore.
- Validação das propriedades de balanceamento.
- Consulta da altura da árvore.
- Contagem acumulada de rotações.
- Testes automatizados e comparação de desempenho.

Na impressão da Árvore AVL, cada nó apresenta sua altura. Na Árvore Rubro-Negra, cada nó apresenta sua cor.

Valores duplicados não são inseridos.

## 3. Tecnologias utilizadas

- Linguagem C++.
- Compilador GCC (g++).
- Terminal Linux ou WSL.
- Visual Studio Code como ambiente de desenvolvimento.

## 4. Como compilar

Abra o terminal na pasta que contém o arquivo `main.cpp` e execute:

```bash
g++ -std=c++11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined -fno-omit-frame-pointer main.cpp -o programa
```

Esse comando compila o programa utilizando o padrão C++11, habilita avisos do compilador e ativa os sanitizadores AddressSanitizer e UndefinedBehaviorSanitizer para ajudar a identificar determinados problemas durante a execução.

## 5. Como executar

Após a compilação, execute:

```bash
./programa
```

O menu principal oferece três opções:

1. Utilizar a Árvore AVL.
2. Utilizar a Árvore Rubro-Negra.
3. Executar os testes automatizados e a comparação.

As opções das árvores permitem inserir, buscar e remover valores, visualizar o percurso em ordem, imprimir a estrutura, validar as propriedades da árvore e consultar a altura e o número acumulado de rotações.

Para sair do programa, selecione a opção `0`.

## 6. Testes automatizados

O programa executa testes com diferentes cenários:

- Árvore vazia.
- Árvore com um elemento.
- Inserção em ordem crescente.
- Inserção em ordem decrescente.
- Inserção em ordem não ordenada.
- Inserção de 1.000 valores em ordem aleatória.

Os testes verificam as operações de inserção, busca e remoção, o tratamento de valores duplicados e inexistentes e a manutenção das propriedades de balanceamento.

O cenário com 1.000 valores também coleta medições de tempo, altura final e número acumulado de rotações para comparar as duas estruturas.

## 7. Comparação entre as estruturas

A Árvore AVL mantém um balanceamento rigoroso, limitando a diferença entre as alturas das subárvores de cada nó. Essa característica favorece alturas reduzidas e pode beneficiar operações de busca.

A Árvore Rubro-Negra utiliza regras de coloração dos nós para manter o balanceamento. Seu mecanismo de correção pode exigir uma quantidade diferente de rotações durante inserções e remoções.

O programa permite observar experimentalmente as diferenças entre as estruturas por meio das seguintes métricas:

- Tempo de inserção.
- Tempo de busca.
- Tempo de remoção.
- Altura final.
- Número acumulado de rotações.

Os resultados de tempo podem variar conforme o computador, o compilador, as opções de compilação e as condições de execução. Por isso, as medições devem ser interpretadas como resultados experimentais, e não como uma prova isolada de superioridade de uma estrutura sobre a outra.

## 8. Considerações finais

O projeto permite estudar e comparar duas implementações clássicas de árvores binárias de busca balanceadas, explorando suas operações fundamentais, suas regras de balanceamento e seu comportamento em diferentes cenários de teste.