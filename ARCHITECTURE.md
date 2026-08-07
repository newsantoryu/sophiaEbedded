Sophia Architecture Standard

1. API First

2. Interfaces antes das implementações

3. HAL abstrai a plataforma

4. Drivers usam HAL

5. Protocolos independentes

6. Uma responsabilidade por classe

7. Testes antes de integração

8. Commits por sprint

Regra de Ouro

Nenhum módulo da camada de Application ou Services pode incluir arquivos da camada Platform. Toda comunicação com o hardware deve ocorrer exclusivamente por interfaces definidas na HAL. Novas plataformas devem ser adicionadas implementando essas interfaces, sem alterar o código existente.


Ao criar uma classe nova, pergunte:
"Ela descreve um comportamento ou implementa um hardware?"

Se descreve um comportamento → vai para HAL.
Se implementa um hardware específico → vai para Platform.