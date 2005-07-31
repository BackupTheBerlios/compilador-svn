%
% Programa de Teste
%

program

%
% Calcula um número elevado ao quadrado
%

function quadrado(integer val) returns integer
declare
	integer temp;
begin
	temp := val * val;
	quadrado := temp;
end;

declare
	integer num;
	real r;
begin
	num := 10;
	r := 1.0
	output quadrado(num);
	output 2.5 * r;
	while num > 0
	loop
		declare
			boolean maior;
    		begin
        		output num;
			maior = num > 5;
			output maior;

			if num > 5
			then
				output 1;
			end_if;

			num := num - 1;
		end;
	end_loop;
end;
