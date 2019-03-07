#include<stdio.h>
#define n 3 //Espacio vectorial de R^n
#include<math.h>
void main()
{
	int opc, i,  seleccion_vector;
	float modulo1=0, modulo2=0,modulo3=0,angulo, v1[n], v2[n], v3[n], producto_escalar = 0, escalar = 0, mayor1 = 0, mayor2 = 0,mayor3=0;
	
	for (i = 0; i < n; i++) //Al inicio v3 debe ser 0
		v3[i] = 0;

	printf("\t\tCALCULADORA VECTORIAL\n\nIntroduzca los 2 vectores con los que desee operar:\n");

	for (i = 0; i < n; i++) //Introducir las componentes de los vectores
	{
		printf("\nComponente %d del vector v1: ", i);
		scanf_s("%f", &v1[i]);
		printf("\nComponente %d del vector v2: ", i);
		scanf_s("%f", &v2[i]);
	}
	system("CLS");
	do //menu de la calculadora, se repite hasta que indiques la opcion para salir
	{
		printf("\t\tCALCULADORA VECTORIAL\n\n");
		printf("Vectores a operar:\n");
		printf("\nv1= "); //Imprime el vector 1
		for (i = 0; i < n; i++)
			printf("%f ", v1[i]);

		printf("\nv2= "); //Imprime el vector 2
		for (i = 0; i < n; i++)
			printf("%f ", v2[i]);
		printf("\n\nvector resultante anterior (por defecto= 0):");
		printf("\nv3= "); //Imprime el vector 3
		for (i = 0; i < n; i++)
			printf("%f ", v3[i]);

		printf("\n\nSeleccione la accion que desee realizar:\n1.Suma de vectores\n2.Resta de vectores\n3.Producto escalar\n4.Escalado positivo del vector\n5.Norma euclidea\n6.Norma infinita\n7.Rotacion alrededor del eje x\n\n8.Salir\n\n");
		do //Introduce la opcion
		{
			scanf_s("%d", &opc);
			if (opc < 1 || opc>8)
				printf("OPCION INCORRECTA!!\n");
		} while (opc < 1 && opc>8); //No pasa hasta que no eliges opcion correcta

		if (opc == 8)
			printf("\nHasta luego!\n");
		else
		{
			switch (opc) //Opciones
			{
			case 1: //Suma
			{
				printf("v1+v2= ");
				for (i = 0; i < n; i++)
				{
					v3[i] = v1[i] + v2[i];
					printf("%f ", v3[i]);
				}
				printf("\n");
				break;
			}
			case 2: //Resta
			{
				printf("v1-v2= ");
				for (i = 0; i < n; i++)
				{
					v3[i] = v1[i] - v2[i];
					printf("%f ", v3[i]);
				}
				printf("\n");
				break;
			}
			case 3: //Producto escalar
			{
				do {
					printf("Seleccione una opcion:\n1.v1*v2\n2.v1*v3\n3.v2*v3\n");
					scanf_s("%d", &seleccion_vector);
				} while (seleccion_vector<1 || seleccion_vector > 3);
				if (seleccion_vector == 1)
				{
					for (i = 0; i < n; i++)
					{
						producto_escalar = producto_escalar+v1[i] * v2[i];
					}
				}
				else if(seleccion_vector==2)
				{
					for (i = 0; i < n; i++)
					{
						producto_escalar = producto_escalar+ v1[i] * v3[i];
					}
				}
				else if(seleccion_vector==3)
				{
					for (i = 0; i < n; i++)
					{
						producto_escalar = producto_escalar+ v2[i] * v3[i];
					}
				}
					printf("El producto escalar es= %f\n", producto_escalar);
				break;
			}
			case 4: //Escalar positivo
			{
				do {
				printf("Seleccione el vector que desea multiplicar:\n1.v1\n2.v2\n3.v3\n");
				scanf_s("%d", &seleccion_vector);
			} while (seleccion_vector < 1 || seleccion_vector > 3);
				if (seleccion_vector == 1)
				{
					printf("Introduzca el escalar por el que lo quiere multiplicar: ");
					scanf_s("%f", &escalar);
					for (i = 0; i < n; i++)
						v1[i] = v1[i] * escalar;
					printf("\nv1= ");
					for (i = 0; i < n; i++)
						printf("%f ", v1[i]);
				}
				else if (seleccion_vector == 2)
				{
					printf("Introduzca el escalar por el que lo quiere multiplicar: ");
					scanf_s("%f", &escalar);
					for (i = 0; i < n; i++)
						v2[i] = v2[i] * escalar;
					printf("\nv2= ");
					for (i = 0; i < n; i++)
						printf("%f ", v2[i]);
				}
				else if (seleccion_vector == 3)
				{
					printf("Introduzca el escalar por el que lo quiere multiplicar: ");
					scanf_s("%f", &escalar);
					for (i = 0; i < n; i++)
						v3[i] = v3[i] * escalar;
					printf("\nv3= ");
					for (i = 0; i < n; i++)
						printf("%f ", v3[i]);
				}
				break;
			}
			case 5: //Norma euclidea
			{
				for (i = 0; i < n; i++)
				{
					modulo1 =(double)modulo1+ v1[i] * v1[i];
					modulo2 =(double)modulo2+ v2[i] * v2[i];
					modulo3 =(double)modulo3+ v3[i] * v3[i];
				}
				modulo1 = sqrt(modulo1);
				modulo2 = sqrt(modulo2);
				modulo3 = sqrt(modulo3);
				printf("El modulo de v1= %.3f\nEl modulo de v2=%.3f\nEl modulo de v3=%.3f\n", modulo1, modulo2,modulo3);
				break;
			}
			case 6: //Norma infinita
			{
			mayor1 =abs( v1[0]);
			mayor2 = abs(v2[0]);
			mayor3 = abs(v3[0]);
				for (i = 1; i < n; i++)
				{
					if (v1[i] >= abs(mayor1))
						mayor1 =abs( v1[i]);
					if (v2[i] >= abs(mayor2))
						mayor2 = abs(v2[i]);
					if (v3[i] >= abs(mayor3))
						mayor3 = abs(v3[i]);
				}
				printf("El mayor componente de v1 es: %f\nEl mayor componente de v2 es: %f\nEl mayor componente de v3 es: %f\n", mayor1, mayor2,mayor3);
				break;
			}
			case 7: //Rotacion alrededor del eje x
			{
				do {
					printf("Seleccione el vector que desea rotar:\n1.v1\n2.v2\n");
					scanf_s("%d", &seleccion_vector);
				} while(seleccion_vector != 1 && seleccion_vector != 2);
				printf("Introduzca en grados el angulo que desea rotar el vector\n");
				scanf_s("%f", &angulo);
				angulo = angulo * ((acos(-1)) / 180);
				if (n == 3)
				{
					switch (seleccion_vector)
					{
					case 1:
					{
						v3[0] = v1[0];
						v3[1] = v1[1] * cos(angulo) - v1[2] * sin(angulo);
						v3[2] = v1[2] * cos(angulo) + v1[1] * sin(angulo);
						printf("\nv3= ");
						for (i = 0; i < n; i++)
							printf("%f ", v3[i]);
						break;
					}
					case 2:
					{
						v3[0] = v2[0];
						v3[1] = v2[1] * cos(angulo) - v2[2] * sin(angulo);
						v3[2] = v2[2] * cos(angulo) + v2[1] * sin(angulo);
						printf("\nv3= ");
						for (i = 0; i < n; i++)
							printf("%f ", v3[i]);
						break;
					}
					}
				}
				else if (n == 2)
					switch (seleccion_vector)
					{
					case 1:
					{
						v3[0] = v1[0] * cos(angulo) - v1[1] * sin(angulo);
						v3[1] = v1[0] * sin(angulo) + v1[1] * cos(angulo);
						printf("\nv1= ");
						for (i = 0; i < n; i++)
							printf("%f ", v3[i]);
						break;
					}
					case 2:
					{
						v3[0] = v2[0] * cos(angulo) - v2[1] * sin(angulo);
						v3[1] = v2[0] * sin(angulo) + v2[1] * cos(angulo);
						printf("\nv2= ");
						for (i = 0; i < n; i++)
							printf("%f ", v3[i]);
						break;
					}
					}
				else
					printf("El vector no puede ser representado en un espacio de R^3\n");
			}
			}
			system("PAUSE");
			system("CLS");
		}
	} while (opc != 8);
	system("PAUSE");
}