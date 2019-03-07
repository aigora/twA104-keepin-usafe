%Pr�ctica sesi�n0 de Matlab
%Autor: David Hergueta Soto N�Exp.54666

clear all

x=-0.5:0.01:1;
y=10-0.15*x+0.5*x.^2 +0.45*x.^3-1.8*x.^4+x.^5+5*10^-3*(.5-rand(size(x)));
%Representaci�n de la nube de puntos
plot(x,y,'.')
hold on

%Obtenci�n de los coeficientes del ajuste polin�mico de grado 5
disp('coeficientes del polinomio de ajuste de grado 5');
c=polyfit(x,y,5)
%funci�n de ajuste de grado 5
func=c(1,1)*x.^5+c(1,2)*x.^4+c(1,3)*x.^3+c(1,4)*x.^2+c(1,5)*x+c(1,6);
plot(x,func,'-r')

%derivada
derivada=diff(func)/0.01;
%valores m�ximos y m�nimos que alcanza en el intervalo
max=max(derivada(50:end));
min=min(derivada(50:end));
fprintf('La derivada de la funcion en el intervalo [0,1] est� comprendida entre [%.3d,%.3d]\n',min,max);

%integral definida
integral=sum(func*0.01+(func+0.01)*0.01)/2
fprintf('La integral de la funci�n en [0,1] vale %.3d\n',integral);