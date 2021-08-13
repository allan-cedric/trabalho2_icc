#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.interpolate import make_interp_spline

# Relaciona os arquivos das tabelas com os nomes dos seus respectivos campos
filename_to_field = {
    "./Resultados/runtime-gen-curve-matcoef.tsv": "Runtime(ms)",
    "./Resultados/l2missratio-gen-curve-matcoef.tsv": "L2 miss ratio",
    "./Resultados/dpavx-gen-curve-matcoef.tsv": "DP (MFLOP/s)",
    "./Resultados/l3bandwith-gen-curve-matcoef.tsv": "L3 bandwidth (MBytes/s)",

    "./Resultados/runtime-LU-U-1.tsv": "Runtime(ms)",
    "./Resultados/l2missratio-LU-U-1.tsv": "L2 miss ratio",
    "./Resultados/dpavx-LU-U-1.tsv": "DP (MFLOP/s)",
    "./Resultados/l3bandwith-LU-U-1.tsv": "L3 bandwidth (MBytes/s)",

    "./Resultados/runtime-LU-U-2.tsv": "Runtime(ms)",
    "./Resultados/l2missratio-LU-U-2.tsv": "L2 miss ratio",
    "./Resultados/dpavx-LU-U-2.tsv": "DP (MFLOP/s)",
    "./Resultados/l3bandwith-LU-U-2.tsv": "L3 bandwidth (MBytes/s)",

    "./Resultados/runtime-LU-O-1.tsv": "Runtime(ms)",
    "./Resultados/l2missratio-LU-O-1.tsv": "L2 miss ratio",
    "./Resultados/dpavx-LU-O-1.tsv": "DP (MFLOP/s)",
    "./Resultados/l3bandwith-LU-O-1.tsv": "L3 bandwidth (MBytes/s)",

    "./Resultados/runtime-LU-O-2.tsv": "Runtime(ms)",
    "./Resultados/l2missratio-LU-O-2.tsv": "L2 miss ratio",
    "./Resultados/dpavx-LU-O-2.tsv": "DP (MFLOP/s)",
    "./Resultados/l3bandwith-LU-O-2.tsv": "L3 bandwidth (MBytes/s)",
}

# Declara par de tabelas que formam o mesmo gráfico
matching_tables = {
    './Resultados/runtime-LU-U-1.tsv': './Resultados/runtime-LU-O-1.tsv',
    './Resultados/l2missratio-LU-U-1.tsv': './Resultados/l2missratio-LU-O-1.tsv',
    './Resultados/dpavx-LU-U-1.tsv': './Resultados/dpavx-LU-O-1.tsv',
    './Resultados/l3bandwith-LU-U-1.tsv': './Resultados/l3bandwith-LU-O-1.tsv',

    './Resultados/runtime-LU-U-2.tsv': './Resultados/runtime-LU-O-2.tsv',
    './Resultados/l2missratio-LU-U-2.tsv': './Resultados/l2missratio-LU-O-2.tsv',
    './Resultados/dpavx-LU-U-2.tsv': './Resultados/dpavx-LU-O-2.tsv',
    './Resultados/l3bandwith-LU-U-2.tsv': './Resultados/l3bandwith-LU-O-2.tsv'
}

# Tabelas
tables = {}

# Le as tabelas
for filename in filename_to_field:
    tables[filename] = pd.read_csv(filename, delimiter='\t')

# Dados do tamanho (Eixo X)
x_data = tables["./Resultados/runtime-gen-curve-matcoef.tsv"]["n"] # A coluna 'n' de todas as tabelas é igual
x_data = np.array(x_data)
x_data_smooth = np.linspace(x_data.min(), x_data.max(), x_data.max() - x_data.min() + 1)

# Gera gráfico para Fatoracao LU
for first, second in matching_tables.items():
    for table in (first, second): 
        # Valores do eixo Y
        y_data = tables[table][filename_to_field[table]].values
        y_data = np.array(y_data)

        # Spline para suavizar o desenho do gŕafico
        spl = make_interp_spline(x_data, y_data, k=2)
        y_data_smooth = spl(x_data_smooth)

        # Plota os pontos críticos
        plt.scatter(x_data, y_data, s=20)

        # Plota o desenho do gráfico
        label = "Unoptimized" if table == first else "Optimized"
        plt.plot(x_data_smooth, y_data_smooth, label=label)

    # Rótulo do eixo X
    plt.xlabel("Tamanho (n)")

    # Rótulo do eixo Y
    plt.ylabel(filename_to_field[table])

    # Título do gŕafico
    if "1" in first:
        title = "It " + filename_to_field[first] + " LU"
    else:
        title = "Aj " + filename_to_field[first] + " LU"
    plt.title(title)
        
    # Legenda
    plt.legend(fontsize='medium')

    # Salva a gráfico no formato PNG
    plt.savefig("./Resultados/" + title.replace(" ", "-").replace("/", "-") + ".png", bbox_inches="tight")

    # Limpa o plot
    plt.clf()

for table in list(filename_to_field.keys())[:4]:
    # Valores do eixo Y
    y_data = tables[table][filename_to_field[table]].values
    y_data = np.array(y_data)

    # Spline para suavizar o desenho do gŕafico
    spl = make_interp_spline(x_data, y_data, k=2)
    y_data_smooth = spl(x_data_smooth)

    # Plota os pontos críticos
    plt.scatter(x_data, y_data, s=20)

    # Plota o desenho do gráfico
    plt.plot(x_data_smooth, y_data_smooth)

    # Rótulo do eixo X
    plt.xlabel("Tamanho (n)")

    # Rótulo do eixo Y
    plt.ylabel(filename_to_field[table])

    # Título do gŕafico
    title =  filename_to_field[table] + " (gen-curve-matcoef)"
    plt.title(title)

    # Salva a gráfico no formato PNG
    plt.savefig("./Resultados/" + title.replace(" ", "-").replace("/", "-") + ".png", bbox_inches="tight")

    # Limpa o plot
    plt.clf()

# # Gerando um gráfico para cada tabela
# for table in tables:

#     for i in [0, 1]:
#         # Valores do eixo Y
#         y_data = tables[table][filename_to_field[table][i]].values
#         y_data = np.array(y_data)

#         # Spline para suavizar o desenho do gŕafico
#         spl = make_interp_spline(x_data, y_data, k=2)
#         y_data_smooth = spl(x_data_smooth)

#         # Plota os pontos críticos
#         plt.scatter(x_data, y_data, s=20)

#         # Plota o desenho do gráfico
#         label = "Unoptimized" if i == 0 else "Optimized"
#         plt.plot(x_data_smooth, y_data_smooth, label=label)

#     # Rótulo do eixo X
#     plt.xlabel("Tamanho (n)")

#     # Rótulo do eixo Y
#     plt.ylabel(" ".join(filename_to_field[table][0].split()[:-2]))

#     # Título do gŕafico
#     if "row-vet" in table:
#         plt.title(filename_to_field[table][0].split()[0] + " (row-vet)")
#     else:
#         plt.title(filename_to_field[table][0].split()[0] + " (mat-row)")


#     # Legenda
#     plt.legend(fontsize='medium')

#     # Salva a gráfico no formato PNG
#     plt.savefig(table.replace(".tsv", ".png"), bbox_inches="tight")

#     # Limpa o plot
#     plt.clf()