%
% Programa de Teste
%

program nada;


%
% Calcula um número elevado ao quadrado
%
function quadrado(integer val) returns integer;
declare
    integer temp;
begin
    temp := val*val;
    quadrado := temp;
end;

declare
    integer num;
begin
    num:=10;
    output quadrado(num)=30;   % Esse "igual" é uma comparação
end;
